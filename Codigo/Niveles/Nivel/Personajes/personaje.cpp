#include "personaje.h"

Personaje::Personaje(QWidget* parent)
    : Entidad(parent), QObject(parent), mirandoDerecha(true), pasoAlternado(false)
{
    animTimer = new QTimer(this);
    connect(animTimer, &QTimer::timeout, this, [=]() {
        pasoAlternado = !pasoAlternado;
        actualizarSprite();
    });
    animTimer->setInterval(200); // velocidad de cambio de sprite (ms)
}

// Mover a la izquierda
void Personaje::moverIzquierda() {
    QVector2D vel = getVelocidad();
    vel.setX(-4);
    setVelocidad(vel);
    mirandoDerecha = false;
    iniciarAnimacionCaminar();
}

// Mover a la derecha
void Personaje::moverDerecha() {
    QVector2D vel = getVelocidad();
    vel.setX(4);
    setVelocidad(vel);
    mirandoDerecha = true;
    iniciarAnimacionCaminar();
}

// Saltar (salto hacia arriba)
void Personaje::saltar() {
    if (!enElSuelo) return;
    QVector2D vel = getVelocidad();
    vel.setY(-16);
    setVelocidad(vel);
    enElSuelo = false;
    detenerAnimacionCaminar(); // Opcional: detener animación al saltar
}

bool Personaje::estaMirandoDerecha() const {
    return mirandoDerecha;
}

void Personaje::iniciarAnimacionCaminar() {
    if (!animTimer->isActive())
        animTimer->start();
}

void Personaje::detenerAnimacionCaminar() {
    animTimer->stop();
    pasoAlternado = false;
    actualizarSprite();
}

// Por defecto no hace nada, se redefine en Roshi / Lunch
void Personaje::actualizarSprite() {

}
