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

#ifndef PHONESERVICE_H
#define PHONESERVICE_H

#include <QWidget>
#include <QModelIndex>
#include <QMap>
#include <QStringList>
#include <xqserviceprovider.h>
#include <xqsharablefile.h>
#include <cntservicescontact.h>

class QLineEdit;
class QPushButton;
class DialerService;
class UriService;
class QLabel;
class FileService;
class XQSharableFile;
class NewDialerService;
class NewUriService;
class NewFileService;


class ServiceApp : public QWidget
{
    Q_OBJECT
public:
    ServiceApp( QWidget *parent = 0, Qt::WFlags f = 0 );
    ~ServiceApp();

    void setLabelNumber(QString label,QString number);

public slots:    
    void endCall();
    
public slots:
    void quit();
    void answerDial();
    void answerUri();
    void answerFile();
    
private:
    QLabel *mLabel;
    QLineEdit *mNumber;
    //QPushButton *mEndCallButton;
    DialerService* mDialService;
    UriService* mUriService;
    FileService* mFileService;
    NewDialerService* mNewDialService;
    NewUriService* mNewUriService;
    NewFileService* mNewFileService;
};

class DialerService : public XQServiceProvider
{
    Q_OBJECT
public:
    DialerService( ServiceApp *parent = 0 );
    ~DialerService();
    
    void complete(QString number);
    bool asyncAnswer() {return mAsyncReqIds.count() > 0;}
    
public slots:
    int dial(const QString& number, bool asyncAnswer);
    QVariant testVariant(QVariant variant);
    CntServicesContactList testContactList(CntServicesContactList list);

protected slots:
   void handleClientDisconnect();
   void handleAnswerDelivered();

protected:
    void showRecipients(QVariant &value);
    void showRecipients(CntServicesContactList &value);

protected:
    ServiceApp* mServiceApp;
    QString mNumber;
    QMap<quint32,int> mAsyncReqIds;
};


class NewDialerService : public XQServiceProvider
{
    Q_OBJECT
    public:
        NewDialerService( ServiceApp *parent = 0 );
        ~NewDialerService();

        void complete(QString number);
        bool asyncAnswer() {return mAsyncReqIds.count() > 0;}

    public slots:
            int dial(const QString& number, bool asyncAnswer);
    QVariant testVariant(QVariant variant);
    CntServicesContactList testContactList(CntServicesContactList list);

    protected slots:
            void handleClientDisconnect();
    void handleAnswerDelivered();

    protected:
        void showRecipients(QVariant &value);
        void showRecipients(CntServicesContactList &value);

    protected:
        ServiceApp* mServiceApp;
        QString mNumber;
        QMap<quint32,int> mAsyncReqIds;
};



class UriService : public XQServiceProvider
{
    Q_OBJECT
    public:
        UriService( ServiceApp *parent = 0 );
        ~UriService();
        bool asyncAnswer() {return mAsyncReqIds.count() > 0;}
        void complete(bool ok);
        
    public slots:
        bool view(const QString& uri);
        bool view(const QString& uri, bool returnValue);
        
    private slots:
        void handleClientDisconnect();
        void handleAnswerDelivered();

    private:
        ServiceApp* mServiceApp;
        QMap<quint32,int> mAsyncReqIds;
        bool mRetValue;
};


class NewUriService : public XQServiceProvider
{
    Q_OBJECT
    public:
        NewUriService( ServiceApp *parent = 0 );
        ~NewUriService();
        bool asyncAnswer() {return mAsyncReqIds.count() > 0;}
        void complete(bool ok);

    public slots:
            bool view(const QString& uri);
    bool view(const QString& uri, bool returnValue);

    private slots:
            void handleClientDisconnect();
    void handleAnswerDelivered();

    private:
        ServiceApp* mServiceApp;
        QMap<quint32,int> mAsyncReqIds;
        bool mRetValue;
};


class FileService : public XQServiceProvider
{
    Q_OBJECT
    public:
        FileService( ServiceApp *parent = 0 );
        ~FileService();
        bool asyncAnswer() {return mAsyncReqIds.count() > 0;}
        void complete(bool ok);

    public slots:
            bool view(QString file);
    bool view(XQSharableFile file);

    private slots:
            void handleClientDisconnect();
    void handleAnswerDelivered();

    private:
        ServiceApp* mServiceApp;
        QMap<quint32,int> mAsyncReqIds;
        bool mRetValue;
};


class NewFileService : public XQServiceProvider
{
    Q_OBJECT
    public:
        NewFileService( ServiceApp *parent = 0 );
        ~NewFileService();
        bool asyncAnswer() {return mAsyncReqIds.count() > 0;}
        void complete(bool ok);

    public slots:
        bool view(QString file);
        bool view(XQSharableFile file);
        
    private slots:
        void handleClientDisconnect();
        void handleAnswerDelivered();

    private:
        ServiceApp* mServiceApp;
        QMap<quint32,int> mAsyncReqIds;
        bool mRetValue;
};


#define TESTCASE_INFO_KEY QLatin1String("XQTestCase")


#endif
