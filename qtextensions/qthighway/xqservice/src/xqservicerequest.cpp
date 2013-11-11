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

#include "xqservicerequest.h"
#include "xqservicethreaddata.h"

#include "xqservicechannel.h"
#include "xqserviceadaptor.h"
#include "xqrequestutil.h"
#include <QStringList>
#include <xqservicemanager.h>

/*!
    \class XQServiceRequest_Private
    \inpublicgroup QtBaseModule

    \ingroup ipc
    \brief Private implementation of the XQServiceRequest.
*/

class XQServiceRequest_Private : public XQServiceRequestCompletedAsync
{
public:
    XQServiceRequest_Private(XQServiceRequest* parent)
        : mSynchronous(true), mParent(parent),mServiceManager(NULL)
    {    
        XQSERVICE_DEBUG_PRINT("XQServiceRequest_Private::XQServiceRequest_Private(1)");
        
    };

    XQServiceRequest_Private(const QString& fullServiceName, 
                             const QString& message, 
                             const bool &synchronous, 
                             XQServiceRequest* parent)
        : mService(fullServiceName), mMessage(message), mSynchronous(synchronous), mParent(parent),mServiceManager(NULL)
    {    
        XQSERVICE_DEBUG_PRINT("XQServiceRequest_Private::XQServiceRequest_Private(2)");
    };

    XQServiceRequest_Private(const XQAiwInterfaceDescriptor &descriptor, 
                             const QString& message, 
                             const bool &synchronous, 
                            XQServiceRequest* parent)
            : mMessage(message), mSynchronous(synchronous), mParent(parent),mServiceManager(NULL)
    {
        XQSERVICE_DEBUG_PRINT("XQServiceRequest_Private::XQServiceRequest_Private(3)");
        
        // Construct service name understood by the QtHighway FW
        // (The descriptor has been created from the  XML)
        mService = descriptor.serviceName() + "." + descriptor.interfaceName();
        XQSERVICE_DEBUG_PRINT("XQServiceRequest_Private(3)::service=%s", qPrintable(mService));

        // Remember the descriptor
        mRequestUtil.mDescriptor = descriptor; 
    };
    
    ~XQServiceRequest_Private();
    // from XQServiceRequestCompletedAsync
    void requestCompletedAsync(const QVariant &retValue);
    void requestErrorAsync(int err);
    
    QList<QVariant> mArguments;
    QString mService;
    QString mMessage;
    bool mSynchronous;
    XQServiceRequest* mParent;
    XQRequestUtil mRequestUtil;
    XQServiceManager* mServiceManager;
    QString mUniqueChannelName;
    
};

XQServiceRequest_Private::~XQServiceRequest_Private()
{
    XQSERVICE_DEBUG_PRINT("XQServiceRequest_Private::~XQServiceRequest_Private");
    delete mServiceManager;
};

void XQServiceRequest_Private::requestCompletedAsync(const QVariant &retValue)
{    
    XQSERVICE_DEBUG_PRINT("XQServiceRequest_Private::requestCompletedAsync");
    emit mParent->requestCompleted(retValue);
}

void XQServiceRequest_Private::requestErrorAsync(int err)
{    
    XQSERVICE_DEBUG_PRINT("XQServiceRequest_Private::requestErrorAsync");
    emit mParent->requestError(err);
}

/*!
    \class XQServiceRequest
    \inpublicgroup QtBaseModule

    \ingroup ipc
    \brief Allows applications to request services from other applications.
    
    The XQServiceRequest class allows applications to request services from other applications.
    A XQServiceRequest encapsulates a service name and the message to be sent to that service.
    
    \note One should be using XQApplicationManager and the related XQAiwRequest instead of XQServiceRequest.
          The XQApplicationManager and related classes encapsulate basic, target-architecture approved
          support for out-of-process Application Interworking, e.g. support for launching URLs
          (including activity URLs), normal files, sharable files, etc. (whatever needed).
    
    \b Examples: \n
    
    How to create synchronous request without parameters and return value? 
    \note The full name (yourservice.Interface) need to be used (with dot (.) between service name and interface name).
    
    \code
        XQServiceRequest request("yourservice.<b>Interface</b>", "functionName1()");
        bool res = request.send();
        if  (!res) {
            int error = request.latestError();
        }
    \endcode
    
    How to create synchronous request with several parameters and return value?
    
    \code
        QString parameter1("+3581234567890");
        int parameter2 = 3;
        XQServiceRequest request("yourservice.<b>Interface</b>", "functionName2(QString, int)");
        request << parameter1;
        request << parameter2;
        int returnvalue;
        bool res = request.send(returnvalue);
        if  (!res) {
            int error = request.latestError();
        }
    \endcode
    
    How to create asynchronous request without return value?
    
    \code
        QString parameter1("+3581234567890");
        int parameter2 = 3;
        XQServiceRequest request("yourservice.Interface", "functionName2(QString, int)", false);
        request << parameter1;
        request << parameter2;
        bool res = request.send();
        if (!res) {
            int error = request.latestError();
        }
    \endcode
    
    How to create asynchronous request with return value?
    
    \code
        QString parameter1("+3581234567890");
        int parameter2 = 3;
        XQServiceRequest request("yourservice.Interface", "functionName2(QString, int)", false);
        request << parameter1;
        request << parameter2;
        connect(request, SIGNAL(requestCompleted(QVariant)), this, SLOT(requestCompleted(QVariant)));
        bool res = request.send();
        if (!res) {
            int error = request.latestError();
        }

        ...

        void requestCompleted(const QVariant& value)
        {
            int returnvalue = value.toInt();
        }
    \endcode
    
    How to use declare custom type?
    
    Header:
    \code
        class CustomType 
        { 
        public: 
            CustomType (){}; 
            virtual ~CustomType(){};

            QString mString1; 
            QString mString2; 
            QUuid mUid;

            template <typename Stream> void serialize(Stream &stream) const; 
            template <typename Stream> void deserialize(Stream &stream); 
        };

        Q_DECLARE_USER_METATYPE(CustomType)
    \endcode
    
    Implementation:
    \code        
        template <typename Stream> void CustomType::serialize(Stream &s) const 
            { 
            s << mString1; 
            s << mString2; 
            s << mUid; 
            }

        template <typename Stream> void CustomType::deserialize(Stream &s) 
            { 
            s >> mString1; 
            s >> mString2; 
            s >> mUid; 
            }

        Q_IMPLEMENT_USER_METATYPE(CustomType)
    \endcode
    
    How to declare custom type that doesn't need data stream operators?
    
    Header:
    \code
        typedef QList<CustomType> CustomTypeList;

        Q_DECLARE_USER_METATYPE_NO_OPERATORS(CustomTypeList)
    \endcode
    
    Implementation:
    \code
        Q_IMPLEMENT_USER_METATYPE_NO_OPERATORS(CustomTypeList)
    \endcode
*/

/*!
    \fn bool XQServiceRequest::send(T& retValue)

    Sends the request. If the request is synchronous, then client is blocked
    until service provider completes the request or request fails for some reason.
    If the request is asynchronous, then client won't be blocked.
    \param retValue Defines refence to a value service provider will return after service.
    \return False if there was no application that could service the request, otherwise true.
*/

/*!
    \fn XQServiceRequest &XQServiceRequest::operator<< (const T &var)

    Adds \a var to the list of arguments for this service request.
    \param var Defines the argument value to add to the list of arguments.
*/

/*!
    \fn XQServiceRequest &XQServiceRequest::operator<< (const char *var)

    Adds \a var to the list of arguments for this service request.
    \param var Defines the argument value to add to the list of arguments.
*/

/*!
    \fn void XQServiceRequest::requestCompleted(const QVariant& value)

    This signal is emitted when service provider returns a return value asynchronously back to the client.
    \param value Result of the request.
*/

/*!
    \fn void XQServiceRequest::requestError(int err);

    This signal is emitted when error has happened in request handling.
    \param err Error code as integer value.
    \sa XQService::ServiceIPCErrors
*/

/*!
    \fn void XQServiceRequest::addVariantArg(const QVariant& var)

    Adds the variant value to the list of arguments, so that the variant's
    value is serialized in send() rather than the variant itself.
    \param var Value to be added to the list of arguments.
*/

/*!
    Construct a null service request.
    setService() and setMessage() must be called before send(), but the
    service may be written prior to the calls.
 */
XQServiceRequest::XQServiceRequest()
{
    XQSERVICE_DEBUG_PRINT("XQServiceRequest::XQServiceRequest(1)");
    mData = new XQServiceRequest_Private(this);
}

/*!
    Construct a service request that will send \a message to
    a \a service when send() is called. The service may be written
    prior to the calls.
    \param service Defines the full service name to send message. The full name is:
                   - The name of the service in the service configuration file
                   - Character *.* (dot)
                   - The name of the interface from the service XML.
    \param message Defines the message to send to the service provider i.e. it is
                 the signature of the service provider function to be called.
    \param synchronous Defines should message be sent synchronously or asynchronously.
                     By default message is sent synchronously.
*/
XQServiceRequest::XQServiceRequest(const QString& service, const QString& message, const bool &synchronous)
{
    XQSERVICE_DEBUG_PRINT("XQServiceRequest::XQServiceRequest(2)");
    XQSERVICE_DEBUG_PRINT("service: %s, message: %s, synchronous: %d", qPrintable(service), qPrintable(message), synchronous);
    mData = new XQServiceRequest_Private(service,message,synchronous,this);
}

/*!
    Copy constructor. Any data previously written to the \a orig
    service will be in the copy.
    \param orig XQServiceRequest from which data will be copied to this object.
*/
XQServiceRequest::XQServiceRequest(const XQServiceRequest& orig)
{
    XQSERVICE_DEBUG_PRINT("XQServiceRequest::XQServiceRequest(3)");
    mData = new XQServiceRequest_Private(orig.mData->mService,orig.mData->mMessage,orig.mData->mSynchronous,this);
    mData->mArguments = orig.mData->mArguments;
}

/*!
    Construct a service request by service descriptor which contains exact details of the service and interface.
    The service may be written prior to the calls.
    \param descriptor Defines details of the service and it's interface.
    \param message Message to be sent when send() is called.
    \param synchronous Defines should message be sent synchronously or asynchronously.
                       By default message is sent synchronously.
*/
XQServiceRequest::XQServiceRequest(const XQAiwInterfaceDescriptor &descriptor, const QString& message, const bool &synchronous)
{
    XQSERVICE_DEBUG_PRINT("XQServiceRequest::XQServiceRequest(2)");
    XQSERVICE_DEBUG_PRINT("service: %s, interface %s, message: %s, synchronous: %d",
                          qPrintable(descriptor.serviceName()), qPrintable(descriptor.interfaceName()),
                          qPrintable(message), synchronous);
    mData = new XQServiceRequest_Private(descriptor,message,synchronous,this);
}

/*!
    Assignment operator.
    Any data previously written to the \a orig
    service will be in the copy.
*/
XQServiceRequest& XQServiceRequest::operator=(const XQServiceRequest& orig)
{
    XQSERVICE_DEBUG_PRINT("XQServiceRequest::operator=");
    if( &orig == this )
        return *this;

    mData->mService = orig.mData->mService;
    mData->mMessage = orig.mData->mMessage;
    mData->mArguments = orig.mData->mArguments;
    mData->mRequestUtil = orig.mData->mRequestUtil;

    return *this;
}

/*!
    Destroys the service request. Unlike QtopiaIpcEnvelope, the
    request is not automatically sent.
*/
XQServiceRequest::~XQServiceRequest()
{
    XQSERVICE_DEBUG_PRINT("XQServiceRequest::~XQServiceRequest %s", qPrintable(mData->mUniqueChannelName));
    XQServiceAdaptor::cancelPendingSend(mData->mUniqueChannelName);
    delete mData;
}

/*!
    Checks if request is NULL.
    \return True if either the service() or message() is not set.
    \sa service(), message()
 */
bool XQServiceRequest::isNull() const
{
    XQSERVICE_DEBUG_PRINT("XQServiceRequest::isNull");
    bool ret = mData->mService.isEmpty() || mData->mService.isNull() || mData->mMessage.isNull();
	XQSERVICE_DEBUG_PRINT("mData->mService.isEmpty() = %d", mData->mService.isEmpty());
	XQSERVICE_DEBUG_PRINT("mData->mService.isNull() = %d", mData->mService.isNull());
	XQSERVICE_DEBUG_PRINT("mData->mMessage.isNull() = %d", mData->mMessage.isNull());
	XQSERVICE_DEBUG_PRINT("return %d", ret);
	return ret;
}

/*!
    Checks if request is synchronous or asynchronous.
    \return True if request is synchronous, false if request is asynchronous.
    \sa setSynchronous()
 */
bool XQServiceRequest::isSynchronous() const
{
    XQSERVICE_DEBUG_PRINT("XQServiceRequest::isSynchronous");
    return mData->mSynchronous;
}

/*!
    Sets request to be synchronous or asynchronous.
    \param synchronous If set to true, request will be synchronous.
                       If set to false, request will be asynchronous.
    \sa isSynchronous()
 */
void XQServiceRequest::setSynchronous(const bool& synchronous)
{
    XQSERVICE_DEBUG_PRINT("XQServiceRequest::setSynchronous");
    mData->mSynchronous = synchronous;
}
/*!
    Sends the request. If the request is synchronous, then client is blocked
    until service provider completes the request or request fails for some reason.
    If the request is asynchronous, then client won't be blocked. If the request
    is asynchronous and clients wants to receive a return value from the service
    provider, then clients should connect to the requestCompleted() signal.
    \return False if there was no application that could service the request, otherwise true.
*/
bool XQServiceRequest::send()
{
    XQSERVICE_DEBUG_PRINT("XQServiceRequest::send(1)");
    QVariant retValue;
    return send(retValue);
}

/*!
    Sends the request. If the request is synchronous, then client is blocked
    until service provider completes the request or request fails for some reason.
    If the request is asynchronous, then client won't be blocked.
    \param retData Defines refence to a value service provider will return after service.
    \return False if there was no application that could service the request, otherwise true.
*/
bool XQServiceRequest::send(QVariant& retData)
{
    XQSERVICE_DEBUG_PRINT("XQServiceRequest::send(2)");
    if (isNull())
    {
        XQSERVICE_DEBUG_PRINT("XQServiceRequest::send error: null request");
        XQService::serviceThreadData()->setLatestError(XQService::EArgumentError);
        return false;
    }
    
    // Handle sharable file argument(s), if any
    if (!handleSharableFileArgs())
    {
        XQSERVICE_DEBUG_PRINT("XQServiceRequest::send error:invalid sharable file");
        XQService::serviceThreadData()->setLatestError(XQService::EArgumentError);
        return false;
    }
    
    mData->mRequestUtil.setSynchronous(mData->mSynchronous); // Ensure option is set !
    mData->mRequestUtil.mOperation = mData->mMessage;  // Save the operation name for startup
    
    // !!!
    // Add the info as extra argument to the request
    // This shall be removed by the server
    // !!!
    addArg(qVariantFromValue(mData->mRequestUtil.mInfo));
       
    // Pass always the util instance onwards as user data.
    // It can be utilized by the XQServiceManager::startServer
    // e.g. to optimize startup of a service server

    // Create unique channel name to separate multiple client requests to same channel name.
    quint32 handle = (unsigned int)mData;
    mData->mUniqueChannelName = QString("%1:").arg(handle) + mData->mService;
    XQSERVICE_DEBUG_PRINT("XQServiceRequest::send(2):uniqueChannel=%s", qPrintable(mData->mUniqueChannelName));
    return XQServiceAdaptor::send(mData->mUniqueChannelName,  message(), mData->mArguments, retData, mData->mSynchronous,mData,
                                 (const void *)&mData->mRequestUtil);
}
/*!
    Sets the full name of the service to which the request will be sent.
    \param fullServiceName Full name of the service to send message to. See
                           XQServiceRequest(const QString& service, const QString& message, const bool &synchronous)
                           for the full name definition.
    \sa service()
 */
void XQServiceRequest::setService(const QString& fullServiceName)
{
    XQSERVICE_DEBUG_PRINT("XQServiceRequest::setService");
    XQSERVICE_DEBUG_PRINT("service: %s", qPrintable(fullServiceName));
    mData->mService = fullServiceName;
    mData->mArguments.clear();
    mData->mRequestUtil.mDescriptor = XQAiwInterfaceDescriptor(); // Invalid descriptor
}

/*!
    Gets the service name to which this request will be sent.
    \return Full service name to which request will be sent.
    \sa setService()
*/
QString XQServiceRequest::service() const
{
    XQSERVICE_DEBUG_PRINT("XQServiceRequest::service");
    XQSERVICE_DEBUG_PRINT("service: %s", qPrintable(mData->mService));
    return mData->mService;
}

/*!
    Sets the \a message to be sent to the service.
    \param message Defines the message to send to a service provider. The message
                   is a valid Qt slot signature published by the service provider.
                   For example, "view(QString)".
    \sa message()
*/
void XQServiceRequest::setMessage(const QString& message)
{
    XQSERVICE_DEBUG_PRINT("XQServiceRequest::setMessage");
    XQSERVICE_DEBUG_PRINT("message: %s", qPrintable(message));
    mData->mMessage = message;
    mData->mArguments.clear();
}

/*!
    Gets the message set for the request.
    \return Message of the request as QString.
    \sa setMessage()
*/
QString XQServiceRequest::message() const
{ 
    XQSERVICE_DEBUG_PRINT("XQServiceRequest::message");
    XQSERVICE_DEBUG_PRINT("message: %s", qPrintable(mData->mMessage));
    return mData->mMessage;
}

/*!
    Gets the complete list of arguments for this service request.
    \return List of arguments set to the request.
    \sa setArguments()
*/
const QList<QVariant> &XQServiceRequest::arguments() const 
{
    XQSERVICE_DEBUG_PRINT("XQServiceRequest::arguments");
    return mData->mArguments; 
}

/*!
    Sets \a arguments for this service request.
    \param arguments Complete list of arguments for this service request
                     i.e. the values to be transferred to service provider
                     function to be called.
    \sa arguments()
*/
void XQServiceRequest::setArguments(const QList<QVariant> &arguments)
{
    XQSERVICE_DEBUG_PRINT("XQServiceRequest::setArguments");
    mData->mArguments = arguments;
}

/*!
    Gets the latest error that happened in the request execution.
    \return The latest error that happened in the request execution.
            Errors are defined in xqserviceglobal.h. 
    \sa XQService::ServiceIPCErrors.
*/
int XQServiceRequest::latestError()
    {
    XQSERVICE_DEBUG_PRINT("XQServiceRequest::latestError");
    return XQServiceAdaptor::latestError();
    }

/*!
    Sets additional options for the request, like embedding or start to background.
    \param info Additional info to be set to the request.
    \sa info()
*/
void XQServiceRequest::setInfo(const XQRequestInfo &info)
{
    XQSERVICE_DEBUG_PRINT("XQServiceRequest::setInfo");
    mData->mRequestUtil.mInfo = info;
}

/*!
    Gets current info set for the request.
    \return Info data set to the request.
    \sa setInfo()
*/
XQRequestInfo XQServiceRequest::info() const
{
    XQSERVICE_DEBUG_PRINT("XQServiceRequest::info");
    return mData->mRequestUtil.mInfo;
}

/*!
    \internal
    Adds the variant \a var to the list of arguments, so that the variant's
    value is serialized in send() rather than the variant itself.
*/
void XQServiceRequest::addArg(const QVariant& v)
{
    XQSERVICE_DEBUG_PRINT("XQServiceRequest::addArg %s,%d", v.typeName());
    XQSERVICE_DEBUG_PRINT("v: %s", qPrintable(v.toString()));
    mData->mArguments.append(v);
}

/*!
    \internal
    Picks the XQSharableFile argument, if any, into the request util
    This way scan parameter is listed only once.
*/
bool XQServiceRequest::handleSharableFileArgs()
{
    XQSERVICE_DEBUG_PRINT("XQServiceRequest::handleSharableFile");

    bool ret = true;
    mData->mRequestUtil.mSharableFileArgs.clear();
    
    for(int i=0; i < mData->mArguments.size(); i++)
    {
        if (QString(mData->mArguments[i].typeName()) == QString("XQSharableFile"))
        {
            XQSERVICE_DEBUG_PRINT("XQServiceRequest::sharable file detected");
            // Pick up the sharable file(s) to utility so that no need to scan any more later
            XQSharableFile file = mData->mArguments[i].value<XQSharableFile>();
            if (!file.isValid())
            {
                // No point to pass invalid file handle onwards
                XQSERVICE_DEBUG_PRINT("\t Invalid sharable file");
                ret = false;
                break;
            }
            if (mData->mRequestUtil.mSharableFileArgs.count() > 0)
            {
                XQSERVICE_DEBUG_PRINT("\t Too many sharable files");
                ret = false;
                break;
            }
            mData->mRequestUtil.mSharableFileArgs.append(file);
            XQSERVICE_DEBUG_PRINT("XQServiceRequest::sharable file added");
        }
    }

    if (!ret)
    {
        mData->mRequestUtil.mSharableFileArgs.clear();
    }

    return ret;
}


/*!
    Serializes all the arguments from the service request.
    \param action Defines the request having arguments to be serialized.
    \return Serialized arguments in byte array.
*/
QByteArray XQServiceRequest::serializeArguments(const XQServiceRequest &action)
{
    XQSERVICE_DEBUG_PRINT("XQServiceRequest::serializeArguments");
    QByteArray ret;
    QBuffer *buffer = new QBuffer(&ret);
    buffer->open(QIODevice::WriteOnly);
    QDataStream stream(buffer);
    stream << action.mData->mArguments;

    delete buffer;
    return ret;
}
/*!
    Deserializes all the arguments from the byte array to service request.
    \param action Defines the request where arguments are deserialized.
    \param data Defines the byte array of serialized arguments.
*/
void XQServiceRequest::deserializeArguments(XQServiceRequest &action,
        const QByteArray &data)
{
    XQSERVICE_DEBUG_PRINT("XQServiceRequest::deserializeArguments");
    QDataStream stream(data);
    stream >> action.mData->mArguments;
}

/*!
    Serializes this request to the stream.
    \param stream Defines stream this request is serialized to.
*/
template <typename Stream> void XQServiceRequest::serialize(Stream &stream) const
{
    XQSERVICE_DEBUG_PRINT("XQServiceRequest::serialize");
    stream << mData->mArguments;
    stream << mData->mService;
    stream << mData->mMessage;
    stream << mData->mSynchronous;
}

/*!
    Deserializes this request from the stream.
    \param stream Defines the stream this request is deserialized from.
*/
template <typename Stream> void XQServiceRequest::deserialize(Stream &stream)
{
    XQSERVICE_DEBUG_PRINT("XQServiceRequest::deserialize");
    stream >> mData->mArguments;
    stream >> mData->mService;
    stream >> mData->mMessage;
    stream >> mData->mSynchronous;
}

Q_IMPLEMENT_USER_METATYPE(XQServiceRequest)
