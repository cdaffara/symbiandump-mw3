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
#ifndef QHBSTYLE_P_H
#define QHBSTYLE_P_H

#include "qhbstyle.h"

#ifndef QT_NO_STYLE_HB

QT_BEGIN_NAMESPACE

class HbStyle;
class QParallelAnimationGroup;

enum Item {
    SP_Arrow,
    SP_BoxButton,
    SP_CheckBoxIndicator,
    SP_CurrentFolderIcon,
    SP_ErasedNoteIcon,
    SP_ErrorNoteIcon,
    SP_HeaderOrderIndicator,
    SP_InfoNoteIcon,
    SP_ItemDecoration,
    SP_LargeMemoryCardIcon,
    SP_MenuIndicator,
    SP_MenuSeparator,
    SP_NewSmallFolderIcon,
    SP_OkNoteIcon,
    SP_QueryNoteIcon,
    SP_RadioButtonIndicator,
    SP_SelectedIndicator,
    SP_SeparatorLine,
    SP_SliderHandle,
    SP_SliderTick,
    SP_SmallFileIcon,
    SP_SmallFolderIcon,
    SP_SubMenuIndicator,
    SP_TreeViewCollapsed,
    SP_TreeViewExpanded,
    SP_WarningNoteIcon,
};

enum MultiPartItem {
    SM_BoxFrame,
    SM_Dialog,
    SM_GroupBox,
    SM_GroupBoxTitle,
    SM_HeaderItem,
    SM_ItemViewItem,
    SM_LineEdit,
    SM_ListParent,
    SM_Menu,
    SM_MenuItem,
    SM_MenuScroller,
    SM_Panel,
    SM_ProgressBarGroove,
    SM_ProgressBarIndicator,
    SM_PushButton,
    SM_ScrollBarGroove,
    SM_ScrollBarHandle,
    SM_SliderGroove,
    SM_SliderProgress,
    SM_TableItem,
    SM_TabShape,
    SM_TextEdit, //todo: or combine this and  SM_LineEdit to "SM_TextEditor"?
    SM_ThemeBackground,
    SM_ToolBar,
    SM_ToolBarButton,
    SM_ToolButton,
    SM_ToolTip,
    SM_ToolBarExtension,
};

enum ItemState {
    SS_Active       = 0x000001,  // "On"
    SS_Inactive     = 0x000002,  // "Off"
    SS_Pressed      = 0x000004,
    SS_Latched      = 0x000008,  // similar to SS_Pressed, but state remains ("toggled")
    SS_Disabled     = 0x000010,
    SS_Filled       = 0x000020,
    SS_Horizontal   = 0x000040,
    SS_Vertical     = 0x000080,
    SS_Selected     = 0x000100,  // item is selected (not necessarily focused)
    SS_Beginning    = 0x000200,  // beginning part of multipart item
    SS_Middle       = 0x000400,  // middle part of multipart item
    SS_End          = 0x000800,  // end part of multipart item
    SS_Flipped      = 0x001000,  // 180 degree rotation
    SS_Mirrored     = 0x002000,  // graphic is drawn mirrored
    SS_Down         = 0x004000,
    SS_Up           = 0x008000,
    SS_Left         = 0x010000,
    SS_Right        = 0x020000,
    SS_RotatedRight = 0x040000,
    SS_RotatedLeft  = 0x080000,
    SS_Edited       = 0x100000,
    SS_Alternate    = 0x200000,
    SS_Focused      = 0x400000   // item is focused (not necessarily selected)
};

Q_DECLARE_FLAGS(ItemStates, ItemState)

class QHbStylePrivate //: public QObjectPrivate
{
    //Q_DECLARE_PUBLIC(QHbStyle)

public:
    QHbStylePrivate();

    virtual ~QHbStylePrivate();

    //Call HbStyle styleManager to avoid name confusion
    HbStyle* styleManager();
    void setStyleManager(HbStyle* style);

    QParallelAnimationGroup* animationGroup();

    //These return true if drawing was done by the style successfully.
    bool drawItem(Item part, QPainter *painter, const QRect &rect, ItemStates state = ItemStates(SS_Active | SS_Horizontal), const QColor &color = QColor(QColor::Invalid));
    bool drawMultiPartItem(MultiPartItem multiPart, QPainter *painter, const QRect &rect, ItemStates state = ItemStates(SS_Active | SS_Horizontal));

    bool isDialog(const QWidget *widget);
    bool hbParameter(const QString &parameterName, int &value);
    void polishFont(QWidget *widget);
    void polishPalette(QWidget *widget);

private:
    HbStyle *m_styleManager;
    QScopedPointer<HbFrameDrawer> m_frameDrawer;
    QScopedPointer<QParallelAnimationGroup> m_animationGroup;
};

QT_END_NAMESPACE

#endif // QT_NO_STYLE_HB

#endif //QHBSTYLE_P_H
