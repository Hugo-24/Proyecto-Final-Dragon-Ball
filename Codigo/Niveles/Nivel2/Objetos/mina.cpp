#include "mina.h"
#include <QPixmap>
#include <QDebug>

Mina::Mina(QWidget* parent, const QVector2D& pos)
    : Objeto(parent, pos)
{
    QPixmap imagen(":/Sprites/Lunch/minas.png");
    if (imagen.isNull()) {
        qDebug() << "Error al cargar el sprite de la mina.";
    } else {
        sprite->setPixmap(imagen.scaled(40, 40)); // Tamaño opcional
    }

    setPosicion(pos);
    sprite->show();
}

void Mina::interactuar(Entidad* entidad) {
    if (!entidad) return;

    // Supón que se define recibirDaño() en Jugador (o Entidad si es genérico)
    qDebug() << "¡Mina explotó!";

    sprite->hide(); // O eliminarla más adelante
}

void Mina::actualizar() {
    // Aquí puedes animar la mina si lo deseas
}
