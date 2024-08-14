#pragma once

#include <QString>

#include "Point.h"

#include <QMatrix4x4>

template <typename T>
void fromVoxelIJKToWorldXYZ(const Point<int>& voxel, const Point<T>& origin, const Point<T>& spacing, const double* direction, Point<T>& world)
{
    QVector4D voxelIJK(voxel.x, voxel.y, voxel.z, 1);
    QVector4D transfromed = QMatrix4x4(
                                direction[0], direction[3], direction[6], 0,
                                direction[1], direction[4], direction[7], 0,
                                direction[2], direction[5], direction[8], 0,
                                0, 0, 0, 1)
        * voxelIJK;

    world.x = transfromed.x() * spacing.x + origin.x;
    world.y = transfromed.y() * spacing.y + origin.y;
    world.z = transfromed.z() * spacing.z + origin.z;
}

template <typename T>
void fromWorldXYZToVoxelIJK(const Point<T>& world, const Point<T>& origin, const Point<T>& spacing, const double* direction, Point<int>& voxel)
{
    QVector4D worldXYZ(world.x, world.y, world.z, 1);
    auto transfromed = QMatrix4x4(
        direction[0], direction[3], direction[6], 0,
        direction[1], direction[4], direction[7], 0,
        direction[2], direction[5], direction[8], 0,
        0, 0, 0, 1);
    auto transfromVec = transfromed.inverted() * (worldXYZ - QVector4D(origin.x, origin.y, origin.z, 1));

    voxel.x = transfromVec.x() / spacing.x;
    voxel.y = transfromVec.y() / spacing.y;
    voxel.z = transfromVec.z() / spacing.z;
}
