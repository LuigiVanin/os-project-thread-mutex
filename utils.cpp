#include "utils.h"

Vector2D TREM_SIZE = {
    .x=  21,
    .y = 21
};

Axis defineTrackAxis(QLabel *track) {
    if (track->width() < track->height()) {
        return Axis::VERTICAL;
    }

    return Axis::HORIZONTAL;
}
