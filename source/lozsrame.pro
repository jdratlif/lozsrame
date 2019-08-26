# $Id: lozsrame.pro,v 1.5 2008/12/18 20:49:18 jdratlif Exp $

TEMPLATE = app
TARGET = 
DEPENDPATH += . exceptions model resources view
INCLUDEPATH += .

HEADERS += exceptions/invalidsramfileexception.hh \
	model/sramfile.hh \
	view/mainwindow.hh

SOURCES += lozsrame.cc \
	exceptions/invalidsramfileexception.cc \
	model/sramfile.cc \
	view/mainwindow.cc
           
FORMS += view/mainwindow.ui
RESOURCES += resources/lozsrame.qrc

win32 {
	RC_FILE = resources/windows.rc
}

macx {
	RC_FILE = resources/lozsrame.icns
}

