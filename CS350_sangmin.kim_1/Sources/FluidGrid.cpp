#include "FluidGrid.h"


FluidGrid::FluidGrid(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax, float gapVal, float hVal)
{
	this->xMin = xmin;
	this->xMax = xmax;
	this->yMin = ymin;
	this->yMax = ymax;
	this->zMin = zmin;
	this->zMax = zmax;
	this->gap = gapVal;
	this->h = hVal;
	
	this->xNum = static_cast<int>(ceil((xMax - xMin) / gap));
	this->yNum = static_cast<int>(ceil((yMax - yMin) / gap));
	this->zNum = static_cast<int>(ceil((zMax - zMin) / gap));

	grid = new std::set<pWaterParticle>***[xNum];
	
	for(int i = 0; i < xNum; ++i)
	{
		grid[i] = new std::set<pWaterParticle>**[yNum];
		for(int j = 0 ; j < yNum; ++j)
		{
			grid[i][j] = new std::set<pWaterParticle>*[zNum];
			for(int k = 0; k < zNum; ++k)
			{
				grid[i][j][k] = new std::set<pWaterParticle>();
			}
		}
	}
}

FluidGrid::~FluidGrid()
{
	for (int i = 0; i < xNum; i++) 
	{
		for (int j = 0; j < yNum; j++) 
		{
			for (int k = 0; k < zNum; k++) 
			{
				grid[i][j][k]->clear();
				delete grid[i][j][k];
			}
			delete[] grid[i][j];
		}
		delete[] grid[i];
	}
	delete[] grid;
}

void FluidGrid::Add(pWaterParticle val)
{
	int ix = static_cast<int>((val->pos.x - xMin) / gap);
	int iy = static_cast<int>((val->pos.y - yMin) / gap);
	int iz = static_cast<int>((val->pos.z - zMin) / gap);

	BoundaryCheck(ix, iy, iz);

	grid[ix][iy][iz]->insert(val);
}

void FluidGrid::GetNeighbors(pWaterParticle val, std::vector<Indices>& neighbors)
{
	int ix = static_cast<int>((val->pos.x - xMin) / gap);
	int iy = static_cast<int>((val->pos.y - yMin) / gap);
	int iz = static_cast<int>((val->pos.z - zMin) / gap);

	BoundaryCheck(ix, iy, iz);

	int ixMin = static_cast<int>((val->pos.x - h - xMin) / gap);
	int ixMax = static_cast<int>((val->pos.x + h - xMin) / gap);
	int iyMin = static_cast<int>((val->pos.y - h - yMin) / gap);
	int iyMax = static_cast<int>((val->pos.y + h - yMin) / gap);
	int izMin = static_cast<int>((val->pos.z - h - zMin) / gap);
	int izMax = static_cast<int>((val->pos.z + h - zMin) / gap);

	BoundaryCheck(ixMin, iyMin, izMin);
	BoundaryCheck(ixMax, iyMax, izMax);

	for(int i = ixMin; i < ixMax + 1; ++i)
	{
		for(int j = iyMin; j < iyMax + 1; ++j)
		{
			for(int k = izMin; k < izMax + 1; ++k)
			{
				Indices index{};
				index.ix = i;
				index.iy = j;
				index.iz = k;
				neighbors.push_back(index);
			}
		}
	}
}

std::set<pWaterParticle>* FluidGrid::Get(int x, int y, int z)
{
	return grid[x][y][z];
}

void FluidGrid::Adjust()
{
	for(int i = 0 ; i < xNum; ++i)
	{
		for(int j = 0 ; j < yNum; ++j)
		{
			for(int k = 0 ; k < zNum; ++k)
			{
				std::set<pWaterParticle>* m = grid[i][j][k];

				for(std::set<pWaterParticle>::iterator iter = m->begin(); iter != m->end();)
				{
					int ix = static_cast<int>(((*iter)->pos.x - xMin) / gap);
					int iy = static_cast<int>(((*iter)->pos.y - yMin) / gap);
					int iz = static_cast<int>(((*iter)->pos.z - zMin) / gap);
					BoundaryCheck(ix, iy, iz);

					if(ix != i || iy != j || iz != k)
					{
						pWaterParticle particle = *iter;
						m->erase(iter++);
						grid[ix][iy][iz]->insert(particle);
					}
					else
					{
						iter++;
					}
				}
			}
		}
	}
}


void FluidGrid::BoundaryCheck(int& xIndex, int& yIndex, int& zIndex)
{
	if(xIndex < 0)
	{
		xIndex = 0;
	}
	if(xIndex > xNum - 1)
	{
		xIndex = xNum - 1;
	}
	if(yIndex < 0)
	{
		yIndex = 0;
	}
	if(yIndex > yNum - 1)
	{
		yIndex = yNum - 1;
	}
	if(zIndex < 0)
	{
		zIndex = 0;
	}
	if(zIndex > zNum - 1)
	{
		zIndex = zNum - 1;
	}
}
