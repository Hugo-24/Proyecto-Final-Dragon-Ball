#pragma once
#include "nivel.h"
#include "mapa.h"
#include "submarinojugador.h"
#include "submarinoenemigo.h"
#include "QProgressBar"
#include <QPushButton>
#include <QMediaPlayer>
#include <QAudioOutput>



class Objeto;
class Torpedo;

class Nivel2 : public Nivel {
    Q_OBJECT
public:
    explicit Nivel2(QWidget* parent = nullptr);
    void cargarNivel() override;
    void mostrarMensajeDerrota();
    void reiniciarNivel() override;

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

signals:
    void volverAlMenu();

private:
    // Fase 1

    SubmarinoJugador* submarino;
    QTimer* timerActualizacion;
    QSet<int> teclasPresionadas;
    QProgressBar* barraVida;

    QVector<QVector<QVector2D>> oleadas;
    int indiceOleadaActual = 0;
    bool esperandoNuevaOleada = false;


    //Fase 2
    Entidad* gokui = nullptr;
    Entidad* bubuzo = nullptr;

    QProgressBar* barraVidaGokui = nullptr;
    QProgressBar* barraVidaBubuzo = nullptr;

    bool fase2Activa = false;

    // Audio de fondo
    QMediaPlayer* musicaFondo = nullptr;
    QAudioOutput* salidaAudio = nullptr;




    // Interacción objetos de colision:
    std::vector<Objeto*> objetosHostiles;
    QVector<Torpedo*> torpedos;
    QVector<Torpedo*> torpedosJugador;
    QVector<SubmarinoEnemigo*> enemigos;

    QPushButton* btnSalir;

    qint64 ultimoDisparo = 0;
    int intervaloDisparo = 150;  // milisegundos

    bool controlesHabilitados = true;



    void agregarMina(const QVector2D& pos);
    void agregarTorpedo(const QVector2D& pos, const QVector2D& direccion, bool delJugador);
    void verificarColisiones();  // método auxiliar para fase 2
    void agregarSubmarinoEnemigo(const QVector2D& pos);
    void mostrarExplosion(const QVector2D& posicion);

    void cargarOleadaActual();

    void limpiarEnemigos();

    void iniciarFase2();

    void mostrarMensajeVictoria();

    void detenerMusica() override;

};


