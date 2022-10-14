#pragma once
#ifndef HEADER_CONSTANTS
#define HEADER_CONSTANTS

#include <iostream>
#include <memory>
#include <string>

const int BOARD_SIZE = 8;

enum class Color {
    White, Black
};

enum class Result {
    Playing, Check, Checkmate, Stalemate
};

#endif