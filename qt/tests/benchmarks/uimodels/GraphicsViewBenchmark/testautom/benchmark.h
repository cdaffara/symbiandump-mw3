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

#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <QObject>

class Benchmark : public QObject
{
    Q_OBJECT
public:
    enum ListType {
        ListTypeNone = -1,
        SimpleListType = 0,
        RecyclingListType = 1
    };

    Benchmark();
    ~Benchmark();

public slots:
    QString benchmarkStr();

    void setTag(const QString &tag);
    QString tag() const;

    void setValue(const qreal value);
    qreal value() const;

    QString listTypeStr();
    int listType() const;
    void setListType(const int type);
    void setListType(const ListType type);

    int listSize() const;
    void setListSize(const int size);

    QString theme() const;
    void setTheme(const QString &theme);

    bool imageBasedRendering() const;
    void setImageBasedRendering(bool imageBasedRendering);

    bool useListItemCache() const;
    void setUseListItemCache(bool useListItemCache);

    void setWidth(const int);
    int width() const;

    void setHeight(const int);
    int height() const;

    void setRotation(const int angle);
    int rotation() const;

private:
    Q_DISABLE_COPY(Benchmark)
    QString m_tag;
    qreal m_value;
    ListType m_listType;
    int m_listSize;
    QString m_theme;
    bool m_imageBasedRendering;
    bool m_useListItemCache;
    int m_width;
    int m_height;
    int m_angle;
};

#endif // BENCHMARK_H
