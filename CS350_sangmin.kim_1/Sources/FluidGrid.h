#pragma once
#include <set>
#include <vector>

#include "FluidParticle.h"

class FluidGrid
{
public:
	FluidGrid(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax, float gapVal, float hVal);
	~FluidGrid();
	void Add(pWaterParticle val);
	void GetNeighbors(pWaterParticle val, std::vector<Indices>& neighbors);
	std::set<pWaterParticle>* Get(int x, int y, int z);
	void Adjust();
private:
	void BoundaryCheck(int& xIndex, int& yIndex, int& zIndex);
	

	
	std::set<pWaterParticle>**** grid;
	float gap;
	float xMin;
	float xMax;
	float yMin;
	float yMax;
	float zMin;
	float zMax;
	float h;
	int xNum, yNum, zNum;
};