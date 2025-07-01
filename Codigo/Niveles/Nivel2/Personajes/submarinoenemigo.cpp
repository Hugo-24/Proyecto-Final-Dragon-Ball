#include "submarinoenemigo.h"
#include <QPixmap>
#include <QDebug>
#include <cmath>

SubmarinoEnemigo::SubmarinoEnemigo(QWidget* parent, const QVector2D& posicion)
    : Entidad(parent),
    direccion(-1, 0),
    velocidad(2.0f),
    vida(30),
    tiempoTotal(0.0f),
    amplitudVertical(4.0f),
    frecuencia(0.2f),
    pasosDesdeUltimoAvance(0),
    pasosPorAvance(10) {

    QPixmap imagen(":/Sprites/Nave/Nave_RR_Izquierda.png");
    if (imagen.isNull()) {
        qDebug() << "No se pudo cargar el sprite del submarino enemigo";
    }

    sprite->setFixedSize(64, 32);
    sprite->setPixmap(imagen.scaled(sprite->size()));
    setPosicion(posicion);
    sprite->show();
}

void SubmarinoEnemigo::actualizar() {
    tiempoTotal += 0.2f;

    // Movimiento vertical senoidal
    float offsetY = amplitudVertical * std::sin(frecuencia * tiempoTotal);
    QVector2D nuevaPos = posicion;
    nuevaPos.setY(posicion.y() + offsetY);

    // Avance horizontal cada X pasos
    pasosDesdeUltimoAvance++;
    if (pasosDesdeUltimoAvance >= pasosPorAvance) {
        nuevaPos.setX(nuevaPos.x() + direccion.normalized().x() * velocidad);
        pasosDesdeUltimoAvance = 0;
    }

    setPosicion(nuevaPos);
    sprite->move(nuevaPos.toPoint());
}

void SubmarinoEnemigo::recibirDaño(int cantidad) {
    vida -= cantidad;
    qDebug() << "Submarino enemigo recibió daño. Vida restante:" << vida;
    if (vida <= 0) {
        sprite->hide();
    }
}

bool SubmarinoEnemigo::estaDestruido() const {
    return vida <= 0;
}

void SubmarinoEnemigo::interactuar(Entidad* otra) {
    if (!otra) return;
    // Aquí puedes añadir daño al jugador si lo deseas
}
