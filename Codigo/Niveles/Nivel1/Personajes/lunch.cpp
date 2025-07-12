#include "lunch.h"
#include "proyectil.h"
#include "nivel1.h"

#include <QTimer>
#include <QDebug>

/**
 * Constructor de Lunch.
 * Inicializa en modo tranquila, con sprite inicial y posición sobre el suelo.
 */
Lunch::Lunch(QWidget* parent)
    : Personaje(parent),
    transformada(false),
    contenedor(parent)
{
    setSprite(":/Sprites/Lunch/R_Idle_BlueLunch.png");
    setPosicion(QVector2D(100, 450));
    sprite->show();
    sprite->raise();
}

Lunch::~Lunch() {
    // No se requiere limpieza adicional
}

/**
 * Actualiza el sprite de Lunch según el estado de movimiento y transformación.
 */
void Lunch::actualizarSprite() {
    if (estaMuerto) return;

    static bool alternar = false;
    QString ruta;

    if (getVelocidad().x() != 0) {
        ruta = transformada
                   ? (mirandoDerecha ? (alternar ? ":/Sprites/Lunch/R_Walk1_Launch.png"
                                                 : ":/Sprites/Lunch/R_Walk2_Launch.png")
                                     : (alternar ? ":/Sprites/Lunch/L_Walk1_Launch.png"
                                                 : ":/Sprites/Lunch/L_Walk2_Launch.png"))
                   : (mirandoDerecha ? (alternar ? ":/Sprites/Lunch/R_Walk1_BlueLunch.png"
                                                 : ":/Sprites/Lunch/R_Walk2_BlueLunch.png")
                                     : (alternar ? ":/Sprites/Lunch/L_Walk1_BlueLunch.png"
                                                 : ":/Sprites/Lunch/L_Walk2_BlueLunch.png"));
        alternar = !alternar;
    } else {
        ruta = transformada
                   ? (mirandoDerecha ? ":/Sprites/Lunch/R_Idle_Launch.png"
                                     : ":/Sprites/Lunch/L_Idle_Launch.png")
                   : (mirandoDerecha ? ":/Sprites/Lunch/R_Idle_BlueLunch.png"
                                     : ":/Sprites/Lunch/L_Idle_BlueLunch.png");
    }

    setSprite(ruta);
}

/**
 * Cambia entre modo tranquila y modo agresiva.
 */
void Lunch::transformar() {
    setPuedeDisparar(true);
    transformada = !transformada;
    actualizarSprite();
    qDebug() << "Lunch se transforma a modo" << (transformada ? "agresiva" : "tranquila");
}

/**
 * Ejecuta el disparo de bazuca con animación y sonido.
 * Solo disponible en modo agresiva.
 */
void Lunch::disparar() {
    if (!transformada) return;

    QStringList frames = mirandoDerecha
                             ? QStringList{":/Sprites/Lunch/R_Bazooka1_Launch.png", ":/Sprites/Lunch/R_Bazooka2_Launch.png",
                                           ":/Sprites/Lunch/R_Bazooka3_Launch.png", ":/Sprites/Lunch/R_Bazooka4_Launch.png"}
                             : QStringList{":/Sprites/Lunch/L_Bazooka1_Launch.png", ":/Sprites/Lunch/L_Bazooka2_Launch.png",
                                           ":/Sprites/Lunch/L_Bazooka3_Launch.png", ":/Sprites/Lunch/L_Bazooka4_Launch.png"};

    int delay = 200;
    for (int i = 0; i < frames.size(); ++i) {
        QTimer::singleShot(i * delay, this, [this, frame = frames[i]]() {
            setSprite(frame);
        });
    }

    QString rutaIdle = mirandoDerecha ? ":/Sprites/Lunch/R_Idle_Launch.png"
                                      : ":/Sprites/Lunch/L_Idle_Launch.png";

    QTimer::singleShot(delay * frames.size() + 50, this, [this, rutaIdle]() {
        setSprite(rutaIdle);
    });

    QMediaPlayer* sfxBazuca = new QMediaPlayer(this);
    QAudioOutput* outBazuca = new QAudioOutput(this);
    sfxBazuca->setAudioOutput(outBazuca);
    sfxBazuca->setSource(QUrl("qrc:/Sonidos/Nivel1-S/disparo-bazuca.mp3"));
    outBazuca->setVolume(100);
    sfxBazuca->play();
}

/**
 * Ejecuta el disparo con subfusil (animación corta, proyectil rápido).
 */
void Lunch::dispararSubfusil() {
    if (!transformada) return;

    QStringList frames = mirandoDerecha
                             ? QStringList{":/Sprites/Lunch/R_Subfusil1_Launch.png", ":/Sprites/Lunch/R_Subfusil2_Launch.png",
                                           ":/Sprites/Lunch/R_Subfusil3_Launch.png", ":/Sprites/Lunch/R_Subfusil4_Launch.png"}
                             : QStringList{":/Sprites/Lunch/L_Subfusil1_Launch.png", ":/Sprites/Lunch/L_Subfusil2_Launch.png",
                                           ":/Sprites/Lunch/L_Subfusil3_Launch.png", ":/Sprites/Lunch/L_Subfusil4_Launch.png"};

    for (int i = 0; i < frames.size(); ++i) {
        QTimer::singleShot(75 * i, this, [this, frame = frames[i]]() {
            setSprite(frame);
            sprite->setFixedSize(80, 80);
        });
    }

    QString rutaIdle = mirandoDerecha ? ":/Sprites/Lunch/R_Idle_Launch.png"
                                      : ":/Sprites/Lunch/L_Idle_Launch.png";

    QTimer::singleShot(300, this, [=]() {
        setSprite(rutaIdle);

        QVector2D direccion = mirandoDerecha ? QVector2D(1, 0) : QVector2D(-1, 0);
        QVector2D offset = QVector2D(40 * direccion.x(), 28);
        QVector2D pos = getPosicion() + offset;

        QMediaPlayer* sfxSubfusil = new QMediaPlayer(this);
        QAudioOutput* outSubfusil = new QAudioOutput(this);
        sfxSubfusil->setAudioOutput(outSubfusil);
        sfxSubfusil->setSource(QUrl("qrc:/Sonidos/Nivel1-S/disparo-subfusil.mp3"));
        outSubfusil->setVolume(80);
        sfxSubfusil->play();

        Proyectil* bala = new Proyectil(contenedor, pos, direccion, "subfusil", 12.0f);
        bala->setEsDelJugador(true);
        bala->getSprite()->raise();

        if (Nivel1* nivel = dynamic_cast<Nivel1*>(contenedor)) {
            nivel->agregarProyectil(bala);
        }
    });
}

/**
 * Retorna si Lunch está en su modo agresivo.
 */
bool Lunch::estaEnModoAgresiva() const {
    return transformada;
}
