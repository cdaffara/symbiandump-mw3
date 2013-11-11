/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description: 
*
*/

#include <QtTest/QtTest>
#include <QEventLoop>
#include <QDebug>
#include <QtCore>

#include <W32STD.H>

#include "mytestwindowgroup.h"
#include "stub/remconcoreapitarget.h"
#include "stub/remconcallhandlingtarget.h"
#include "stub/remconinterfaceselector.h"

#include "targetwrapper.h"

#include <xqkeycapture.h>
#include <QFile>

const int KAllFlagsOn = XQKeyCapture::CaptureBasic | 
                        XQKeyCapture::CaptureCallHandlingExt |
                        XQKeyCapture::CaptureSideKeys |
                        XQKeyCapture::CaptureEnableRemoteExtEvents;
const Qt::Key KNotSpecifiedKey = Qt::Key_F35;

class TestXQKeyCapture : public QObject
{
    Q_OBJECT

public:
    TestXQKeyCapture(): QObject(), keyCapture(0) {};


private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();
    
    void testCreateAndDestroy();

    void testCaptureKey_data();
    void testCaptureKey();

    void testCaptureKeyList_data();
    void testCaptureKeyList();

    void testCaptureKey_S60_data();
    void testCaptureKey_S60();

    void testCaptureKeyList_S60_data();
    void testCaptureKeyList_S60();

    void testCaptureKeyUpAndDowns_data();
    void testCaptureKeyUpAndDowns();

    void testCaptureKeyUpAndDownsList_data();
    void testCaptureKeyUpAndDownsList();

    void testCaptureKeyUpAndDowns_S60_data();
    void testCaptureKeyUpAndDowns_S60();

    void testCaptureKeyUpAndDownsList_S60_data();
    void testCaptureKeyUpAndDownsList_S60();

    void testCaptureLongKey_data();
    void testCaptureLongKey();

    void testCaptureLongKeyList_data();
    void testCaptureLongKeyList();

    void testCaptureLongKey_S60_data();
    void testCaptureLongKey_S60();

    void testCaptureLongKeyList_S60_data();
    void testCaptureLongKeyList_S60();

    void testCancelCaptureKey_data();
    void testCancelCaptureKey();

    void testCancelCaptureKeyList_data();
    void testCancelCaptureKeyList();

    void testCancelCaptureKey_S60_data();
    void testCancelCaptureKey_S60();

    void testCancelCaptureKeyList_S60_data();
    void testCancelCaptureKeyList_S60();

    void testCancelCaptureKeyUpAndDowns_data();
    void testCancelCaptureKeyUpAndDowns();

    void testCancelCaptureKeyUpAndDownsList_data();
    void testCancelCaptureKeyUpAndDownsList();

    void testCancelCaptureKeyUpAndDowns_S60_data();
    void testCancelCaptureKeyUpAndDowns_S60();

    void testCancelCaptureKeyUpAndDownsList_S60_data();
    void testCancelCaptureKeyUpAndDownsList_S60();

    void testCancelCaptureLongKey_data();
    void testCancelCaptureLongKey();

    void testCancelCaptureLongKeyList_data();
    void testCancelCaptureLongKeyList();

    void testCancelCaptureLongKey_S60_data();
    void testCancelCaptureLongKey_S60();

    void testCancelCaptureLongKeyList_S60_data();
    void testCancelCaptureLongKeyList_S60();

    void testCaptureRemoteKeys_data();
    void testCaptureRemoteKeys();

    void testCaptureRemoteKeys_SelectorFail_data();
    void testCaptureRemoteKeys_SelectorFail();

    void testCaptureRemoteKeys_ApiCore_data();
    void testCaptureRemoteKeys_ApiCore();    
    
    void testCaptureRemoteKeys_ApiCoreCancel_data();
    void testCaptureRemoteKeys_ApiCoreCancel();
    
    void testCaptureRemoteKeys_CallHandling_data();
    void testCaptureRemoteKeys_CallHandling();    

    void testCaptureRemoteKeys_CallHandlingCancel_data();
    void testCaptureRemoteKeys_CallHandlingCancel();    
    
    void testCaptureRemoteKeys_Basic_data();
    void testCaptureRemoteKeys_Basic();
    
    void testCaptureRemoteKeys_Extended_data();
    void testCaptureRemoteKeys_Extended();
    
    void testCancelCaptureRemoteKeys_data();
    void testCancelCaptureRemoteKeys();


    void testErrorString();
    void testErrorId();
    
    void testKeyMapperFile();

private:
    QString clearString(const QString& line);
    QString clearString(const QString& line, const QString& prefix, const QString& comment);
    void setProperKeys(bool extended);
    
private:
    XQKeyCapture* keyCapture;

    Qt::Key aKey;
    Qt::KeyboardModifiers aModifiersMap;
    Qt::KeyboardModifiers aModifier;
    
    int numOfArgs;
    WindowGroupActionType actionType;
    QList<unsigned int> results;
    QList<unsigned int> additionalResults;
    
    QList<long int> cancelResults;
    QList<long int> cancelAdditionalResults;
    
    long int additionalRequestHandle;
    
    bool willBeAdditionalRequest;
    bool additionalResult;
    bool ignoreWindowGroupAction;
    
    
public slots:
    void windowGroupAction(WindowGroupActionType, QList<unsigned int>);
    void windowGroupActionCancel(WindowGroupActionType, QList<long int>);
    
public:
    //for events handling    
    bool event(QEvent *);
    bool eventFilter(QObject *, QEvent *);
    void processEvent(QEvent *ev);
    
    void resetKeys();
    QWidget myGlobalWidget;
    Qt::Key myLastKeyPressed;
    Qt::Key myLastKeyReleased;
    Qt::Key myLastKeyPress;
    Qt::Key myLastKeyExtendedPress;
    Qt::Key myLastKeyRelease;
    Qt::Key myLastKeyExtendedRelease;
    QMap<TRemConCoreApiOperationId, Qt::Key> myKeyMapping;    
};

// ======== MEMBER FUNCTIONS ========
void TestXQKeyCapture::resetKeys() 
{
    myLastKeyPressed = KNotSpecifiedKey;
    myLastKeyReleased = KNotSpecifiedKey;
    myLastKeyPress = KNotSpecifiedKey;
    myLastKeyExtendedPress = KNotSpecifiedKey;
    myLastKeyRelease = KNotSpecifiedKey;
    myLastKeyExtendedRelease = KNotSpecifiedKey;
}
void TestXQKeyCapture::initTestCase()
{
    bool ret = connect(MyTestWindowGroup::Instance(), SIGNAL(windowGroupAction(WindowGroupActionType, QList<unsigned int>)),
            this, SLOT(windowGroupAction(WindowGroupActionType, QList<unsigned int>)));
    QVERIFY(ret);
    ret = connect(MyTestWindowGroup::Instance(), SIGNAL(windowGroupActionCancel(WindowGroupActionType, QList<long int>)),
            this, SLOT(windowGroupActionCancel(WindowGroupActionType, QList<long int>)));
    QVERIFY(ret);
        
    myGlobalWidget.installEventFilter(this);
    
    // load key mapping
    
    TargetWrapper* myTargetWrapper = new TargetWrapper();
    myTargetWrapper->init(XQKeyCapture::CaptureBasic);
    myKeyMapping = myTargetWrapper->keyMapping;
    delete myTargetWrapper;
    myGlobalWidget.setFocus();
    myGlobalWidget.activateWindow();
}

void TestXQKeyCapture::cleanupTestCase()
{
    //is this needed - looking for panic reason.
    myGlobalWidget.removeEventFilter(this);
    myGlobalWidget.deleteLater();
    //delete MyTestWindowGroup::Instance();
}

void TestXQKeyCapture::init()
{
    keyCapture = new XQKeyCapture();
    resetKeys();
}

void TestXQKeyCapture::cleanup()
{
    delete keyCapture;
    CRemConCoreApiTarget::setNewLLeave(false);
    CRemConCallHandlingTarget::setNewLLeave(false);
    myGlobalWidget.setVisible(false);
}

void TestXQKeyCapture::testCreateAndDestroy()
{
    //empty, all done in init and cleanup
}

////////////////////////////////////////////////////////////////
// CAPTURE
//Capture Key
////////////////////////////////////////////////////////////////
void TestXQKeyCapture::testCaptureKey_data()
{
    QTest::addColumn<unsigned int>("qtKey");
    QTest::addColumn<unsigned int>("qtMask");
    QTest::addColumn<unsigned int>("qtModifier");

    QTest::addColumn<unsigned int>("symbianKey");
    QTest::addColumn<unsigned int>("symbianMask");
    QTest::addColumn<unsigned int>("symbianModifier");
        
    QTest::addColumn<long int>("reqNum");
    
    QTest::addColumn<bool>("additional");
    QTest::addColumn<unsigned int>("additionalSymbianKey");
    
    QTest::newRow("esc_key") << static_cast<unsigned int>(Qt::Key_Escape) 
                            << static_cast<unsigned int>(Qt::NoModifier)
                            << static_cast<unsigned int>(Qt::NoModifier) 
                            << static_cast<unsigned int>(EKeyEscape)
                            << static_cast<unsigned int>(0)
                            << static_cast<unsigned int>(0)
                            << static_cast<long int>(12)
                            << false
                            << static_cast<unsigned int>(0);

    QTest::newRow("esc_key_not_supported") << static_cast<unsigned int>(Qt::Key_Escape) 
                            << static_cast<unsigned int>(Qt::NoModifier)
                            << static_cast<unsigned int>(Qt::NoModifier) 
                            << static_cast<unsigned int>(EKeyEscape)
                            << static_cast<unsigned int>(0)
                            << static_cast<unsigned int>(0)
                            << static_cast<long int>(KErrNotSupported)
                            << false
                            << static_cast<unsigned int>(0);
    
    QTest::newRow("esc_key_modifiers") << static_cast<unsigned int>(Qt::Key_Escape) 
                            << static_cast<unsigned int>(Qt::ShiftModifier | Qt::ControlModifier | Qt::AltModifier | Qt:: KeypadModifier)
                            << static_cast<unsigned int>(Qt::ShiftModifier | Qt::ControlModifier | Qt::AltModifier | Qt:: KeypadModifier) 
                            << static_cast<unsigned int>(EKeyEscape)
                            << static_cast<unsigned int>(EModifierShift | EModifierCtrl | EModifierAlt | EModifierKeypad)
                            << static_cast<unsigned int>(EModifierShift | EModifierCtrl | EModifierAlt | EModifierKeypad)
                            << static_cast<long int>(13)
                            << false
                            << static_cast<unsigned int>(0);
    
    QTest::newRow("shift_key") << static_cast<unsigned int>(Qt::Key_Shift) 
                            << static_cast<unsigned int>(Qt::ShiftModifier)
                            << static_cast<unsigned int>(Qt::ShiftModifier) 
                            << static_cast<unsigned int>(EKeyLeftShift)
                            << static_cast<unsigned int>(EModifierShift)
                            << static_cast<unsigned int>(EModifierShift)
                            << static_cast<long int>(12)
                            << true
                            << static_cast<unsigned int>(EKeyRightShift);

    QTest::newRow("control_key") << static_cast<unsigned int>(Qt::Key_Control) 
                            << static_cast<unsigned int>(Qt::NoModifier)
                            << static_cast<unsigned int>(Qt::NoModifier) 
                            << static_cast<unsigned int>(EKeyLeftCtrl)
                            << static_cast<unsigned int>(0)
                            << static_cast<unsigned int>(0)
                            << static_cast<long int>(12)
                            << true
                            << static_cast<unsigned int>(EKeyRightCtrl);
                            
    QTest::newRow("meta_keyR") << static_cast<unsigned int>(Qt::Key_Super_R) 
                            << static_cast<unsigned int>(Qt::NoModifier)
                            << static_cast<unsigned int>(Qt::NoModifier) 
                            << static_cast<unsigned int>(EKeyRightFunc)
                            << static_cast<unsigned int>(0)
                            << static_cast<unsigned int>(0)
                            << static_cast<long int>(12)
                            << false
                            << static_cast<unsigned int>(0);
    QTest::newRow("meta_keyL") << static_cast<unsigned int>(Qt::Key_Super_L) 
                            << static_cast<unsigned int>(Qt::NoModifier)
                            << static_cast<unsigned int>(Qt::NoModifier) 
                            << static_cast<unsigned int>(EKeyLeftFunc)
                            << static_cast<unsigned int>(0)
                            << static_cast<unsigned int>(0)
                            << static_cast<long int>(12)
                            << false
                            << static_cast<unsigned int>(0);
}

void TestXQKeyCapture::testCaptureKey()
{
    numOfArgs = 3;
    actionType = WGATCaptureKey;
    additionalResult = false;
    ignoreWindowGroupAction = false;
    
    QFETCH(unsigned int, qtKey);
    QFETCH(unsigned int, qtMask);
    QFETCH(unsigned int, qtModifier);

    QFETCH(unsigned int, symbianKey);
    QFETCH(unsigned int, symbianMask);
    QFETCH(unsigned int, symbianModifier);

    QFETCH(long int, reqNum);
    
    QFETCH(bool, additional);
    QFETCH(unsigned int, additionalSymbianKey);
    
    willBeAdditionalRequest = additional;
    
    results.clear();
    results << symbianKey << symbianMask << symbianModifier;
    
    additionalResults.clear();
    additionalResults << additionalSymbianKey << symbianMask << symbianModifier;
    
    MyTestWindowGroup::Instance()->setRequestNumber(reqNum);

    keyCapture->captureKey(static_cast<Qt::Key>(qtKey), Qt::KeyboardModifier(qtMask), Qt::KeyboardModifier(qtModifier));
}

void TestXQKeyCapture::testCaptureKeyList_data() 
{
    testCaptureKey_data();
}

void TestXQKeyCapture::testCaptureKeyList()
{
    numOfArgs = 3;
    actionType = WGATCaptureKey;
    additionalResult = false;
    ignoreWindowGroupAction = false;
    
    QFETCH(unsigned int, qtKey);
    QFETCH(unsigned int, qtMask);
    QFETCH(unsigned int, qtModifier);

    QFETCH(unsigned int, symbianKey);
    QFETCH(unsigned int, symbianMask);
    QFETCH(unsigned int, symbianModifier);

    QFETCH(long int, reqNum);
    
    QFETCH(bool, additional);
    QFETCH(unsigned int, additionalSymbianKey);
    
    willBeAdditionalRequest = additional;
    
    results.clear();
    results << symbianKey << symbianMask << symbianModifier;
    
    additionalResults.clear();
    additionalResults << additionalSymbianKey << symbianMask << symbianModifier;
    
    MyTestWindowGroup::Instance()->setRequestNumber(reqNum);

    keyCapture->captureKey(QList<Qt::Key>() << static_cast<Qt::Key>(qtKey), Qt::KeyboardModifier(qtMask), Qt::KeyboardModifier(qtModifier));
}

////////////////////////////////////////////////////////////////
//Capture Key Up And Downs
////////////////////////////////////////////////////////////////

void TestXQKeyCapture::testCaptureKey_S60_data()
{
    QTest::addColumn<unsigned int>("s60Key");
    QTest::addColumn<unsigned int>("qtMask");
    QTest::addColumn<unsigned int>("qtModifier");

    QTest::addColumn<unsigned int>("symbianKey");
    QTest::addColumn<unsigned int>("symbianMask");
    QTest::addColumn<unsigned int>("symbianModifier");
        
    QTest::addColumn<long int>("reqNum");
    
    QTest::addColumn<bool>("additional");
    QTest::addColumn<unsigned int>("additionalSymbianKey");
    
    QTest::newRow("esc_key") << static_cast<unsigned int>(EKeyEscape) 
                            << static_cast<unsigned int>(Qt::NoModifier)
                            << static_cast<unsigned int>(Qt::NoModifier) 
                            << static_cast<unsigned int>(EKeyEscape)
                            << static_cast<unsigned int>(0)
                            << static_cast<unsigned int>(0)
                            << static_cast<long int>(12)
                            << false
                            << static_cast<unsigned int>(0);

    QTest::newRow("esc_key_not_supported") << static_cast<unsigned int>(EKeyEscape) 
                            << static_cast<unsigned int>(Qt::NoModifier)
                            << static_cast<unsigned int>(Qt::NoModifier) 
                            << static_cast<unsigned int>(EKeyEscape)
                            << static_cast<unsigned int>(0)
                            << static_cast<unsigned int>(0)
                            << static_cast<long int>(KErrNotSupported)
                            << false
                            << static_cast<unsigned int>(0);
    
    QTest::newRow("esc_key_modifiers") << static_cast<unsigned int>(EKeyEscape) 
                            << static_cast<unsigned int>(Qt::ShiftModifier | Qt::ControlModifier | Qt::AltModifier | Qt:: KeypadModifier)
                            << static_cast<unsigned int>(Qt::ShiftModifier | Qt::ControlModifier | Qt::AltModifier | Qt:: KeypadModifier) 
                            << static_cast<unsigned int>(EKeyEscape)
                            << static_cast<unsigned int>(EModifierShift | EModifierCtrl | EModifierAlt | EModifierKeypad)
                            << static_cast<unsigned int>(EModifierShift | EModifierCtrl | EModifierAlt | EModifierKeypad)
                            << static_cast<long int>(13)
                            << false
                            << static_cast<unsigned int>(0);
    
    QTest::newRow("shift_key") << static_cast<unsigned int>(EKeyLeftShift) 
                            << static_cast<unsigned int>(Qt::ShiftModifier)
                            << static_cast<unsigned int>(Qt::ShiftModifier) 
                            << static_cast<unsigned int>(EKeyLeftShift)
                            << static_cast<unsigned int>(EModifierShift)
                            << static_cast<unsigned int>(EModifierShift)
                            << static_cast<long int>(12)
                            << true
                            << static_cast<unsigned int>(EKeyRightShift);

    QTest::newRow("control_key") << static_cast<unsigned int>(EKeyRightCtrl) 
                            << static_cast<unsigned int>(Qt::NoModifier)
                            << static_cast<unsigned int>(Qt::NoModifier) 
                            << static_cast<unsigned int>(EKeyRightCtrl)
                            << static_cast<unsigned int>(0)
                            << static_cast<unsigned int>(0)
                            << static_cast<long int>(12)
                            << false
                            << static_cast<unsigned int>(0);
                            
    QTest::newRow("meta_key") << static_cast<unsigned int>(EKeyRightFunc) 
                            << static_cast<unsigned int>(Qt::NoModifier)
                            << static_cast<unsigned int>(Qt::NoModifier) 
                            << static_cast<unsigned int>(EKeyRightFunc)
                            << static_cast<unsigned int>(0)
                            << static_cast<unsigned int>(0)
                            << static_cast<long int>(12)
                            << false
                            << static_cast<unsigned int>(0);
    QTest::newRow("meta_keyL") << static_cast<unsigned int>(EKeyLeftFunc) 
                            << static_cast<unsigned int>(Qt::NoModifier)
                            << static_cast<unsigned int>(Qt::NoModifier) 
                            << static_cast<unsigned int>(EKeyLeftFunc)
                            << static_cast<unsigned int>(0)
                            << static_cast<unsigned int>(0)
                            << static_cast<long int>(12)
                            << false
                            << static_cast<unsigned int>(0);

}

void TestXQKeyCapture::testCaptureKey_S60()
{
    numOfArgs = 3;
    actionType = WGATCaptureKey;
    additionalResult = false;
    ignoreWindowGroupAction = false;
    
    QFETCH(unsigned int, s60Key);
    QFETCH(unsigned int, qtMask);
    QFETCH(unsigned int, qtModifier);

    QFETCH(unsigned int, symbianKey);
    QFETCH(unsigned int, symbianMask);
    QFETCH(unsigned int, symbianModifier);

    QFETCH(long int, reqNum);
    
    QFETCH(bool, additional);
    QFETCH(unsigned int, additionalSymbianKey);
    
    willBeAdditionalRequest = additional;
    
    results.clear();
    results << symbianKey << symbianMask << symbianModifier;
    
    additionalResults.clear();
    additionalResults << additionalSymbianKey << symbianMask << symbianModifier;
    
    MyTestWindowGroup::Instance()->setRequestNumber(reqNum);
    
    keyCapture->captureKey(static_cast<TUint>(s60Key), Qt::KeyboardModifier(qtMask), Qt::KeyboardModifier(qtModifier));
}

void TestXQKeyCapture::testCaptureKeyList_S60_data()
{
    testCaptureKey_S60_data();
}

void TestXQKeyCapture::testCaptureKeyList_S60()
{
    numOfArgs = 3;
    actionType = WGATCaptureKey;
    additionalResult = false;
    ignoreWindowGroupAction = false;
    
    QFETCH(unsigned int, s60Key);
    QFETCH(unsigned int, qtMask);
    QFETCH(unsigned int, qtModifier);

    QFETCH(unsigned int, symbianKey);
    QFETCH(unsigned int, symbianMask);
    QFETCH(unsigned int, symbianModifier);

    QFETCH(long int, reqNum);
    
    QFETCH(bool, additional);
    QFETCH(unsigned int, additionalSymbianKey);
    
    willBeAdditionalRequest = additional;
    
    results.clear();
    results << symbianKey << symbianMask << symbianModifier;
    
    additionalResults.clear();
    additionalResults << additionalSymbianKey << symbianMask << symbianModifier;
    
    MyTestWindowGroup::Instance()->setRequestNumber(reqNum);
    keyCapture->captureKey(QList<TUint>() << static_cast<TUint>(s60Key), Qt::KeyboardModifier(qtMask), Qt::KeyboardModifier(qtModifier));
}

////////////////////////////////////////////////////////////////
//Capture Key Up And Downs
////////////////////////////////////////////////////////////////
void TestXQKeyCapture::testCaptureKeyUpAndDowns_data()
{
    QTest::addColumn<unsigned int>("qtKey");
    QTest::addColumn<unsigned int>("qtMask");
    QTest::addColumn<unsigned int>("qtModifier");

    QTest::addColumn<unsigned int>("symbianKey");
    QTest::addColumn<unsigned int>("symbianMask");
    QTest::addColumn<unsigned int>("symbianModifier");
        
    QTest::addColumn<long int>("reqNum");
    
    QTest::addColumn<bool>("additional");
    QTest::addColumn<unsigned int>("additionalSymbianKey");
    
    QTest::newRow("esc_key") << static_cast<unsigned int>(Qt::Key_Escape) 
                            << static_cast<unsigned int>(Qt::NoModifier)
                            << static_cast<unsigned int>(Qt::NoModifier) 
                            << static_cast<unsigned int>(EStdKeyEscape)
                            << static_cast<unsigned int>(0)
                            << static_cast<unsigned int>(0)
                            << static_cast<long int>(12)
                            << false
                            << static_cast<unsigned int>(0);
}

void TestXQKeyCapture::testCaptureKeyUpAndDowns()
{
    numOfArgs = 3;
    actionType = WGATCaptureKeyUpAndDowns;
    additionalResult = false;
    ignoreWindowGroupAction = false;
    
    QFETCH(unsigned int, qtKey);
    QFETCH(unsigned int, qtMask);
    QFETCH(unsigned int, qtModifier);

    QFETCH(unsigned int, symbianKey);
    QFETCH(unsigned int, symbianMask);
    QFETCH(unsigned int, symbianModifier);

    QFETCH(long int, reqNum);
    
    QFETCH(bool, additional);
    QFETCH(unsigned int, additionalSymbianKey);
    
    willBeAdditionalRequest = additional;
    
    results.clear();
    results << symbianKey << symbianMask << symbianModifier;
    
    additionalResults.clear();
    additionalResults << additionalSymbianKey << symbianMask << symbianModifier;
    
    MyTestWindowGroup::Instance()->setRequestNumber(reqNum);
    keyCapture->captureKeyUpAndDowns(static_cast<Qt::Key>(qtKey), Qt::KeyboardModifier(qtMask), Qt::KeyboardModifier(qtModifier));
}

void TestXQKeyCapture::testCaptureKeyUpAndDownsList_data()
{
    testCaptureKeyUpAndDowns_data();
}

void TestXQKeyCapture::testCaptureKeyUpAndDownsList()
{
    numOfArgs = 3;
    actionType = WGATCaptureKeyUpAndDowns;
    additionalResult = false;
    ignoreWindowGroupAction = false;
    
    QFETCH(unsigned int, qtKey);
    QFETCH(unsigned int, qtMask);
    QFETCH(unsigned int, qtModifier);

    QFETCH(unsigned int, symbianKey);
    QFETCH(unsigned int, symbianMask);
    QFETCH(unsigned int, symbianModifier);

    QFETCH(long int, reqNum);
    
    QFETCH(bool, additional);
    QFETCH(unsigned int, additionalSymbianKey);
    
    willBeAdditionalRequest = additional;
    
    results.clear();
    results << symbianKey << symbianMask << symbianModifier;
    
    additionalResults.clear();
    additionalResults << additionalSymbianKey << symbianMask << symbianModifier;
    
    MyTestWindowGroup::Instance()->setRequestNumber(reqNum);
    keyCapture->captureKeyUpAndDowns(QList<Qt::Key>() << static_cast<Qt::Key>(qtKey), Qt::KeyboardModifier(qtMask), Qt::KeyboardModifier(qtModifier));
}

////////////////////////////////////////////////////////////////
//Capture Key Up And Downs
////////////////////////////////////////////////////////////////
void TestXQKeyCapture::testCaptureKeyUpAndDowns_S60_data()
{
    QTest::addColumn<unsigned int>("qtKey");
    QTest::addColumn<unsigned int>("qtMask");
    QTest::addColumn<unsigned int>("qtModifier");

    QTest::addColumn<unsigned int>("symbianKey");
    QTest::addColumn<unsigned int>("symbianMask");
    QTest::addColumn<unsigned int>("symbianModifier");
        
    QTest::addColumn<long int>("reqNum");
    
    QTest::addColumn<bool>("additional");
    QTest::addColumn<unsigned int>("additionalSymbianKey");
    
    QTest::newRow("esc_key") << static_cast<unsigned int>(EStdKeyEscape) 
                            << static_cast<unsigned int>(Qt::NoModifier)
                            << static_cast<unsigned int>(Qt::NoModifier) 
                            << static_cast<unsigned int>(EStdKeyEscape)
                            << static_cast<unsigned int>(0)
                            << static_cast<unsigned int>(0)
                            << static_cast<long int>(12)
                            << false
                            << static_cast<unsigned int>(0);
}

void TestXQKeyCapture::testCaptureKeyUpAndDowns_S60()
{
    numOfArgs = 3;
    actionType = WGATCaptureKeyUpAndDowns;
    additionalResult = false;
    ignoreWindowGroupAction = false;
    
    QFETCH(unsigned int, qtKey);
    QFETCH(unsigned int, qtMask);
    QFETCH(unsigned int, qtModifier);

    QFETCH(unsigned int, symbianKey);
    QFETCH(unsigned int, symbianMask);
    QFETCH(unsigned int, symbianModifier);

    QFETCH(long int, reqNum);
    
    QFETCH(bool, additional);
    QFETCH(unsigned int, additionalSymbianKey);
    
    willBeAdditionalRequest = additional;
    
    results.clear();
    results << symbianKey << symbianMask << symbianModifier;
    
    additionalResults.clear();
    additionalResults << additionalSymbianKey << symbianMask << symbianModifier;
    
    MyTestWindowGroup::Instance()->setRequestNumber(reqNum);
    keyCapture->captureKeyUpAndDowns(static_cast<TUint>(qtKey), Qt::KeyboardModifier(qtMask), Qt::KeyboardModifier(qtModifier));
}

void TestXQKeyCapture::testCaptureKeyUpAndDownsList_S60_data()
{
    testCaptureKeyUpAndDowns_S60_data();
}

void TestXQKeyCapture::testCaptureKeyUpAndDownsList_S60()
{
    numOfArgs = 3;
    actionType = WGATCaptureKeyUpAndDowns;
    additionalResult = false;
    ignoreWindowGroupAction = false;
    
    QFETCH(unsigned int, qtKey);
    QFETCH(unsigned int, qtMask);
    QFETCH(unsigned int, qtModifier);

    QFETCH(unsigned int, symbianKey);
    QFETCH(unsigned int, symbianMask);
    QFETCH(unsigned int, symbianModifier);

    QFETCH(long int, reqNum);
    
    QFETCH(bool, additional);
    QFETCH(unsigned int, additionalSymbianKey);
    
    willBeAdditionalRequest = additional;
    
    results.clear();
    results << symbianKey << symbianMask << symbianModifier;
    
    additionalResults.clear();
    additionalResults << additionalSymbianKey << symbianMask << symbianModifier;
    
    MyTestWindowGroup::Instance()->setRequestNumber(reqNum);
    keyCapture->captureKeyUpAndDowns(QList<TUint>() << static_cast<TUint>(qtKey), Qt::KeyboardModifier(qtMask), Qt::KeyboardModifier(qtModifier));
}

////////////////////////////////////////////////////////////////
//Capture Long Key
////////////////////////////////////////////////////////////////
void TestXQKeyCapture::testCaptureLongKey_data()
{
    QTest::addColumn<unsigned int>("qtKey");
    QTest::addColumn<unsigned int>("qtMask");
    QTest::addColumn<unsigned int>("qtModifier");
    QTest::addColumn<int>("longFlags");
    
    QTest::addColumn<unsigned int>("symbianKey");
    QTest::addColumn<unsigned int>("symbianMask");
    QTest::addColumn<unsigned int>("symbianModifier");
    QTest::addColumn<int>("symbianPriority");
    QTest::addColumn<int>("symbianLongFlags");
        
    QTest::addColumn<long int>("reqNum");
    
    QTest::addColumn<bool>("additional");
    QTest::addColumn<unsigned int>("additionalSymbianKey");
    
    
    QTest::newRow("esc_key") << static_cast<unsigned int>(Qt::Key_Escape) 
                            << static_cast<unsigned int>(Qt::NoModifier)
                            << static_cast<unsigned int>(Qt::NoModifier) 
                            << static_cast<int>(XQKeyCapture::LongNormal) 
                            << static_cast<unsigned int>(EKeyEscape)
                            << static_cast<unsigned int>(0)
                            << static_cast<unsigned int>(0)
                            << static_cast<int>(0) //priority
                            << static_cast<int>(XQKeyCapture::LongNormal) 
                            << static_cast<long int>(12)
                            << false
                            << static_cast<unsigned int>(0);

}

void TestXQKeyCapture::testCaptureLongKey()
{
    numOfArgs = 6;
    actionType = WGATCaptureLongKey;
    additionalResult = false;
    ignoreWindowGroupAction = false;
    
    QFETCH(unsigned int, qtKey);
    QFETCH(unsigned int, qtMask);
    QFETCH(unsigned int, qtModifier);
    QFETCH(int, longFlags);
    
    QFETCH(unsigned int, symbianKey);
    QFETCH(unsigned int, symbianMask);
    QFETCH(unsigned int, symbianModifier);
    QFETCH(int, symbianPriority);
    QFETCH(int, symbianLongFlags);
    
    QFETCH(long int, reqNum);
    
    QFETCH(bool, additional);
    QFETCH(unsigned int, additionalSymbianKey);
    
    willBeAdditionalRequest = additional;
    
    results.clear();
    results << symbianKey << symbianKey << symbianMask << symbianModifier << symbianPriority << symbianLongFlags;
    
    additionalResults.clear();
    additionalResults << additionalSymbianKey << symbianMask << symbianModifier << symbianPriority << symbianLongFlags;
    
    MyTestWindowGroup::Instance()->setRequestNumber(reqNum);
    keyCapture->captureLongKey(static_cast<Qt::Key>(qtKey), Qt::KeyboardModifier(qtMask), Qt::KeyboardModifier(qtModifier), static_cast<XQKeyCapture::LongFlags>(longFlags));
}

void TestXQKeyCapture::testCaptureLongKeyList_data()
{
    testCaptureLongKey_data();
}

void TestXQKeyCapture::testCaptureLongKeyList()
{
    numOfArgs = 6;
    actionType = WGATCaptureLongKey;
    additionalResult = false;
    ignoreWindowGroupAction = false;
    
    QFETCH(unsigned int, qtKey);
    QFETCH(unsigned int, qtMask);
    QFETCH(unsigned int, qtModifier);
    QFETCH(int, longFlags);
    
    QFETCH(unsigned int, symbianKey);
    QFETCH(unsigned int, symbianMask);
    QFETCH(unsigned int, symbianModifier);
    QFETCH(int, symbianPriority);
    QFETCH(int, symbianLongFlags);
    
    QFETCH(long int, reqNum);
    
    QFETCH(bool, additional);
    QFETCH(unsigned int, additionalSymbianKey);
    
    willBeAdditionalRequest = additional;
    
    results.clear();
    results << symbianKey << symbianKey << symbianMask << symbianModifier << symbianPriority << symbianLongFlags;
    
    additionalResults.clear();
    additionalResults << additionalSymbianKey << symbianMask << symbianModifier << symbianPriority << symbianLongFlags;
    
    MyTestWindowGroup::Instance()->setRequestNumber(reqNum);
    keyCapture->captureLongKey(QList<Qt::Key>() << static_cast<Qt::Key>(qtKey), Qt::KeyboardModifier(qtMask), Qt::KeyboardModifier(qtModifier), static_cast<XQKeyCapture::LongFlags>(longFlags));
}

////////////////////////////////////////////////////////////////
//Capture Long Key
////////////////////////////////////////////////////////////////
void TestXQKeyCapture::testCaptureLongKey_S60_data()
{
    QTest::addColumn<unsigned int>("qtKey");
    QTest::addColumn<unsigned int>("qtMask");
    QTest::addColumn<unsigned int>("qtModifier");
    QTest::addColumn<int>("longFlags");
    
    QTest::addColumn<unsigned int>("symbianKey");
    QTest::addColumn<unsigned int>("symbianMask");
    QTest::addColumn<unsigned int>("symbianModifier");
    QTest::addColumn<int>("symbianPriority");
    QTest::addColumn<int>("symbianLongFlags");
        
    QTest::addColumn<long int>("reqNum");
    
    QTest::addColumn<bool>("additional");
    QTest::addColumn<unsigned int>("additionalSymbianKey");
    
    
    QTest::newRow("esc_key") << static_cast<unsigned int>(EKeyEscape) 
                            << static_cast<unsigned int>(Qt::NoModifier)
                            << static_cast<unsigned int>(Qt::NoModifier) 
                            << static_cast<int>(XQKeyCapture::LongNormal) 
                            << static_cast<unsigned int>(EKeyEscape)
                            << static_cast<unsigned int>(0)
                            << static_cast<unsigned int>(0)
                            << static_cast<int>(0) //priority
                            << static_cast<int>(XQKeyCapture::LongNormal) 
                            << static_cast<long int>(12)
                            << false
                            << static_cast<unsigned int>(0);

}

void TestXQKeyCapture::testCaptureLongKey_S60()
{
    numOfArgs = 6;
    actionType = WGATCaptureLongKey;
    additionalResult = false;
    ignoreWindowGroupAction = false;
    
    QFETCH(unsigned int, qtKey);
    QFETCH(unsigned int, qtMask);
    QFETCH(unsigned int, qtModifier);
    QFETCH(int, longFlags);
    
    QFETCH(unsigned int, symbianKey);
    QFETCH(unsigned int, symbianMask);
    QFETCH(unsigned int, symbianModifier);
    QFETCH(int, symbianPriority);
    QFETCH(int, symbianLongFlags);
    
    QFETCH(long int, reqNum);
    
    QFETCH(bool, additional);
    QFETCH(unsigned int, additionalSymbianKey);
    
    willBeAdditionalRequest = additional;
    
    results.clear();
    results << symbianKey << symbianKey << symbianMask << symbianModifier << symbianPriority << symbianLongFlags;
    
    additionalResults.clear();
    additionalResults << additionalSymbianKey << symbianMask << symbianModifier << symbianPriority << symbianLongFlags;
    
    MyTestWindowGroup::Instance()->setRequestNumber(reqNum);
    keyCapture->captureLongKey(static_cast<TUint>(qtKey), Qt::KeyboardModifier(qtMask), Qt::KeyboardModifier(qtModifier), static_cast<XQKeyCapture::LongFlags>(longFlags));
}

void TestXQKeyCapture::testCaptureLongKeyList_S60_data()
{
    testCaptureLongKey_S60_data();
}

void TestXQKeyCapture::testCaptureLongKeyList_S60()
{
    numOfArgs = 6;
    actionType = WGATCaptureLongKey;
    additionalResult = false;
    ignoreWindowGroupAction = false;
    
    QFETCH(unsigned int, qtKey);
    QFETCH(unsigned int, qtMask);
    QFETCH(unsigned int, qtModifier);
    QFETCH(int, longFlags);
    
    QFETCH(unsigned int, symbianKey);
    QFETCH(unsigned int, symbianMask);
    QFETCH(unsigned int, symbianModifier);
    QFETCH(int, symbianPriority);
    QFETCH(int, symbianLongFlags);
    
    QFETCH(long int, reqNum);
    
    QFETCH(bool, additional);
    QFETCH(unsigned int, additionalSymbianKey);
    
    willBeAdditionalRequest = additional;
    
    results.clear();
    results << symbianKey << symbianKey << symbianMask << symbianModifier << symbianPriority << symbianLongFlags;
    
    additionalResults.clear();
    additionalResults << additionalSymbianKey << symbianMask << symbianModifier << symbianPriority << symbianLongFlags;
    
    MyTestWindowGroup::Instance()->setRequestNumber(reqNum);
    keyCapture->captureLongKey(QList<TUint>() << static_cast<TUint>(qtKey), Qt::KeyboardModifier(qtMask), Qt::KeyboardModifier(qtModifier), static_cast<XQKeyCapture::LongFlags>(longFlags));
}

////////////////////////////////////////////////////////////////
// CANCEL
//Cancel Capture Key
////////////////////////////////////////////////////////////////
void TestXQKeyCapture::testCancelCaptureKey_data()
{
    QTest::addColumn<unsigned int>("qtKey");
    QTest::addColumn<unsigned int>("qtMask");
    QTest::addColumn<unsigned int>("qtModifier");

    QTest::addColumn<long int>("reqNum");
    
    QTest::addColumn<bool>("additional");
    QTest::addColumn<long int>("additionalReqNum");
    
    QTest::newRow("esc_key") << static_cast<unsigned int>(Qt::Key_Escape) 
                            << static_cast<unsigned int>(Qt::NoModifier)
                            << static_cast<unsigned int>(Qt::NoModifier) 
                            << static_cast<long int>(12)
                            << false
                            << static_cast<long int>(0);

    QTest::newRow("esc_key_not_supported") << static_cast<unsigned int>(Qt::Key_Escape) 
                            << static_cast<unsigned int>(Qt::NoModifier)
                            << static_cast<unsigned int>(Qt::NoModifier) 
                            << static_cast<long int>(KErrNotSupported)
                            << false
                            << static_cast<long int>(0);
    
    QTest::newRow("esc_key_modifiers") << static_cast<unsigned int>(Qt::Key_Escape) 
                            << static_cast<unsigned int>(Qt::ShiftModifier | Qt::ControlModifier | Qt::AltModifier | Qt:: KeypadModifier)
                            << static_cast<unsigned int>(Qt::ShiftModifier | Qt::ControlModifier | Qt::AltModifier | Qt:: KeypadModifier) 
                            << static_cast<long int>(13)
                            << false
                            << static_cast<long int>(0);
    
    QTest::newRow("shift_key") << static_cast<unsigned int>(Qt::Key_Shift) 
                            << static_cast<unsigned int>(Qt::ShiftModifier)
                            << static_cast<unsigned int>(Qt::ControlModifier) 
                            << static_cast<long int>(15)
                            << true
                            << static_cast<long int>(16);

    QTest::newRow("control_key") << static_cast<unsigned int>(Qt::Key_Control) 
                            << static_cast<unsigned int>(Qt::ControlModifier)
                            << static_cast<unsigned int>(Qt::ShiftModifier) 
                            << static_cast<long int>(17)
                            << true
                            << static_cast<long int>(18);
                            
    QTest::newRow("meta_key") << static_cast<unsigned int>(Qt::Key_Meta) 
                            << static_cast<unsigned int>(Qt::NoModifier)
                            << static_cast<unsigned int>(Qt::NoModifier) 
                            << static_cast<long int>(19)
                            << true
                            << static_cast<long int>(20);


}

void TestXQKeyCapture::testCancelCaptureKey()
{
    numOfArgs = 1;
    actionType = WGATCancelCaptureKey;
    additionalResult = false;
    
    QFETCH(unsigned int, qtKey);
    QFETCH(unsigned int, qtMask);
    QFETCH(unsigned int, qtModifier);

    QFETCH(long int, reqNum);
    
    QFETCH(bool, additional);
    QFETCH(long int, additionalReqNum);
    
    willBeAdditionalRequest = additional;
    additionalRequestHandle = additionalReqNum;
    
    cancelResults.clear();
    cancelResults << reqNum;
    
    cancelAdditionalResults.clear();
    cancelAdditionalResults << additionalReqNum;
    
    MyTestWindowGroup::Instance()->setRequestNumber(reqNum);
    ignoreWindowGroupAction = true;
    keyCapture->captureKey(static_cast<Qt::Key>(qtKey), Qt::KeyboardModifier(qtMask), Qt::KeyboardModifier(qtModifier));
    ignoreWindowGroupAction = false;
    willBeAdditionalRequest = additional;
    keyCapture->cancelCaptureKey(static_cast<Qt::Key>(qtKey), Qt::KeyboardModifier(qtMask), Qt::KeyboardModifier(qtModifier));
}

void TestXQKeyCapture::testCancelCaptureKeyList_data()
{
    testCancelCaptureKey_data();
}

void TestXQKeyCapture::testCancelCaptureKeyList()
{
    numOfArgs = 1;
    actionType = WGATCancelCaptureKey;
    additionalResult = false;
    
    QFETCH(unsigned int, qtKey);
    QFETCH(unsigned int, qtMask);
    QFETCH(unsigned int, qtModifier);

    QFETCH(long int, reqNum);
    
    QFETCH(bool, additional);
    QFETCH(long int, additionalReqNum);
    
    willBeAdditionalRequest = additional;
    additionalRequestHandle = additionalReqNum;
    
    cancelResults.clear();
    cancelResults << reqNum;
    
    cancelAdditionalResults.clear();
    cancelAdditionalResults << additionalReqNum;
    
    MyTestWindowGroup::Instance()->setRequestNumber(reqNum);
    ignoreWindowGroupAction = true;
    keyCapture->captureKey(static_cast<Qt::Key>(qtKey), Qt::KeyboardModifier(qtMask), Qt::KeyboardModifier(qtModifier));
    ignoreWindowGroupAction = false;
    willBeAdditionalRequest = additional;
    keyCapture->cancelCaptureKey(QList<Qt::Key>() << static_cast<Qt::Key>(qtKey), Qt::KeyboardModifier(qtMask), Qt::KeyboardModifier(qtModifier));
}

////////////////////////////////////////////////////////////////
// CANCEL
//Cancel Capture Key
////////////////////////////////////////////////////////////////
void TestXQKeyCapture::testCancelCaptureKey_S60_data()
{
    QTest::addColumn<unsigned int>("qtKey");
    QTest::addColumn<unsigned int>("qtMask");
    QTest::addColumn<unsigned int>("qtModifier");

    QTest::addColumn<long int>("reqNum");
    
    QTest::addColumn<bool>("additional");
    QTest::addColumn<long int>("additionalReqNum");
    
    QTest::newRow("esc_key") << static_cast<unsigned int>(EKeyEscape) 
                            << static_cast<unsigned int>(Qt::NoModifier)
                            << static_cast<unsigned int>(Qt::NoModifier) 
                            << static_cast<long int>(12)
                            << false
                            << static_cast<long int>(0);

    QTest::newRow("esc_key_not_supported") << static_cast<unsigned int>(EKeyEscape) 
                            << static_cast<unsigned int>(Qt::NoModifier)
                            << static_cast<unsigned int>(Qt::NoModifier) 
                            << static_cast<long int>(KErrNotSupported)
                            << false
                            << static_cast<long int>(0);
    
    QTest::newRow("esc_key_modifiers") << static_cast<unsigned int>(EKeyEscape) 
                            << static_cast<unsigned int>(Qt::ShiftModifier | Qt::ControlModifier | Qt::AltModifier | Qt:: KeypadModifier)
                            << static_cast<unsigned int>(Qt::ShiftModifier | Qt::ControlModifier | Qt::AltModifier | Qt:: KeypadModifier) 
                            << static_cast<long int>(13)
                            << false
                            << static_cast<long int>(0);
    
    QTest::newRow("shift_key") << static_cast<unsigned int>(EKeyLeftShift) 
                            << static_cast<unsigned int>(Qt::ShiftModifier)
                            << static_cast<unsigned int>(Qt::ControlModifier) 
                            << static_cast<long int>(15)
                            << true
                            << static_cast<long int>(16);

    QTest::newRow("control_key") << static_cast<unsigned int>(EKeyLeftCtrl) 
                            << static_cast<unsigned int>(Qt::ControlModifier)
                            << static_cast<unsigned int>(Qt::ShiftModifier) 
                            << static_cast<long int>(17)
                            << true
                            << static_cast<long int>(18);
                            
    QTest::newRow("meta_key") << static_cast<unsigned int>(EKeyRightCtrl) 
                            << static_cast<unsigned int>(Qt::NoModifier)
                            << static_cast<unsigned int>(Qt::NoModifier) 
                            << static_cast<long int>(19)
                            << true
                            << static_cast<long int>(20);
}

void TestXQKeyCapture::testCancelCaptureKey_S60()
{
    numOfArgs = 1;
    actionType = WGATCancelCaptureKey;
    additionalResult = false;
    
    QFETCH(unsigned int, qtKey);
    QFETCH(unsigned int, qtMask);
    QFETCH(unsigned int, qtModifier);

    QFETCH(long int, reqNum);
    
    QFETCH(bool, additional);
    QFETCH(long int, additionalReqNum);
    
    willBeAdditionalRequest = additional;
    additionalRequestHandle = additionalReqNum;
    
    cancelResults.clear();
    cancelResults << reqNum;
    
    cancelAdditionalResults.clear();
    cancelAdditionalResults << additionalReqNum;
    
    MyTestWindowGroup::Instance()->setRequestNumber(reqNum);
    ignoreWindowGroupAction = true;
    keyCapture->captureKey(static_cast<TUint>(qtKey), Qt::KeyboardModifier(qtMask), Qt::KeyboardModifier(qtModifier));
    ignoreWindowGroupAction = false;
    willBeAdditionalRequest = additional;
    keyCapture->cancelCaptureKey(static_cast<TUint>(qtKey), Qt::KeyboardModifier(qtMask), Qt::KeyboardModifier(qtModifier));
}

void TestXQKeyCapture::testCancelCaptureKeyList_S60_data()
{
    testCancelCaptureKey_S60_data();
}

void TestXQKeyCapture::testCancelCaptureKeyList_S60()
{
    numOfArgs = 1;
    actionType = WGATCancelCaptureKey;
    additionalResult = false;
    
    QFETCH(unsigned int, qtKey);
    QFETCH(unsigned int, qtMask);
    QFETCH(unsigned int, qtModifier);

    QFETCH(long int, reqNum);
    
    QFETCH(bool, additional);
    QFETCH(long int, additionalReqNum);
    
    willBeAdditionalRequest = additional;
    additionalRequestHandle = additionalReqNum;
    
    cancelResults.clear();
    cancelResults << reqNum;
    
    cancelAdditionalResults.clear();
    cancelAdditionalResults << additionalReqNum;
    
    MyTestWindowGroup::Instance()->setRequestNumber(reqNum);
    ignoreWindowGroupAction = true;
    keyCapture->captureKey(static_cast<TUint>(qtKey), Qt::KeyboardModifier(qtMask), Qt::KeyboardModifier(qtModifier));
    ignoreWindowGroupAction = false;
    willBeAdditionalRequest = additional;
    keyCapture->cancelCaptureKey(QList<TUint>() << static_cast<TUint>(qtKey), Qt::KeyboardModifier(qtMask), Qt::KeyboardModifier(qtModifier));
}

////////////////////////////////////////////////////////////////
//Cancel Capture Key Up And Downs
////////////////////////////////////////////////////////////////
void TestXQKeyCapture::testCancelCaptureKeyUpAndDowns_data()
{
    QTest::addColumn<unsigned int>("qtKey");
    QTest::addColumn<unsigned int>("qtMask");
    QTest::addColumn<unsigned int>("qtModifier");

    QTest::addColumn<long int>("reqNum");
    
    QTest::addColumn<bool>("additional");
    QTest::addColumn<long int>("additionalReqNum");
    
    QTest::newRow("esc_key") << static_cast<unsigned int>(Qt::Key_Escape) 
                            << static_cast<unsigned int>(Qt::NoModifier)
                            << static_cast<unsigned int>(Qt::NoModifier) 
                            << static_cast<long int>(34)
                            << false
                            << static_cast<long int>(35);

}

void TestXQKeyCapture::testCancelCaptureKeyUpAndDowns()
{
    numOfArgs = 1;
    actionType = WGATCancelCaptureKeyUpAndDowns;
    additionalResult = false;
    
    QFETCH(unsigned int, qtKey);
    QFETCH(unsigned int, qtMask);
    QFETCH(unsigned int, qtModifier);

    QFETCH(long int, reqNum);
    
    QFETCH(bool, additional);
    QFETCH(long int, additionalReqNum);
    
    willBeAdditionalRequest = additional;
    additionalRequestHandle = additionalReqNum;
    
    cancelResults.clear();
    cancelResults << reqNum;
    
    cancelAdditionalResults.clear();
    cancelAdditionalResults << additionalReqNum;
    
    MyTestWindowGroup::Instance()->setRequestNumber(reqNum);
    ignoreWindowGroupAction = true;
    keyCapture->captureKeyUpAndDowns(static_cast<Qt::Key>(qtKey), Qt::KeyboardModifier(qtMask), Qt::KeyboardModifier(qtModifier));
    ignoreWindowGroupAction = false;
    willBeAdditionalRequest = additional;
    keyCapture->cancelCaptureKeyUpAndDowns(static_cast<Qt::Key>(qtKey), Qt::KeyboardModifier(qtMask), Qt::KeyboardModifier(qtModifier));
}

void TestXQKeyCapture::testCancelCaptureKeyUpAndDownsList_data()
{
    testCancelCaptureKeyUpAndDowns_data();
}

void TestXQKeyCapture::testCancelCaptureKeyUpAndDownsList()
{
    numOfArgs = 1;
    actionType = WGATCancelCaptureKeyUpAndDowns;
    additionalResult = false;
    
    QFETCH(unsigned int, qtKey);
    QFETCH(unsigned int, qtMask);
    QFETCH(unsigned int, qtModifier);

    QFETCH(long int, reqNum);
    
    QFETCH(bool, additional);
    QFETCH(long int, additionalReqNum);
    
    willBeAdditionalRequest = additional;
    additionalRequestHandle = additionalReqNum;
    
    cancelResults.clear();
    cancelResults << reqNum;
    
    cancelAdditionalResults.clear();
    cancelAdditionalResults << additionalReqNum;
    
    MyTestWindowGroup::Instance()->setRequestNumber(reqNum);
    ignoreWindowGroupAction = true;
    keyCapture->captureKeyUpAndDowns(static_cast<Qt::Key>(qtKey), Qt::KeyboardModifier(qtMask), Qt::KeyboardModifier(qtModifier));
    ignoreWindowGroupAction = false;
    willBeAdditionalRequest = additional;
    keyCapture->cancelCaptureKeyUpAndDowns(QList<Qt::Key>() << static_cast<Qt::Key>(qtKey), Qt::KeyboardModifier(qtMask), Qt::KeyboardModifier(qtModifier));
}

////////////////////////////////////////////////////////////////
//Cancel Capture Key Up And Downs
////////////////////////////////////////////////////////////////
void TestXQKeyCapture::testCancelCaptureKeyUpAndDowns_S60_data()
{
    QTest::addColumn<unsigned int>("qtKey");
    QTest::addColumn<unsigned int>("qtMask");
    QTest::addColumn<unsigned int>("qtModifier");

    QTest::addColumn<long int>("reqNum");
    
    QTest::addColumn<bool>("additional");
    QTest::addColumn<long int>("additionalReqNum");
    
    QTest::newRow("esc_key") << static_cast<unsigned int>(EKeyEscape) 
                            << static_cast<unsigned int>(Qt::NoModifier)
                            << static_cast<unsigned int>(Qt::NoModifier) 
                            << static_cast<long int>(34)
                            << false
                            << static_cast<long int>(35);

}

void TestXQKeyCapture::testCancelCaptureKeyUpAndDowns_S60()
{
    numOfArgs = 1;
    actionType = WGATCancelCaptureKeyUpAndDowns;
    additionalResult = false;
    
    QFETCH(unsigned int, qtKey);
    QFETCH(unsigned int, qtMask);
    QFETCH(unsigned int, qtModifier);

    QFETCH(long int, reqNum);
    
    QFETCH(bool, additional);
    QFETCH(long int, additionalReqNum);
    
    willBeAdditionalRequest = additional;
    additionalRequestHandle = additionalReqNum;
    
    cancelResults.clear();
    cancelResults << reqNum;
    
    cancelAdditionalResults.clear();
    cancelAdditionalResults << additionalReqNum;
    
    MyTestWindowGroup::Instance()->setRequestNumber(reqNum);
    ignoreWindowGroupAction = true;
    keyCapture->captureKeyUpAndDowns(static_cast<TUint>(qtKey), Qt::KeyboardModifier(qtMask), Qt::KeyboardModifier(qtModifier));
    ignoreWindowGroupAction = false;
    willBeAdditionalRequest = additional;
    keyCapture->cancelCaptureKeyUpAndDowns(static_cast<TUint>(qtKey), Qt::KeyboardModifier(qtMask), Qt::KeyboardModifier(qtModifier));
}

void TestXQKeyCapture::testCancelCaptureKeyUpAndDownsList_S60_data()
{
    testCancelCaptureKeyUpAndDowns_S60_data();
}

void TestXQKeyCapture::testCancelCaptureKeyUpAndDownsList_S60()
{
    numOfArgs = 1;
    actionType = WGATCancelCaptureKeyUpAndDowns;
    additionalResult = false;
    
    QFETCH(unsigned int, qtKey);
    QFETCH(unsigned int, qtMask);
    QFETCH(unsigned int, qtModifier);

    QFETCH(long int, reqNum);
    
    QFETCH(bool, additional);
    QFETCH(long int, additionalReqNum);
    
    willBeAdditionalRequest = additional;
    additionalRequestHandle = additionalReqNum;
    
    cancelResults.clear();
    cancelResults << reqNum;
    
    cancelAdditionalResults.clear();
    cancelAdditionalResults << additionalReqNum;
    
    MyTestWindowGroup::Instance()->setRequestNumber(reqNum);
    ignoreWindowGroupAction = true;
    keyCapture->captureKeyUpAndDowns(static_cast<TUint>(qtKey), Qt::KeyboardModifier(qtMask), Qt::KeyboardModifier(qtModifier));
    ignoreWindowGroupAction = false;
    willBeAdditionalRequest = additional;
    keyCapture->cancelCaptureKeyUpAndDowns(QList<TUint>() << static_cast<TUint>(qtKey), Qt::KeyboardModifier(qtMask), Qt::KeyboardModifier(qtModifier));
}

////////////////////////////////////////////////////////////////
//Cancel Capture Long Key
////////////////////////////////////////////////////////////////
void TestXQKeyCapture::testCancelCaptureLongKey_data()
{
    QTest::addColumn<unsigned int>("qtKey");
    QTest::addColumn<unsigned int>("qtMask");
    QTest::addColumn<unsigned int>("qtModifier");
    QTest::addColumn<int>("longFlags");
    
    QTest::addColumn<long int>("reqNum");
    
    QTest::addColumn<bool>("additional");
    QTest::addColumn<long int>("additionalReqNum");
    
    QTest::newRow("esc_key") << static_cast<unsigned int>(Qt::Key_Escape) 
                            << static_cast<unsigned int>(Qt::NoModifier)
                            << static_cast<unsigned int>(Qt::NoModifier)
                            << static_cast<int>(XQKeyCapture::LongNormal) 
                            << static_cast<long int>(22)
                            << false
                            << static_cast<long int>(23);





}

void TestXQKeyCapture::testCancelCaptureLongKey()
{
    numOfArgs = 1;
    actionType = WGATCancelCaptureLongKey;
    additionalResult = false;
    
    QFETCH(unsigned int, qtKey);
    QFETCH(unsigned int, qtMask);
    QFETCH(unsigned int, qtModifier);
    QFETCH(int, longFlags);

    QFETCH(long int, reqNum);
    
    QFETCH(bool, additional);
    QFETCH(long int, additionalReqNum);
    
    willBeAdditionalRequest = additional;
    additionalRequestHandle = additionalReqNum;
    
    cancelResults.clear();
    cancelResults << reqNum;
    
    cancelAdditionalResults.clear();
    cancelAdditionalResults << additionalReqNum;
    
    MyTestWindowGroup::Instance()->setRequestNumber(reqNum);
    ignoreWindowGroupAction = true;
    keyCapture->captureLongKey(static_cast<Qt::Key>(qtKey), Qt::KeyboardModifier(qtMask), Qt::KeyboardModifier(qtModifier), static_cast<XQKeyCapture::LongFlags>(longFlags));
    ignoreWindowGroupAction = false;
    willBeAdditionalRequest = additional;
    keyCapture->cancelCaptureLongKey(static_cast<Qt::Key>(qtKey), Qt::KeyboardModifier(qtMask), Qt::KeyboardModifier(qtModifier), static_cast<XQKeyCapture::LongFlags>(longFlags));
}

void TestXQKeyCapture::testCancelCaptureLongKeyList_data()
{
    testCancelCaptureLongKey_data();
}

void TestXQKeyCapture::testCancelCaptureLongKeyList()
{
    numOfArgs = 1;
    actionType = WGATCancelCaptureLongKey;
    additionalResult = false;
    
    QFETCH(unsigned int, qtKey);
    QFETCH(unsigned int, qtMask);
    QFETCH(unsigned int, qtModifier);
    QFETCH(int, longFlags);

    QFETCH(long int, reqNum);
    
    QFETCH(bool, additional);
    QFETCH(long int, additionalReqNum);
    
    willBeAdditionalRequest = additional;
    additionalRequestHandle = additionalReqNum;
    
    cancelResults.clear();
    cancelResults << reqNum;
    
    cancelAdditionalResults.clear();
    cancelAdditionalResults << additionalReqNum;
    
    MyTestWindowGroup::Instance()->setRequestNumber(reqNum);
    ignoreWindowGroupAction = true;
    keyCapture->captureLongKey(static_cast<Qt::Key>(qtKey), Qt::KeyboardModifier(qtMask), Qt::KeyboardModifier(qtModifier), static_cast<XQKeyCapture::LongFlags>(longFlags));
    ignoreWindowGroupAction = false;
    willBeAdditionalRequest = additional;
    keyCapture->cancelCaptureLongKey(QList<Qt::Key>() << static_cast<Qt::Key>(qtKey), Qt::KeyboardModifier(qtMask), Qt::KeyboardModifier(qtModifier), static_cast<XQKeyCapture::LongFlags>(longFlags));
}

////////////////////////////////////////////////////////////////
//Cancel Capture Long Key
////////////////////////////////////////////////////////////////
void TestXQKeyCapture::testCancelCaptureLongKey_S60_data()
{
    QTest::addColumn<unsigned int>("qtKey");
    QTest::addColumn<unsigned int>("qtMask");
    QTest::addColumn<unsigned int>("qtModifier");
    QTest::addColumn<int>("longFlags");
    
    QTest::addColumn<long int>("reqNum");
    
    QTest::addColumn<bool>("additional");
    QTest::addColumn<long int>("additionalReqNum");
    
    QTest::newRow("esc_key") << static_cast<unsigned int>(EKeyEscape) 
                            << static_cast<unsigned int>(Qt::NoModifier)
                            << static_cast<unsigned int>(Qt::NoModifier)
                            << static_cast<int>(XQKeyCapture::LongNormal) 
                            << static_cast<long int>(22)
                            << false
                            << static_cast<long int>(23);

}

void TestXQKeyCapture::testCancelCaptureLongKey_S60()
{
    numOfArgs = 1;
    actionType = WGATCancelCaptureLongKey;
    additionalResult = false;
    
    QFETCH(unsigned int, qtKey);
    QFETCH(unsigned int, qtMask);
    QFETCH(unsigned int, qtModifier);
    QFETCH(int, longFlags);

    QFETCH(long int, reqNum);
    
    QFETCH(bool, additional);
    QFETCH(long int, additionalReqNum);
    
    willBeAdditionalRequest = additional;
    additionalRequestHandle = additionalReqNum;
    
    cancelResults.clear();
    cancelResults << reqNum;
    
    cancelAdditionalResults.clear();
    cancelAdditionalResults << additionalReqNum;
    
    MyTestWindowGroup::Instance()->setRequestNumber(reqNum);
    ignoreWindowGroupAction = true;
    keyCapture->captureLongKey(static_cast<TUint>(qtKey), Qt::KeyboardModifier(qtMask), Qt::KeyboardModifier(qtModifier), static_cast<XQKeyCapture::LongFlags>(longFlags));
    ignoreWindowGroupAction = false;
    willBeAdditionalRequest = additional;
    keyCapture->cancelCaptureLongKey(static_cast<TUint>(qtKey), Qt::KeyboardModifier(qtMask), Qt::KeyboardModifier(qtModifier), static_cast<XQKeyCapture::LongFlags>(longFlags));
}

void TestXQKeyCapture::testCancelCaptureLongKeyList_S60_data()
{
    testCancelCaptureLongKey_S60_data();
}

void TestXQKeyCapture::testCancelCaptureLongKeyList_S60()
{
    numOfArgs = 1;
    actionType = WGATCancelCaptureLongKey;
    additionalResult = false;
    
    QFETCH(unsigned int, qtKey);
    QFETCH(unsigned int, qtMask);
    QFETCH(unsigned int, qtModifier);
    QFETCH(int, longFlags);

    QFETCH(long int, reqNum);
    
    QFETCH(bool, additional);
    QFETCH(long int, additionalReqNum);
    
    willBeAdditionalRequest = additional;
    additionalRequestHandle = additionalReqNum;
    
    cancelResults.clear();
    cancelResults << reqNum;
    
    cancelAdditionalResults.clear();
    cancelAdditionalResults << additionalReqNum;
    
    MyTestWindowGroup::Instance()->setRequestNumber(reqNum);
    ignoreWindowGroupAction = true;
    keyCapture->captureLongKey(static_cast<TUint>(qtKey), Qt::KeyboardModifier(qtMask), Qt::KeyboardModifier(qtModifier), static_cast<XQKeyCapture::LongFlags>(longFlags));
    ignoreWindowGroupAction = false;
    willBeAdditionalRequest = additional;
    keyCapture->cancelCaptureLongKey(QList<TUint>() << static_cast<TUint>(qtKey), Qt::KeyboardModifier(qtMask), Qt::KeyboardModifier(qtModifier), static_cast<XQKeyCapture::LongFlags>(longFlags));
}

void TestXQKeyCapture::testCaptureRemoteKeys_data()
    {
    QTest::addColumn<unsigned int>("flags");
    
    // there are 4 flags: 
    // CaptureNone = 0x0,
    // CaptureBasic = 0x1,
    // CaptureCallHandlingExt = 0x2,
    // CaptureSideKeys = 0x4, // for future use
    // CaptureEnableRemoteExtEvents = 0x4000
    // so we should iterate through all combinations:
    for(unsigned int i(0); i <= KAllFlagsOn; ++i) {
        QString desc("flag:0x" + QString::number(i,16));
        QTest::newRow(desc.toAscii()) << i ;
        }
    
    }

void TestXQKeyCapture::testCaptureRemoteKeys()
    {
    QFETCH(unsigned int, flags);
    XQKeyCapture::CapturingFlag flag = static_cast<XQKeyCapture::CapturingFlag>(flags);
    int oldCoreApiCount = CRemConCoreApiTarget::getCount();
    int oldCallHandlingCount = CRemConCallHandlingTarget::getCount();
    int expectedCoreApiCount(oldCoreApiCount);
    int expectedCallHandlingCount(oldCallHandlingCount);
    keyCapture->captureRemoteKeys(static_cast<XQKeyCapture::CapturingFlag>(flag));
    if(flag & XQKeyCapture::CaptureBasic) expectedCoreApiCount++;
    if(flag & XQKeyCapture::CaptureCallHandlingExt) expectedCallHandlingCount++;
    int newCoreApiCount = CRemConCoreApiTarget::getCount();
    int newCallHandlingCount = CRemConCallHandlingTarget::getCount();
    QVERIFY2(expectedCoreApiCount == newCoreApiCount, "expected core api creation count was different");
    QVERIFY2(expectedCallHandlingCount == newCallHandlingCount, "expected call handling api creation count was different");    
    }

void TestXQKeyCapture::testCaptureRemoteKeys_SelectorFail_data()
    {
    testCaptureRemoteKeys_data();
    }

void TestXQKeyCapture::testCaptureRemoteKeys_SelectorFail()
    {
    QFETCH(unsigned int, flags);
    CRemConInterfaceSelector::setNewLLeave();
    bool result = keyCapture->captureRemoteKeys(static_cast<XQKeyCapture::CapturingFlag>(flags));
    QVERIFY2(!result , "CaptureRemoteKeys did not fail and it should.");
    CRemConInterfaceSelector::setNewLLeave();
    result = keyCapture->cancelCaptureRemoteKeys(static_cast<XQKeyCapture::CapturingFlag>(flags));
    QVERIFY2(!result , "CaptureRemoteKeys did not fail and it should.");        
    }

void TestXQKeyCapture::testCaptureRemoteKeys_ApiCore_data()
    {
    testCaptureRemoteKeys_data();
    }

void TestXQKeyCapture::testCaptureRemoteKeys_ApiCore()
    {
    QFETCH(unsigned int, flags);
    XQKeyCapture::CapturingFlag flag = static_cast<XQKeyCapture::CapturingFlag>(flags);
    CRemConCoreApiTarget::setNewLLeave();
    bool result = keyCapture->captureRemoteKeys(flag);
    bool expected = !(flag & XQKeyCapture::CaptureBasic);   
    QVERIFY2(result == expected, "CaptureRemoteKeys did not fail and it should.");
    }

void TestXQKeyCapture::testCaptureRemoteKeys_ApiCoreCancel_data()
    {
    testCaptureRemoteKeys_data();
    }
void TestXQKeyCapture::testCaptureRemoteKeys_ApiCoreCancel()
    {
    QFETCH(unsigned int, flags);
    
    // set all flags on
    XQKeyCapture::CapturingFlag flag = static_cast<XQKeyCapture::CapturingFlag>(KAllFlagsOn);    
    bool result = keyCapture->captureRemoteKeys(flag);
    QVERIFY2(result, "CaptureRemoteKeys failed.");
    
    // check cancel for each flag
    CRemConCoreApiTarget::setNewLLeave();
    flag = static_cast<XQKeyCapture::CapturingFlag>(flags);
    // it should fail only if CaptureBasic is not removed and was set.
    bool expected = (flag & XQKeyCapture::CaptureBasic);
    result = keyCapture->cancelCaptureRemoteKeys(flag);
    QVERIFY2(result == expected, "CaptureRemoteKeys did not fail and it should.");
    }


void TestXQKeyCapture::testCaptureRemoteKeys_CallHandling_data()
    {
    testCaptureRemoteKeys_data();
    }
void TestXQKeyCapture::testCaptureRemoteKeys_CallHandling()
    {
    QFETCH(unsigned int, flags);
    XQKeyCapture::CapturingFlag flag = static_cast<XQKeyCapture::CapturingFlag>(flags);
    
    CRemConCallHandlingTarget::setNewLLeave();
    bool result = keyCapture->captureRemoteKeys(flag);
    bool expected = !(flag & XQKeyCapture::CaptureCallHandlingExt);   
    QVERIFY2(result == expected, "CaptureRemoteKeys did not fail and it should."); 
    }

void TestXQKeyCapture::testCaptureRemoteKeys_CallHandlingCancel_data()
    {
    testCaptureRemoteKeys_data();
    }
void TestXQKeyCapture::testCaptureRemoteKeys_CallHandlingCancel()
    {
    QFETCH(unsigned int, flags);
    
    // set all flags
    XQKeyCapture::CapturingFlag flag = static_cast<XQKeyCapture::CapturingFlag>(KAllFlagsOn);    
    bool result = keyCapture->captureRemoteKeys(flag);
    QVERIFY2(result, "CaptureRemoteKeys failed.");
    
    // check cancel
    CRemConCallHandlingTarget::setNewLLeave();
    flag = static_cast<XQKeyCapture::CapturingFlag>(flags);
    // it should fail only if CaptureBasic flag is unset
    bool expected = (flag & XQKeyCapture::CaptureCallHandlingExt);
    result = keyCapture->cancelCaptureRemoteKeys(flag);
    QVERIFY2(result == expected, "CaptureRemoteKeys did not fail and it should.");
    }

void TestXQKeyCapture::testCaptureRemoteKeys_Basic_data()
    {
    QTest::addColumn<unsigned int>("buttonAction");
    QTest::addColumn<unsigned int>("operationId");
    QTest::addColumn<unsigned int>("operationQtKey");
    // this test will take few minutes to go thru all possible button / operation
    for(unsigned int buttonA(0); buttonA < 3; buttonA++) {  // 0 - ERemConCoreApiButtonPress, 1 - ERemConCoreApiButtonRelease, 2 - ERemConCoreApiButtonClick
        foreach(TRemConCoreApiOperationId operationId, myKeyMapping.keys()) {
            QTest::newRow(QString::number(operationId).toAscii()) << buttonA << static_cast<unsigned int>(operationId) << static_cast<unsigned int>(myKeyMapping.value(operationId));
        }
    }
    }

void TestXQKeyCapture::testCaptureRemoteKeys_Basic()
    {
    QFETCH(unsigned int, buttonAction);
    QFETCH(unsigned int, operationId);
    QFETCH(unsigned int, operationQtKey);    
    // this test in the beginning was supposed to test whole event delivery process; 
    // now it will just check proper translation between qt and symbian.
    myGlobalWidget.setVisible(true);
    myGlobalWidget.setFocus();
    myGlobalWidget.activateWindow();
    
    TRemConCoreApiOperationId opId = static_cast<TRemConCoreApiOperationId>(operationId);   
    TargetWrapper* myTargetWrapper = new TargetWrapper();
    XQKeyCapture::CapturingFlags myFlag = XQKeyCapture::CaptureBasic;
    myTargetWrapper->init(myFlag);
    int oldCount = CRemConCoreApiTarget::getSendResponseCount();
    TRemConCoreApiButtonAction btnAction = static_cast<TRemConCoreApiButtonAction>(buttonAction);   
    myTargetWrapper->MrccatoCommand(opId, btnAction);
    
    // verify core Api calls
    QVERIFY2(CRemConCoreApiTarget::getSendResponseCount() == oldCount+1, "Send Response Count is not equal");
    QVERIFY2(CRemConCoreApiTarget::getLastOperationId() == opId, "Operation Id is different then requested");
    
    // verify proper symbian -> qt key call 
    bool extendedEvents = (myFlag & XQKeyCapture::CaptureEnableRemoteExtEvents);        
    setProperKeys(extendedEvents);    
    if(btnAction == ERemConCoreApiButtonClick) {
        QVERIFY2(myTargetWrapper->mapKey(opId) == myLastKeyPressed, "Qt key does not correspond to proper Symbian key");
        QVERIFY2(myTargetWrapper->mapKey(opId) == myLastKeyReleased, "Qt key does not correspond to proper Symbian key");
    }
    if(btnAction == ERemConCoreApiButtonPress) {
        QVERIFY2(myTargetWrapper->mapKey(opId) == myLastKeyPressed, "Qt key does not correspond to proper Symbian key");
        QVERIFY2(myLastKeyReleased == KNotSpecifiedKey, "Qt key does not correspond to proper Symbian key");
    }
    if(btnAction == ERemConCoreApiButtonRelease) {
        QVERIFY2(myLastKeyPressed == KNotSpecifiedKey, "Qt key does not correspond to proper Symbian key");
        QVERIFY2(myTargetWrapper->mapKey(opId) == myLastKeyReleased, "Qt key does not correspond to proper Symbian key");
    }    
    delete myTargetWrapper;
    }

void TestXQKeyCapture::testCaptureRemoteKeys_Extended_data()
    {
    testCaptureRemoteKeys_data();
    }

void TestXQKeyCapture::testCaptureRemoteKeys_Extended()
    {
    QFETCH(unsigned int, flags);
    XQKeyCapture::CapturingFlag flag = static_cast<XQKeyCapture::CapturingFlag>(flags);
    
    TargetWrapper* myTargetWrapper = new TargetWrapper();
    myTargetWrapper->init(flag);
    
    bool expected = (flag & XQKeyCapture::CaptureCallHandlingExt);
    bool extendedEvents = (flag & XQKeyCapture::CaptureEnableRemoteExtEvents);
    myGlobalWidget.setVisible(true);
    myGlobalWidget.setFocus();
    myGlobalWidget.activateWindow();
    
    // key call
    resetKeys();
    myTargetWrapper->AnswerCall();
    setProperKeys(extendedEvents);
    QVERIFY((myLastKeyPressed == Qt::Key_Call) == expected);
    QVERIFY((myLastKeyReleased == Qt::Key_Call) == expected);
    
    // End call
    resetKeys();
    myTargetWrapper->EndCall();
    setProperKeys(extendedEvents);   
    QVERIFY((myLastKeyPressed == Qt::Key_Hangup) == expected);
    QVERIFY((myLastKeyReleased == Qt::Key_Hangup) == expected);
    
    // Answer End Call
    resetKeys();
    myTargetWrapper->AnswerEndCall();
    setProperKeys(extendedEvents);
    QVERIFY((myLastKeyPressed == Qt::Key_Hangup) == expected);
    QVERIFY((myLastKeyReleased == Qt::Key_Hangup) == expected);
    delete myTargetWrapper;
    }

void TestXQKeyCapture::testCancelCaptureRemoteKeys_data()
    {
    testCaptureRemoteKeys_data();
    }

void TestXQKeyCapture::testCancelCaptureRemoteKeys()
    {
    QFETCH(unsigned int, flags);
    keyCapture->cancelCaptureRemoteKeys(static_cast<XQKeyCapture::CapturingFlag>(flags));
    }



////////////////////////////////////////////////////////////////
// ERRORS
//errorString
////////////////////////////////////////////////////////////////
void TestXQKeyCapture::testErrorString()
{
    keyCapture->errorString();
}

////////////////////////////////////////////////////////////////
// ERRORS
//errorId
////////////////////////////////////////////////////////////////
void TestXQKeyCapture::testErrorId()
{
    keyCapture->errorId();
}

QString TestXQKeyCapture::clearString(const QString& line) 
{
    QString s(line);
    s.replace(" ", "");
    s.replace("\t", "");
    return s.trimmed();
}

QString TestXQKeyCapture::clearString(const QString& line, const QString& prefix, const QString& comment) 
{
    QString s(line);
    s.replace(prefix, comment);
    s.replace(" ", "");
    s.replace("\t", "");
    return s.trimmed();
}

void TestXQKeyCapture::setProperKeys(bool extendedEvents) 
{
    if(extendedEvents) {
        myLastKeyPressed = myLastKeyExtendedPress;
        myLastKeyReleased = myLastKeyExtendedRelease;
        QVERIFY((myLastKeyPress == KNotSpecifiedKey));
        QVERIFY((myLastKeyRelease == KNotSpecifiedKey));
    } else {
        myLastKeyPressed = myLastKeyPress;
        myLastKeyReleased = myLastKeyRelease;
        QVERIFY((myLastKeyExtendedPress == KNotSpecifiedKey));
        QVERIFY((myLastKeyExtendedRelease == KNotSpecifiedKey));
    }       
}

////////////////////////////////////////////////////////////////
// TEST KEY MAPPER FILE
////////////////////////////////////////////////////////////////
void TestXQKeyCapture::testKeyMapperFile()
{
    // test only for emulator build
    #ifdef __WINSCW__   
    
    QString firstline("static const KeyMapping keyMapping[] = {");
    QString lastline("};");
    QString comment("//");
    
    QStringList qt;
    QStringList kc;

    QFile qtFile("c:\\qkeymapper_s60.cpp");
    QVERIFY2(qtFile.open(QIODevice::ReadOnly | QIODevice::Text), "Failed to open: qtFile");
    
    QFile kcFile("c:\\keymapper.cpp");
    QVERIFY2(kcFile.open(QIODevice::ReadOnly | QIODevice::Text), "Failed to open: kcFile");
    
    QTextStream inQtFile(&qtFile);
    bool test(false);
    while(!inQtFile.atEnd()) {
        QString line = inQtFile.readLine();
        // trim everything that is on the right side of comment and add to list if needed
        if(test) qt.append(clearString(line.split(comment).at(0)));
        if(line.contains(firstline)) {
            test = true;
        }
        if(line.contains(lastline)) {
            test = false;
        }
    }
    test = false;
    QTextStream inKcFile(&kcFile);
    while(!inKcFile.atEnd()) {
        QString line = inKcFile.readLine();
        // trim everything that is on the right side of comment and add to list if needed
        if(test) kc.append(clearString(line.split(comment).at(0)));
        if(line.contains(firstline)) {
            test = true;
        }
        if(line.contains(lastline)) {
            test = false;
        }
    }
    
    QVERIFY2(qt.count() == kc.count(), "Amount of lines inside key definition is different");
    
    for(int i = 0; i < kc.size(); i++) {
        QString keys = kc.at(i);
        QVERIFY2(qt.contains(keys), "qtFile does not contain key(s) from capture keys");
    }
    
    for(int i = 0; i < qt.size(); i++) {
        QString keys = qt.at(i);
        QVERIFY2(kc.contains(keys), "kcFile does not contain qt keys");
    }    
#else
    // Skip test on hw
    QSKIP("This test is valid only on emulator", SkipSingle);
#endif // __WINSCW__
}


////////////////////////////////////////////////////////////////
// REQUEST SLOT
//windowGroupAction
////////////////////////////////////////////////////////////////
void TestXQKeyCapture::windowGroupAction(WindowGroupActionType wgat, QList<unsigned int> paramList)
{
    if(!ignoreWindowGroupAction)
    {
        QVERIFY(wgat == actionType);
        QVERIFY(paramList.count() == numOfArgs);
        
        for(int i = 0; i < numOfArgs; i++)
            {
            if(additionalResult) {
                QVERIFY2(paramList[i] == additionalResults[i],QString("Unequal [1]: %1 != %2").arg(paramList[i]).arg(additionalResults[i]).toLatin1().data());
            } else {
                QVERIFY2(paramList[i] == results[i], QString("Unequal [2]: %1 != %2").arg(paramList[i]).arg(results[i]).toLatin1().data());
            }
            }
    }
    if(willBeAdditionalRequest){
        additionalResult = true;
        MyTestWindowGroup::Instance()->setRequestNumber(additionalRequestHandle);
        }
}

////////////////////////////////////////////////////////////////
//windowGroupActionCancel
////////////////////////////////////////////////////////////////
void TestXQKeyCapture::windowGroupActionCancel(WindowGroupActionType wgat, QList<long int> paramList)
{
    QVERIFY(wgat == actionType);
    QVERIFY(paramList.count() == numOfArgs);
    
    for(int i = 0; i < numOfArgs; i++){
        if(additionalResult) QVERIFY(paramList[i] == cancelAdditionalResults[i]);
        else QVERIFY(paramList[i] == cancelResults[i]);
    }

    if(willBeAdditionalRequest){
        additionalResult = true;
    }
}

bool TestXQKeyCapture::event(QEvent *ev)
{
    processEvent(ev);  
    return false;
}

bool TestXQKeyCapture::eventFilter(QObject *o, QEvent *ev)
{
    processEvent(ev);
    return qApp->eventFilter(o, ev);
}

void TestXQKeyCapture::processEvent(QEvent *ev)
{
    if(ev){
        if(ev->type() == QEvent::KeyPress) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(ev);
            myLastKeyPress = static_cast<Qt::Key>(keyEvent->key());
        } else if(ev->type() == QEvent::KeyRelease) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(ev);
            myLastKeyRelease = static_cast<Qt::Key>(keyEvent->key());
        } else if(ev->type() == XQKeyCapture::remoteEventType_KeyPress()) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(ev);
            myLastKeyExtendedPress = static_cast<Qt::Key>(keyEvent->key());
        } else if(ev->type() == XQKeyCapture::remoteEventType_KeyRelease()) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(ev);
            myLastKeyExtendedRelease = static_cast<Qt::Key>(keyEvent->key());
        } 
    }
}

////////////////////////////////////////////////////////////////
// MAIN
//main
////////////////////////////////////////////////////////////////

#define _XQKEYCAPTURE_UNITTEST_LOG_TO_C_

#ifdef _XQKEYCAPTURE_UNITTEST_LOG_TO_C_
int main(int argc, char* argv[]) 
    {
        QApplication app(argc, argv);
        TestXQKeyCapture tc;
        int c = 3;
        char* v[] = {argv[0], "-o", "c:/test.txt"};
        return QTest::qExec(&tc, c, v);
    }
#else
    QTEST_MAIN(TestXQKeyCapture)
#endif

#include "test_xqkeycapture.moc"
