QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    articlemodel.cpp \
    colletion.cpp \
    databasemanager.cpp \
    focus.cpp \
    form.cpp \
    gamedetailwidget.cpp \
    logindialog.cpp \
    main.cpp \
    mainwindow.cpp \
    message_recieve.cpp \
    newaccount.cpp \
    page_profile.cpp \
    publish.cpp \
    searchresultwidget.cpp \
    usermanager.cpp

HEADERS += \
    articlemodel.h \
    colletion.h \
    databasemanager.h \
    focus.h \
    form.h \
    gamedetailwidget.h \
    logindialog.h \
    mainwindow.h \
    message_recieve.h \
    newaccount.h \
    page_profile.h \
    publish.h \
    searchresultwidget.h \
    usermanager.h

FORMS += \
    colletion.ui \
    focus.ui \
    form.ui \
    gamedetailwidget.ui \
    logindialog.ui \
    mainwindow.ui \
    message_recieve.ui \
    newaccount.ui \
    page_profile.ui \
    publish.ui \
    searchresultwidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += picture.qrc

DISTFILES += \
    Black Myth.jpg \
    Delta Force.jpg

