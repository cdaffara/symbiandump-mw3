#
# Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies). 
# All rights reserved.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, version 2.1 of the License.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not, 
# see "http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html/".
#
# Description:  The configure script of ecomext-qt
#

import sys
import getopt
import os
from optparse import OptionParser

def showHelp():
	print "Usage: 'compile [--help|-h] [--clean|-c] [--target|-t <target>] [--tool|-o]'"
	print ""
	print "Options:"
	print "  --help,-h              Show help and quit"
	print "  --clean,-c             Clean ecomext-qt"
	print "  --target,-t <target>   Build only for specific target. Target syntax from makefiles has to be used,"
	print "                         eg.: \'release-armv5\' \'debug-winscw\'. Default is to build without target specified. "
	print "  --tool,-o              Compile xqecom tool instead of copying precompiled xqecom.exe (win32 version)."
	print "                         Warning: Compilation uses qt sources directly and is very probable to broke on other qt versions."
	
def copyPRF():
	os.system("qmake 1>>&0 2>&1")
	os.system("make installprf") #this will clean exe

def copyEXE():
	os.system("qmake 1>>&0 2>&1")
	os.system("make installtool") #this will clean exe
	
def compileTOOL():
	print "compile xqecom"
	os.chdir("xqecom")	
	os.system("qmake -platform win32-g++")
	os.system("make release")
	os.chdir('..')
	
def compileXQPLUGINFRAMEWORK(buildArg):
	print "compile xqplugins	",buildArg
	os.chdir("xqplugins")
	os.system("qmake")
	mcommand = "make "+buildArg
	os.system(mcommand)
	os.chdir('..')
	
def cleanME():
	os.system("qmake 1>>&0 2>&1")
	print "Clean tool"
	os.system("make cleantool") #this will clean exe
	print "Clean prf"
	os.system("make cleanprf") #this will clean prf
	os.system("make clean 1>>&0 2>&1")
	os.system("make distclean 1>>&0 2>&1")

	
def main(argv):
	try:
		opts, args = getopt.getopt(argv, "hct:o", ["help", "clean","target=","tool"])
	except getopt.GetoptError:
		showHelp()
		sys.exit(2)
	
	buildArg = ""	
	buildTool = 0
	
	for opt, arg in opts:
#		print opt,arg
		if opt in ("-h", "--help"):
			showHelp()
			sys.exit()
		elif opt in ("-c", "--clean"):
			cleanME()
			sys.exit()
		elif opt in ("-t", "--target"):
			buildArg = arg
		elif opt in ("-o", "--tool"):
			buildTool = 1
	
	copyPRF()
	if buildTool == 1:
		compileTOOL()
	else:
		copyEXE()
	compileXQPLUGINFRAMEWORK(buildArg)
	
if __name__ == "__main__":
    main(sys.argv[1:])
#eof
