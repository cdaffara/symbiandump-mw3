/*
#
# Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
# All rights reserved.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, version 2.1 of the License.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not,
# see "http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html/".
#
# Description:
#
*/

#include <QDebug>
#include <QApplication>
#include <QKeyEvent>
#include <QLabel>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QStackedWidget>
#include <QImageReader>
#include <QDebug>
#include <QTimer>
#include <QPushButton>
#include <QLineEdit>
#include <QDebug>
#include <QString>
#include <QCheckBox>

#include <QListView>
#include <QMap>
#include <QAction>

#include "ststest.h"
#include <XQSystemToneService>

STSTest::STSTest( QWidget *parent) 
:
QMainWindow(parent),
sts(new XQSystemToneService())
{
    qDebug() << QString("[sts] STSTest::STSTest 1");
    QWidget *window = new QWidget(this);
    qDebug() << QString("[sts] STSTest::STSTest 2");
    QVBoxLayout* layout = new QVBoxLayout(this);

    callbackCleanTimer.setSingleShot(true);
    connect(&callbackCleanTimer, SIGNAL(timeout()), this, SLOT(cleanCallback()));
    
    QPushButton *quitButton = new QPushButton("QUIT", this);
    connect(quitButton, SIGNAL(clicked()), qApp, SLOT(quit()));

    QPushButton *playToneButton = new QPushButton("Play Tone", this);
    connect(playToneButton, SIGNAL(clicked()), this, SLOT(playTone()));

    QPushButton *playAlarmButton = new QPushButton("Play Alarm", this);
    connect(playAlarmButton, SIGNAL(clicked()), this, SLOT(playAlarm()));

    QPushButton *playAlarmAsToneButton = new QPushButton("Play Alarm as Tone", this);
    connect(playAlarmAsToneButton, SIGNAL(clicked()), this, SLOT(playAlarmAsTone()));

    QPushButton *stopAlarmButton = new QPushButton("Stop Alarm", this);
    connect(stopAlarmButton, SIGNAL(clicked()), this, SLOT(stopAlarm()));
    

    connect(sts, SIGNAL(toneStarted()), this, SLOT(startCallback()));
    connect(sts, SIGNAL(alarmStarted(unsigned int)), this, SLOT(startCallback(unsigned int)));
    connect(sts, SIGNAL(alarmFinished(unsigned int)), this, SLOT(stopCallback(unsigned int)));

    qDebug() << QString("[sts] STSTest::STSTest 3");

    box = new QComboBox(this);
    spinBox = new QSpinBox(this);
    
    layout->setMargin(10);
    layout->setSpacing(0);
    qDebug() << QString("[sts] STSTest::STSTest 4"); 
    QList<QPair<int, QString> > tonesList;
    

    // alarms
    tonesList.append(QPair<int,QString>(XQSystemToneService::CalendarAlarmTone, "[A] Calendar Alarm"));
    tonesList.append(QPair<int,QString>(XQSystemToneService::ClockAlarmTone, "[A] Clock Alarm"));
    tonesList.append(QPair<int,QString>(XQSystemToneService::ToDoAlarmTone, "[A] ToDo Alarm"));

    tonesList.append(QPair<int,QString>(XQSystemToneService::IncomingCallTone, "[A] Incoming Call"));
    tonesList.append(QPair<int,QString>(XQSystemToneService::IncomingCallLine2Tone, "[A] Incomming Call Line 2"));
    tonesList.append(QPair<int,QString>(XQSystemToneService::IncomingDataCallTone, "[A] Incomming Data Call"));

    // tones
    tonesList.append(QPair<int,QString>(XQSystemToneService::BurstModeTone, "[T] Burst Mode"));
    tonesList.append(QPair<int,QString>(XQSystemToneService::CaptureTone, "[T] Capture"));
    tonesList.append(QPair<int,QString>(XQSystemToneService::CallRecordingTone, "[T] Call Recording"));
    tonesList.append(QPair<int,QString>(XQSystemToneService::RecordingStartTone, "[T] Recording Start"));
    tonesList.append(QPair<int,QString>(XQSystemToneService::SelfTimerTone, "[T] Self Timer"));

    tonesList.append(QPair<int,QString>(XQSystemToneService::ConfirmationBeepTone, "[T] Confirmation Beep"));
    tonesList.append(QPair<int,QString>(XQSystemToneService::DefaultBeepTone, "[T] Default Beep"));
    tonesList.append(QPair<int,QString>(XQSystemToneService::ErrorBeepTone, "[T] Error Beep"));
    tonesList.append(QPair<int,QString>(XQSystemToneService::InformationBeepTone, "[T] Information Beep"));
    tonesList.append(QPair<int,QString>(XQSystemToneService::WarningBeepTone, "[T] Warning Beep"));
    tonesList.append(QPair<int,QString>(XQSystemToneService::IntegratedHandsFreeActivatedTone, "[T] Integrated Hands Free Act."));

    tonesList.append(QPair<int,QString>(XQSystemToneService::TouchScreenTone, "[T] Touch Screen"));

    tonesList.append(QPair<int,QString>(XQSystemToneService::ChatAlertTone, "[T] Chat Alert"));
    tonesList.append(QPair<int,QString>(XQSystemToneService::EmailAlertTone, "[T] Email Alert"));
    tonesList.append(QPair<int,QString>(XQSystemToneService::MmsAlertTone, "[T] MMS Alert"));
    tonesList.append(QPair<int,QString>(XQSystemToneService::SmsAlertTone, "[T] SMS Alert"));
    tonesList.append(QPair<int,QString>(XQSystemToneService::DeliveryReportTone, "[T] Delivery Report"));
    tonesList.append(QPair<int,QString>(XQSystemToneService::MessageSendFailureTone, "[T] Message Send Failure"));

    tonesList.append(QPair<int,QString>(XQSystemToneService::BatteryLowTone, "[T] Battery Low"));
    tonesList.append(QPair<int,QString>(XQSystemToneService::BatteryRechargedTone, "[T] Battery Recharge"));
    tonesList.append(QPair<int,QString>(XQSystemToneService::PowerOnTone, "[T] Power On"));
    tonesList.append(QPair<int,QString>(XQSystemToneService::PowerOffTone, "[T] Power Off"));
    tonesList.append(QPair<int,QString>(XQSystemToneService::WakeUpTone, "[T] Wake Up"));
    tonesList.append(QPair<int,QString>(XQSystemToneService::WrongChargerTone, "[T] Wrong Charger"));

    tonesList.append(QPair<int,QString>(XQSystemToneService::AutomaticRedialCompleteTone, "[T] Automatic Redial Compl."));

    tonesList.append(QPair<int,QString>(XQSystemToneService::VoiceStartTone, "[T] Voice Start"));
    tonesList.append(QPair<int,QString>(XQSystemToneService::VoiceErrorTone, "[T] Voice Error"));
    tonesList.append(QPair<int,QString>(XQSystemToneService::VoiceAbortTone, "[T] Voice Abort"));

    qDebug() << QString("[sts] STSTest::STSTest 5");
    QListIterator<QPair<int, QString> > iter(tonesList);
    while (iter.hasNext()) {
        QPair<int, QString> item = iter.next();
        box->addItem(item.second, item.first);
    }

    layout->addStretch(4);
    layout->addWidget(box);

    qDebug() << QString("[sts] STSTest::STSTest 6");
    layout->addStretch(1);
    layout->addWidget(playToneButton);
    layout->addStretch(1);
    layout->addWidget(playAlarmButton);
    layout->addStretch(1);
    layout->addWidget(playAlarmAsToneButton);
    layout->addStretch(1);	
    currCtxLabel = new QLabel("Last context: ?", this);
    layout->addWidget(currCtxLabel);
    layout->addStretch(2);
    layout->addWidget(stopAlarmButton);
    layout->addStretch(1);
    layout->addWidget(new QLabel("Context:", this));
    layout->addWidget(spinBox);

    layout->addStretch(1);
    callbackLabel = new QLabel("SIGNALS: ---", this);
    layout->addWidget(callbackLabel);
    
    layout->addStretch(4);
    layout->addWidget(quitButton);

    
    window->setLayout(layout);
    setCentralWidget(window);
    window->show();;

}

STSTest::~STSTest()
{
    delete sts;
}

void STSTest::playTone()
{
    int id = box->itemData(box->currentIndex(), Qt::UserRole).toInt();
    
    qDebug("Beep!!! (%d)", id);
    
    sts->playTone((XQSystemToneService::ToneType)id);
    
    currCtxLabel->setText(QString("Last context: -"));
}

void STSTest::playAlarm()
{
    int id = box->itemData(box->currentIndex(), Qt::UserRole).toInt();
    
    qDebug("Beep!!! (%d)", id);
    unsigned int ctx = 0;
    
    sts->playAlarm((XQSystemToneService::AlarmType)id, ctx);
    
    currCtxLabel->setText(QString("Last context: %1").arg(ctx));
}

void STSTest::playAlarmAsTone()
{
    int id = box->itemData(box->currentIndex(), Qt::UserRole).toInt();
    
    qDebug("Beep!!! (%d)", id);
    unsigned int ctx = 0;
    
    sts->playAlarm((XQSystemToneService::ToneType)id, ctx);
    
    currCtxLabel->setText(QString("Last context: %1").arg(ctx));
}

void STSTest::stopAlarm()
{
    int ctx = spinBox->value();

    qDebug("Silence! (%d)", ctx);
    sts->stopAlarm(ctx);
}

void STSTest::startCallback()
{
    callbackLabel->setText(QString("SIGNALS: playing tone started..."));
    callbackCleanTimer.start(1000);
}

void STSTest::startCallback(unsigned int ctx)
{
    callbackLabel->setText(QString("SIGNALS: (%1) playing alarm started...").arg(ctx));
    callbackCleanTimer.start(1000);
}

void STSTest::stopCallback(unsigned int ctx)
{
    callbackLabel->setText(QString("SIGNALS: (%1) playing alarm finished...").arg(ctx));
    callbackCleanTimer.start(1000);
}

void STSTest::cleanCallback()
{
    callbackLabel->setText("SIGNALS: ---");
}

int main(int argc, char* argv[])
{  
    QApplication app(argc, argv);

    STSTest* tester = new STSTest();
    tester->showMaximized();

    int rv = app.exec();
    delete tester;
    return rv;
}
    
