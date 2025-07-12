#include "proyectil.h"
#include <QPixmap>
#include <QDebug>
#include <QWidget>

/**
 * Constructor del proyectil.
 * Recibe: parent (widget contenedor), posición inicial, dirección (como vector),
 * tipo de proyectil (ej. "roshi", "lunch", "subfusil", "roca"), y velocidad.
 */
Proyectil::Proyectil(QWidget* parent,
                     const QVector2D& posicion,
                     const QVector2D& direccion,
                     const QString& tipo,
                     float velocidad)
    : Objeto(parent, posicion),
    direccion(direccion.normalized()), // Normaliza la dirección para que siempre tenga magnitud 1
    velocidad(velocidad)
{
    this->tipo = tipo;
    QPixmap imagen;

    // === Selección del sprite según el tipo de proyectil ===
    if (tipo == "roshi") {
        imagen.load(":/Sprites/Proyectiles/Bola_Energia.png");
        sprite->setFixedSize(80, 80);     // Tamaño redondo, grande
        danio = 15;
        esDelJugador = true;
    }
    else if (tipo == "lunch") {
        if (direccion.x() >= 0)
            imagen.load(":/Sprites/Proyectiles/R_BalaCohete.png");
        else
            imagen.load(":/Sprites/Proyectiles/L_BalaCohete.png");
        sprite->setFixedSize(80, 40);     // Bala larga (bazuca)
        danio = 15;
        esDelJugador = true;
    }
    else if (tipo == "subfusil") {
        imagen.load(":/Sprites/Proyectiles/BalaSubfusil.png");
        sprite->setFixedSize(28, 8);      // Bala rápida y pequeña
        danio = 5;
        esDelJugador = false;
    }
    else if (tipo == "roca") {
        imagen.load(":/Sprites/Proyectiles/RocaGigante.png");
        sprite->setFixedSize(120, 120);   // Más grande que el personaje
        danio = 20;                       // Alto daño
        esDelJugador = false;
    }

    // Validar que la imagen haya sido cargada correctamente
    if (imagen.isNull()) {
        qDebug() << "No se pudo cargar el sprite del proyectil tipo:" << tipo;
    } else {
        sprite->setPixmap(imagen.scaled(sprite->size()));
    }

    sprite->show();
}

/**
 * Destructor: elimina el sprite visual del proyectil.
 */
Proyectil::~Proyectil() {
    if (sprite) {
        sprite->deleteLater();
        sprite = nullptr;
    }
}

/**
 * Actualiza la posición lógica del proyectil en cada frame.
 */
void Proyectil::actualizar() {
    // Movimiento lineal constante
    posicion += direccion * velocidad;

    // Si se sale del área válida, ocultar el sprite
    if (posicion.x() < -100 || posicion.x() > 8200) {
        sprite->hide();
    }
}

/**
 * Método que se ejecuta al colisionar con otra entidad.
 * Por ahora simplemente oculta el sprite.
 */
void Proyectil::interactuar(Entidad* otra) {
    if (!otra) return;
    qDebug() << "Proyectil impactó con una entidad.";
    sprite->hide();
}

/**
 * Devuelve el daño que este proyectil puede causar.
 */
int Proyectil::getDanio() const {
    return danio;
}

/**
 * Marca si el proyectil es aliado (jugador) o enemigo.
 */
void Proyectil::setEsDelJugador(bool valor) {
    esDelJugador = valor;
}

/**
 * Consulta si el proyectil pertenece al jugador.
 */
bool Proyectil::esDelJugadorFunc() const {
    return esDelJugador;
}

/**
 * Devuelve el tipo de proyectil (como string).
 */
QString Proyectil::getTipo() const {
    return tipo;
}
