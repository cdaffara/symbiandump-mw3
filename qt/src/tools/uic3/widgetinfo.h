/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the tools applications of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in the Technology Preview License Agreement accompanying
** this package.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
**
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef WIDGETINFO_H
#define WIDGETINFO_H

#include <QObject>

QT_BEGIN_NAMESPACE

class QString;
struct QMetaObject;
class QMetaEnum;

class WidgetInfo: public QObject
{
protected:
    WidgetInfo();

public:
    static bool isValidProperty(const QString &className, const QString &name);
    static bool isValidEnumerator(const QString &className, const QString &name);
    static bool isValidSignal(const QString &className, const QString &name);
    static bool isValidSlot(const QString &className, const QString &name);

    static QString resolveEnumerator(const QString &className, const QString &name);

private:
    static const QMetaObject *metaObject(const QString &widgetName);
    static bool checkEnumerator(const QMetaObject *meta, const QString &name);
    static bool checkEnumerator(const QMetaEnum &metaEnum, const QString &name);

    static QString resolveEnumerator(const QMetaObject *meta, const QString &name);
    static QString resolveEnumerator(const QMetaEnum &metaEnum, const QString &name);
};

QT_END_NAMESPACE

#endif // WIDGETINFO_H