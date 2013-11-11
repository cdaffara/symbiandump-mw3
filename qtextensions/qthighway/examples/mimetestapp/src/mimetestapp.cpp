/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <QtGlobal>
#include <QApplication>
#include <QKeyEvent>
#include <QLabel>
#include <QStackedWidget>
#include <QImageReader>
#include <QDebug>
#include <QTimer>
#include <QPushButton>
#include <QList>
#include <QLineEdit>
#include <QString>
#include <QCheckBox>
#include <QAction>
#include <QListView>
#include <QUrl>
#include <math.h>

#include "mimetestapp.h"

// Erroneus services
#define ERR_INTERFACE1 "xxxx.yyy"
#define ERR_OPERATION1 "dial(QString,QString)"

MimeTestApp::MimeTestApp(QWidget *parent, Qt::WFlags f)
    : QWidget(parent, f)
{
    /* Adjust the palette */
#if defined(Q_WS_S60)
    QPalette p = qApp->palette();
    QColor color(192,192,192);
    QColor bg(201,250,250);
    p.setColor(QPalette::Highlight, color.lighter(200));
    p.setColor(QPalette::Text, Qt::black);
    p.setColor(QPalette::Base, bg);
    p.setColor(QPalette::WindowText, Qt::black);
    p.setColor(QPalette::Window, bg);
    p.setColor(QPalette::ButtonText, Qt::black);
    p.setColor(QPalette::Button, color.lighter(150));
    p.setColor(QPalette::Link, QColor(240,40,40));

    qApp->setPalette(p);
#endif

    QPushButton *quitButton = new QPushButton(tr("quit"));
    connect(quitButton, SIGNAL(clicked()), qApp, SLOT(quit()));

    vl = new QVBoxLayout;
    vl->setMargin(0);
    vl->setSpacing(0);

    QStringList args = QApplication::arguments();
    foreach (QString arg, args)
    {
        QLabel *label = new QLabel(arg);
        vl->addWidget(label);
    }
    vl->addWidget(quitButton);
    setLayout(vl);

#if defined(Q_WS_X11) || defined(Q_WS_WIN)
    setFixedSize(QSize(360,640)); // nHD
#elif defined(Q_WS_S60)
    showMaximized();
    showFullScreen();
#endif
}

MimeTestApp::~MimeTestApp()
{
}


