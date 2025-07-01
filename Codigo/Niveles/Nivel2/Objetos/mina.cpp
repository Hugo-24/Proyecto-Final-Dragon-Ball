#include "mina.h"
#include <QPixmap>
#include <QDebug>
#include <QTimer>
#include <QRandomGenerator>
#include <QObject>

Mina::Mina(QWidget* parent, const QVector2D& pos)
    : Objeto(parent, pos), contenedor(parent)  // guardamos el parent
{
    QPixmap imagen(":/Sprites/Nave/minas.png");
    if (imagen.isNull()) {
        qDebug() << "Error al cargar el sprite de la mina.";
    } else {
        sprite->setPixmap(imagen.scaled(40, 40)); // Tamaño opcional
    }

    setPosicion(pos);
    sprite->show();
}
void Mina::interactuar(Entidad* entidad) {
    if (!entidad || !sprite->isVisible()) return;

    qDebug() << "¡Mina explotó!";
    sprite->hide();  // Desaparece visualmente

    // 🌀 Retroceso solo si fue el jugador
    QWidget* contenedor = qobject_cast<QWidget*>(sprite->parent());
    if (contenedor) {
        QTimer* animador = new QTimer(contenedor);
        int pasos = 10;
        int contador = 0;
        QVector2D inicio = entidad->getPosicion();
        QVector2D destino = inicio - QVector2D(150, 0);
        QVector2D delta = (destino - inicio) / pasos;

        QObject::connect(animador, &QTimer::timeout, [=]() mutable {
            if (contador >= pasos) {
                animador->stop();
                animador->deleteLater();
                return;
            }
            entidad->setPosicion(entidad->getPosicion() + delta);
            entidad->actualizar();
            contador++;
        });

        animador->start(20);
    }

    // 🧠 Asegúrate de que esta parte siempre se ejecute después de cada explosión
    QTimer::singleShot(10000, [this]() {
        QWidget* contenedor = qobject_cast<QWidget*>(sprite->parent());
        if (!contenedor) return;

        int maxX = contenedor->width() - sprite->width();
        int maxY = contenedor->height() - sprite->height();

        int nuevaX = QRandomGenerator::global()->bounded(0, maxX);
        int nuevaY = QRandomGenerator::global()->bounded(0, maxY);

        setPosicion(QVector2D(nuevaX, nuevaY));
        sprite->show();
        actualizar();
    });
}




void Mina::actualizar() {
    // Aquí puedes animar la mina si lo deseas
}
