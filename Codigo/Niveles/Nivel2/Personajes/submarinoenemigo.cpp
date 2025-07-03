#include "submarinoenemigo.h"
#include <QPixmap>
#include <QDebug>
#include <cmath>

SubmarinoEnemigo::SubmarinoEnemigo(QWidget* parent, const QVector2D& posicion)
    : Entidad(parent),
    direccion(-1, 0),
    velocidad(2.0f),
    vida(100),
    tiempoTotal(0.0f),
    radio(30.0f),
    centroMovimiento(posicion),
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

    // Barra de vida
    barraVida = new QProgressBar(parent);
    barraVida->setFixedSize(60, 8);
    barraVida->setRange(0, 100);
    barraVida->setValue(vida);
    barraVida->setStyleSheet(
        "QProgressBar {"
        "  border: 1px solid gray;"
        "  border-radius: 3px;"
        "  background: #ddd;"
        "}"
        "QProgressBar::chunk {"
        "  background-color: #e74c3c;"
        "}"
        );
    barraVida->setTextVisible(false);
    barraVida->show();

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

    tiempoTotal += 0.1f;

    if (enModoAtaque && objetivo) {
        QVector2D direccion = (objetivo->getPosicion() - posicion);
        float distancia = direccion.length();

        float distanciaMinima = 180.0f; // 🛑 Distancia deseada para no superponerse

        if (distancia > distanciaMinima) {
            QVector2D dirNormalizado = direccion.normalized();
            setPosicion(posicion + dirNormalizado * velocidad);
        }
        // Si está suficientemente cerca, no se mueve más
    } else {
        float angle = tiempoTotal;
        float x = centroMovimiento.x() + radio * std::cos(angle);
        float y = centroMovimiento.y() + radio * std::sin(angle);
        setPosicion(QVector2D(x, y));
    }

    sprite->move(posicion.toPoint());

    // Mantener en pantalla (solo por seguridad)
    int maxX = sprite->parentWidget()->width() - sprite->width();
    int maxY = sprite->parentWidget()->height() - sprite->height();

    if (posicion.x() < 0) posicion.setX(0);
    if (posicion.y() < 0) posicion.setY(0);
    if (posicion.x() > maxX) posicion.setX(maxX);
    if (posicion.y() > maxY) posicion.setY(maxY);

    sprite->move(posicion.toPoint());

    // Posición de la barra de vida
    QPoint barraPos = posicion.toPoint() + QPoint((sprite->width() - barraVida->width()) / 2, -barraVida->height() - 5);
    barraVida->move(barraPos);
    barraVida->setValue(vida);
}


void SubmarinoEnemigo::verificarFoco(const Entidad* jugador) {
    QRect zonaFoco(posicion.x() - 500, posicion.y() - 60, 800, 120); //Rectangulo que verifica foco

    if (zonaFoco.contains(jugador->getPosicion().toPoint())) {
        enModoAtaque = true;
        tiempoEnAtaque = 0;
        temporizadorAtaque->start(8000);
        temporizadorDisparo->start(1500);
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
    QVector2D dir = (objetivo->getPosicion() - getPosicion()).normalized();

    emit torpedoDisparado(posTorpedo, dir);
}

void SubmarinoEnemigo::recibirDaño(int cantidad) {
    vida -= cantidad;
    if (vida < 0) vida = 0;

    barraVida->setValue(vida);

    if (vida == 0) {
        sprite->hide();
        barraVida->hide();
    }
}

bool SubmarinoEnemigo::estaDestruido() const {
    return vida <= 0;
}

void SubmarinoEnemigo::interactuar(Entidad* otra) {
    // Interacción si se desea (no implementado aún)
}

void SubmarinoEnemigo::setObjetivo(Entidad* obj) {
    objetivo = obj;
}

