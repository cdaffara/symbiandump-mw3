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

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSize>
#include <QString>
#include <QFlags>

class Settings : public QObject {
    Q_OBJECT

public:
    enum Option {
        NoOptions = 0x0,
        UseListItemCache = 0x2,
        UseOpenGL = 0x4,
        OutputFps = 0x5,
        NoResourceUsage = 0x6,
    };
    Q_DECLARE_FLAGS(Options, Option)

    Settings();
    ~Settings();

    const QString &scriptName() const
        { return m_scriptName; }
    void setScriptName(const QString& scriptName)
        { m_scriptName = scriptName; }

    const QString &outputFileName() const
        { return m_outputFileName; }
    void setOutputFileName(const QString& outputFileName)
        { m_outputFileName = outputFileName; }

    int resultFormat() const
        { return m_resultFormat; }
    void setResultFormat(int resultFormat) 
        { m_resultFormat = resultFormat; }

    const QSize& size() const
        { return m_size; }
    void setSize(const QSize& size)
        { m_size = size; }

    int angle() const
        { return m_angle; }
    void setAngle(int angle) 
        { m_angle = angle; }

    const Options& options() const
        { return m_options; }
    void setOptions(Options options)
        { m_options = options; }

    int listItemCount()
        { return m_listItemCount; }

    void setListItemCount(int items)
        { m_listItemCount = items; }

private:

    QString m_scriptName;
    QString m_outputFileName;
    int m_resultFormat;
    QSize m_size;
    int m_angle;    
    int m_listItemCount;
    Options m_options;    
};

Q_DECLARE_OPERATORS_FOR_FLAGS(Settings::Options)

#endif
