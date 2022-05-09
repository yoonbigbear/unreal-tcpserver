#include "pathfinder_grid.h"

PathFinderGrid::~PathFinderGrid()
{

}

bool PathFinderGrid::MoveUp(int index)
{
    return false;
}

bool PathFinderGrid::MoveDown(int index)
{
    return false;
}

bool PathFinderGrid::MoveLeft(int index)
{
    return false;
}

bool PathFinderGrid::MoveRight(int index)
{
    return false;
}

PathFinderGrid::PathFinderGrid(int x_size, int y_size)
{

    x_size_ = x_size;
    y_size_ = y_size;
}
