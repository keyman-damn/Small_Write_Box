QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    databasemanager.cpp \
    focus.cpp \
    form.cpp \
    logindialog.cpp \
    main.cpp \
    mainwindow.cpp \
    message_recieve.cpp \
    newaccount.cpp

HEADERS += \
    databasemanager.h \
    focus.h \
    form.h \
    logindialog.h \
    mainwindow.h \
    message_recieve.h \
    newaccount.h

FORMS += \
    focus.ui \
    form.ui \
    logindialog.ui \
    mainwindow.ui \
    message_recieve.ui \
    newaccount.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += picture.qrc

