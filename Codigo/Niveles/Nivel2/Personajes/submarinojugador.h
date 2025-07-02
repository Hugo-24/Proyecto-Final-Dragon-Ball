#ifndef SUBMARINOJUGADOR_H
#define SUBMARINOJUGADOR_H

#include "entidad.h"
#include <QKeyEvent>
#include <QTimer>

class SubmarinoJugador : public Entidad {

private:

    int vidaMaxima = 100;
    int vidaActual = 100;
public:
    SubmarinoJugador(QWidget* parent = nullptr);

    void procesarEntrada(const QSet<int>& teclasPresionadas);
    void aplicarFisica();
    void actualizar();

    int getVida() const { return vidaActual; }
    int getVidaMaxima() const { return vidaMaxima; }

    void recibirDanio(int cantidad);




};

#endif // SUBMARINOJUGADOR_H
