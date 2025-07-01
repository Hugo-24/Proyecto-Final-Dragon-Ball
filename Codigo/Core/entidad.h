#ifndef ENTIDAD_H
#define ENTIDAD_H

#include <QVector2D>
#include <QLabel>
#include <QWidget>

class Entidad {
public:
    Entidad(QWidget* parent = nullptr);
    virtual ~Entidad();

    virtual void mover();       // lógica de movimiento
    virtual void actualizar();  // actualiza la posición del sprite


    void setPosicion(const QVector2D& pos);
    QVector2D getPosicion() const;

    void setVelocidad(const QVector2D& vel);
    QVector2D getVelocidad() const;

    QLabel* getSprite();

    virtual void interactuar(Entidad* otra);




protected:
    QVector2D posicion;
    QVector2D velocidad;
    QLabel* sprite; // imagen visual de la entidad
};

#endif // ENTIDAD_H
