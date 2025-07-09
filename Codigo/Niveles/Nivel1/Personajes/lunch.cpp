#include "lunch.h"
#include "proyectil.h"
#include "nivel1.h"  // Para permitir agregar proyectiles a la escena

#include <QTimer>
#include <QDebug>

// Constructor
Lunch::Lunch(QWidget* parent)
    : Personaje(parent),
    transformada(false),
    rafagaActiva(false), // Ya no se usa activamente
    timerRafaga(new QTimer(this)), // Ya no se usa activamente
    contenedor(parent) // Guardamos el parent que es la escena (Nivel1)
{
    // Sprite inicial: Lunch en modo tranquila (rubia con moño azul)
    setSprite(":/Sprites/Lunch/R_Idle_BlueLunch.png");
    setPosicion(QVector2D(100, 450));
    sprite->show();
    sprite->raise();
}

// Actualiza el sprite según el movimiento y estado
void Lunch::actualizarSprite() {
    if (estaMuerto) return;
    static bool alternar = false;
    QString ruta;

    if (getVelocidad().x() != 0) { // Si se está moviendo horizontalmente
        if (transformada) {
            ruta = mirandoDerecha
                       ? (alternar ? ":/Sprites/Lunch/R_Walk1_Launch.png"
                                   : ":/Sprites/Lunch/R_Walk2_Launch.png")
                       : (alternar ? ":/Sprites/Lunch/L_Walk1_Launch.png"
                                   : ":/Sprites/Lunch/L_Walk2_Launch.png");
        } else {
            ruta = mirandoDerecha
                       ? (alternar ? ":/Sprites/Lunch/R_Walk1_BlueLunch.png"
                                   : ":/Sprites/Lunch/R_Walk2_BlueLunch.png")
                       : (alternar ? ":/Sprites/Lunch/L_Walk1_BlueLunch.png"
                                   : ":/Sprites/Lunch/L_Walk2_BlueLunch.png");
        }
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

// Alterna entre modo agresivo y modo tranquila
void Lunch::transformar() {
    setPuedeDisparar(true); // Asegura que pueda volver a disparar tras la transformación
    transformada = !transformada;
    actualizarSprite();
    qDebug() << "Lunch se transforma a modo" << (transformada ? "agresiva" : "tranquila");
}

// Disparo de bazuca (solo en modo agresiva)
void Lunch::disparar() {
    if (!transformada) return; // Solo puede usar bazuca en modo agresiva

    // Animación bazuca (4 frames)
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

    // Volver a sprite idle después de la animación
    QString rutaIdle = mirandoDerecha
                           ? ":/Sprites/Lunch/R_Idle_Launch.png"
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

// Disparo de subfusil (ráfaga rápida)
void Lunch::dispararSubfusil() {
    if (!transformada) return; // Solo puede usar subfusil en modo agresiva

    // Animación subfusil (más rápida que la bazuca)
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

    for (int i = 0; i < frames.size(); ++i) {
        QTimer::singleShot(75 * i, this, [this, frame = frames[i]]() {
            setSprite(frame);
            sprite->setFixedSize(80, 80); // Ajusta aquí el tamaño deseado
        });
    }

    // Sprite de regreso a idle
    QString rutaIdle = mirandoDerecha
                           ? ":/Sprites/Lunch/R_Idle_Launch.png"
                           : ":/Sprites/Lunch/L_Idle_Launch.png";

    // Lanzar proyectil luego de la animación (300ms aprox)
    QTimer::singleShot(300, this, [=]() {
        setSprite(rutaIdle);

        QVector2D direccion = mirandoDerecha ? QVector2D(1, 0) : QVector2D(-1, 0);
        QVector2D offset = QVector2D(40 * direccion.x(), 28); // Offset visual
        QVector2D pos = getPosicion() + offset;

        // Sonido subfusil
        QMediaPlayer* sfxSubfusil = new QMediaPlayer(this);
        QAudioOutput* outSubfusil = new QAudioOutput(this);
        sfxSubfusil->setAudioOutput(outSubfusil);
        sfxSubfusil->setSource(QUrl("qrc:/Sonidos/Nivel1-S/disparo-subfusil.mp3"));
        outSubfusil->setVolume(80);
        sfxSubfusil->play();

        // Crear proyectil subfusil
        Proyectil* bala = new Proyectil(contenedor, pos, direccion, "subfusil", 12.0f); // Más veloz que bazuca
        bala->setEsDelJugador(true);  // ← ¡SOLUCIONA el bug del daño al jugador!
        bala->getSprite()->raise();

        // Agregarlo al nivel si es posible
        if (Nivel1* nivel = dynamic_cast<Nivel1*>(contenedor)) {
            nivel->agregarProyectil(bala);
        }
    });
}

// Getter del estado de transformación
bool Lunch::estaEnModoAgresiva() const {
    return transformada;
}
