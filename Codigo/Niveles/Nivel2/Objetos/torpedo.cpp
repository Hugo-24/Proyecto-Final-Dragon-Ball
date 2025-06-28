#include "torpedo.h"
#include <QDebug>
#include <cmath>

Torpedo::Torpedo(QWidget* parent, const QVector2D& posicion, const QVector2D& direccion)
    : Objeto(parent, posicion), direccion(direccion), velocidad(8.0f),
    tiempoTotal(0.0f), amplitud(10.0f), frecuencia(0.05f), posicionInicialY(posicion.y()) {

    QPixmap imagen(":/Sprites/Lunch/torpedo_jugador.png");
    if (imagen.isNull()) {
        qDebug() << "No se pudo cargar el sprite del torpedo";
    }

    // 👉 Establece primero el tamaño del QLabel (sprite)
    sprite->setFixedSize(26, 10); // Tamaño del contenedor
    sprite->setPixmap(imagen.scaled(sprite->size())); // Ajusta el QPixmap al QLabel
    sprite->show();
}


void Torpedo::actualizar() {
    // Movimiento horizontal constante
    posicion.setX(posicion.x() + direccion.normalized().x() * velocidad);

    // Movimiento vertical senoidal
    tiempoTotal += 0.5f; // Puedes ajustar la velocidad de oscilación
    float offsetY = amplitud * std::sin(frecuencia * posicion.x());
    posicion.setY(posicionInicialY + offsetY);

    sprite->move(posicion.toPoint());
}


void Torpedo::interactuar(Entidad* entidad) {
    if (!entidad) return;

    // Ejemplo: aplicar daño si es un enemigo
    qDebug() << "Torpedo colisionó con una entidad.";
    sprite->hide(); // O eliminarlo del escenario
}
