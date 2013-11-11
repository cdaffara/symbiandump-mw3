@rem
@rem Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
@rem All rights reserved.
@rem
@rem This program is free software: you can redistribute it and/or modify
@rem it under the terms of the GNU Lesser General Public License as published by
@rem the Free Software Foundation, version 2.1 of the License.
@rem 
@rem This program is distributed in the hope that it will be useful,
@rem but WITHOUT ANY WARRANTY; without even the implied warranty of
@rem MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
@rem GNU Lesser General Public License for more details.
@rem
@rem You should have received a copy of the GNU Lesser General Public License
@rem along with this program.  If not, 
@rem see "http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html/".
@rem
@rem Description:
@rem

@echo on

IF ["%1"] EQU [""] GOTO :NODEL
call del MON.sym
call del MON.dat

:NODEL
call qmake -platform symbian-abld
call bldmake bldfiles
call abld clean
call abld reallyclean

IF ["%1"] EQU [""] GOTO :NORMAL

echo -- Build and instrument QtHighway for Code coverage --
call ctcwrap -i d -C "EXCLUDE+*\moc*.cpp" abld build winscw udeb

:NORMAL
echo -- Build QtHighway --
call abld build winscw udeb

call cd xqservice\tsrc
call runall.cmd %1
call cd \qthighway

IF ["%1"] EQU [""] GOTO :END

call ctcpost MON.sym MON.dat xqservice\tsrc\tst_xqservicechannel\MON.sym xqservice\tsrc\tst_xqservicechannel\MON.dat xqservice\tsrc\ut_xqserviceadaptor\MON.sym xqservice\tsrc\ut_xqserviceadaptor\MON.dat xqservice\tsrc\ut_xqservicechannel\MON.sym xqservice\tsrc\ut_xqservicechannel\MON.dat xqservice\tsrc\ut_xqserviceprovider\MON.sym xqservice\tsrc\ut_xqserviceprovider\MON.dat xqservice\tsrc\ut_xqservicerequest\MON.sym xqservice\tsrc\ut_xqservicerequest\MON.dat -p profile.txt

echo -- Coverage calculated --

:END
echo -- Build Complete --


