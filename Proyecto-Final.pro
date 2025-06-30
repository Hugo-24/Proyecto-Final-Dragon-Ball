QT += core gui widgets

CONFIG += c++17
TEMPLATE = app
TARGET = DragonBallGame

# =========================================
# INCLUDE PATHS
# =========================================
INCLUDEPATH += \
    Codigo/Core \
    Codigo/UI \
    Codigo/Niveles/Nivel \
    Codigo/Niveles/Nivel/Personajes \
    Codigo/Niveles/Nivel1 \
    Codigo/Niveles/Nivel1/Personajes \
    Codigo/Niveles/Nivel2 \
    Codigo/Niveles/Nivel2/Personajes \
    Codigo/Niveles/Nivel2/Objetos

# =========================================
# CORE DEL JUEGO
# =========================================
SOURCES += \
    Codigo/Core/entidad.cpp \
    Codigo/Core/juego.cpp \
    Codigo/Core/mapa.cpp \
    Codigo/Core/objeto.cpp \

HEADERS += \
    Codigo/Core/entidad.h \
    Codigo/Core/juego.h \
    Codigo/Core/mapa.h \
    Codigo/Core/objeto.h \

# =========================================
# UI / INTERFAZ
# =========================================
SOURCES += \
    main.cpp \
    Codigo/UI/mainwindow.cpp \
    Codigo/UI/menu_principal.cpp

HEADERS += \
    Codigo/UI/mainwindow.h \
    Codigo/UI/menu_principal.h

FORMS += \
    Codigo/UI/mainwindow.ui

# =========================================
# NIVEL BASE
# =========================================
SOURCES += \
    Codigo/Niveles/Nivel/nivel.cpp \
    Codigo/Niveles/Nivel/Personajes/personaje.cpp

HEADERS += \
    Codigo/Niveles/Nivel/nivel.h \
    Codigo/Niveles/Nivel/Personajes/personaje.h

# =========================================
# NIVEL 1
# =========================================
SOURCES += \
    Codigo/Niveles/Nivel1/nivel1.cpp \
    Codigo/Niveles/Nivel1/Personajes/roshi.cpp \
    Codigo/Niveles/Nivel1/Personajes/lunch.cpp

HEADERS += \
    Codigo/Niveles/Nivel1/nivel1.h \
    Codigo/Niveles/Nivel1/Personajes/roshi.h \
    Codigo/Niveles/Nivel1/Personajes/lunch.h
# =========================================
# NIVEL 2
# =========================================
SOURCES += \
    Codigo/Niveles/Nivel2/nivel2.cpp \
    Codigo/Niveles/Nivel2/Personajes/submarinojugador.cpp \
    Codigo/Niveles/Nivel2/Objetos/mina.cpp \
    Codigo/Niveles/Nivel2/Objetos/torpedo.cpp

HEADERS += \
    Codigo/Niveles/Nivel2/nivel2.h \
    Codigo/Niveles/Nivel2/Personajes/submarinojugador.h \
    Codigo/Niveles/Nivel2/Objetos/mina.h \
    Codigo/Niveles/Nivel2/Objetos/torpedo.h

# =========================================
# RECURSOS
# =========================================
RESOURCES += \
    Codigo/Core/recursos.qrc

# =========================================
# INSTALACIÓN (opcional)
# =========================================
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
