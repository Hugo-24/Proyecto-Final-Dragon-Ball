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
    pasosPorAvance(10),
    enModoAtaque(false),
    tiempoEnAtaque(0.0f),
    objetivo(nullptr)
{
    QPixmap imagen(":/Sprites/Nave/Nave_RR_Izquierda.png");
    if (imagen.isNull()) {
        qDebug() << "No se pudo cargar el sprite del submarino enemigo";
    }

    sprite->setFixedSize(104, 42);
    sprite->setPixmap(imagen.scaled(sprite->size()));
    setPosicion(posicion);
    sprite->show();

    // Timers
    temporizadorAtaque = new QTimer(this);
    connect(temporizadorAtaque, &QTimer::timeout, this, &SubmarinoEnemigo::salirDeModoAtaque);

    temporizadorDisparo = new QTimer(this);
    connect(temporizadorDisparo, &QTimer::timeout, this, &SubmarinoEnemigo::dispararTorpedo);
}

void SubmarinoEnemigo::actualizar() {
    if (objetivo && !enModoAtaque) {
        verificarFoco(objetivo);
    }

    if (enModoAtaque) return; // No se mueve si está atacando

    tiempoTotal += 0.2f;

    float offsetY = amplitudVertical * std::sin(frecuencia * tiempoTotal);
    QVector2D nuevaPos = posicion;
    nuevaPos.setY(posicion.y() + offsetY);

    pasosDesdeUltimoAvance++;
    if (pasosDesdeUltimoAvance >= pasosPorAvance) {
        nuevaPos.setX(nuevaPos.x() + direccion.normalized().x() * velocidad);
        pasosDesdeUltimoAvance = 0;
    }

    setPosicion(nuevaPos);
    sprite->move(nuevaPos.toPoint());
}

void SubmarinoEnemigo::verificarFoco(const Entidad* jugador) {
    QRect zonaFoco(posicion.x() - 200, posicion.y() - 50, 400, 100);

    if (zonaFoco.contains(jugador->getPosicion().toPoint())) {
        enModoAtaque = true;
        tiempoEnAtaque = 0;
        temporizadorAtaque->start(8000);     // 8 segundos
        temporizadorDisparo->start(1500);    // Disparo cada 1.5 segundos
    }
}

void SubmarinoEnemigo::salirDeModoAtaque() {
    enModoAtaque = false;
    temporizadorAtaque->stop();
    temporizadorDisparo->stop();
}

void SubmarinoEnemigo::dispararTorpedo() {
    if (!sprite || !enModoAtaque) return;

    QVector2D posTorpedo = this->getPosicion() + QVector2D(-sprite->width(), sprite->height() / 2);
    QVector2D direccion(-1, 0);  // Dirección hacia la izquierda

    emit torpedoDisparado(posTorpedo, direccion);
    qDebug() << "Torpedo enemigo en:" << posTorpedo << " dirección:" << direccion;

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
    // Daño o reacción al colisionar con jugador (si se desea)
}
