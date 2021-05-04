QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    base64.cpp \
    installation_of_correspondence.cpp \
    main.cpp \
    mainwindow.cpp \
    one_of_four.cpp \
    popup.cpp \
    some_of_four.cpp \
    test.cpp \
    test_widget.cpp \
    tinyxml2.cpp \
    write_answer.cpp

HEADERS += \
    base64.hpp \
    installation_of_correspondence.h \
    mainwindow.h \
    one_of_four.h \
    popup.h \
    question_base.h \
    some_of_four.h \
    test.h \
    test_widget.h \
    tinyxml2.h \
    write_answer.h

FORMS += \
    mainwindow.ui \
    popup.ui \
    test_widget.ui
    
RESOURCES += res.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
