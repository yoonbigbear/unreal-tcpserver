#ifndef _NAVIGATION_H_
#define _NAVIGATION_H_

#include "pch.h"
#include <detour/recast.h>
#include <detour/DetourCommon.h>
#include <detour/DetourCrowd.h>
#include <detour/DetourNavMesh.h>
#include <detour/DetourNavMeshQuery.h>

class Navigation
{
public:
    Navigation();
    ~Navigation();

    dtNavMesh* navigation() { return navigation_; }
    void navigation(dtNavMesh* nav) { navigation_ = nav; }

    dtNavMeshQuery* query() { return query_; }
    void query(dtNavMeshQuery* query) { query_ = query; }

    dtNavMesh* LoadAll(const char* path);
    void SaveAll(const char* path, const dtNavMesh* mesh);

    void RandomPoint(float* dest);

    bool CanMoveTo(float* pt);

private:
    dtNavMesh* navigation_;
    dtNavMeshQuery* query_;
    dtCrowd* crowd_;

    dtQueryFilter filter_;

    int m_nrandPoints;
    bool m_randPointsInCircle;
    static const int MAX_RAND_POINTS = 64;
    float m_randPoints[MAX_RAND_POINTS * 3];
};

#endif // _NAVIGATION_H_