/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
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

#include "qitem_p.h"

#include "qunioniterator_p.h"

QT_BEGIN_NAMESPACE

using namespace QPatternist;

UnionIterator::UnionIterator(const Item::Iterator::Ptr &it1,
                             const Item::Iterator::Ptr &it2) : m_it1(it1),
                                                               m_it2(it2),
                                                               m_position(0),
                                                               m_node1(m_it1->next()),
                                                               m_node2(m_it2->next())
{
    Q_ASSERT(m_it1);
    Q_ASSERT(m_it2);
}

Item UnionIterator::next()
{
    ++m_position;
    if(m_node1 && m_node2)
    {
        if(m_node1.asNode().model() != m_node2.asNode().model())
        {
            m_current = m_node1;
            m_node1 = m_it1->next();
            return m_current;
        }

        switch(m_node1.asNode().compareOrder(m_node2.asNode()))
        {
            case QXmlNodeModelIndex::Precedes:
            {
                m_current = m_node1;
                m_node1 = m_it1->next();
                return m_current;
            }
            case QXmlNodeModelIndex::Follows:
            {
                m_current = m_node2;
                m_node2 = m_it2->next();
                return m_current;
            }
            default:
            {
                m_current = m_node2;
                m_node1 = m_it1->next();
                m_node2 = m_it2->next();
                return m_current;
            }
        }
    }

    if(m_node1)
    {
        m_current = m_node1;
        m_node1 = m_it1->next();
        return m_current;
    }

    if(m_node2)
    {
        m_current = m_node2;
        m_node2 = m_it2->next();
        return m_current;
    }

    m_current.reset();
    m_position = -1;
    return Item();
}

Item UnionIterator::current() const
{
    return m_current;
}

xsInteger UnionIterator::position() const
{
    return m_position;
}

Item::Iterator::Ptr UnionIterator::copy() const
{
    return Item::Iterator::Ptr(new UnionIterator(m_it1->copy(), m_it2->copy()));
}

QT_END_NAMESPACE