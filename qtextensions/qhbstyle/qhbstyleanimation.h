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
#ifndef QHBSTYLEANIMATION_H
#define QHBSTYLEANIMATION_H

#include <QObject>
#include <qstyle.h>
#include <qnamespace.h>

class QPixmap;
class QPainter;

class QHbStyleAnimation : public QObject
{
Q_OBJECT

Q_PROPERTY(QPoint point READ point WRITE setPoint)

public:
    QHbStyleAnimation(QWidget* target, QObject *parent = 0);
    ~QHbStyleAnimation();

    const QWidget* target() const;

    QPoint point();
    void setPoint(const QPoint& rect);

    void createAnimationIcon(QStyle::ControlElement element, Qt::Orientations orientation);
    void paintAnimation(QPainter *painter);

private:
    QScopedPointer<QPixmap> m_animationIcon;
    QScopedPointer<QWidget> m_target;
    QPoint m_point;
    QScopedPointer<QPixmap> m_mask;
};

#endif // QHBSTYLEANIMATION_H
