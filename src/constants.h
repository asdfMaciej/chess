#pragma once
#ifndef HEADER_CONSTANTS
#define HEADER_CONSTANTS

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <cmath>
#include <SFML/Graphics.hpp>

const int BOARD_SIZE = 8;

enum class Color {
    White, Black
};

enum class Result {
    Playing, Check, Checkmate, Stalemate
};

struct Position {
    int row;
    int col;
};

std::ostream &operator<<(std::ostream &os, Position const &m);
bool operator==(const Position& lhs, const Position& rhs);

#endif