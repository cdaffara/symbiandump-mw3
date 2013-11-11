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

#ifndef __FILELOGGER_H__
#define __FILELOGGER_H__

#include <QObject>
#include <QStringList>

#include "testfunctionresult.h"

class ResultLogger : public QObject
{
    Q_OBJECT
public:

    enum ResultFormat {
        Plain = 0,
        Xml = 1,
        LightXml = 2
    };

    ResultLogger();
    ~ResultLogger();

public slots:
    TestFunctionResult *createTestFunctionResult(const QString &name);
    TestFunctionResult *getTestFunctionResult(const QString &name);
    void setResultFileInformation(const QString &filename, ResultFormat format=Plain);

private:
    void writeEntriesToFile(const QString &filename, ResultFormat format);

private:
    Q_DISABLE_COPY(ResultLogger)
    QList<TestFunctionResult*> m_testFunctions;
    QString m_fileName;
    ResultFormat m_format;
};

#endif // __FILELOGGER_H__
