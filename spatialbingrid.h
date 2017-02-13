#ifndef SPATIALBINGRID_H
#define SPATIALBINGRID_H

#include <QVector>
#include <QVector3D>


struct Voxel {
    QVector3D center;
    QVector<float> values;
    float numParticles;
};

class SpatialBinGrid
{
public:
    SpatialBinGrid();
    int getIndex(int i, int j, int k) { return i*m_ny*m_nz + j*m_nz + k; }
    Voxel &operator()(int i, int j, int k) { return m_voxels[getIndex(i,j,k)]; }
    QVector<Voxel> &voxels();
    int numValues() const;
    void setNumValues(int numValues);

    QString title() const;
    void setTitle(const QString &title);

    int nx() const;
    void setNx(int nx);

    int ny() const;
    void setNy(int ny);

    int nz() const;
    void setNz(int nz);

private:
    int m_nx, m_ny, m_nz;
    QVector<Voxel> m_voxels;
    QString m_title;
};

#endif // SPATIALBINGRID_H
