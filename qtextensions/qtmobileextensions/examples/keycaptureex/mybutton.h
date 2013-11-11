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

#ifndef MYKEY_H_
#define MYKEY_H_

#include <QPushButton>
#include <QPlainTextEdit>
#include <QEvent>
#include <QKeyEvent>
#include <QMap>

class MyButton : public QPushButton 
{
    Q_OBJECT
    
public:
    
    MyButton(QPlainTextEdit *logger, QWidget *parent = 0) : QPushButton(QString("focus here"), parent)
    {
        justLogger = logger;
        keyLabels.insert(Qt::Key_VolumeUp, "Volume Up");        
        keyLabels.insert(Qt::Key_VolumeDown, "Volume Down");
        keyLabels.insert(Qt::Key_Hangup, "Hangup");        
        keyLabels.insert(Qt::Key_Play, "Play");        
        keyLabels.insert(Qt::Key_MediaNext, "Media Next");        
        keyLabels.insert(Qt::Key_MediaPrevious, "Media Previous");        
        keyLabels.insert(Qt::Key_Forward, "Forward");        
        keyLabels.insert(Qt::Key_Back, "Back");        
    }
    
    ~MyButton() 
    {
    }
    
    /*bool event(QEvent *event)
    {
        if (justLogger) {
            if (event->type() != QEvent::KeyPress) {
//                QString keyName = mapNaturalName((dynamic_cast<QKeyEvent*>event)->key());
//                justLogger->appendPlainText(QString("P> %1").arg(keyName));
                justLogger->appendPlainText(QString("P> %1").arg(static_cast<QKeyEvent*>(event)->key()));
            }
            if (event->type() != QEvent::KeyRelease) {
//                QString keyName = mapNaturalName((dynamic_cast<QKeyEvent*>event)->key());
//                justLogger->appendPlainText(QString("r> %1").arg(keyName));
                justLogger->appendPlainText(QString("R> %1").arg(static_cast<QKeyEvent*>(event)->key()));
            }
        }
        return QPushButton::event(event);
    }*/
    
    void keyPressEvent(QKeyEvent *e)
    {
        QString keyName = mapNaturalName(static_cast<Qt::Key>(e->key()));
        addTextLine(QString("P> %1").arg(keyName));
    }
    
    void keyReleaseEvent(QKeyEvent *e)
    {
        QString keyName = mapNaturalName(static_cast<Qt::Key>(e->key()));
        addTextLine(QString("R> %1").arg(keyName));
    }
    
    void addTextLine(QString aText)
    {
        if ( !aText.endsWith("\n"))
            aText = aText + "\n";
        QString msg = justLogger->toPlainText();
        msg = aText + msg;
        justLogger->setPlainText(msg);
    }
    
private:
    
    QString mapNaturalName(Qt::Key key) {
        QString name = keyLabels[key];
        if (name.isEmpty()) {
            name = QString("0x%1").arg(static_cast<int>(key), 0, 16);
        }
        return name;
    }
    
private:
    // not own
    QPlainTextEdit *justLogger;
    QMap<Qt::Key, QString> keyLabels;
};


#endif /* MYKEY_H_ */
