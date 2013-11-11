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
*       QT wrapper API for  for System Tone Service 
*
*/

#include "xqsystemtoneservice.h"
#include "xqsystemtoneservice_p.h"

STS_EXPORT XQSystemToneService::XQSystemToneService(QObject *parent) :
    QObject(parent)
{
    d_p = new XQSystemToneServicePrivate(this);
}

STS_EXPORT XQSystemToneService::~XQSystemToneService()
{
    delete d_p;
}

STS_EXPORT void XQSystemToneService::playTone(ToneType toneType)
{
    d_p->playTone(toneType);
    emit toneStarted();
}

STS_EXPORT void XQSystemToneService::playAlarm(AlarmType alarmType, unsigned int& context)
{
    context = d_p->playAlarm(alarmType);
}

STS_EXPORT void XQSystemToneService::stopAlarm(unsigned int context)
{
    d_p->stopAlarm(context);
}

STS_EXPORT void XQSystemToneService::playAlarm(ToneType toneType, unsigned int& context)
{
    context = d_p->playAlarm(toneType);
}

