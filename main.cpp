#include <QString>
#include <QDebug>
#include <QVector3D>
#include <functional>
#include "lammpstextdumpreader.h"
#include "spatialbingrid.h"
#include "vtkwriter.h"

int main(int argc, char *argv[])
{
    std::function<float(Voxel &voxel)> customFunctor;
    // If you want to combine values, uncomment the next lines and write a custom functor on each voxel.
    // One example is if three values are the diagonal components of the stress, you could sum them and divide by three to get pressure.
//    customFunctor = [](Voxel &voxel) {
//        return 1.0/3*(voxel.values[0]+voxel.values[1]+voxel.values[2]);
//    };

    if(argc != 7 && argc!=10) {
        qDebug() << "./dump2vtk inFileName outFileName numVoxelsX numVoxelsY numVoxelsZ timestep [voxelSizeX=1 voxelSizeY=1 voxelSizeZ=1]";
        qDebug() << "timestep can be any number starting from 0, or 'all'";
        qDebug() << "outFileName should not have .vtk extension";

        return 0;
    }

    QString inFileName = QString::fromUtf8(argv[1]);
    QString outFileName = QString::fromUtf8(argv[2]);
    int nx = atoi(argv[3]);
    int ny = atoi(argv[4]);
    int nz = atoi(argv[5]);
    QString timestepStr = QString::fromUtf8(argv[6]);
    bool allTimesteps = false;
    int timestep = 0;

    if(timestepStr==QString("all")) {
        allTimesteps = true;
    } else {
        bool ok;
        timestep = timestepStr.toInt(&ok);
        if(!ok) {
            qDebug() << "Error could not parse timestep argument.";
            return 0;
        }
    }

    QVector3D voxelSize(1.0,1.0,1.0);
    if(argc==10) {
        voxelSize[0] = atof(argv[7]);
        voxelSize[1] = atof(argv[8]);
        voxelSize[2] = atof(argv[9]);
    }
    qDebug() << "Will load LAMMPS chunk dump file: " << inFileName;

    LAMMPSTextDumpReader reader(inFileName, nx, ny, nz);

    if(allTimesteps) {
        qDebug() << "Will write all timesteps to VTK files.";
        int currentTimestep = 0;
        while(reader.hasNextTimeStep()) {
            SpatialBinGrid grid = reader.getNextTimeStep();
            VTKWriter writer;
            QString fileName = QString("%1.%2.vtk").arg(outFileName).arg(currentTimestep);
            qDebug() << "Will write VTK file with " << grid.numValues() << " values per chunk and " << grid.voxels().size() << " chunks to " << fileName;
            writer.write(grid, fileName, voxelSize,customFunctor);
            currentTimestep += 1;
        }
    } else {
        qDebug() << "Will write timesteps " << timestep << " to VTK file.";
        int currentTimestep = 0;
        while(reader.hasNextTimeStep()) {
            SpatialBinGrid grid = reader.getNextTimeStep();
            if(timestep==currentTimestep) {
                VTKWriter writer;
                qDebug() << "Will write VTK file with " << grid.numValues() << " values per chunk and " << grid.voxels().size() << " chunks to " << outFileName+".vtk";
                writer.write(grid, outFileName+".vtk", voxelSize, customFunctor);
                return 0;
            }
            currentTimestep += 1;
        }
    }


    return 0;
}
