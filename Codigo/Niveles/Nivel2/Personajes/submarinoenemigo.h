#pragma once
#include "entidad.h"
#include <QVector2D>
#include <QTimer>
#include <QObject>
#include <QProgressBar>

class SubmarinoEnemigo : public QObject, public Entidad {
    Q_OBJECT

public:
    SubmarinoEnemigo(QWidget* parent, const QVector2D& posicion);
    void actualizar() override;
    void interactuar(Entidad* otra) override;
    void recibirDaño(int cantidad);
    bool estaDestruido() const;
    int getVida() const;

    void setObjetivo(Entidad* obj); // ← Añade esta línea
    void verificarFoco(const Entidad* jugador);


signals:
    void torpedoDisparado(const QVector2D& pos, const QVector2D& dir);

private slots:
    void dispararTorpedo();
    void salirDeModoAtaque();

private:
    QVector2D direccion;
    float velocidad;
    int vida;

    // Patrullaje
    float tiempoTotal;
    float radio;
    QVector2D centroMovimiento;

    // Modo ataque
    bool enModoAtaque;
    float tiempoEnAtaque;
    QTimer* temporizadorAtaque;
    QTimer* temporizadorDisparo;

    QProgressBar* barraVida;

    Entidad* objetivo;
};
