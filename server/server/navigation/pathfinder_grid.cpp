#include "pathfinder_grid.h"

PathFinderGrid::~PathFinderGrid()
{
    if (grids_)
    {
        delete grids_;
        grids_ = nullptr;
    }
}

PathFinderGrid::PathFinderGrid(int x_size, int y_size)
{
    grids_ = new int[x_size * y_size];
    x_size_ = x_size;
    y_size_ = y_size;
}
