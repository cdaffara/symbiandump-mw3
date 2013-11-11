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

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.

#ifndef Patternist_Tokenizer_H
#define Patternist_Tokenizer_H

#include <QPair>
#include <QSharedData>
#include <QString>
#include <QUrl>

#include "qparsercontext_p.h"
#include "qtokensource_p.h"

/**
 * @file
 * @short Contains functions and classes used by the parser and tokenizer.
 */

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

namespace QPatternist
{
    typedef QPair<QString, Expression::Ptr> AttributeHolder;
    typedef QVector<AttributeHolder> AttributeHolderVector;

    class OrderSpecTransfer
    {
    public:
        typedef QList<OrderSpecTransfer> List;
        inline OrderSpecTransfer()
        {
        }

        inline OrderSpecTransfer(const Expression::Ptr &aExpr,
                                 const OrderBy::OrderSpec aOrderSpec) : expression(aExpr),
                                                                        orderSpec(aOrderSpec)
        {
            Q_ASSERT(expression);
        }

        Expression::Ptr     expression;
        OrderBy::OrderSpec  orderSpec;
    };

    /**
     * @short The value the parser, but not the tokenizers, uses for tokens and
     * non-terminals.
     *
     * It is inefficient but ensures nothing leaks, by invoking C++
     * destructors even in the cases the code throws exceptions. This might be
     * able to be done in a more efficient way -- suggestions are welcome.
     */
    class TokenValue
    {
    public:
        QString                         sval;

        Expression::Ptr                 expr;
        Expression::List                expressionList;

        Cardinality                     cardinality;
        ItemType::Ptr                   itemType;
        SequenceType::Ptr               sequenceType;
        FunctionArgument::List          functionArguments;
        FunctionArgument::Ptr           functionArgument;
        QVector<QXmlName>               qNameVector;
        QXmlName                        qName;
        /**
         * Holds enum values.
         */
        EnumUnion                       enums;

        AttributeHolder                 attributeHolder;
        AttributeHolderVector           attributeHolders;
        OrderSpecTransfer::List         orderSpecs;
        OrderSpecTransfer               orderSpec;
    };
}

QT_END_NAMESPACE

/**
 * Macro for the data type of semantic values; int by default.
 * See section Data Types of Semantic Values.
 */
#define YYSTYPE QPatternist::TokenValue

#include "qquerytransformparser_p.h" /* This inclusion must be after TokenValue. */

QT_BEGIN_NAMESPACE

namespace QPatternist
{
    /**
     * @short Base class for all tokenizers.
     *
     * The main entry point is nextToken(), which ones calls to retrieve the stream
     * of tokens this Tokenizer delivers.
     *
     * @see <a href="http://www.w3.org/TR/xquery-xpath-parsing/">Building a
     * Tokenizer for XPath or XQuery</a>
     * @author Frans Englich <frans.englich@nokia.com>
     */
    class Tokenizer : public TokenSource
    {
    public:
        inline Tokenizer(const QUrl &queryU) : m_queryURI(queryU)
        {
            Q_ASSERT(queryU.isValid());
        }

        typedef QExplicitlySharedDataPointer<Tokenizer> Ptr;

        /**
         * Switches the Tokenizer to only do scanning, and returns complete
         * strings for attribute value templates as opposed to the tokens for
         * the contained expressions.
         *
         * The current position in the stream is returned. It can be used to
         * later resume regular tokenization.
         */
        virtual int commenceScanOnly() = 0;

        /**
         * Resumes regular parsing from @p position. The tokenizer must be in
         * the scan-only state, which the commenceScanOnly() call transists to.
         *
         * The tokenizer will return the token POSITION_SET once after this
         * function has been called.
         */
        virtual void resumeTokenizationFrom(const int position) = 0;

        /**
         * @returns the URI of the resource being tokenized.
         */
        inline const QUrl &queryURI() const
        {
            return m_queryURI;
        }

        virtual void setParserContext(const ParserContext::Ptr &parseInfo) = 0;

    protected:
        /**
         * Returns a string representation of @p token.
         *
         * This function is used for debugging purposes. The implementation of
         * this function is in querytransformparser.ypp.
         */
        static QString tokenToString(const Token &token);

    private:
        Q_DISABLE_COPY(Tokenizer)
        const QUrl m_queryURI;
    };

}

#undef Patternist_DEBUG_PARSER // disable it for now

QT_END_NAMESPACE

QT_END_HEADER

#endif
