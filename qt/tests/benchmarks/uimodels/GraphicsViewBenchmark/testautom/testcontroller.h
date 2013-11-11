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

#ifndef TESTCONTROLLER_H
#define TESTCONTROLLER_H

#include <QObject>
#include <QTime>

#include "resultlogger.h"
#include "dummydatagen.h"
#include "resourcemoninterface.h"
#include "theme.h"

class QMutex;
class QWaitCondition;
class MainView;
class ItemRecyclingList;
class SimpleList;
class RecycledListItem;
class ListItem;

class TestController : public QObject
{
    Q_OBJECT
public:

    TestController(const QString &fname, MainView *mMainView, ResultLogger::ResultFormat format, QObject *parent=0);
    virtual ~TestController();

    QString resultFileName();
    ResultLogger::ResultFormat resultFileFormat();

public slots:
    // Test case methods.
    void fillList(int itemCount, Benchmark::ListType listType, TestFunctionResult *testFunctionResult, const QString &tag);
    void deleteList(TestFunctionResult *testFunctionResult, const QString &tag);
    void themeChange(int theme, TestFunctionResult *testFunctionResult, const QString &tag);
    void scrollList(int maxTimeMs, TestFunctionResult *testFunctionResult, const QString &tag);
    void forceUpdate(int maxTimeMs, TestFunctionResult *testFunctionResult, const QString &tag);
    void addToBeginningOfList(int itemCount, TestFunctionResult *testFunctionResult, const QString &tag);
    void removeFromBeginningOfList(int itemCount, TestFunctionResult *testFunctionResult, const QString &tag);

    // Test configuration methods.
    void applicationExit();
    void changeTheme(Theme::Themes theme);
    void setImageBasedRendering(const bool imageBasedRendering);
    bool imageBasedRendering() const;
    void setSubtreeCache(const bool enabled);
    bool subtreeCache() const;
    void setScrollStep(const int step);
    int scrollStep() const;
    void setCpuMemLogging(const bool enabled);
    void rotateMainWindow(const int angle);
    int mainWindowRotationAngle() const;
    void setTwoColumns(const bool twoCols);
    bool twoColumns() const;

signals:
    void stop();
    void testDone();
    void doScrollRecyclingList(TestFunctionResult *testFunctionResult, const QString &tag);
    void doScrollSimpleList(TestFunctionResult *testFunctionResult, const QString &tag);
    void doForceUpdate(TestFunctionResult *testFunctionResult, const QString &tag);

private slots:
    void scrollRecyclingList(TestFunctionResult *testFunctionResult, const QString &tag);
    void scrollSimpleList(TestFunctionResult *testFunctionResult, const QString &tag);
    void doForceUpdateViewport(TestFunctionResult *testFunctionResult, const QString &tag);

    void aboutToQuit();
    void testIsDone();

    void printResourceLoad();

private:
    Q_DISABLE_COPY(TestController)

    bool loadPlugin();
    void createCpuBenchmark(TestFunctionResult *testFunctionResult, const QString &tag, int listItems,  Benchmark::ListType type, const QString &theme, ResourceMonitorInterface::CpuUsage cpuUsage);
    void createMemoryBenchmark(TestFunctionResult *testFunctionResult, const QString &tag, int listItems,  Benchmark::ListType type, const QString &theme, ResourceMonitorInterface::MemoryAllocation allocation);
    
    MainView *mMainView;

    SimpleList *mCurrentSimpleList;
    ItemRecyclingList *mCurrentRecyclingList;
    QTime mTestDuration;
    int mMaxTestDurationTime;
    int mScrollDirection;
    QString mFileName;
    ResultLogger::ResultFormat mResultFormat;
    DummyDataGenerator mDataGenerator;
    ResourceMonitorInterface* mResMon;
    bool mSubtreeCacheEnabled;
    int mScrollStep;
    bool mCpuMemLogging;
    int mLastElapsed;
};

#endif // TESTCONTROLLER_H
