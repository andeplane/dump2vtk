#ifndef VTKWRITER_H
#define VTKWRITER_H
#include <QVector>
#include "spatialbingrid.h"

class VTKWriter
{
public:
    VTKWriter();
    void write(QVector<SpatialBinGrid> &grids, QString fileName, QVector3D voxelSize);
    void write(SpatialBinGrid &grid, QString fileName, QVector3D voxelSize);
};

#endif // VTKWRITER_H
