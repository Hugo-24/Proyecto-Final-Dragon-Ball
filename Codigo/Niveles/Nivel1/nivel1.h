#ifndef NIVEL1_H
#define NIVEL1_H

#include "nivel.h"
#include "personaje.h"
#include "proyectil.h"
#include "soldadopatrullarroja.h"
#include "corazon.h"

#include <QVector>
#include <QTimer>
#include <QSet>
#include <QLabel>
#include <QWidget>
#include <QPushButton>
#include <QMessageBox>
#include <QMediaPlayer>
#include <QAudioOutput>

/**
 * Clase que representa el primer nivel del videojuego.
 * Incluye selección de personaje (Roshi o Lunch), scroll lateral,
 * sistema de enemigos, proyectiles, corazones de vida y condiciones de victoria/derrota.
 */
class Nivel1 : public Nivel {
    Q_OBJECT
signals:
    void volverAlMenu(); // Señal para volver al menú principal

public:
    explicit Nivel1(QWidget* parent = nullptr);

    void cargarNivel() override;             // Inicializa el nivel con fondos, personaje y lógica
    void agregarProyectil(Proyectil* p);     // Permite que Lunch agregue proyectiles
    float getOffsetX() const { return offsetX; } // Devuelve el scroll horizontal actual
    int getVidas() const { return vidas; }       // Vidas actuales del jugador
    void setVidas(int v) { vidas = v; }          // Cambia las vidas del jugador

protected:
    void keyPressEvent(QKeyEvent* event) override;   // Captura teclas presionadas
    void keyReleaseEvent(QKeyEvent* event) override; // Captura teclas soltadas

private:
    // === Personaje principal y control ===
    Personaje* jugador;         // Puntero al personaje jugable actual
    QSet<int> teclasPresionadas; // Registro de teclas actualmente presionadas

    // === Escenario y scroll lateral ===
    QWidget* fondoNivel;       // Contenedor del fondo (scrollable)
    QLabel* fondoSeleccion;    // Imagen de fondo durante selección de personaje
    QWidget* selector;         // Panel con botones de selección
    QLabel* mapa1;             // No usado directamente (puedes quitarlo si no se usa)
    QLabel* mapa2;             // No usado directamente (puedes quitarlo si no se usa)
    int offsetX;               // Valor del desplazamiento horizontal

    // === Enemigos y proyectiles ===
    QVector<SoldadoPatrullaRoja*> enemigos; // Lista de enemigos activos
    QVector<Proyectil*> proyectiles;        // Lista de proyectiles activos
    void agregarEnemigoEnPosicion(const QVector2D& pos, bool haciaDerecha); // Agrega un enemigo en una posición específica
    void iniciarSpawningDeEnemigos();  // Inicia generación progresiva de enemigos
    void agregarEnemigosIniciales();   // Agrega los enemigos del inicio

    // === Control de tiempo ===
    QTimer* timer;                 // Timer principal del juego (~60 FPS)
    QTimer* timerSpawnEnemigos = nullptr; // Timer para generar tandas de enemigos

    // === Control de progreso y vidas ===
    int totalEnemigosCreados = 0;    // Total de enemigos generados
    const int maxEnemigosNivel = 100; // Límite de enemigos por nivel
    QLabel* contadorEnemigosLabel = nullptr; // Muestra enemigos derrotados
    bool controlesHabilitados = true; // Por si se quiere desactivar control temporalmente

    // === Corazones: vidas y curación ===
    QVector<QLabel*> corazones;         // HUD de vidas visuales (máximo 5)
    QVector<Corazon*> corazonesCurativos; // Corazones que aparecen tras matar enemigos
    int vidas;                             // Vidas actuales del jugador
    void inicializarCorazones(int cantidad);        // Inicializa HUD
    void actualizarCorazones(int nuevaVida);        // Refresca los corazones del HUD

    // === Reproductores de música y efectos de sonido ===
    QMediaPlayer* reproductorNivel = nullptr;
    QAudioOutput* salidaAudioNivel = nullptr;
    QMediaPlayer* reproductorSeleccion = nullptr;
    QAudioOutput* salidaAudioSeleccion = nullptr;
    QMediaPlayer* reproductorVictoria = nullptr;
    QAudioOutput* salidaAudioVictoria = nullptr;

    // === Estado del juego ===
    bool estaMuerto = false;           // True si el jugador ha muerto
    int enemigosDerrotados = 0;        // Contador de enemigos eliminados

    void mostrarMensajeDerrota();      // Pantalla de derrota
    void mostrarExplosion(const QVector2D& pos); // Efecto visual al usar bazuca
    void mostrarMensajeVictoria();     // Mensaje cuando se gana el nivel

    // === Limpieza de recursos ===
    void reiniciarNivel() override;        // Reinicia completamente el nivel
    void limpiarTodosLosProyectiles();     // Elimina proyectiles activos
    void limpiarTodosLosEnemigos();        // Elimina enemigos activos
    void limpiarCorazones();               // Elimina corazones del HUD
    void limpiarJugador();                 // Elimina jugador actual
    void limpiarFondo();                   // Elimina fondo del nivel
    void detenerMusica() override;         // Detiene la música en curso
};

#endif // NIVEL1_H
