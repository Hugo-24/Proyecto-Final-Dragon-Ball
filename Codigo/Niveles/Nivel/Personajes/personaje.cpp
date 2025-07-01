#include "personaje.h"

Personaje::Personaje(QWidget* parent)
    : Entidad(parent), QObject(parent), mirandoDerecha(true) {}

// Mover a la izquierda
void Personaje::moverIzquierda() {
    QVector2D vel = getVelocidad();
    vel.setX(-4);
    setVelocidad(vel);
    mirandoDerecha = false;
    actualizarSprite();
}

// Mover a la derecha
void Personaje::moverDerecha() {
    QVector2D vel = getVelocidad();
    vel.setX(4);
    setVelocidad(vel);
    mirandoDerecha = true;
    actualizarSprite();
}

// Saltar (salto hacia arriba)
void Personaje::saltar() {
    if (!enElSuelo) return; // Solo puede saltar desde el suelo

    QVector2D vel = getVelocidad();
    vel.setY(-20); // salto
    setVelocidad(vel);
    enElSuelo = false;
}

// Este método se redefine en subclases para cambiar el sprite
void Personaje::actualizarSprite() {
    // Por defecto, no hace nada
}
bool Personaje::estaMirandoDerecha() const {
    return mirandoDerecha;
}
