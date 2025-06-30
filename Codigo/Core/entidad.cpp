#include "entidad.h"

Entidad::Entidad(QWidget* parent) {
    sprite = new QLabel(parent);
    sprite->setScaledContents(true);
    sprite->setFixedSize(64, 64); // Tamaño inicial (puede cambiarse)
}

Entidad::~Entidad() {
    delete sprite;
}

// Movimiento simple: posición += velocidad
void Entidad::mover() {
    posicion += velocidad;
    // Limitar movimiento horizontal
    if (posicion.x() < 0) {
        posicion.setX(0);
    }
    if (posicion.x() > 736 - sprite->width()) {  // Suponiendo fondo de 736px
        posicion.setX(736 - sprite->width());
    }
}

// Actualiza sprite en pantalla
void Entidad::actualizar() {
    mover();
    sprite->move(static_cast<int>(posicion.x()), static_cast<int>(posicion.y()));
}

// Posición manual del sprite
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

// Cambia la imagen del sprite
void Entidad::setSprite(const QString& ruta) {
    QPixmap pix(ruta);                     // Cargar imagen
    sprite->setPixmap(pix);                // Asignar imagen al QLabel
    sprite->resize(pix.size());            // Ajustar tamaño del QLabel
}

// Física básica: gravedad + límite suelo
void Entidad::aplicarFisica() {
    QVector2D vel = getVelocidad();
    QVector2D pos = getPosicion();

    vel.setY(vel.y() + 0.8); // gravedad
    pos += vel;

    if (pos.y() > 480) {
        pos.setY(480);
        vel.setY(0);
        enElSuelo = true;  // ¡Está en el suelo!
    } else {
        enElSuelo = false; // Está en el aire
    }

    setVelocidad(vel);
    setPosicion(pos);
}
