/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in the either Technology Preview License Agreement or the
** Beta Release License Agreement.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at http://qt.nokia.com/contact.
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QApplication>
#if ENABLE_OPENGL 
#ifndef QT_NO_OPENGL
#include <QGLWidget>
#endif
#endif
#include <qconfig.h>
#include "mainview.h"
#include "itemrecyclinglist.h"
#include "recycledlistitem.h"
#include "dummydatagen.h"
#include "commandline.h"
#include "controller.h"
#include "settings.h"

int main(int argc, char *argv[])
{
    Settings settings;
    bool resolutionFromCmdLine = false;

    if (!readSettingsFromCommandLine(argc, argv, settings))
        return 1;

    if (settings.size().width() > 0 && settings.size().height() > 0)
        resolutionFromCmdLine = true;

    QApplication app(argc, argv);
    app.setApplicationName("GraphicsViewBenchmark");

    MainView mainView(settings.options().testFlag(Settings::UseOpenGL),
                      settings.options().testFlag(Settings::OutputFps));

    if ((settings.angle() % 360) !=0)
        mainView.rotateContent(settings.angle());
 
    if (resolutionFromCmdLine) {
        mainView.resize(settings.size().width(),settings.size().height());
        mainView.show();
    }
    else {
#if defined(Q_OS_SYMBIAN) || defined(Q_WS_MAEMO_5)
    mainView.showFullScreen();
#else
    mainView.resize(360, 640);
    mainView.show();
#endif
    }

    Controller ctrl(&mainView, settings);

    return app.exec();
}
