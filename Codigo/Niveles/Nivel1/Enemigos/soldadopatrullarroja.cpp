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
    timerAnimacion->start(200);

    // Timer para disparar
    timerDisparo = new QTimer(this);
    connect(timerDisparo, &QTimer::timeout, this, &SoldadoPatrullaRoja::disparar);
    timerDisparo->start(tiempoDisparo);

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
    sprite->setPixmap(pix.scaled(sprite->size()));
}

// Animación de caminar (3 sprites)
void SoldadoPatrullaRoja::animarCaminar() {
    const QString base = mirandoDerecha ? ":/Sprites/SoldadoRR/R_Walk" : ":/Sprites/SoldadoRR/L_Walk";
    cambiarSprite(base + QString::number((frameActualCaminar % 3) + 1) + "_SoldadoRR.png");
    ++frameActualCaminar;
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

        Proyectil* bala = new Proyectil(nivel, posDisparo, direccion, "subfusil", 6.0f);
        bala->setEsDelJugador(false);
        bala->getSprite()->raise();

        nivel->agregarProyectil(bala);
    });
}

// Actualiza IA cada frame (llamado desde Nivel1)
void SoldadoPatrullaRoja::actualizar() {
    if (muerto || !objetivo) return;

    // Calcular distancia al jugador
    float distancia = (objetivo->getPosicion() - getPosicion()).length();
    float rangoDisparo = 450.0f;
    float rangoDeteccion = 600.0f;

    // Determinar dirección
    mirandoDerecha = (objetivo->getPosicion().x() > getPosicion().x());

    // Movimiento IA: actualiza solo la posición lógica
    QVector2D nuevaPos = getPosicion();

    if (distancia > rangoDisparo && distancia < rangoDeteccion) {
        estado = Caminar;
        QVector2D dir = mirandoDerecha ? QVector2D(1, 0) : QVector2D(-1, 0);
        nuevaPos += dir * 1.5f;
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
    QPoint arriba = sprite->pos() + QPoint(2, -10);
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

    // Cambiar sprite a muerto
    QString rutaMuerte = mirandoDerecha ? ":/Sprites/SoldadoRR/R_Soldado_Muerto.png"
                                        : ":/Sprites/SoldadoRR/L_Soldado_Muerto.png";
    cambiarSprite(rutaMuerte);

    if (barraVida) {
        barraVida->hide();
        barraVida->deleteLater();
        barraVida = nullptr;
    }

    if (timerAnimacion) {
        timerAnimacion->stop();
        timerAnimacion->deleteLater();
        timerAnimacion = nullptr;
    }

    if (timerDisparo) {
        timerDisparo->stop();
        timerDisparo->deleteLater();
        timerDisparo = nullptr;
    }

    if (sprite) {
        sprite->deleteLater();
        sprite = nullptr;
    }

    QTimer::singleShot(2000, this, [=]() {
        if (sprite) sprite->hide();
        deleteLater();
    });
}

bool SoldadoPatrullaRoja::estaMuerto() const {
    return muerto;
}
