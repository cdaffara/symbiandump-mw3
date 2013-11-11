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

#include <QDebug>
#include <QMutex>
#include <QWaitCondition>
#include <QApplication>
#include <QDir>
#include <QPluginLoader>

#include "testcontroller.h"
#include "itemrecyclinglist.h"
#include "simplelist.h"
#include "scrollbar.h"
#include "mainview.h"
#include "theme.h"
#include "themeevent.h"
#include "filllists.h"

static const int EmitDoneTimeout = 1000;

Q_DECLARE_METATYPE(TestController*)

TestController::TestController(const QString &fname, MainView *mv, ResultLogger::ResultFormat format, QObject *parent)
    : QObject(parent),
    mMainView(mv),
    mCurrentSimpleList(0),
    mCurrentRecyclingList(0),
    mTestDuration(),
    mMaxTestDurationTime(0),
    mScrollDirection(1),
    mFileName(fname),
    mResultFormat(format),
    mDataGenerator(),
    mResMon(0),
    mSubtreeCacheEnabled(false),
    mScrollStep(36),
    mCpuMemLogging(true),
    mLastElapsed(0)
{
    qRegisterMetaType<TestController*>();    
    connect(QApplication::instance(), SIGNAL(aboutToQuit()), this, SLOT(aboutToQuit()));

    if(loadPlugin()) {
        //qDebug() << "Loaded plugin!";
        if (mResMon->Prepare("tst_GraphicsViewBenchmark")) {
        }
    }
    else {
        qDebug() << "Failed to load plugin!";
    }
}


TestController::~TestController()
{
    delete mResMon;
}

void TestController::printResourceLoad()
{
#if defined Q_OS_LINUX
    if (mResMon) {
        ResourceMonitorInterface::CpuUsage usage = mResMon->CPULoad();
        ResourceMonitorInterface::MemoryAllocation allocation = mResMon->MemoryLoad();
        qDebug() << "CPULoad: total" << usage.systemUsage << "(%), app" << usage.appTreadUsage << "(%)";
        qDebug() << "MemoryLoad: occupied by app chunks" << allocation.allocatedInAppThread << "(bytes), number of free app chunks" << allocation.numberOfAllocatedCellsInAppThread << ", avaliable in app heap" << allocation.availableMemoryInAppThreadHeap <<"(bytes), total memory" << allocation.totalMemoryInSystem << "(bytes), free memory" << allocation.availableMemoryInSystem << "(bytes)";
    }
#endif
}

QString TestController::resultFileName()
{
    return mFileName;
}

ResultLogger::ResultFormat TestController::resultFileFormat()
{
    return mResultFormat;
}

void TestController::setSubtreeCache(const bool enabled)
{
    mSubtreeCacheEnabled = enabled;
}
bool TestController::subtreeCache() const
{
    return mSubtreeCacheEnabled;
}

void TestController::setScrollStep(const int step)
{
    mScrollStep = step;
}
int TestController::scrollStep() const
{
    return mScrollStep;
}

void TestController::rotateMainWindow(const int angle)
{
    if (mMainView)
        mMainView->rotateContent(angle);
    QTimer::singleShot(EmitDoneTimeout, this, SLOT(testIsDone()));
}

int TestController::mainWindowRotationAngle() const
{
    if (mMainView)
        return mMainView->rotationAngle();
    return 0;
}

void TestController::setCpuMemLogging(const bool enabled)
{
    mCpuMemLogging = enabled;
}

void TestController::aboutToQuit()
{
    testIsDone();
    emit stop();
}

void TestController::changeTheme(Theme::Themes theme)
{
    Theme::p()->setTheme(theme);
}

void TestController::testIsDone()
{
    mScrollDirection = 1;    
    mCurrentRecyclingList = 0;
    mCurrentSimpleList = 0;
    mMaxTestDurationTime = 0;
    if (mMainView)
        mMainView->fpsReset();

    emit testDone();
}

void TestController::fillList(int itemCount, Benchmark::ListType listType,
                              TestFunctionResult *testFunctionResult,  const QString &tag)
{
    if (!mMainView) {
        QTimer::singleShot(EmitDoneTimeout, this, SLOT(testIsDone()));
        return;
    }

    Benchmark *b = testFunctionResult->createBenchmark();
    b->setRotation(mainWindowRotationAngle());
    b->setListSize(itemCount);
    b->setUseListItemCache(mSubtreeCacheEnabled);
    b->setImageBasedRendering(mMainView->imageBasedRendering());
    b->setTheme(Theme::p()->currentThemeName());
    b->setListType(listType);
    b->setTag(tag);

    if (listType==Benchmark::RecyclingListType){
        if(mResMon && mCpuMemLogging) {
            //CPU/MEM begin
            mResMon->BeginMeasureCPULoad(); 
            mResMon->BeginMeasureMemoryLoad();
        }
        
        QTime t;
        t.start();
        ItemRecyclingList *list = new ItemRecyclingList();

        list->setListItemCaching(mSubtreeCacheEnabled);

        mMainView->setTestWidget(list);

        mDataGenerator.Reset();

        fillRecyclingList(mDataGenerator, itemCount, list);

        int took = t.elapsed();

        b->setValue(took);
        if(mResMon && mCpuMemLogging) {
            //CPU/MEM end
            createCpuBenchmark(testFunctionResult, tag, itemCount, listType,Theme::p()->currentThemeName(), mResMon->EndMeasureCPULoad());
            createMemoryBenchmark(testFunctionResult, tag, itemCount, listType,Theme::p()->currentThemeName(), mResMon->EndMeasureMemoryLoad());
        }
    }
    else {
        if(mResMon && mCpuMemLogging) {
            //CPU/MEM begin
            mResMon->BeginMeasureCPULoad(); 
            mResMon->BeginMeasureMemoryLoad();
        }
        
        QTime t;
        t.start();
        SimpleList *list = new SimpleList();
        list->setListItemCaching(mSubtreeCacheEnabled);
        mMainView->setTestWidget(list);

        mDataGenerator.Reset();

        fillSimpleList(mDataGenerator, itemCount, list);

        int took = t.elapsed();
        b->setValue(took);
        
        if(mResMon && mCpuMemLogging) {
            //CPU/MEM end
            createCpuBenchmark(testFunctionResult, tag, itemCount, listType,Theme::p()->currentThemeName(), mResMon->EndMeasureCPULoad());
            createMemoryBenchmark(testFunctionResult, tag, itemCount, listType,Theme::p()->currentThemeName(), mResMon->EndMeasureMemoryLoad());
        }
    }

    // Make sure we record the correct view size
    b->setWidth(mMainView->size().width());
    b->setHeight(mMainView->size().height());

    QTimer::singleShot(EmitDoneTimeout, this, SLOT(testIsDone()));
}

void TestController::scrollList(int maxTimeMs, TestFunctionResult *testFunctionResult, const QString &tag)
{
    if (!mMainView) {
        QTimer::singleShot(EmitDoneTimeout, this, SLOT(testIsDone()));
        return;
    }

    mMaxTestDurationTime = maxTimeMs;

    QGraphicsWidget *tw = mMainView->testWidget();
    ItemRecyclingList *list = dynamic_cast<ItemRecyclingList *>(tw);    
    if (list) {
        if (mMainView)
            mMainView->fpsReset();
        mTestDuration.start();
        mLastElapsed = 0;
        mCurrentRecyclingList = list;
        connect(this, SIGNAL(doScrollRecyclingList(TestFunctionResult*, const QString &)),
                this, SLOT(scrollRecyclingList(TestFunctionResult*, const QString &)), Qt::QueuedConnection);
        
        if(mResMon && mCpuMemLogging) {
            //CPU/MEM begin
            mResMon->BeginMeasureCPULoad(); 
            mResMon->BeginMeasureMemoryLoad();
        }
        
        emit doScrollRecyclingList(testFunctionResult, tag);
    }
    else {
        SimpleList *list = dynamic_cast<SimpleList*>(tw);
        if (list) {
            if (mMainView)
                mMainView->fpsReset();
            mTestDuration.start();
            mLastElapsed = 0;
            mCurrentSimpleList = list;
            connect(this, SIGNAL(doScrollSimpleList(TestFunctionResult*, const QString &)),
                    this, SLOT(scrollSimpleList(TestFunctionResult*, const QString &)), Qt::QueuedConnection);
            
            if(mResMon && mCpuMemLogging) {
                //CPU/MEM begin        
                mResMon->BeginMeasureCPULoad(); 
                mResMon->BeginMeasureMemoryLoad();
            }
            
            emit doScrollSimpleList(testFunctionResult, tag);
        }
        else {
            QTimer::singleShot(EmitDoneTimeout, this, SLOT(testIsDone()));
        }
   }
}

void TestController::addToBeginningOfList(int itemCount, TestFunctionResult *testFunctionResult, const QString &tag)
{
    if (!mMainView) {
        QTimer::singleShot(EmitDoneTimeout, this, SLOT(testIsDone()));
        return;
    }

    Benchmark *b = testFunctionResult->createBenchmark();
    b->setTag(tag);
    b->setRotation(mainWindowRotationAngle());
    b->setListSize(itemCount);
    b->setUseListItemCache(mSubtreeCacheEnabled);
    b->setImageBasedRendering(mMainView->imageBasedRendering());
    b->setTheme(Theme::p()->currentThemeName());
    b->setWidth(mMainView->size().width());
    b->setHeight(mMainView->size().height());

    QGraphicsWidget *tw = mMainView->testWidget();
    ItemRecyclingList *list = dynamic_cast<ItemRecyclingList *>(tw);

    if (list) {
        QString type = list->objectName();
        
        if(mResMon && mCpuMemLogging) {
            //CPU/MEM begin
            mResMon->BeginMeasureCPULoad(); 
            mResMon->BeginMeasureMemoryLoad();
        }
        
        QTime t;
        t.start();
        for (int i=0; i<itemCount; ++i )
            list->insertItem(0, newRecyclingListItem(mDataGenerator, i));
        int took = t.elapsed();
        
        if(mResMon && mCpuMemLogging) {
            //CPU/MEM end
            createCpuBenchmark(testFunctionResult, tag, itemCount, Benchmark::RecyclingListType,Theme::p()->currentThemeName(), mResMon->EndMeasureCPULoad());
            createMemoryBenchmark(testFunctionResult, tag, itemCount, Benchmark::RecyclingListType,Theme::p()->currentThemeName(), mResMon->EndMeasureMemoryLoad());
        }
        
        b->setValue(took);
        b->setListType(Benchmark::RecyclingListType);
    }
    else {
        SimpleList *list = dynamic_cast<SimpleList*>(tw);
        if (list) {
            QString type = list->objectName();
            
            if(mResMon && mCpuMemLogging) {
                //CPU/MEM begin
                mResMon->BeginMeasureCPULoad(); 
                mResMon->BeginMeasureMemoryLoad();
            }
            
            QTime t;
            t.start();
            for (int i=0; i<itemCount; ++i )
                list->insertItem(0,newSimpleListItem(mDataGenerator, i));
            int took = t.elapsed();

            if(mResMon && mCpuMemLogging) {
                //CPU/MEM end
                createCpuBenchmark(testFunctionResult, tag, itemCount, Benchmark::SimpleListType, Theme::p()->currentThemeName(), mResMon->EndMeasureCPULoad());
                createMemoryBenchmark(testFunctionResult, tag, itemCount, Benchmark::SimpleListType, Theme::p()->currentThemeName(), mResMon->EndMeasureMemoryLoad());
            }
            
            b->setValue(took);
            b->setListType(Benchmark::SimpleListType);
        }
   }

   QTimer::singleShot(EmitDoneTimeout, this, SLOT(testIsDone()));
}

void TestController::removeFromBeginningOfList(int itemCount, TestFunctionResult *testFunctionResult, const QString &tag)
{
    if (!mMainView) {
        QTimer::singleShot(EmitDoneTimeout, this, SLOT(testIsDone()));
        return;
    }

    Benchmark *b = testFunctionResult->createBenchmark();
    b->setTag(tag);
    b->setRotation(mainWindowRotationAngle());
    b->setListSize(itemCount);
    b->setUseListItemCache(mSubtreeCacheEnabled);
    b->setImageBasedRendering(mMainView->imageBasedRendering());
    b->setTheme(Theme::p()->currentThemeName());
    b->setWidth(mMainView->size().width());
    b->setHeight(mMainView->size().height());

    QGraphicsWidget *tw = mMainView->testWidget();
    ItemRecyclingList *list = dynamic_cast<ItemRecyclingList *>(tw);

    if (list) {
        QString type = list->objectName();
        
        if(mResMon && mCpuMemLogging) {
            //CPU/MEM begin
            mResMon->BeginMeasureCPULoad(); 
            mResMon->BeginMeasureMemoryLoad();
        }
        
        QTime t;
        t.start();

        for (int i=0; i<itemCount; ++i)
           delete list->takeItem(0);

        int took = t.elapsed();
        
        if(mResMon && mCpuMemLogging) {
            //CPU/MEM end
            createCpuBenchmark(testFunctionResult, tag, itemCount, Benchmark::RecyclingListType,Theme::p()->currentThemeName(), mResMon->EndMeasureCPULoad());
            createMemoryBenchmark(testFunctionResult, tag, itemCount, Benchmark::RecyclingListType, Theme::p()->currentThemeName(), mResMon->EndMeasureMemoryLoad());
        }
        
        b->setValue(took);
        b->setListType(Benchmark::RecyclingListType);
    }
    else {
        SimpleList *list = dynamic_cast<SimpleList*>(tw);
        if (list) {
            QString type = list->objectName();
            
            if(mResMon && mCpuMemLogging) {
                //CPU/MEM begin
                mResMon->BeginMeasureCPULoad(); 
                mResMon->BeginMeasureMemoryLoad();
            }
            
            QTime t;
            t.start();

            for (int i=0; i<itemCount; ++i)
                delete list->takeItem(0);

            int took = t.elapsed();
            
            if(mResMon && mCpuMemLogging) {
                //CPU/MEM end
                createCpuBenchmark(testFunctionResult, tag, itemCount, Benchmark::SimpleListType, Theme::p()->currentThemeName(), mResMon->EndMeasureCPULoad());
                createMemoryBenchmark(testFunctionResult, tag, itemCount, Benchmark::SimpleListType,Theme::p()->currentThemeName(), mResMon->EndMeasureMemoryLoad());
            }
            
            b->setValue(took);
            b->setListType(Benchmark::SimpleListType);
        }
    }

    QTimer::singleShot(EmitDoneTimeout, this, SLOT(testIsDone()));
}



void TestController::themeChange(int theme, TestFunctionResult *testFunctionResult, const QString &tag)
{
    if (!mMainView) {
        QTimer::singleShot(EmitDoneTimeout, this, SLOT(testIsDone()));
        return;
    }

    QString listName = "";
    int itemCount = 0;
    Benchmark::ListType type = Benchmark::ListTypeNone;

    QGraphicsWidget *tw = mMainView->testWidget();
    ItemRecyclingList *list = dynamic_cast<ItemRecyclingList *>(tw);
    if (list) {
        type = Benchmark::RecyclingListType;
        itemCount = list->indexCount();
    }
    else {
        SimpleList *list = dynamic_cast<SimpleList*>(tw);
        if (list) {
            type = Benchmark::SimpleListType;
            itemCount = list->itemCount();
        }
        else {
            QTimer::singleShot(EmitDoneTimeout, this, SLOT(testIsDone()));
            return;
        }
    }

    Benchmark *b = testFunctionResult->createBenchmark();
    b->setTag(tag);
    b->setTheme(Theme::p()->currentThemeName());

    //CPU/MEM begin
    QString currentTheme = Theme::p()->currentThemeName();
    if(mResMon && mCpuMemLogging) {
        mResMon->BeginMeasureCPULoad(); 
        mResMon->BeginMeasureMemoryLoad();
    }
    
    QTime t;
    t.start();
    Theme::p()->setTheme(Theme::Themes(theme));
    int took = t.elapsed();
    b->setValue(took);
    b->setRotation(mainWindowRotationAngle());
    b->setListType(type);
    b->setListSize(itemCount);
    b->setUseListItemCache(mSubtreeCacheEnabled);
    b->setImageBasedRendering(mMainView->imageBasedRendering());
    b->setWidth(mMainView->size().width());
    b->setHeight(mMainView->size().height());

    if(mResMon && mCpuMemLogging) {
        //CPU/MEM end
        createCpuBenchmark(testFunctionResult, tag, itemCount, type, currentTheme, mResMon->EndMeasureCPULoad());
        createMemoryBenchmark(testFunctionResult, tag, itemCount, type,currentTheme, mResMon->EndMeasureMemoryLoad());
    }
    
    QTimer::singleShot(EmitDoneTimeout, this, SLOT(testIsDone()));
}

void TestController::forceUpdate(int maxTimeMs, TestFunctionResult *testFunctionResult, const QString &tag)
{
    if (!mMainView) {
        QTimer::singleShot(EmitDoneTimeout, this, SLOT(testIsDone()));
        return;
    }

    QGraphicsWidget *tw = mMainView->testWidget();
    ItemRecyclingList *list = dynamic_cast<ItemRecyclingList *>(tw);
    if (list) {
        mCurrentRecyclingList = list;
    }
    else {
        SimpleList *list = dynamic_cast<SimpleList*>(tw);
        if (list) {
            mCurrentSimpleList = list;
        }
        else {
            QTimer::singleShot(EmitDoneTimeout, this, SLOT(testIsDone()));
        }
    }
    connect(this, SIGNAL(doForceUpdate(TestFunctionResult*, const QString &)),
            this, SLOT(doForceUpdateViewport(TestFunctionResult*, const QString &)), Qt::QueuedConnection);
    mMaxTestDurationTime = maxTimeMs;
    mMainView->fpsReset();
    mTestDuration.start();

    emit doForceUpdate(testFunctionResult, tag);
}

void TestController::applicationExit()
{
    QApplication::exit();
}

void TestController::deleteList(TestFunctionResult *testFunctionResult, const QString &tag)
{
    Benchmark *b = testFunctionResult->createBenchmark();
    b->setTag(tag);
    b->setRotation(mainWindowRotationAngle());
    b->setUseListItemCache(mSubtreeCacheEnabled);
    b->setImageBasedRendering(mMainView->imageBasedRendering());
    b->setTheme(Theme::p()->currentThemeName());
    b->setWidth(mMainView->size().width());
    b->setHeight(mMainView->size().height());

    int itemCount = 0;
    QGraphicsWidget *tw = mMainView->takeTestWidget();
    ItemRecyclingList *list = dynamic_cast<ItemRecyclingList *>(tw);
    if (list) {
        itemCount = list->indexCount();
        
        if(mResMon && mCpuMemLogging) {
            //CPU/MEM begin
            mResMon->BeginMeasureCPULoad(); 
            mResMon->BeginMeasureMemoryLoad();
        }
        
        QTime t;
        t.start();
        delete list;
        int took = t.elapsed();
        b->setValue(took);
        b->setListSize(itemCount);
        b->setListType(Benchmark::RecyclingListType);
        
        if(mResMon && mCpuMemLogging) {
            //CPU/MEM end
            createCpuBenchmark(testFunctionResult, tag, itemCount, Benchmark::RecyclingListType,Theme::p()->currentThemeName(), mResMon->EndMeasureCPULoad());
            createMemoryBenchmark(testFunctionResult, tag, itemCount, Benchmark::RecyclingListType,Theme::p()->currentThemeName(), mResMon->EndMeasureMemoryLoad());
        }
        
    }
    else {
        SimpleList *list = dynamic_cast<SimpleList*>(tw);
        if (list) {
            itemCount = list->itemCount();
            
            if(mResMon && mCpuMemLogging) {
                //CPU/MEM begin
                mResMon->BeginMeasureCPULoad(); 
                mResMon->BeginMeasureMemoryLoad();
            }
            
            QTime t;
            t.start();
            delete list;
            int took = t.elapsed();
            b->setValue(took);
            b->setListSize(itemCount);
            b->setListType(Benchmark::SimpleListType);
            
            if(mResMon && mCpuMemLogging) {
                //CPU/MEM end
                createCpuBenchmark(testFunctionResult, tag, itemCount, Benchmark::SimpleListType,Theme::p()->currentThemeName(), mResMon->EndMeasureCPULoad());
                createMemoryBenchmark(testFunctionResult, tag, itemCount, Benchmark::SimpleListType,Theme::p()->currentThemeName(), mResMon->EndMeasureMemoryLoad());
            }
        }
    }

    QTimer::singleShot(EmitDoneTimeout, this, SLOT(testIsDone()));
}

void TestController::scrollSimpleList(TestFunctionResult *testFunctionResult, const QString &tag)
{
    int elapsed = mTestDuration.elapsed();
    if(!mCurrentSimpleList ||
       elapsed > mMaxTestDurationTime) {
        if (mMainView) {
            qreal fps = mMainView->fps();

            Benchmark *b = testFunctionResult->createBenchmark();
            b->setTag(tag);
            b->setValue(fps);
            if (mCurrentSimpleList)
                b->setListSize(mCurrentSimpleList->itemCount());
            b->setRotation(mainWindowRotationAngle());
            b->setImageBasedRendering(mMainView->imageBasedRendering());
            b->setTheme(Theme::p()->currentThemeName());
            b->setUseListItemCache(mSubtreeCacheEnabled);
            b->setListType(Benchmark::SimpleListType);
            b->setWidth(mMainView->size().width());
            b->setHeight(mMainView->size().height());

            if(mResMon && mCpuMemLogging) {
                //CPU/MEM end
                createCpuBenchmark(testFunctionResult, tag, mCurrentSimpleList->itemCount(), Benchmark::SimpleListType,Theme::p()->currentThemeName(), mResMon->EndMeasureCPULoad());
               createMemoryBenchmark(testFunctionResult, tag, mCurrentSimpleList->itemCount(), Benchmark::SimpleListType,Theme::p()->currentThemeName(), mResMon->EndMeasureMemoryLoad());
            }
        }
        disconnect(SIGNAL(doScrollSimpleList(TestFunctionResult*, const QString &)),
                   this, SLOT(scrollSimpleList(TestFunctionResult*, const QString &)));

        if (mCurrentSimpleList) { // Scroll to begin before next case.
            ScrollBar *sb = mCurrentSimpleList->verticalScrollBar();

            if (sb) {
                sb->setSliderPosition(0);
            }
        }
        QTimer::singleShot(EmitDoneTimeout, this, SLOT(testIsDone()));
        return;
    }

    ScrollBar *sb = mCurrentSimpleList->verticalScrollBar();

    if (sb) {
        int sinceLast = elapsed - mLastElapsed;
        qreal sliderSize = sb->sliderSize();
        qreal currentVal = sb->sliderPosition();
        qreal increment = (mScrollDirection*mScrollStep*sinceLast)/16.6666; // Targeting 1.0 = 1 per 60FPS frame
        qreal target = currentVal + increment;
        while (target<0.0 || target>sliderSize) {
            if (target < 0.0) {
                target = -target; // Reflect
                mScrollDirection = -1*mScrollDirection;
            }
            else if (target > sliderSize) {
                target = sliderSize - (target-sliderSize); // Reflect
                mScrollDirection = -1*mScrollDirection;
            }
        }

        sb->setSliderPosition(target);
    }

    mLastElapsed = elapsed;

    emit doScrollSimpleList(testFunctionResult, tag);
}

void TestController::scrollRecyclingList(TestFunctionResult *testFunctionResult, const QString &tag)
{
    int elapsed = mTestDuration.elapsed();
    if(!mCurrentRecyclingList ||
       elapsed > mMaxTestDurationTime) {
       if (mMainView) {
            qreal fps = mMainView->fps();

            Benchmark *b = testFunctionResult->createBenchmark();
            b->setTag(tag);
            b->setValue(fps);
            if (mCurrentRecyclingList)
                b->setListSize(mCurrentRecyclingList->indexCount());
            b->setRotation(mainWindowRotationAngle());
            b->setImageBasedRendering(mMainView->imageBasedRendering());
            b->setTheme(Theme::p()->currentThemeName());
            b->setUseListItemCache(mSubtreeCacheEnabled);            
            b->setListType(Benchmark::RecyclingListType);
            b->setWidth(mMainView->size().width());
            b->setHeight(mMainView->size().height());

            if(mResMon && mCpuMemLogging) {
                //CPU/MEM end
                createCpuBenchmark(testFunctionResult, tag, mCurrentRecyclingList->indexCount(), Benchmark::RecyclingListType,Theme::p()->currentThemeName(), mResMon->EndMeasureCPULoad());
                createMemoryBenchmark(testFunctionResult, tag, mCurrentRecyclingList->indexCount(), Benchmark::RecyclingListType,Theme::p()->currentThemeName(), mResMon->EndMeasureMemoryLoad());
            }
                    
        }
        disconnect(SIGNAL(doScrollRecyclingList(TestFunctionResult*, const QString &)),
                  this, SLOT(scrollRecyclingList(TestFunctionResult*, const QString &)));

         if (mCurrentRecyclingList) { // Scroll to begin before next case.
            ScrollBar *sb = mCurrentRecyclingList->verticalScrollBar();

            if (sb) {
                sb->setSliderPosition(0);
            }
        }
        QTimer::singleShot(EmitDoneTimeout, this, SLOT(testIsDone()));
        return;
    }

    ScrollBar *sb = mCurrentRecyclingList->verticalScrollBar();

    if (sb) {
        int sinceLast = elapsed - mLastElapsed;
        qreal sliderSize = sb->sliderSize();
        qreal currentVal = sb->sliderPosition();
        qreal increment = (mScrollDirection*mScrollStep*sinceLast)/16.6666; // Targeting 1.0 = 1 per 60FPS frame
        qreal target = currentVal + increment;
        while (target<0.0 || target>sliderSize) {
            if (target < 0.0) {
                target = -target; // Reflect
                mScrollDirection = -1*mScrollDirection;
            }
            else if (target > sliderSize) {
                target = sliderSize - (target-sliderSize); // Reflect
                mScrollDirection = -1*mScrollDirection;
            }
        }

        sb->setSliderPosition(target);
    }

    mLastElapsed = elapsed;

    emit doScrollRecyclingList(testFunctionResult, tag);
}

void TestController::doForceUpdateViewport(TestFunctionResult *testFunctionResult, const QString &tag)
{
    if (mTestDuration.elapsed() > mMaxTestDurationTime) {
       if (mMainView) {
            qreal fps = mMainView->fps();
            Benchmark::ListType type = Benchmark::ListTypeNone;
            int items = 0;

            QGraphicsWidget *tw = mMainView->testWidget();
            ItemRecyclingList *list = dynamic_cast<ItemRecyclingList *>(tw);
            if (list) {
                type = Benchmark::RecyclingListType;
                items = list->indexCount();
            }
            else {
                SimpleList *list = dynamic_cast<SimpleList*>(tw);
                if (list) {
                    type = Benchmark::SimpleListType;
                    items = list->itemCount();
                }
            }
            Benchmark *b = testFunctionResult->createBenchmark();
            b->setTag(tag);
            b->setValue(fps);
            b->setRotation(mainWindowRotationAngle());
            b->setListSize(items);
            b->setImageBasedRendering(mMainView->imageBasedRendering());
            b->setTheme(Theme::p()->currentThemeName());
            b->setUseListItemCache(mSubtreeCacheEnabled);
            b->setListType(type);
            b->setWidth(mMainView->size().width());
            b->setHeight(mMainView->size().height());

            mMainView->fpsReset();
        }
       disconnect(SIGNAL(doForceUpdate(TestFunctionResult*, const QString &)),
                  this, SLOT(doForceUpdateViewport(TestFunctionResult*, const QString &)));
       QTimer::singleShot(EmitDoneTimeout, this, SLOT(testIsDone()));
        return;
    }
    else if (mMainView) {
        mMainView->viewport()->update();
        emit doForceUpdate(testFunctionResult, tag);
        return;
    }
    else {
        mMainView->fpsReset();
        QTimer::singleShot(EmitDoneTimeout, this, SLOT(testIsDone()));
        return;
    }
}

void TestController::setImageBasedRendering(const bool imageBasedRendering)
{
    if (mMainView)
        mMainView->setImageBasedRendering(imageBasedRendering);
}
bool TestController::imageBasedRendering() const
{
    if (mMainView)
        return mMainView->imageBasedRendering();
    return false;
}

void TestController::setTwoColumns(const bool twoCols)
{
    QGraphicsWidget *tw = mMainView->testWidget();
    ItemRecyclingList *list = dynamic_cast<ItemRecyclingList *>(tw);
    if (list) {
        list->setTwoColumns(twoCols);
    }
    else {
        SimpleList *list = dynamic_cast<SimpleList*>(tw);
        if (list) {
            list->setTwoColumns(twoCols);
        }
    }

    QTimer::singleShot(EmitDoneTimeout, this, SLOT(testIsDone()));
}

bool TestController::twoColumns() const
{
    QGraphicsWidget *tw = mMainView->takeTestWidget();
    ItemRecyclingList *list = dynamic_cast<ItemRecyclingList *>(tw);
    if (list) {
        return list->twoColumns();
    }
    else {
        SimpleList *list = dynamic_cast<SimpleList*>(tw);
        if (list)
            return list->twoColumns();
    }
    return false;
}

bool TestController::loadPlugin()
{
    QDir pluginsDir(qApp->applicationDirPath());
#if defined Q_OS_LINUX
    pluginsDir.cdUp();
    pluginsDir.cd("plugins");
#endif
    foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
        QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = pluginLoader.instance();
        if (plugin) {
            mResMon = qobject_cast<ResourceMonitorInterface *>(plugin);
            if (mResMon) 
                return true;
        }
        else {
            qDebug() << "Error while loading plugin: "
                    << pluginsDir.absoluteFilePath(fileName)
                    << ". Error: " << pluginLoader.errorString();
        }
    }
    return false;
}

void TestController::createCpuBenchmark(TestFunctionResult *testFunctionResult, const QString &tag, int listItems,  Benchmark::ListType type, const QString &theme, ResourceMonitorInterface::CpuUsage cpuUsage)
{
    Benchmark* bThread = testFunctionResult->createBenchmark();
    Benchmark* bSystem = testFunctionResult->createBenchmark();
    
    bThread->setTag(QString("CPU AppThread " + tag));
    bThread->setRotation(mainWindowRotationAngle());
    bThread->setListSize(listItems);
    bThread->setListType(type);
    bThread->setTheme(theme);
    bThread->setValue(cpuUsage.appTreadUsage);
    bThread->setWidth(mMainView->size().width());
    bThread->setHeight(mMainView->size().height());
    bSystem->setTag(QString("CPU System " + tag));
    bSystem->setRotation(mainWindowRotationAngle());
    bSystem->setListSize(listItems);
    bSystem->setListType(type);
    bSystem->setTheme(theme);
    bSystem->setValue(cpuUsage.systemUsage);
    bSystem->setWidth(mMainView->size().width());
    bSystem->setHeight(mMainView->size().height());
}

void TestController::createMemoryBenchmark(TestFunctionResult *testFunctionResult, const QString &tag, int listItems,  Benchmark::ListType type, const QString &theme, ResourceMonitorInterface::MemoryAllocation allocation)
{
    Benchmark* bAllocated = testFunctionResult->createBenchmark();
    Benchmark* bAppHeap = testFunctionResult->createBenchmark();
    Benchmark* bAvailableSystem = testFunctionResult->createBenchmark();
    
    bAllocated->setTag(QString("Memory AppAllocInCells " + tag));
    bAllocated->setRotation(mainWindowRotationAngle());
    bAllocated->setListSize(listItems);
    bAllocated->setListType(type);
    bAllocated->setTheme(theme);
    bAllocated->setUseListItemCache(mSubtreeCacheEnabled);
    if (mMainView)
        bAllocated->setImageBasedRendering(mMainView->imageBasedRendering());
    bAllocated->setValue(allocation.allocatedInAppThread);
    bAllocated->setWidth(mMainView->size().width());
    bAllocated->setHeight(mMainView->size().height());
    bAppHeap->setTag(QString("Memory AvailAppHeap " + tag));
    bAppHeap->setRotation(mainWindowRotationAngle());
    bAppHeap->setListSize(listItems);
    bAppHeap->setListType(type);
    bAppHeap->setTheme(theme);
    bAppHeap->setUseListItemCache(mSubtreeCacheEnabled);
    if (mMainView)
        bAppHeap->setImageBasedRendering(mMainView->imageBasedRendering());
    bAppHeap->setValue(allocation.availableMemoryInAppThreadHeap);
    bAppHeap->setWidth(mMainView->size().width());
    bAppHeap->setHeight(mMainView->size().height());
    bAvailableSystem->setTag(QString("Memory AvailSystem " + tag));
    bAvailableSystem->setRotation(mainWindowRotationAngle());
    bAvailableSystem->setListSize(listItems);
    bAvailableSystem->setListType(type);
    bAvailableSystem->setTheme(theme);
    bAvailableSystem->setUseListItemCache(mSubtreeCacheEnabled);
    if (mMainView)
        bAvailableSystem->setImageBasedRendering(mMainView->imageBasedRendering());
    bAvailableSystem->setValue(allocation.availableMemoryInSystem);
    bAvailableSystem->setWidth(mMainView->size().width());
    bAvailableSystem->setHeight(mMainView->size().height());
}

