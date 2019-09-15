TEMPLATE = app
TARGET = lozsrame
DEPENDPATH += . exceptions model resources view
INCLUDEPATH += .
QT += widgets

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
