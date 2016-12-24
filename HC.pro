HEADERS += \
    server.h \
    json.h \
    hcdef.h \
    sqlconn.h \
    redis.h \
    util.h

SOURCES += \
    server.cpp \
    main.cpp \
    json.cpp \
    sqlconn.cpp \
    redis.cpp \
    util.cpp
QT +=network sql
CONFIG +=c++11
LIBS +=-L/usr/local/lib -ltufao1 -lmysqlclient -lhiredis
INCLUDEPATH +=-I/usr/local/include -I/usr/include
