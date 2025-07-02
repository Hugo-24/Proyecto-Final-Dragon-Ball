#pragma once
#include "nivel.h"
#include "mapa.h"
#include "submarinojugador.h"
#include "submarinoenemigo.h"
#include "QProgressBar"

class Objeto;
class Torpedo;

class Nivel2 : public Nivel {
    Q_OBJECT
public:
    explicit Nivel2(QWidget* parent = nullptr);
    void cargarNivel() override;

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

private:


    SubmarinoJugador* submarino;
    QTimer* timerActualizacion;
    QSet<int> teclasPresionadas;
    QProgressBar* barraVida;


    // Interacción objetos de colision:
    std::vector<Objeto*> objetosHostiles;
    QVector<Torpedo*> torpedos;
    QVector<Torpedo*> torpedosJugador;
    QVector<SubmarinoEnemigo*> enemigos;



    void agregarMina(const QVector2D& pos);
    void agregarTorpedo(const QVector2D& pos, const QVector2D& direccion, bool delJugador);
    void verificarColisiones();  // método auxiliar para fase 2
    void agregarSubmarinoEnemigo(const QVector2D& pos);
    void mostrarExplosion(const QVector2D& posicion);


};


