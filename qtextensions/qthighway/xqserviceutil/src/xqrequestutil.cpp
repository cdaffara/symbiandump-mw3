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
#include "xqserviceerrdefs.h"

#include <w32std.h> // RWsSession
#include <coemain.h>
#include <QCoreApplication>

#include "xqrequestutil.h"
   
// ======== LOCAL CLASSES ========

class CParentObserver: public CActive
{
public:

    /**
     * Starts to observe if parent is still running and close embedded view when parent closed
     */
    static void closeWhenParentClosedL();

    /**
     * C++ destructor.
     */
    virtual ~CParentObserver();
        
protected:
    
    // @see CActive
    virtual void RunL();
    
    // @see CActive
    virtual void DoCancel(); 
    
    // @see CActive
    virtual TInt RunError( TInt aError ); 
    
private:

    CParentObserver();
    void ConstructL();
    void IssueRequest();
    void HandleWsEventL(const TWsEvent &aEvent);

private: // Data

    class ParentObserverDestructor
    {
    public:
        ParentObserverDestructor():iObserver(NULL){}
        
        ~ParentObserverDestructor()
        {
            delete iObserver;
        }
        
        CParentObserver* iObserver;
    };
    
    static ParentObserverDestructor iInstance;
    
    /** Window server session */
    RWsSession iWsSession;
    
    /** Window group for receiving window server events */
    RWindowGroup* iWindowGroup;
};

CParentObserver::ParentObserverDestructor CParentObserver::iInstance;

// ---------------------------------------------------------------------------
// C++ constructor.
// ---------------------------------------------------------------------------
//
CParentObserver::CParentObserver():
    CActive(CActive::EPriorityStandard)
{
    CActiveScheduler::Add(this);
}

// ---------------------------------------------------------------------------
// Symbian 2nd phase constructor.
// ---------------------------------------------------------------------------
//
void CParentObserver::ConstructL()
{
    // Connect to window server server
    User::LeaveIfError(iWsSession.Connect());
    
    // Construct window group
    iWindowGroup = new(ELeave) RWindowGroup(iWsSession);
    User::LeaveIfError( iWindowGroup->Construct((TUint32)this, EFalse));
    User::LeaveIfError(iWindowGroup->EnableGroupChangeEvents());
}

// ---------------------------------------------------------------------------
// Starts to observe if parent is still running and close embedded view when parent closed
// ---------------------------------------------------------------------------
//
void CParentObserver::closeWhenParentClosedL()
{
    if (!iInstance.iObserver) {
        iInstance.iObserver = new (ELeave) CParentObserver();
        iInstance.iObserver->ConstructL();
    }
    
    iInstance.iObserver->IssueRequest();
}

// ---------------------------------------------------------------------------
// C++ destructor.
// ---------------------------------------------------------------------------
//
CParentObserver::~CParentObserver()
{
    Cancel();
    
    // Cleanup window group
    if(iWindowGroup) {
        iWindowGroup->DisableGroupChangeEvents();
        iWindowGroup->Close();
        delete iWindowGroup;
    }
    
    // Cleanup window server session
    iWsSession.Close();
}

//------------------------------------------------------------------------------
// CParentObserver::IssueRequest
//------------------------------------------------------------------------------
//
void CParentObserver::IssueRequest()
{
    if(!IsActive()) {
        // Request events from window server
        iWsSession.EventReady(&iStatus);
        SetActive();
    }
}

//------------------------------------------------------------------------------
// CParentObserver::HandleWsEventL
//------------------------------------------------------------------------------
//
void CParentObserver::HandleWsEventL(const TWsEvent &aEvent)
{
    if (aEvent.Type() == EEventWindowGroupsChanged) {
        RWindowGroup rootWG = CCoeEnv::Static()->RootWin();
        int rootWGId = rootWG.Identifier();
        
        RArray<RWsSession::TWindowGroupChainInfo> list;
        CleanupClosePushL(list);
        iWsSession.WindowGroupList(&list);
        
        for(int idx = 0; idx < list.Count(); idx++)
        {
            RWsSession::TWindowGroupChainInfo info = list[idx];
            if (info.iId == rootWGId && info.iParentId <= 0) {
               qApp->quit();
               break;
            }
        }
        CleanupStack::PopAndDestroy(); // list
    }
}

//------------------------------------------------------------------------------
// CParentObserver::RunL
//------------------------------------------------------------------------------
//
void CParentObserver::RunL()
{
    TInt err = iStatus.Int();
    if(err == KErrNone) {
        // No errors occured, fetch event
        TWsEvent wsEvent;
        iWsSession.GetEvent(wsEvent);
        
        // Continue listening
        IssueRequest();
        
        // Forward event to observer
        HandleWsEventL(wsEvent);
    }
    else {
        // Continue listening
        IssueRequest();
    }    
}

//------------------------------------------------------------------------------
// CParentObserver::DoCancel
//------------------------------------------------------------------------------
//
void CParentObserver::DoCancel()
{
    // Cancel event ready from window server
    iWsSession.EventReadyCancel();
}

//------------------------------------------------------------------------------
// CParentObserver::RunError
//------------------------------------------------------------------------------
//
TInt CParentObserver::RunError(TInt /*aError*/)
{
    // Issue a new request
    IssueRequest();
    
    return KErrNone;
}

/*!
    Observes if client is still running and close embedded view when client closed
*/
void XQServiceUtils::closeWhenClientClosed()
{
    XQSERVICE_DEBUG_PRINT("XQServiceUtil::closeEVWhenClientClosed");
    
    TRAPD(err, CParentObserver::closeWhenParentClosedL());
    
    XQSERVICE_DEBUG_PRINT("XQServiceUtil::closeEVWhenClientClosed return value: ", err);
}

XQRequestUtil::XQRequestUtil()
{
    XQSERVICE_DEBUG_PRINT("XQRequestUtil::XQRequestUtil");
}

XQRequestUtil::~XQRequestUtil()
{
    XQSERVICE_DEBUG_PRINT("XQRequestUtil::~XQRequestUtil");
}

void XQRequestUtil::setSynchronous(bool on)
{
    XQSERVICE_DEBUG_PRINT("XQRequestInfo::setSynchronous %d", on);
    mInfo.setInfo(XQServiceUtils::OptSynchronous, on);
}


const XQSharableFile *XQRequestUtil::getSharableFile(int index) const
{
    if (index >= 0 && index < mSharableFileArgs.size())
    {
        return &mSharableFileArgs.at(index);
    }

    return 0;
}

void XQRequestUtil::setSecurityInfo(const quint32 sid, const quint32 vid, const quint32 caps )
{
    XQSERVICE_DEBUG_PRINT("XQRequestUtil::setSecurityInfo");

    mInfo.setInfo(XQServiceUtils::InfoSID, sid);
    mInfo.setInfo(XQServiceUtils::InfoVID, vid);
    mInfo.setInfo(XQServiceUtils::InfoCap, caps);

}


int XQRequestUtil::mapError(int error)
{
    XQSERVICE_DEBUG_PRINT("XQRequestUtil::mapError");
    XQSERVICE_DEBUG_PRINT("\terror=%d", error);
    
    int mappedError(XQService::ENoError);

    switch (error)
    {
        case KErrNone:
        {
            mappedError = XQService::ENoError;
            break;
        }
        
        case KErrPermissionDenied:
        case KErrServerTerminated:
        {
            mappedError = XQService::EConnectionClosed;
            break;
        }
        case KErrServerBusy:
        {
            mappedError = XQService::EConnectionError;
            break;
        }
        case KErrArgument:
        {
            mappedError = XQService::EArgumentError;
            break;
        }

        case KErrNoMemory:
        {
            mappedError = XQService::EIPCError;
            break;
        }
        case KErrNotFound:
        {
            mappedError = XQService::EServerNotFound;
            break;
        }
        

        default:
        {
            mappedError = error;  // Leave it as is
            break;
        }
    }


    XQSERVICE_DEBUG_PRINT("\tmapper error=%d", mappedError);
    return mappedError;

}

// connectionName=[requesthandle:]channel
QString XQRequestUtil::channelName(const QString &connectionName)
{
    XQSERVICE_DEBUG_PRINT("XQRequestUtil::channelName %s", qPrintable(connectionName));
    QString ret = connectionName;
    QStringList l = ret.split(":");
    if (l.count() > 1)
    {
        ret = l.at(1);
    }

    XQSERVICE_DEBUG_PRINT("XQRequestUtil::channelName ret=%s", qPrintable(ret));
    return ret;
    
}
   
