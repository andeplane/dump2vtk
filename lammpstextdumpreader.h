#ifndef LAMMPSTEXTDUMPREADER_H
#define LAMMPSTEXTDUMPREADER_H

#include <QFile>
#include "spatialbingrid.h"

class LAMMPSTextDumpReader
{
public:
    LAMMPSTextDumpReader(QString fileName, int nx, int ny, int nz);
    SpatialBinGrid getNextTimeStep();
    QString fileName() const;
    void setFileName(const QString &fileName);

private:
    int m_nx, m_ny, m_nz;
    QString m_fileName;
    QFile m_file;
};

#endif // LAMMPSTEXTDUMPREADER_H
