#include "goku.h"
#include "proyectil.h"
#include "nivel3.h"

#include <QPixmap>
#include <QTimer>
#include <QDebug>
#include <QMediaPlayer>
#include <QAudioOutput>

// Constructor
Goku::Goku(QWidget* parent)
    : Personaje(parent), contenedor(parent)
{
    // Sprite inicial
    setSprite(":/Sprites/Goku/R_Goku_Idle.png");
    setPosicion(QVector2D(100, 450));

    // Mostrar sprite desde el inicio
    sprite->show();
    sprite->raise();
}

// Destructor
Goku::~Goku() {
}

// Permite activar/desactivar el movimiento del jugador (por cinemáticas, por ejemplo)
void Goku::setPuedeMoverse(bool valor) {
    puedeMoverse = valor;
}

bool Goku::getPuedeMoverse() const {
    return puedeMoverse;
}

// Ataque cuerpo a cuerpo (por ahora es placeholder)
void Goku::atacar() {
    if (estaMuerto) return;

    QString rutaGolpe = mirandoDerecha
                            ? ":/Sprites/Goku/R_Goku_Attack.png"
                            : ":/Sprites/Goku/L_Goku_Attack.png";

    setSprite(rutaGolpe);

    QLabel* spriteLabel = getSprite();
    QString rutaIdle = mirandoDerecha
                           ? ":/Sprites/Goku/R_Goku_Idle.png"
                           : ":/Sprites/Goku/L_Goku_Idle.png";

    // Regresar al estado idle tras 400 ms
    QTimer::singleShot(400, this, [spriteLabel, rutaIdle]() {
        if (spriteLabel)
            spriteLabel->setPixmap(QPixmap(rutaIdle));
    });
}

// Lanza una bola de energía (similar al ataque de Roshi)
void Goku::lanzarEnergia() {
    if (estaMuerto || !puedeDisparar) return;

    // Cambiar sprite al de ataque
    QString rutaDisparo = mirandoDerecha
                              ? ":/Sprites/Goku/R_Goku_Attack.png"
                              : ":/Sprites/Goku/L_Goku_Attack.png";
    setSprite(rutaDisparo);

    // Regresar a sprite idle después de 500 ms
    QLabel* spriteLabel = getSprite();
    QString rutaIdle = mirandoDerecha
                           ? ":/Sprites/Goku/R_Goku_Idle.png"
                           : ":/Sprites/Goku/L_Goku_Idle.png";

    QTimer::singleShot(500, this, [spriteLabel, rutaIdle]() {
        if (spriteLabel)
            spriteLabel->setPixmap(QPixmap(rutaIdle));
    });

    // Posición y dirección de la bola
    QVector2D direccion = mirandoDerecha ? QVector2D(1, 0) : QVector2D(-1, 0);
    QVector2D offset = QVector2D(40 * direccion.x(), 5);
    QVector2D posBola = getPosicion() + offset;

    // Crear y configurar proyectil
    Proyectil* bola = new Proyectil(contenedor, posBola, direccion, "roshi", 8.0f); // Reutiliza tipo "roshi"
    bola->setEsDelJugador(true); // Importante: no dañe al jugador
    bola->getSprite()->raise();

    // Agregar el proyectil al nivel (si es Nivel3)
    if (Nivel3* nivel = dynamic_cast<Nivel3*>(contenedor)) {
        nivel->agregarProyectil(bola);
    }

    // Sonido del disparo (reutilizado del nivel 1)
    QMediaPlayer* sfx = new QMediaPlayer(this);
    QAudioOutput* out = new QAudioOutput(this);
    sfx->setAudioOutput(out);
    sfx->setSource(QUrl("qrc:/Sonidos/Nivel1-S/disparo-roshi.mp3"));
    out->setVolume(100);
    sfx->play();
}

// Actualiza el sprite de Goku según movimiento (idle o caminando)
void Goku::actualizarSprite() {
    if (estaMuerto || !puedeMoverse) return;

    static bool alternar = false;
    QString ruta;

    if (getVelocidad().x() != 0) {
        ruta = mirandoDerecha
                   ? (alternar ? ":/Sprites/Goku/R_Goku_Walk1.png" : ":/Sprites/Goku/R_Goku_Walk2.png")
                   : (alternar ? ":/Sprites/Goku/L_Goku_Walk1.png" : ":/Sprites/Goku/L_Goku_Walk2.png");
        alternar = !alternar;
    } else {
        ruta = mirandoDerecha
                   ? ":/Sprites/Goku/R_Goku_Idle.png"
                   : ":/Sprites/Goku/L_Goku_Idle.png";
    }

    setSprite(ruta);
}
