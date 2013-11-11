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

#include "xqserviceipcmarshal.h"

/*!
    \file xqserviceipcmarshal.h
*/

/*!
    \def Q_DECLARE_USER_METATYPE_NO_OPERATORS(TYPE)

    This macro declares \a TYPE as a user-defined type within the Qt
    metatype system. It should be used in header files, just after
    the declaration of TYPE. A corresponding invocation of
    Q_IMPLEMENT_USER_METATYPE_NO_OPERATORS(TYPE) should appear in
    a source file. This macro should be used instead of
    Q_DECLARE_USER_METATYPE when no need to declare datastream operators.
    
    This example declares the class MyClass that doesn't need to declare
    datastream operators:
    
    \code
        Q_DECLARE_USER_METATYPE(MyClass)
    \endcode
    
    \sa Q_DECLARE_USER_METATYPE_ENUM(), Q_IMPLEMENT_USER_METATYPE(), Q_IMPLEMENT_USER_METATYPE_ENUM(), Q_REGISTER_USER_METATYPE()
*/

/*!
    \def Q_DECLARE_USER_METATYPE(TYPE)

    This macro declares \a TYPE as a user-defined type within the
    Qt metatype system.  It should be used in header files, just
    after the declaration of \a TYPE.  A corresponding invocation
    of Q_IMPLEMENT_USER_METATYPE should appear in a source file.

    This example declares the class \c{MyClass}:

    \code
        Q_DECLARE_USER_METATYPE(MyClass)
    \endcode

    \sa Q_DECLARE_USER_METATYPE_ENUM(), Q_IMPLEMENT_USER_METATYPE(), Q_IMPLEMENT_USER_METATYPE_ENUM(), Q_REGISTER_USER_METATYPE()
*/

/*!
    \def Q_DECLARE_USER_METATYPE_TYPEDEF(TAG,TYPE)

    This macro declares \a TYPE as a user-defined type within the
    Qt metatype system, but declares it as a typedef for a pre-existing
    metatype.  The \a TAG is an identifier that will usually be the same
    as \a TYPE, but may be slightly different if \a TYPE is nested.
    For example, if \a TYPE is \c{Foo::Bar}, then \a TAG should be
    \c{Foo_Bar} to make it a valid identifier.

    This macro should be used in header files, just after the declaration
    of \a TYPE.  A corresponding invocation of
    Q_IMPLEMENT_USER_METATYPE_TYPEDEF should appear in a source file.

    This example declares the types \c{Foo} and \c{Bar} as typedef aliases.

    \code
        typedef Foo Bar;
        Q_DECLARE_USER_METATYPE(Foo)
        Q_DECLARE_USER_METATYPE_TYPEDEF(Bar, Bar)
    \endcode

    \sa Q_DECLARE_USER_METATYPE_ENUM(), Q_IMPLEMENT_USER_METATYPE(), Q_IMPLEMENT_USER_METATYPE_ENUM(), Q_REGISTER_USER_METATYPE()
*/

/*!
    \def Q_IMPLEMENT_USER_METATYPE_NO_OPERATORS(TYPE)

    This macro implements the code necessary to register \a TYPE as a user-defined
    type within the Qt metatype system.
    
    This example implements the registration, logic for the class MyClass that
    doesn't need to declare datastream operators:
    
    \code
        Q_IMPLEMENT_USER_METATYPE_NO_OPERATORS(MyClass)
    \endcode
    
    On most systems, this macro will arrange for registration to be performed at program
    startup. On systems that don't support global constructors properly, it may be
    necessary to manually call Q_REGISTER_USER_METATYPE().

    \sa Q_IMPLEMENT_USER_METATYPE_ENUM(), Q_IMPLEMENT_USER_METATYPE(), Q_DECLARE_METATYPE(), Q_REGISTER_USER_METATYPE()
*/

/*!
    \def Q_DECLARE_USER_METATYPE_ENUM(TYPE)

    This macro declares \a TYPE as a user-defined enumerated type within
    the Qt metatype system.  It should be used in header files, just
    after the declaration of \a TYPE.  A corresponding invocation
    of Q_IMPLEMENT_USER_METATYPE_ENUM should appear in a source file.

    This example declares the enumerated type \c{MyEnum}:

    \code
        Q_DECLARE_USER_METATYPE_ENUM(MyEnum)
    \endcode

    This macro differs from Q_DECLARE_USER_METATYPE in that it explicitly
    declares datastream operators for the type.

    \sa Q_IMPLEMENT_USER_METATYPE_ENUM(), Q_IMPLEMENT_USER_METATYPE(), Q_DECLARE_METATYPE(), Q_REGISTER_USER_METATYPE()
*/

/*!
    \def Q_IMPLEMENT_USER_METATYPE(TYPE)

    This macro implements the code necessary to register \a TYPE
    as a user-defined type within the Qt metatype system.

    This example implements the registration logic for the class \c{MyClass}:

    \code
        Q_IMPLEMENT_USER_METATYPE(MyClass)
    \endcode

    On most systems, this macro will arrange for registration to be
    performed at program startup.  On systems that don't support
    global constructors properly, it may be necessary to manually
    call Q_REGISTER_USER_METATYPE().

    \sa Q_DECLARE_USER_METATYPE(), Q_DECLARE_USER_METATYPE_ENUM(), Q_IMPLEMENT_USER_METATYPE_ENUM(), Q_REGISTER_USER_METATYPE()
*/

/*!
    \def Q_IMPLEMENT_USER_METATYPE_TYPEDEF(TAG,TYPE)

    This macro implements the code necessary to register \a TYPE
    as a user-defined typedef alias within the Qt metatype system.
    The \a TAG should be the same as the tag used in the
    corresponding invocation of Q_DECLARE_USER_METATYPE_TYPEDEF.

    This example implements the registration logic for the typedef'ed
    types \c{Foo} and \c{Bar}:

    \code
        typedef Foo Bar;
        Q_IMPLEMENT_USER_METATYPE(Foo)
        Q_IMPLEMENT_USER_METATYPE_TYPEDEF(Bar, Bar)
    \endcode

    On most systems, this macro will arrange for registration to be
    performed at program startup.  On systems that don't support
    global constructors properly, it may be necessary to manually
    call Q_REGISTER_USER_METATYPE().

    \sa Q_DECLARE_USER_METATYPE(), Q_DECLARE_USER_METATYPE_ENUM(), Q_IMPLEMENT_USER_METATYPE_ENUM(), Q_REGISTER_USER_METATYPE()
*/

/*!
    \def Q_IMPLEMENT_USER_METATYPE_ENUM(TYPE)

    This macro implements the code necessary to register \a TYPE
    as a user-defined type within the Qt metatype system.  \a TYPE
    is assumed to be an enumerated type.  Using this macro relieves
    the need to manually declare \c{operator<<} and \c{operator>>}
    for the enumerated type.  Non-enumerated types should use
    Q_IMPLEMENT_USER_METATYPE() instead.

    This example implements the registration logic for the type \c{MyEnum}:

    \code
        Q_IMPLEMENT_USER_METATYPE_ENUM(MyEnum)
    \endcode

    On most systems, this macro will arrange for registration to be
    performed at program startup.  On systems that don't support
    global constructors properly, it may be necessary to manually
    call Q_REGISTER_USER_METATYPE().

    \sa Q_DECLARE_USER_METATYPE_ENUM(), Q_DECLARE_USER_METATYPE(), Q_IMPLEMENT_USER_METATYPE(), Q_REGISTER_USER_METATYPE()
*/

/*!
    \def Q_REGISTER_USER_METATYPE(TYPE)

    This macro can be called as a function to manually register \a TYPE
    as a user-defined type within the Qt metatype system.  It is only
    needed if the system does not support global constructors properly.

    \sa Q_DECLARE_USER_METATYPE(), Q_DECLARE_USER_METATYPE_ENUM(), Q_IMPLEMENT_USER_METATYPE(), Q_IMPLEMENT_USER_METATYPE_ENUM()
*/

Q_IMPLEMENT_USER_METATYPE_NO_OPERATORS(QUuid)
