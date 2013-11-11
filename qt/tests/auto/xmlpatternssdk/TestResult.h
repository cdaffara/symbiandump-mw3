/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the test suite of the Qt Toolkit.
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

#ifndef PatternistSDK_TestResult_H
#define PatternistSDK_TestResult_H

#include <QList>
#include <QObject>
#include <QPointer>
#include <QString>

#include <QtXmlPatterns/private/qitem_p.h>
#include "ErrorHandler.h"

#include "ASTItem.h"


QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

namespace QPatternistSDK
{
    class ASTItem;
    class XMLWriter;

    /**
     * @short represents the result produced by running a test case.
     *
     * This information TestResult houses is:
     *
     * - The result status() of the run. Whether the test case succeeded or not, for example.
     * - The astTree() which reflects the compiled test case
     * - The messages issued when compiling and running the test case, retrievable via messages()
     * - The data -- XPath Data Model items() -- the test case evaluated to, if any.
     *
     * @ingroup PatternistSDK
     * @author Frans Englich <frans.englich@nokia.com>
     */
    class Q_PATTERNISTSDK_EXPORT TestResult : public QObject
    {
        Q_OBJECT

    public:
        enum Status
        {
            /**
             * Used when the status is unknown.
             */
            Unknown = 0,

            /**
             * The test case passed.
             */
            Pass,

            /**
             * The test case failed.
             */
            Fail,

            /**
             * The test was not run. Similar to "SKIP".
             */
            NotTested
        };

        /**
         * A list of TestResult instances.
         */
        typedef QList<QPointer<TestResult> > List;

        /**
         * Constructs a TestResult.
         *
         * @param testName the name of the test. For example, @c Literal-001.
         * @param astTree may be @c null, signalling no AST being available, or point to one.
         * @param status the result status of running the test-case. Whether the test-case
         * passed or failed, and so forth.
         * @param errors the errors and warnings that were reported while running the test-case
         * @param items the XDM items that were outputted, if any
         * @param serialized the output when serialized
         */
        TestResult(const QString &testName,
                   const Status status,
                   ASTItem *astTree,
                   const ErrorHandler::Message::List &errors,
                   const QPatternist::Item::List &items,
                   const QString &serialized);

        virtual ~TestResult();

        Status status() const;

        QString comment() const;
        void setComment(const QString &comment);

        QPatternist::Item::List items() const;

        ErrorHandler::Message::List messages() const;

        /**
         * Serializes itself to @p receiver, into a test-case element,
         * as per @c XQTSResult.xsd.
         */
        void toXML(XMLWriter &receiver) const;

        ASTItem *astTree() const;

        /**
         * @returns a string representation for @p status, as per the anonymous
         * type inside the type test-case, in @c XQTSResult.xsd. For example, if @p status
         * is NotTested, is "not tested" returned.
         */
        static QString displayName(const TestResult::Status status);

        static Status statusFromString(const QString &string);

        /**
         * @returns the output of this test result(if any) as when
         * being serialized.
         */
        QString asSerialized() const;

    private:
        const Status                        m_status;
        QString                             m_comment;
        const ErrorHandler::Message::List   m_messages;
        QPointer<ASTItem>                   m_astTree;
        QString                             m_testName;
        const QPatternist::Item::List       m_items;
        const QString                       m_asSerialized;
    };
}

QT_END_NAMESPACE

QT_END_HEADER

#endif
// vim: et:ts=4:sw=4:sts=4
