#include "constants.h"

std::ostream &operator<<(std::ostream &os, Position const &m) {
    std::string pos = "?";
    if      (m.col == 0) pos = "a";
    else if (m.col == 1) pos = "b";
    else if (m.col == 2) pos = "c";
    else if (m.col == 3) pos = "d";
    else if (m.col == 4) pos = "e";
    else if (m.col == 5) pos = "f";
    else if (m.col == 6) pos = "g";
    else if (m.col == 7) pos = "h";
    return os << pos << 8 - m.row;
}

bool operator==(const Position& lhs, const Position& rhs) {
    return lhs.col == rhs.col && lhs.row == rhs.row;
}