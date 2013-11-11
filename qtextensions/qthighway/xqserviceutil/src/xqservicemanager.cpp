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

#include <e32std.h>
#include <w32std.h>
#include <apaidpartner.h>
#include <apparc.h>
#include <apgcli.h>
#include <apacln.h>
#include <apgtask.h>
#include <barsread2.h>
#include <apacmdln.h>
#include <coemain.h>

#include <QBuffer>
#include <QString>
#include <QCoreApplication>
#include <QHash>

#include "xqservicelog.h"
#include <xqserviceglobal.h>
#include "xqrequestutil.h"
#include "xqservicemanager.h"
#include "xqserviceipcconst.h"

#include "xqservicemetadata/xqservicemetadata_p.h"
#include <xqservicemetadata/xqaiwinterfacedescriptor.h>
#include "xqconversions.h"
#include "processinfo.h"


/*!
    \class XQServiceManagerPrivate
    \brief Private implementation of the XQServiceManager.
*/
class XQServiceManagerPrivate 
    {
    public:
        XQServiceManagerPrivate();
        ~XQServiceManagerPrivate();

        enum matchMode
        {
            MatchInterfaceName,
            MatchServiceAndInterfaceName
        };
        
        int StartServer(const QString& aService,  bool embedded, int& applicationUid, quint64& processId,
                       XQRequestUtil *util);
        TInt Discover(const QString& aService,TUid& aAppUid, QList<XQAiwInterfaceDescriptor>& interfaces, int matchMode,
                      bool findFirst=false);
        int  LatestError() const {return iLatestError;};
        bool IsRunning(const XQAiwInterfaceDescriptor& implementation) const;
        
    private:
        void StartServerL(const TUid& uid, bool embedded, TUint64& processId, XQRequestUtil *util);
        TInt Discover(const TDesC& aService,TUid& aAppUid, QList<XQAiwInterfaceDescriptor>& interfaces, int matchMode,
                      bool findFirst=false);
        TInt Discover1(const TDesC& aService,TUid& aAppUid, QList<XQAiwInterfaceDescriptor>& interfaces, int matchMode,
                       bool findFirst=false);
        TInt Discover2(const TDesC& aService,TUid& aAppUid, QList<XQAiwInterfaceDescriptor>& interfaces, int matchMode,
                       bool findFirst=false);
        CApaAppServiceInfoArray* AvailableServiceImplementations1L();
        CApaAppServiceInfoArray* AvailableServiceImplementations2L();
        TUint64 getAppPid(const TUid& aAppUid);   
        int doMapErrors(TInt aError);
        
        TVersion iVersion;
        TApaAppInfo  iAppInfo;
        int iLatestError;
        RApaLsSession iApaSession;
        XQAiwInterfaceDescriptor iImplDescriptor; 
    };

/*!
    \class XQServiceManager
    \brief Discovery and service startup. 
*/

/*!
    Constructor.
*/
XQServiceManager::XQServiceManager()
{
    XQSERVICE_DEBUG_PRINT("XQServiceManager::XQServiceManager");
    d = new XQServiceManagerPrivate();
}

/*!
    Destructor.
*/
XQServiceManager::~XQServiceManager()
{
    XQSERVICE_DEBUG_PRINT("XQServiceManager::~XQServiceManager");
    delete d;
}

/*!
    Starts service
    \param service The full name of service (servicename + interfacename).
    \param embedded Start in embedded mode.
    \param applicationUid Returned applicatiion.
    \param threadId Returned process id of the application.
    \return Error code if error occured, 0 otherwise.
*/
int XQServiceManager::startServer(const QString& service, bool embedded, int& applicationUid, quint64& threadId)
{
    XQSERVICE_DEBUG_PRINT("XQServiceManager::startServer(1)");
    return startServer(service,embedded,applicationUid,threadId,NULL);
}

/*!
    Starts service
    \param service The full name of service (servicename + interfacename).
    \param embedded Start in embedded mode.
    \param applicationUid Returned applicatiion.
    \param threadId Returned process id of the application.
    \param userData Additional user data.
    \return Error code if error occured, 0 otherwise.
*/
int XQServiceManager::startServer(const QString& service, bool embedded, int& applicationUid, quint64& threadId,
                                 const void *userData)
{
    XQSERVICE_DEBUG_PRINT("XQServiceManager::startServer(2)");

    // The "XQServiceRequest::send(QVariant& retData)" function passed the utility as user data
    // the IPC layer. Could be NULL if no descriptor was given upon creating the request.
    // The util data is known to be writeable (though passed as const userData)
    XQRequestUtil *util = static_cast<XQRequestUtil*>((void *)userData);
    if (util == 0)
    {
        // Something is badly wrong as this should be always avaible
        return XQService::EMgrInternalError;
    }
    
    return d->StartServer(service,embedded,applicationUid,threadId, util);
}


/*!
    Finds implementations for the given interface.
    \param interfaceName Interfacename to match.
    \return List of implementations.
*/
QList<XQAiwInterfaceDescriptor>  XQServiceManager::findInterfaces ( const QString &interfaceName ) const
    {
    XQSERVICE_DEBUG_PRINT("XQServiceManager::findInterfaces 1");
    QList<XQAiwInterfaceDescriptor> interfaces;
    TUid appUid;
    interfaces.clear();
    TInt error=d->Discover(interfaceName, appUid, interfaces, XQServiceManagerPrivate::MatchInterfaceName);
    return interfaces;
    }

/*!
    Finds implementations for the given interface implemented by given service.
    \param serviceName Service name.
    \param interfaceName Interfacename to match.
    \return List of implementations.
*/
QList<XQAiwInterfaceDescriptor>  XQServiceManager::findInterfaces ( const QString &serviceName, const QString &interfaceName ) const
{
    XQSERVICE_DEBUG_PRINT("XQServiceManager::findInterfaces 2");
    QList<XQAiwInterfaceDescriptor> interfaces;
    TUid appUid;
    interfaces.clear(); 
    TInt error=d->Discover(serviceName + "." + interfaceName, appUid, interfaces,
                           XQServiceManagerPrivate::MatchServiceAndInterfaceName);
    return interfaces;
}


/*!
    Finds the first implementation for the given interface name.
    \param interfaceName Interfacename to match.
    \return List of implementations.
*/
QList<XQAiwInterfaceDescriptor>  XQServiceManager::findFirstInterface ( const QString &interfaceName ) const
{
    XQSERVICE_DEBUG_PRINT("XQServiceManager::findFirstInterface 1");
    QList<XQAiwInterfaceDescriptor> interfaces;
    TUid appUid;
    interfaces.clear();
    TInt error=d->Discover(interfaceName, appUid, interfaces, XQServiceManagerPrivate::MatchInterfaceName, true);
    return interfaces;
}

/*!
    Finds the first implementation for the given service + interface names.
    \param serviceName Service name.
    \param interfaceName Interfacename to match.
    \return List of implementations.
*/
QList<XQAiwInterfaceDescriptor>  XQServiceManager::findFirstInterface ( const QString &serviceName, const QString &interfaceName ) const
{
    XQSERVICE_DEBUG_PRINT("XQServiceManager::findFirstInterface 2");
    QList<XQAiwInterfaceDescriptor> interfaces;
    TUid appUid;
    interfaces.clear(); 
    // Catenate to get full name
    TInt error=d->Discover(serviceName + "." + interfaceName, appUid, interfaces,
                           XQServiceManagerPrivate::MatchServiceAndInterfaceName, true);
    return interfaces;
}


/*!
    Gets the latest error occured.
    \return Latest error code as integer value.
*/
int XQServiceManager::latestError() const
{
    return d->LatestError();
}

/*!
    Checks if the given \a implmentation is running.
    \param implementation Implementation to be checked.
    \return true if given \a implementation is running, false otherwise.
*/
bool XQServiceManager::isRunning(const XQAiwInterfaceDescriptor& implementation) const
{
    XQSERVICE_DEBUG_PRINT("XQServiceManager::isRunning");
    return d->IsRunning(implementation);
}

// ====== Private part ==============

XQServiceManagerPrivate::XQServiceManagerPrivate()
{
    XQSERVICE_DEBUG_PRINT("XQServiceManagerPrivate::XQServiceManagerPrivate");
    iLatestError = 0;
    iApaSession.Connect();    
}

XQServiceManagerPrivate::~XQServiceManagerPrivate()
{
    XQSERVICE_DEBUG_PRINT("XQServiceManagerPrivate::~XQServiceManagerPrivate");
    iApaSession.Close();
};

// aService is here the full name (service + interface)
int XQServiceManagerPrivate::StartServer(const QString& aService,  bool embedded, int& applicationUid, quint64& processId,
                                        XQRequestUtil *util)
{
    XQSERVICE_DEBUG_PRINT("XQServiceManagerPrivate::startServer(2)");
    XQSERVICE_DEBUG_PRINT("aService: %s", qPrintable(aService));
    
    TUid appUid;
    appUid.iUid=0;
    
    TInt error = KErrNone;
    QList<XQAiwInterfaceDescriptor> interfaces;
    TPtrC serverName( reinterpret_cast<const TUint16*>(aService.utf16()) );
    if (util->mDescriptor.isValid()) 
    {
        iImplDescriptor=util->mDescriptor;  // Descriptor given by caller
        appUid.iUid = util->mDescriptor.property(XQAiwInterfaceDescriptor::ImplementationId).toInt();
        XQSERVICE_DEBUG_PRINT("ApplicationUid from descriptor: %x", appUid.iUid);
    }
    
    // Need to discover service first if descriptor did not contained valid UID
    // Otherwise, go directly starting the service server
    if (appUid.iUid == 0)
    {
        // Find the first implementation
        error = Discover(serverName,appUid,interfaces, XQServiceManagerPrivate::MatchServiceAndInterfaceName, true);
        if (interfaces.count())
        {
            iImplDescriptor=interfaces[0];  // Descriptor search upon start
        }
    }
    if (error)
        {
        return doMapErrors(error);
        }

    TRAP(error, StartServerL(appUid,embedded,processId, util));
    applicationUid = appUid.iUid ;

    XQSERVICE_DEBUG_PRINT("ApplicationUid: %x, processId: %d", applicationUid, processId);
    XQSERVICE_DEBUG_PRINT("error: %d", error);
    return doMapErrors(error);
}

void XQServiceManagerPrivate::StartServerL(const TUid& uid, bool embedded, TUint64& processId,
                                           XQRequestUtil *util)
{
    XQSERVICE_DEBUG_PRINT("XQServiceManagerPrivate::StartServerL");
    Q_UNUSED(embedded);  // Not used any more. XQRequestUtil applied instead
    
    bool toBackground = false;
    // Apply the utility's option for embedding  instead
    bool embed = util->mInfo.isEmbedded();
    toBackground = util->mInfo.isBackground();
    
    XQSERVICE_DEBUG_PRINT("\tembedded got from utility=%d", embed);
    XQSERVICE_DEBUG_PRINT("\tbackground got from utility=%d", toBackground);

    // retrieve application information
    User::LeaveIfError( iApaSession.GetAppInfo( iAppInfo, uid ) );

    TApaAppCapabilityBuf caps;
    User::LeaveIfError(iApaSession.GetAppCapability(caps, uid));
    if (!toBackground)
    {
        // If service wants to be launched to background.. respect it
        toBackground = caps().iLaunchInBackground;
        XQSERVICE_DEBUG_PRINT("\tbackground from apparch=%d", toBackground);
    }

    // Consistency check
    if (embed && toBackground)
    {
        User::Leave(KErrArgument);
    }

    /*
    // Using the "iAppInfo.iCaption" is wrong as the channel name (full servicename) shall be used:
    // e.g. tester's "com.nokia.servicesd.serviceapp.Dialer" or
    //       "com.nokia.servicesd.serviceapp.Dialer.PROCESS-ID" in case of embedded launch
    // Anyway, this is not needed as the "XQServiceIpcClient::connectToServer()" takes care of the checking logic
    // earlier (the "startServer" logic will be applied only when needed)
    TFindServer find( iAppInfo.iCaption );
    TFullName fullName;
    TInt err = find.Next( fullName );
    XQSERVICE_DEBUG_PRINT("err: %d", err);
    */
    TInt err = KErrNotFound;  // Assume not found
    if ( err != KErrNone )
        {
        CApaCommandLine* cmdLine = CApaCommandLine::NewLC();
        cmdLine->SetExecutableNameL( iAppInfo.iFullName );
        TApaCommand  aLaunchCommand = toBackground ? EApaCommandBackground : EApaCommandRun;
        cmdLine->SetCommandL(aLaunchCommand);

        // just use the Secure ID as server differentiator
        //cmdLine->SetServerRequiredL( uid.iUid );
        RProcess client;
        CleanupClosePushL(client);
        
        cmdLine->SetServerRequiredL(client.Id().Id());
        if (embed) {
            CCoeEnv* env= CCoeEnv::Static();
            if (env)  // Could be NULL 
            {
                RWindowGroup& wg = env->RootWin();
                wg.AllowProcessToCreateChildWindowGroups(iAppInfo.iUid);
                TInt parentId = wg.Identifier();
                if (parentId)
                {
                    // pass our window group ID to the embedded child process
                    cmdLine->SetParentWindowGroupID(parentId);
                    XQSERVICE_DEBUG_PRINT("\tParent ID %x set for %x (%x)", (int)parentId, iAppInfo.iUid, uid.iUid);
                }
            }
            else
            {
                // Can not be embedded (non GUI client)
                embed = false;
            }
        }
        else {
            if(!CProcessInfo::EnsureProcessCanStartL(uid))
                User::Leave(KErrAlreadyExists);
        }
        CleanupStack::PopAndDestroy(&client);
        
        TRequestStatus requestStatusForRendezvous;
        
        // start application with command line parameters
        //User::LeaveIfError( iApaSession.StartApp( *cmdLine, threadId, &requestStatusForRendezvous) );
        QString startupArgs = QString::fromLatin1(XQServiceUtils::StartupArgService);
        if (embed)
        {
            startupArgs += (" " + QString::fromLatin1(XQServiceUtils::StartupArgEmbedded));
        }

        //
        // Add interface name and operation (message) name to the command line as startup args.
        // Usable in practise only for the embedded launch
        // Can be used  by service application to prepare the UI to the coming call.
        //
        QStringList l = util->mOperation.split("("); 
        QString oper = l.value(0); //  // Pick only the function name and ignore parameters
        
        startupArgs += (" " + QString::fromLatin1(XQServiceUtils::StartupArgInterfaceName) + iImplDescriptor.interfaceName() );
        startupArgs += (" " + QString::fromLatin1(XQServiceUtils::StartupArgServiceName) + iImplDescriptor.serviceName() );
        startupArgs += (" " + QString::fromLatin1(XQServiceUtils::StartupArgOperationName) + oper);
        
        XQSERVICE_DEBUG_PRINT("\tStartupArgs:%s", qPrintable(startupArgs));
        TPtrC cmdLineArgs( reinterpret_cast<const TUint16*>(startupArgs.utf16()) );
        
        RProcess newApp;
        XQSERVICE_DEBUG_PRINT("XQServiceManagerPrivate::Creating process");
        User::LeaveIfError(newApp.Create(iAppInfo.iFullName, cmdLineArgs));
        CleanupClosePushL(newApp);
        
        XQSERVICE_DEBUG_PRINT("XQServiceManagerPrivate::Created process");
        cmdLine->SetProcessEnvironmentL(newApp);
        TProcessId newAppId = newApp.Id();
        processId = newAppId.Id();
        XQSERVICE_DEBUG_PRINT("XQServiceManagerPrivate::Rendezvous for %x", processId);
        newApp.Rendezvous(requestStatusForRendezvous);  // Asynchronous
        newApp.Resume();
        
        User::WaitForRequest( requestStatusForRendezvous ); // Make the  rendezvouz
        XQSERVICE_DEBUG_PRINT("XQServiceManagerPrivate::Rendezvous done %d", requestStatusForRendezvous.Int());

        if (!embed) {
            CProcessInfo::AddProcessL(uid, newApp);
        }
        
        User::LeaveIfError( requestStatusForRendezvous.Int()); 
        CleanupStack::PopAndDestroy(2,cmdLine); // newApp, cmdLine
        
        XQSERVICE_DEBUG_PRINT("XQServiceManagerPrivate::Done"); 
        }
    
}


TUint64 XQServiceManagerPrivate::getAppPid(const TUid& aAppUid)
{
    XQSERVICE_DEBUG_PRINT("XQServiceManagerPrivate::getAppPid");
    TUint64 pid = 0;
    // Get the current task
    RWsSession ws;
    if (ws.Connect()==KErrNone) {
        XQSERVICE_DEBUG_PRINT("Connected to window server");
        TApaTaskList tasklist( ws );
        TApaTask task = tasklist.FindApp( aAppUid );        
        if (task.Exists()) {
            XQSERVICE_DEBUG_PRINT("Application found");
            pid=task.ThreadId().Id();
        }
    }
    return pid;
}

CApaAppServiceInfoArray* XQServiceManagerPrivate::AvailableServiceImplementations1L()
{
    XQSERVICE_DEBUG_PRINT("XQServiceManagerPrivate::AvailableServiceImplementations1L");
    // retrieve list of available services implementations from apparc
    CApaAppServiceInfoArray* apaInfo = 
                    iApaSession.GetServiceImplementationsLC(TUid::Uid(KXQServiceUid));
    CleanupStack::Pop( apaInfo );
    return apaInfo;
}

CApaAppServiceInfoArray* XQServiceManagerPrivate::AvailableServiceImplementations2L()
{
    XQSERVICE_DEBUG_PRINT("XQServiceManagerPrivate::AvailableServiceImplementations2L");
    // retrieve list of available services implementations from apparc
    CApaAppServiceInfoArray* apaInfo = 
                    iApaSession.GetServiceImplementationsLC(TUid::Uid(KXQServiceUid2));
    CleanupStack::Pop( apaInfo );
    return apaInfo;
}

TInt XQServiceManagerPrivate::Discover(const QString& aService,TUid& aAppUid, QList<XQAiwInterfaceDescriptor>& interfaces,
                                      int matchMode, bool findFirst)
    {
    TPtrC serverName( reinterpret_cast<const TUint16*>(aService.utf16()) );
    TInt error=Discover(serverName, aAppUid, interfaces, matchMode, findFirst);
    XQSERVICE_DEBUG_PRINT("XQServiceManagerPrivate::Discover (1)");
    return error;
    }

TInt XQServiceManagerPrivate::Discover( const TDesC& aService,
                                      TUid& aAppUid, QList<XQAiwInterfaceDescriptor>& interfaces,
                                      int matchMode, bool findFirst)
    {
    XQSERVICE_DEBUG_PRINT("XQServiceManagerPrivate::Discover (2)");
    TInt discoverResult1 = KErrNotFound;
    TInt discoverResult2 = KErrNotFound;

    // Discover first possible reg files with old format
    discoverResult1 = Discover1(aService, aAppUid, interfaces, matchMode, findFirst);
    // Discover then  reg files with new format (add results)
    discoverResult2 = Discover2(aService, aAppUid, interfaces, matchMode, findFirst);
    
    if (discoverResult1 == KErrNone || discoverResult2 == KErrNone)
        {
        // Results merged
        return KErrNone;
        }
    else
        {
        return KErrNotFound;
        }
    }

TInt XQServiceManagerPrivate::Discover1( const TDesC& aService,
                                      TUid& aAppUid, QList<XQAiwInterfaceDescriptor>& interfaces,
                                      int matchMode, bool findFirst)
    {
    XQSERVICE_DEBUG_PRINT("XQServiceManagerPrivate::Discover1");
        
    CApaAppServiceInfoArray* apaInfo = NULL;
    TInt error = KErrNone;
    TRAP(error, apaInfo = AvailableServiceImplementations1L());
    XQSERVICE_DEBUG_PRINT("Discover status=%d", error);
    if (error)
        {
        return error;  // This is fatal as nothing found
        }
    TArray<TApaAppServiceInfo> implArray( apaInfo->Array() );
    XQSERVICE_DEBUG_PRINT("implArray.Count(): %d", implArray.Count());
    if ( !implArray.Count() )
        {
        delete apaInfo;
        return KErrNotFound;   // No services found
        }
    
    TBool found( EFalse );
    QString serviceName = QString::fromUtf16(aService.Ptr(),aService.Length());
    XQSERVICE_DEBUG_PRINT("serviceName: %s", qPrintable(serviceName));
    TBool firstUidPicked(EFalse);
    
    for ( TInt ii = 0; ii < implArray.Count(); ii++ )  
        {

        //
        // Reset error for each check round
        //
        error = KErrNone;
            
        TUid uid = implArray[ii].Uid();
        XQSERVICE_DEBUG_PRINT("implArray[%d].UID=%x", ii, uid);
            
        RResourceReader res;
        // read opaque data
        TRAP(error,res.OpenL( implArray[ii].OpaqueData() ) )
        if ( error )
            {
            XQSERVICE_DEBUG_PRINT("OpaqueData error: %d", error);
            delete apaInfo;
            apaInfo = NULL;
            continue;  // Next could be OK
            }
        
        //the first WORD contains the number of elements in the resource
        int count = 0;
        TRAP(error,count = res.ReadInt16L());
        if ( error )
            {
            XQSERVICE_DEBUG_PRINT("resource error1: %d", error);
            res.Close();
            delete apaInfo;
            apaInfo = NULL;
            continue;  // Next could be OK
            }      
        QByteArray xmlConf ;
        XQSERVICE_DEBUG_PRINT("resource line count: %d", count);
        for (int i=0; i < count ; i++) 
            {
            TPtrC16 xmlBuf;
            TRAP(error, xmlBuf.Set( res.ReadTPtrC16L() ));
            if (error)
                {
                XQSERVICE_DEBUG_PRINT("resource error2: %d", error);
                res.Close();
                delete apaInfo;
                apaInfo = NULL;
                break;
                }
            else
                {
                QString str=QString::fromUtf16(xmlBuf.Ptr(),xmlBuf.Length());
                XQSERVICE_DEBUG_PRINT("resource str: %s", qPrintable(str));
                xmlConf.append(str.toAscii());
                }
            }

        // If we stop loop for the first resource error,
        // it will hit cases where same interface has been implemented by multiple services
        // So go on checking all the resource files
        if (error)
        {
            continue; // Next could be OK
        }
        
        XQSERVICE_DEBUG_PRINT("resource data: %s", xmlConf.constData());
        QBuffer buf(&xmlConf);
        ServiceMetaData* metaData = new ServiceMetaData(&buf);
        if (metaData->extractMetadata()) 
            {
            ServiceMetaDataResults results=metaData->parseResults();
            // interfaces = results.interfaces;  // return value set here ???!!

            // Go through all interfaces and pick the UI for the first matching one.
            // THIS NEED TO BE FIXED IF SOMEONE WANTS DEDICATED IMPLEMENTATION
            // Fill in the implementationId for all interfaces
            foreach (XQAiwInterfaceDescriptor interface,results.interfaces)
                {
                QString sn;
                QString snDeprecated;
                if (results.version == ServiceMetaDataResults::VERSION_1)
                    {
                    // Old version of the XML format. The parser took care of adaptation
                    // discovery-name = service-name + interface name
                    XQSERVICE_DEBUG_PRINT("version 1");
                    }
                else
                    {
                    // discovery-name = interface name
                   XQSERVICE_DEBUG_PRINT("version 2");
                    }

                // Deprecated service name, if any
                QString deprecatedServiceName = interface.customProperty("deprecatedsn");
                bool deprNameExists = !deprecatedServiceName.isEmpty();
                if (deprNameExists)
                {
                    XQSERVICE_DEBUG_PRINT("deprecatedServiceName: %s", qPrintable(deprecatedServiceName));
                }
                
                // This is the name used in match
                // TODO: Version handling support: Take the latest version if multiple matches
                switch (matchMode)
                {
                    case MatchInterfaceName :
                        sn = interface.interfaceName();
                        break;
                    case MatchServiceAndInterfaceName :
                        sn =interface.serviceName() + "." + interface.interfaceName();
                        snDeprecated = deprecatedServiceName + "." + interface.interfaceName();
                        break;
                    default:
                        sn = interface.interfaceName();
                       break;
                }

                XQSERVICE_DEBUG_PRINT("compare name is: %s", qPrintable(sn));
                XQSERVICE_DEBUG_PRINT("requested name: %s", qPrintable(serviceName));
                if ((!serviceName.compare(sn,Qt::CaseInsensitive)) ||
                    (deprNameExists && !serviceName.compare(snDeprecated,Qt::CaseInsensitive)))
                    {
                    TUid appUid = implArray[ii].Uid();
                    if (!firstUidPicked)
                        {
                        aAppUid = appUid;
                        firstUidPicked = ETrue;
                        XQSERVICE_DEBUG_PRINT("First service found UID3=%x", appUid.iUid);
                        }
                    XQSERVICE_DEBUG_PRINT("Service found UID3=%x", appUid.iUid);
                    //  Add impl. UID to interface
                    interface.setProperty(XQAiwInterfaceDescriptor::ImplementationId, (int)appUid.iUid);
                    found = ETrue;

                    // Add the matched interface to result set
                    interfaces.append(interface);
                    
                    }
                } // forearch interface
            } 
        else
            {
             error = metaData->getLatestError();
             iLatestError = error;
             XQSERVICE_DEBUG_PRINT("metadata error: %d", error); 
            }  
        
        delete metaData;
        metaData = NULL;
        res.Close();


        // If only first found needed, quit the loop.
        if (findFirst && firstUidPicked)
            {
            XQSERVICE_DEBUG_PRINT("First service returned UID3=%x", aAppUid.iUid);
            break;
            }
        
        } // for implArray ...
    
    delete apaInfo;
    if (!found)
        {
        error = KErrNotFound;
        }
    if (found)
        {
        error = KErrNone;
        }
    
    XQSERVICE_DEBUG_PRINT("Discover error: %d", error);
    
    return error;
    }


TInt XQServiceManagerPrivate::Discover2( const TDesC& aService,
                                      TUid& aAppUid, QList<XQAiwInterfaceDescriptor>& interfaces,
                                      int matchMode, bool findFirst)
    {
    XQSERVICE_DEBUG_PRINT("XQServiceManagerPrivate::Discover2");
        
    CApaAppServiceInfoArray* apaInfo = NULL;
    TInt error = KErrNone;
    
    TRAP(error, apaInfo = AvailableServiceImplementations2L());
    XQSERVICE_DEBUG_PRINT("Discover status=%d", error);
    
    if (error)
        {
        return error;  // This is fatal as nothing found
        }
    TArray<TApaAppServiceInfo> implArray( apaInfo->Array() );
    XQSERVICE_DEBUG_PRINT("implArray.Count(): %d", implArray.Count());
    
    if ( !implArray.Count() )
        {
        delete apaInfo;
        return KErrNotFound;   // No services found
        }
    
    TBool found( EFalse );
    
    QString serviceName = QString::fromUtf16(aService.Ptr(),aService.Length());
    XQSERVICE_DEBUG_PRINT("serviceName: %s", qPrintable(serviceName));
    TBool firstUidPicked(EFalse);

    for ( TInt ii = 0; ii < implArray.Count(); ii++ )  
        {
        TUid uid = implArray[ii].Uid();
        XQSERVICE_DEBUG_PRINT("implArray[%d].UID=%x", ii, uid);

        QByteArray xmlConf ;

        TPtrC8 opaque = implArray[ii].OpaqueData();
        const TPtrC16 tmpXml((TText16*) opaque.Ptr(),(opaque.Length()+1)>>1);
        QString strXml = XQConversions:: s60DescToQString( tmpXml ) ;
        // XQSERVICE_DEBUG_PRINT("XML conf: %s", qPrintable(strXml));
        XQSERVICE_DEBUG_PRINT("size of xml conf.: %d characters", strXml.size());
        xmlConf.append(strXml.toAscii());
        
        XQSERVICE_DEBUG_PRINT("resource data: %s", xmlConf.constData());
        QBuffer buf(&xmlConf);
        ServiceMetaData* metaData = new ServiceMetaData(&buf);
        if (metaData->extractMetadata()) 
            {
            ServiceMetaDataResults results=metaData->parseResults();

            // Go through all interfaces and pick the UI for the first matching one.
            // THIS NEED TO BE FIXED IF SOMEONE WANTS DEDICATED IMPLEMENTATION
            // Fill in the implementationId for all interfaces
            foreach (XQAiwInterfaceDescriptor interface,results.interfaces)
                {
                QString sn;
                QString snDeprecated;
                if (results.version == ServiceMetaDataResults::VERSION_1)
                    {
                    // Old version of the XML format. The parser took care of adaptation
                    // discovery-name = service-name + interface name
                    XQSERVICE_DEBUG_PRINT("version 1");
                    }
                else
                    {
                    // discovery-name = interface name
                   XQSERVICE_DEBUG_PRINT("version 2");
                    }

                // Deprecated service name, if any
                QString deprecatedServiceName = interface.customProperty("deprecatedsn");
                bool deprNameExists = !deprecatedServiceName.isEmpty();
                if (deprNameExists)
                {
                    XQSERVICE_DEBUG_PRINT("deprecatedServiceName: %s", qPrintable(deprecatedServiceName));
                }
                // This is the name used in match
                // TODO: Version handling support: Take the latest version if multiple matches
                switch (matchMode)
                {
                    case MatchInterfaceName :
                        sn = interface.interfaceName();
                        break;
                    case MatchServiceAndInterfaceName :
                        sn =interface.serviceName() + "." + interface.interfaceName();
                        snDeprecated = deprecatedServiceName + "." + interface.interfaceName();
                        break;
                    default:
                        sn = interface.interfaceName();
                        break;
                }

                XQSERVICE_DEBUG_PRINT("compare name is: %s", qPrintable(sn));
                XQSERVICE_DEBUG_PRINT("requested name: %s", qPrintable(serviceName));
                if ((!serviceName.compare(sn,Qt::CaseInsensitive)) ||
                    (deprNameExists && !serviceName.compare(snDeprecated,Qt::CaseInsensitive)))
                    {
                    TUid appUid = implArray[ii].Uid();
                    if (!firstUidPicked)
                        {
                        aAppUid = appUid;
                        firstUidPicked = ETrue;
                        XQSERVICE_DEBUG_PRINT("First service found UID3=%x", appUid.iUid);
                        }
                    XQSERVICE_DEBUG_PRINT("Service found UID3=%x", appUid.iUid);
                    //  Add impl. UID to interface
                    interface.setProperty(XQAiwInterfaceDescriptor::ImplementationId, (int)appUid.iUid);
                    found = ETrue;

                    // Add the matched interface to result set
                    interfaces.append(interface);
                    }

                    if (found)
                    {
                        error = KErrNone;
                    }
                } // forearch interface
            }
        else
            {
             error = metaData->getLatestError();
             iLatestError = error;
             XQSERVICE_DEBUG_PRINT("metadata error: %d", error); 
            }  
        
        delete metaData;
        metaData = NULL;

        // If only first found needed, quit the loop.
        if (findFirst && firstUidPicked)
            {
            XQSERVICE_DEBUG_PRINT("First service returned UID3=%x", aAppUid.iUid);
            break;
            }
        } // for implArray ...
    
    delete apaInfo;
    if (!found)
        {
        error = KErrNotFound;
        }
    
    if (found)
        {
        error = KErrNone;
        }
    
    XQSERVICE_DEBUG_PRINT("Discover error: %d", error);
    
    return error;
}

bool XQServiceManagerPrivate::IsRunning(const XQAiwInterfaceDescriptor& implementation) const
{
    XQSERVICE_DEBUG_PRINT("XQServiceManagerPrivate::IsRunning");
    QString fullServiceName = implementation.serviceName() + "." +  implementation.interfaceName();
    TPtrC serverName( reinterpret_cast<const TUint16*>(fullServiceName.utf16()) );
    
    TFindServer findServer(serverName);
    TFullName name;
    bool b = findServer.Next(name) == KErrNone;
    XQSERVICE_DEBUG_PRINT("XQServiceManagerPrivate::IsRunning=%d",b);
    return b;
}

    
int XQServiceManagerPrivate::doMapErrors(TInt aError)
{
    XQSERVICE_DEBUG_PRINT("XQServiceManagerPrivate::doMapErrors");
    XQSERVICE_DEBUG_PRINT("aError: %d", aError);
    int error(XQService::ENoError);
    
    switch (aError)
    {
    case KErrNone: {
        error = XQService::ENoError;
        break;
    }
    case KErrPermissionDenied:
    case KErrServerTerminated: {
        error = XQService::EConnectionClosed;
        break;
    }
    case KErrServerBusy: {
        error = XQService::EConnectionError;
        break;
    }
    case KErrArgument:
    {
        error = XQService::EArgumentError;
        break;
    }
        
    case KErrNoMemory: {
        error = XQService::EIPCError;
        break;
    }
    case KErrNotFound: {
        error = XQService::EServerNotFound;
        break;
    }

    default:
    {

        if (aError >= XQService::EMetaNoService && aError <= XQService::EMetaDuplicatedCustomKey)
        {
            iLatestError = error;
            return error;  // Already real error
        }
        
        error = XQService::EUnknownError;
        break;
    }
    }

    // Save error
    iLatestError = error;
    
    XQSERVICE_DEBUG_PRINT("error: %d", error);
    return error;
}



