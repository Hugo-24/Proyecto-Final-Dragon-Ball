#include "entidad.h"

Entidad::Entidad(QWidget* parent) {
    sprite = new QLabel(parent);
    sprite->setScaledContents(true);
    sprite->setFixedSize(64, 64); // tamaño por defecto, puede cambiarse
}

Entidad::~Entidad() {
    delete sprite;
}

void Entidad::mover() {
    posicion += velocidad;
}

void Entidad::actualizar() {
    mover();
    sprite->move(static_cast<int>(posicion.x()), static_cast<int>(posicion.y()));
}

void Entidad::setPosicion(const QVector2D& pos) {
    posicion = pos;
    sprite->move(static_cast<int>(pos.x()), static_cast<int>(pos.y()));
}

QVector2D Entidad::getPosicion() const {
    return posicion;
}

void Entidad::setVelocidad(const QVector2D& vel) {
    velocidad = vel;
}

QVector2D Entidad::getVelocidad() const {
    return velocidad;
}

QLabel* Entidad::getSprite() {
    return sprite;
}

void Entidad::interactuar(Entidad* /*otra*/) {
    // No hace nada por defecto
}
