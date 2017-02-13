#include <QString>
#include <QDebug>
#include <QVector3D>
#include "lammpstextdumpreader.h"
#include "spatialbingrid.h"
#include "vtkwriter.h"

int main(int argc, char *argv[])
{
    if(argc != 6 && argc!=9) {
        qDebug() << "./dump2vtk file out.vtk numVoxelsX numVoxelsY numVoxelsZ [voxelSizeX=1 voxelSizeY=1 voxelSizeZ=1]";
        return 0;
    }

    QString inFileName = QString::fromUtf8(argv[1]);
    QString outFileName = QString::fromUtf8(argv[2]);
    int nx = atoi(argv[3]);
    int ny = atoi(argv[4]);
    int nz = atoi(argv[5]);
    QVector3D voxelSize(1.0,1.0,1.0);
    if(argc==9) {
        voxelSize[0] = atof(argv[6]);
        voxelSize[1] = atof(argv[7]);
        voxelSize[2] = atof(argv[8]);
    }
    qDebug() << "Will load LAMMPS chunk dump file: " << inFileName;

    LAMMPSTextDumpReader reader(inFileName, nx, ny, nz);
    SpatialBinGrid grid = reader.getNextTimeStep();
    VTKWriter writer;
    qDebug() << "Will write VTK file with " << grid.numValues() << " values per chunk and " << grid.voxels().size() << " chunks.";
    writer.write(grid, outFileName, voxelSize);

    return 0;
}
