#ifndef GENERALBLUE_H
#define GENERALBLUE_H

#include "entidad.h"        // Clase base que representa cualquier entidad con sprite y posición
#include <QTimer>           // Temporizador para ataques y animaciones del jefe
#include <QProgressBar>     // Barra visual que muestra la vida del jefe
#include <QObject>          // Permite usar señales/slots en Qt
#include <QLabel>           // Usado para mostrar el texto "ZA WARUDO!"
#include <QMediaPlayer>     // Permite reproducir sonidos del jefe (ej: disparos, efectos)
#include <QAudioOutput>     // Controla la salida de audio de los efectos del jefe

// Declaraciones adelantadas
class Nivel3;
class Goku;
class Proyectil;

/**
 * GeneralBlue: jefe principal del Nivel 3.
 * Tiene lógica de IA, múltiples ataques, animaciones, sonidos y barra de vida.
 */
class GeneralBlue : public QObject, public Entidad {
    Q_OBJECT

public:
    GeneralBlue(Nivel3* nivel, QWidget* parent, const QVector2D& posicionInicial, Goku* objetivo);
    ~GeneralBlue();

    void actualizar() override;              // Actualiza posición, IA, sprite y barra de vida
    void recibirDanio(int cantidad);         // Lógica de daño y curación
    bool estaDerrotado() const;              // Retorna true si el jefe ha sido derrotado
    QProgressBar* getBarraVida() const;      // Devuelve la barra de vida visual (externa)
    int getVida() const;                     // Devuelve el valor actual de la vida

private:
    // --- IA y movimiento ---
    void moverHaciaJugador();                // Persigue al jugador hasta cierta distancia
    void iniciarAtaque();                    // Escoge y ejecuta un ataque al azar
    void saltar();                           // Salta si Goku está en el aire

    // --- Ataques ---
    void animarDisparo();                    // Disparo de subfusil (5 frames + proyectil)
    void lanzarRoca();                       // Lanza roca gigante (5 frames + proyectil)
    void usarTelequinesis();                 // Paraliza a Goku si no es interrumpido

    // --- Estados especiales ---
    void soltarCorazon();                    // Crea un corazón curativo cuando pierde suficiente vida
    void morir();                            // Ejecuta animación de derrota y limpieza

    // --- Sprite ---
    void cambiarSprite(const QString& ruta); // Cambia el sprite según la dirección y acción
    void actualizarSprite();                 // Anima el movimiento del jefe si se está desplazando

    // --- Referencias clave ---
    Nivel3* nivel;           // Nivel donde se encuentra
    Goku* objetivo;          // Jugador objetivo (Goku)

    // --- Estado del jefe ---
    int vida = 250;                      // Vida actual
    int vidaMaxima = 250;               // Vida máxima (referencia para curación)
    int vidaAnteriorParaCuracion = 250;// Controla cuándo se debe soltar un corazón

    bool derrotado = false;             // True si fue vencido
    bool ejecutandoTelequinesis = false;// True si está haciendo el ataque mental
    bool mirandoDerecha = true;         // Dirección actual
    bool caminando = false;             // (no usado directamente)
    bool enElAire = false;              // True si está saltando
    bool estaAtacando = false;          // (no usado actualmente)

    // --- Recursos visuales y sonoros ---
    QMediaPlayer* sfxTelequinesis = nullptr; // Reproduce sonido de ZA WARUDO
    QAudioOutput* salidaTelequinesis = nullptr;
    QLabel* labelZaWarudo = nullptr;         // Texto visual del ataque mental

    QVector2D velocidad = QVector2D(0, 0);    // Velocidad actual (solo se usa en salto)

    // --- Temporizadores ---
    QTimer* timerAtaque = nullptr;    // Controla la frecuencia de ataques
    QTimer* timerAnimacion = nullptr; // (no usado actualmente)

    QProgressBar* barraVida = nullptr; // Barra de vida visual sobre el jefe
};

#endif // GENERALBLUE_H
