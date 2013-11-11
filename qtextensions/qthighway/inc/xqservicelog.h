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

#ifndef XQSERVICELOG_H
#define XQSERVICELOG_H

//#define XQSERVICE_DEBUG
//#define XQSERVICE_DEBUG_FILE

#include <qglobal.h>

#ifdef XQSERVICE_DEBUG
#include <e32std.h>
#include <e32debug.h>
#ifdef XQSERVICE_DEBUG_FILE
#include <f32file.h>
#include <flogger.h>
#endif //XQSERVICE_DEBUG_FILE
#endif //XQSERVICE_DEBUG

#ifdef XQSERVICE_DEBUG
#define XQSERVICE_DEBUG_PRINT(args...) qDebug(args);
#define XQSERVICE_WARNING_PRINT(args...) qWarning(args);
#define XQSERVICE_CRITICAL_PRINT(args...) qCritical(args);
#define XQSERVICE_FATAL_PRINT(args...) qFatal(args);
#define XQSERVICEMESSAGEHANDLER XqServiceMessageHandler::xqServiceMessageHandler
#define XQSERVICE_CONNECT(a, b, c, d) if (!QObject::connect((a), (b), (c), (d))) {qCritical("%s: connection failed", __PRETTY_FUNCTION__);}else{qDebug("%s: connection OK", __PRETTY_FUNCTION__);}
#else //XQSERVICE_DEBUG not defined
#define XQSERVICE_DEBUG_PRINT(args...)
#define XQSERVICE_WARNING_PRINT(args...)
#define XQSERVICE_CRITICAL_PRINT(args...)
#define XQSERVICE_FATAL_PRINT(args...)
#define XQSERVICE_CONNECT(a, b, c, d) QObject::connect((a), (b), (c), (d));
#define XQSERVICEMESSAGEHANDLER 0
#endif //XQSERVICE_DEBUG

#ifdef XQSERVICE_DEBUG

class XqServiceMessageHandler
    {
    
    public:
    
        static void xqServiceMessageHandler(QtMsgType /*type*/, const char *msg)
        {
        	_LIT(format, "[Qt Message] %S");
        	const TPtrC8 ptr(reinterpret_cast<const TUint8*>(msg));
#ifndef XQSERVICE_DEBUG_FILE
            // RDebug::Print has a cap of 256 characters so break it up
            const int maxBlockSize = 256 - ((const TDesC &)format).Length();
            HBufC* hbuffer = q_check_ptr(HBufC::New(qMin(maxBlockSize, ptr.Length())));
            for (int i = 0; i < ptr.Length(); i += hbuffer->Length()) {
                hbuffer->Des().Copy(ptr.Mid(i, qMin(maxBlockSize, ptr.Length()-i)));
                RDebug::Print(format, hbuffer);
            }
            delete hbuffer;
#else //XQSERVICE_DEBUG_FILE defined
            _LIT(KLogDir, "qt");
            _LIT(KLogFile, "xqservice.log");
            _LIT(KLogStarting, "*** Starting XQService application ***");
            static bool logStarted;
            if ( !logStarted ){
                RFileLogger::Write(KLogDir, KLogFile, EFileLoggingModeAppend, KLogStarting);        
                logStarted = true; 
            }
            RFileLogger::Write(KLogDir, KLogFile, EFileLoggingModeAppend, ptr);
#endif //XQSERVICE_DEBUG_FILE
        }
    
    };

#endif //XQSERVICE_DEBUG

#endif //XQSERVICELOG_H
