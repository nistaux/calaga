#pragma once

#include <stdbool.h>

#include "enemies.h"

int get_field_available();
int find_field_space(MoveType moveType);
void take_field_space(int startLoc, MoveType moveType);
void free_field_space(int startLoc, MoveType moveType);