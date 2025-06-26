#pragma once

#include <QWidget>
#include "Mapa.h"
#include <vector>

//#include "Jugador.h"
//#include "Enemigo.h"
//#include "Objeto.h"

class Nivel : public QWidget
{
    Q_OBJECT

public:
    explicit Nivel(QWidget *parent = nullptr);
    virtual ~Nivel();

    virtual void cargarNivel() = 0; // Cada nivel lo implementa
    virtual void reiniciarNivel();  // Opcional, por si quieres reiniciar estado

protected:
    Mapa *mapa;
    //Jugador* jugador;
    //std::vector<Enemigo*> enemigos;
    //std::vector<Objeto*> objetos;
};
