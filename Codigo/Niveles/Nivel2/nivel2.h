#pragma once
#include "nivel.h"
#include "mapa.h"
#include "submarinojugador.h"

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


    // Interacción objetos de colision:
    std::vector<Objeto*> objetosHostiles;
    QVector<Torpedo*> torpedos;



    void agregarMina(const QVector2D& pos);
    void agregarTorpedo(const QVector2D& pos, const QVector2D& direccion);
    void verificarColisiones();  // método auxiliar para fase 2
};


