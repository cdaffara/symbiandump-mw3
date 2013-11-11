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

HEADERS += at_servicesregression.h
SOURCES += at_servicesregression.cpp

#DEFINES += QT_NO_DEBUG_STREAM

# capability
TARGET.CAPABILITY = ALL -TCB

drmfiles.sources += drmfiles/one_v2.jpg
drmfiles.sources += drmfiles/324kbps_MPEG4_352x288_AAC_15fps.3gp
drmfiles.sources += drmfiles/111-test1_v2.odf

drmfiles.path = c:/data/Others/

DEPLOYMENT += drmfiles

LIBS+= -lxqservice -lxqserviceutil -lws32 -lcone