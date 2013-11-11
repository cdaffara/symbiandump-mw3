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

#include "benchmark.h"

Benchmark::Benchmark()
    : QObject(),
    m_tag(""),
    m_value(0),
    m_listType(ListTypeNone),
    m_listSize(0),
    m_theme(""),
    m_imageBasedRendering(false),
    m_useListItemCache(false),
    m_width(0),
    m_height(0),
    m_angle(0)
{
}

Benchmark::~Benchmark()
{
}

void Benchmark::setTag(const QString &tag)
{
    m_tag = tag;
}
QString Benchmark::tag() const
{
    return m_tag;
}
void Benchmark::setValue(const qreal value)
{
    m_value = value;
}
qreal Benchmark::value() const
{
    return m_value;
}

int Benchmark::listSize() const
{
    return m_listSize;
}

void Benchmark::setListSize(const int size)
{
    m_listSize = size;
}

QString Benchmark::theme() const
{
    return m_theme;
}

void Benchmark::setTheme(const QString &theme)
{
    m_theme = theme;
}

bool Benchmark::imageBasedRendering() const
{
    return m_imageBasedRendering;
}

void Benchmark::setImageBasedRendering(bool imageBasedRendering)
{
    m_imageBasedRendering = imageBasedRendering;
}

bool Benchmark::useListItemCache() const
{
    return m_useListItemCache;
}

void Benchmark::setUseListItemCache(bool useListItemCache)
{
    m_useListItemCache = useListItemCache;
}

QString Benchmark::listTypeStr()
{
    if (m_listType==RecyclingListType)
        return QString("ItemRecyclingList");
    else if(m_listType==SimpleListType)
        return QString("SimpleList");
    return QString("Unkown");
}

int Benchmark::listType() const
{
    return int(m_listType);
}

void Benchmark::setListType(const int type)
{
    m_listType = ListType(type);
}

void Benchmark::setRotation(const int angle)
{
    m_angle = angle;
}

int Benchmark::rotation() const
{
    return m_angle;
}

void Benchmark::setListType(const ListType type)
{
    m_listType = type;
}

QString Benchmark::benchmarkStr()
{
    QString s = m_tag + QString(" [Type=%1, List size=%2, Theme=%3, ListItemCache=%4, Width=%5, Height=%6, Angle=%7").arg(listTypeStr()).arg(m_listSize).arg(m_theme).arg(m_useListItemCache).arg(m_width).arg(m_height).arg(m_angle);
    if (imageBasedRendering())
        s = s + QString(", Render to non displayed pixmap");
    else
        s = s + QString(", Render to screen");
    s = s + "]";
    return s;
}

int Benchmark::width() const
{
    return m_width;
}

void Benchmark::setWidth(const int size)
{
    m_width = size;
}

int Benchmark::height() const
{
    return m_height;
}

void Benchmark::setHeight(const int size)
{
    m_height = size;
}

