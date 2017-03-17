#ifndef VTKWRITER_H
#define VTKWRITER_H
#include <QVector>
#include <functional>
#include "spatialbingrid.h"

class VTKWriter
{
public:
    VTKWriter();
    void write(SpatialBinGrid &grid, QStringList headers, QString fileName, QVector3D voxelSize, std::function<float(Voxel &voxel)> customFunctor);
};

#endif // VTKWRITER_H
