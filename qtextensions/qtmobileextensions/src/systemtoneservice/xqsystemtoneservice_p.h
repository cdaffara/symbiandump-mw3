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

#ifndef XQSYSTEMTONESERVICE_P_H
#define XQSYSTEMTONESERVICE_P_H

#include <xqsystemtoneservice.h>

class XQSystemToneServicePrivate : public MStsPlayAlarmObserver
{

public:
  XQSystemToneServicePrivate(XQSystemToneService *qptr);

  ~XQSystemToneServicePrivate();

  void playTone(XQSystemToneService::ToneType toneType);

  unsigned int playAlarm(XQSystemToneService::AlarmType alarmType);

  void stopAlarm(unsigned int contextId);

  void PlayAlarmComplete(unsigned int aAlarmContext);

  unsigned int playAlarm(XQSystemToneService::ToneType toneType);
  
private:

  CSystemToneService::TToneType mapToneType(XQSystemToneService::ToneType toneType);

  CSystemToneService::TAlarmType mapAlarmType(XQSystemToneService::AlarmType toneType);

private:
  
  XQSystemToneService *q_ptr;
  
  CSystemToneService *sts;
  
};

#endif /*XQSYSTEMTONESERVICE_P_H*/
