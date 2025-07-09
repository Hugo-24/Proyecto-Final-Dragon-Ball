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
 * Nivel1 representa el primer nivel del juego, con scroll lateral,
 * selección de personajes, enemigos, proyectiles, corazones curativos y sistema de victoria.
 */
class Nivel1 : public Nivel {
    Q_OBJECT
signals:
    void volverAlMenu(); // Señal para regresar al menú principal

public:
    explicit Nivel1(QWidget* parent = nullptr);
    void cargarNivel() override;
    void agregarProyectil(Proyectil* p);
    float getOffsetX() const { return offsetX; }
    int getVidas() const { return vidas; }
    void setVidas(int v) { vidas = v; }

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

private:
    // Jugador y entradas
    Personaje* jugador;
    QSet<int> teclasPresionadas;

    // Escenario y scroll
    QWidget* fondoNivel;
    QLabel* fondoSeleccion;
    QWidget* selector;
    QLabel* mapa1;
    QLabel* mapa2;
    int offsetX;

    // Enemigos y proyectiles
    QVector<SoldadoPatrullaRoja*> enemigos;
    QVector<Proyectil*> proyectiles;
    void agregarEnemigoEnPosicion(const QVector2D& pos, bool haciaDerecha); // <-- CORREGIDA
    void iniciarSpawningDeEnemigos(); // <-- AGREGADA
    void agregarEnemigosIniciales();

    // Timer principal
    QTimer* timer;

    // Timer para spawn progresivo de enemigos
    QTimer* timerSpawnEnemigos = nullptr;
    int totalEnemigosCreados = 0;
    const int maxEnemigosNivel = 100;
    QLabel* contadorEnemigosLabel = nullptr; // Contador visual de enemigos derrotados
    bool controlesHabilitados = true;        // Permite pausar movimiento inicial

    // Corazones
    QVector<QLabel*> corazones;         // Corazones de vidas (máx 5)
    QVector<Corazon*> corazonesCurativos; // Corazones curativos que se recogen
    int vidas;
    void inicializarCorazones(int cantidad);
    void actualizarCorazones(int nuevaVida);

    // Música
    QMediaPlayer* reproductorNivel = nullptr;
    QAudioOutput* salidaAudioNivel = nullptr;
    QMediaPlayer* reproductorSeleccion = nullptr;
    QAudioOutput* salidaAudioSeleccion = nullptr;
    QMediaPlayer* reproductorVictoria = nullptr;
    QAudioOutput* salidaAudioVictoria = nullptr;

    // Lógica de juego
    bool estaMuerto = false;
    int enemigosDerrotados = 0;
    void mostrarMensajeDerrota();
    void mostrarExplosion(const QVector2D& pos);
    void mostrarMensajeVictoria();

    // Limpieza
    void reiniciarNivel() override;
    void limpiarTodosLosProyectiles();
    void limpiarTodosLosEnemigos();
    void limpiarCorazones();
    void limpiarJugador();
    void limpiarFondo();
    void detenerMusica() override;
};

#endif // NIVEL1_H
