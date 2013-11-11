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

#ifndef TESTTHREAD_H
#define TESTTHREAD_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QScriptable>

#include "testcontroller.h"

class QApplication;

class TestThread : public QThread, protected QScriptable
{
    Q_OBJECT
public:
    virtual ~TestThread();

    Q_PROPERTY(TestController *mController READ controller WRITE setController)
    Q_PROPERTY(ResultLogger *mResultLogger  READ resultLogger WRITE setResultLogger)

    void addTestFunctionBenchmark(const QString &functionName, const QString &benchmarkTag, const qreal value);

public slots:
    void setController(TestController *controller);
    TestController* controller() const;
    void setResultLogger(ResultLogger *logger);
    ResultLogger* resultLogger() const;

    // These are available to test script
    void fillListTest(int itemCount, int listType, TestFunctionResult *testFunctionResult, const QString &tag);
    void deleteListTest(TestFunctionResult *testFunctionResult, const QString &tag);
    void scrollListTest(int scrollMaxTime, TestFunctionResult *testFunctionResult, const QString &tag);
    void addToBeginningOfListTest(int itemCount, TestFunctionResult *testFunctionResult, const QString &tag);
    void removeFromBeginningOfListTest(int itemCount, TestFunctionResult *testFunctionResult, const QString &tag);
    void forceUpdateTest(int updateMaxTime, TestFunctionResult *testFunctionResult, const QString &tag);
    void themeChangeTest(int theme, TestFunctionResult *testFunctionResult, const QString &tag);

    int themeCount() const;
    void setTheme(int index);
    int currentThemeId() const;
    QString currentThemeName() const;
    QString themeName(int theme) const;
    void setImageBasedRendering(const bool imageBasedRendering);
    void setSubtreeCache(const bool enabled);
    void setScrollStep(const int step);
    bool imageBasedRendering() const;
    bool subtreeCache() const;
    int scrollStep() const;
    void rotateMainWindow(const int angle);
    int mainWindowRotationAngle() const;
    void setTwoColumns(const bool twoCols);
    bool twoColumns() const;

signals:
    void fillList(int itemCount, Benchmark::ListType listType, TestFunctionResult *testFunctionResult, const QString &tag);
    void deleteList(TestFunctionResult *testFunctionResult, const QString &tag);
    void themeChange(int theme, TestFunctionResult *testFunctionResult, const QString &tag);
    void scrollList(int maxTimeMs, TestFunctionResult *testFunctionResult, const QString &tag);
    void forceUpdate(int maxTimeMs, TestFunctionResult *testFunctionResult, const QString &tag);
    void addToBeginningOfList(int itemCount, TestFunctionResult *testFunctionResult, const QString &tag);
    void removeFromBeginningOfList(int itemCount, TestFunctionResult *testFunctionResult, const QString &tag);
    void doRotateMainWindow(int);
    void doSetTwoColumns(bool);
    void changeTheme(Theme::Themes theme);

protected slots:
    void stop();

protected:
    TestThread(TestController *ctrl, QObject *parent=0);

    bool m_doExit;
    //QApplication *mApplication;
    ResultLogger *m_resultLogger;

private slots:
    void testDone();

private:
    Q_DISABLE_COPY(TestThread)

    QMutex m_mutex;
    QWaitCondition m_waitCondition;
    TestController *m_controller;    
};

#endif // TESTTHREAD_H
