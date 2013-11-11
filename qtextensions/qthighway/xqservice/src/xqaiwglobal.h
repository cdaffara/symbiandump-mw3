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

#ifndef XQAIWGLOBAL_H_
#define XQAIWGLOBAL_H_

#if !defined(XQAIW_EXPORT)
#if defined(XQ_BUILD_XQSERVICE_LIB) // TODO !!! REMEMBER TO CHANGE THAT IF MOVED TO DIFFERENT LIB !!!
#define XQAIW_EXPORT Q_DECL_EXPORT
#else
#define XQAIW_EXPORT Q_DECL_IMPORT
#endif
#endif

#endif /* XQAIWGLOBAL_H_ */
