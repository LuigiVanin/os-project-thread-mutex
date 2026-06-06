#include "track.h"
#include "pthread.h"


Track::Track(int id, QLabel* ui, bool is_critical) {
    this->ID = id;
    this->ui = ui;

    // NOTE: Outros trens devem consultar esse mutex de região crítica para saber se está ocupado
    //       ou não, caso esteja ocupado por um trem devem esperar fora do trilho até que o mesmo esteja
    //       vago.
    this->critical = is_critical;

    if (this->critical) {
        pthread_mutex_init(&this->occupant_mutex, NULL);
        // pthread_mutex_init(this->occupant_mutex, NULL);
    }

}

Track::~Track() {
    if (this->isCritical()) {
        pthread_mutex_destroy(&this->occupant_mutex);
    }
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

int Track::getCurrentOccupant() {
    return this->occupant_id;
}

bool Track::tryOccupy(int trem_id) {
    // caso não seja trecho crítico pode sempre liberar
    if (!this->isCritical()) {
        this->occupant_id = trem_id;
        return true;
    }

    auto occupied = pthread_mutex_trylock(&this->occupant_mutex) == 0;
    
    if (occupied) this->occupant_id = trem_id;

    return occupied;
}

void Track::release(int trem_id) {
    if (occupant_id != trem_id) return;

    if (!this->isCritical()) {
        this->occupant_id = -1;
        return;
    }

    pthread_mutex_unlock(&this->occupant_mutex);
    this->occupant_id = -1;
}