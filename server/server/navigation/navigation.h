#ifndef _NAVIGATION_H_
#define _NAVIGATION_H_

#include "pch.h"
#include <detour/recast.h>

class Navigation
{
public:
    class dtNavMesh* LoadAll(const char* path);
    void SaveAll(const char* path, const dtNavMesh* mesh);
private:
    dtNavMesh* nav_mesh_;
    class dtNavMeshQuery* m_navQuery;
    class dtCrowd* m_crowd;
};

#endif // _NAVIGATION_H_