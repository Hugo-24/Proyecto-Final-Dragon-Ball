#ifndef SUBMARINOJUGADOR_H
#define SUBMARINOJUGADOR_H

#include "entidad.h"
#include <QKeyEvent>
#include <QTimer>

class SubmarinoJugador : public Entidad {
public:
    SubmarinoJugador(QWidget* parent = nullptr);

    void procesarEntrada(const QSet<int>& teclasPresionadas);
    void aplicarFisica();
    void actualizar();

};

#endif // SUBMARINOJUGADOR_H
