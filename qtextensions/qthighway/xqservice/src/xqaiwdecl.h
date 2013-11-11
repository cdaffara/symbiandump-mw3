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
* Description: Common application interworking declarations 
*              All declations shall have XQ prefix
*
*/

#ifndef XQAIWDECL_H
#define XQAIWDECL_H

#include <QLatin1String>

//
// All declations shall have XQ prefix
//


//
// Common Application Interworking declarations
// ---------------------------------------------
//

/*!
    \file xqaiwdecl.h
*/

/*!
    \def XQI_FILE_VIEW
    File viewing interface.
    \sa XQOP_FILE_VIEW, XQOP_FILE_VIEW_SHARABLE for related operations.
*/
/*!
    \def XQOP_FILE_VIEW
    File viewing interface related operation.
    \sa XQI_FILE_VIEW for interface definition.
*/
/*!
    \def XQOP_FILE_VIEW_SHARABLE
    File viewing interface and related operation.
    \sa XQI_FILE_VIEW interface definition.
*/

#define XQI_FILE_VIEW QLatin1String("com.nokia.symbian.IFileView")
#define XQOP_FILE_VIEW QLatin1String("view(QString)")
#define XQOP_FILE_VIEW_SHARABLE QLatin1String("view(XQSharableFile)")

/*!
    \def XQI_URI_VIEW
    URI viewing interface.
    \sa XQOP_URI_VIEW for related operation.
*/
/*!
    \def XQOP_URI_VIEW
    URI viewing interface related operations.
    \sa XQI_URI_VIEW for interface definition.
*/
#define XQI_URI_VIEW QLatin1String("com.nokia.symbian.IUriView")
#define XQOP_URI_VIEW QLatin1String("view(QString)")

/*!
    \def XQI_FILE_SHARE
    File sharing interface.
    \sa XQOP_FILE_SHARE for related operations.
*/
/*!
    \def XQOP_FILE_SHARE
    File sharing interface related operation.
    \sa XQI_FILE_SHARE for interface definition.
*/
#define XQI_FILE_SHARE QLatin1String("com.nokia.symbian.IFileShare")
#define XQOP_FILE_SHARE QLatin1String("send(QVariant)")

/*!
    \def XQURI_SCHEME_ACTIVITY
    The scheme of the activity URI.
*/
#define XQURI_SCHEME_ACTIVITY QLatin1String("appto")
/*!
    \def XQURI_KEY_ACTIVITY_NAME
    Activity key name in query part of the activity URI.
*/
#define XQURI_KEY_ACTIVITY_NAME QLatin1String("activityname")

/*!
    \def XQURI_SCHEME_HTTP
    Miscellaneous needed URI scheme.
    \sa XQURI_SCHEME_HTTPS, XQURI_SCHEME_MAILTO, XQURI_SCHEME_FILE for other URI schemes.
*/
/*!
    \def XQURI_SCHEME_HTTPS
    Miscellaneous needed URI scheme.
    \sa XQURI_SCHEME_HTTP, XQURI_SCHEME_MAILTO, XQURI_SCHEME_FILE for other URI schemes.
*/
/*!
    \def XQURI_SCHEME_MAILTO
    Miscellaneous needed URI scheme.
    \sa XQURI_SCHEME_HTTP, XQURI_SCHEME_HTTPS, XQURI_SCHEME_FILE for other URI schemes.
*/
/*!
    \def XQURI_SCHEME_FILE
    Miscellaneous needed URI scheme.
    \sa XQURI_SCHEME_HTTP, XQURI_SCHEME_HTTPS, XQURI_SCHEME_MAILTO for other URI schemes.
*/
#define XQURI_SCHEME_HTTP QLatin1String("http")
#define XQURI_SCHEME_HTTPS QLatin1String("https")
#define XQURI_SCHEME_MAILTO QLatin1String("mailto")
#define XQURI_SCHEME_FILE QLatin1String("file")


/*!
    \def XQCUSTOM_PROP_AIW_TEXT_FILE
    Service XML custom property, which contains the name of text resource file that
    contains the CUSTOM_PROP_AIW_TEXT.
    \sa XQAiwRequest::createAction()
*/
#define XQCUSTOM_PROP_AIW_TEXT_FILE QLatin1String("aiw_action_text_file")

/*!
    \def XQCUSTOM_PROP_AIW_TEXT
    Service XML custom property, that contains the Text Id within the
    CUSTOM_PROP_AIW_TEXT_FILE.  The localized text is attached to
    QAction returned by the XQAiwRequest::createAction().
*/
#define XQCUSTOM_PROP_AIW_TEXT QLatin1String("aiw_action_text")

/*!
    \def XQCUSTOM_PROP_AIW_ICON
    Service XML custom property, that contains the name of the icon to be attached
    to QAction returned by the XQAiwRequest::createAction().
*/
#define XQCUSTOM_PROP_AIW_ICON QLatin1String("aiw_action_icon")

/*!
    \def XQCUSTOM_PROP_SCHEMES
    Service XML custom property, that contains schemes the XQI_URI_VIEW can handle.
*/
#define XQCUSTOM_PROP_SCHEMES QLatin1String("schemes")

/*!
    \def XQCUSTOM_PROP_AIW_SERVICE_STATUS
    Service XML custom property, which contains the name of text key that
    contains the service status.
    \sa XQApplicationManager::status()
*/
#define XQCUSTOM_PROP_AIW_SERVICE_STATUS QLatin1String("aiw_status")

/*!
    \def XQINFO_KEY_WINDOW_TITLE
    A key name for XQRequestInfo object to pass a window title
    to be shown in service application instead of the default title.
    Service application is responsible to handle the key value.
    \sa XQRequestInfo::setInfo()
*/
#define XQINFO_KEY_WINDOW_TITLE QLatin1String("WindowTitle")


//
// Media related Application Interworking declarations
// ---------------------------------------------------
//

/*!
    \def XQI_IMAGE_FETCH
    Image fetching interface.
    \sa XQOP_IMAGE_FETCH for related operations.
*/
/*!
    \def XQOP_IMAGE_FETCH
    Image fetching interface related operation.
    \sa XQI_IMAGE_FETCH interface definition.
*/
#define XQI_IMAGE_FETCH QLatin1String("com.nokia.symbian.IImageFetch")
#define XQOP_IMAGE_FETCH QLatin1String("fetch()")

/*!
    \def XQI_MUSIC_FETCH
    Music fetching interface.
    \sa XQOP_MUSIC_FETCH for related operations.
*/
/*!
    \def XQOP_MUSIC_FETCH
    Music fetching  interface related operation.
    \sa XQI_MUSIC_FETCH for interface definition.
*/
#define XQI_MUSIC_FETCH QLatin1String("com.nokia.symbian.IMusicFetch")
#define XQOP_MUSIC_FETCH QLatin1String("fetch()")

/*!
    \def XQI_TONE_FETCH
    Tones fetching interface.
    \sa XQOP_TONE_FETCH for related operations.
*/
/*!
    \def XQOP_TONE_FETCH
    Tones fetching interface related operation.
    \sa XQI_TONE_FETCH for interface definition.
*/
#define XQI_TONE_FETCH QLatin1String("com.nokia.symbian.IToneFetch")
#define XQOP_TONE_FETCH QLatin1String("fetch()")

/*!
    \def XQI_VIDEO_FETCH
    Video fetching interface.
    \sa XQOP_VIDEO_FETCH for related operations.
*/
/*!
    \def XQOP_VIDEO_FETCH
    Video fetching interface related operation.
    \sa XQI_VIDEO_FETCH for interface definition.
*/
#define XQI_VIDEO_FETCH QLatin1String("com.nokia.symbian.IVideoFetch")
#define XQOP_VIDEO_FETCH QLatin1String("fetch()")

/*!
    \enum XQService::VideoBrowseSortKey
    Video collection browser sort parameter.
    \sa XQOP_VIDEO_BROWSE for operation definition.
*/
/*!
    \var XQService::VideoBrowseSortKey  XQService::SortUndefined
    Sort type undefined.
*/
/*!
    \var XQService::VideoBrowseSortKey  XQService::SortDateTime
    Sort by date/time.
*/
/*!
    \var XQService::VideoBrowseSortKey  XQService::SortTitle
    Sort by title.
*/
/*!
    \var XQService::VideoBrowseSortKey  XQService::SortSize
    Sort by size.
*/
namespace XQService
{
    enum VideoBrowseSortKey
    {
        SortUndefined = 0,
        SortDateTime  = 1,
        SortTitle     = 2,
        SortSize      = 3
    };
}

/*!
    \def XQI_VIDEO_BROWSE
    Video collection browser interface.
    \sa XQOP_VIDEO_BROWSE for related operations.
*/
/*!
    \def XQOP_VIDEO_BROWSE
    Video collection browser interface related operation.
    \sa XQI_VIDEO_BROWSE for interface definition.
*/
#define XQI_VIDEO_BROWSE QLatin1String("com.nokia.symbian.IVideoBrowse")
#define XQOP_VIDEO_BROWSE QLatin1String("browseVideos(int, int)")

/*!
    \def XQI_VIDEO_PLAY
    Video player interface.
    \sa XQOP_VIDEO_PLAY, XQOP_VIDEO_PLAY_PD, XQOP_VIDEO_CLOSE
        for related operations.
*/
/*!
    \def XQOP_VIDEO_PLAY
    Video player interface related operation.
    \sa XQI_VIDEO_PLAY for interface definition.
*/
/*!
    \def XQOP_VIDEO_PLAY_PD
    Video player interface related operation.
    \sa XQI_VIDEO_PLAY for interface definition.
*/
/*!
    \def XQOP_VIDEO_CLOSE
    Video player interface related operation.
    \sa XQI_VIDEO_PLAY for interface definition.
*/
#define XQI_VIDEO_PLAY QLatin1String("com.nokia.symbian.IVideoView")
#define XQOP_VIDEO_PLAY QLatin1String("playMedia(QString)")
#define XQOP_VIDEO_PLAY_PD QLatin1String("playMedia(QString, int)")
#define XQOP_VIDEO_CLOSE QLatin1String("closePlayer()")

/*!
    \def XQI_CAMERA_CAPTURE
    Camera capture interface.
    \sa XQOP_CAMERA_CAPTURE for related operations.
*/
/*!
    \def XQOP_CAMERA_CAPTURE
    Camera capture interface related operation.
    \sa XQI_CAMERA_CAPTURE for interface definition, \n
        XQCAMERA_INDEX, XQCAMERA_QUALITY, XQCAMERA_MODE_SWITCH,
        XQCAMERA_INDEX_SWITCH, XQCAMERA_QUALITY_CHANGE for operation
        parameters
*/
/*!
    \def XQCAMERA_INDEX
    XQOP_CAMERA_CAPTURE operation parameter.\n \b Values (int):
    - 0 = primary
    - 1 = secondary
    \sa XQOP_CAMERA_CAPTURE for operation definition.
*/
/*!
    \def XQCAMERA_QUALITY
    XQOP_CAMERA_CAPTURE operation parameter.\n \b Values (int):
    - 0 = default
    - 1 = lowest
    - 2 = highest
    \sa XQOP_CAMERA_CAPTURE for operation definition.
*/
/*!
    \def XQCAMERA_MODE_SWITCH
    XQOP_CAMERA_CAPTURE operation parameter.\n \b Values (bool)
    \sa XQOP_CAMERA_CAPTURE for operation definition.
*/
/*!
    \def XQCAMERA_INDEX_SWITCH
    XQOP_CAMERA_CAPTURE operation parameter.\n \b Values (bool)
    \sa XQOP_CAMERA_CAPTURE for operation definition.
*/
/*!
    \def XQCAMERA_QUALITY_CHANGE
    XQOP_CAMERA_CAPTURE operation parameter.\n \b Values (bool)
    \sa XQOP_CAMERA_CAPTURE for operation definition.
*/
#define XQI_CAMERA_CAPTURE QLatin1String("com.nokia.symbian.ICameraCapture")
#define XQOP_CAMERA_CAPTURE QLatin1String("capture(int,QVariantMap)")
#define XQCAMERA_INDEX QLatin1String("CameraIndex")
#define XQCAMERA_QUALITY QLatin1String("Quality")
#define XQCAMERA_MODE_SWITCH QLatin1String("AllowModeSwitch")
#define XQCAMERA_INDEX_SWITCH QLatin1String("AllowCameraSwitch")
#define XQCAMERA_QUALITY_CHANGE QLatin1String("AllowQualityChange")


//
// Logs related Application Interworking declarations
// ---------------------------------------------------
//

/*!
    \enum XQService::LogsViewIndex
    XQLOGS_VIEW_INDEX parameter values.
    \sa XQOP_LOGS_SHOW for operation definition.
*/
/*!
    \var XQService::LogsViewIndex  XQService::LogsViewAll
    All recent.
*/
/*!
    \var XQService::LogsViewIndex  XQService::LogsViewReceived
    Received.
*/
/*!
    \var XQService::LogsViewIndex  XQService::LogsViewCalled
    Called.
*/
/*!
    \var XQService::LogsViewIndex  XQService::LogsViewMissed
    Missed.
*/
namespace XQService
{  
    enum LogsViewIndex
    {
        LogsViewAll = 0,
        LogsViewReceived,
        LogsViewCalled,
        LogsViewMissed
    };
}

/*!
    \def XQI_LOGS_VIEW
    Logs view interface.
    \sa XQOP_LOGS_SHOW for related operations.
*/
/*!
    \def XQOP_LOGS_SHOW
    Logs view interface related operation.
    \sa XQI_LOGS_VIEW for interface definition, \n
        XQLOGS_VIEW_INDEX, XQLOGS_SHOW_DIALPAD, XQLOGS_DIALPAD_TEXT
        for operation parameters.
*/
/*!
    \def XQLOGS_VIEW_INDEX
    XQOP_LOGS_SHOW operation parameter. \n
    \b Values (XQService::LogsViewIndex)
    \sa XQOP_LOGS_SHOW for operation definition.
*/
/*!
    \def XQLOGS_SHOW_DIALPAD
    XQOP_LOGS_SHOW operation parameter. \n
    \b Values (bool)
    \sa XQOP_LOGS_SHOW for operation definition.
*/
/*!
    \def XQLOGS_DIALPAD_TEXT
    XQOP_LOGS_SHOW operation parameter. \n
    \b Values (QString)
    \sa XQOP_LOGS_SHOW for operation definition.
*/
#define XQI_LOGS_VIEW QLatin1String("com.nokia.symbian.ILogsView") 
#define XQOP_LOGS_SHOW QLatin1String("show(QVariantMap)")
#define XQLOGS_VIEW_INDEX QLatin1String("ViewIndex")
#define XQLOGS_SHOW_DIALPAD QLatin1String("ShowDialpad")
#define XQLOGS_DIALPAD_TEXT QLatin1String("DialpadText")

//
// Contacts related Application Interworking declarations
// ------------------------------------------------------
//

/*!
    \def XQI_CONTACTS_EDIT
    Contacts related Application Interworking declaration.
    \sa XQOP_CONTACTS_EDIT_CREATE_NEW, XQOP_CONTACTS_EDIT_CREATE_NEW_WITH_SUBTYPE, XQOP_CONTACTS_EDIT_CREATE_NEW_VCARD,
        XQOP_CONTACTS_EDIT_EXISTING, XQOP_CONTACTS_EDIT_UPDATE_EXISTING, XQOP_CONTACTS_EDIT_UPDATE_EXISTING_WITH_SUBTYPE for related operations.
*/
/*!
    \def XQOP_CONTACTS_EDIT_CREATE_NEW
    Launches editor for creating a new contact with a given detail.
    \param Parameter1 (QString) Indicates what type of contact field is supplied.
        One of the following values (you need to include qcontactdetails.h):
        - QContactPhoneNumber::DefinitionName
        - QContactEmailAddress::DefinitionName
        - QContactOnlineAccount::DefinitionName
    \param Parameter2 (QString) The actual detail value, for eg. "123456" 
    \return One of these constants defined in cntservicescontact.h:
        - KCntServicesReturnValueContactSaved
        - KCntServicesReturnValueContactDeleted
        - KCntServicesReturnValueContactNotModified
    \sa XQI_CONTACTS_EDIT for interface definition
*/
/*!
    \def XQOP_CONTACTS_EDIT_CREATE_NEW_WITH_SUBTYPE
    Launches editor for creating a new contact with a given detail.
    \param Parameter1 (QString) Indicates what type of contact field is supplied.
        One of the following values (you need to include qcontactdetails.h):
        - QContactPhoneNumber::DefinitionName
        - QContactEmailAddress::DefinitionName
        - QContactOnlineAccount::DefinitionName
    \param Parameter2 (QString) The actual detail value, for eg. "123456"
    \param Parameter3 (QString) sub type, for eg. QContactPhoneNumber::SubTypeMobile
    \return One of these constants defined in cntservicescontact.h:
        - KCntServicesReturnValueContactSaved
        - KCntServicesReturnValueContactDeleted
        - KCntServicesReturnValueContactNotModified
    \sa XQI_CONTACTS_EDIT for interface definition
*/
/*!
    \def XQOP_CONTACTS_EDIT_CREATE_NEW_VCARD
    Launches editor for creating a new contact based on vCard  indicated in arg.
    The user is able to edit and save the contact. 
    \param Parameter1 (QString) vCard file name including the full path.
    \return One of these constants defined in cntservicescontact.h:
        - KCntServicesReturnValueContactSaved
        - KCntServicesReturnValueContactDeleted
        - KCntServicesReturnValueContactNotModified
    \sa XQI_CONTACTS_EDIT for interface definition
*/
/*!
    \def XQOP_CONTACTS_EDIT_EXISTING
    Launch editor for existing contact that is specified with contact ID. 
    Only the default backend storage is supported.
    \param Parameter1 (int) Contact ID.
    \return One of these constants defined in cntservicescontact.h:
        - KCntServicesReturnValueContactSaved
        - KCntServicesReturnValueContactDeleted
        - KCntServicesReturnValueContactNotModified
    \sa XQI_CONTACTS_EDIT for interface definition
*/
/*!
    \def XQOP_CONTACTS_EDIT_UPDATE_EXISTING
    Launch editor for updating an existing contact with a given detail.
    A contact fetch view is opened first, where the user can choose which contact
    to update. 
    \param Parameter1 (QString) Indicates what type of contact field is supplied.
        One of the following values (you need to include qcontactdetails.h):
        - QContactPhoneNumber::DefinitionName
        - QContactEmailAddress::DefinitionName
        - QContactOnlineAccount::DefinitionName
    \param Parameter2 (QString) The actual detail value, for eg. "123456"
    \return One of these constants defined in cntservicescontact.h:
        - KCntServicesReturnValueContactSaved
        - KCntServicesReturnValueContactDeleted
        - KCntServicesReturnValueContactNotModified
    \sa XQI_CONTACTS_EDIT for interface definition
*/
/*!
    \def XQOP_CONTACTS_EDIT_UPDATE_EXISTING_WITH_SUBTYPE
    Launch editor for updating an existing contact with a given detail.
    Becomes available in wk30.
    A contact fetch view is opened first, where the user can choose which contact
    to update. 
    \param Parameter1 (QString) Indicates what type of contact field is supplied.
        One of the following values (you need to include qcontactdetails.h):
        - QContactPhoneNumber::DefinitionName
        - QContactEmailAddress::DefinitionName
        - QContactOnlineAccount::DefinitionName
    \param Parameter2 (QString) The actual detail value, for eg. "123456"
    \param Parameter3 (QString) Sub type
    \return One of these constants defined in cntservicescontact.h:
        - KCntServicesReturnValueContactSaved
        - KCntServicesReturnValueContactDeleted
        - KCntServicesReturnValueContactNotModified
    \sa XQI_CONTACTS_EDIT for interface definition
 */
#define XQI_CONTACTS_EDIT QLatin1String("com.nokia.symbian.IContactsEdit")
#define XQOP_CONTACTS_EDIT_CREATE_NEW QLatin1String("editCreateNew(QString,QString)")
#define XQOP_CONTACTS_EDIT_CREATE_NEW_WITH_SUBTYPE QLatin1String("editCreateNew(QString,QString,QString)")
#define XQOP_CONTACTS_EDIT_CREATE_NEW_VCARD QLatin1String("editCreateNewFromVCard(QString)")
#define XQOP_CONTACTS_EDIT_EXISTING QLatin1String("editExisting(int)")
#define XQOP_CONTACTS_EDIT_UPDATE_EXISTING QLatin1String("editUpdateExisting(QString,QString)")
#define XQOP_CONTACTS_EDIT_UPDATE_EXISTING_WITH_SUBTYPE QLatin1String("editUpdateExisting(QString,QString,QString)")

/*!
    \def XQI_CONTACTS_FETCH
    Contacts related Application Interworking declaration.
    \sa XQOP_CONTACTS_FETCH_MULTIPLE, XQOP_CONTACTS_FETCH_SINGLE for related operations.
*/
/*!
    \def XQOP_CONTACTS_FETCH_MULTIPLE
    Fetch multiple contacts. cntservicescontact.h has some constant definitions
    and a class for transforming the result.
    \param Parameter1 (QString) Title string.
    \param Parameter2 (QString) Action type string, from cntservicescontact.h 
    \return An object of type CntServicesContactList containing the contact ID.
    \sa XQI_CONTACTS_FETCH for interface definition.
*/
 /*!
    \def XQOP_CONTACTS_FETCH_SINGLE
    Fetch single contact. cntservicescontact.h has some constant definitions
    and a class for transforming the result.
    \param Parameter1 (QString) Title string.
    \param Parameter2 (QString) Action type string, from cntservicescontact.h 
    \return An object of type CntServicesContactList containing the contact ID.
    \sa XQI_CONTACTS_FETCH for interface definition.
*/
#define XQI_CONTACTS_FETCH QLatin1String("com.nokia.symbian.IContactsFetch")
#define XQOP_CONTACTS_FETCH_MULTIPLE QLatin1String("multiFetch(QString,QString)")
#define XQOP_CONTACTS_FETCH_SINGLE QLatin1String("singleFetch(QString,QString)")


/*!
    \def XQI_CONTACTS_VIEW
    Contacts related Application Interworking declaration.
    \sa XQOP_CONTACTS_VIEW_CONTACT_CARD, XQOP_CONTACTS_VIEW_TEMP_CONTACT_CARD_WITH_DETAIL,
        XQOP_CONTACTS_VIEW_GROUP for related operations.
*/
/*!
    \def XQOP_CONTACTS_VIEW_CONTACT_CARD
    Opens contact card.
    \param Parameter1 (int) Contact ID.
    \sa XQI_CONTACTS_VIEW for interface definition.
*/
/*!
    \def XQOP_CONTACTS_VIEW_TEMP_CONTACT_CARD_WITH_DETAIL
    Open contact card for in-memory contact which is not saved yet.
    In the UI there is an option to save the contact either by updating an
    existing contact or by creating a new one.
    \param Parameter1 (QString) Indicates what type of contact field is supplied.
        One of the following values (you need to include qcontactdetails.h):
        - QContactPhoneNumber::DefinitionName
        - QContactEmailAddress::DefinitionName
        - QContactOnlineAccount::DefinitionName
    \param Parameter2 (QString) The actual detail value, for eg. "123456"
    \sa XQI_CONTACTS_VIEW for interface definition.
*/
/*!
    \def XQOP_CONTACTS_VIEW_GROUP
    Open the group members view.
    On the flip side of the group members view, there is the "group action" view.
    \param Parameter1 (int) The group ID (native ID).
           Only groups in the main Qt Mobility backend are supported.
    \sa XQI_CONTACTS_VIEW for interface definition.
*/
#define XQI_CONTACTS_VIEW QLatin1String("com.nokia.symbian.IContactsView")
#define XQOP_CONTACTS_VIEW_CONTACT_CARD QLatin1String("openContactCard(int)")
#define XQOP_CONTACTS_VIEW_TEMP_CONTACT_CARD_WITH_DETAIL QLatin1String("openTemporaryContactCard(QString,QString)")
#define XQOP_CONTACTS_VIEW_GROUP QLatin1String("openGroup(int)")

//
// Messaging related Application Interworking declarations
// ------------------------------------------------------
//

/*!
    \def XQI_MESSAGE_SEND
    Messaging related Application Interworking declaration.
    \sa XQOP_MESSAGE_SEND, XQOP_MESSAGE_SEND_WITH_ID, XQOP_MESSAGE_SEND_WITH_ALIAS for related operations.
*/
/*!
    \def XQOP_MESSAGE_SEND
    Messaging related Application Interworking interface related operation.
    \sa XQI_MESSAGE_SEND for interface definition.
*/
/*!
    \def XQOP_MESSAGE_SEND_WITH_ID
    Messaging related Application Interworking interface related operation.
    \sa XQI_MESSAGE_SEND for interface definition.
*/
/*!
    \def XQOP_MESSAGE_SEND_WITH_ALIAS
    Messaging related Application Interworking interface related operation.
    \sa XQI_MESSAGE_SEND for interface definition.
*/
#define XQI_MESSAGE_SEND QLatin1String("com.nokia.symbian.IMessageSend")
#define XQOP_MESSAGE_SEND QLatin1String("send(QVariantMap,QString)")
#define XQOP_MESSAGE_SEND_WITH_ID QLatin1String("send(QString,qint32,QString)")
#define XQOP_MESSAGE_SEND_WITH_ALIAS QLatin1String("send(QString,QString,QString)")

/*!
    \def XQI_MESSAGE_VIEW
    Messaging related Application Interworking declaration.
    \sa XQOP_MESSAGE_VIEW for related operations.
*/
/*!
    \def XQOP_MESSAGE_VIEW
    Messaging related Application Interworking interface related operation.
    \sa XQI_MESSAGE_VIEW for interface definition.
*/
#define XQI_MESSAGE_VIEW QLatin1String("com.nokia.symbian.IMessageView")
#define XQOP_MESSAGE_VIEW QLatin1String("view(int)")

/*!
 Incomplete list of interfaces.
 To be completed with operation signatures and related declarations.
*/
#define XQI_FILE_FETCH QLatin1String("com.nokia.symbian.IFileFetch")



#endif
