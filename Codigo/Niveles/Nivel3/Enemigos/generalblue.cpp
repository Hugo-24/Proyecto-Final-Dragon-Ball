#include "generalblue.h"
#include "nivel3.h"
#include "goku.h"
#include "proyectil.h"
#include "corazon.h"

#include <QMediaPlayer>
#include <QAudioOutput>
#include <QRandomGenerator>

/**
 * Constructor: Inicializa todo el jefe (IA, sprite, timers, barra de vida).
 */
GeneralBlue::GeneralBlue(Nivel3* nivel, QWidget* parent, const QVector2D& posicionInicial, Goku* objetivo)
    : Entidad(parent), nivel(nivel), objetivo(objetivo)
{
    this->posicion = posicionInicial;

    // Sprite del jefe
    sprite->setFixedSize(80, 80);
    moverSpriteConOffset(0); // Aplica la posición al QLabel
    sprite->show();

    // Orientación inicial
    mirandoDerecha = objetivo && objetivo->getPosicion().x() > posicion.x();
    cambiarSprite(mirandoDerecha
                      ? ":/Sprites/GeneralBlue/R_Idle_GeneralBlue.png"
                      : ":/Sprites/GeneralBlue/L_Idle_GeneralBlue.png");

    vidaAnteriorParaCuracion = vida;
    // Timer de ataques automáticos
    timerAtaque = new QTimer(this);
    connect(timerAtaque, &QTimer::timeout, this, &GeneralBlue::iniciarAtaque);
    timerAtaque->start(1000);  // Cada 1 segundo ataca
}

/**
 * Destructor: detiene timers y limpia memoria.
 */
GeneralBlue::~GeneralBlue() {
    if (timerAtaque) timerAtaque->stop();
    if (barraVida) barraVida->deleteLater();
}

/**
 * Mueve al jefe hacia el jugador hasta cierta distancia.
 */
void GeneralBlue::actualizar() {
    if (derrotado || !objetivo || ejecutandoTelequinesis) return;
    moverHaciaJugador();

    // Gravedad
    if (enElAire) {
        velocidad.setY(velocidad.y() + 0.6f); // gravedad suave
        posicion += velocidad;
        if (posicion.y() >= 444) {
            posicion.setY(444);
            velocidad.setY(0);
            enElAire = false;
        }
    }

    // Mover sprite visualmente
    moverSpriteConOffset(nivel->getOffsetX());

    // No actualizar sprite si está en animación especial (ej: telequinesis)
    if (!ejecutandoTelequinesis) {
        actualizarSprite();
    }

    // Barra de vida
    if (sprite && barraVida) {
        barraVida->setValue(vida);
        barraVida->move(sprite->pos() + QPoint(10, -12));
    }
}
/**
 * Lógica de persecución: se mueve hasta estar cerca del jugador (200 px).
 */
void GeneralBlue::moverHaciaJugador() {
    QVector2D posJugador = objetivo->getPosicion();
    float distancia = (posJugador - posicion).length();

    // Cambiar dirección solo si hay diferencia significativa
    if (qAbs(posJugador.x() - posicion.x()) > 10.0f) {
        mirandoDerecha = posJugador.x() > posicion.x();
        cambiarSprite(mirandoDerecha ? ":/Sprites/GeneralBlue/R_Idle_GeneralBlue.png"
                                     : ":/Sprites/GeneralBlue/L_Idle_GeneralBlue.png");
    }

    // Solo se mueve si está lejos
    if (distancia > 400.0f) {
        float desplazamiento = 3.5f;
        velocidad.setX(mirandoDerecha ? desplazamiento : -desplazamiento);
        posicion += QVector2D(velocidad.x(), 0);
    } else {
        velocidad.setX(0);
    }

    // Mantener en el suelo si no está en el aire
    if (!enElAire) posicion.setY(444);
}
/**
 * Decide y lanza uno de los 3 ataques posibles.
 */
void GeneralBlue::iniciarAtaque() {
    if (derrotado|| !nivel->controlesHabilitados) return;  // YA NO se bloquea por ejecutandoTelequinesis

    // Si Goku está en el aire, saltar para alcanzarlo
    if (objetivo && objetivo->getPosicion().y() < 444 && !enElAire) {
        saltar();
        return;
    }
    // Selección normal de ataque aleatorio
    signed short int aleatorio = QRandomGenerator::global()->bounded(5);
    switch (aleatorio) {
    case 0: animarDisparo();QTimer::singleShot(500, this, [=]() {animarDisparo();});QTimer::singleShot(500, this, [=]() {animarDisparo();}); break;
    case 1: lanzarRoca(); break;
    case 2: usarTelequinesis(); break;
    case 3: animarDisparo();QTimer::singleShot(500, this, [=]() {animarDisparo();});QTimer::singleShot(500, this, [=]() {animarDisparo();}); break;
    case 4: lanzarRoca(); break;
    }
}

/**
 * Ataque de subfusil: 5 sprites + proyectil y sonido.
 */
void GeneralBlue::animarDisparo() {
    // QStringList es una lista de cadenas de texto (strings). Se usa para almacenar rutas de sprites.
    QStringList frames = mirandoDerecha
                             ? QStringList{
                                   ":/Sprites/GeneralBlue/R_Shoot1.png",
                                   ":/Sprites/GeneralBlue/R_Shoot2.png",
                                   ":/Sprites/GeneralBlue/R_Shoot3.png",
                                   ":/Sprites/GeneralBlue/R_Shoot4.png",
                                   ":/Sprites/GeneralBlue/R_Shoot5.png"
                               }
                             : QStringList{
                                   ":/Sprites/GeneralBlue/L_Shoot1.png",
                                   ":/Sprites/GeneralBlue/L_Shoot2.png",
                                   ":/Sprites/GeneralBlue/L_Shoot3.png",
                                   ":/Sprites/GeneralBlue/L_Shoot4.png",
                                   ":/Sprites/GeneralBlue/L_Shoot5.png"
                               };

    int delay = 100; // Tiempo en milisegundos entre cada frame

    // Se programan 5 llamadas con delay creciente para mostrar cada frame de la animación
    for (int i = 0; i < frames.size(); ++i) {
        QTimer::singleShot(i * delay, this, [=]() {
            try {
                // Mostrar el frame i de disparo
                cambiarSprite(frames[i]);
            } catch (const std::exception& e) {
                // En caso de fallo, se muestra el error en consola
                qDebug() << "[EXCEPCIÓN Sprite disparo]: " << e.what();
            }
        });
    }

    // Una vez terminada la animación, se ejecuta el disparo del proyectil
    QTimer::singleShot(frames.size() * delay + 20, this, [=]() {
        try {
            // Se vuelve al sprite idle (reposo)
            cambiarSprite(mirandoDerecha ? ":/Sprites/GeneralBlue/R_Idle_GeneralBlue.png"
                                         : ":/Sprites/GeneralBlue/L_Idle_GeneralBlue.png");
        } catch (const std::exception& e) {
            qDebug() << "[EXCEPCIÓN Idle disparo]: " << e.what();
        }

        // Dirección del disparo
        QVector2D dir = mirandoDerecha ? QVector2D(1, 0) : QVector2D(-1, 0);

        // Posición inicial de la bala, desplazada desde el centro del personaje
        QVector2D posBala = getPosicion() + QVector2D(mirandoDerecha ? 60 : -20, 20);

        try {
            // Se crea el proyectil
            Proyectil* bala = new Proyectil(nivel, posBala, dir, "subfusil", 12.0f);
            bala->setEsDelJugador(false); // Es enemigo
            nivel->agregarProyectil(bala);
        } catch (const std::exception& e) {
            qDebug() << "[EXCEPCIÓN Proyectil disparo]: " << e.what();
        }

        // Sonido de disparo
        QMediaPlayer* sfx = new QMediaPlayer(this);
        QAudioOutput* out = new QAudioOutput(this);
        sfx->setAudioOutput(out);
        sfx->setSource(QUrl("qrc:/Sonidos/Nivel1-S/disparo-subfusil.mp3"));
        out->setVolume(100);
        sfx->play();
    });
}

void GeneralBlue::lanzarRoca() {
    // Carga los sprites de animación del ataque de roca
    QStringList frames = mirandoDerecha
                             ? QStringList{
                                   ":/Sprites/GeneralBlue/R_Roca1.png",
                                   ":/Sprites/GeneralBlue/R_Roca2.png",
                                   ":/Sprites/GeneralBlue/R_Roca3.png",
                                   ":/Sprites/GeneralBlue/R_Roca4.png",
                                   ":/Sprites/GeneralBlue/R_Roca5.png"
                               }
                             : QStringList{
                                   ":/Sprites/GeneralBlue/L_Roca1.png",
                                   ":/Sprites/GeneralBlue/L_Roca2.png",
                                   ":/Sprites/GeneralBlue/L_Roca3.png",
                                   ":/Sprites/GeneralBlue/L_Roca4.png",
                                   ":/Sprites/GeneralBlue/L_Roca5.png"
                               };

    int delay = 140; // Tiempo entre cada frame de la animación

    // Mostrar cada sprite de la animación en orden
    for (int i = 0; i < frames.size(); ++i) {
        QTimer::singleShot(i * delay, this, [=]() {
            try {
                cambiarSprite(frames[i]);
            } catch (const std::exception& e) {
                qDebug() << "[EXCEPCIÓN Sprite roca]: " << e.what();
            }
        });
    }

    // Al final, lanzar el proyectil "roca"
    QTimer::singleShot(frames.size() * delay + 20, this, [=]() {
        try {
            cambiarSprite(mirandoDerecha ? ":/Sprites/GeneralBlue/R_Idle_GeneralBlue.png"
                                         : ":/Sprites/GeneralBlue/L_Idle_GeneralBlue.png");
        } catch (const std::exception& e) {
            qDebug() << "[EXCEPCIÓN Idle roca]: " << e.what();
        }

        QVector2D dir = mirandoDerecha ? QVector2D(1, 0) : QVector2D(-1, 0);
        QVector2D pos = getPosicion() + QVector2D(mirandoDerecha ? 30 : -40, -20);

        try {
            Proyectil* roca = new Proyectil(nivel, pos, dir, "roca", 8.0f);
            roca->setEsDelJugador(false);
            nivel->agregarProyectil(roca);
        } catch (const std::exception& e) {
            qDebug() << "[EXCEPCIÓN Proyectil roca]: " << e.what();
        }
    });
}

void GeneralBlue::usarTelequinesis() {
    // Si ya está usando la habilidad, no repetirla
    if (ejecutandoTelequinesis) return;

    // Sonido de habilidad después de un leve retraso
    QTimer::singleShot(600, this, [=]() {
        QMediaPlayer* sfx = new QMediaPlayer(this);
        QAudioOutput* out = new QAudioOutput(this);
        sfx->setAudioOutput(out);
        sfx->setSource(QUrl("qrc:/Sonidos/Nivel3-S/efecto-za-warudo.mp3"));
        out->setVolume(100);
        sfx->play();
    });

    ejecutandoTelequinesis = true;
    qDebug() << "Telequinesis iniciada";

    // Lista de sprites para la animación de telequinesis
    QStringList frames = mirandoDerecha
                             ? QStringList{
                                   ":/Sprites/GeneralBlue/R_TK1.png",
                                   ":/Sprites/GeneralBlue/R_TK2.png",
                                   ":/Sprites/GeneralBlue/R_TK3.png",
                                   ":/Sprites/GeneralBlue/R_TK4.png"
                               }
                             : QStringList{
                                   ":/Sprites/GeneralBlue/L_TK1.png",
                                   ":/Sprites/GeneralBlue/L_TK2.png",
                                   ":/Sprites/GeneralBlue/L_TK3.png",
                                   ":/Sprites/GeneralBlue/L_TK4.png"
                               };

    int delay = 150;  // Tiempo entre frames
    int total = frames.size() * delay;  // Duración total de la animación

    // Ejecutar la animación frame por frame
    for (int i = 0; i < frames.size(); ++i) {
        QTimer::singleShot(i * delay, this, [=]() {
            try {
                cambiarSprite(frames[i]);
            } catch (const std::exception& e) {
                qDebug() << "[EXCEPCIÓN Sprite telequinesis]: " << e.what();
            }
        });
    }

    // Aplicar el efecto paralizante al jugador después de la animación
    QTimer::singleShot(total + 100, this, [=]() {
        try {
            cambiarSprite(mirandoDerecha
                              ? ":/Sprites/GeneralBlue/R_Idle_GeneralBlue.png"
                              : ":/Sprites/GeneralBlue/L_Idle_GeneralBlue.png");
        } catch (const std::exception& e) {
            qDebug() << "[EXCEPCIÓN Idle telequinesis]: " << e.what();
        }

        // Si hay objetivo (jugador), paralizarlo
        if (objetivo) {
            objetivo->setPuedeDisparar(false);
            objetivo->setPuedeMoverse(false);

            // Después de 4 segundos, se libera
            QTimer::singleShot(4000, this, [=]() {
                objetivo->setPuedeDisparar(true);
                objetivo->setPuedeMoverse(true);
                ejecutandoTelequinesis = false;
                qDebug() << "Telequinesis terminada";
            });
        }
    });
}

void GeneralBlue::saltar() {
    if (!enElAire) {
        velocidad.setY(-16.0f); // impulso hacia arriba
        enElAire = true;
    }
    signed short int aleatorio = QRandomGenerator::global()->bounded(3); // Ahora con 4 posibles
    switch (aleatorio) {
    case 0: animarDisparo();QTimer::singleShot(500, this, [=]() {animarDisparo();});QTimer::singleShot(500, this, [=]() {animarDisparo();}); break;
    case 1: lanzarRoca(); break;
    case 2: usarTelequinesis(); break;
    }
}
/**
 * Recibe daño, interrumpe ataques y lanza corazones.
 */
void GeneralBlue::recibirDanio(int cantidad) {
    if (derrotado) return;

    vida -= cantidad;

    if (vida <= 0) {
        morir();
        return;
    }

    // Solo soltar corazón cada 1/8 de vida perdida
    int vidaActualSegmento = vida / (vidaMaxima / 8);
    int vidaAnteriorSegmento = vidaAnteriorParaCuracion / (vidaMaxima / 8);
    if (vidaActualSegmento < vidaAnteriorSegmento) {
        vidaAnteriorParaCuracion = vida;
        soltarCorazon();
    }
}

/**
 * Crea un corazón curativo en el escenario.
 */
void GeneralBlue::soltarCorazon() {
    if (!nivel) return;
    QVector2D pos = getPosicion();
    Corazon* corazon = new Corazon(nivel, pos, nivel); // QWidget* y Nivel* (mismo puntero)
    nivel->agregarCorazon(corazon);
}

/**
 * Marca derrota y lanza animación final.
 */
void GeneralBlue::morir() {
    derrotado = true;
    cambiarSprite(mirandoDerecha ? ":/Sprites/GeneralBlue/R_Muerto.png"
                                 : ":/Sprites/GeneralBlue/L_Muerto.png");
    timerAtaque->stop();
    QTimer::singleShot(3000, this, [=]() {
        if (sprite) sprite->hide();
        if (barraVida) barraVida->hide();
        deleteLater();
    });
}

/**
 * Cambia sprite según dirección.
 */
void GeneralBlue::cambiarSprite(const QString& ruta) {
    if (sprite) {
        QPixmap pix(ruta);
        sprite->setPixmap(pix.scaled(sprite->size()));
    }
}
void GeneralBlue::actualizarSprite() {
    if (derrotado || ejecutandoTelequinesis) return;

    static bool alternar = false;
    QString ruta;

    if (velocidad.x() != 0) {
        ruta = mirandoDerecha
                   ? (alternar ? ":/Sprites/GeneralBlue/R_Walk1.png" : ":/Sprites/GeneralBlue/R_Walk2.png")
                   : (alternar ? ":/Sprites/GeneralBlue/L_Walk1.png" : ":/Sprites/GeneralBlue/L_Walk2.png");
        alternar = !alternar;
    } else {
        ruta = mirandoDerecha
                   ? ":/Sprites/GeneralBlue/R_Idle_GeneralBlue.png"
                   : ":/Sprites/GeneralBlue/L_Idle_GeneralBlue.png";
    }

    cambiarSprite(ruta);
}
/**
 * Devuelve si fue derrotado.
 */
bool GeneralBlue::estaDerrotado() const {
    return derrotado;
}

/**
 * Devuelve la barra de vida (por si se desea manipular externamente).
 */
QProgressBar* GeneralBlue::getBarraVida() const {
    return barraVida;
}
int GeneralBlue::getVida() const {
    return vida;
}
