/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in the either Technology Preview License Agreement or the
** Beta Release License Agreement.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at http://qt.nokia.com/contact.
** $QT_END_LICENSE$
**
****************************************************************************/

// List types to test.
var listTypeSimple = 0;
var listTypeRecycling = 1;

function Tests(tester)
{
    this.tester = tester;

    //Execute tests.
    this.tests1();

    //Calculate the metrics from the measured results for test1
    this.metrics1();

    //this.tests_rotation();
    // ADD ANY NEW TEST CASES AND METRICS BELOW THIS
}

/*
  ===================
  VERY IMPORTANT NOTE
  ===================

  THIS SCRIPT MUST ***NEVER*** BE CHANGED/REPLACED WHEN
  RUNNING AUTOMATED PERFORMACE TESTS INTENDED FOR TREND
  TRACKING.

  TO ADD NEW TEST CASES, CREATE NEW TEST FUNCTIONS RUN
  AFTER THESE ONES
*/
Tests.prototype.tests1 = function() {

    var slowSpeed = 2
    var normalSpeed = 8
    var fastSpeed = 64

    var maxScrollTime = 11000;
    var maxUpdateTime = 11000;
    var addAndRemoveItemsCount = 100;

    // Sizes of list that test are run.
    // We repeat small list again
    var sizes= new Array();
    sizes[0] = 50; // Small number of data/items
    sizes[1] = 500; // Large number of data/items
    sizes[2] = 50; // Small number of data/items again for regression

    var themeCount = this.tester.themeCount()-1;

    var logger = this.tester.resultLogger();

    var fillListResult = logger.createTestFunctionResult("fillListTest");
    var addToBeginningResult = logger.createTestFunctionResult("addToBeginningOfListTest");
    var removeFromResult = logger.createTestFunctionResult("removeFromBeginningOfListTest");
    var deleteListResult = logger.createTestFunctionResult("deleteListTest");
    var renderListResult = logger.createTestFunctionResult("renderListTest");
    var themeChangeResult = logger.createTestFunctionResult("themeChangeTest");

    var originalThemeId = this.tester.currentThemeId();

    //Empty additional tag
    var regression = "";

    // Test all list sizes.    
    for (var listSize = 0; listSize<sizes.length; listSize=listSize+1) {

    //If second 50 item list is being tested, append Regression string to the test tag.
    if(listSize == 2)
        {
        regression += String(" Regression");
        }
        
        // Test all list types
        for (var listType = listTypeSimple; listType<=listTypeRecycling; listType=listType+1) {

            this.tester.fillListTest(sizes[listSize], listType, fillListResult, "Fill List" + regression);

            this.tester.addToBeginningOfListTest(addAndRemoveItemsCount, addToBeginningResult, "Add items to beginning of list." + regression);

            this.tester.removeFromBeginningOfListTest(addAndRemoveItemsCount, removeFromResult, "Remove items from begining of list." + regression);

            // Test all themes - Blue=Simple, Lime=Complex
            for (var themeId = 0; themeId<=themeCount; themeId=themeId+1) {
                this.tester.setTheme(themeId);

                // 12 CORE SCROLL TESTS (per theme, per algorithm)

                // Render to screen, Uncached, 3 scroll speed + forced update
                this.tester.setImageBasedRendering(false);
                this.tester.setSubtreeCache(false);
                this.tester.setScrollStep(slowSpeed);
                this.tester.scrollListTest(maxScrollTime, renderListResult, "Scroll Slow" + regression);
                this.tester.setScrollStep(normalSpeed);
                this.tester.scrollListTest(maxScrollTime, renderListResult, "Scroll Normal" + regression);
                this.tester.setScrollStep(fastSpeed);
                this.tester.scrollListTest(maxScrollTime, renderListResult, "Scroll Fast" + regression);
                this.tester.forceUpdateTest(maxUpdateTime, renderListResult, "Force Update" + regression);

                // Render to screen, Cached, 3 scroll speeds + forced udpate
                this.tester.setImageBasedRendering(false);
                this.tester.setSubtreeCache(true);
                this.tester.setScrollStep(slowSpeed);
                this.tester.scrollListTest(maxScrollTime, renderListResult, "Scroll Slow" + regression);
                this.tester.setScrollStep(normalSpeed);
                this.tester.scrollListTest(maxScrollTime, renderListResult, "Scroll Normal" + regression);
                this.tester.setScrollStep(fastSpeed);
                this.tester.scrollListTest(maxScrollTime, renderListResult, "Scroll Fast" + regression);
                this.tester.forceUpdateTest(maxUpdateTime, renderListResult, "Force Update" + regression);

                // Render to screen rotated 90degrees, Uncached+Cached, Medium scroll speed
                this.tester.setImageBasedRendering(false);
                this.tester.setTwoColumns(true); // Same amount items on screen that it's in portrait mode
                this.tester.rotateMainWindow(90);
                this.tester.setScrollStep(normalSpeed);
                this.tester.setSubtreeCache(false);
                this.tester.scrollListTest(maxScrollTime, renderListResult, "Scroll Normal Rotated" + regression);
                this.tester.setSubtreeCache(true);
                this.tester.scrollListTest(maxScrollTime, renderListResult, "Scroll Normal Rotated" + regression);
                this.tester.rotateMainWindow(-90); // REVERT rotation
                this.tester.setTwoColumns(false); // Reduce rows when moving back to portrait mode

                // Render to pixmap, Uncached+Cached, Medium scroll speed
                this.tester.setImageBasedRendering(true);
                this.tester.setScrollStep(normalSpeed);
                this.tester.setSubtreeCache(false);
                this.tester.scrollListTest(maxScrollTime, renderListResult, "Scroll Normal" + regression);
                this.tester.setSubtreeCache(true);
                this.tester.scrollListTest(maxScrollTime, renderListResult, "Scroll Normal" + regression);
            }

            this.tester.setTheme(originalThemeId);

            // These test are run only with different list sizes and types.
            for (var tId = themeCount; tId>=0; tId=tId-1) {
                var tag ="Change theme. From " + this.tester.currentThemeName() + " to " + this.tester.themeName(tId);
                this.tester.themeChangeTest(tId, themeChangeResult, tag + regression);
            }

            this.tester.setTheme(originalThemeId);

            this.tester.deleteListTest(deleteListResult, "Delete list." + regression);
        }
    }
};

String.prototype.startsWith = function(str) {
    return (this.match("^"+str)==str);
}

Tests.prototype.metrics1 = function() {

    var addAndRemoveItemsCount = 100; // Should be same as above

    var resultset,i,result,target,points,metricmax,pixelcount;
    var logger = this.tester.resultLogger();

    var metric1 = logger.createTestFunctionResult("GraphicsView.PerformanceMetrics.1");
    // Later metrics added should use PerformanceMetrics.2, etc.

    //print(startupresult.benchmarks().at(0));

    // The metrics computed below are meant to synthesise
    // a single number from a set of related tests
    // The idea is that improvements in those areas should
    // show a boost in the relevant metric
    // Larger result is always better
    // 100 is target value but can be exceeded
    metricmax = 100.0;
    points = 0.0; // Add one for each data point to scale later

    response = metric1.createBenchmark();
    response.setTag("Response");
    var accum = 0.0;
    var tag;

    // Get the startup result
    // This only applies AFTER the main was called
    // Target of 100 milliseconds
    // If we meet them then this will contribute 1.0
    // Faster will be higher than one
    target = 100.0; // ms
    resultset = logger.getTestFunctionResult("Startup").benchmarks();
    //print("Startup count:",resultset.count());
    result = resultset.at(0);
    accum += target/(result.value()+0.00000001); // +0.0000001 to make sure we never divide by zero
    points += 1.0;
    //print(accum);

    // Target is 10ms setup + 0.25ms per item for all list types
    resultset = logger.getTestFunctionResult("fillListTest").benchmarks();
    i = 0;
    target = 0.25;
    for (;i<resultset.count();i++) {
        result = resultset.at(i);
        tag = result.tag();
        if (tag.startsWith("CPU")) continue;
        if (tag.startsWith("Memory")) continue;
        //print(tag,"fill: "+result.value(),result.listSize());
        accum += ((10.0 + (target*result.listSize()))/result.value());
        //print(accum);
        points += 1.0;
    }

    // Target is 50ms for 640x360 screen
    resultset = logger.getTestFunctionResult("themeChangeTest").benchmarks();
    i = 0;
    target = (640.0*360.0)/50.0; // target is per-pixel
    for (;i<resultset.count();i++) {
        result = resultset.at(i);
        tag = result.tag();
        if (tag.startsWith("CPU")) continue;
        if (tag.startsWith("Memory")) continue;
        pixelcount = result.width()*result.height();
        //print(tag,result.value(),((target*pixelcount)/result.value()));
        accum += ((target*pixelcount)/result.value());
        //print(accum);
        points += 1.0;
    }

    // addAndRemoveItemsCount items, target is 0.25ms per item
    resultset = logger.getTestFunctionResult("addToBeginningOfListTest").benchmarks();
    i = 0;
    target = 0.25*addAndRemoveItemsCount;
    for (;i<resultset.count();i++) {
        result = resultset.at(i);
        tag = result.tag();
        if (tag.startsWith("CPU")) continue;
        if (tag.startsWith("Memory")) continue;
        //print(tag,"add: "+result.value(),result.listSize(),result);
        accum += ((target)/result.value());
        //print(accum);
        points += 1.0;
    }

    // addAndRemoveItemsCount items, target is 0.25ms per item
    resultset = logger.getTestFunctionResult("removeFromBeginningOfListTest").benchmarks();
    i = 0;
    target = 0.25*addAndRemoveItemsCount;
    for (;i<resultset.count();i++) {
        result = resultset.at(i);
        tag = result.tag();
        if (tag.startsWith("CPU")) continue;
        if (tag.startsWith("Memory")) continue;
        //print(tag,"remove: "+result.value(),result.listSize(),result);
        accum += ((target)/result.value());
        //print(accum);
        points += 1.0;
    }

    // Target is 10ms general tear down + 0.1ms per item for all list types
    resultset = logger.getTestFunctionResult("deleteListTest").benchmarks();
    i = 0;
    target = 0.1;
    for (;i<resultset.count();i++) {
        result = resultset.at(i);
        tag = result.tag();
        if (tag.startsWith("CPU")) continue;
        if (tag.startsWith("Memory")) continue;
        //print(tag,"delete: "+result.value(),result.listSize());
        accum += ((10.0+(target*result.listSize()))/result.value());
        //print(accum);
        points += 1.0;
    }

    accum *= metricmax / points;
    //print(accum);
    response.setValue(accum);

    resourceusage = metric1.createBenchmark()
    resourceusage.setTag("ResourceUsage");
    resourceusage.setValue("0.5");

    rendering = metric1.createBenchmark();
    rendering.setTag("Rendering");
    points = 0.0; // Add one for each data point to scale later
    accum = 0.0;
    // Target is 60FPS in all cases
    resultset = logger.getTestFunctionResult("renderListTest").benchmarks();
    i = 0;
    target = 60.0;
    for (;i<resultset.count();i++) {
        result = resultset.at(i);
        tag = result.tag();
        if (tag.startsWith("CPU")) continue;
        if (tag.startsWith("Memory")) continue;
        //print("r: "+result.value(),result.value()/target);
        //print(result.benchmarkStr());
        accum += result.value()/target;
        //print(accum);
        points += 1.0;
    }
    if (points>0.0) {
        accum *= metricmax / points;
    }
    //print("Final render:",accum);
    rendering.setValue(accum);

    datascaling = metric1.createBenchmark()
    datascaling.setTag("DataScaling");
    datascaling.setValue("0.5");

    smoothness = metric1.createBenchmark()
    smoothness.setTag("Smoothness");
    smoothness.setValue("0.5");
}

Tests.prototype.tests_rotation = function() {

    var maxScrollTime = 11000;

    var logger = this.tester.resultLogger();

    var fillListResult = logger.createTestFunctionResult("Rotate. fillListTest");
    var scrollListResult = logger.createTestFunctionResult("Rotate. scrollListTest");
    var deleteListResult = logger.createTestFunctionResult("Rotate. deleteListTest");
    this.tester.setSubtreeCache(false);
    this.tester.setImageBasedRendering(false);

    for (var listType = listTypeSimple; listType<=listTypeRecycling; listType=listType+1) {
        this.tester.fillListTest(200, listType, fillListResult, "Creating and filling list.");
        this.tester.scrollListTest(maxScrollTime, scrollListResult, "Scroll list.");
        this.tester.rotateMainWindow(90);
        this.tester.setTwoColumns(true); // Same amount items on screen that it's in portrait mode
        this.tester.scrollListTest(maxScrollTime, scrollListResult, "Scroll rotated list.");
        this.tester.deleteListTest(deleteListResult, "Delete list.");
        this.tester.rotateMainWindow(-90);
        this.tester.setTwoColumns(false); // Reduce rows when moving back to portrait mode
    }
}

