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

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QTime>

#include "mainview.h"
#include "dummydatagen.h"
#include "settings.h"
#include "resourcemoninterface.h"

class Button;
class Menu;

class Controller : public QObject
{
    Q_OBJECT
public:
    Controller(MainView *mw, Settings &settings, QObject *parent=0);
    ~Controller();

signals:

    void doAutoScroll();

public slots:
    
#if (QT_VERSION >= 0x040600)
    void toggleListItemCaching();
#endif
    void toggleColumns();
    void toggleListType();

private slots:
    void quit();
    void changeTheme();
    void autoScroll();
    void startAutoScroll();
    void changeRotation();

private:
    bool loadPlugin();
    
private:
    enum ListType {
        Simple = 0,
        Recycling = 1
    };

    QString newListName(ListType currentType);

    Q_DISABLE_COPY(Controller)
    MainView *m_mainView;
    ListType m_currentListType;
    Button *m_createListMenuItem;
    Button *m_subTreeMenuItem;
    DummyDataGenerator m_dummyGen;
    int m_scrollDirection;
    QTime m_testDuration;
    Settings &m_settings;
    ResourceMonitorInterface* m_resMon;
};


#endif // CONTROLLER_H
