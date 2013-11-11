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

#include <QFile>
#include <QTextStream>
#include <QDebug>

#include "resultlogger.h"
#include "benchmarks.h"
#include "benchmark.h"

ResultLogger::ResultLogger()
    : QObject(), m_testFunctions(), m_fileName(""), m_format()
{
}

ResultLogger::~ResultLogger()
{
    writeEntriesToFile(m_fileName, m_format);

    qDeleteAll(m_testFunctions);
    m_testFunctions.clear();
}

void ResultLogger::setResultFileInformation(const QString &filename, ResultFormat format)
{
    m_fileName = filename;
    m_format = format;
}

TestFunctionResult *ResultLogger::createTestFunctionResult(const QString &name)
{
    TestFunctionResult *tf = new TestFunctionResult();
    tf->setName(name);
    m_testFunctions.append(tf);
    return tf;
}

TestFunctionResult *ResultLogger::getTestFunctionResult(const QString &name)
{
    for (int i = 0; i <m_testFunctions.count(); ++i) {
        TestFunctionResult *tf = m_testFunctions.at(i);
        if (tf->name() == name)
            return m_testFunctions.at(i);
    }
    return 0;
}

void ResultLogger::writeEntriesToFile(const QString &filename, ResultFormat format)
{
    if(m_testFunctions.count() == 0)
        return;

    QTextStream *out = 0;
    QFile file;

    if (filename.length() > 0) {
        file.setFileName(filename);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            qWarning() << "Opening result file failed: " << filename;
            return;
        }

        out = new QTextStream(&file);
    }
    else {
        out = new QTextStream(stdout);
    }

    if (format == Xml) {
        *out << "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n";
        *out << "<TestCase name=\"GraphicsViewBenchmark\">\n";
        *out << QString("<Environment><QtVersion>%1</QtVersion><QTestVersion>%2</QTestVersion></Environment>\n").arg(QT_VERSION_STR).arg(QT_VERSION_STR).toAscii();
    }
    if (format == Xml || format == LightXml) {
        for (int i = 0; i <m_testFunctions.count(); ++i) {
            TestFunctionResult *tf = m_testFunctions.at(i);

            *out << QString("<TestFunction name=\"%1\">\n").arg(tf->name());

            Benchmarks* benchmarks = tf->benchmarks();

            if (tf->errors().length() == 0 &&
                (benchmarks && benchmarks->count() <= 0)) {
                *out << "<Incident type=\"pass\" file=\"\" line=\"0\"/>\n";
            }
            else if (tf->errors().length() > 0) {
                *out << "<Incident type=\"fail\" file=\"\" line=\"0\">\n";
                *out << "<DataTag><![CDATA[ message ]]></DataTag>\n";
                *out << "<Description><![CDATA[";
                QStringList &errors = tf->errors();
                for (int e = 0; e < errors.length(); ++e)
                    *out << errors.at(e) << " ";
                *out << "]]></Description>\n";
                *out << "</Incident>\n";
            }

            for (int j = 0; j < benchmarks->count(); ++j) {
                Benchmark *bm = benchmarks->at(j);
                *out << QString("<BenchmarkResult metric=\"walltime\" tag=\"%1\" value=\"%2\" iterations=\"1\"/>\n").arg(bm->benchmarkStr()).arg(bm->value(), 0, 'f', 2);
            }

            *out << "</TestFunction>\n";
        }
        *out << "<TestFunction name=\"cleanupTestCase\"><Incident type=\"pass\" file=\"\" line=\"0\"/></TestFunction>\n";
    }
    else {
        for (int i = 0; i <m_testFunctions.count(); ++i) {
            TestFunctionResult *tf = m_testFunctions.at(i);

            QStringList &errors = tf->errors();
            for (int e = 0; e < errors.length(); ++e)
                *out << errors.at(e) << " ";
            if (errors.length() > 0)
                *out << "\n";

            Benchmarks* benchmarks = tf->benchmarks();
            for (int j = 0; j < benchmarks->count(); ++j) {
                *out << tf->name() << " " << benchmarks->at(j)->benchmarkStr() << " " << QString("%1").arg(benchmarks->at(j)->value(), 0, 'f', 2) << "\n";
            }
        }
    }
    if (format == Xml) {
        *out << "</TestCase>\n";
    }

    delete out;
    file.close();
}
