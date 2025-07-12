#ifndef SOLDADOPATRULLAROJA_H
#define SOLDADOPATRULLAROJA_H

#include "entidad.h"
#include <QTimer>
#include <QProgressBar>
#include <QObject>
#include "proyectil.h"

class Nivel1; // Declaración adelantada para evitar inclusión circular

/**
 * Clase que representa un enemigo tipo Soldado de la Patrulla Roja.
 * Camina hasta una distancia del jugador, luego se detiene y dispara.
 * Tiene animaciones, barra de vida, y desaparece tras morir.
 */
class SoldadoPatrullaRoja : public QObject, public Entidad {
    Q_OBJECT // Necesario para usar señales y slots en esta clase

signals:
    // Señal que emite al crear un proyectil
    void proyectilCreado(Proyectil* p);

public:
    // Constructor
    SoldadoPatrullaRoja(Nivel1* nivel, QWidget* parent, const QVector2D& posInicial, Entidad* objetivo, bool mirarDerecha = true);

    // Método llamado cada frame para actualizar comportamiento
    void actualizar() override;

    // Lógica de daño y muerte
    void recibirDanio(int cantidad);
    bool estaMuerto() const;

    // Acceso a la barra de vida (útil desde el nivel)
    QProgressBar* getBarraVida() const { return barraVida; }
    int getVida() const { return vida; }

    ~SoldadoPatrullaRoja();

private:
    // Métodos de animación y disparo
    void animarCaminar();
    void animarDisparo();
    void disparar();

    // Referencia al jugador o entidad objetivo
    Entidad* objetivo;

    // Dirección actual del sprite
    bool mirandoDerecha;

    // Estado lógico
    bool muerto = false;

    // Vida y lógica de disparo
    int vida = 30;
    int tiempoDisparo = 1000; // Cada 1000 ms dispara

    // Temporizadores para animaciones y disparos
    QTimer* timerAnimacion;
    QTimer* timerDisparo;

    // Control de fotogramas para animaciones
    int frameActualCaminar = 0;
    int frameActualDisparo = 0;

    // Estados posibles del enemigo
    enum Estado { Caminar, Disparar, Muriendo, Inactivo };
    Estado estado = Caminar;

    // Barra de vida flotante sobre el sprite
    QProgressBar* barraVida = nullptr;

    // Referencia al nivel actual (Nivel1)
    Nivel1* nivel = nullptr;

    // Métodos auxiliares
    void actualizarBarraVida();
    void cambiarSprite(const QString& ruta);
    void iniciarMuerte();
};

#endif //SOLDADOPATRULLAROJA_H
