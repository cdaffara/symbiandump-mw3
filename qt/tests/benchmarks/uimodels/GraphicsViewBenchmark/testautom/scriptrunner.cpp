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
#include <QScriptEngine>
#include <QFile>
#include <QStringList>

#include "scriptrunner.h"
#include "resultlogger.h"

static const QString TestScriptName = ":/testscripts/testscript.js";

Q_DECLARE_METATYPE(ScriptRunner*)
Q_DECLARE_METATYPE(ResultLogger*)
Q_DECLARE_METATYPE(TestFunctionResult*)
Q_DECLARE_METATYPE(Benchmark*)
Q_DECLARE_METATYPE(Benchmarks*)

QScriptValue fileLoggerToScriptValue(QScriptEngine *engine, ResultLogger* const &in)
{
    return engine->newQObject(in);
}

void fileLoggerFromScriptValue(const QScriptValue &object, ResultLogger* &out)
{
    out = qobject_cast<ResultLogger*>(object.toQObject());
}

QScriptValue testFunctionResultToScriptValue(QScriptEngine *engine, TestFunctionResult* const &in)
{
    return engine->newQObject(in);
}

void testFunctionResultFromScriptValue(const QScriptValue &object, TestFunctionResult* &out)
{
    out = qobject_cast<TestFunctionResult*>(object.toQObject());
}

QScriptValue benchmarkToScriptValue(QScriptEngine *engine, Benchmark* const &in)
{
    return engine->newQObject(in);
}

void benchmarkFromScriptValue(const QScriptValue &object, Benchmark* &out)
{
    out = qobject_cast<Benchmark*>(object.toQObject());
}

QScriptValue benchmarksToScriptValue(QScriptEngine *engine, Benchmarks* const &in)
{
    return engine->newQObject(in);
}

void benchmarksFromScriptValue(const QScriptValue &object, Benchmarks* &out)
{
    out = qobject_cast<Benchmarks*>(object.toQObject());
}

QScriptValue scriptRunnerToScriptValue(QScriptEngine *engine, ScriptRunner* const &in)
{
    return engine->newQObject(in);
}

void scriptRunnerFromScriptValue(const QScriptValue &object, ScriptRunner* &out)
{
    out = qobject_cast<ScriptRunner*>(object.toQObject());
}

ScriptRunner::ScriptRunner(const QString &scriptName, TestController *ctrl, QObject *parent)
    : TestThread(ctrl, parent),
    m_engine(new QScriptEngine(this)),
    m_testScriptFileName(TestScriptName)
{
    if (scriptName.length() > 0)
        m_testScriptFileName = scriptName;

    qScriptRegisterMetaType(m_engine,
                            scriptRunnerToScriptValue,
                            scriptRunnerFromScriptValue);


    qScriptRegisterMetaType(m_engine,
                            fileLoggerToScriptValue,
                            fileLoggerFromScriptValue);

    qScriptRegisterMetaType(m_engine,
                            testFunctionResultToScriptValue,
                            testFunctionResultFromScriptValue);

    qScriptRegisterMetaType(m_engine,
                            benchmarkToScriptValue,
                            benchmarkFromScriptValue);

    qScriptRegisterMetaType(m_engine,
                            benchmarksToScriptValue,
                            benchmarksFromScriptValue);


    if (!m_resultLogger) {
        m_resultLogger = new ResultLogger();

        m_logger = m_engine->newQObject(m_resultLogger, QScriptEngine::QtOwnership);
        m_engine->globalObject().setProperty("fileLogger", m_logger);

        m_resultLogger->setResultFileInformation(controller()->resultFileName(),
                                              controller()->resultFileFormat());
    }
}


ScriptRunner::~ScriptRunner()
{
    if (m_engine->isEvaluating())
        m_engine->abortEvaluation();
}

void ScriptRunner::run()
{
    if(!controller())
        return;

    if (m_doExit) return;

    tests();

    controller()->applicationExit();
}

void ScriptRunner::tests()
{
    QFile file(m_testScriptFileName);

    if (m_doExit) return;

    TestFunctionResult *tf = m_resultLogger->getTestFunctionResult("initTestCase");
    if (!tf)
        tf = m_resultLogger->createTestFunctionResult("initTestCase");

    if(file.open(QIODevice::ReadOnly)) {
        QString scriptContent = file.readAll();

        if (scriptContent.length() <= 0) {
            tf->addError("Can't evaluate empty script from file: '" + m_testScriptFileName +"'");
            stop();
            return;
        }

        QScriptSyntaxCheckResult result = QScriptEngine::checkSyntax(scriptContent);
        if (result.state() != QScriptSyntaxCheckResult::Valid)
        {
            QString err = "Can't evaluate script content from file. Check syntax of script on file: '" + m_testScriptFileName +"'"
                       + " Error: " + result.errorMessage()
                       + " line: " + result.errorLineNumber()
                       + " column: " + result.errorColumnNumber();
            tf->addError(err);
            stop();
            return;
        }
        if (m_doExit) return;

        QScriptValue val = m_engine->evaluate(scriptContent, m_testScriptFileName);
        if(m_engine->hasUncaughtException()) {
            QString err = "Can't evaluate script content from file. Check syntax of script on file: '" + m_testScriptFileName + "'"
                       + " Error: " + m_engine->uncaughtExceptionBacktrace().join(" ")
                       + " at line " + m_engine->uncaughtExceptionLineNumber();
            tf->addError(err);
            stop();
            return;
        }
    }
    else {
        stop();
        tf->addError("Failed to read script from file '" + m_testScriptFileName +"'");
        return;
    }

    if (m_doExit) return;

    QScriptValue ctor = m_engine->evaluate("Tests");
    QScriptValue script = m_engine->newQObject(this, QScriptEngine::QtOwnership);
    QScriptValue scripttests = ctor.construct(QScriptValueList() << script);
    if(m_engine->hasUncaughtException()) {
        QString err = "Can't evaluate script content from file. Check syntax of script on file: '" + m_testScriptFileName +"'"
                 + " Error: " + m_engine->uncaughtExceptionBacktrace().join(" ");
        tf->addError(err);
        stop();
        return;
    }
}
