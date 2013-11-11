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

#include "filllists.h"
#include "itemrecyclinglist.h"
#include "simplelist.h"
#include "theme.h"

void fillRecyclingList(DummyDataGenerator &dataGenerator, int itemCount, ItemRecyclingList *&list)
{
    for (int i=0; i<itemCount; ++i) {
        list->addItem(newRecyclingListItem(dataGenerator, i));
    }
}

void fillSimpleList(DummyDataGenerator &dataGenerator, int itemCount, SimpleList *&list)
{
    for (int i=0; i<itemCount; ++i) {
        list->addItem(newSimpleListItem(dataGenerator, i));
    }
}

ListItem *newSimpleListItem(DummyDataGenerator &dataGenerator, const int id)
{
    ListItem *item = new ListItem();
    item->setText(dataGenerator.randomName(), ListItem::FirstPos );
    item->setText(dataGenerator.randomPhoneNumber(QString("%1").arg(id)), ListItem::SecondPos );
    item->setIcon(new IconItem(dataGenerator.randomIconItem(), item), ListItem::LeftIcon );
    item->setIcon(new IconItem(dataGenerator.randomStatusItem(), item), ListItem::RightIcon);
    item->setFont(Theme::p()->font(Theme::ContactName), ListItem::FirstPos);
    item->setFont(Theme::p()->font(Theme::ContactNumber), ListItem::SecondPos);
    item->setBorderPen(Theme::p()->listItemBorderPen());
    item->setRounding(Theme::p()->listItemRounding());
    item->icon(ListItem::LeftIcon)->setRotation(Theme::p()->iconRotation(ListItem::LeftIcon));
    item->icon(ListItem::RightIcon)->setRotation(Theme::p()->iconRotation(ListItem::RightIcon));
    item->icon(ListItem::LeftIcon)->setSmoothTransformationEnabled(Theme::p()->isIconSmoothTransformationEnabled(ListItem::LeftIcon));
    item->icon(ListItem::RightIcon)->setSmoothTransformationEnabled(Theme::p()->isIconSmoothTransformationEnabled(ListItem::RightIcon));
#if (QT_VERSION >= 0x040600)
    item->icon(ListItem::LeftIcon)->setOpacityEffectEnabled(Theme::p()->isIconOpacityEffectEnabled(ListItem::LeftIcon));
    item->icon(ListItem::RightIcon)->setOpacityEffectEnabled(Theme::p()->isIconOpacityEffectEnabled(ListItem::RightIcon));
#endif
    return item;    
}


RecycledListItem *newRecyclingListItem(DummyDataGenerator &dataGenerator, const int id)
{
    RecycledListItem *item = new RecycledListItem();
    item->item()->setText(dataGenerator.randomName(), ListItem::FirstPos );
    item->item()->setText(dataGenerator.randomPhoneNumber(QString("%1").arg(id)), ListItem::SecondPos );
    item->item()->setIcon(new IconItem(dataGenerator.randomIconItem(), item), ListItem::LeftIcon );
    item->item()->setIcon(new IconItem(dataGenerator.randomStatusItem(), item), ListItem::RightIcon);
    item->item()->setFont(Theme::p()->font(Theme::ContactName), ListItem::FirstPos);
    item->item()->setFont(Theme::p()->font(Theme::ContactNumber), ListItem::SecondPos);
    item->item()->setBorderPen(Theme::p()->listItemBorderPen());
    item->item()->setRounding(Theme::p()->listItemRounding());
    item->item()->icon(ListItem::LeftIcon)->setRotation(Theme::p()->iconRotation(ListItem::LeftIcon));
    item->item()->icon(ListItem::RightIcon)->setRotation(Theme::p()->iconRotation(ListItem::RightIcon));
    item->item()->icon(ListItem::LeftIcon)->setSmoothTransformationEnabled(Theme::p()->isIconSmoothTransformationEnabled(ListItem::LeftIcon));
    item->item()->icon(ListItem::RightIcon)->setSmoothTransformationEnabled(Theme::p()->isIconSmoothTransformationEnabled(ListItem::RightIcon));
#if (QT_VERSION >= 0x040600)
    item->item()->icon(ListItem::LeftIcon)->setOpacityEffectEnabled(Theme::p()->isIconOpacityEffectEnabled(ListItem::LeftIcon));
    item->item()->icon(ListItem::RightIcon)->setOpacityEffectEnabled(Theme::p()->isIconOpacityEffectEnabled(ListItem::RightIcon));
#endif
    return item;
}
