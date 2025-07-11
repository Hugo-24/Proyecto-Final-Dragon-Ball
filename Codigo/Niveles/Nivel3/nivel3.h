#ifndef NIVEL3_H
#define NIVEL3_H

#include "nivel.h"
#include "goku.h"
#include "generalblue.h"
#include "proyectil.h"
#include "corazon.h"

#include <QVector>
#include <QTimer>
#include <QSet>
#include <QLabel>
#include <QWidget>
#include <QPushButton>
#include <QMediaPlayer>
#include <QAudioOutput>

/**
 * Nivel3 representa el enfrentamiento final contra General Blue.
 * Combate en una arena estática dentro de una cueva.
 */
class Nivel3 : public Nivel {
    Q_OBJECT

signals:
    void volverAlMenu(); // Señal para regresar al menú principal

public:
    explicit Nivel3(QWidget* parent = nullptr);
    void cargarNivel() override;
    void agregarProyectil(Proyectil* p);
    float getOffsetX() const { return offsetX; }
    void agregarCorazon(Corazon* c);
    bool controlesHabilitados = false;


protected:
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

    void detenerMusica() override;
    void reiniciarNivel() override;

private:
    // Jugador y jefe
    Goku* jugador;
    GeneralBlue* jefe;

    // Barras de vida
    QVector<QLabel*> corazones;
    QProgressBar* barraVidaJefe;

    // Lógica de juego
    QTimer* timer;
    QSet<int> teclasPresionadas;
    bool estaMuerto = false;
    int vidas = 5;
    int corazonesGenerados = 0;
    void actualizarMovimientoJugador();  // ← Para controlar el movimiento WASD

    // Proyectiles y corazones curativos
    QVector<Proyectil*> proyectiles;
    QVector<Corazon*> corazonesCurativos;

    // Música
    QMediaPlayer* musicaFondo;
    QAudioOutput* salidaAudio;

    QMediaPlayer* sonidoVictoria;
    QAudioOutput* salidaVictoria;

    // Fondo del nivel
    QLabel* fondo;

    // Scroll (aunque el nivel es estático, esto puede usarse para offset visual)
    int offsetX;

    // Métodos auxiliares
    void inicializarCorazones(int cantidad);
    void actualizarCorazones(int nuevaVida);

    void mostrarMensajeDerrota();
    void mostrarMensajeVictoria();

    void limpiarProyectiles();
    void limpiarCorazones();
    void limpiarJugador();
    void limpiarJefe();
    void limpiarFondo();
};

#endif // NIVEL3_H
