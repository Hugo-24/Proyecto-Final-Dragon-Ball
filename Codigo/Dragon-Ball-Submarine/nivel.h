#pragma once
#include <QWidget>

class Nivel : public QWidget {
    Q_OBJECT

public:
    explicit Nivel(QWidget* parent = nullptr) : QWidget(parent) {}
    virtual void cargarNivel() = 0;
};
