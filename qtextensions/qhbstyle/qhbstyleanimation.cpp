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
#include "qhbstyleanimation.h"
#include <qpixmap.h>
#include <qwidget.h>
#include <qpainter.h>
#include <hbicon.h>
#include <qbitmap.h>

#include "qhbstyle.h"
#include "hbframedrawer.h"


QHbStyleAnimation::QHbStyleAnimation(QWidget* target, QObject *parent) :
    QObject(parent), m_animationIcon(0), m_target(target), m_point(QPoint(0, 0)), m_mask(0)
{
}

QHbStyleAnimation::~QHbStyleAnimation()
{
}

const QWidget* QHbStyleAnimation::target()const
{
    return m_target.data();
}

QPoint QHbStyleAnimation::point()
{
    return m_point;
}

void QHbStyleAnimation::setPoint(const QPoint& point)
{
    m_point = point;
    m_target->update();
}

void QHbStyleAnimation::createAnimationIcon(QStyle::ControlElement element, Qt::Orientations orientation)
{
    //Create mask
    HbFrameDrawer drawer;
    if (orientation == Qt::Horizontal){
        drawer.setFrameGraphicsName("qtg_fr_progbar_h_mask");
        drawer.setFrameType(HbFrameDrawer::ThreePiecesHorizontal);
    }
    else {
        drawer.setFrameGraphicsName("qtg_fr_progbar_v_mask");
        drawer.setFrameType(HbFrameDrawer::ThreePiecesVertical);
    }
    drawer.setFillWholeRect(true);
    m_mask.reset(new QPixmap(m_target->rect().size()));
    m_mask->fill(Qt::transparent);
    QPainter p(m_mask.data());
    drawer.paint(&p, m_target->rect());
    p.end();

    //Create animated icon
    QString iconName;
    switch (element) {
        case QStyle::CE_ProgressBarContents: {
             if (orientation == Qt::Horizontal)
                iconName = QLatin1String("qtg_graf_progbar_h_wait");
             else
                iconName = QLatin1String("qtg_graf_progbar_v_wait");
            break;
        }
        default:
            break;
    }

    if (!iconName.isNull() && !m_target->rect().isEmpty()) {
        HbIcon* icon = new HbIcon(iconName);
        if(orientation == Qt::Horizontal)
            icon->setSize(QSize(icon->width(), m_target->rect().height()));
        else
            icon->setSize(QSize(m_target->rect().width(), icon->height()));

        const qreal rectWidth = m_target->rect().width();
        const qreal iconWidth = icon->width();
        const qreal rectHeight = m_target->rect().height();
        const qreal iconHeight = icon->height();

        const int animationWidth = (orientation == Qt::Horizontal) ?  int(rectWidth + iconWidth) : int(rectWidth);
        const int animationHeight = (orientation == Qt::Horizontal) ?  int(rectHeight) : int(rectHeight + iconHeight);

        m_animationIcon.reset(new QPixmap(animationWidth, animationHeight));
        m_animationIcon->fill(Qt::transparent);
        QPainter p(m_animationIcon.data());

        if (orientation == Qt::Horizontal) {
            if (iconWidth > 0)
                for (qreal i = 0 ; i < (rectWidth + iconWidth); i += iconWidth)
                    icon->paint(&p, QRectF(i, 0, iconWidth, iconHeight), Qt::IgnoreAspectRatio, Qt::AlignCenter, QIcon::Normal, QIcon::On);
        } else {
            if (iconHeight > 0)
                for(qreal i = 0 ; i < (rectHeight + iconHeight) ; i += iconHeight)
                    icon->paint(&p, QRectF(0, i, iconWidth, iconHeight), Qt::IgnoreAspectRatio, Qt::AlignCenter, QIcon::Normal, QIcon::On);
        }
        p.end();
    }
}

void QHbStyleAnimation::paintAnimation(QPainter *painter)
{
    Q_ASSERT(m_animationIcon);
    Q_ASSERT(painter);

    //Take part from animation icon
    QPixmap icon(m_target->rect().size());
    icon.fill(Qt::transparent);
    QPainter p(&icon);
    p.setCompositionMode(QPainter::CompositionMode_SourceOver);
    p.drawPixmap(QPointF(0, 0), *m_mask);
    p.setCompositionMode(QPainter::CompositionMode_SourceIn);
    p.drawPixmap(m_point, *m_animationIcon, QRect(0, 0, m_target->rect().width() + m_point.rx() * -1, m_target->rect().height() + m_point.ry() * -1));
    p.end();

    //paint animation
    painter->drawPixmap(QPointF(0, 0), icon);
}
