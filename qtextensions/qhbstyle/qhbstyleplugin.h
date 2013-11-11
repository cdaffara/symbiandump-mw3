/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation, version 2.1 of the License.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this program.  If not,
* see "http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html/".
*
* Description:
*
*/
#ifndef QHBSTYLEPLUGIN_H
#define QHBSTYLEPLUGIN_H

#include <QStylePlugin>

class QStringList;
class QStyle;

class QHbStylePlugin : public QStylePlugin
 {

    Q_OBJECT

    public:
        QHbStylePlugin() {};

        QStringList keys() const;
        QStyle *create(const QString &key);
 };

#endif //QHBSTYLEPLUGIN_H
