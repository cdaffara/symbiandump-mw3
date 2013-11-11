/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Globals from XQService
*
*/

#ifndef XQSERVICEGLOBAL_H
#define XQSERVICEGLOBAL_H

#include <QtCore/qglobal.h>
#include <QtCore/qplugin.h>
#include <xqserviceerrdefs.h>

#if defined(Q_OS_WIN)
#  if defined(QT_NODLL)
#    undef QT_MAKEDLL
#    undef QT_DLL
#  elif defined(QT_MAKEDLL)
#    if defined(QT_DLL)
#      undef QT_DLL
#    endif
#    if defined(QT_BUILD_SFW_LIB)
#      define Q_SFW_EXPORT Q_DECL_EXPORT
#    else
#      define Q_SFW_EXPORT Q_DECL_IMPORT
#    endif
#  elif defined(QT_DLL) /* use a Qt DLL library */
#    define Q_SFW_EXPORT Q_DECL_IMPORT
#  endif
#else
#endif

#if !defined(Q_SFW_EXPORT)
#  if defined(QT_SHARED)
#    define Q_SFW_EXPORT Q_DECL_EXPORT
#  else
#    define Q_SFW_EXPORT
#  endif
#endif

#if !defined(XQSERVICE_EXPORT)
#if defined(XQ_BUILD_XQSERVICE_LIB)
#define XQSERVICE_EXPORT Q_DECL_EXPORT
#else
#define XQSERVICE_EXPORT Q_DECL_IMPORT
#endif
#endif

#if !defined(XQSERVICEUTIL_EXPORT)
#if defined(XQ_BUILD_XQSERVICEUTIL_LIB)
#define XQSERVICEUTIL_EXPORT Q_DECL_EXPORT
#else
#define XQSERVICEUTIL_EXPORT Q_DECL_IMPORT
#endif
#endif

namespace XQService
{
	enum ServiceIPCErrors
	    {
	    ENoError=0, 
	    EConnectionError = QtService::IPC_ERR_START_VALUE ,  /*!< Error in IPC Connection */
	    EConnectionClosed,                      /*!< IPC Connection is closed             */
	    EServerNotFound,                        /*!< Can not find server                  */
	    EIPCError,                              /*!< Known IPC error                      */
	    EUnknownError,                          /*!< Unknown IPC error                    */
        ERequestPending,                        /*!< Already pending request              */
        EMessageNotFound,                       /*!< Message slot not found               */
        EArgumentError                          /*!< Argument error                       */
	    };

    enum ServiceMetadataErrors
    {
        EMetaNoService = QtService::METADATA_ERR_START_VALUE,    /*!< Can not find service root node in XML file                  */
        EMetaNoServiceName,                          /*!< Can not find service name in XML file                                   */
        EMetaNoServiceFilepath,                      /*!< Can not find service filepath in XML file                               */
        EMetaNoServiceInterface,                     /*!< No interface for the service in XML file                                */
        EMetaNoInterfaceVersion,                     /*!< Can not find interface version in XML file                              */
        EMetaNoInterfaceName,                        /*!< Can not find interface name in XML file                                 */
        EMetaUnableToOpenFile,                       /*!< Error opening XML file                                                  */
        EMetaInvalidXmlFile,                         /*!< Not a valid XML file                                                    */
        EMetaParseService,                           /*!< Error parsing service node                                              */
        EMetaParseInterface,                         /*!< Error parsing interface node                                            */
        EMetaDuplicatedInterface,                    /*!< The same interface is defined twice                                     */
        EMetaInvalidVersion,
        EMetaDuplicatedTag,                          /*!< The tag appears twice                                                   */
        EMetaInvalidCustomTag,                       /*!< The customproperty tag is not corectly formatted or otherwise incorrect */
        EMetaDuplicatedCustomKey                     /*!< The customproperty appears twice                                        */
    };

    enum ServiceManagerErrors
    {
        EMgrInternalError = QtService::RESOLVER_ERR_START_VALUE  /* Internal error */
    };
    
}            
            
#endif
