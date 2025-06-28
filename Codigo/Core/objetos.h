#pragma once
#include <QWidget>
#include <QLabel>
#include <QVector2D>

class Entidad;  // Declaración adelantada

class Objeto {
public:
    Objeto(QWidget* parent = nullptr);
    virtual ~Objeto();

    virtual void interactuar(Entidad* otra) = 0; // Acción al colisionar con una entidad
    virtual void actualizar(); // Por defecto no hace nada (se puede sobrescribir)

    QLabel* getSprite();
    QVector2D getPosicion() const;
    void setPosicion(const QVector2D& pos);

protected:
    QLabel* sprite;
    QVector2D posicion;
};
