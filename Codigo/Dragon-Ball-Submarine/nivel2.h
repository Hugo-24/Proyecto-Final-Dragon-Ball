#pragma once
#include "Nivel.h"

class Nivel2 : public Nivel {
    Q_OBJECT
public:
    explicit Nivel2(QWidget* parent = nullptr);
    void cargarNivel() override;
};
