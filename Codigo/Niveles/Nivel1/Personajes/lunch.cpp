#include "lunch.h"
#include "proyectil.h"

#include <QTimer>
#include <QDebug>

Lunch::Lunch(QWidget* parent)
    : Personaje(parent),
    transformada(false),
    rafagaActiva(false),
    timerRafaga(new QTimer(this)),
    contenedor(parent) // Guardamos el parent que es la escena (Nivel1)
{
    setSprite(":/Sprites/Lunch/R_Idle_BlueLunch.png");
    setPosicion(QVector2D(100, 450));
    sprite->show();
    sprite->raise();

    // Conectamos el temporizador para disparo automático (subfusil)
    connect(timerRafaga, &QTimer::timeout, this, [=]() {
        if (!rafagaActiva || !transformada) return;

        QVector2D direccion = mirandoDerecha ? QVector2D(1, 0) : QVector2D(-1, 0);
        QVector2D offset = QVector2D(40 * direccion.x(), 10);
        QVector2D pos = getPosicion() + offset;

        Proyectil* bala = new Proyectil(contenedor, pos, direccion, "subfusil", 10.0f);
        bala->getSprite()->raise();
    });
}

void Lunch::actualizarSprite() {
    QString ruta;

    if (transformada) {
        ruta = mirandoDerecha
                   ? ":/Sprites/Lunch/R_Idle_Launch.png"
                   : ":/Sprites/Lunch/L_Idle_Launch.png";
    } else {
        ruta = mirandoDerecha
                   ? ":/Sprites/Lunch/R_Idle_BlueLunch.png"
                   : ":/Sprites/Lunch/L_Idle_BlueLunch.png";
    }

    setSprite(ruta);
}

void Lunch::transformar() {
    transformada = !transformada;
    actualizarSprite();
    qDebug() << "Lunch se transforma a modo" << (transformada ? "agresiva" : "tranquila");
}

void Lunch::atacar() {
    if (!transformada) return;

    QString rutaGolpe = mirandoDerecha
                            ? ":/Sprites/Lunch/R_Punching_Launch.png"
                            : ":/Sprites/Lunch/L_Punching_Launch.png";

    setSprite(rutaGolpe);

    QString rutaIdle = mirandoDerecha
                           ? ":/Sprites/Lunch/R_Idle_Launch.png"
                           : ":/Sprites/Lunch/L_Idle_Launch.png";

    QTimer::singleShot(350, this, [this, rutaIdle]() {
        setSprite(rutaIdle);
    });
}

void Lunch::disparar() {
    if (!transformada) return;

    QStringList frames = mirandoDerecha
                             ? QStringList{
                                           ":/Sprites/Lunch/R_Bazooka1_Launch.png",
                                           ":/Sprites/Lunch/R_Bazooka2_Launch.png",
                                           ":/Sprites/Lunch/R_Bazooka3_Launch.png",
                                           ":/Sprites/Lunch/R_Bazooka4_Launch.png"}
                             : QStringList{
                                           ":/Sprites/Lunch/L_Bazooka1_Launch.png",
                                           ":/Sprites/Lunch/L_Bazooka2_Launch.png",
                                           ":/Sprites/Lunch/L_Bazooka3_Launch.png",
                                           ":/Sprites/Lunch/L_Bazooka4_Launch.png"};

    int delay = 200;

    for (int i = 0; i < frames.size(); ++i) {
        QTimer::singleShot(i * delay, this, [this, frame = frames[i]]() {
            setSprite(frame);
        });
    }

    QString rutaIdle = mirandoDerecha
                           ? ":/Sprites/Lunch/R_Idle_Launch.png"
                           : ":/Sprites/Lunch/L_Idle_Launch.png";

    QTimer::singleShot(delay * frames.size() + 50, this, [this, rutaIdle]() {
        setSprite(rutaIdle);
    });
}

void Lunch::comenzarRafaga() {
    if (!transformada || rafagaActiva) return;

    rafagaActiva = true;

    QStringList frames = mirandoDerecha
                             ? QStringList{
                                           ":/Sprites/Lunch/R_Subfusil1_Launch.png",
                                           ":/Sprites/Lunch/R_Subfusil2_Launch.png",
                                           ":/Sprites/Lunch/R_Subfusil3_Launch.png",
                                           ":/Sprites/Lunch/R_Subfusil4_Launch.png"}
                             : QStringList{
                                           ":/Sprites/Lunch/L_Subfusil1_Launch.png",
                                           ":/Sprites/Lunch/L_Subfusil2_Launch.png",
                                           ":/Sprites/Lunch/L_Subfusil3_Launch.png",
                                           ":/Sprites/Lunch/L_Subfusil4_Launch.png"};

    // Mostrar animación (4 frames)
    for (int i = 0; i < frames.size(); ++i) {
        QTimer::singleShot(100 * i, this, [this, frame = frames[i]]() {
            setSprite(frame);
        });
    }

    // Luego del último frame, se mantiene y empieza a disparar
    QTimer::singleShot(400, this, [this, frames]() {
        setSprite(frames.last());
        if (!timerRafaga->isActive()) {
            timerRafaga->start(120); // Dispara cada 120 ms
        }
    });
}

void Lunch::detenerRafaga() {
    if (!transformada) return;

    rafagaActiva = false;
    timerRafaga->stop();
    actualizarSprite();
}

bool Lunch::estaEnModoAgresiva() const {
    return transformada;
}
