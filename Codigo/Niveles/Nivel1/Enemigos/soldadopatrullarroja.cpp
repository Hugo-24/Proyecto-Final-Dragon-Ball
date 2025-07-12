#include "soldadopatrullarroja.h"
#include "proyectil.h"
#include <QDebug>
#include "nivel1.h"

// Constructor
SoldadoPatrullaRoja::SoldadoPatrullaRoja(Nivel1* nivelRef, QWidget* parent, const QVector2D& posInicial, Entidad* objetivo, bool mirarDerecha)
    : Entidad(parent), objetivo(objetivo), mirandoDerecha(mirarDerecha), nivel(nivelRef)
{
    // Inicializa solo la posición lógica (sin mover sprite)
    posicion = posInicial;

    // Corrige la posición visual según el scroll actual
    moverSpriteConOffset(nivel ? nivel->getOffsetX() : 0);
    sprite->setFixedSize(64, 64);
    sprite->show();

    // Cargar sprite idle inicial
    cambiarSprite(mirandoDerecha ? ":/Sprites/SoldadoRR/R_Idle_SoldadoRR.png"
                                 : ":/Sprites/SoldadoRR/L_Idle_SoldadoRR.png");

    // Timer para animación de caminar y disparo
    timerAnimacion = new QTimer(this);
    connect(timerAnimacion, &QTimer::timeout, this, [=]() {
        if (muerto) return;
        if (estado == Caminar) animarCaminar();
        else if (estado == Disparar) animarDisparo();
    });
    timerAnimacion->start(200); // Velocidad de animación: cambia frame cada 200ms

    // Timer para disparar
    timerDisparo = new QTimer(this);
    connect(timerDisparo, &QTimer::timeout, this, &SoldadoPatrullaRoja::disparar);
    timerDisparo->start(tiempoDisparo); // Disparo cada 1000ms (modo fácil)

    // Barra de vida encima del sprite
    barraVida = new QProgressBar(parent);
    barraVida->setRange(0, vida);
    barraVida->setValue(vida);
    barraVida->setFixedSize(60, 6);
    barraVida->setStyleSheet(
        "QProgressBar { border: 1px solid black; background-color: #222; }"
        "QProgressBar::chunk { background-color: red; }"
        );
    barraVida->show();
}

// Cambia el sprite visual del soldado
void SoldadoPatrullaRoja::cambiarSprite(const QString& ruta) {
    if (muerto || !sprite) return;
    QPixmap pix(ruta);
    sprite->setPixmap(pix.scaled(sprite->size())); // Escala el nuevo sprite a 64x64
}

SoldadoPatrullaRoja::~SoldadoPatrullaRoja() {
    // Liberar barra de vida
    if (barraVida) {
        barraVida->deleteLater();
        barraVida = nullptr;
    }

    // Detener y liberar timer de animación
    if (timerAnimacion) {
        timerAnimacion->stop();
        timerAnimacion->deleteLater();
        timerAnimacion = nullptr;
    }

    // Detener y liberar timer de disparo
    if (timerDisparo) {
        timerDisparo->stop();
        timerDisparo->deleteLater();
        timerDisparo = nullptr;
    }
}

// Animación de caminar (3 sprites)
void SoldadoPatrullaRoja::animarCaminar() {
    const QString base = mirandoDerecha ? ":/Sprites/SoldadoRR/R_Walk" : ":/Sprites/SoldadoRR/L_Walk";
    cambiarSprite(base + QString::number((frameActualCaminar % 3) + 1) + "_SoldadoRR.png");
    ++frameActualCaminar; // Avanza frame para la próxima llamada
}

// Animación de disparo (3 sprites)
void SoldadoPatrullaRoja::animarDisparo() {
    const QString base = mirandoDerecha ? ":/Sprites/SoldadoRR/R_Shoot" : ":/Sprites/SoldadoRR/L_Shoot";
    cambiarSprite(base + QString::number((frameActualDisparo % 3) + 1) + "_SoldadoRR.png");
    ++frameActualDisparo;
}

// Lógica completa para disparar proyectiles sincronizada con animación
void SoldadoPatrullaRoja::disparar() {
    if (estado != Disparar || muerto || !nivel) return;

    frameActualDisparo = 0;
    const QString base = mirandoDerecha
                             ? ":/Sprites/SoldadoRR/R_Shoot"
                             : ":/Sprites/SoldadoRR/L_Shoot";

    // Animación de 3 frames (150 ms cada uno)
    for (int i = 0; i < 3; ++i) {
        QTimer::singleShot(i * 150, this, [=]() {
            if (!muerto)
                cambiarSprite(base + QString::number((i % 3) + 1) + "_SoldadoRR.png");
        });
    }

    // Disparo tras animación (450ms después)
    QTimer::singleShot(450, this, [=]() {
        if (muerto || !nivel) return;

        QVector2D posBase = getPosicion();  // POSICIÓN LÓGICA sin offset
        float offsetX = mirandoDerecha ? 30 : -10;
        QVector2D posDisparo = posBase + QVector2D(offsetX, 28);
        QVector2D direccion = mirandoDerecha ? QVector2D(1, 0) : QVector2D(-1, 0);

        Proyectil* bala = new Proyectil(nivel, posDisparo, direccion, "subfusil", 8.0f);
        bala->setEsDelJugador(false);  // Marca que la bala es del enemigo
        bala->getSprite()->raise();    // Asegura que se dibuje encima del soldado

        nivel->agregarProyectil(bala); // Agrega la bala al nivel
    });
}

// Actualiza IA cada frame (llamado desde Nivel1)
void SoldadoPatrullaRoja::actualizar() {
    if (muerto || !objetivo) return;

    // Calcular distancia al jugador
    float distancia = (objetivo->getPosicion() - getPosicion()).length();
    float rangoDisparo = 350.0f;
    float rangoDeteccion = 1200.0f;

    // Determinar dirección
    mirandoDerecha = (objetivo->getPosicion().x() > getPosicion().x());

    // Movimiento IA: actualiza solo la posición lógica
    QVector2D nuevaPos = getPosicion();

    if (distancia > rangoDisparo && distancia < rangoDeteccion) {
        estado = Caminar;
        QVector2D dir = mirandoDerecha ? QVector2D(1, 0) : QVector2D(-1, 0);
        nuevaPos += dir * 1.5f; // Avance rápido
    } else if (distancia <= rangoDisparo) {
        estado = Disparar;
    } else {
        estado = Caminar;
        QVector2D dir = mirandoDerecha ? QVector2D(1, 0) : QVector2D(-1, 0);
        nuevaPos += dir * 1.0f; // Patrullaje más lento fuera del rango
    }

    // Mantener al enemigo en el suelo
    if (nuevaPos.y() != 444) {
        nuevaPos.setY(444);
    }

    // Asignar solo la posición lógica, sin mover sprite
    setPosicionLogica(nuevaPos);

    // Resetear velocidad
    setVelocidad(QVector2D(0, 0));

    // Actualizar barra de vida encima del sprite
    actualizarBarraVida();
}

// Actualiza la barra de vida
void SoldadoPatrullaRoja::actualizarBarraVida() {
    if (!barraVida || !sprite || !sprite->isVisible()) return;

    barraVida->setValue(vida);
    QPoint arriba = sprite->pos() + QPoint(2, -10); // Posición relativa encima del sprite
    barraVida->move(arriba);
}

// Recibe daño del jugador
void SoldadoPatrullaRoja::recibirDanio(int cantidad) {
    if (muerto) return;

    vida -= cantidad;
    if (vida <= 0) {
        iniciarMuerte();
    } else {
        actualizarBarraVida();
    }
}

// Marca al enemigo como muerto y lo elimina
void SoldadoPatrullaRoja::iniciarMuerte() {
    muerto = true;
    estado = Muriendo;

    // Cambiar sprite a imagen de muerte
    QString rutaMuerte = mirandoDerecha ? ":/Sprites/SoldadoRR/R_Soldado_Muerto.png"
                                        : ":/Sprites/SoldadoRR/L_Soldado_Muerto.png";
    if (sprite) {
        sprite->setPixmap(QPixmap(rutaMuerte).scaled(sprite->size()));
    }

    // Ocultar y eliminar barra de vida
    if (barraVida) {
        barraVida->hide();
        barraVida = nullptr;
    }

    // Detener y eliminar timers
    if (timerAnimacion) {
        timerAnimacion->stop();
        timerAnimacion = nullptr;
    }
    if (timerDisparo) {
        timerDisparo->stop();
        timerDisparo = nullptr;
    }

    // Esperar unos segundos para mostrar sprite de muerte antes de eliminar
    QTimer::singleShot(1000, this, [this]() {
        if (sprite) {
            sprite->hide();
            sprite = nullptr;
        }
    });
}

bool SoldadoPatrullaRoja::estaMuerto() const {
    return muerto;
}
