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

#ifndef PROCESSINFO_H_
#define PROCESSINFO_H_

#include <QHash>
#include <e32base.h>

class CProcessInfo : public CActive
{
public:
    static void AddProcessL(const TUid& appUid, RProcess& appProcess);
    static bool EnsureProcessCanStartL(const TUid& appUid);
    static void Destroy();
    
protected:
    CProcessInfo(const TUid& appUid);
    ~CProcessInfo();
    void ConstructL(RProcess& appProcess);
    void DoCancel();
    void RunL();
    
protected:
    class ProcessInfoMap
    {
    public:
        ~ProcessInfoMap();
        QHash<TInt32, CProcessInfo*> map;
    };
    
    static ProcessInfoMap iProcessInfoMap;
    const TUid iAppUid;
};

class ProcessInfoDestroyer : public QObject
{
    Q_OBJECT
public:
    static void enable();
    static void disable();
    
public slots:
    void destroy();
    
private:
    ProcessInfoDestroyer();
    
    static ProcessInfoDestroyer *instance;
};

#endif /* PROCESSINFO_H_ */
