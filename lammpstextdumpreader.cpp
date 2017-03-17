#include "lammpstextdumpreader.h"
#include <QDebug>

LAMMPSTextDumpReader::LAMMPSTextDumpReader(QString fileName, int nx, int ny, int nz) :
    m_filePosition(0), m_nx(nx), m_ny(ny), m_nz(nz), m_fileName(fileName)
{
    m_file.setFileName(fileName);
    if(!m_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        char error[1024];
        sprintf(error, "Error: could not open file %s", m_fileName.toUtf8().constData());
        qFatal(error);
    }
    QTextStream stream(&m_file);
    stream.readLine();
    stream.readLine();

    QString line = stream.readLine();
    QStringList words = line.split(" ", QString::SplitBehavior::SkipEmptyParts);
    qDebug() << "Read header: " << words;
    for(int i=6; i<words.length(); i++) {
        m_headers.append(words[i]);
    }
    qDebug() << "Actually headers: " << m_headers;
    m_filePosition = stream.pos();
}

SpatialBinGrid LAMMPSTextDumpReader::getNextTimeStep()
{
    SpatialBinGrid grid;
    grid.setNx(m_nx);
    grid.setNy(m_ny);
    grid.setNz(m_nz);

    bool foundHeader = false;
    QTextStream stream(&m_file);
    stream.seek(m_filePosition);
    QVector3D voxelSize(1.0/m_nx, 1.0/m_ny, 1.0/m_nz);
    int voxelCount = 0;
    int numVoxelsInTimestep = 0;
    int timestep = 0;
    int numAtoms = 0;
    while(!stream.atEnd()) {
        QString line = stream.readLine().trimmed();
        // qDebug() << "Line: " << line;
        QStringList words = line.split(" ", QString::SplitBehavior::SkipEmptyParts);

        if(!foundHeader) {
            qDebug() << "Words: " << words;
            if(words.size()!=3) {
                char error[1024];
                sprintf(error, "Error could not parse header: %s", line.toUtf8().constData());
                qFatal(error);
            }
            bool ok;
            timestep = words[0].toInt(&ok);
            if(!ok) { qDebug() << "Error, could not parse line: " << line; exit(1); }
            numVoxelsInTimestep = words[1].toInt(&ok);
            if(!ok) { qDebug() << "Error, could not parse line: " << line; exit(1); }
            numAtoms = int(words[2].toFloat(&ok));
            if(!ok) { qDebug() << "Error, could not parse line: " << line; exit(1); }
            if(numVoxelsInTimestep != m_nx*m_ny*m_nz) { qDebug() << "Error, number of voxels in timestep " << timestep << " does not match the input nx ny nz."; exit(1); }

            grid.voxels().resize(numVoxelsInTimestep);
            grid.setTitle(QString("Timestep %1").arg(timestep));
            foundHeader = true;
            continue;
        }

        // Parse values
        // Format:
        // #Chunk Coord1 Coord2 Coord3 Ncount value1 value2 value3

        int numValues = words.size()-5;
        bool ok;
        QVector3D center;
        center[0] = words[1].toFloat(&ok);
        if(!ok) { qDebug() << "Error, could not parse line: " << line; exit(1); }

        center[1] = words[2].toFloat(&ok);
        if(!ok) { qDebug() << "Error, could not parse line: " << line; exit(1); }

        center[2] = words[3].toFloat(&ok);
        if(!ok) { qDebug() << "Error, could not parse line: " << line; exit(1); }

        float numParticles = words[4].toFloat(&ok);
        if(!ok) { qDebug() << "Error, could not parse line: " << line; exit(1); }

        int i = center.x() / voxelSize.x();
        int j = center.y() / voxelSize.y();
        int k = center.z() / voxelSize.z();
        Voxel &voxel = grid(i,j,k);

        voxel.center = center;
        voxel.numParticles = numParticles;
        voxel.values.resize(numValues);

        for(int i=0; i<numValues; i++) {
            voxel.values[i] = words[i+5].toFloat(&ok);
            if(!ok) { qDebug() << "Error, could not parse line: " << line; exit(1); }
        }
        voxelCount++;
        if(voxelCount == numVoxelsInTimestep) break; // Finished with this timestep
    }
    m_filePosition = stream.pos();
    return grid;
}

bool LAMMPSTextDumpReader::hasNextTimeStep()
{
    QTextStream stream(&m_file);
    stream.seek(m_filePosition);
    return !stream.atEnd();
}

QString LAMMPSTextDumpReader::fileName() const
{
    return m_fileName;
}

void LAMMPSTextDumpReader::setFileName(const QString &fileName)
{
    m_fileName = fileName;
}

QStringList LAMMPSTextDumpReader::headers() const
{
    return m_headers;
}
