#include "track.h"


Track::Track(int id, QLabel* ui, bool is_critical) {
    this->ID = id;
    this->ui = ui;

    // NOTE: Outros trens devem consultar esse mutex de região crítica para saber se está ocupado
    //       ou não, caso esteja ocupado por um trem devem esperar fora do trilho até que o mesmo esteja
    //       vago.
    this->critical = is_critical;


}

int Track::getID() {
    return this->ID;
}

QLabel* Track::getUi() {
    return this->ui;
}

bool Track::isCritical() {
    return this->critical;
}