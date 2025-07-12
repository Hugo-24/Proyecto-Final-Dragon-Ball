#ifndef NIVEL3_H
#define NIVEL3_H

#include "nivel.h"              // Clase base común a todos los niveles
#include "goku.h"               // Jugador exclusivo del Nivel 3
#include "generalblue.h"       // Jefe final del nivel
#include "proyectil.h"         // Para gestionar disparos de ambos personajes
#include "corazon.h"           // Corazones curativos

#include <QVector>             // Contenedores para proyectiles, corazones, etc.
#include <QTimer>              // Bucle del juego y eventos diferidos
#include <QSet>                // Seguimiento de teclas presionadas
#include <QLabel>              // Elementos visuales (corazones, fondo)
#include <QWidget>             // Contenedor principal del nivel
#include <QPushButton>         // Botones opcionales (no usados por ahora)
#include <QMediaPlayer>        // Reproducción de música/sonidos
#include <QAudioOutput>        // Control de salida de audio

/**
 * Nivel3 representa el enfrentamiento final contra General Blue.
 * Combate en una arena estática dentro de una cueva (sin scroll).
 */
class Nivel3 : public Nivel {
    Q_OBJECT

signals:
    void volverAlMenu(); // Señal para regresar al menú principal tras derrota o victoria

public:
    explicit Nivel3(QWidget* parent = nullptr); // Constructor
    void cargarNivel() override;                // Carga completa del escenario, jugador y jefe

    void agregarProyectil(Proyectil* p);        // Añade un nuevo proyectil a la escena
    void agregarCorazon(Corazon* c);            // Añade un corazón curativo al nivel

    float getOffsetX() const { return offsetX; } // Offset visual (aunque este nivel es estático)

    bool controlesHabilitados = false;          // Controla si el jugador puede moverse/disparar

protected:
    void keyPressEvent(QKeyEvent* event) override;    // Entrada de teclado
    void keyReleaseEvent(QKeyEvent* event) override;  // Liberación de teclas

    void detenerMusica() override;      // Detiene y limpia la música
    void reiniciarNivel() override;     // Reinicia por completo el estado del nivel

private:
    // ==== ENTIDADES PRINCIPALES ====
    Goku* jugador;               // Jugador controlado
    GeneralBlue* jefe;           // Jefe enemigo

    // ==== INTERFAZ Y ESTADO ====
    QVector<QLabel*> corazones;      // Corazones de vida del HUD
    QProgressBar* barraVidaJefe;    // Barra de vida visual del jefe

    QTimer* timer;                   // Bucle principal del nivel (~60 FPS)
    QSet<int> teclasPresionadas;    // Conjunto de teclas activas
    bool estaMuerto = false;        // Bandera para evitar múltiples muertes
    int vidas = 5;                  // Vidas del jugador
    int corazonesGenerados = 0;    // Contador de corazones soltados por el jefe

    void actualizarMovimientoJugador();  // Maneja movimiento horizontal y saltos

    // ==== ENTIDADES DINÁMICAS ====
    QVector<Proyectil*> proyectiles;     // Proyectiles en escena
    QVector<Corazon*> corazonesCurativos; // Corazones que caen en el campo

    // ==== MÚSICA Y SONIDOS ====
    QMediaPlayer* musicaFondo;      // Música de fondo del combate
    QAudioOutput* salidaAudio;      // Salida de audio asociada

    QMediaPlayer* sonidoVictoria;   // Sonido al ganar
    QAudioOutput* salidaVictoria;   // Salida de audio para victoria

    // ==== ESCENARIO ====
    QLabel* fondo;                  // Imagen de fondo de la cueva

    int offsetX;                    // No se usa como scroll, pero puede usarse para desplazamientos

    // ==== FUNCIONES AUXILIARES ====
    void inicializarCorazones(int cantidad);     // Crea los corazones del HUD
    void actualizarCorazones(int nuevaVida);     // Muestra corazones llenos/vacíos según vidas

    void mostrarMensajeDerrota();                // Pantalla de derrota
    void mostrarMensajeVictoria();               // Pantalla de victoria

    void limpiarProyectiles();                   // Limpieza segura de todos los proyectiles
    void limpiarCorazones();                     // Limpieza segura de corazones curativos
    void limpiarJugador();                       // Elimina a Goku
    void limpiarJefe();                          // Elimina al jefe
    void limpiarFondo();                         // Elimina el fondo del nivel
};

#endif // NIVEL3_H
