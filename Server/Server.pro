TEMPLATE = app

QT  += core gui multimedia

INCLUDEPATH += ../Common/

win32: LIBS += -lws2_32

SOURCES += \
    main.cpp \
    rb.rmdserver.cpp   \
    rb.chatserver.cpp \
    rb.voipserver.cpp  \
    ../Common/rn.service.cpp \
    ../Common/rn.network.cpp \
    ../Common/rc.sleep.cpp \
    ../Common/rc.log.cpp \
    ../Common/rb.encryptor.cpp
    
HEADERS += \
    rb.rmdserver.h \
    rb.chatserver.h \
    rb.voipserver.h  \
    ../Common/rn.socket.h \
    ../Common/rn.service.h \
    ../Common/rn.network.h  \
    ../Common/rn.encryptor.h \
    ../Common/rc.sleep.h \
    ../Common/rc.log.h \
    ../Common/rb.encryptor.h \
    ../Common/include.h
