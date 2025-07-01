#pragma once
#include "entidad.h"
#include <QVector2D>

class SubmarinoEnemigo : public Entidad {
public:
    SubmarinoEnemigo(QWidget* parent, const QVector2D& posicion);

    void actualizar() override;
    void interactuar(Entidad* otra) override;
    void recibirDaño(int cantidad);
    bool estaDestruido() const;

private:
    QVector2D direccion;
    float velocidad;
    int vida;

    // Patrullaje vertical con avance
    float tiempoTotal;
    float amplitudVertical;
    float frecuencia;
    int pasosDesdeUltimoAvance;
    int pasosPorAvance;
};
