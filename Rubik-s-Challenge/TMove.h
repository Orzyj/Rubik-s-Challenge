#ifndef TMOVE_H
#define TMOVE_H

#include "directionsTypes.h"
#include <string>

struct TMove{
    int id;
    Direction direction;
    char axis;
    float angle;
    int selectedOption;
};

inline std::string getMoveText (Direction direction) {
    switch(direction) {
    case Direction::Up:    return "Góra";
    case Direction::Down:  return "Dół";
    case Direction::Left:  return "Lewo"; ;
    case Direction::Right: return "Prawo";
    case Direction::None:  return "Brak";
    }
}

#endif // TMOVE_H
