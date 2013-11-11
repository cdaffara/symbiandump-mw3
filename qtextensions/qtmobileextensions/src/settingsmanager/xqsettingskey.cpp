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

#include "xqsettingskey.h"

/****************************************************
 *
 * XQSettingsKey
 *
 ****************************************************/

/*!
    \class XQSettingsKey

    \brief The XQSettingsKey defines the target for XQSettingsManager operations.
*/

/*!
    \enum XQSettingsKey::Target

    This enum defines the possible item target for an XQSettingsKey object.
*/
/*! \var XQSettingsKey::TargetCentralRepository XQSettingsKey::TargetCentralRepository
    The settings key targets to Symbian OS's Central Repository.
*/
/*! \var XQSettingsKey::TargetPublishAndSubscribe XQSettingsKey::TargetPublishAndSubscribe
    The settings key targets to Symbian OS's Publish and Subscribe property.
*/

/*!
    Constructs an XQSettingsKey object.
    \param target This is either XQSettingsKey::TargetCentralRepository or 
    XQSettingsKey::TargetPublishAndSubscribe.
    \param uid In case of Central Repository a CenRep Uid. In case of
    Publish and Subscribe an RProperty category Uid.
    \param key Settings Key
*/
XQSettingsKey::XQSettingsKey(
    XQSettingsKey::Target target, long int uid, unsigned long int key) :
    m_target(target), m_uid(uid), m_key(key)
{   
}

/*!
    Destructs an XQSettingsKey object.
*/
XQSettingsKey::~XQSettingsKey()
{
}

/*!
    Gets a target of the item.

    \return target Either XQSettingsKey::TargetCentralRepository or 
    XQSettingsKey::TargetPublishAndSubscribe.
*/
XQSettingsKey::Target XQSettingsKey::target() const
{
    return m_target;
}

/*!
    Gets a uid of the item

    \return uid. In case of Central Repository a CenRep Uid. In case of
    Publish and Subscribe an RProperty category Uid.
*/
long int XQSettingsKey::uid() const
{    
    return m_uid;
}

/*!
    Gets a key of the item

    \return Key
*/
unsigned long int XQSettingsKey::key() const
{
    return m_key;
}

/*!
    \class XQCentralRepositorySettingsKey

    \brief The XQCentralRepositorySettingsKey defines the Central Repository
    target for XQSettingsManager operations.
*/

/*!
    Constructs an XQCentralRepositorySettingsKey object.
    \param repositoryUid CenRep Uid
    \param key Settings Key
*/
XQCentralRepositorySettingsKey::XQCentralRepositorySettingsKey(long int repositoryUid, unsigned long int key) :
    XQSettingsKey(XQSettingsKey::TargetCentralRepository, repositoryUid, key)
{
}

/*!
    Destructs an XQCentralRepositorySettingsKey object.
*/
XQCentralRepositorySettingsKey::~XQCentralRepositorySettingsKey()
{
}

/*!
    \class XQPublishAndSubscribeSettingsKey

    \brief The XQPublishAndSubscribeSettingsKey defines the Publish and
    Subscribe target for XQSettingsManager operations.
*/

/*!
    Constructs an XQPublishAndSubscribeSettingsKey object.
    \param categoryUid RProperty category Uid.
    \param key Settings Key
*/
XQPublishAndSubscribeSettingsKey::XQPublishAndSubscribeSettingsKey(long int categoryUid, unsigned long int key) :
    XQSettingsKey(XQSettingsKey::TargetPublishAndSubscribe, categoryUid, key)
{
}

/*!
    Destructs an XQPublishAndSubscribeSettingsKey object.
*/
XQPublishAndSubscribeSettingsKey::~XQPublishAndSubscribeSettingsKey()
{
}

// End of file
