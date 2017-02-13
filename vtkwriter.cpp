#include "vtkwriter.h"

#include <QFile>
#include <QTextStream>

VTKWriter::VTKWriter()
{

}

void VTKWriter::write(QVector<SpatialBinGrid> &grids, QString fileName, QVector3D voxelSize)
{

}

void VTKWriter::write(SpatialBinGrid &grid, QString fileName, QVector3D voxelSize)
{
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        char error[1024];
        sprintf(error, "Error: could not open file %s", fileName.toUtf8().constData());
        qFatal(error);
    }

    QTextStream out(&file);
    out << "# vtk DataFile Version 2.0" << endl;
    out << "structured point" << endl;
    out << "ASCII" << endl;
    out << "" << endl;
    out << "DATASET STRUCTURED_POINTS" << endl;
    out << "DIMENSIONS " << grid.nx() << " " << grid.ny() << " " << grid.nz() << endl;
    out << "ORIGIN 0.0 0.0 0.0" << endl;
    out << "SPACING " << voxelSize.x() << " " << voxelSize.y() << " " << voxelSize.z() << endl;
    out << "POINT_DATA " << grid.voxels().size() << endl;
    out << "SCALARS values float " << grid.numValues() << endl;
    out << "LOOKUP_TABLE default" << endl;

    for(int k=0; k<grid.nz(); k++) {
        for(int j=0; j<grid.ny(); j++) {
            for(int i=0; i<grid.nx(); i++) {
                const Voxel &voxel = grid(i,j,k);
                for(int n=0; n<grid.numValues(); n++) {
                    out << voxel.values[n] << " ";
                }
                out << endl;
            }
        }
    }
    file.close();
}
