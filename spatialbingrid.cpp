#include "spatialbingrid.h"

SpatialBinGrid::SpatialBinGrid() : m_nx(0), m_ny(0), m_nz(0)
{

}

QVector<Voxel> &SpatialBinGrid::voxels()
{
    return m_voxels;
}

int SpatialBinGrid::numValues() const
{
    if(m_voxels.size()>0) {
        return m_voxels.front().values.size();
    }
    return 0;
}

QString SpatialBinGrid::title() const
{
    return m_title;
}

void SpatialBinGrid::setTitle(const QString &title)
{
    m_title = title;
}

int SpatialBinGrid::nx() const
{
    return m_nx;
}

void SpatialBinGrid::setNx(int nx)
{
    m_nx = nx;
}

int SpatialBinGrid::ny() const
{
    return m_ny;
}

void SpatialBinGrid::setNy(int ny)
{
    m_ny = ny;
}

int SpatialBinGrid::nz() const
{
    return m_nz;
}

void SpatialBinGrid::setNz(int nz)
{
    m_nz = nz;
}
