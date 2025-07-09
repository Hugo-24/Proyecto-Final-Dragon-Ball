#include "proyectil.h"
#include <QPixmap>
#include <QDebug>
#include <QWidget>

Proyectil::Proyectil(QWidget* parent,
                     const QVector2D& posicion,
                     const QVector2D& direccion,
                     const QString& tipo,
                     float velocidad)
    : Objeto(parent, posicion),
    direccion(direccion.normalized()),
    velocidad(velocidad)
{
    this->tipo = tipo;
    QPixmap imagen;

    // Cargar imagen según tipo
    if (tipo == "roshi") {
        imagen.load(":/Sprites/Proyectiles/Bola_Energia.png");
        sprite->setFixedSize(80, 80);
        danio = 15;  // Roshi causa más daño
        esDelJugador = true; // Proyectil aliado
    } else if (tipo == "lunch") {
        if (direccion.x() >= 0)
            imagen.load(":/Sprites/Proyectiles/R_BalaCohete.png");
        else
            imagen.load(":/Sprites/Proyectiles/L_BalaCohete.png");
        sprite->setFixedSize(80, 40);
        danio = 15;  // Bazuca de Lunch
        esDelJugador = true; // Proyectil aliado
    } else if (tipo == "subfusil") {
        imagen.load(":/Sprites/Proyectiles/BalaSubfusil.png");
        sprite->setFixedSize(28, 8);
        danio = 5;  // Subfusil más débil
        esDelJugador = false; // Proyectil enemigo por defecto
    }

    // Validar imagen
    if (imagen.isNull()) {
        qDebug() << "No se pudo cargar el sprite del proyectil tipo:" << tipo;
    } else {
        sprite->setPixmap(imagen.scaled(sprite->size()));
    }

    sprite->show();
}

Proyectil::~Proyectil() {
    if (sprite) {
        sprite->deleteLater();
        sprite = nullptr;
    }
}

// Actualiza la posición del proyectil
void Proyectil::actualizar() {
    // Movimiento lineal constante
    posicion += direccion * velocidad;

    // Eliminación si sale de pantalla
    if (posicion.x() < -100 || posicion.x() > 8200) {
        sprite->hide();
    }
}

// Acción en colisión (a definir)
void Proyectil::interactuar(Entidad* otra) {
    if (!otra) return;
    qDebug() << "Proyectil impactó con una entidad.";
    sprite->hide(); // Por ahora solo se oculta al impactar
}

// Devuelve la cantidad de daño que causa este proyectil
int Proyectil::getDanio() const {
    return danio;
}

// Nuevo: indicar si el proyectil pertenece al jugador
void Proyectil::setEsDelJugador(bool valor) {
    esDelJugador = valor;
}

// Nuevo: consultar si el proyectil pertenece al jugador
bool Proyectil::esDelJugadorFunc() const {
    return esDelJugador;
}
QString Proyectil::getTipo() const {
    return tipo;
}
