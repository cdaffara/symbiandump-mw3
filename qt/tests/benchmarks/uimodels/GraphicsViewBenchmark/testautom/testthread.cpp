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
#include <QTimer>

#include "testthread.h"
#include "theme.h"

static const int SleepTimeBetweenCases = 1000;

Q_DECLARE_METATYPE(TestThread*)

TestThread::TestThread(TestController *ctrl, QObject *parent)
    : QThread(parent),    
    m_doExit(false),
    m_resultLogger(0)
{
    setController(ctrl);
    connect(controller(), SIGNAL(stop()), this, SLOT(stop()));
    connect(controller(), SIGNAL(testDone()), this, SLOT(testDone()), Qt::QueuedConnection);
    connect(this, SIGNAL(changeTheme(Theme::Themes)),
            controller(), SLOT(changeTheme(Theme::Themes)));

    connect(this, SIGNAL(fillList(int,Benchmark::ListType,TestFunctionResult*, const QString &)),
            controller(), SLOT(fillList(int,Benchmark::ListType, TestFunctionResult*, const QString &)),
            Qt::QueuedConnection);

    connect(this, SIGNAL(doRotateMainWindow(int)), controller(), SLOT(rotateMainWindow(int)),
            Qt::QueuedConnection);
    connect(this, SIGNAL(doSetTwoColumns(bool)), controller(), SLOT(setTwoColumns(bool)),
            Qt::QueuedConnection);

    connect(this, SIGNAL(deleteList(TestFunctionResult*, const QString &)),
            controller(), SLOT(deleteList(TestFunctionResult*, const QString &)),
            Qt::QueuedConnection);
    connect(this, SIGNAL(themeChange(int,TestFunctionResult*, const QString &)),
            controller(), SLOT(themeChange(int,TestFunctionResult*, const QString &)),
            Qt::QueuedConnection);
    connect(this, SIGNAL(scrollList(int,TestFunctionResult*, const QString &)),
            controller(), SLOT(scrollList(int,TestFunctionResult*, const QString &)),
            Qt::QueuedConnection);
    connect(this, SIGNAL(forceUpdate(int,TestFunctionResult*, const QString &)),
            controller(), SLOT(forceUpdate(int,TestFunctionResult*, const QString &)),
            Qt::QueuedConnection);
    connect(this, SIGNAL(addToBeginningOfList(int,TestFunctionResult*, const QString &)),
            controller(), SLOT(addToBeginningOfList(int,TestFunctionResult*, const QString &)),
            Qt::QueuedConnection);
    connect(this, SIGNAL(removeFromBeginningOfList(int,TestFunctionResult*, const QString &)),
            controller(), SLOT(removeFromBeginningOfList(int,TestFunctionResult*, const QString &)),
            Qt::QueuedConnection);
}

TestThread::~TestThread()
{
    delete m_resultLogger;
}

void TestThread::stop()
{
    m_doExit = true;
    testDone();
}

void TestThread::testDone()
{
    m_waitCondition.wakeAll();
}

TestController *TestThread::controller() const
{
    return m_controller;
}
void TestThread::setController(TestController *ctrl)
{
    m_controller = ctrl;
}

void TestThread::setResultLogger(ResultLogger *logger)
{
    if (m_resultLogger == logger)
        return;
    if (m_resultLogger)
        delete m_resultLogger;
    m_resultLogger = logger;
}

ResultLogger* TestThread::resultLogger() const
{
    return m_resultLogger;
}


void TestThread::addTestFunctionBenchmark(const QString &functionName, const QString &benchmarkTag, const qreal value)
{
    if (!m_resultLogger)
        return;
    TestFunctionResult *f = m_resultLogger->getTestFunctionResult(functionName);
    if (!f)
        f = m_resultLogger->createTestFunctionResult(functionName);

    Benchmark *b = f->createBenchmark();
    b->setTag(benchmarkTag);
    b->setValue(value);
    b->setTheme(Theme::p()->currentThemeName());
}

void TestThread::setScrollStep(const int step)
{
    controller()->setScrollStep(step);
}
int TestThread::scrollStep() const
{
    return controller()->scrollStep();
}

bool TestThread::imageBasedRendering() const
{
    return controller()->imageBasedRendering();
}

bool TestThread::subtreeCache() const
{
    return controller()->subtreeCache();
}

void TestThread::rotateMainWindow(const int angle)
{
    if (m_doExit) return;
    m_mutex.lock();
    emit doRotateMainWindow(angle);
    m_waitCondition.wait(&m_mutex);
    m_mutex.unlock();
    if (m_doExit) return;
    msleep(SleepTimeBetweenCases);
}

int TestThread::mainWindowRotationAngle() const
{
    return controller()->mainWindowRotationAngle();
}

int TestThread::themeCount() const
{
    return Theme::p()->themes().count();
}

int TestThread::currentThemeId() const
{
    return int(Theme::p()->theme());
}

void TestThread::setTheme(int index)
{
    emit changeTheme(Theme::Themes(index));
    if (m_doExit) return;
    msleep(SleepTimeBetweenCases);
}

QString TestThread::currentThemeName() const
{
    return Theme::p()->currentThemeName();
}
QString TestThread::themeName(int theme) const
{
    return Theme::p()->themes().at(theme);
}

void TestThread::setImageBasedRendering(const bool imageBasedRendering)
{
    controller()->setImageBasedRendering(imageBasedRendering);
}

void TestThread::setTwoColumns(const bool twoCols)
{
    if (m_doExit) return;
    m_mutex.lock();
    emit doSetTwoColumns(twoCols);
    m_waitCondition.wait(&m_mutex);
    m_mutex.unlock();
    if (m_doExit) return;
    msleep(SleepTimeBetweenCases);
}

bool TestThread::twoColumns() const
{
    return controller()->twoColumns();
}

void TestThread::fillListTest(int itemCount, int listType, TestFunctionResult *testFunctionResult, const QString &tag)
{
    if (m_doExit) return;
    m_mutex.lock();
    emit fillList(itemCount, Benchmark::ListType(listType), testFunctionResult, tag);
    m_waitCondition.wait(&m_mutex);
    m_mutex.unlock();
    if (m_doExit) return;
    msleep(SleepTimeBetweenCases);
}

void TestThread::deleteListTest(TestFunctionResult *testFunctionResult, const QString &tag)
{
    if (m_doExit) return;
    m_mutex.lock();
    emit deleteList(testFunctionResult, tag);
    m_waitCondition.wait(&m_mutex);
    m_mutex.unlock();
    if (m_doExit) return;
    msleep(SleepTimeBetweenCases);
}

void TestThread::scrollListTest(int maxTimeMs,TestFunctionResult *testFunctionResult, const QString &tag)
{
    if (m_doExit) return;
    m_mutex.lock();
    emit scrollList(maxTimeMs, testFunctionResult, tag);
    m_waitCondition.wait(&m_mutex);
    m_mutex.unlock();
    if (m_doExit) return;
    msleep(SleepTimeBetweenCases);
}

void TestThread::addToBeginningOfListTest(int itemCount,TestFunctionResult *testFunctionResult, const QString &tag)
{
    if (m_doExit) return;
    m_mutex.lock();
    emit addToBeginningOfList(itemCount, testFunctionResult, tag);
    m_waitCondition.wait(&m_mutex);
    m_mutex.unlock();
    if (m_doExit) return;
    msleep(SleepTimeBetweenCases);
}

void TestThread::removeFromBeginningOfListTest(int itemCount, TestFunctionResult *testFunctionResult, const QString &tag)
{
    if (m_doExit) return;
    m_mutex.lock();
    emit removeFromBeginningOfList(itemCount, testFunctionResult, tag);
    m_waitCondition.wait(&m_mutex);
    m_mutex.unlock();
    if (m_doExit) return;
    msleep(SleepTimeBetweenCases);
}

void TestThread::forceUpdateTest(int updateMaxTime, TestFunctionResult *testFunctionResult, const QString &tag)
{
    if (m_doExit) return;
    m_mutex.lock();
    emit forceUpdate(updateMaxTime, testFunctionResult, tag);
    m_waitCondition.wait(&m_mutex);
    m_mutex.unlock();
    if (m_doExit) return;
    msleep(SleepTimeBetweenCases);
}


void TestThread::themeChangeTest(int theme, TestFunctionResult *testFunctionResult, const QString &tag)
{
    if (m_doExit) return;
    m_mutex.lock();
    emit themeChange(theme, testFunctionResult, tag);
    m_waitCondition.wait(&m_mutex);
    m_mutex.unlock();
    if (m_doExit) return;
    msleep(SleepTimeBetweenCases);
}


void TestThread::setSubtreeCache(const bool enabled)
{
    controller()->setSubtreeCache(enabled);
}
