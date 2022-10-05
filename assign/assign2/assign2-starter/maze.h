#pragma once

#include "stack.h"
#include "grid.h"
#include "set.h"
#include <string>

// Prototypes to be shared with other modules

bool isOutOfMazeBound(const Grid<bool>& maze,const GridLocation& location);

Set<GridLocation> generateValidMoves(Grid<bool>& g, GridLocation cur);

void validatePath(Grid<bool>& g, Stack<GridLocation> path);

void readMazeFile(std::string filename, Grid<bool>& maze);

void readSolutionFile(std::string filename, Stack<GridLocation>& soln);

Set<GridLocation> visitedLocationSet(Stack<GridLocation> curPath);

Stack<GridLocation> solveMaze(Grid<bool>& maze);
