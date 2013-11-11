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

#include "xqservicelog.h"

#include <xqserviceprovider.h>
#include <qmetaobject.h>
#include <QByteArray>

#include <xqserviceadaptor.h>
//#include <xqserviceservice.h>
#include <xqserviceutil.h>
#include "xqrequestutil.h"

/*!
    \class ServiceAdaptorProxy
    \brief Proxy class for converting signal and slot members into IPC message names
*/
class ServiceAdaptorProxy : public XQServiceAdaptor
{
    Q_OBJECT

public:
    ServiceAdaptorProxy(const QString &channel, QObject *parent=0);
    virtual ~ServiceAdaptorProxy() ;
    
    QString memberToMessage( const QByteArray& member );
};

ServiceAdaptorProxy::ServiceAdaptorProxy(const QString &channel, QObject *parent) :
        XQServiceAdaptor(channel, parent) 
{
    XQSERVICE_DEBUG_PRINT("ServiceAdaptorProxy::ServiceAdaptorProxy");
    XQSERVICE_DEBUG_PRINT("channel: %s", qPrintable(channel));
}

ServiceAdaptorProxy::~ServiceAdaptorProxy()
{
    XQSERVICE_DEBUG_PRINT("ServiceAdaptorProxy::~ServiceAdaptorProxy");
}

QString ServiceAdaptorProxy::memberToMessage( const QByteArray& member )
{
    XQSERVICE_DEBUG_PRINT("ServiceAdaptorProxy::memberToMessage");
    XQSERVICE_DEBUG_PRINT("member: %s", member.constData());
// TO BE CHECKED
//    return m_channel + "::" + XQServiceAdaptor::memberToMessage( member );
    return XQServiceAdaptor::memberToMessage( member );
}

/*!
    \class XQServiceProvider_Private
    \inpublicgroup QtBaseModule

    \brief Private implementation of XQServiceProvider
*/
class XQServiceProvider_Private
{
public:
    XQServiceProvider_Private(const QString &service);

    ~XQServiceProvider_Private();
    
    XQServiceAdaptor *m_adaptor;

    QString m_service;
    bool m_publishAllCalled;
    QObject* plugin;
};

XQServiceProvider_Private::XQServiceProvider_Private(const QString &service) :
        m_adaptor(NULL),
        m_service(service),
        m_publishAllCalled(false),
		plugin(NULL)
{
    XQSERVICE_DEBUG_PRINT("XQServiceProvider_Private::XQServiceProvider_Private");
    XQSERVICE_DEBUG_PRINT("service: %s", qPrintable(service));
    m_adaptor = new ServiceAdaptorProxy(service);
}

XQServiceProvider_Private::~XQServiceProvider_Private()
{
    XQSERVICE_DEBUG_PRINT("XQServiceProvider_Private::~XQServiceProvider_Private");
    delete m_adaptor;
}

/*!
    \class XQServiceProvider
    \inpublicgroup QtBaseModule

    \brief Base class of the Qt Extension's service framework for implementing out-of-process service providers.
           It supports implementing out-of-process slots that can be invoked from remote service clients

    See \ref terminology for terms used in this documentation.
    
    Service messages consist of a service name, a message name, and a list of parameter values.
    Qt extension dispatches service messages to the applications associated with the service
    name, on the application's \c{QPE/Application/appname} channel, where
    \c{appname} is the application's name.

    <b>Service registration</b> \n
    Service provider need to register it's service into the system before they can be used by
    the service client. Registration is done by creating a XML formatted service configuration
    file and defining the service in the provider's .pro-file. QMake will notice service provider
    from the .pro-file, with help of the service.prf file, and generate necessary resources for
    the Symbian application registry. The helper application sqsreg.exe will generate an application
    registration resource file ( _reg.rss) from the configuration-file and provider's definitions
    that include the needed declarations for the services provided.
    
    <b>Service Names Allocation</b> \n
    The harmonize service and interface names the Symba specific names and guidelines can be found
    from http://s60wiki.nokia.com/S60Wiki/QtFw_for_S60_coding_conventions/Service_name_registry#Service.

    Before implementing a service you need to allocate the name according to the guidelines. Please
    inform intended service clients (customers) and matti.parnanen@nokia.com.
    
    <b>Service Configuration File</b> \n
    All the service configuration are added to the run-time service registry to make them available
    for service discovery and creating service requests.
    \note Only one service element with multiple interface is supported!
    
    To support services a new configuration was introduced to qmake and two new variables for
    that configuration:
    
    \code
        CONFIG = service
        service.file = <service configuration file path>
        service.options = <embeddable> (optional, default not embeddable), <hidden> (optional, default not hidden) 
    \endcode
    
    The fornat of the service configuration file is same as XML format used in Qt Service Framework.
    Example configuration file:
    
    \code
        <?xml version="1.0" encoding="utf-8" ?>
        <service>
            <name>Music Fetcher</name>
            <filepath>No path</filepath>
            <description>Music Fetcher</description>
            <interface>
                <name><b>com.nokia.symbian.IMusicFetch</b></name>
                <version>1.0</version>
                <description>Interface for fetching music files</description>
            </interface>
        </service>
    \endcode
    
    \code
        <ELEMENT service ( name, filepath, description?, interface+ ) >
        <ELEMENT description ( #CDATA ) >
        <ELEMENT filepath ( #PCDATA ) >
        <ELEMENT interface ( '''name''', version, description?, capabilities?, customproperty* ) >
        <ELEMENT capabilities ( #PCDATA ) >
        <ELEMENT name ( #PCDATA ) >
        <ELEMENT version ( #PCDATA ) >
        <ELEMENT customproperty ( #CDATA ) >
        <ATTLIST customproperty key NMTOKEN #REQUIRED >
    \endcode
    
    <b>Changing service or interface names</b> \n
    Before you think about changing the name of the already released and used service implementation, read this
    http://s60wiki.nokia.com/S60Wiki/QtFw_for_S60_coding_conventions/Service_name_registry#About_changing_service_or_interface_names
    first.
    
    The basic message is the service name, interface name and operation (message) slot signatures for the API. And for API changes you have to apply development time API deprecation process.
    
    <b>Service Registration tools</b> \n
    The needed utility files for service registration:
    - xqsreg.exe should be in \epoc32\tools or some other directory that can be found from the path
    
    If necessary you can copy those files to target directories from qthighway/bin.
    
    Usage: \n
    How to create a simple synchronously working service provider?
    \code
        class YourService : public XQServiceProvider
            {
            Q_OBJECT

            public:
                YourService ( ServiceApp *parent = 0 );
                ~YourService ();

            public slots:
                void functionName1(); 
                int functionName2(const QString& number, int times);

            private:
                ServiceApp *mServiceApp;
            };
    \endcode
    
    Implementation:
    \code
        YourService::YourService(ServiceApp* parent) 
        : XQServiceProvider(QLatin1String("yourservice.Interface"), parent), mServiceApp(parent)
            {
            publishAll();
            }

        YourService::~YourService() { } 

        void YourService::functionName1() { } 

        int YourService::functionName2(const QString& number, int times)
            {
            int returnValue = 1;
            return returnValue;
            }
    \endcode
    
    Additions to .pro-file:
    \code
        CONFIG += service
        SERVICE.FILE = service_conf.xml
        SERVICE.OPTIONS = embeddable
        SERVICE.OPTIONS += hidden
    \endcode
    
    Service configuration file (service_conf.xml):
    \code
    <?xml version="1.0" encoding="utf-8" ?>
    <service>
        <name>yourservice</name>
        <filepath>No path</filepath>
        <description>Service description</description>
        <interface>
            <name>Interface</name>
            <version>1.0</version>
            <description>Interface description</description>
        </interface>
    </service>
    \endcode
    
    How to create a simple asynchronously working service provider?
    Header:
    \code
        class YourService : public XQServiceProvider
        {

        Q_OBJECT

        public:
            YourService ( ServiceApp *parent = 0 );
            ~YourService ();
            void compleAsyncFunction();

        public slots:
            void functionName1(); 
            int functionName2(const QString& number, int times);

        private:
            ServiceApp *mServiceApp;
            int mAsyncRequestIndex;
            QVariant mReturnValue;
         
        };
    \endcode
    
    Implementation:
    \code
        YourService::YourService(ServiceApp* parent) 
        : XQServiceProvider(QLatin1String("yourservice.Interface"), parent), mServiceApp(parent)
            {
            publishAll();
            }

        YourService::~YourService() { } 

        void YourService::compleAsyncFunction()
            {
            completeRequest(mAsyncRequestIndex, mReturnValue);
            }

        void YourService::functionName1() 
            { 
            mAsyncRequestIndex = setCurrentRequestAsync();
            mReturnValue.setValue(0);
            } 

        int YourService::functionName2(const QString& number, int times)
            {
            mAsyncRequestIndex = setCurrentRequestAsync();
            mReturnValue.setValue(1);
            return mReturnValue.toInt();
            }
    \endcode
    
    <b>Examples:</b> \n
    The use of XQServiceProvider will be demonstrated using the \c{Time}
    service.  This has a single message called \c{editTime()} which asks
    the service to pop up a dialog allowing the user to edit the current time.
    \code
    class TimeService : public XQServiceProvider
    {
        Q_OBJECT
    public:
        TimeService( QObject *parent = 0 );

    public slots:
        void editTime(QTime time);
    };

    TimeService::TimeService( QObject *parent )
        : XQServiceProvider( "Time", parent )
    {
        publishAll();
    }
    \endcode

    The call to publishAll() causes all public slots within \c{TimeService}
    to be automatically registered as Service messages.  This can be
    useful if the service has many message types.

    The client can send a request to the service using QtopiaServiceRequest:

    \code
    XQServiceRequest req( "Time", "editTime()" );
    req << QTime::currentTime();
    req.send();
    \endcode
    
    <b>URI viewer</b> \n
    This is a simple example for implementing out-of-process scheme handlers.
    - "http", "https" and are handled via standard QDesktopServices::openUrl() function. 
       This is fire-and-forget launch. The options are ignored and no control and signals available after the launch.
    - "appto" is routed to Activity Manager for opening the attached activity.
      This is fire-and-forget launch. The options are ignored and no control and signals available after the launch.
    - The "file" scheme is handled as the QFile based create below. 
      So the com.nokia.symbian.IFileView interface is applied as for the QFile.
    
    Service application needs to publish support for:
    - The common interface "com.nokia.symbian.IUriView", and
    - The scheme(s), like "testo" in the example below. The custom custom property "schemes" contains one or more schemes as comma separated list (CSV)
    - The slot "view(QString)" to view the URI

    \code
        <?xml version="1.0" encoding="utf-8" ?>
        <service>
          <name>serviceapp</name>
          <filepath>No path</filepath>
          <description>Test service</description>
          <interface>
             <name>com.nokia.symbian.IUriView</name>
             <version>1.0</version>
             <description>Interface for showing URIs</description>
             <customproperty key="schemes">testto</customproperty>
           </interface>
        </service>
    \endcode
    
    An service application that offers support for a scheme implements the common "UriService" with the pre-defined "view" slot:
    
    \code
        class UriService : public XQServiceProvider
        {
            Q_OBJECT
            public:
                UriService( ServiceApp *parent = 0 );
                ~UriService();
                bool asyncAnswer() {return mAsyncAnswer;}
                void complete(bool ok);
                
            public slots:
                bool view(const QString& uri);
                
            private slots:
                void handleClientDisconnect();

            private:
                ServiceApp* mServiceApp;
                bool mAsyncAnswer;
                int mAsyncReqId;
                bool mRetValue;
        };
    \endcode
    
    Client application accesses the service via the URI:
    
    \code
        // Assume in example we have own scheme "testo" but this can be applied to 
        // "mailto", etc. standard schemes.
        //
        // (As mentioned in the documentation, some schemes are CURRENTLY handled specially,
        // like "http" scheme uses QDesktopServices::openUrl).  
        // 
        QUrl url("testto://authority?param1=value1&param1=value2"); 

        // The difference to the previous example is is how request is created
        // via application mgr.

        request = mAiwMgr.create(url);
        if (request == NULL)
        {
            // No handlers for the URI
            return;
         }

        // Set function parameters
        QList<QVariant> args;
        args << uri.toString();
        request->setArguments(args);

        // Send the request
        bool res = request->send();
        if  (!res) 
        {
            // Request failed. 
            int error = request->lastError();
            // Handle error
        }

        // If making multiple requests to same service, you can save the request as member variable
        // In this example all done.
        delete request;
    \endcode
   
    <b>File viewer</b> \n
    As for URis, a service application that support viewing a file with a dedicated MIME-type need to publish support for:
    - The common interface "com.nokia.symbian.IFileView".
    - The slot "view(QString)" to view the non-data-caged file by file name.
    - The slot "view(XQSharable)" to view the data-caged file by sharable file handle.
    - MIME type list (registered in the .pro file).
    So there are multiple service applications implementing the same interface.
    
    In service provider side you need the following entry in XML:
    
    \code
        <interface>
            <name>com.nokia.symbian.IFileView</name>
            <version>1.0</version>
            <description>Interface for showing Files</description>
        </interface>
    \endcode
    
    The file viewer application shall offer slots both for viewing filename (QString) and viewing sharable file (XQSharable):
    
    \code
    class FileService : public XQServiceProvider
    {
        Q_OBJECT
        public:
            FileService( ServiceApp *parent = 0 );
            ~FileService();
            bool asyncAnswer() {return mAsyncAnswer;}
            void complete(bool ok);

        public slots:
            bool view(QString file);
            bool view(XQSharableFile file);
            
        private slots:
            void handleClientDisconnect();

        private:
            ServiceApp* mServiceApp;
            bool mAsyncAnswer;
            int mAsyncReqId;
            bool mRetValue;
    };
    \endcode
    
    In the .pro file the service publishes the supported MIME types, e.g:
    
    \code
        RSS_RULES += \
          "datatype_list = " \
          "      {" \
          "      DATATYPE" \
          "          {" \
          "          priority = EDataTypePriorityNormal;" \
          "          type = \"text/plain\";" \
          "          }" \
          "      };" \
    \endcode
    
    In the client side (see the "examples/appmgrclient" and "examples/serviceapp" included in the QtHighway release) access to
    file:
    
    \code
        // Not data caged file
        QFile file("C:\\data\\Others\\test.txt");

        request = mAiwMgr.create(file);
        if (request == NULL)
        {
               // No handlers for the URI
               return;
         }
        // By default operation is "view(QString)"

        // Set function parameters
        QList<QVariant> args;
        args << file.fileName();
        request->setArguments(args);

        // Send the request
        bool res = request->send();
        if  (!res) 
        {
           // Request failed. 
          int error = request->lastError();

          // Handle error
        }
     
        // If making multiple requests to same service, you can save the request as member variable
        // In this example all done.
        delete request;
    \endcode
    
    <b>Sharable file viewer</b> \n
    The same rules as for file name based view applies, but different argument type (XQSharableFile) used
    in request. See the "examples/appmgrclient" and "examples/serviceapp" included in the QtHighway release.
    
    \code
        XQSharableFile sf;
        // Open the file for sharing from own private  directory
        // If you have handle available, just set it by "setHandle()" function
        if (!sf.open("c:\\private\\e0022e74\\test.txt"))
        {
            // Failed to open sharable file
            return;
        }

        // Create request for the sharable file
        XQAiwreqiuest req = mAiwMgr.create(sf);
        if (!req)
        {
            // No viewer app found for the file
            // As we opened the handle, we need to close it !
            sf.close(); 
            return;  
        }
        // By default operation is "view(XQSharableFile)"

        // Set function parameters
        // Not only one sharable handle supported,  otherwise upon send EArgumentError error occurs
        QList<QVariant> args;
        args << qVariantFromValue(sf);  
        req->setArguments(args);

        // Send the request
        bool res = request->send();
        if  (!res) 
        {
            // Request failed. 
            int error = request->lastError();
            // Handle error
        }

        // As we opened the handle, we need to close it !
        sf.close(); 

        // If making multiple requests to same service, you can save the request as member variable
        // In this example all done.
        delete request;
    \endcode
    
    <b> Create interface action </b> \n
    One interface XML may offer one action to be displayed by client application.
    See the "examples/appmgrclient" and "examples/hbserviceprovider" included in the QtHighway release.
    
    \code
    HbAction* ShareUiPrivate::fetchServiceAction(XQAiwInterfaceDescriptor interfaceDescriptor)
        {
        QDEBUG_WRITE("ShareUiPrivate::fetchServiceAction start");
        // create the request for each descriptor.
        
        XQAiwRequest* request = mAppManager.create(interfaceDescriptor,SELECT_OP,false);
        QAction action = request->createAction());
        if (!action)
            return 0;

        // if Orbit widgets do not support QAction
        // Need to convert QAction to HbAction first
        HbAction* hbAction = convertAction(action);
        if(hbAction)
            {
            // Connect triggered signals to enable the request to emit triggered 
            connect(hbAction, SIGNAL(triggered()), action, SIGNAL(triggered()));

            // connect the request's triggered action to the slot in app
            connect(request, SIGNAL(triggered()), this, SLOT(onTriggered()));
            }
            
        return hbAction;
        }
    \endcode
    
    In service provider side you need to have the following entries in XML to be converted to QAction by the create:
    
    \code
        <interface>
            <name>Dialer></name>
            <version=1.0</version>
            <description>Dial interface</description>
            <customproperty key="aiw_action_text_file">hbserviceprovider</customproperty>
            <customproperty key="aiw_action_text">txt_aiw_action_text</customproperty>
        </interface>
    \endcode
*/

/*!
    \fn void XQServiceProvider::returnValueDelivered()
    
    This signal is emitted when request has been completed and its return value has been
    delivered to the service client. Synchronous request get completed upon returning
    from slot call, asynchronous gets completed when completeRequest is called.
*/

/*!
    \fn void XQServiceProvider::clientDisconnected()
    
    This signal is emitted if there is on-going request and client destroys
    the XQAiwRequest for some reason, e.g when exiting the client process.
*/

/*!
    Construct a remote service object for \a service and attach it to \a parent.
    \param service Defines the full service name that is implemented. 
                   The full service name is:
                   - The name of the service from the service configuration file
                   - Character *.* (dot)
                   - The name of the interface from the service configuration file
    \param parent Parent of this QObject
*/
XQServiceProvider::XQServiceProvider( const QString& service, QObject *parent )
    : QObject( parent )
{
    XQSERVICE_DEBUG_PRINT("XQServiceProvider::XQServiceProvider");
    XQSERVICE_DEBUG_PRINT("service: %s", qPrintable(service));
    m_data = new XQServiceProvider_Private(service);
    connect(m_data->m_adaptor, SIGNAL(returnValueDelivered()), this, SIGNAL(returnValueDelivered())); 
    connect(m_data->m_adaptor, SIGNAL(clientDisconnected()), this, SIGNAL(clientDisconnected())); 
}

/*!
    Destroys this service handling object.
*/
XQServiceProvider::~XQServiceProvider()
{
    XQSERVICE_DEBUG_PRINT("XQServiceProvider::~XQServiceProvider");
    if (m_data)
        delete m_data;
}


void XQServiceProvider::SetPlugin(QObject* impl_plugin)
    {
    m_data->plugin=impl_plugin;
    }


/*!
    Publishes all slots on this object within subclasses of XQServiceProvider.
    This is typically called from a subclass constructor.
*/
void XQServiceProvider::publishAll()
{
    XQSERVICE_DEBUG_PRINT("XQServiceProvider::publishAll");
	if (!m_data->plugin) {
	    m_data->m_adaptor->publishAll(this,XQServiceProvider::staticMetaObject.methodCount(),XQServiceAdaptor::Slots);
	}
    else {
        m_data->m_adaptor->publishAll(m_data->plugin, 0, XQServiceAdaptor::Slots);
    } 
	
	if (XQServiceUtil::isEmbedded())
	    XQServiceUtils::closeWhenClientClosed();
}

/*!
    Sets current request to asynchronous mode so that provider can complete the
    request later via the completeRequest() call.
    \return Request ID which shall be used in the completeRequest() call.
    \note There can be several clients accessing the same service at the same time. Avoid saving
          the index to XQServiceProvider instance as member variable as when another new request
          comes in, it will have different index and you will potentially override the index of
          the first request. You should ensure the completeRequest() gets the correct index e.g.
          by attaching the index as user data to data object maintain a map of indexes based on
          some key. \n
          <b> This should be used for a request set asynchronous by client only.  This has no
          impact to the client side behaviour. For the synchronous request client side "send"
          will wait for the completion. For the asyncronous request the client side gets
          signalled when  the completion happens. </b>
*/
int XQServiceProvider::setCurrentRequestAsync()
{
    XQSERVICE_DEBUG_PRINT("XQServiceProvider::setCurrentRequestAsync");
    return m_data->m_adaptor->setCurrentRequestAsync();
}

/*!
    \fn bool XQServiceProvider::completeRequest(int index, const T& retValue)
    
    Completes asynchronous request.
    \param index Defines the index of the asynchronous request to complete. 
    \param retValue defines the return value for the request.
    \return true if request could be completed successfully, otherwise false.
    \sa completeRequest()
*/

/*!
    Completes the asynchronous request with the given value
    \param index Request ID got from the setCurrentRequestAsync call.
    \param retValue Returned value.
    \return true on success, false if index points to non-existing request.
    \note <b>You need to check the return value. </b>
          If false it means connection to client has been lost and the complete will not ever succeed. 
          So if you have e.g. a code that quits application using the ReturnValueDelived signal only,
          that signal will never be emitted as request can not be completed.
*/
bool XQServiceProvider::completeRequest(int index, const QVariant& retValue)
{
    XQSERVICE_DEBUG_PRINT("XQServiceProvider::completeRequest");
    XQSERVICE_DEBUG_PRINT("index: %d, retValue: %s", index, qPrintable(retValue.toString()));
    return m_data->m_adaptor->completeRequest(index, retValue);
}

/*!
    Return additional request information attached to request
    \return Request info.
*/
XQRequestInfo XQServiceProvider::requestInfo() const
{
    return m_data->m_adaptor->requestInfo();
}

#include "xqserviceprovider.moc"
