#ifndef UTILS_H
#define UTILS_H
#include "QLabel"

typedef struct {
    int x, y;
} Vector2D;

typedef struct {
    Vector2D start;
    Vector2D end;
} Path;

extern Vector2D TREM_SIZE;

enum class Axis {
    HORIZONTAL,
    VERTICAL,
};

Axis defineTrackAxis(QLabel *track);

#endif // UTILS_H
