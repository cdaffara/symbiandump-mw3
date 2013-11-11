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

#ifndef MIMETESTAPP_H
#define MIMETESTAPP_H

#include <QVBoxLayout>
#include <QWidget>
#include <QLabel>
#include <xqserviceprovider.h>
#include <xqsharablefile.h>

class FileService;
class MimeTestApp : public QWidget
{
    Q_OBJECT
            
public:
        MimeTestApp( QWidget *parent = 0, Qt::WFlags f = 0 );
        ~MimeTestApp();
        
        void setLabel(QString label);

private slots:

private:
        QVBoxLayout *mVl;
        QLabel *mLabel;
        FileService *mFileService;
};


class FileService : public XQServiceProvider
{
    Q_OBJECT
    public:
        FileService( MimeTestApp *parent = 0 );
        ~FileService();

    public slots:
        bool view(QString file);
        bool view(XQSharableFile file);

    private:
        MimeTestApp* mMimeTestApp;
};


#endif
