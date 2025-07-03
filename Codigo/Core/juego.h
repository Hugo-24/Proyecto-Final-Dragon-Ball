#pragma once
#include <QWidget>
#include <map>
#include "nivel.h"
#include "nivel1.h"
#include "nivel2.h"

class Juego : public QWidget {
    Q_OBJECT
signals:
    void regresarAlMenu();
public:
    Juego(QWidget *parent = nullptr);
    void cambiarNivel(const std::string& id);
    Nivel* getNivelActual() const { return nivelActual; }


private:
    std::map<std::string, Nivel*> niveles;
    Nivel* nivelActual;
};
