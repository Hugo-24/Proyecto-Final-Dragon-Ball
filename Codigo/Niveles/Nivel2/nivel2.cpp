#include "Nivel2.h"
#include <QKeyEvent>
#include <QDebug>

Nivel2::Nivel2(QWidget* parent) : Nivel(parent) {
    submarino = nullptr;
    timerActualizacion = new QTimer(this);
    setFocusPolicy(Qt::StrongFocus); // Habilita captura de teclado
}

void Nivel2::cargarNivel() {
    // Fondo submarino
    mapa = new Mapa(":/Sprites/Fondos/fondo_nivel2.jpg", this);
    mapa->setGeometry(0, 0, 800, 600);
    mapa->show();

    // Submarino jugador
    submarino = new SubmarinoJugador(this);
    submarino->getSprite()->raise(); // Sobre el fondo

    // Timer para movimiento y física
    connect(timerActualizacion, &QTimer::timeout, this, [=]() {
        submarino->procesarEntrada(teclasPresionadas);
        submarino->aplicarFisica();
        submarino->actualizar();
    });

    timerActualizacion->start(16); // Aproximadamente 60 FPS

    this->setFocus();
}

// Evento al presionar una tecla
void Nivel2::keyPressEvent(QKeyEvent* event) {
    teclasPresionadas.insert(event->key());
}

// Evento al soltar una tecla
void Nivel2::keyReleaseEvent(QKeyEvent* event) {
    teclasPresionadas.remove(event->key());
}
