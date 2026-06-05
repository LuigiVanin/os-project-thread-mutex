#include "track.h"

Track::Track(int id, QLabel* ui) {
    this->ID = id;
    this->ui = ui;
}

int Track::getID() {
    return this->ID;
}

QLabel* Track::getUi() {
    return this->ui;
}
