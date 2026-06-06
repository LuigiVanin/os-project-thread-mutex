#ifndef TRACK_H
#define TRACK_H
#include "QLabel"
#include "pthread.h"

class Track
{
public:
    Track(int id, QLabel* ui, bool is_critical = false);
    ~Track();
    int getID();
    QLabel* getUi();
    bool isCritical();

    bool tryOccupy(int trem_id);
    void release(int trem_id);

    int getCurrentOccupant();

private:
    int ID;
    bool critical;
    QLabel* ui;

    pthread_mutex_t occupant_mutex;
    int occupant_id = -1; // inicialmente sempre vazio
};

#endif // TRACK_H
