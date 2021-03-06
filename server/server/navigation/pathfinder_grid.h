#ifndef _PATHFINDER_GRID_
#define _PATHFINDER_GRID_

#include "pch.h"


class PathFinderGrid
{
public:
    PathFinderGrid() = delete;
    PathFinderGrid(int x_size, int y_size);

    ~PathFinderGrid();

    bool MoveUp(int index);
    bool MoveDown(int index);
    bool MoveLeft(int index);
    bool MoveRight(int index);

private:
    int x_size_ = 0;
    int y_size_ = 0;
};

#endif // !_PATHFINDER_GRID_
