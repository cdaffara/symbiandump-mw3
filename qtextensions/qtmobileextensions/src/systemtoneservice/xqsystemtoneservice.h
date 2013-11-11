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

#ifndef XQSYSTEMTONESERVICE_H
#define XQSYSTEMTONESERVICE_H

#include <QObject>

#include <systemtoneservice.h>

#ifdef __STS_QT_LIB_BUILD__ 
#  define STS_EXPORT Q_DECL_EXPORT
#else
#  define STS_EXPORT Q_DECL_IMPORT
#endif

class XQSystemToneServicePrivate;

/**
 * The System Tone Service (STS) is a component of the Multimedia Services (mmserv) 
 * collection of the Multimedia Middleware (mmmw) package. STS provides multimedia APIs 
 * specific to playing system tones (calendar, message alerts, warnings, etc).
 * 
 * Here you can find Qt wrapper for this component.
 * 
 * Main concepts behind this API:
 *   * tone - short sound used in the manner "fire and forget" therefore there are no signals when tone ends.
 *   * alarm - these are kind of looped/longer sounds that may be interrupted by calling app; also there are notifications (signals) when alarm ends playing.
 *    
 */
class XQSystemToneService : public QObject
{
  Q_OBJECT

public:

/**
 * Supported tone types.
*/
  Q_DECL_IMPORT enum ToneType {
	   // Capture Tones
	   BurstModeTone = CSystemToneService::EBurstMode,
	   CaptureTone = CSystemToneService::ECapture,
	   CallRecordingTone = CSystemToneService::ECallRecording,
	   RecordingStartTone = CSystemToneService::ERecordingStart,
	   RecordingStopTone = CSystemToneService::ERecordingStop,
	   SelfTimerTone = CSystemToneService::ESelfTimer,
	   
	   // General Tones
	   ConfirmationBeepTone = CSystemToneService::EConfirmationBeep,
	   DefaultBeepTone = CSystemToneService::EDefaultBeep,
	   ErrorBeepTone = CSystemToneService::EErrorBeep,
	   InformationBeepTone = CSystemToneService::EInformationBeep,
	   WarningBeepTone = CSystemToneService::EWarningBeep,
	   IntegratedHandsFreeActivatedTone = CSystemToneService::EIntegratedHandsFreeActivated,
	   
	   // Key Tones
	   TouchScreenTone = CSystemToneService::ETouchScreen,
	   
	   // Location Tones
	   LocationRequestTone = CSystemToneService::ELocationRequest,
	   
	   // Messaging Tones
	   ChatAlertTone = CSystemToneService::EChatAlert,
	   EmailAlertTone = CSystemToneService::EEmailAlert,
	   MmsAlertTone = CSystemToneService::EMmsAlert,
	   SmsAlertTone = CSystemToneService::ESmsAlert,
	   DeliveryReportTone = CSystemToneService::EDeliveryReport,
	   MessageSendFailureTone = CSystemToneService::EMessageSendFailure,
	   
	   // Power Tones
	   BatteryLowTone = CSystemToneService::EBatteryLow,
	   BatteryRechargedTone = CSystemToneService::EBatteryRecharged,
	   PowerOnTone = CSystemToneService::EPowerOn,
	   PowerOffTone = CSystemToneService::EPowerOff,
	   WakeUpTone = CSystemToneService::EWakeUp,
	   WrongChargerTone = CSystemToneService::EWrongCharger,
	   
	   // Telephony Tones
	   AutomaticRedialCompleteTone = CSystemToneService::EAutomaticRedialComplete,
	   
	   // Voice Recognition Tones
	   VoiceStartTone = CSystemToneService::EVoiceStart,
	   VoiceErrorTone = CSystemToneService::EVoiceError,
	   VoiceAbortTone = CSystemToneService::EVoiceAbort
  };
  
Q_DECL_IMPORT enum AlarmType {
       // Calendar Tones
       CalendarAlarmTone = CSystemToneService::ECalendarAlarm,
       ClockAlarmTone = CSystemToneService::EClockAlarm,
       ToDoAlarmTone = CSystemToneService::EToDoAlarm,

       // Telephony Alarms
       IncomingCallTone = CSystemToneService::EIncomingCall,
       IncomingCallLine2Tone = CSystemToneService::EIncomingCallLine2,
       IncomingDataCallTone = CSystemToneService::EIncomingDataCall,
  };

  
public:
/**
* XQSystemToneService object constructor. 
*/
  STS_EXPORT XQSystemToneService(QObject *parent = 0);

/**
* Simple destructor.
*/
  STS_EXPORT ~XQSystemToneService();

/**
* Play selected tone.
*
* @param  toneType  selected tone type.
* @see  XQSystemToneService::error()
*/
  STS_EXPORT void playTone(ToneType toneType);

/**
* Play selected alarm. 
*
* @param  alarmType  selected tone type.
* @param  contextId  unique playback id for current playback event. May be used for matching playback signals and/or to later stop playing.
* @see  XQSystemToneService::stopAlarm(unsigned int contextId)
* @see  XQSystemToneService::error()
*/
  STS_EXPORT void playAlarm(AlarmType alarmType, unsigned int& contextId);
  
  
/**
* Cancel tone being played currently.
*
* @param contextId id, must be same as given by playTone method.
* @see  XQSystemToneService::playAlarm(ToneType toneType, unsigned int& contextId)
*/
  STS_EXPORT void stopAlarm(unsigned int contextId);

/**
* Play selected tone. 
*
* @param  toneType  selected tone type.
* @param  contextId  unique playback id for current playback event. May be used for matching playback signals and/or to later stop playing.
* @see  XQSystemToneService::stopAlarm(unsigned int contextId)
* @see  XQSystemToneService::error()
*/
  STS_EXPORT void playAlarm(ToneType toneType, unsigned int& contextId);
  
signals:

/**
* Playback of given tone has been started.
* 
*/
  STS_EXPORT void toneStarted();

/**
* Playback of given alarm has been started.
*
* @param contextId id, the same as given by playTone method.
* 
*/
  STS_EXPORT void alarmStarted(unsigned int contextId);
  
/**
* Playback of given alarm has been finished.
*
* @param contextId id, the same as given by playTone method.
* 
*/
  STS_EXPORT void alarmFinished(unsigned int contextId);

private:

  XQSystemToneServicePrivate *d_p;

  friend class XQSystemToneServicePrivate;
  
};

#endif /*SYSTEMTONESERVICE_QT_H*/
