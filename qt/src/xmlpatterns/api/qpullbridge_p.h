/****************************************************************************
**
** Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the QtXmlPatterns module of the Qt Toolkit.
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

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.

#ifndef PATTERNIST_PULLBRIDGE_P_H
#define PATTERNIST_PULLBRIDGE_P_H

#include <QtCore/QPair>
#include <QtCore/QStack>

#include "qabstractxmlforwarditerator_p.h"
#include "qabstractxmlpullprovider_p.h"
#include "qitem_p.h"

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

namespace QPatternist
{
    class PullBridge : public AbstractXmlPullProvider
    {
    public:
        inline PullBridge(const QXmlNodeModelIndex::Iterator::Ptr &it) : m_current(StartOfInput)
        {
            Q_ASSERT(it);
            m_iterators.push(qMakePair(StartOfInput, it));
        }

        virtual Event next();
        virtual Event current() const;
        virtual QXmlName name() const;
        /**
         * Returns always an empty QVariant.
         */
        virtual QVariant atomicValue() const;
        virtual QString stringValue() const;
        virtual QHash<QXmlName, QString> attributes();
        virtual QHash<QXmlName, QXmlItem> attributeItems();

        QXmlNodeModelIndex index() const;
        QSourceLocation sourceLocation() const;

    private:
        typedef QStack<QPair<Event, QXmlNodeModelIndex::Iterator::Ptr> > IteratorStack;
        IteratorStack      m_iterators;
        QXmlNodeModelIndex m_index;
        Event              m_current;
    };
}

QT_END_NAMESPACE

QT_END_HEADER

#endif
