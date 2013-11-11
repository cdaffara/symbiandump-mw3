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

#ifndef T_TEST5_H
#define T_TEST5_H

#include <QObject>
#include <QVariant>
#include <QMainWindow>
#include <QWidget>
#include <QComboBox>
#include <QSpinBox>
#include <QLabel>
#include <QTimer>

#include <xqsystemtoneservice.h>

class STSTest : public QMainWindow
{

Q_OBJECT

public:
   
    STSTest(QWidget *parent = 0);
    
    ~STSTest();
    
public slots: 

    void playTone();

    void playAlarm();
	
    void playAlarmAsTone();

    void stopAlarm();


    void startCallback();

    void startCallback(unsigned int ctx);

    void stopCallback(unsigned int ctx);
    
    void cleanCallback();

private:

    QLabel *callbackLabel;
    QTimer callbackCleanTimer;

    QLabel *currCtxLabel;
    QSpinBox *spinBox;
    QComboBox *box;
    XQSystemToneService *sts;
    
};

#endif // T_TEST5_H
