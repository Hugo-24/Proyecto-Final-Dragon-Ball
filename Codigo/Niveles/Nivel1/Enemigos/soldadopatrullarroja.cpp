#include "soldadopatrullarroja.h"
#include "proyectil.h"
#include <QDebug>
#include "nivel1.h"
// Constructor
SoldadoPatrullaRoja::SoldadoPatrullaRoja(QWidget* parent, const QVector2D& posInicial, Entidad* objetivo, bool mirarDerecha)
    : Entidad(parent), objetivo(objetivo), mirandoDerecha(mirarDerecha)
{
    setPosicion(posInicial);
    sprite->setFixedSize(64, 64);
    sprite->show();

    // Cargar sprite idle inicial
    cambiarSprite(mirandoDerecha ? ":/Sprites/SoldadoRR/R_Idle_SoldadoRR.png"
                                 : ":/Sprites/SoldadoRR/L_Idle_SoldadoRR.png");

    // Timer para animación
    timerAnimacion = new QTimer(this);
    connect(timerAnimacion, &QTimer::timeout, this, [=]() {
        if (estado == Caminar) animarCaminar();
        else if (estado == Disparar) animarDisparo();
    });
    timerAnimacion->start(200); // Cada 200 ms cambia el frame

    // Timer para disparo
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

// Cambia el sprite visual
void SoldadoPatrullaRoja::cambiarSprite(const QString& ruta) {
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

// Lógica para disparar proyectiles
void SoldadoPatrullaRoja::disparar() {
    if (estado != Disparar || muerto) return;

    QVector2D direccion = mirandoDerecha ? QVector2D(1, 0) : QVector2D(-1, 0);
    QVector2D posicionDisparo = getPosicion() + QVector2D(10, 10); // Ajustar según sprite

    Proyectil* bala = new Proyectil(sprite->parentWidget(), posicionDisparo, direccion, "subfusil", 10.0f);
    bala->getSprite()->raise();

    // AGREGAR proyectil al nivel para que se actualice y colisione
    Nivel1* nivel = qobject_cast<Nivel1*>(sprite->window());
    if (nivel) {
        nivel->agregarProyectil(bala);
    } else {
        qDebug() << "No se pudo acceder al Nivel1 para agregar proyectil enemigo";
    }
}

// Se actualiza cada frame (llamado desde Nivel1)
void SoldadoPatrullaRoja::actualizar() {
    if (muerto) return;

    // Calcular distancia al jugador
    float distancia = (objetivo->getPosicion() - getPosicion()).length();

    float rangoDisparo = 150.0f;
    float rangoDeteccion = 200.0f;

    // Determinar dirección
    mirandoDerecha = (objetivo->getPosicion().x() > getPosicion().x());

    // Si está lejos, caminar hacia el jugador
    if (distancia > rangoDisparo && distancia < rangoDeteccion) {
        estado = Caminar;
        QVector2D dir = mirandoDerecha ? QVector2D(1, 0) : QVector2D(-1, 0);
        setPosicion(getPosicion() + dir * 1.5f);
    }
    else if (distancia <= rangoDisparo) {
        estado = Disparar;
    } else {
        estado = Caminar; // Seguir caminando aleatoriamente si no lo ve
    }

    // Asegurar que estén sobre el suelo del mismo nivel que Roshi (444px)
    QVector2D pos = getPosicion();
    if (pos.y() != 444) {
        pos.setY(444);
        setPosicion(pos);
    }

    actualizarBarraVida();
}
// Recibe daño
void SoldadoPatrullaRoja::recibirDanio(int cantidad) {
    if (muerto) return;

    vida -= cantidad;
    if (vida <= 0) {
        iniciarMuerte();
    } else {
        actualizarBarraVida();
    }
}

// Muestra barra sobre sprite
void SoldadoPatrullaRoja::actualizarBarraVida() {
    if (!barraVida) return;

    barraVida->setValue(vida);
    QPoint arriba = sprite->pos() + QPoint(2, -10);
    barraVida->move(arriba);
}

// Marca como muerto
void SoldadoPatrullaRoja::iniciarMuerte() {
    muerto = true;
    estado = Muriendo;

    // Mostrar sprite de muerte
    QString rutaMuerte = mirandoDerecha ? ":/Sprites/SoldadoRR/R_Soldado_Muerto.png"
                                        : ":/Sprites/SoldadoRR/L_Soldado_Muerto.png";
    cambiarSprite(rutaMuerte);
    barraVida->hide();

    // Eliminar luego de 3 segundos
    QTimer::singleShot(3000, this, [=]() {
        sprite->hide();
        barraVida->deleteLater();
        deleteLater();
    });
}

bool SoldadoPatrullaRoja::estaMuerto() const {
    return muerto;
}
