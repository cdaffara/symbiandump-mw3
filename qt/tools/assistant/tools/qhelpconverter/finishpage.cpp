/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Assistant of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in the Technology Preview License Agreement accompanying
** this package.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
**
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtGui/QTextEdit>
#include <QtGui/QLayout>
#include <QtGui/QSpacerItem>
#include <QtGui/QApplication>

#include "finishpage.h"

QT_BEGIN_NAMESPACE

FinishPage::FinishPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Converting File"));
    setSubTitle(tr("Creating the new Qt help files from the old ADP file."));
    setFinalPage(true);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addItem(new QSpacerItem(20, 20, QSizePolicy::Minimum,
        QSizePolicy::Fixed));

    m_textEdit = new QTextEdit();
    layout->addWidget(m_textEdit);

    layout->addItem(new QSpacerItem(20, 40, QSizePolicy::Minimum,
        QSizePolicy::Expanding));
}

void FinishPage::appendMessage(const QString &msg)
{
    m_textEdit->append(msg);
    qApp->processEvents();
}

QT_END_NAMESPACE
