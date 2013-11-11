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

#ifndef QFILESYSTEMITERATOR_H
#define QFILESYSTEMITERATOR_H

#include <QtCore/qdir.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

QT_MODULE(Core)

class QFileSystemIteratorPrivate;
class //Q_CORE_EXPORT
QFileSystemIterator
{
public:
    enum IteratorFlag {
        NoIteratorFlags = 0x0,
        FollowSymlinks = 0x1,
        Subdirectories = 0x2
    };
    Q_DECLARE_FLAGS(IteratorFlags, IteratorFlag)

    QFileSystemIterator(const QDir &dir, IteratorFlags flags = NoIteratorFlags);
    QFileSystemIterator(const QString &path,
                 IteratorFlags flags = NoIteratorFlags);
    QFileSystemIterator(const QString &path,
                 QDir::Filters filter,
                 IteratorFlags flags = NoIteratorFlags);
    QFileSystemIterator(const QString &path,
                 const QStringList &nameFilters,
                 QDir::Filters filters = QDir::NoFilter,
                 IteratorFlags flags = NoIteratorFlags);

    virtual ~QFileSystemIterator();

    void next();
    bool atEnd() const;

    QString fileName() const;
    QString filePath() const;
    QFileInfo fileInfo() const;
    QString path() const;

private:
    Q_DISABLE_COPY(QFileSystemIterator)

    QFileSystemIteratorPrivate *d;
    friend class QDir;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(QFileSystemIterator::IteratorFlags)

QT_END_NAMESPACE

QT_END_HEADER

#endif
