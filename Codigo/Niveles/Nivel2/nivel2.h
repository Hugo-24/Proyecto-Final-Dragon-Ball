#pragma once
#include "nivel.h"
#include "mapa.h"
#include "submarinojugador.h"

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
};


