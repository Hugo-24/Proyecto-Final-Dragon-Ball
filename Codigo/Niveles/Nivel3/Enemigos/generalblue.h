#pragma once

#include "entidad.h"
#include <QTimer>
#include <QProgressBar>
#include <QObject>
#include <QLabel>           // Para mostrar el mensaje "ZA WARUDO!"
#include <QTimer>           // Para temporizadores de animación
#include <QMediaPlayer>     // Para reproducir el sonido
#include <QAudioOutput>     // Para la salida de audio
class Nivel3;
class Goku;
class Proyectil;

/**
 * Clase GeneralBlue: Jefe del Nivel 3 con IA, animaciones, y múltiples ataques.
 */
class GeneralBlue : public QObject, public Entidad {
    Q_OBJECT

public:
    GeneralBlue(Nivel3* nivel, QWidget* parent, const QVector2D& posicionInicial, Goku* objetivo);
    ~GeneralBlue();

    void actualizar() override;              // Lógica de movimiento y orientación
    void recibirDanio(int cantidad);         // Recibe daño y verifica interrupción o muerte
    bool estaDerrotado() const;              // Si fue vencido
    QProgressBar* getBarraVida() const;      // Para gestión externa (opcional)
    int getVida() const;

private:
    void moverHaciaJugador();                // Movimiento automático hasta distancia fija
    void iniciarAtaque();                    // Escoge y lanza ataque
    void animarDisparo();                    // Subfusil (5 sprites + proyectil)
    void lanzarRoca();                       // Roca (5 sprites + proyectil)
    void usarTelequinesis();                 // Paraliza a Goku (si no es interrumpido)
    void soltarCorazon();                    // Suelta un corazón en el mapa
    void morir();                            // Muerte con animación
    void cambiarSprite(const QString& ruta);// Cambiar sprite según dirección
    void saltar();                           // Salto vertical
    void actualizarSprite();           // Animación de caminar alternando sprites
    Nivel3* nivel;
    Goku* objetivo;

    int vida = 250;
    int vidaMaxima = 250;
    int vidaAnteriorParaCuracion = 250;

    bool derrotado = false;
    bool ejecutandoTelequinesis = false;
    bool mirandoDerecha = true;
    bool caminando = false;
    bool enElAire = false;
    bool estaAtacando = false;

    QMediaPlayer* sfxTelequinesis = nullptr;
    QAudioOutput* salidaTelequinesis = nullptr;
    QLabel* labelZaWarudo = nullptr;

    QVector2D velocidad = QVector2D(0, 0);

    QTimer* timerAtaque = nullptr;
    QTimer* timerAnimacion = nullptr;

    QProgressBar* barraVida = nullptr;
};
