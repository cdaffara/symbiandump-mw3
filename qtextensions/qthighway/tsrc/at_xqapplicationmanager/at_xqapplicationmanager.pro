#
# Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
# All rights reserved.
# This component and the accompanying materials are made available
# under the terms of "Eclipse Public License v1.0"
# which accompanies this distribution, and is available
# at the URL "http://www.eclipse.org/legal/epl-v10.html".
#
# Initial Contributors:
# Nokia Corporation - initial contribution.
#
# Contributors:
#
# Description: 
#

TEMPLATE = app

QT += testlib
CONFIG  += qtestlib

HEADERS += at_xqapplicationmanager.h \
		   ../inc/testdata.h
SOURCES += at_xqapplicationmanager.cpp

#DEFINES += QT_NO_DEBUG_STREAM

# capability
TARGET.CAPABILITY = ALL -TCB

drmfiles.sources += drmfiles/test.txt
drmfiles.sources += drmfiles/111-test1.odf
drmfiles.sources += drmfiles/one.jpg
drmfiles.sources += drmfiles/RoAcqoffer-111-aac-i15m.ort
drmfiles.sources += drmfiles/SD_Celebration_SP.dcf
drmfiles.sources += drmfiles/SD_jpg_sun.dcf
drmfiles.sources += drmfiles/STC1_128_44_16_2_CBR.wma

drmfiles.path = c:/data/Others/

DEPLOYMENT += drmfiles

LIBS+= -lxqservice -lxqserviceutil