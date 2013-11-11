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
//Qt includes
#include <qapplication.h>
#include <qpainter.h>
#include <qstyleoption.h>
#include <qevent.h>
#include <qdebug.h>

//Qt widgets
#include <QtGui/qgroupbox.h>
#include <QtGui/qheaderview.h>
#include <QtGui/qlistview.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qscrollbar.h>
#include <QtGui/qtabbar.h>
#include <QtGui/qtableview.h>
#include <QtGui/qtreeview.h>
#include <QtGui/qtextedit.h>
#include <QtGui/qtoolbar.h>
#include <QtGui/qtoolbutton.h>
#include <QtGui/qradiobutton.h>
#include <QtGui/qcheckbox.h>
#include <QtGui/qprogressbar.h>
#include <QtGui/qcombobox.h>
#include <QtGui/qspinbox.h>
#include <QtGui/qlineedit.h>

//Animation
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>
#include <QTime>

//Hb includes
#include <hbinstance.h>
#include <hbicon.h>
#include <hbframedrawer.h>
#include <hbstyle.h>
#include <hbcolorscheme.h>
#include <hbfontspec.h>

#include "qhbstyle.h"
#include "qhbstyle_p.h"
#include "qhbstyleanimation.h"

QT_BEGIN_NAMESPACE

QHbStylePrivate::QHbStylePrivate() : m_styleManager(0),
    m_frameDrawer(0),
    m_animationGroup(0)
{
}

QHbStylePrivate::~QHbStylePrivate()
{
}

HbStyle* QHbStylePrivate::styleManager()
{
    if (!m_styleManager) {
        HbInstance *instance = HbInstance::instance();
        setStyleManager(instance->style());
    }
    
    return m_styleManager;
}

void QHbStylePrivate::setStyleManager(HbStyle* style)
{
    Q_ASSERT(style);
    m_styleManager = style;
}

QParallelAnimationGroup* QHbStylePrivate::animationGroup()
{
    if (m_animationGroup.isNull())
        m_animationGroup.reset(new QParallelAnimationGroup());
    return m_animationGroup.data();
}

/*!
  \internal
 */
QHbStyle::QHbStyle() : QCommonStyle()
{
    m_private = new QHbStylePrivate();
}

/*!
  \internal
 */
QHbStyle::~QHbStyle()
{
    delete m_private;
}

void QHbStyle::drawPrimitive(PrimitiveElement element, const QStyleOption *option,
                       QPainter *painter, const QWidget *widget) const
{
    switch (element) {
        case PE_IndicatorViewItemCheck: {
            if (const QStyleOptionViewItemV4 *itemOption = qstyleoption_cast<const QStyleOptionViewItemV4 *>(option)) {
                ItemStates state;
                if (itemOption->state & State_Selected)
                    state |= SS_Selected;
                m_private->drawItem(SP_ItemDecoration, painter, option->rect, state);
            }
            break;
        }
        case PE_IndicatorHeaderArrow: {
            if (const QStyleOptionHeader *header = qstyleoption_cast<const QStyleOptionHeader *>(option)) {
                ItemStates state;
                if (header->sortIndicator & QStyleOptionHeader::SortDown)
                    m_private->drawItem(SP_HeaderOrderIndicator, painter, header->rect, ItemStates(state|SS_Flipped));
                else if (header->sortIndicator & QStyleOptionHeader::SortUp)
                    m_private->drawItem(SP_HeaderOrderIndicator, painter, header->rect, ItemStates(state));
            }
            break;
        }
        case PE_IndicatorBranch: {
            if (option->state & State_Children) {
                QRect indicatorRect = option->rect;
                const int rectSide = proxy()->pixelMetric(PM_MenuButtonIndicator, option, widget);
                indicatorRect = QRect(0, 0, rectSide, rectSide);
                indicatorRect.moveCenter(option->rect.center());
                if (option->state & State_Open)
                    m_private->drawItem(SP_TreeViewExpanded, painter, indicatorRect);
                else
                    m_private->drawItem(SP_TreeViewCollapsed, painter, indicatorRect);
            }
            break;
        }
        case PE_PanelItemViewRow: {
            if (const QStyleOptionViewItemV4 *vopt = qstyleoption_cast<const QStyleOptionViewItemV4 *>(option)) {
            ItemStates state = SS_Active;
            if (vopt->state & State_HasFocus)
                state |= SS_Focused;
            if (vopt->state & State_Sunken || vopt->state & State_Raised)
                state |= SS_Pressed;

#ifndef QT_NO_TABLEVIEW
                if (qobject_cast<const QTableView *>(widget)) {
                    m_private->drawMultiPartItem(SM_TableItem, painter, vopt->rect, state);
                    break;
                }
#endif
            if (vopt->features & QStyleOptionViewItemV2::Alternate)
                state |= SS_Alternate; //@todo: how?
            m_private->drawMultiPartItem(SM_ItemViewItem, painter, vopt->rect, state);
            }
            break;
        }
        case PE_PanelItemViewItem: {
            if (const QStyleOptionViewItemV4 *vopt = qstyleoption_cast<const QStyleOptionViewItemV4 *>(option)) {
                ItemStates state = SS_Active;
                if (vopt->state & State_HasFocus)
                    state |= SS_Focused;
                if (vopt->state & State_Sunken || vopt->state & State_Raised)
                    state |= SS_Pressed;
                m_private->drawMultiPartItem(SM_ItemViewItem, painter, vopt->rect, state);
            }
            break;
        }
        case PE_IndicatorArrowLeft:
        case PE_IndicatorArrowRight:
        case PE_IndicatorArrowUp:
        case PE_IndicatorArrowDown: {
            ItemStates state;
            if (element == PE_IndicatorArrowRight)
                state = SS_Right;
            else if (element == PE_IndicatorArrowLeft)
                state = SS_Left;
            else if (element == PE_IndicatorArrowUp)
                state = SS_Up;
            else
                state = SS_Down;
            m_private->drawItem(SP_Arrow, painter, option->rect, state);
            break;
        }
        case PE_PanelTipLabel: {
            m_private->drawMultiPartItem(SM_ToolTip, painter, option->rect);
            break;
        }
        case PE_Frame: {
            if (const QStyleOptionFrameV3 *frame = qstyleoption_cast<const QStyleOptionFrameV3 *>(option)) {
#ifndef QT_NO_TEXTEDIT
                if (qobject_cast<const QTextEdit *>(widget))
                    m_private->drawMultiPartItem(SM_TextEdit, painter, frame->rect);
#endif //QT_NO_TEXTEDIT
            }
            break;
        }
        case PE_FrameTabWidget: {
            m_private->drawMultiPartItem(SM_Panel, painter, option->rect);
            break;
        }
#ifndef QT_NO_LINEEDIT
        case PE_PanelLineEdit: {
#ifndef QT_NO_COMBOBOX
            if ( (widget && qobject_cast<const QComboBox *>(widget->parentWidget()) ) && (!widget->hasFocus()))
                break;
#endif
#ifndef QT_NO_SPINBOX
            if (widget && qobject_cast<const QSpinBox *>(widget->parentWidget()))
                break;
#endif
            if (const QStyleOptionFrame *lineEdit = qstyleoption_cast<const QStyleOptionFrame *>(option)) {
                ItemStates state = (lineEdit->state & State_Enabled) ? SS_Active : SS_Inactive;
                if (lineEdit->state & State_HasFocus)
                    state |= SS_Selected;
                m_private->drawMultiPartItem(SM_LineEdit, painter, lineEdit->rect, state);
            }
            break;
        }
#endif // QT_NO_LINEEDIT
        case PE_PanelButtonTool: {
            if (const QStyleOptionToolButton *toolbutton = qstyleoption_cast<const QStyleOptionToolButton *>(option)) {
                //draw button
                const bool isDisabled = !(toolbutton->state & State_Enabled);
                const bool isPressed = (toolbutton->state & State_Sunken) ||
                                       (toolbutton->state & State_On);

                // 'latched' is a checkable button that is pressed down
                bool isLatched = false;
#ifndef QT_NO_TOOLBUTTON
                if (const QToolButton *toolButtonWidget = qobject_cast<const QToolButton*>(widget))
                    isLatched = toolButtonWidget->isCheckable() && isPressed;
#endif
                ItemStates state = (isDisabled) ? SS_Disabled : SS_Active;
                if (isLatched)
                    state = state | SS_Latched;
                else if (isPressed)
                    state = state | SS_Pressed;

                if (toolbutton->state & State_Selected || toolbutton->state & State_HasFocus)
                    state = state | SS_Selected;
#ifndef QT_NO_TOOLBAR
                if (widget && !qobject_cast<const QToolBar*>(widget->parentWidget()))
                    m_private->drawMultiPartItem(SM_ToolButton, painter, toolbutton->rect, state);
                else
#endif
                    m_private->drawMultiPartItem(SM_ToolBarButton, painter, toolbutton->rect, state);
            }
            break;
        }
        case PE_IndicatorCheckBox: {
            ItemStates state = (option->state & State_On) ? SS_Active : SS_Inactive;
            if (option->direction == Qt::RightToLeft) state |= SS_Mirrored;
            m_private->drawItem(SP_CheckBoxIndicator, painter, option->rect, state);
            break;
        }
        case PE_IndicatorRadioButton: {
            const ItemStates state = (option->state & State_On) ? SS_Active : SS_Inactive;
            m_private->drawItem(SP_RadioButtonIndicator, painter, option->rect, state);
            break;
        }
        case PE_FrameFocusRect: {
            if (const QStyleOptionFocusRect *highlight = qstyleoption_cast<const QStyleOptionFocusRect *>(option)) {
                if (false
#ifndef QT_NO_LISTVIEW
                        || qobject_cast<const QListView *>(widget)
#endif
#ifndef QT_NO_TABLEVIEW
                        || qobject_cast<const QTableView *>(widget)
#endif
#ifndef QT_NO_TREEVIEW
                        || qobject_cast<const QTreeView *>(widget)
#endif
                    )
                    if (option->state & State_HasFocus)
                        m_private->drawMultiPartItem(SM_ItemViewItem, painter, highlight->rect, SS_Focused);
            }
            break;
        }
        case PE_FrameMenu: {
            break;
        }
        case PE_PanelMenu: {
            m_private->drawMultiPartItem(SM_Menu, painter, option->rect);
            break;
        }
        case PE_Widget: {
            if (m_private->isDialog(widget))
                m_private->drawMultiPartItem(SM_Dialog, painter, option->rect);
            break;
        }
        case PE_IndicatorMenuCheckMark: {
            m_private->drawItem(SP_ItemDecoration, painter, option->rect);
            break;
        }
        case PE_FrameGroupBox: {
            ItemStates groupBoxStates;
            if ((option->state & State_Sunken) || (option->state & State_Raised))
                groupBoxStates |= SS_Pressed;
            if (option->state & State_HasFocus)
                groupBoxStates |= SS_Selected;
            if (option->state & State_On)
                groupBoxStates |= SS_Active;
            else if (option->state & State_Off)
                groupBoxStates |= SS_Inactive;
            m_private->drawMultiPartItem(SM_GroupBox, painter, option->rect, groupBoxStates);
            break;
        }
        // Qt3 primitives are not supported
        case PE_Q3CheckListController:
        case PE_Q3CheckListExclusiveIndicator:
        case PE_Q3CheckListIndicator:
        case PE_Q3DockWindowSeparator:
        case PE_Q3Separator: {
            Q_ASSERT(false);
            break;
        }
        case PE_PanelScrollAreaCorner: //no corner for scroll area
        case PE_IndicatorTabTear: // no tab tear in uiemo
        case PE_PanelMenuBar: { //no panel menu in uiemo
            break;
        }
        default: {
            QCommonStyle::drawPrimitive(element, option, painter, widget);
            break;
        }
    }
}

void QHbStyle::drawControl(ControlElement element, const QStyleOption *option,
                     QPainter *painter, const QWidget *widget) const
{
    switch (element) {
        case CE_HeaderEmptyArea: {
            const bool isHorizontal = (option->state & State_Horizontal);
            ItemStates states = (isHorizontal) ? SS_Horizontal : ItemState(SS_Vertical);
            if (!isHorizontal)
                states |= (option->direction == Qt::LeftToRight) ? SS_RotatedRight : SS_RotatedLeft;
            m_private->drawMultiPartItem(SM_ListParent, painter, option->rect, states);
            break;
        }
        case CE_HeaderSection: {
            if (const QStyleOptionHeader *header = qstyleoption_cast<const QStyleOptionHeader *>(option)) {
                //Draw corner button as normal pushButton.
                if (qobject_cast<const QAbstractButton *>(widget)) {
                    QStyleOptionButton cornerButton;
                    cornerButton.initFrom(widget);
                    drawControl(CE_PushButtonBevel, &cornerButton, painter, widget);
                } else {
                    const bool isVertical = (header->orientation == Qt::Vertical);
                    ItemStates states = (isVertical) ? SS_Vertical : ItemState(SS_Horizontal);
                    if (isVertical)
                        states |= (header->direction == Qt::LeftToRight) ? SS_RotatedRight : SS_RotatedLeft;
                    m_private->drawMultiPartItem(SM_ListParent, painter, option->rect, states);
                }
            }
            break;
        }
        case CE_ItemViewItem: {
        //@todo: headerviews and listviews should show selection tick at the beginning of the row (in place of checkbox rect)
        //@todo: headerview should select also parent when child is selected
        //@todo: headerview should draw highlight rect
            if (const QStyleOptionViewItemV4 *itemOption = qstyleoption_cast<const QStyleOptionViewItemV4 *>(option)) {
                const QRect checkRect = subElementRect(SE_ItemViewItemCheckIndicator, itemOption, widget);
                const QRect iconRect = subElementRect(SE_ItemViewItemDecoration, itemOption, widget);
                QRect textRect = subElementRect(SE_ItemViewItemText, itemOption, widget);

                //background for list items (other itemviews use PE_PanelItemViewRow drawing)
                if (qobject_cast<const QListView *>(widget))
                    proxy()->drawPrimitive(PE_PanelItemViewItem, itemOption, painter, widget);

                //checkbox
                if (itemOption->features & QStyleOptionViewItemV2::HasCheckIndicator && checkRect.isValid()) {
                    QStyleOptionViewItemV4 checkOption;
                    checkOption.QStyleOption::operator=(*itemOption);
                    checkOption.rect = checkRect;
                    proxy()->drawPrimitive(PE_IndicatorViewItemCheck, &checkOption, painter, widget);
                }

                //selection indication
                if (itemOption->state & State_Selected) {
                    const QAbstractItemView *itemView = qobject_cast<const QAbstractItemView *>(widget);
                    if (itemView->selectionMode() != QAbstractItemView::NoSelection) {
                        QStyleOptionViewItemV4 selectOption;
                        selectOption.QStyleOption::operator=(*itemOption);
                        int iconSize = 0;
                        if (m_private->hbParameter(QLatin1String("hb-param-graphic-size-secondary"), iconSize)) {
                            QRect selectRect = QRect(0, 0, iconSize, iconSize);
                            if (itemOption->direction == Qt::LeftToRight) {
                                //translate to end of text area and reduce text area
                                selectRect.translate(textRect.topRight().x() - selectRect.width(), textRect.topRight().y());
                            } else {
                                //translate to the beginning of textRect, move textRect to the right
                                selectRect.translate(textRect.topLeft().x(), textRect.topRight().y());
                                textRect.translate(selectRect.width(), 0);
                            }
                            textRect.setWidth(textRect.width() - selectRect.width());
                            selectOption.rect = selectRect;
                            proxy()->drawPrimitive(PE_IndicatorViewItemCheck, &selectOption, painter, widget);
                        }
                    }
                }

                //text
                if (itemOption->text.length() > 0) {
                    uint flags = Qt::AlignVCenter | Qt::TextShowMnemonic;
                    if (!proxy()->styleHint(SH_UnderlineShortcut, itemOption, widget))
                        flags |= Qt::TextHideMnemonic;

                    drawItemText(painter, textRect, flags, itemOption->palette, (itemOption->state & State_Enabled), itemOption->text);
                }
                //icon
                if (!itemOption->icon.isNull()) {
                    QIcon::Mode mode = QIcon::Normal;
                    if (!(option->state & State_Enabled))
                        mode = QIcon::Disabled;
                    else if (option->state & State_Selected)
                        mode = QIcon::Selected;
                    QIcon::State state = itemOption->state & State_Open ? QIcon::On : QIcon::Off;
                    itemOption->icon.paint(painter, iconRect, itemOption->decorationAlignment, mode, state);
                }
            }
            break;
        }
        case CE_ShapedFrame: {
            if (const QStyleOptionFrameV3 *frame = qstyleoption_cast<const QStyleOptionFrameV3 *>(option)) {
                const int frameShape  = frame->frameShape;
                const int lineWidth = frame->lineWidth;
                const int midLineWidth = frame->midLineWidth;
                QPalette::ColorRole foregroundRole = QPalette::WindowText;
                int frameShadow = QFrame::Plain;
                if (frame->state & State_Sunken)
                    frameShadow = QFrame::Sunken;
                else if (frame->state & State_Raised)
                    frameShadow = QFrame::Raised;

                switch (frameShape) {
                case QFrame::Box:
                case QFrame::WinPanel:
                    if (frameShadow == QFrame::Plain)
                        qDrawPlainRect(painter, frame->rect, frame->palette.color(foregroundRole), lineWidth);
                    else
                        qDrawShadeRect(painter, frame->rect, frame->palette, frameShadow == QFrame::Sunken, lineWidth, midLineWidth);
                    break;
                case QFrame::StyledPanel:
                    if (widget)
                        widget->style()->drawPrimitive(PE_Frame, option, painter, widget);
                    else
                        proxy()->drawPrimitive(PE_Frame, option, painter, widget);
                    break;
                case QFrame::Panel:
                    //@todo: support sunken / raised?
                    m_private->drawMultiPartItem(SM_Panel, painter, option->rect);
                    break;
                case QFrame::HLine:
                case QFrame::VLine: {
                    ItemStates states = (frameShape == QFrame::HLine) ? SS_Horizontal : SS_Vertical;
                    //@todo: support sunken / raised separators?
                    m_private->drawItem(SP_SeparatorLine, painter, frame->rect, states);
                    break;
                    }
                }
            }
            break;
        }
#ifndef QT_NO_TABBAR
        case CE_TabBarTab: {
            if (const QStyleOptionTab *tab = qstyleoption_cast<const QStyleOptionTab *>(option)) {
                proxy()->drawControl(CE_TabBarTabShape, tab, painter, widget);
                proxy()->drawControl(CE_TabBarTabLabel, tab, painter, widget);
            }
            break;
        }
        case CE_TabBarTabShape: {
            if (const QStyleOptionTab *tab = qstyleoption_cast<const QStyleOptionTab *>(option)) {
                ItemStates states = (tab->shape == QTabBar::TriangularSouth ||
                                     tab->shape == QTabBar::RoundedSouth ||
                                     tab->shape == QTabBar::TriangularNorth ||
                                     tab->shape == QTabBar::RoundedNorth) ? SS_Horizontal : SS_Vertical;
                if (tab->state & State_Selected)
                    states |= SS_Selected;
                if (tab->state & State_Raised || tab->state & State_Sunken)
                    states |= SS_Pressed;
                if (!(tab->state & State_Enabled))
                    states |= SS_Disabled;

                if (tab->shape == QTabBar::RoundedSouth || tab->shape == QTabBar::TriangularSouth ||
                    tab->shape == QTabBar::RoundedEast || tab->shape == QTabBar::TriangularEast)
                    states |= SS_Flipped;

                if (tab->direction == Qt::RightToLeft) states |= SS_Mirrored;

                //Tab's position
                if (tab->position == QStyleOptionTab::Beginning)
                    states |= (states & SS_Flipped) ? SS_End : SS_Beginning;
                else if (tab->position == QStyleOptionTab::Middle)
                    states |= SS_Middle;
                if (tab->position == QStyleOptionTab::End)
                    states |= (states & SS_Flipped) ? SS_Beginning : SS_End;

                m_private->drawMultiPartItem(SM_TabShape, painter, tab->rect, states);
            }
            break;
        }
        case CE_TabBarTabLabel: {
            if (const QStyleOptionTabV3 *tab = qstyleoption_cast<const QStyleOptionTabV3 *>(option)) {
                const bool enabled = tab->state & State_Enabled;
                const QPixmap icon = tab->icon.pixmap(proxy()->pixelMetric(PM_TabBarIconSize, tab, widget),
                                             enabled ? QIcon::Normal : QIcon::Disabled);

                const bool verticalTabs = tab->shape == QTabBar::RoundedEast
                                    || tab->shape == QTabBar::RoundedWest
                                    || tab->shape == QTabBar::TriangularEast
                                    || tab->shape == QTabBar::TriangularWest;

                QRect tr = tab->rect;
                //add a small space so that text/icon does not start from the border
                const int margin = proxy()->pixelMetric(PM_DefaultFrameWidth, tab, widget);
                if (!verticalTabs)
                    tr.adjust(margin, 0, -margin, 0);
                else
                    tr.adjust(0, margin, 0, -margin);

                // Need to do rotation separately here, instead of in drawItem/drawMultiItemPart, since we want to rotate text as well.
                if (verticalTabs) {
                    painter->save();
                    int newX, newY, newRotation;
                    if (tab->shape == QTabBar::RoundedEast || tab->shape == QTabBar::TriangularEast) {
                        newX = tr.width();
                        newY = tr.y();
                        newRotation = 90;
                    } else {
                        newX = 0;
                        newY = tr.y() + tr.height();
                        newRotation = -90;
                    }
                    tr.setRect(0, 0, tr.height(), tr.width());
                    QTransform m;
                    m.translate(newX, newY);
                    m.rotate(newRotation);
                    painter->setTransform(m, true);
                }

                const int frameWidth = proxy()->pixelMetric((verticalTabs) ?
                    PM_LayoutVerticalSpacing : PM_LayoutHorizontalSpacing, option, widget);
                const Qt::TextElideMode elideMode = (tab->direction == Qt::LeftToRight) ? Qt::ElideRight : Qt::ElideLeft;
                const QRect textRect = QRect(0,
                                             0,
                                             tab->rect.width() - icon.width() - frameWidth * 2,
                                             tab->rect.height() - icon.height() - frameWidth * 2);
                QString txt = tab->fontMetrics.elidedText(tab->text, elideMode, (verticalTabs ? textRect.height() : textRect.width()));

                //Icon
                if (!icon.isNull()) {
                    if (tab->text.isEmpty())
                        painter->drawPixmap(tr.center().x() - (icon.height() >> 1),
                                            tr.center().y() - (icon.height() >> 1),
                                            icon);
                    else
                        painter->drawPixmap(tr.left(),
                                            tr.center().y() - (icon.height() >> 1),
                                            icon);
                    tr.setLeft(tr.left() + icon.width() + frameWidth);
                } else {
                    tr.setLeft(tr.left() + frameWidth);
                }

                //Text
                if (tab->text.length() > 0) {
                    int alignment = Qt::AlignLeft | Qt::AlignVCenter | Qt::TextShowMnemonic;
                    if (!proxy()->styleHint(SH_UnderlineShortcut, tab, widget))
                        alignment |= Qt::TextHideMnemonic;
                    proxy()->drawItemText(painter, tr, alignment, tab->palette, enabled, txt, QPalette::ButtonText);
                }

                if (verticalTabs)
                    painter->restore();
            }
            break;
        }
#ifndef QT_NO_COMBOBOX
    case CE_ComboBoxLabel:
        if (const QStyleOptionComboBox *combo = qstyleoption_cast<const QStyleOptionComboBox *>(option)) {
            QRect editRect = proxy()->subControlRect(CC_ComboBox, combo, SC_ComboBoxEditField, widget);
            const int spacing = proxy()->pixelMetric(PM_LayoutHorizontalSpacing, combo, widget);
            if (!combo->currentIcon.isNull()) {
                const QIcon::Mode mode = combo->state & State_Enabled ? QIcon::Normal
                                                             : QIcon::Disabled;
                const QPixmap pixmap = combo->currentIcon.pixmap(combo->iconSize, mode);
                QRect iconRect(editRect);
                iconRect.setWidth(combo->iconSize.width() + spacing);
                iconRect = alignedRect(combo->direction,
                                       Qt::AlignLeft | Qt::AlignVCenter,
                                       iconRect.size(), editRect);
                if (combo->editable)
                    painter->fillRect(iconRect, combo->palette.brush(QPalette::Base));
                proxy()->drawItemPixmap(painter, iconRect, Qt::AlignCenter, pixmap);

                if (combo->direction == Qt::RightToLeft)
                    editRect.translate(-spacing - combo->iconSize.width(), 0);
                else
                    editRect.translate(combo->iconSize.width() + spacing, 0);
            }
            if (!combo->currentText.isEmpty() && !combo->editable) {
                const Qt::TextElideMode elideMode = (combo->direction == Qt::LeftToRight) ? Qt::ElideRight : Qt::ElideLeft;
                const QString txt = combo->fontMetrics.elidedText(combo->currentText, elideMode, editRect.width());
                proxy()->drawItemText(painter, editRect.adjusted(1, 0, -1, 0),
                             visualAlignment(combo->direction, Qt::AlignLeft | Qt::AlignVCenter),
                             combo->palette, combo->state & State_Enabled, txt);
            }
        }
        break;
#endif // QT_NO_COMBOBOX
#endif //QT_NO_TABBAR
        case CE_PushButton: {
            if (const QStyleOptionButton *btn = qstyleoption_cast<const QStyleOptionButton *>(option)) {
                proxy()->drawControl(CE_PushButtonBevel, btn, painter, widget);
                QStyleOptionButton subopt = *btn;
                subopt.rect = subElementRect(SE_PushButtonContents, btn, widget);
                proxy()->drawControl(CE_PushButtonLabel, &subopt, painter, widget);
                if ((btn->state & State_HasFocus)) {
                    QStyleOptionFocusRect fropt;
                    fropt.QStyleOption::operator=(*btn);
                    fropt.rect = subElementRect(SE_PushButtonFocusRect, btn, widget);
                    proxy()->drawPrimitive(PE_FrameFocusRect, &fropt, painter, widget);
                }
            }
            break;
        }
        case CE_PushButtonBevel: {
            if (const QStyleOptionButton *button = qstyleoption_cast<const QStyleOptionButton *>(option)) {
                const bool isDisabled = !(button->state & State_Enabled);
                const bool isFlat = button->features & QStyleOptionButton::Flat;
                const bool isPressed = (button->state & State_Sunken) ||
                                       (button->state & State_On);
                // 'latched' is a checkable button that is pressed down
                const QPushButton *pbutton = qobject_cast<const QPushButton *>(widget);
                const bool isLatched = (!pbutton) ? false : (pbutton->isCheckable() && isPressed);

                ItemStates state = (isDisabled) ? SS_Disabled : SS_Active;
                if (isLatched)
                    state = state | SS_Latched;
                else if (isPressed)
                    state = state | SS_Pressed;

                if (button->state & State_Selected || button->state & State_HasFocus)
                    state = state | SS_Selected;

                //todo: does Hb have flat buttons?
                if (button->features & QStyleOptionButton::HasMenu) {
                    //draw menu indicator
                    const int menuButtonIndicator = proxy()->pixelMetric(PM_MenuButtonIndicator, button, widget);
                    QStyleOptionButton menuOpt = *button;
                    menuOpt.rect = QRect(button->rect.right() - menuButtonIndicator,
                                         button->rect.y() + (button->rect.height() - menuButtonIndicator) / 2,
                                         menuButtonIndicator,
                                         menuButtonIndicator);
                    menuOpt.rect = visualRect(button->direction, button->rect, menuOpt.rect);
                    proxy()->drawPrimitive(PE_IndicatorArrowDown, &menuOpt, painter, widget);
                }
                if (isFlat) //lets draw flat buttons as toolbuttons
                    m_private->drawMultiPartItem(SM_ToolButton, painter, button->rect, state);
                else
                    m_private->drawMultiPartItem(SM_PushButton, painter, button->rect, state);
                }
            break;
        }
        case CE_MenuScroller: {
            ItemStates states = (option->state & State_DownArrow) ? SS_Down : SS_Up;
            painter->fillRect(option->rect, option->palette.background());
            m_private->drawMultiPartItem(SM_MenuScroller, painter, option->rect, states);
            QStyleOption arrowOpt = *option;
            arrowOpt.state |= State_Enabled;
            const int side = proxy()->pixelMetric(PM_MenuScrollerHeight, option, widget);
            arrowOpt.rect = option->rect;
            arrowOpt.rect.setWidth(side);
            arrowOpt.rect.moveCenter(option->rect.center());
            proxy()->drawPrimitive(((option->state & State_DownArrow) ? PE_IndicatorArrowDown : PE_IndicatorArrowUp),
                    &arrowOpt, painter, widget);
            break;
        }
        case CE_MenuItem: {
            if (const QStyleOptionMenuItem *menuItem = qstyleoption_cast<const QStyleOptionMenuItem *>(option)) {
                if (menuItem->menuItemType == QStyleOptionMenuItem::Separator) {
                    const int margin = proxy()->pixelMetric(PM_MenuHMargin, menuItem, widget);
                    const int yoff = menuItem->rect.y() - 1 + menuItem->rect.height() / 2;
                    const int startX = menuItem->rect.x() + margin;
                    const int endX = menuItem->rect.x() + menuItem->rect.width() - margin;
                    QRect separatorRect = QRect(QPoint(startX, yoff), QPoint(endX, yoff));
                    m_private->drawItem(SP_MenuSeparator, painter, separatorRect);
                    return;
                }

                const bool isDisabled = !(menuItem->state & State_Enabled);
                const bool isSelected = (menuItem->state & State_Selected);
                const bool isPressed = (menuItem->state & State_Sunken) || (menuItem->state & State_Raised);
                ItemStates state = (isDisabled) ? SS_Disabled : SS_Active;

                if (isSelected)
                    state = state | SS_Selected;

                if (isPressed)
                    state = state | SS_Pressed;

                m_private->drawMultiPartItem(SM_MenuItem, painter, menuItem->rect, state);

                uint text_flags = Qt::AlignLeading | Qt::TextShowMnemonic | Qt::TextDontClip
                    | Qt::TextSingleLine | Qt::AlignVCenter;
                if (!styleHint(SH_UnderlineShortcut, menuItem, widget))
                    text_flags |= Qt::TextHideMnemonic;

                if (menuItem->menuHasCheckableItems) {
                    const QRect checkBoxRect = subElementRect(SE_ViewItemCheckIndicator, menuItem, widget);
                    if (checkBoxRect.isValid()) {
                        ItemStates checkBoxState;
                        if (menuItem->checked) checkBoxState |= SS_Selected;
                        if (menuItem->direction == Qt::RightToLeft) checkBoxState |= SS_Mirrored;
                        m_private->drawItem(SP_ItemDecoration, painter, checkBoxRect, checkBoxState);
                    }
                }

                if (!menuItem->icon.isNull()) {
                    const QRect iconRect = subElementRect(SE_ItemViewItemDecoration, menuItem, widget);
                    if (iconRect.isValid()) {
                        QPixmap pix = menuItem->icon.pixmap(pixelMetric(PM_SmallIconSize),
                                !isDisabled ? QIcon::Normal : QIcon::Disabled);
                        drawItemPixmap(painter, iconRect, text_flags, pix);
                    }
                }

                if (menuItem->text.length() > 0) {
                    const QRect textRect = subElementRect(SE_ItemViewItemText, menuItem, widget);
                    if (textRect.isValid())
                        QCommonStyle::drawItemText(painter, textRect, text_flags, menuItem->palette,
                            (menuItem->state & State_Enabled), menuItem->text, QPalette::Text);
                }
            }
            break;
        }
        case CE_MenuBarEmptyArea:
        case CE_MenuEmptyArea: {
            break;
        }
#ifndef QT_NO_PROGRESSBAR
        case CE_ProgressBar: {
            if (const QStyleOptionProgressBarV2 *progressBar = qstyleoption_cast<const QStyleOptionProgressBarV2 *>(option)) {
                drawControl(CE_ProgressBarGroove, progressBar, painter, widget);
                drawControl(CE_ProgressBarContents, progressBar, painter, widget);
                //drawControl(CE_ProgressBarLabel, progressBar, painter, widget);
            }
            break;
        }
        case CE_ProgressBarGroove: {
            if (const QStyleOptionProgressBarV2 *progressBar = qstyleoption_cast<const QStyleOptionProgressBarV2 *>(option)) {
                const QRect progressBarGroove = subElementRect(SE_ProgressBarGroove, progressBar, widget);
                const bool horizontal = progressBar->orientation == Qt::Horizontal;
                ItemStates state = 0;
                if (horizontal)
                    state = state | SS_Horizontal;
                else
                    state = state | SS_Vertical;

                m_private->drawMultiPartItem(SM_ProgressBarGroove, painter, progressBarGroove, state);
            }
            break;
        }
        case CE_ProgressBarContents: {
            if (const QStyleOptionProgressBarV2 *progressBar = qstyleoption_cast<const QStyleOptionProgressBarV2 *>(option)) {
                if (progressBar->minimum == 0 && progressBar->maximum == 0) {
                    //waiting bar
                    animateControl(CE_ProgressBarContents, option, painter, widget);
                } else {
                    QRect rect = subElementRect(SE_ProgressBarGroove, progressBar, widget);
                    const qint64 minimum = qint64(progressBar->minimum);
                    const qint64 maximum = qint64(progressBar->maximum);
                    const qint64 progress = qint64(progressBar->progress);
                    if (progressBar->orientation == Qt::Horizontal) {
                        const qreal scale = rect.width() / qreal(maximum - minimum);
                        qint64 width = scale * progress;
                        width = progress >= maximum ? rect.width() : width;

                        if ((progressBar->direction == Qt::LeftToRight) ^ progressBar->invertedAppearance) {
                            rect = QRect(rect.x(), rect.y(), width, rect.height());
                        } else {
                            rect = QRect(rect.x() + (rect.width() - width), rect.y(), rect.width() - (rect.width() - width), rect.height());
                        }
                        m_private->drawMultiPartItem(SM_ProgressBarIndicator, painter, rect, SS_Horizontal);
                    } else{ //Vertical
                        const qreal scale = rect.height() / qreal(maximum - minimum);
                        qint64 height = scale * progress;
                        height = progress >= maximum ? rect.height() : height;
                        if (progressBar->invertedAppearance) {
                            rect = QRect(rect.x(), rect.y(), rect.width(), height);
                        } else {
                            rect = QRect(rect.x(), rect.y() + (rect.height() - height), rect.width(), rect.height() - (rect.height() - height));
                        }
                        m_private->drawMultiPartItem(SM_ProgressBarIndicator, painter, rect, SS_Vertical);
                    }
                }
            }
            break;
        }
        case CE_ProgressBarLabel: {
            if (const QStyleOptionProgressBarV2 *progressBar = qstyleoption_cast<const QStyleOptionProgressBarV2 *>(option)) {
                if (progressBar->textVisible && (progressBar->minimum != 0 || progressBar->maximum != 0)) {
                    const QString minText = QString().setNum(progressBar->minimum);
                    const QString maxText = QString().setNum(progressBar->maximum);
                    const QRect textRect = subElementRect(SE_ProgressBarGroove, progressBar, widget);
                    if (progressBar->orientation == Qt::Horizontal) {
                        if (progressBar->invertedAppearance) {
                            //minText
                            proxy()->drawItemText(painter, textRect, Qt::AlignRight | Qt::TextSingleLine, progressBar->palette,
                                         progressBar->state & State_Enabled, minText, QPalette::Text);

                            //maxText
                            proxy()->drawItemText(painter, textRect, Qt::AlignLeft | Qt::TextSingleLine, progressBar->palette,
                                         progressBar->state & State_Enabled, maxText, QPalette::Text);

                        } else {
                            //minText
                            proxy()->drawItemText(painter, textRect, Qt::AlignLeft | Qt::TextSingleLine, progressBar->palette,
                                         progressBar->state & State_Enabled, minText, QPalette::Text);

                            //maxText
                            proxy()->drawItemText(painter, textRect, Qt::AlignRight | Qt::TextSingleLine, progressBar->palette,
                                         progressBar->state & State_Enabled, maxText, QPalette::Text);
                        }
                    } else { //Vertical
                        if (progressBar->invertedAppearance) {
                            //minText
                            proxy()->drawItemText(painter, textRect, Qt::AlignTop | Qt::TextSingleLine, progressBar->palette,
                                         progressBar->state & State_Enabled, minText, QPalette::Text);

                            //maxText
                            proxy()->drawItemText(painter, textRect, Qt::AlignBottom | Qt::TextSingleLine, progressBar->palette,
                                         progressBar->state & State_Enabled, maxText, QPalette::Text);

                        } else {
                            //minText
                            proxy()->drawItemText(painter, textRect, Qt::AlignBottom | Qt::TextSingleLine, progressBar->palette,
                                         progressBar->state & State_Enabled, minText, QPalette::Text);

                            //maxText
                            proxy()->drawItemText(painter, textRect, Qt::AlignTop | Qt::TextSingleLine, progressBar->palette,
                                         progressBar->state & State_Enabled, maxText, QPalette::Text);
                        }

                    }

                }
            }
            break;
        }
#endif //QT_NO_PROGRESSBAR
        case CE_ToolButtonLabel:
        default: {
            QCommonStyle::drawControl(element, option, painter, widget);
            break;
        }
    }
}

void QHbStyle::drawComplexControl(ComplexControl control, const QStyleOptionComplex *option,
                            QPainter *painter, const QWidget *widget) const
{
    switch (control) {
#ifndef QT_NO_COMBOBOX
        case CC_ComboBox: {
            if (const QStyleOptionComboBox *cmb = qstyleoption_cast<const QStyleOptionComboBox *>(option)) {
                const QRect cmbxFrame = cmb->rect;

                const bool isDisabled = !(cmb->state & State_Enabled);
                ItemStates state = (isDisabled) ? SS_Disabled : SS_Active;

                if (cmb->state & State_Active &&
                    cmb->state & State_Enabled &&
                    !cmb->state & State_HasFocus &&
                    !cmb->state & State_MouseOver &&
                    !cmb->state & State_Selected) {
                    state |= SS_Active;
                }
                if (cmb->state & State_Active &&
                    cmb->state & State_Enabled &&
                    cmb->state & State_On &&
                    !cmb->state & State_HasFocus &&
                    !cmb->state & State_MouseOver &&
                    !cmb->state & State_Selected) {
                    state |= SS_Active;
                }
                else if (cmb->state & State_Active &&
                         cmb->state & State_Enabled &&
                         cmb->state & State_HasFocus &&
                         cmb->state & State_MouseOver &&
                         cmb->state & State_Selected) {
                    state |= SS_Pressed;
                }
                else if (cmb->state & State_Active &&
                         cmb->state & State_Enabled &&
                         cmb->state & State_HasFocus &&
                         cmb->state & State_MouseOver) {
                    state |= SS_Pressed;
                }
               /* else if (cmb->state & State_Active &&
                         cmb->state & State_Enabled &&
                         cmb->state & State_Sunken) {
                    state |= SS_Pressed;
                }*/

                // Button frame
                QStyleOptionFrame  buttonOption;
                buttonOption.QStyleOption::operator=(*cmb);
                const int buttonMaxHeight = cmbxFrame.height();
                const int buttonMaxWidth = buttonMaxHeight; //button is rect
                const int topLeftPoint = (cmb->direction == Qt::LeftToRight) ? (cmbxFrame.width() - buttonMaxWidth) : 0;

                const QRect buttonRect(topLeftPoint, cmbxFrame.top(), buttonMaxHeight, buttonMaxWidth);
                if (cmb->direction == Qt::RightToLeft){

                    state |= SS_Mirrored;
                }

                if (cmb->subControls & SC_ComboBoxFrame) {

                    QRect frameRect = QRect(cmb->rect);
                    int frameWidth = pixelMetric(PM_DefaultFrameWidth);
                    int maxRight = cmb->rect.height() - 2 * frameWidth;
                    frameRect.adjust(0, 0, -maxRight, 0);

                    const QRect frame = subControlRect(CC_ComboBox, option, SC_ComboBoxFrame, widget);
                    //Draw the frame
                    m_private->drawMultiPartItem(SM_BoxFrame, painter, frame, state);
                }
                //Draw the dropdown button
                m_private->drawItem(SP_BoxButton, painter, buttonRect, state); //@todo: remove magic
            }
            break;
        }
#endif //QT_NO_COMBOBOX
#ifndef QT_NO_SLIDER
        case CC_Slider: {
            if (const QStyleOptionSlider *optionSlider = qstyleoption_cast<const QStyleOptionSlider *>(option)) {
                const QSlider* slider = qobject_cast<const QSlider*>(widget);
                const QRect sliderGroove = subControlRect(control, optionSlider, SC_SliderGroove, widget);
                const QRect sliderHandle = subControlRect(control, optionSlider, SC_SliderHandle, widget);
                const bool horizontal = optionSlider->orientation == Qt::Horizontal;
                const bool isDisabled = !(optionSlider->state & State_Enabled);
                ItemStates grooveState = (isDisabled) ? SS_Disabled : SS_Active;
                if (horizontal)
                    grooveState = grooveState | SS_Horizontal;
                else
                    grooveState = grooveState | SS_Vertical;

                ItemStates handleState = grooveState;

                if (slider && slider->isSliderDown())
                    handleState = handleState | SS_Pressed;
                else if ((optionSlider->state & State_Sunken) || (optionSlider->state & State_On))
                    grooveState = grooveState | SS_Pressed;

                //Draw ticks
                if (optionSlider->subControls & SC_SliderTickmarks) {
                    const QRect tickRect = subControlRect(control, optionSlider, SC_SliderTickmarks, widget);
                    const bool ticksAbove = optionSlider->tickPosition & QSlider::TicksAbove;
                    const bool ticksBelow = optionSlider->tickPosition & QSlider::TicksBelow;

                    int tickOffset = proxy()->pixelMetric(PM_SliderTickmarkOffset, optionSlider, widget);
                    const int thickness = proxy()->pixelMetric(PM_SliderControlThickness, optionSlider, widget);
                    const int available = proxy()->pixelMetric(PM_SliderSpaceAvailable, optionSlider, widget);
                    int interval = optionSlider->tickInterval;
                    if (interval <= 0) {
                        interval = optionSlider->singleStep;
                        if (sliderPositionFromValue(optionSlider->minimum, optionSlider->maximum, interval,available)
                            - sliderPositionFromValue(optionSlider->minimum, optionSlider->maximum, 0, available) < 3)
                            interval = optionSlider->pageStep;
                    }
                    if (!interval)
                        interval = 1;
                    int pos;
                    const int fudge = proxy()->pixelMetric(PM_SliderLength, optionSlider, widget) / 2;
                    // Since there is no subrect for tickmarks do a translation here.
                    int v = optionSlider->minimum;

                    while (v <= (optionSlider->maximum + 1)) {
                        if ((v == optionSlider->maximum + 1) && (interval == 1))
                            break;
                        const int v_ = qMin(v, optionSlider->maximum);
                        pos = sliderPositionFromValue(optionSlider->minimum, optionSlider->maximum,
                                                              v_, available) + fudge;
                        QRect destRect = QRect();
                        if (horizontal) {
                            if (ticksAbove) {
                                destRect = QRect(pos, tickRect.y() - tickOffset + 1, tickRect.width(), tickRect.height());
                                m_private->drawItem(SP_SliderTick, painter, destRect, grooveState);
                            }
                            if (ticksBelow) {
                                destRect = QRect(pos, tickOffset + thickness - 1, tickRect.width(), tickRect.height());
                                m_private->drawItem(SP_SliderTick, painter, destRect, grooveState);
                            }
                        } else {
                            if (ticksAbove) {
                                destRect = QRect(tickRect.x() - tickOffset + 1, pos, tickRect.width(), tickRect.height());
                                m_private->drawItem(SP_SliderTick, painter, destRect, grooveState);
                            }
                            if (ticksBelow) {
                                destRect = QRect(tickOffset + thickness - 1, pos, tickRect.width(), tickRect.height());
                                m_private->drawItem(SP_SliderTick, painter, destRect, grooveState);
                            }
                        }
                        // in the case where maximum is max int
                        int nextInterval = v + interval;
                        if (nextInterval < v)
                            break;
                        v = nextInterval;
                    }
                }

                QRect filledRect;
                QRect filledRectMask;
                if ( horizontal ){
                    if (slider && (slider->layoutDirection() == Qt::LeftToRight) ^ slider->invertedAppearance()){
                        filledRect = QRect( sliderGroove.x(),
                                            sliderGroove.y(),
                                            qMax(sliderGroove.width()-sliderHandle.right(), sliderHandle.right()),
                                            sliderGroove.height());

                        int x = qMin(sliderHandle.left(), sliderGroove.x() + sliderGroove.width()-sliderHandle.right());
                        filledRectMask = QRect(x,
                                               sliderGroove.y(),
                                               sliderGroove.width()-x,
                                               sliderGroove.height());
                    } else {
                        filledRect = QRect( qMin(sliderGroove.width()-sliderHandle.left(),sliderHandle.left()),
                                            sliderGroove.y(),
                                            qMax(sliderGroove.width()-sliderHandle.left(), sliderHandle.left()),
                                            sliderGroove.height());

                        filledRectMask = QRect( sliderGroove.x(),
                                                sliderGroove.y(),
                                                qMax(sliderGroove.width()-sliderHandle.right(), sliderHandle.right()),
                                                sliderGroove.height());

                    }
                } else {
                    if (slider && (slider->layoutDirection() == Qt::LeftToRight) ^ slider->invertedAppearance()){
                        filledRect = QRect(sliderGroove.x(),
                                           qMin(sliderGroove.height()-sliderHandle.top(), sliderHandle.top()),
                                           sliderGroove.width(),
                                           qMax(sliderGroove.height()-sliderHandle.top(), sliderHandle.top()));

                        filledRectMask = QRect(sliderGroove.x(),
                                               sliderGroove.y(),
                                               sliderGroove.width(),
                                               qMax(sliderGroove.height()-sliderHandle.bottom(), sliderHandle.bottom()));
                    } else {
                        filledRect = QRect(sliderGroove.x(),
                                           sliderGroove.y(),
                                           sliderGroove.width(),
                                           qMax(sliderGroove.height()-sliderHandle.bottom(),sliderHandle.bottom()));

                        int y = qMin(sliderHandle.top(), sliderGroove.y() + sliderGroove.height()-sliderHandle.bottom());
                        filledRectMask = QRect( sliderGroove.x(),
                                                y,
                                                sliderGroove.width(),
                                                sliderGroove.height()-y );
                    }
                }

                if (filledRect.width() <  filledRectMask.width() || filledRect.height() <  filledRectMask.height()){
                    // Progress + groove
                    m_private->drawMultiPartItem(SM_SliderGroove, painter, filledRect, grooveState );
                    m_private->drawMultiPartItem(SM_SliderProgress, painter, filledRect, grooveState | SS_Filled);

                    // Groove
                    m_private->drawMultiPartItem(SM_SliderGroove, painter, filledRectMask, grooveState);
                } else {
                    // Groove
                    m_private->drawMultiPartItem(SM_SliderGroove, painter, filledRectMask, grooveState);

                    // Progess + groove
                    m_private->drawMultiPartItem(SM_SliderGroove, painter, filledRect, grooveState );
                    m_private->drawMultiPartItem(SM_SliderProgress, painter, filledRect, grooveState | SS_Filled);
                }

                //handle
                m_private->drawItem(SP_SliderHandle, painter, sliderHandle, handleState);
            }
            break;
        }
#endif //QT_NO_SLIDER
#ifndef QT_NO_SCROLLBAR
        case CC_ScrollBar: {
            if (const QStyleOptionSlider *optionSlider = qstyleoption_cast<const QStyleOptionSlider *>(option)) {
                ItemStates handleStates;
                ItemStates grooveStates;
                const bool horizontal = optionSlider->orientation == Qt::Horizontal;
                const QRect handleRect = subControlRect(control, optionSlider, SC_ScrollBarSlider, widget);
                const QRect grooveRect = subControlRect(control, optionSlider, SC_ScrollBarGroove, widget);
                if (horizontal) {
                    handleStates |= SS_Horizontal;
                    grooveStates |= SS_Horizontal;
                } else {
                    handleStates |= SS_Vertical;
                    grooveStates |= SS_Vertical;
                }
                const SubControls subControls = optionSlider->subControls;
                const bool sliderPressed = ((optionSlider->state & State_Sunken) && (subControls & SC_ScrollBarSlider));
                const bool groovePressed = ((optionSlider->state & State_Sunken) && (subControls & SC_ScrollBarGroove));

                if (sliderPressed)
                    handleStates |= SS_Pressed;
                if (groovePressed)
                    grooveStates |= SS_Pressed;

                m_private->drawMultiPartItem(SM_ScrollBarGroove, painter, grooveRect, grooveStates);
                m_private->drawMultiPartItem(SM_ScrollBarHandle, painter, handleRect, handleStates);
            }
            break;
        }
#endif // QT_NO_SCROLLBAR
#ifndef QT_NO_GROUPBOX
        case CC_GroupBox: {
            if (const QStyleOptionGroupBox *groupBox = qstyleoption_cast<const QStyleOptionGroupBox *>(option)) {
                // Draw frame
                const QRect textRect = subControlRect(CC_GroupBox, option, SC_GroupBoxLabel, widget);

                QRect headerRect = textRect;
                headerRect.setWidth(groupBox->rect.width());
                if (groupBox->subControls & SC_GroupBoxFrame) {
                    QStyleOptionFrameV2 frame;
                    frame.QStyleOption::operator=(*groupBox);
                    frame.features = groupBox->features;
                    frame.lineWidth = groupBox->lineWidth;
                    frame.midLineWidth = groupBox->midLineWidth;
                    frame.rect = subControlRect(CC_GroupBox, option, SC_GroupBoxFrame, widget);
                    proxy()->drawPrimitive(PE_FrameGroupBox, &frame, painter, widget);
                }

                // Draw title
                if ((groupBox->subControls & SC_GroupBoxLabel) && !groupBox->text.isEmpty()) {
                    // Draw title background
                    m_private->drawMultiPartItem(SM_GroupBoxTitle, painter, headerRect);

                    const QColor textColor = groupBox->textColor;
                    painter->save();

                    if (textColor.isValid())
                        painter->setPen(textColor);
                    int alignment = int(groupBox->textAlignment);
                    if (!styleHint(SH_UnderlineShortcut, option, widget))
                        alignment |= Qt::TextHideMnemonic;

                    proxy()->drawItemText(painter, headerRect,  Qt::TextShowMnemonic | Qt::AlignHCenter | Qt::AlignVCenter | alignment,
                                 groupBox->palette, groupBox->state & State_Enabled, groupBox->text,
                                 textColor.isValid() ? QPalette::NoRole : QPalette::WindowText);
                    painter->restore();
                }
                const QRect checkBoxRect = subControlRect(CC_GroupBox, option, SC_GroupBoxCheckBox, widget);
                // Draw checkbox
                if (groupBox->subControls & SC_GroupBoxCheckBox) {
                    QStyleOptionButton box;
                    box.QStyleOption::operator=(*groupBox);
                    box.rect = checkBoxRect;
                    proxy()->drawPrimitive(PE_IndicatorCheckBox, &box, painter, widget);
                }
            }
            break;
        }
#endif //QT_NO_GROUPBOX
#ifndef QT_NO_TOOLBUTTON
        case CC_ToolButton: {
            if (const QStyleOptionToolButton *toolBtn = qstyleoption_cast<const QStyleOptionToolButton *>(option)) {
                const QRect buttonRect(subControlRect(control, toolBtn, SC_ToolButton, widget));
                QRect menuRect = QRect();
                if (toolBtn->subControls & SC_ToolButtonMenu)
                    menuRect = subControlRect(control, toolBtn, SC_ToolButtonMenu, widget);

                // Draw button bevel
                if (toolBtn->subControls & SC_ToolButton)
                    proxy()->drawPrimitive(PE_PanelButtonTool, toolBtn, painter, widget);

                //draw focus
                if (toolBtn->state & State_HasFocus) {
                    QStyleOptionFocusRect frameOpt;
                    frameOpt.QStyleOption::operator=(*toolBtn);
                    frameOpt.rect = subElementRect(SE_PushButtonFocusRect, toolBtn, widget); //can we use this, or should we just reduce the button rect?
                    if (toolBtn->features & QStyleOptionToolButton::MenuButtonPopup)
                        frameOpt.rect.adjust(0, 0, -proxy()->pixelMetric(PM_MenuButtonIndicator), 0);
                    proxy()->drawPrimitive(PE_FrameFocusRect, &frameOpt, painter, widget);
                }

                if (toolBtn->text.length() > 0 || !toolBtn->icon.isNull() || (toolBtn->features & QStyleOptionToolButton::Arrow)) {
                    //draw label
                    QStyleOptionToolButton label = *toolBtn;
                    int fw = proxy()->pixelMetric(PM_DefaultFrameWidth, option, widget);
                    label.rect = buttonRect.adjusted(fw, fw, -fw, -fw);
                    proxy()->drawControl(CE_ToolButtonLabel, &label, painter, widget);
                }
                if (toolBtn->subControls & SC_ToolButtonMenu) {
                    //draw menu indicator
                    const int menuButtonIndicator = proxy()->pixelMetric(PM_MenuButtonIndicator, toolBtn, widget);
                    QStyleOptionToolButton menuOpt = *toolBtn;
                    menuOpt.rect = QRect(toolBtn->rect.right() - menuButtonIndicator,
                                        toolBtn->rect.y() + (toolBtn->rect.height() - menuButtonIndicator) / 2,
                                        menuButtonIndicator,
                                        menuButtonIndicator);
                    menuOpt.rect = visualRect(toolBtn->direction, toolBtn->rect, menuOpt.rect);

                    PrimitiveElement pe;
                    bool arrow = true;
                    switch(toolBtn->arrowType) {
                        case Qt::UpArrow: {
                            pe = PE_IndicatorArrowUp;
                            break;
                        }
                        case Qt::LeftArrow: {
                            pe = PE_IndicatorArrowLeft;
                            break;
                        }
                        case Qt::RightArrow: {
                            pe = PE_IndicatorArrowRight;
                            break;
                        }
                        case Qt::DownArrow: {
                            pe = PE_IndicatorArrowDown;
                            break;
                        }
                        default: {
                            arrow = false;
                        }
                    }
                    if (arrow)
                        proxy()->drawPrimitive(pe, &menuOpt, painter, widget);
                }
            }
            break;
        }
#endif //QT_NO_TOOLBUTTON
        case CC_SpinBox: {
            if (const QStyleOptionSpinBox *optionSpinbox = qstyleoption_cast<const QStyleOptionSpinBox *>(option)) {
                const QRect spinboxFrame = subControlRect(control, optionSpinbox, SC_SpinBoxFrame, widget);
                const QRect spinboxButtonUpRect = subControlRect(control, optionSpinbox, SC_SpinBoxUp, widget);
                const QRect spinboxButtonDownRect = subControlRect(control, optionSpinbox, SC_SpinBoxDown, widget);
                const QRect spinboxEditorRect = subControlRect(control, optionSpinbox, SC_SpinBoxEditField, widget);

                //Frame & background
                const bool isDisabled = !(optionSpinbox->state & State_Enabled);
                ItemStates state = (isDisabled) ? SS_Disabled : SS_Active;
                if (optionSpinbox->state & State_HasFocus)
                    state |= SS_Selected;
                //Draw the rounded border of edit field frame under button, half spin button width
                //Label drawn to spinboxEditorRect
                m_private->drawMultiPartItem(SM_BoxFrame, painter, spinboxEditorRect.adjusted((-0.5*spinboxButtonDownRect.width()),0,0,0), state);


                QStyle::State buttonState;
                //Buttons
                if (optionSpinbox->subControls & SC_SpinBoxUp) {
                    if (!(optionSpinbox->stepEnabled & QAbstractSpinBox::StepUpEnabled))
                        buttonState &= ~State_Enabled;
                    if (optionSpinbox->activeSubControls == SC_SpinBoxUp && (optionSpinbox->state & State_Sunken)) {
                        buttonState |= State_On;
                        buttonState |= State_Sunken;
                    } else {
                        buttonState |= State_Raised;
                        buttonState &= ~State_Sunken;
                    }
                    const bool isPressed = (buttonState & State_Sunken);
                    ItemStates upButtonState = (isPressed) ? ItemStates(SS_Pressed  | SS_Active) : ItemStates(SS_Active);
                    if (optionSpinbox->direction == Qt::RightToLeft)
                        upButtonState = upButtonState | SS_Flipped;
                    else
                        upButtonState = upButtonState | SS_Flipped | SS_Mirrored;
                    if (optionSpinbox->state & State_HasFocus)
                        upButtonState |= SS_Selected;
                    if (!(optionSpinbox->stepEnabled & QAbstractSpinBox::StepUpEnabled))
                        upButtonState |= SS_Disabled;
                    m_private->drawItem(SP_BoxButton, painter, spinboxButtonUpRect, upButtonState);
                }

                if (optionSpinbox->subControls & SC_SpinBoxDown) {
                    if (!(optionSpinbox->stepEnabled & QAbstractSpinBox::StepDownEnabled))
                        buttonState &= ~State_Enabled;
                    if (optionSpinbox->activeSubControls == SC_SpinBoxDown && (optionSpinbox->state & State_Sunken)) {
                        buttonState |= State_On;
                        buttonState |= State_Sunken;
                    } else {
                        buttonState |= State_Raised;
                        buttonState &= ~State_Sunken;
                    }
                    const bool isPressed = (buttonState & State_Sunken);
                    ItemStates downButtonState = (isPressed) ? ItemStates(SS_Pressed  | SS_Active) : ItemStates(SS_Active);
                    if (optionSpinbox->direction == Qt::RightToLeft)
                        downButtonState = downButtonState;
                    else
                        downButtonState = downButtonState | SS_Mirrored;
                    if (optionSpinbox->state & State_HasFocus)
                        downButtonState |= SS_Selected;
                    if (!(optionSpinbox->stepEnabled & QAbstractSpinBox::StepDownEnabled))
                        downButtonState |= SS_Disabled;
                    m_private->drawItem(SP_BoxButton, painter, spinboxButtonDownRect, downButtonState);
                }
            }
            break;
        }
        case CC_TitleBar:
        case CC_Q3ListView:
        case CC_Dial:
        case CC_MdiControls:
        default: {
            QCommonStyle::drawComplexControl(control, option, painter, widget);
            break;
        }
    }
}

QSize QHbStyle::sizeFromContents(ContentsType type, const QStyleOption *option,
                           const QSize &size, const QWidget *widget) const
{
    QSize newSize = QCommonStyle::sizeFromContents(type, option, size, widget);
    switch (type) {
#ifndef QT_NO_MENU
        case CT_MenuItem: {
            if (qstyleoption_cast<const QStyleOptionMenuItem *>(option)) {
                const int verticalMargin = pixelMetric(PM_MenuVMargin);
                const int horizontalMargin = pixelMetric(PM_MenuHMargin);
                newSize += QSize(horizontalMargin, 2 * verticalMargin);
            }
            break;
        }
#endif
#ifndef QT_NO_ITEMVIEWS
        case CT_ItemViewItem: {
            newSize += QSize(0,22);
            break;
        }
#endif
        case CT_PushButton: {
            newSize += QSize(0, 2 * proxy()->pixelMetric(PM_ButtonMargin, option, widget));
            break;
        }
        default:
            break;
    }
    return newSize;
}

QRect QHbStyle::subElementRect(SubElement element, const QStyleOption *option, const QWidget *widget) const
{
    const QRect baseSize = QCommonStyle::subElementRect(element, option, widget);
    QRect elementSize = baseSize;
    switch (element) {
        case SE_LineEditContents: {
            qreal metric = 0;
            m_private->styleManager()->parameter(QLatin1String("hb-param-margin-gene-middle-horizontal"), metric);
            const int metricValue = metric + 0.5;
            elementSize = visualRect(
                option->direction, option->rect, option->rect.adjusted(metricValue, 0, 0, 0));
            }
            break;
        case SE_ItemViewItemText: {
            if (const QStyleOptionMenuItem *menuItem = qstyleoption_cast<const QStyleOptionMenuItem *>(option)) {
                elementSize = menuItem->rect;
                const QRect iconRect = subElementRect(SE_ItemViewItemDecoration, option, widget);
                const QRect checkBoxRect = subElementRect(SE_ViewItemCheckIndicator, option, widget);
                const int indicatorSpacing = proxy()->pixelMetric(PM_LayoutHorizontalSpacing, option, widget);
                int totalXMod = qMax(0, qMax((checkBoxRect.isValid() ? checkBoxRect.topRight().x() : 0),
                                     (iconRect.isValid() ? iconRect.topRight().x() : 0)));
                const int widthMod = checkBoxRect.width() + iconRect.width() + indicatorSpacing;
                totalXMod = (menuItem->direction == Qt::LeftToRight) ? qMax(0, totalXMod - elementSize.topLeft().x()): 0;
                totalXMod += indicatorSpacing;
                elementSize.translate(totalXMod, 0);
                elementSize.setWidth(menuItem->rect.width() - widthMod);
            } else if (const QStyleOptionViewItemV4 *itemView = qstyleoption_cast<const QStyleOptionViewItemV4 *>(option)) {
                elementSize = itemView->rect;
                if (itemView->decorationPosition == QStyleOptionViewItem::Left ||
                    itemView->decorationPosition == QStyleOptionViewItem::Right) {
                    const QRect iconRect = subElementRect(SE_ItemViewItemDecoration, option, widget);
                    const QRect checkBoxRect = subElementRect(SE_ViewItemCheckIndicator, option, widget);
                    const int indicatorSpacing = proxy()->pixelMetric(PM_LayoutHorizontalSpacing, option, widget);
                    int totalXMod = qMax(0, qMax((checkBoxRect.isValid() ? checkBoxRect.topRight().x() : 0),
                                         (iconRect.isValid() ? iconRect.topRight().x() : 0)));
                    const int widthMod = checkBoxRect.width() + iconRect.width() + indicatorSpacing;
                    totalXMod = (itemView->direction == Qt::LeftToRight) ? qMax(0, totalXMod - elementSize.topLeft().x()): 0;
                    totalXMod += indicatorSpacing;
                    elementSize.translate(totalXMod, 0);
                    elementSize.setWidth(itemView->rect.width() - widthMod);
                    elementSize = visualRect(itemView->direction, itemView->rect, elementSize);
                } else {
                    const QRect iconRect = subElementRect(SE_ItemViewItemDecoration, option, widget);
                    const bool decoratorOnTop = (itemView->decorationPosition == QStyleOptionViewItem::Top);
                    if (decoratorOnTop)
                        elementSize.translate(0, iconRect.height());
                    else
                        elementSize.translate(0, -iconRect.height());
                }
            }
            break;
        }
        case SE_ViewItemCheckIndicator: {
            if (const QStyleOptionMenuItem *menuItem = qstyleoption_cast<const QStyleOptionMenuItem *>(option)) {
                if (menuItem->menuHasCheckableItems) {
                    const int indicatorWidth = proxy()->pixelMetric(PM_IndicatorWidth, option, widget);
                    const int htAdjust = (menuItem->rect.height() - indicatorWidth) / 2;
                    elementSize = QRect(menuItem->rect.x(), menuItem->rect.y() + htAdjust, indicatorWidth, indicatorWidth);
                    elementSize = visualRect(menuItem->direction, menuItem->rect, elementSize);
                } else { elementSize = QRect(); }
            } else if (const QStyleOptionViewItemV4 *itemOption = qstyleoption_cast<const QStyleOptionViewItemV4 *>(option)) {
                if (itemOption->features & QStyleOptionViewItemV2::HasCheckIndicator) {
                    const int indicatorWidth = proxy()->pixelMetric(PM_IndicatorWidth, option, widget);
                    const int htAdjust = (itemOption->rect.height() - indicatorWidth) / 2;
                    elementSize = QRect(itemOption->rect.x(), itemOption->rect.y() + htAdjust, indicatorWidth, indicatorWidth);
                    elementSize = visualRect(itemOption->direction, itemOption->rect, elementSize);
                } else { elementSize = QRect(); }
            }
            break;
        }
        case SE_ItemViewItemDecoration: {
            if (const QStyleOptionMenuItem *menuItem = qstyleoption_cast<const QStyleOptionMenuItem *>(option)) {
                if (!menuItem->icon.isNull()) {
                    const QRect checkBoxRect = subElementRect(SE_ViewItemCheckIndicator, option, widget);
                    const int imageWidth = proxy()->pixelMetric(PM_SmallIconSize, option, widget);
                    const int indicatorSpacing = proxy()->pixelMetric(PM_LayoutHorizontalSpacing, option, widget);
                    const int htAdjust = (menuItem->rect.height() - imageWidth) / 2;
                    if (checkBoxRect.isValid()) {
                        elementSize = QRect(menuItem->rect.x() + checkBoxRect.width() + indicatorSpacing, menuItem->rect.y() + htAdjust, imageWidth, imageWidth);
                    } else {
                        elementSize = QRect(menuItem->rect.x() + indicatorSpacing, menuItem->rect.y() + htAdjust, imageWidth, imageWidth);
                    }
                    elementSize = visualRect(menuItem->direction, menuItem->rect, elementSize);
                } else { elementSize = QRect(); }
            } else if (const QStyleOptionViewItemV4 *itemOption = qstyleoption_cast<const QStyleOptionViewItemV4 *>(option)) {
                if (!itemOption->icon.isNull()) {
                    const QRect checkBoxRect = subElementRect(SE_ViewItemCheckIndicator, option, widget);
                    const int imageWidth = proxy()->pixelMetric(PM_SmallIconSize, option, widget);
                    const int indicatorSpacing = proxy()->pixelMetric(PM_LayoutHorizontalSpacing, option, widget);
                    const int htAdjust = (itemOption->rect.height() - imageWidth) / 2;
                    if (checkBoxRect.isValid()) {
                        elementSize = QRect(itemOption->rect.x() + checkBoxRect.width() + indicatorSpacing, itemOption->rect.y() + htAdjust, imageWidth, imageWidth);
                    } else {
                        elementSize = QRect(itemOption->rect.x() + indicatorSpacing, itemOption->rect.y() + htAdjust, imageWidth, imageWidth);
                    }
                    elementSize = visualRect(itemOption->direction, itemOption->rect, elementSize);
                } else { elementSize = QRect(); }
            }
            break;
        }
        case SE_PushButtonFocusRect: {
            if (const QStyleOptionButton *button = qstyleoption_cast<const QStyleOptionButton *>(option)) {
                const int margin = proxy()->pixelMetric(PM_FocusFrameHMargin, button, widget);
                elementSize = baseSize.adjusted(-margin, -margin, margin, margin);
            }
            break;
        }
        case SE_ProgressBarGroove: {
            if (const QStyleOptionProgressBarV2 *progressBar = qstyleoption_cast<const QStyleOptionProgressBarV2 *>(option)) {
                elementSize = progressBar->rect;
            }
            break;
        }
        default:
            break;
    }
    return elementSize;
}

QRect QHbStyle::subControlRect(ComplexControl cc, const QStyleOptionComplex *option,
                         SubControl sc, const QWidget *widget) const
{
    const QRect baseSize = QCommonStyle::subControlRect(cc, option, sc, widget);
    QRect elementSize = baseSize;
    switch (cc) {
        case CC_ComboBox: {
            if (const QStyleOptionComboBox *cmb = qstyleoption_cast<const QStyleOptionComboBox *>(option)) {
                const int buttonIconSize = pixelMetric(PM_ButtonIconSize);
                const int buttonMargin = cmb->frame ? 2 : 0;
                const int frameThickness = cmb->frame ? pixelMetric(PM_ComboBoxFrameWidth, cmb, widget) : 0;
                const int buttonWidth = qMax(cmb->rect.height(), buttonIconSize);

                QSize buttonSize;
                buttonSize.setWidth(buttonWidth + 2 * buttonMargin);
                buttonSize.setHeight(qMax(8, (cmb->rect.height() >> 1) - frameThickness));
                buttonSize = buttonSize.expandedTo(QApplication::globalStrut());
                switch (sc) {
                    case SC_ComboBoxArrow: {
                       elementSize = option->rect;
                       break;
                   }
                    case SC_ComboBoxFrame: {
                        QRect frameRect = QRect(cmb->rect);
                        int frameWidth = pixelMetric(PM_DefaultFrameWidth, cmb, widget);
                        int maxRight = cmb->rect.height() - 2 * frameWidth;
                        if(cmb->direction == Qt::RightToLeft) {
                            frameRect.adjust(+ 0.25 * buttonWidth, 0, -0.25 * buttonWidth, 0);
                        }else{
                            frameRect.adjust(0, 0, -maxRight-4, 0);
                        }
                        elementSize = frameRect;
                        break;
                    }
                    case SC_ComboBoxEditField: {
                        int withFrameX = 0;
                        int offSet = 0;
                        if(cmb->direction == Qt::RightToLeft) {
                            withFrameX = cmb->rect.x() + cmb->rect.width() - frameThickness;
                            offSet = buttonWidth;
                        }
                        else{
                            withFrameX = cmb->rect.x() + cmb->rect.width() - frameThickness - buttonSize.width();
                        }
                        elementSize = QRect(
                            frameThickness + offSet,
                            frameThickness - 2,
                            withFrameX - frameThickness - offSet,
                            cmb->rect.height() - 2 * frameThickness );
                        break;
                    }
                    case SC_ComboBoxListBoxPopup: {
                        QRect mover = cmb->rect;
                        mover.moveBottom(cmb->rect.top() - 2);
                        elementSize = mover;
                        break;
                    }
                    default:
                        break;
                }
            }
            break;
        }
#ifndef QT_NO_SCROLLBAR
//todo: this was lifted "as-is" from QS60Style. Check that it is valid for uiemo.
        case CC_ScrollBar: {
             if (const QStyleOptionSlider *scrollbarOption = qstyleoption_cast<const QStyleOptionSlider *>(option)) {
                 const QRect scrollBarRect = scrollbarOption->rect;
                 const bool isHorizontal = scrollbarOption->orientation == Qt::Horizontal;
                 const int maxlen = isHorizontal ? scrollBarRect.width() : scrollBarRect.height();
                 int sliderlen;

                 // calculate slider length
                 if (scrollbarOption->maximum != scrollbarOption->minimum) {
                     const uint range = scrollbarOption->maximum - scrollbarOption->minimum;
                     sliderlen = (qint64(scrollbarOption->pageStep) * maxlen) / (range + scrollbarOption->pageStep);

                     const int slidermin = proxy()->pixelMetric(PM_ScrollBarSliderMin, scrollbarOption, widget);
                     if (sliderlen < slidermin || range > (INT_MAX >> 1))
                         sliderlen = slidermin;
                     if (sliderlen > maxlen)
                         sliderlen = maxlen;
                 } else {
                     sliderlen = maxlen;
                 }

                 const int sliderstart = sliderPositionFromValue(scrollbarOption->minimum,
                                                                 scrollbarOption->maximum,
                                                                 scrollbarOption->sliderPosition,
                                                                 maxlen - sliderlen,
                                                                 scrollbarOption->upsideDown);

                 switch (sc) {
                    case SC_ScrollBarSubPage: {  // between top/left button and slider
                         if (isHorizontal)
                             elementSize.setRect(0, 0, sliderstart, scrollBarRect.height());
                         else
                             elementSize.setRect(0, 0, scrollBarRect.width(), sliderstart);
                         break;
                     }
                     case SC_ScrollBarAddPage: {         // between bottom/right button and slider
                         const int addPageLength = sliderstart + sliderlen;
                         if (isHorizontal)
                             elementSize = scrollBarRect.adjusted(addPageLength, 0, 0, 0);
                         else
                             elementSize = scrollBarRect.adjusted(0, addPageLength, 0, 0);
                         break;
                     }
                     case SC_ScrollBarGroove: {
                         elementSize = scrollBarRect;
                         break;
                     }
                    case SC_ScrollBarSlider: {
                         if (scrollbarOption->orientation == Qt::Horizontal)
                             elementSize.setRect(sliderstart, 0, sliderlen, scrollBarRect.height());
                         else
                             elementSize.setRect(0, sliderstart, scrollBarRect.width(), sliderlen);
                         break;
                     }
                     case SC_ScrollBarSubLine:            // top/left button
                     case SC_ScrollBarAddLine:            // bottom/right button
                     default: {
                         break;
                     }
                 }
                 elementSize = visualRect(scrollbarOption->direction, scrollBarRect, elementSize);
             }
             break;
         }
#endif // QT_NO_SCROLLBAR
#ifndef QT_NO_GROUPBOX
        case CC_GroupBox: {
             if (const QStyleOptionGroupBox *groupBox = qstyleoption_cast<const QStyleOptionGroupBox *>(option)) {
                 switch (sc) {
                     case SC_GroupBoxFrame: {
                         int topMargin = 0;
                         int topHeight = 0;
                         int verticalAlignment = proxy()->styleHint(SH_GroupBox_TextLabelVerticalAlignment, groupBox, widget);
                         if (groupBox->text.size() || (groupBox->subControls & SC_GroupBoxCheckBox)) {
                             topHeight = groupBox->fontMetrics.height();
                             if (verticalAlignment & Qt::AlignVCenter)
                                 topMargin = topHeight / 2;
                             else if (verticalAlignment & Qt::AlignTop)
                                 topMargin = topHeight;
                         }

                         QRect frameRect = groupBox->rect;
                         frameRect.setTop(topMargin);
                         elementSize = frameRect;
                         break;
                     }
                     case SC_GroupBoxContents: {
                         const QRect titleRect = proxy()->subControlRect(cc, option, SC_GroupBoxLabel, widget);
                         const QRect frameRect = proxy()->subControlRect(cc, option, SC_GroupBoxFrame, widget);
                         elementSize = frameRect;
                         elementSize.setHeight(frameRect.height() + titleRect.height());
                         elementSize.translate((groupBox->direction == Qt::LeftToRight) ? titleRect.bottomLeft() : titleRect.bottomRight());
                         break;
                     }
                     case SC_GroupBoxCheckBox:
                     case SC_GroupBoxLabel: {
                         QFontMetrics fontMetrics = groupBox->fontMetrics;
                         const int height = fontMetrics.height();
                         //margins
                         int labelTopMargin = 0; int labelBottomMargin = 0;
                         m_private->hbParameter(QLatin1String("hb-param-margin-gene-top"), labelTopMargin);
                         m_private->hbParameter(QLatin1String("hb-param-margin-gene-bottom"), labelBottomMargin);

                         //height
                         const int indicatorHeight = proxy()->pixelMetric(PM_IndicatorHeight, option, widget);
                         elementSize = groupBox->rect;
                         elementSize.setHeight(qMax(height, indicatorHeight) + labelTopMargin + labelBottomMargin);

                         const int indicatorSpace = proxy()->pixelMetric(PM_CheckBoxLabelSpacing, option, widget);
                         const bool hasCheckBox = groupBox->subControls & SC_GroupBoxCheckBox;
                         const int indicatorWidth = proxy()->pixelMetric(PM_IndicatorWidth, option, widget);
                         const int checkBoxSize = hasCheckBox ? (indicatorWidth + indicatorSpace) : 0;

                         QRect totalRect;
                         // Adjust totalRect if checkbox is set
                         if (hasCheckBox) {
                             int top = 0; int left = 0; int width = 0;
                             int height = elementSize.height();
                             // Adjust for check box
                             if (sc == SC_GroupBoxCheckBox) {
                                 top = labelTopMargin + elementSize.top() + (fontMetrics.height() - indicatorHeight) / 2;
                                 const int right =  elementSize.right() - checkBoxSize;
                                 left = right - checkBoxSize;
                                 width = height = checkBoxSize;
                             // Adjust for label
                             } else {
                                 left = (groupBox->direction == Qt::LeftToRight) ? elementSize.left()
                                                                                 : (elementSize.left() + checkBoxSize);
                                 width = elementSize.width() - checkBoxSize;
                             }
                             totalRect.setRect(left, top, width, height);
                         }
                         elementSize = visualRect(option->direction, option->rect, totalRect);
                         break;
                     }
                     default: {
                         break;
                     }
                 }
             }
             break;
         }
#endif //QT_NO_GROUPBOX
#ifndef QT_NO_SLIDER
        case CC_Slider: {
             if (const QStyleOptionSlider *slider = qstyleoption_cast<const QStyleOptionSlider *>(option)) {
                 qreal metric = 0;
                 m_private->styleManager()->parameter(QLatin1String("hb-param-margin-gene-middle-horizontal"), metric);
                 const int metricValue = metric + 0.5;
                 switch (sc) {
                     //Hb differentiates between major and minor ticks.
                     //Unfortunately Qt does not, so we consider all ticks as major.
                     //Tick sizes from Hb widget illustrations.
                     case SC_SliderTickmarks: {
                         //This just returns first tick rect for slider. Others need to be translated.
                        const bool horizontal = (slider->orientation == Qt::Horizontal);
                        const qreal unitValue = HbDeviceProfile::current().unitValue();
                        qreal width = 0; qreal height = 0;
                        //width and height unit values from Hb widget gallery
                        if (horizontal) {
                             width = 0.5 * unitValue + 0.5;
                             height = unitValue + 0.5;
                        } else {
                             height = 0.5 * unitValue + 0.5;
                             width = unitValue + 0.5;
                        }
                        const QRect sliderGroove = subControlRect(cc, slider, SC_SliderGroove, widget);
                        QRect tickRect = QRect(sliderGroove.x(), sliderGroove.y(), width, height);
                        if (horizontal)
                             tickRect.translate(0, -metricValue);
                        else
                             tickRect.translate(-metricValue, 0);
                        elementSize = tickRect;
                        break;
                     }
                    case SC_SliderGroove: {
                        const int thickness = proxy()->pixelMetric(PM_SliderThickness, slider, widget);
                        const int tickOffset = proxy()->pixelMetric(PM_SliderTickmarkOffset, slider, widget);
                         if (slider->orientation == Qt::Horizontal)
                             elementSize.setRect(slider->rect.x(), slider->rect.y() + metricValue + tickOffset,
                                         slider->rect.width(), thickness - 2 * (metricValue + tickOffset));
                         else
                             elementSize.setRect(slider->rect.x() + tickOffset + metricValue, slider->rect.y(),
                                         thickness - 2 * (metricValue + tickOffset), slider->rect.height());
                         break;
                    }
                    default: {
                        break;
                    }
                 }
             }
             break;
         }
#endif //QT_NO_SLIDER
#ifndef QT_NO_SPINBOX
        case CC_SpinBox: {
            if (const QStyleOptionSpinBox *spinbox = qstyleoption_cast<const QStyleOptionSpinBox *>(option)) {
                const int buttonIconSize = pixelMetric(PM_ButtonIconSize);
                // Spinbox buttons should be no larger than one fourth of total width.
                const int maxSize = qMax(spinbox->rect.width() / 4, buttonIconSize + 4); //@magic
                QSize buttonSize;
                buttonSize.setHeight(qMin(maxSize, qMax(8, spinbox->rect.height())));
                buttonSize.setWidth(buttonSize.height()); //make buttons square

                switch (sc) {
                    case SC_SpinBoxFrame:
                         elementSize = option->rect.adjusted(0, 0, -buttonSize.width() + 5, 0); //@magic
                         break;
                    case SC_SpinBoxDown: {
                        if (option->direction == Qt::RightToLeft)
                            elementSize = QRect(option->rect.right() - buttonSize.width(), option->rect.y(), buttonSize.width(), option->rect.height());
                        else
                            elementSize = QRect(option->rect.x(), option->rect.y(), buttonSize.width(), option->rect.height());
                    }
                    break;
                    case SC_SpinBoxUp: {
                        if (option->direction == Qt::RightToLeft)
                            elementSize = QRect(option->rect.x(), option->rect.y(), buttonSize.width(), option->rect.height());
                        else
                            elementSize = QRect(option->rect.right() - buttonSize.width(), option->rect.y(), buttonSize.width(), option->rect.height());
                    }
                    break;
                    case SC_SpinBoxEditField:
                        elementSize = option->rect.adjusted(buttonSize.width(), 0, -buttonSize.width(), 0);
                        break;
                    default:
                        break;
                 }
             }
             break;
         }
#endif //QT_NO_SPINBOX
         default: {
             break;
         }
    }
    return elementSize;
}

int QHbStyle::styleHint(StyleHint hint, const QStyleOption *option, const QWidget *widget,
                  QStyleHintReturn *returnData) const
{
    int retValue = 0;
    switch (hint) {
        case SH_RequestSoftwareInputPanel:
            retValue = RSIP_OnMouseClick;
            break;
        case SH_ToolButtonStyle:
            retValue = Qt::ToolButtonIconOnly;
            break;
        case SH_TabWidget_DefaultTabPosition: {
            retValue = QTabWidget::North;
            break;
        }
        case SH_TabBar_ElideMode:
            if (option)
                retValue = (option->direction == Qt::LeftToRight) ? Qt::ElideRight : Qt::ElideLeft;
            else if (widget)
                retValue = (widget->layoutDirection() == Qt::LeftToRight) ? Qt::ElideRight : Qt::ElideLeft;
            else
                retValue = (QApplication::layoutDirection() == Qt::LeftToRight) ? Qt::ElideRight : Qt::ElideLeft;
            break;
        case SH_ItemView_ShowDecorationSelected:
            retValue = true;
            break;
        case SH_SpinControls_DisableOnBounds: {
            retValue = true;
            break;
        }
        case SH_MessageBox_TextInteractionFlags: {
            retValue = Qt::LinksAccessibleByMouse;
            break;
        }
        case SH_MessageBox_CenterButtons: {
            retValue = true;
            break;
        }
        case SH_Button_FocusPolicy: {
            retValue = Qt::StrongFocus;
            break;
        }
        case SH_Table_GridLineColor: {
            retValue = Qt::green; //@todo: fetch this
            break;
        }
        case SH_TabBar_Alignment: {
            retValue = Qt::AlignCenter;
            break;
        }
        case SH_Header_ArrowAlignment: {
            if (option)
                retValue = (option->direction == Qt::LeftToRight) ? Qt::AlignLeft : Qt::AlignRight;
            else if (widget)
                retValue = (widget->layoutDirection() == Qt::LeftToRight) ? Qt::AlignLeft : Qt::AlignRight;
            else
                retValue = (QApplication::layoutDirection() == Qt::LeftToRight) ? Qt::AlignLeft : Qt::AlignRight;
            break;
        }
        case SH_ToolTipLabel_Opacity: {
            retValue = 255;
            break;
        }
        case SH_ScrollBar_ContextMenu: {
            retValue = false;
            break;
        }
        case SH_Menu_MouseTracking: {
            retValue = 0;
            break;
        }
        case SH_ComboBox_ListMouseTracking: {
            retValue = 0;
            break;
        }
        case SH_ComboBox_Popup: {
            retValue = 1;
            break;
        }
        case SH_UnderlineShortcut: {
            retValue = false;
            break;
        }
        case SH_GroupBox_TextLabelColor: {
            QColor test = HbColorScheme::color("popupforeground");
            retValue = int(HbColorScheme::color("popupforeground").rgba()); //@todo: should use "qtc_viewtitle" but that is not yet available
            break;
        }
        case SH_GroupBox_TextLabelVerticalAlignment: {
            retValue = Qt::AlignBottom;
            break;
        }
        case SH_ItemView_ActivateItemOnSingleClick: {
            retValue = true;
            break;
        }
        case SH_ItemView_ArrowKeysNavigateIntoChildren: {
            retValue = true;
            break;
        }
        case SH_ItemView_PaintAlternatingRowColorsForEmptyArea: {
            retValue = true;
            break;
        }
        default: {
            retValue = QCommonStyle::styleHint(hint, option, widget, returnData);
            break;
        }
    }
    return retValue;
}

int QHbStyle::pixelMetric(PixelMetric metric, const QStyleOption *option, const QWidget *widget) const
{
    qreal metricValue = 0.0;
    const qreal unitValue = HbDeviceProfile::current().unitValue();
    bool valueFound = false;
    switch(metric) {
        case PM_ButtonMargin: {
            //Hb defines different margin values for each margin. We could use any of them,
            // average, or mean, but lets settle for top margin for now.
            valueFound = m_private->styleManager()->parameter(QLatin1String("hb-param-background-button"), metricValue);
            break;
        }
        case PM_ButtonDefaultIndicator: {
            //Lets set default button indication frame width to zero as there is no such concept in uiemo
            valueFound = true;
            break;
        }
        case PM_MenuButtonIndicator: {
            // Hb returns a square area for icon in a button.
            valueFound = m_private->styleManager()->parameter(QLatin1String("hb-param-graphic-size-secondary"), metricValue);
            break;
        }
        case PM_ButtonShiftVertical:
        case PM_ButtonShiftHorizontal: {
            //No button shifting in Hb
            valueFound = true;
            break;
        }
        case PM_DefaultFrameWidth: {
            valueFound = true;
            metricValue = 2.0;
            break;
        }
        case PM_SpinBoxFrameWidth: {
            valueFound = m_private->styleManager()->parameter(QLatin1String("hb-param-margin-gene-middle-horizontal"), metricValue);
            break;
        }
        case PM_ComboBoxFrameWidth: {
            valueFound = m_private->styleManager()->parameter(QLatin1String("hb-param-margin-gene-top"), metricValue);
            break;
        }
        case PM_MaximumDragDistance: {
            valueFound = true;
            metricValue = -1.0; //disable maximum drag distance functionality
            break;
        }
        case PM_ScrollBarExtent: {
            valueFound = m_private->styleManager()->parameter(QLatin1String("hb-param-widget-scroll-bar-indicative-width"), metricValue);
            break;
        }
        case PM_ScrollBarSliderMin: {
            valueFound = true;
            metricValue = 8.0 * unitValue; //8.0 from uiemo graphic designers (to be updated to the specification)
            //todo: for indicative scrollbars the slider is 4.0uns. Can we query the state of scrollbar?
            break;
        }
        case PM_SliderThickness: {
            int numberOfTicks = 0;
            if (const QStyleOptionSlider *optionSlider = qstyleoption_cast<const QStyleOptionSlider *>(option)) {
                if (optionSlider->tickPosition & QSlider::TicksAbove)
                    ++numberOfTicks;
                if (optionSlider->tickPosition & QSlider::TicksBelow)
                    ++numberOfTicks;
            }
            metricValue = proxy()->pixelMetric(PM_SliderControlThickness, option, widget) +
                          numberOfTicks * (1.0 * unitValue + 0.5); //tickmarks are one unit tall
            valueFound = true;
            break;
        }
        // Slider handle size values from Hb widget illustrations library.
        case PM_SliderControlThickness: {
            valueFound = true;
            metricValue = 4.0 * unitValue;
            break;
        }
        case PM_SliderLength: {
            valueFound = true;
            metricValue = 2.0 * unitValue;
            break;
        }
        case PM_SliderTickmarkOffset: {
            valueFound = true;
            if (const QStyleOptionSlider *optionSlider = qstyleoption_cast<const QStyleOptionSlider *>(option))
                if (optionSlider->tickPosition & QSlider::TicksAbove || optionSlider->tickPosition & QSlider::TicksBelow)
                    //tick marks are one unit tall
                    metricValue = 1.0 * unitValue + 0.5;
            break;
        }
        case PM_SliderSpaceAvailable: {       // available space for slider to move
            if (widget) {
                const QSize sliderSize = widget->size();
                qreal margin = 0.0;
                if (m_private->styleManager()->parameter(QLatin1String("hb-param-margin-gene-screen"), margin)) {
                    metricValue = qMax(sliderSize.width(), sliderSize.height()) - 2 * margin;
                    valueFound = true;
                }
            } else if (const QStyleOptionSlider *optionSlider = qstyleoption_cast<const QStyleOptionSlider *>(option)) {
                valueFound = true;
                metricValue = (optionSlider->orientation == Qt::Horizontal) ? optionSlider->rect.width() : optionSlider->rect.height();
            }
            break;
        }
        case PM_DockWidgetTitleBarButtonMargin:
        case PM_DockWidgetTitleMargin:
        case PM_DockWidgetSeparatorExtent:
        case PM_DockWidgetHandleExtent: {
            break; //todo: no suitable values in Hb?
        }
        case PM_DockWidgetFrameWidth: {
            valueFound = false; //was true
            QSize screenSize = HbDeviceProfile::current().logicalSize();
            metricValue = screenSize.width();
            break;
        }
        //Hb does not have tabs. Lets use toolbar layout data
        case PM_TabBarTabHSpace: {
                qreal toolbarWidth = 0.0;
                if (m_private->styleManager()->parameter(QLatin1String("hb-param-widget-chrome-height"), toolbarWidth)) {
                    valueFound = true;
                    qreal iconWidth = 0.0;
                    if (m_private->styleManager()->parameter(QLatin1String("hb-param-graphic-size-function"), iconWidth))
                        metricValue = (toolbarWidth - iconWidth) / 2;
                    else
                        metricValue = toolbarWidth / 2;
                }
            break;
        }
        case PM_TabBarTabVSpace: {
                qreal toolbarHeight = 0.0;
                if (m_private->styleManager()->parameter(QLatin1String("hb-param-widget-toolbar-height"), toolbarHeight)) {
                    valueFound = true;
                    qreal iconHeight = 0.0;
                    if (m_private->styleManager()->parameter(QLatin1String("hb-param-graphic-size-function"), iconHeight))
                        metricValue = (toolbarHeight - iconHeight) / 2;
                    else
                        metricValue = toolbarHeight / 2;
                }
                break;
            }
        case PM_TabBarBaseHeight: {
            valueFound = true;
            metricValue = 2.0;
            break;
        }
        case PM_TabBarTabOverlap:
        case PM_TabBarBaseOverlap: {
            metricValue = 1.0;
            valueFound = true;
            break;
        }
        case PM_ProgressBarChunkWidth: {
            valueFound = m_private->styleManager()->parameter(QLatin1String("hb-param-widget-progress-bar-height"), metricValue);
            break;
        }
        case PM_SplitterWidth: {
            //No splitter in Hb, so lets use interactive scrollbar width
            valueFound = m_private->styleManager()->parameter(QLatin1String("hb-param-widget-scroll-bar-interactive-width"), metricValue);
            break;
        }
        case PM_TitleBarHeight: {
            valueFound = m_private->styleManager()->parameter(QLatin1String("hb-param-widget-chrome-height"), metricValue);
            break;
        }
        case PM_MenuScrollerHeight: {
            //No menu scroller in Hb, lets use interactive scrollbar width
            valueFound = m_private->styleManager()->parameter(QLatin1String("hb-param-widget-scroll-bar-interactive-width"), metricValue);
            break;
        }
        case PM_MenuHMargin: {
            valueFound = m_private->styleManager()->parameter(QLatin1String("hb-param-margin-gene-left"), metricValue);
            break;
        }
        case PM_MenuVMargin: {
            valueFound = m_private->styleManager()->parameter(QLatin1String("hb-param-margin-gene-top"), metricValue);
            break;
        }
        case PM_MenuPanelWidth: {
            valueFound = m_private->styleManager()->parameter(QLatin1String("hb-param-background-list-popup"), metricValue);
            break;
        }
        case PM_MenuTearoffHeight: {
            valueFound = true;
            metricValue = 0.0;
            break;
        }
        case PM_MenuDesktopFrameWidth: {
            valueFound = m_private->styleManager()->parameter(QLatin1String("hb-param-background-list-popup"), metricValue);
            break;
        }
        case PM_MenuBarPanelWidth: {
            valueFound = true;
            metricValue = 0.0;
            break;
        }
        case PM_MenuBarItemSpacing: {
            valueFound = m_private->styleManager()->parameter(QLatin1String("hb-param-margin-gene-middle-horizontal"), metricValue);
            break;
        }
        case PM_MenuBarVMargin: {
            valueFound = m_private->styleManager()->parameter(QLatin1String("hb-param-margin-gene-top"), metricValue);
            break;
        }
        case PM_MenuBarHMargin: {
            valueFound = m_private->styleManager()->parameter(QLatin1String("hb-param-margin-gene-left"), metricValue);
            break;
        }
        // Hb indicators are squares and radiobuttons and checkboxes are of similar size.
        case PM_IndicatorWidth:
        case PM_IndicatorHeight:
        case PM_ExclusiveIndicatorWidth:
        case PM_ExclusiveIndicatorHeight:
        case PM_CheckListButtonSize: {
            valueFound = m_private->styleManager()->parameter(QLatin1String("hb-param-graphic-size-secondary"), metricValue);
            // todo: or hb-param-graphic-size-primary-small?
            break;
        }
        case PM_CheckListControllerSize: {
            break;
        }
        case PM_DialogButtonsSeparator: {
            qreal buttonArea = 0.0;
            if (m_private->styleManager()->parameter(QLatin1String("hb-param-widget-chrome-height"), buttonArea)) {
                qreal buttonIconArea = 0.0;
                valueFound = true;
                if (m_private->styleManager()->parameter(QLatin1String("hb-param-graphic-size-function"), buttonIconArea))
                    metricValue = (buttonArea - buttonIconArea) / 2;
                else
                    metricValue = buttonArea / 2;
            }
            break;
        }
        case PM_DialogButtonsButtonWidth:
        case PM_DialogButtonsButtonHeight: {
            valueFound = m_private->styleManager()->parameter(QLatin1String("hb-param-graphic-size-function"), metricValue);
            break;
        }
        case PM_MdiSubWindowFrameWidth: {
            valueFound = true;
            metricValue = 0.75 * unitValue; //0.75 from uiemo documentation (margin for TitleBar)
            break;
        }
        case PM_MdiSubWindowMinimizedWidth: {
            //todo: will it cause issues, if we use Hb minimized titlebar size? Will Qt want to put dialog buttons visible?
            valueFound = m_private->styleManager()->parameter(QLatin1String("hb-param-widget-chrome-height"), metricValue);
            metricValue = metricValue / 6; //from Hb documentation: minimized TitleBar width
            break;
        }
        case PM_HeaderMargin: {
            valueFound = m_private->styleManager()->parameter(QLatin1String("hb-param-margin-gene-middle-vertical"), metricValue);
            break;
        }
        case PM_HeaderMarkSize: {
            valueFound = m_private->styleManager()->parameter(QLatin1String("hb-param-graphic-size-secondary"), metricValue);
            break;
        }
        case PM_HeaderGripMargin: {
            valueFound = m_private->styleManager()->parameter(QLatin1String("hb-param-margin-gene-middle-vertical"), metricValue);
            break;
        }
        case PM_TabBar_ScrollButtonOverlap: {
            valueFound = true; //Lets put the tabs side-by-side
            break;
        }
        case PM_TabBarTabShiftHorizontal:
        case PM_TabBarTabShiftVertical: {
            //todo: should we have tab shifting?
            break;
        }
        case PM_TabBarScrollButtonWidth: {
                qreal margin = 0.0;
                if (m_private->styleManager()->parameter(QLatin1String("hb-param-margin-gene-top"), margin)) {
                    qreal buttonIconWidth = 0.0;
                    if (m_private->styleManager()->parameter(QLatin1String("hb-param-graphic-size-secondary"), buttonIconWidth)) {
                        metricValue = margin * 2 + buttonIconWidth;
                        valueFound = true;
                    }
                }
            break;
        }
        case PM_ToolBarItemSpacing:
        case PM_ToolBarItemMargin:
        case PM_ToolBarFrameWidth: {
            valueFound = true; //Hb Toolbar buttons are laid out with no margins between them
            break;
        }
        case PM_ToolBarHandleExtent: {
            valueFound = m_private->styleManager()->parameter(QLatin1String("hb-param-widget-chrome-height"), metricValue);
            metricValue = (2 *  metricValue) / 3; //Use minimized Chrome width for toolbar handle size.
            break;
        }
        case PM_ToolBarSeparatorExtent: {
            valueFound = m_private->styleManager()->parameter(QLatin1String("hb-param-widget-toolbar-height"), metricValue);
            break;
        }
        case PM_ToolBarExtensionExtent: {
            valueFound = m_private->styleManager()->parameter(QLatin1String("hb-param-graphic-size-secondary"), metricValue);
            break;
        }
        case PM_SpinBoxSliderHeight: {//todo: what's this...
            break;
        }
        case PM_ToolBarIconSize: {
            valueFound = m_private->styleManager()->parameter(QLatin1String("hb-param-graphic-size-function"), metricValue);
            break;
        }
        case PM_LargeIconSize:
        case PM_ListViewIconSize: {
            valueFound = m_private->styleManager()->parameter(QLatin1String("hb-param-graphic-size-primary-large"), metricValue);
            break;
        }
        case PM_TabBarIconSize:
        case PM_SmallIconSize:
        case PM_IconViewIconSize: {
            valueFound = m_private->styleManager()->parameter(QLatin1String("hb-param-graphic-size-primary-small"), metricValue);
            break;
        }
        case PM_FocusFrameVMargin:
        case PM_FocusFrameHMargin: {
            valueFound = true;
            metricValue = 4.0; //from hbstyle.cpp (P_PushButton_focus); value is already in pixels
            break;
        }
        case PM_ToolTipLabelFrameWidth: {
            valueFound = m_private->styleManager()->parameter(QLatin1String("hb-param-background-popup-preview"), metricValue);
            break;
        }
        case PM_CheckBoxLabelSpacing: {
            valueFound = m_private->styleManager()->parameter(QLatin1String("hb-param-margin-gene-middle-horizontal"), metricValue);
            break;
        }
        case PM_SizeGripSize: {
            //todo: AFAIK, Hb does not have sizegrips
            break;
        }
        case PM_MessageBoxIconSize: {
            valueFound = m_private->styleManager()->parameter(QLatin1String("hb-param-graphic-size-primary-large"), metricValue);
            break;
        }
        case PM_ButtonIconSize: {
            valueFound = m_private->styleManager()->parameter(QLatin1String("hb-param-graphic-size-function"), metricValue);
            break;
        }
        case PM_RadioButtonLabelSpacing: {
            valueFound = m_private->styleManager()->parameter(QLatin1String("hb-param-margin-gene-middle-horizontal"), metricValue);
            break;
        }
        case PM_LayoutLeftMargin: {
            valueFound = m_private->styleManager()->parameter(QLatin1String("hb-param-margin-gene-left"), metricValue);
            break;
        }
        case PM_LayoutTopMargin: {
            valueFound = m_private->styleManager()->parameter(QLatin1String("hb-param-margin-gene-top"), metricValue);
            break;
        }
        case PM_LayoutRightMargin: {
            valueFound = m_private->styleManager()->parameter(QLatin1String("hb-param-margin-gene-right"), metricValue);
            break;
        }
        case PM_LayoutBottomMargin: {
            valueFound = m_private->styleManager()->parameter(QLatin1String("hb-param-margin-gene-bottom"), metricValue);
            break;
        }
        case PM_LayoutHorizontalSpacing: {
            valueFound = m_private->styleManager()->parameter(QLatin1String("hb-param-margin-gene-middle-horizontal"), metricValue);
            break;
        }
        case PM_LayoutVerticalSpacing: {
            valueFound = m_private->styleManager()->parameter(QLatin1String("hb-param-margin-gene-middle-vertical"), metricValue);
            break;
        }
        case PM_TabCloseIndicatorWidth:
        case PM_TabCloseIndicatorHeight: {
            valueFound = m_private->styleManager()->parameter(QLatin1String("hb-param-graphic-size-primary-small"), metricValue);
            break;
        }
        case PM_TextCursorWidth: {
            valueFound = true;
            metricValue = 1.0; //directly from Hb designers
            break;
        }
        case PM_SubMenuOverlap:
        default: {
            break;
        }
    }
    if (!valueFound)
        return metricValue = QCommonStyle::pixelMetric(metric, option, widget);
    else
        return metricValue + 0.5; //rounding since hb values are qreals
}

QPixmap QHbStyle::standardPixmap(StandardPixmap standardPixmap, const QStyleOption *option,
                           const QWidget *widget) const
{
    return QCommonStyle::standardPixmap(standardPixmap, option, widget);
}

void QHbStyle::polish(QWidget *widget)
{
    QCommonStyle::polish(widget);

    if (!widget)
        return;

    if (false
#ifndef QT_NO_SCROLLBAR
        || qobject_cast<QScrollBar *>(widget)
#endif
        ) {
        widget->setAttribute(Qt::WA_OpaquePaintEvent, false);
    }

    if (m_private->isDialog(widget)) {
        widget->setAttribute(Qt::WA_StyledBackground);
    }

    m_private->polishFont(widget);
    m_private->polishPalette(widget);

#ifndef QT_NO_PROGRESSBAR
    if (qobject_cast<QProgressBar *>(widget))
        widget->installEventFilter(this);
#endif
}

void QHbStyle::polish(QApplication *app)
{
    QCommonStyle::polish(app);
}

void QHbStyle::polish(QPalette &palette)
{
    QCommonStyle::polish(palette);
}

void QHbStyle::unpolish(QWidget *widget)
{
    if (!widget)
        return;

    if (false
    #ifndef QT_NO_SCROLLBAR
        || qobject_cast<QScrollBar *>(widget)
    #endif
        )
        widget->setAttribute(Qt::WA_OpaquePaintEvent);

    if (m_private->isDialog(widget)) {
        widget->setAttribute(Qt::WA_StyledBackground, false);
    }

    QCommonStyle::unpolish(widget);

#ifndef QT_NO_PROGRESSBAR
    if (qobject_cast<QProgressBar *>(widget)) {
        widget->removeEventFilter(this);
    }
#endif
}

void QHbStyle::unpolish(QApplication *app)
{
    QCommonStyle::unpolish(app);
}

QPalette QHbStyle::standardPalette() const
{
    // This function not called if system support system colors
    return QCommonStyle::standardPalette();
}

QIcon QHbStyle::standardIconImplementation(StandardPixmap standardIcon, const QStyleOption *option,
                                     const QWidget *widget) const
{
    QRect iconRect;
    if (option) {
        iconRect = option->rect;
    } else if (widget) {
        iconRect = widget->rect();
    } else {
        iconRect.setWidth(proxy()->pixelMetric(PM_SmallIconSize, option, widget));
        iconRect.setHeight(proxy()->pixelMetric(PM_SmallIconSize, option, widget));
    }
    QString iconName;

    switch (standardIcon) {
        case SP_TitleBarMenuButton: {
            break;
        }
        case SP_TitleBarMinButton: {
            break;
        }
        case SP_TitleBarMaxButton: {
            break;
        }
        case SP_TitleBarCloseButton: {
            break;
        }
        case SP_TitleBarNormalButton: {
            break;
        }
        case SP_TitleBarShadeButton: {
            break;
        }
        case SP_TitleBarUnshadeButton: {
            break;
        }
        case SP_TitleBarContextHelpButton: {
            break;
        }
        case SP_DockWidgetCloseButton: {
            break;
        }
        case SP_MessageBoxInformation: {
            iconName = QLatin1String("note_info");
            break;
        }
        case SP_MessageBoxWarning: {
            iconName = QLatin1String("note_warning");
            break;
        }
        case SP_MessageBoxCritical: {
            iconName = QLatin1String("note_error");
            break;
        }
        case SP_MessageBoxQuestion: {
            iconName = QLatin1String("qtg_large_question_mark");
            break;
        }
        case SP_DesktopIcon: {
            break;
        }
        case SP_TrashIcon: {
            break;
        }
        case SP_ComputerIcon: {
            iconName = QLatin1String("qtg_small_mobile");
            break;
        }
        case SP_DriveFDIcon:
        case SP_DriveHDIcon:
        case SP_DriveCDIcon:
        case SP_DriveDVDIcon: {
            iconName = QLatin1String("qtg_large_mmc");
            break;
        }
        case SP_DriveNetIcon: {
            break;
        }
        case SP_DirOpenIcon: {
            break;
        }
        case SP_DirClosedIcon: {
            break;
        }
        case SP_DirLinkIcon: {
            break;
        }
        case SP_FileIcon: {
            iconName = QLatin1String("qtg_large_notes");
            break;
        }
        case SP_FileLinkIcon: {
            break;
        }
        case SP_ToolBarHorizontalExtensionButton: {
            iconName = QLatin1String("qtg_mono_more");
            break;
        }
        case SP_ToolBarVerticalExtensionButton: {
            iconName = QLatin1String("qtg_mono_more");
            break;
        }
        case SP_FileDialogStart: {
            break;
        }
        case SP_FileDialogEnd: {
            break;
        }
        case SP_FileDialogToParent: {
            break;
        }
        case SP_FileDialogNewFolder: {
            break;
        }
        case SP_FileDialogDetailedView: {
            break;
        }
        case SP_FileDialogInfoView: {
            iconName = QLatin1String("qtg_large_info");
            break;
        }
        case SP_FileDialogContentsView: {
            break;
        }
        case SP_FileDialogListView: {
            break;
        }
        case SP_FileDialogBack: {
            break;
        }
        case SP_DirIcon: {
            break;
        }
        case SP_DialogOkButton: {
            iconName = QLatin1String("qtg_large_ok");
            break;
        }
        case SP_DialogCancelButton: {
            break;
        }
        case SP_DialogHelpButton: {
            iconName = QLatin1String("qtg_large_help");
            break;
        }
        case SP_DialogOpenButton: {
            break;
        }
        case SP_DialogSaveButton: {
            break;
        }
        case SP_DialogCloseButton: {
            break;
        }
        case SP_DialogApplyButton: {
            iconName = QLatin1String("qtg_large_ok");
            break;
        }
        case SP_DialogResetButton: {
            break;
        }
        case SP_DialogDiscardButton: {
            iconName = QLatin1String("qtg_small_fail");
            break;
        }
        case SP_DialogYesButton: {
            break;
        }
        case SP_DialogNoButton: {
            break;
        }
        case SP_ArrowUp: {
            break;
        }
        case SP_ArrowDown: {
            break;
        }
        case SP_ArrowLeft: {
            break;
        }
        case SP_ArrowRight: {
            break;
        }
        case SP_ArrowBack: {
            break;
        }
        case SP_ArrowForward: {
            break;
        }
        case SP_DirHomeIcon: {
            break;
        }
        case SP_CommandLink: {
            break;
        }
        case SP_VistaShield: {
            break;
        }
        case SP_BrowserReload: {
            break;
        }
        case SP_BrowserStop: {
            break;
        }
        case SP_MediaPlay: {
            iconName = QLatin1String("qtg_graf_progslider_handle_play_normal");
            break;
        }
        case SP_MediaStop: {
            break;
        }
        case SP_MediaPause: {
            iconName = QLatin1String("qtg_graf_progslider_handle_pause_normal");
            break;
        }
        case SP_MediaSkipForward: {
            break;
        }
        case SP_MediaSkipBackward: {
            break;
        }
        case SP_MediaSeekForward: {
            break;
        }
        case SP_MediaSeekBackward: {
            break;
        }
        case SP_MediaVolume: {
            iconName = QLatin1String("qgn_indi_nslider_unmuted");
            break;
        }
        case SP_MediaVolumeMuted: {
            iconName = QLatin1String("qgn_indi_nslider_muted");
            break;
        }
        default: {
            break;
        }
    }

    QIcon icon;
    if (!iconName.isNull()) {
        HbIcon* hbicon = new HbIcon(iconName);
        hbicon->setSize(iconRect.size());
        icon =  QIcon(hbicon->qicon());
        delete hbicon;
    } else {
        icon = QCommonStyle::standardIconImplementation(standardIcon, option, widget);
    }
    return icon;
}

int QHbStyle::layoutSpacingImplementation(QSizePolicy::ControlType control1,
                                    QSizePolicy::ControlType control2,
                                    Qt::Orientation orientation,
                                    const QStyleOption *option,
                                    const QWidget *widget) const
{
    return QCommonStyle::layoutSpacingImplementation(control1, control2, orientation, option, widget);
}

bool QHbStyle::eventFilter(QObject *watched, QEvent *event)
{
    switch( event->type()) {
#ifndef QT_NO_PROGRESSBAR
        case QEvent::StyleChange:
        case QEvent::Show: {
            if (m_private->animationGroup()->state() != QAbstractAnimation::Running )
                m_private->animationGroup()->start();
            break;
        }
        case QEvent::ApplicationLayoutDirectionChange:
        case QEvent::LayoutDirectionChange:
        case QEvent::Resize:
        case QEvent::Destroy:
        case QEvent::Hide: {
            if (QProgressBar *bar = qobject_cast<QProgressBar *>(watched)) {
                const int count = m_private->animationGroup()->animationCount();
                for (int i(count-1); i >= 0; i--) {
                    QAbstractAnimation* animation = m_private->animationGroup()->animationAt(i);
                    if (QPropertyAnimation *pAnimation = qobject_cast<QPropertyAnimation *>(animation)) {
                        QHbStyleAnimation* styleAnimation = qobject_cast<QHbStyleAnimation *>(pAnimation->targetObject());
                        if (bar == styleAnimation->target()) {
                            animation = m_private->animationGroup()->takeAnimation(i);
                            animation->deleteLater();
                        }
                    }
                }
                if (m_private->animationGroup()->animationCount() == 0 &&
                    m_private->animationGroup()->state() == QAbstractAnimation::Running)
                    m_private->animationGroup()->stop();
            }
            break;
        }
#endif // QT_NO_PROGRESSBAR
        default: {
            break;
        }
    };

    return QCommonStyle::eventFilter(watched, event);
}

void QHbStyle::animateControl(ControlElement element, const QStyleOption *option,
                                     QPainter *painter, const QWidget *widget) const{
    switch(element) {
        case CE_ProgressBarContents: {
            if (const QProgressBar *bar = static_cast<const QProgressBar *>(widget)) {
                 if (bar->minimum() == 0 && bar->maximum() == 0) {
                     QHbStyleAnimation* styleAnimation = 0;
                     const int count = m_private->animationGroup()->animationCount();
                     bool alreadyAnimated = false;
                     for (int i(0); i < count; i++) {
                         QAbstractAnimation* animation = m_private->animationGroup()->animationAt(i);
                         if (QPropertyAnimation *pAnimation = qobject_cast<QPropertyAnimation *>(animation)) {
                             styleAnimation = qobject_cast<QHbStyleAnimation *>(pAnimation->targetObject());
                             if (bar == styleAnimation->target()) {
                                 alreadyAnimated = true;
                                 break;
                             }
                         }
                     }
                     if (!alreadyAnimated) {
                         QHbStyleAnimation* target = new QHbStyleAnimation(const_cast<QProgressBar*>(bar));
                         target->createAnimationIcon(CE_ProgressBarContents, bar->orientation());
                         QPropertyAnimation* animation = new QPropertyAnimation(target, "point");
                         animation->setLoopCount(-1); //run until stopped
                         const int chunk = pixelMetric(PM_ProgressBarChunkWidth, option, widget)-1;
                         if (bar->orientation()== Qt::Horizontal) {
                             if ((option->direction == Qt::LeftToRight) ^ const_cast<QProgressBar*>(bar)->invertedAppearance()) {
                                 animation->setStartValue(bar->rect().topLeft());
                                 animation->setEndValue(QPoint(bar->rect().x()-chunk, bar->rect().y()));
                             } else {
                                 animation->setStartValue(QPoint(bar->rect().x()-chunk, bar->rect().y()));
                                 animation->setEndValue(bar->rect().topLeft());
                             }
                         }
                         else {
                             if ((option->direction == Qt::LeftToRight) ^ const_cast<QProgressBar*>(bar)->invertedAppearance()) {
                                 animation->setStartValue(bar->rect().topLeft());
                                 animation->setEndValue(QPoint(bar->rect().x(), bar->rect().y()-chunk));
                             } else {
                                 animation->setStartValue(QPoint(bar->rect().x(), bar->rect().y()-chunk));
                                 animation->setEndValue(bar->rect().topLeft());
                             }
                         }
                         m_private->animationGroup()->addAnimation(animation);
                     } else {
                        styleAnimation->paintAnimation(painter);
                     }
                 }
             }
            break;
        }
        default: {
            break;
        }
    }

    if (m_private->animationGroup()->animationCount() > 0 &&
        m_private->animationGroup()->state() != QAbstractAnimation::Running)
        m_private->animationGroup()->start();
}

bool QHbStylePrivate::drawItem(Item part, QPainter *painter, const QRect &rect, ItemStates state, const QColor &color)
{
    QString iconName;
    switch(part) {
        case SP_Arrow: {
            if (state & SS_Up)
                iconName = QLatin1String("qgn_indi_input_arrow_up");
            else if (state & SS_Down)
                iconName = QLatin1String("qgn_indi_input_arrow_down");
            else if (state && SS_Left)
                iconName = QLatin1String("qgn_indi_input_arrow_left");
            else
                iconName = QLatin1String("qgn_indi_input_arrow_right");
            break;
        }
        case SP_BoxButton: {
            if (state & SS_Disabled)
                iconName = QString("qtg_graf_combobox_button_disabled");
            else if (state & SS_Pressed)
                iconName = QString("qtg_graf_combobox_button_pressed");
            else if (state & SS_Selected)
                iconName = QString("qtg_graf_combobox_button_highlight");
            else
                iconName = QString("qtg_graf_combobox_button_normal");
            break;
        }
        case SP_CheckBoxIndicator: {
            if (state & SS_Inactive)
                iconName = QLatin1String("qtg_small_unselected");
            else
                iconName = QLatin1String("qtg_small_selected");
            break;
        }
        case SP_HeaderOrderIndicator: {
            iconName = QLatin1String("qtg_mono_sort");
            break;
        }
        case SP_ItemDecoration: {
            if (state & SS_Selected)
                iconName = QString("qtg_small_tick");
            break;
        }
        case SP_MenuSeparator: {
            iconName = QLatin1String("qtg_graf_popup_separator");
            break;
        }
        case SP_RadioButtonIndicator: {
            if (state & SS_Inactive)
                iconName = QLatin1String("qtg_small_radio_unselected");
            else
                iconName = QLatin1String("qtg_small_radio_selected");
            break;
        }
        case SP_SliderHandle: {
            if (state & SS_Horizontal) {
                if (state & SS_Pressed)
                    iconName = QLatin1String("qtg_graf_slider_h_handle_pressed");
                else
                    iconName = QLatin1String("qtg_graf_slider_h_handle_normal");
            } else {
                if (state & SS_Pressed)
                    iconName = QLatin1String("qtg_graf_slider_v_handle_pressed");
                else
                    iconName = QLatin1String("qtg_graf_slider_v_handle_normal");
            }
            break;
        }
        case SP_SliderTick: {
            if (state & SS_Horizontal)
                iconName = QLatin1String("qtg_graf_slider_h_tick_major");
            else
                iconName = QLatin1String("qtg_graf_slider_v_tick_major");
            break;
        }
        case SP_SeparatorLine: {
            // @todo: or "qtg_graf_popup_separator" and states and rotation
            if (state & SS_Horizontal)
                iconName = QLatin1String("qtg_graf_devider_h_thin");
            else
                iconName = QLatin1String("qtg_graf_devider_v_thin");
            break;
        }
        case SP_TreeViewExpanded: {
            iconName = QLatin1String("qtg_small_expand");
            break;
        }
        case SP_TreeViewCollapsed: {
            iconName = QLatin1String("qtg_small_collapse");
            break;
        }
        case SP_SubMenuIndicator:
        default: {
            return false;
        }
    }
    if (!iconName.isNull() && !rect.isEmpty()) {
        HbIcon *icon = new HbIcon(iconName);
        icon->setSize(rect.size());
        if (color.spec() != QColor::Invalid)
            icon->setColor(color);
        if (state & SS_Mirrored)
            icon->setMirroringMode(HbIcon::Forced);

        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);

        if (state & SS_Flipped) {
            QTransform m;
            m.rotate(180);
            m.translate(-rect.width() - 2 * rect.left(), -rect.height() - 2 * rect.top());
            painter->setTransform(m, true);
        }
        icon->paint(painter, rect, Qt::IgnoreAspectRatio, Qt::AlignCenter, QIcon::Normal, QIcon::On);
        painter->restore();
        delete icon;
    }
    return true;
}

bool QHbStylePrivate::drawMultiPartItem(MultiPartItem multiPart, QPainter *painter, const QRect &rect, ItemStates state)
{
    //Q_Q(QHbStyle);

    if (m_frameDrawer.isNull())
        m_frameDrawer.reset(new HbFrameDrawer());

    HbFrameDrawer::FrameType frameType = HbFrameDrawer::Undefined;
    QString frameName;
    qreal border = 0.0;
    HbIcon::MirroringMode mirrorMode = HbIcon::Default;
    bool fillRect = false;

    QStringList framePartList;
    QString frameGraphicsFooter;
    QString frameGraphicsHeader;
    switch (multiPart) {
        case SM_BoxFrame: {
            fillRect = true;
            frameType = HbFrameDrawer::ThreePiecesHorizontal;
            if (state & SS_Disabled)
                frameName = QString("qtg_fr_combobox_disabled");
            else if (state & SS_Pressed)
                frameName = QString("qtg_fr_combobox_pressed");
            else if (state & SS_Edited)
                frameName = QString("qtg_fr_combobox_edit");
            else if (state & SS_Selected)
                frameName = QString("qtg_fr_combobox_highlight");
            else
                frameName = QString("qtg_fr_combobox_normal");
            break;
        }
        case SM_Dialog: {
            frameName = QLatin1String("qtg_fr_popup");
            frameType = HbFrameDrawer::NinePieces;
            break;
        }
        case SM_GroupBox: {
            styleManager()->parameter(QLatin1String("hb-param-background-groupbox"), border);
            if (state & SS_Pressed)
                frameName = QLatin1String("qtg_fr_groupbox_pressed");
            else if (state & SS_Selected && state & SS_Active)
                frameName = QLatin1String("qtg_fr_groupbox_highlight");
            else
                frameName = QLatin1String("qtg_fr_groupbox_normal");
            frameType = HbFrameDrawer::NinePieces;
            break;
        }
        case SM_GroupBoxTitle: {
            frameName = QLatin1String("qtg_fr_groupbox");
            frameType = HbFrameDrawer::NinePieces;
            break;
        }
        case SM_ItemViewItem: {
            if (state & SS_Pressed)
                frameName = QLatin1String("qtg_fr_list_pressed");
            else if (state & SS_Focused)
                frameName = QLatin1String("qtg_fr_list_highlight");
            frameType = HbFrameDrawer::NinePieces;
            styleManager()->parameter(QLatin1String("hb-param-background-list-main"), border);
            break;
        }
        case SM_TextEdit: //@todo: fallthrough for now, since no specific graphic for editors in releases
        case SM_LineEdit: {
            styleManager()->parameter(QLatin1String("hb-param-background-editor"), border);
            if (state & SS_Selected)
                frameName = QLatin1String("qtg_fr_lineedit_highlight");
            else
                frameName = QLatin1String("qtg_fr_lineedit_normal");
            frameType = HbFrameDrawer::NinePieces;
            break;
        }
        case SM_ListParent: {
            if (state & SS_Pressed)
                frameName = QLatin1String("qtg_fr_list_pressed");
            else if (state & SS_Focused)
                frameName = QLatin1String("qtg_fr_list_highlight");
            else
                frameName = QLatin1String("qtg_fr_list_parent_normal");
            frameType = HbFrameDrawer::NinePieces;
            styleManager()->parameter(QLatin1String("hb-param-background-list-main"), border);
            break;
        }
        case SM_Menu: {
            frameName = QLatin1String("qtg_fr_popup_secondary");
            frameType = HbFrameDrawer::NinePieces;
            break;
        }
        case SM_MenuScroller: {
            if (state & SS_Down)
                frameName = QLatin1String("qtg_graf_list_mask_b");
            else if (state & SS_Up)
                frameName = QLatin1String("qtg_graf_list_mask_t");
            frameType = HbFrameDrawer::OnePiece;
            break;
        }
        case SM_MenuItem: {
            if (state & SS_Pressed)
                frameName = QLatin1String("qtg_fr_popup_list_pressed");
            else if (state & SS_Selected)
                frameName = QLatin1String("qtg_fr_popup_list_highlight");
            else
                frameName = QLatin1String("qtg_fr_popup_list_normal");
            frameType = HbFrameDrawer::NinePieces;
            break;
        }
        case SM_Panel: {
            frameName = QLatin1String("qtg_fr_groupbox_normal");
            frameType = HbFrameDrawer::NinePieces;
            styleManager()->parameter(QLatin1String("hb-param-background-list-main"), border);
            break;
        }
        case SM_ProgressBarGroove: {
            fillRect = true;
            if (state & SS_Horizontal) {
                frameName = QLatin1String("qtg_fr_progbar_h_frame");
                frameType = HbFrameDrawer::ThreePiecesHorizontal;
            } else {
                frameName = QLatin1String("qtg_fr_progbar_v_frame");
                frameType = HbFrameDrawer::ThreePiecesVertical;
            }
            break;
        }
        case SM_ProgressBarIndicator: {
            fillRect = true;
            if (state & SS_Horizontal) {
                frameName = QLatin1String("qtg_fr_progbar_h_filled");
                frameType = HbFrameDrawer::ThreePiecesHorizontal;
            } else {
                frameName = QLatin1String("qtg_fr_progbar_v_filled");
                frameType = HbFrameDrawer::ThreePiecesVertical;
            }
            break;
        }
        //@todo: enable separate graphic for texteditor. Graphic was not included in the wk12 release.
        /*case SM_TextEdit: {
            styleManager()->parameter(QLatin1String("hb-param-background-editor"), border);
            if (state & SS_Selected)
                frameName = QLatin1String("qtg_fr_textedit_highlight");
            else
                frameName = QLatin1String("qtg_fr_textedit_normal");
            frameType = HbFrameDrawer::NinePieces;
            break;
        }*/
        case SM_ToolButton: {
            frameType = HbFrameDrawer::ThreePiecesHorizontal;
            frameGraphicsHeader = QLatin1String("qtg_fr_tb_h_");

            framePartList << QLatin1String("_cl") << QLatin1String("_c") << QLatin1String("_cr");

            if (state & SS_Disabled)
                frameGraphicsFooter = QLatin1String("disabled");
            else if (state & SS_Pressed)
                frameGraphicsFooter = QLatin1String("pressed");
            else if (state & SS_Selected)
                frameGraphicsFooter = QLatin1String("highlight");
            else
                frameGraphicsFooter = QLatin1String("normal");
            break;
        }
        case SM_PushButton: {
             frameType = HbFrameDrawer::NinePieces;
             if (state & SS_Disabled)
                 frameName = QLatin1String("qtg_fr_btn_disabled");
             else if (state & SS_Pressed)
                 frameName = QLatin1String("qtg_fr_btn_pressed");
             else if (state & SS_Selected)
                 frameName = QLatin1String("qtg_fr_btn_highlight");
             else if (state & SS_Latched)
                 frameName = QLatin1String("qtg_fr_btn_latched");
             else
                 frameName = QLatin1String("qtg_fr_btn_normal");
             styleManager()->parameter(QLatin1String("hb-param-background-button"), border);
             break;
        }
        case SM_ScrollBarGroove: {
            fillRect = true;
            if (state & SS_Horizontal) {
                if (state & SS_Pressed)
                    frameName = QLatin1String("qtg_fr_scroll_h_active_frame_pressed");
                else
                    frameName = QLatin1String("qtg_fr_scroll_h_active_frame_normal");
                frameType = HbFrameDrawer::ThreePiecesHorizontal;
            } else {
                if (state & SS_Pressed)
                    frameName = QLatin1String("qtg_fr_scroll_v_active_frame_pressed");
                else
                    frameName = QLatin1String("qtg_fr_scroll_v_active_frame_normal");
                frameType = HbFrameDrawer::ThreePiecesVertical;
            }
            break;
        }
        case SM_ScrollBarHandle: {
            fillRect = true;
            if (state & SS_Horizontal) {
                if (state & SS_Pressed)
                    frameName = QLatin1String("qtg_fr_scroll_h_active_handle_pressed");
                else
                    frameName = QLatin1String("qtg_fr_scroll_h_active_handle_normal");
                frameType = HbFrameDrawer::ThreePiecesHorizontal;
            } else {
                if (state & SS_Pressed)
                    frameName = QLatin1String("qtg_fr_scroll_v_active_handle_pressed");
                else
                    frameName = QLatin1String("qtg_fr_scroll_v_active_handle_normal");
                frameType = HbFrameDrawer::ThreePiecesVertical;
            }
            break;
        }
        case SM_SliderGroove: {
            fillRect = true;
            if (state & SS_Horizontal) {
                if (state & SS_Pressed)
                    frameName = QLatin1String("qtg_fr_slider_h_frame_pressed");
                else
                    frameName = QLatin1String("qtg_fr_slider_h_frame_normal");
                frameType = HbFrameDrawer::ThreePiecesHorizontal;
            } else {
                if (state & SS_Pressed)
                    frameName = QLatin1String("qtg_fr_slider_v_frame_pressed");
                else
                    frameName = QLatin1String("qtg_fr_slider_v_frame_normal");
                frameType = HbFrameDrawer::ThreePiecesVertical;
            }
            break;
        }
        case SM_SliderProgress: {
            fillRect = true;
            if (state & SS_Filled) {
                if (state & SS_Horizontal) {
                    frameName = QLatin1String("qtg_fr_slider_h_filled");
                    frameType = HbFrameDrawer::ThreePiecesHorizontal;
                } else {
                    frameName = QLatin1String("qtg_fr_slider_v_filled");
                    frameType = HbFrameDrawer::ThreePiecesVertical;
                }
            }
            break;
        }
        case SM_TableItem: {
            if (state & SS_Pressed)
                frameName = QLatin1String("qtg_fr_grid_pressed");
            else if (state & SS_Focused)
                frameName = QLatin1String("qtg_fr_grid_highlight");
            frameType = HbFrameDrawer::NinePieces;
            styleManager()->parameter(QLatin1String("hb-param-background-list-main"), border);
            break;
        }
        case SM_TabShape: {
            if (state & SS_Horizontal) {
                frameType = HbFrameDrawer::ThreePiecesHorizontal;
                frameGraphicsHeader = QLatin1String("qtg_fr_tb_h_");

                if (state & SS_Beginning)
                    framePartList << QLatin1String("_l") << QLatin1String("_c") << QLatin1String("_cr");
                else if (state & SS_Middle)
                    framePartList << QLatin1String("_cl") << QLatin1String("_c") << QLatin1String("_cr");
                else if (state & SS_End)
                    framePartList << QLatin1String("_cl") << QLatin1String("_c") << QLatin1String("_r");
                else
                    framePartList << QLatin1String("_cl") << QLatin1String("_c") << QLatin1String("_r");
            } else if (state & SS_Vertical) {
                frameType = HbFrameDrawer::ThreePiecesVertical;
                frameGraphicsHeader = QLatin1String("qtg_fr_tb_v_");
                if (state & SS_Beginning)
                    framePartList << QLatin1String("_t") << QLatin1String("_c") << QLatin1String("_cb");
                else if (state & SS_Middle)
                    framePartList << QLatin1String("_ct") << QLatin1String("_c") << QLatin1String("_cb");
                else if (state & SS_End)
                    framePartList << QLatin1String("_ct") << QLatin1String("_c") << QLatin1String("_b");
                else
                    framePartList << QLatin1String("_t") << QLatin1String("_c") << QLatin1String("_b");
            }
            if (state & SS_Disabled)
                frameGraphicsFooter = QLatin1String("disabled");
            else if (state & SS_Pressed)
                frameGraphicsFooter = QLatin1String("pressed");
            else if (state & SS_Selected)
                frameGraphicsFooter = QLatin1String("highlight");
            else
                frameGraphicsFooter = QLatin1String("normal");
            break;
        }
        case SM_ToolBarButton: {
            if (state & SS_Horizontal) {
                frameType = HbFrameDrawer::ThreePiecesHorizontal;
                if (state & SS_Disabled)
                    frameName = QLatin1String("qtg_fr_tb_h_disabled");
                else if (state & SS_Pressed)
                    frameName = QLatin1String("qtg_fr_tb_h_pressed");
                else if (state & SS_Latched)
                    frameName = QLatin1String("qtg_fr_tb_h_latched");
                else if (state & SS_Selected)
                    frameName = QLatin1String("qtg_fr_tb_h_highlight");
                else
                    frameName = QLatin1String("qtg_fr_tb_h_normal");
            } else {
                frameType = HbFrameDrawer::ThreePiecesVertical;
                if (state & SS_Disabled)
                    frameName = QLatin1String("qtg_fr_tb_v_disabled");
                else if (state & SS_Pressed)
                    frameName = QLatin1String("qtg_fr_tb_v_pressed");
                else if (state & SS_Latched)
                    frameName = QLatin1String("qtg_fr_tb_v_latched");
                else if (state & SS_Selected)
                    frameName = QLatin1String("qtg_fr_tb_v_highlight");
                else
                    frameName = QLatin1String("qtg_fr_tb_v_normal");
            }
            styleManager()->parameter(QLatin1String("hb-param-background-button"), border);
            break;
        }
        case SM_ToolBarExtension:{
//            fillRect = true;
//            styleManager()->parameter(QLatin1String("hb-param-background-editor"), border);
            frameName = QLatin1String("qtg_fr_tb_ext");
            frameType = HbFrameDrawer::NinePieces;
            }
            break;
        case SM_ToolTip: {
            fillRect = true;
            frameType = HbFrameDrawer::NinePieces;
            frameName = QLatin1String("qtg_fr_popup_preview");
            break;
        }
        case SM_HeaderItem:
        case SM_ThemeBackground:
        case SM_ToolBar:
        default: {
            break;
        }
    }

    if (frameType != HbFrameDrawer::Undefined)
        m_frameDrawer->setFrameType(frameType);
    else
        return false;

    if (!frameName.isNull()) {
        m_frameDrawer->setFrameGraphicsName(frameName);
    } else if (framePartList.count() > 0) {
        m_frameDrawer->setFileNameSuffixList(framePartList);
        m_frameDrawer->setFrameGraphicsName(QString ("%0%1").arg(frameGraphicsHeader).arg(frameGraphicsFooter));
    } else {
        return false;
    }

    m_frameDrawer->setBorderWidth(border);
    m_frameDrawer->setFillWholeRect(fillRect);
    if (state & SS_Mirrored)
        mirrorMode = HbIcon::Forced;

    m_frameDrawer->setMirroringMode(mirrorMode);

    const bool rotated = (state & SS_Flipped || state & SS_RotatedRight || state & SS_RotatedLeft) ? true : false;
    QRect validRect = rect;
    if (rotated) {
        QTransform m;
        painter->save();

        //Calculate new coordinates
        int newX = 0, newY = 0;
        if (state & SS_RotatedRight) {
            newX = 0;
            newY = rect.y() + rect.height() - 1;
        } else if (state & SS_RotatedLeft) {
            newX = rect.width();
            newY = rect.y() - 1;
        } else if (state & SS_Flipped) {
            if (state & SS_Horizontal) {
                newX = rect.width() + 2 * rect.left();
                newY = rect.height() + rect.top();
            } else {
                newX = rect.width() + rect.left();
                newY = rect.height() + 2 * rect.top();
            }
        }

        //Translate rect and transform
        if ((state & SS_RotatedRight) || (state & SS_RotatedLeft))
            validRect.setRect(0, 0, rect.height(), rect.width());
        m.translate(newX, newY);

        // Set rotation
        int rotation = 0;
        if (state & SS_Flipped)
            rotation = 180;
        else if (state & SS_RotatedRight)
            rotation = -90;
        else if (state & SS_RotatedLeft)
            rotation = 90;
        m.rotate(rotation);
        painter->setTransform(m, true);
    }
    m_frameDrawer->paint(painter, validRect);

    if (rotated)
        painter->restore();

    //Need to clear the list after use.
    framePartList.clear();
    m_frameDrawer->setFileNameSuffixList(framePartList);
    return true;
}

bool QHbStylePrivate::isDialog(const QWidget *widget)
{
    return (widget ? (widget->windowType() == Qt::Dialog) : false);
}

bool QHbStylePrivate::hbParameter(const QString &parameterName, int &value)
{
    bool retValue = false;
    qreal valueInReal = 0.0;
    retValue = styleManager()->parameter(parameterName, valueInReal);
    valueInReal += 0.5; //to make the real->int to round correctly
    value = valueInReal;
    return retValue;
}

void QHbStylePrivate::polishFont(QWidget *widget)
{
    HbFontSpec::Role fontRole = HbFontSpec::Undefined;
    qreal fontSize = 0.0;
    bool valueFound = false;

    //Widget font role specifications from hb documentation.
    if (false
#ifndef QT_NO_COMBOBOX
        || qobject_cast<QComboBox *>(widget)
#endif
#ifndef QT_NO_SPINBOX
        || qobject_cast<QSpinBox *>(widget)
#endif
        || qobject_cast<QRadioButton *>(widget)
        ) {
        valueFound = styleManager()->parameter(QLatin1String("b-param-text-height-secondary"), fontSize);
        fontRole = HbFontSpec::Primary;
    } else if (false
        || qobject_cast<QPushButton *>(widget)
#ifndef QT_NO_TOOLBUTTON
        || qobject_cast<QToolButton *>(widget)
#endif
#ifndef QT_NO_TABWIDGET
        || qobject_cast<QTabWidget *>(widget)
#endif
        ) {
        valueFound = styleManager()->parameter(QLatin1String("hb-param-text-height-tiny"), fontSize);
        fontRole = HbFontSpec::Primary;
    } else if (false
#ifndef QT_NO_HEADERVIEW
        || qobject_cast<QHeaderView *>(widget)
#endif
#ifndef QT_NO_LISTVIEW
        || qobject_cast<QListView *>(widget)
#endif
#ifndef QT_NO_TABLEVIEW
        || qobject_cast<QTableView *>(widget)
#endif
#ifndef QT_NO_TREEVIEW
        || qobject_cast<QTreeView *>(widget)
#endif
#ifndef QT_NO_LINEEDIT
        || qobject_cast<QLineEdit *>(widget)
#endif
#ifndef QT_NO_TEXTEDIT
        || qobject_cast<QTextEdit *>(widget)
#endif
        || qobject_cast<QSlider *>(widget)
#ifndef QT_NO_SLIDER
        || qobject_cast<QGroupBox *>(widget)
#endif
        || qobject_cast<QCheckBox *>(widget)
        ) {
        fontRole = HbFontSpec::Secondary;
        valueFound = styleManager()->parameter(QLatin1String("hb-param-text-height-secondary"), fontSize);
    } else if (false
#ifndef QT_NO_PROGRESSBAR
        || qobject_cast<QProgressBar *>(widget)
#endif
        ) {
        fontRole = HbFontSpec::Secondary;
        valueFound = styleManager()->parameter(QLatin1String("hb-param-text-height-tiny"), fontSize);
    }

    HbFontSpec *fontSpec = new HbFontSpec(fontRole);
    if (valueFound) {
        fontSpec->setTextHeight(fontSize);
        QFont widgetFont = fontSpec->font();
        widgetFont.setPixelSize(fontSpec->font().pixelSize());
        widget->setFont(widgetFont);
    }
    delete fontSpec;
}

void QHbStylePrivate::polishPalette(QWidget *widget)
{
    QPalette widgetPalette = widget->palette();
    if (false
#ifndef QT_NO_TEXTEDIT
        || qobject_cast<QTextEdit *>(widget)
#endif
    ) {
        widgetPalette.setColor(QPalette::Active, QPalette::Highlight, HbColorScheme::color("qtc_lineedit_marker_normal"));
        widgetPalette.setColor(QPalette::Active, QPalette::HighlightedText, HbColorScheme::color("qtc_lineedit_selected"));
        widgetPalette.setColor(QPalette::Active, QPalette::Text, HbColorScheme::color("qtc_lineedit_normal"));
        //QTextEdits have specific graphic in QHbStyle for background
        widgetPalette.setColor(QPalette::Active, QPalette::Base, Qt::transparent);
    } else if (false
#ifndef QT_NO_LINEEDIT
        || qobject_cast<QLineEdit *>(widget)
#endif
        ) {
        widgetPalette.setColor(QPalette::Active, QPalette::Highlight, HbColorScheme::color("qtc_lineedit_marker_normal"));
        widgetPalette.setColor(QPalette::Active, QPalette::HighlightedText, HbColorScheme::color("qtc_lineedit_selected"));
        widgetPalette.setColor(QPalette::Active, QPalette::Text, HbColorScheme::color("qtc_lineedit_normal"));
    }
    widget->setPalette(widgetPalette);
}

QT_END_NAMESPACE
