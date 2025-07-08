#pragma once

#include "entidad.h"
#include <QTimer>
#include <QProgressBar>
#include <QObject>
#include "proyectil.h"
/**
 * Clase que representa un enemigo tipo Soldado de la Patrulla Roja.
 * Camina hasta una distancia del jugador, luego se detiene y dispara.
 * Tiene animaciones, barra de vida, y desaparece tras morir.
 */
class SoldadoPatrullaRoja : public QObject, public Entidad {
    Q_OBJECT // Necesario para señales/slots
signals:
    void proyectilCreado(Proyectil* p);
public:
    SoldadoPatrullaRoja(QWidget* parent, const QVector2D& posInicial, Entidad* objetivo, bool mirarDerecha = true);

    void actualizar() override;
    void recibirDanio(int cantidad);
    bool estaMuerto() const;

private:
    void animarCaminar();
    void animarDisparo();
    void disparar();

    Entidad* objetivo;       // A quién apunta (el jugador)
    bool mirandoDerecha;     // Dirección del sprite
    bool muerto = false;

    int vida = 20;           // Vida inicial
    int tiempoDisparo = 1500; // Modo fácil: 1.5 segundos por disparo

    // Animación y control
    QTimer* timerAnimacion;
    QTimer* timerDisparo;

    int frameActualCaminar = 0;
    int frameActualDisparo = 0;

    enum Estado { Caminar, Disparar, Muriendo, Inactivo };
    Estado estado = Caminar;

    QProgressBar* barraVida = nullptr;

    void actualizarBarraVida();
    void cambiarSprite(const QString& ruta);

    void iniciarMuerte();
};
