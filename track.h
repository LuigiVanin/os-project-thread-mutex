#ifndef TRACK_H
#define TRACK_H
#include "QLabel"
#include "pthread.h"

class Track
{
public:
    Track(int id, QLabel* ui, bool is_critical = false);

    int getID();
    QLabel* getUi();
    bool isCritical();

private:
    int ID;
    bool critical;
    QLabel* ui;
    pthread_mutex_t occupant_mutex;
};

#endif // TRACK_H
