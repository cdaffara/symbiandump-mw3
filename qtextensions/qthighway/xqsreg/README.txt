XQ Service registration application

============= IMPORTANT NOTE: ==========

Due to notoriously returning problem with xqsreg compilation with
ever-changing private implementation in QT (which is understandable
but yet annoying) there is major change to build process.


============= BUILD STEPS: =============

1. Copy WHOLE xqsreg directory (one containing this README file)
   to: 

     [QTDIR]\src\tools

   where QTDIR is path to your qt sources (typically for SDK release: \sf\mw\qt\)


2. Ensure that QT is properly configured. In just-downoladed SDK one
   need to run such command in qt directory:

   # configure.exe -platform win32-g++ -make make -xplatform symbian-abld -nokia-developer -dont-process -no-qmake


3. Rebuild project in [QTDIR]\src\tools\bootstrap library:

   # qmake -platform win32-g++ -spec \sf\mw\qt\mkspecs\win32-g++    
   # make


4. Rebuild project in [QTDIR]\src\tools\xqsreg directory:

   # qmake -platform win32-g++ -spec \sf\mw\qt\mkspecs\win32-g++ 
   # make


5. Check if xqsreg.exe tool is in proper bin directory:

   # dir \epoc32\tools\xqsreg.exe

   (exact path depends on where QT_INSTALL_BINS points to, during compilation)
