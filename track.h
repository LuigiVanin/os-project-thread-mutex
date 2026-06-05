#ifndef TRACK_H
#define TRACK_H
#include "QLabel"

class Track
{
public:
    Track(int id, QLabel* ui);

    int getID();
    QLabel* getUi();

private:
    int ID;
    QLabel* ui;
};

#endif // TRACK_H
