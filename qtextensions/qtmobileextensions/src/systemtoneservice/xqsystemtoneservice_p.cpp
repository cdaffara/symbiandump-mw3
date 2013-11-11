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

#include <xqsystemtoneservice_p.h>

XQSystemToneServicePrivate::XQSystemToneServicePrivate(XQSystemToneService *qptr)
:
q_ptr(qptr),
sts(CSystemToneService::Create())
{
}

XQSystemToneServicePrivate::~XQSystemToneServicePrivate()
{
    if (sts) {
        CSystemToneService::Delete(sts);
    }
}

void XQSystemToneServicePrivate::playTone(XQSystemToneService::ToneType toneType)
{
    if (sts) {
        sts->PlayTone(mapToneType(toneType));
        emit q_ptr->toneStarted();
    }
}

unsigned int XQSystemToneServicePrivate::playAlarm(XQSystemToneService::AlarmType alarmType)
{
    unsigned int ctx = 0;
    if (sts) {
        sts->PlayAlarm(mapAlarmType(alarmType), ctx, *this);
        emit q_ptr->alarmStarted(ctx);
    }
    return ctx;
}

void XQSystemToneServicePrivate::stopAlarm(unsigned int contextId)
{
    if (sts) {
        sts->StopAlarm(contextId);
    }
}

unsigned int XQSystemToneServicePrivate::playAlarm(XQSystemToneService::ToneType toneType)
{
    unsigned int ctx = 0;
    if (sts) {
        sts->PlayAlarm(mapToneType(toneType), ctx, *this);
        emit q_ptr->alarmStarted(ctx);
    }
    return ctx;
}

CSystemToneService::TToneType XQSystemToneServicePrivate::mapToneType(XQSystemToneService::ToneType toneType)
{
    return static_cast<CSystemToneService::TToneType>(toneType);
}

CSystemToneService::TAlarmType XQSystemToneServicePrivate::mapAlarmType(XQSystemToneService::AlarmType alarmType)
{
    return static_cast<CSystemToneService::TAlarmType>(alarmType);
}

void XQSystemToneServicePrivate::PlayAlarmComplete(unsigned int contextId)
{
    emit q_ptr->alarmFinished(contextId);
}
