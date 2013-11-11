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
#include "xqservicelog.h"

#include "mimetestapp.h"

MimeTestApp::MimeTestApp(QWidget *parent, Qt::WFlags f)
    : QWidget(parent, f), mFileService(0), mLabel(0), mVl(0)
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

    mVl = new QVBoxLayout;
    mVl->setMargin(0);
    mVl->setSpacing(0);

    mLabel = new QLabel("");
    mVl->addWidget(mLabel);
    
    QStringList args = QApplication::arguments();
    QString argsTxt;
    foreach (QString arg, args)
    {
        argsTxt += arg + "\n";
    }

    QLabel *cmdArgsLabel = new QLabel(argsTxt);
    mVl->addWidget(cmdArgsLabel);
    
    mVl->addWidget(quitButton);
    setLayout(mVl);

#if defined(Q_WS_X11) || defined(Q_WS_WIN)
    setFixedSize(QSize(360,640)); // nHD
#elif defined(Q_WS_S60)
    showMaximized();
    showFullScreen();
#endif

    // Activate service
    mFileService = new FileService(this);
    
}

MimeTestApp::~MimeTestApp()
{
    delete mFileService;
}

void MimeTestApp::setLabel(QString label)
{
    XQSERVICE_DEBUG_PRINT("MimeTestApp2::setLabel");
    mLabel->setText(label);
}


// ----------FileService---------------

FileService::FileService(MimeTestApp* parent)
: XQServiceProvider(QLatin1String("mimetestapp2.com.nokia.symbian.IFileView"),parent),
  mMimeTestApp(parent)

{
    XQSERVICE_DEBUG_PRINT("MimeTestApp2::FileService (mimetestapp2)");
    publishAll();
}

FileService::~FileService()
{
    XQSERVICE_DEBUG_PRINT("MimeTestApp2::~FileService  (mimetestapp2)");
}

bool FileService::view(QString file)
{
    XQSERVICE_DEBUG_PRINT("MimeTestApp2::view(QString) (mimetestapp2) %s",
                         qPrintable(file));

    QString label = "IFileView:" + QString ("File=%1\n").arg(file);
    mMimeTestApp->setLabel(label);
    
    return true;
}


bool FileService::view(XQSharableFile sf)
{
    XQSERVICE_DEBUG_PRINT("MimeTestApp2::view(XQSharebleFile) (mimetestapp2)");
    QString label = "IFileView:" + QString ("File=%1\n").arg(sf.fileName());

    RFile file;
    bool ok = sf.getHandle( file );
    if (ok)
    {
        HBufC8* data = HBufC8::NewL(100);
        TPtr8 ptr = data->Des();
        TInt err  = file.Read( ptr );
        QString text = QString::fromUtf8((const char *)(data->Ptr()), data->Length());
        XQSERVICE_DEBUG_PRINT("MimeTestApp2::file read,%d,%s", err, qPrintable(text));
        sf.close();
        delete data;
    }

    mMimeTestApp->setLabel(label);
    
    return true;
}


