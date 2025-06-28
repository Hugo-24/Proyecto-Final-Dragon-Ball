QT += core gui widgets

CONFIG += c++17
TEMPLATE = app
TARGET = DragonBallGame

# INCLUDES
INCLUDEPATH += \
    Codigo/Core \
    Codigo/UI \
    Codigo/Niveles/Nivel \
    Codigo/Niveles/Nivel1 \
    Codigo/Niveles/Nivel2 \
    Codigo/Niveles/Nivel2/Personajes \
    Codigo/Niveles/Nivel2/Objetos

# SOURCES
SOURCES += \
    Codigo/Core/objeto.cpp \
    main.cpp \
    Codigo/Core/entidad.cpp \
    Codigo/Core/juego.cpp \
    Codigo/Core/mapa.cpp \
    Codigo/UI/mainwindow.cpp \
    Codigo/UI/menu_principal.cpp \
    Codigo/Niveles/Nivel/nivel.cpp \
    Codigo/Niveles/Nivel1/nivel1.cpp \
    Codigo/Niveles/Nivel2/nivel2.cpp \
    Codigo/Niveles/Nivel2/Objetos/mina.cpp \
    Codigo/Niveles/Nivel2/Objetos/torpedo.cpp \
    Codigo/Niveles/Nivel2/Personajes/submarinojugador.cpp

# HEADERS
HEADERS += \
    Codigo/Core/entidad.h \
    Codigo/Core/juego.h \
    Codigo/Core/mapa.h \
    Codigo/Core/objeto.h \
    Codigo/UI/mainwindow.h \
    Codigo/UI/menu_principal.h \
    Codigo/Niveles/Nivel/nivel.h \
    Codigo/Niveles/Nivel1/nivel1.h \
    Codigo/Niveles/Nivel2/nivel2.h \
    Codigo/Niveles/Nivel2/Objetos/mina.h \
    Codigo/Niveles/Nivel2/Objetos/torpedo.h \
    Codigo/Niveles/Nivel2/Personajes/submarinojugador.h

# FORMULARIOS DE Qt Designer
FORMS += \
    Codigo/UI/mainwindow.ui

# ARCHIVOS DE RECURSOS (sprites, imágenes, etc.)
RESOURCES += Codigo/Core/recursos.qrc
# OPCIONAL: instalación (solo si haces deploy)
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

