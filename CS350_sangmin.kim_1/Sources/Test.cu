#include "SandParticle.h"
#include <cuda_runtime.h>
#include "device_launch_parameters.h"
#include <stdio.h>
#include "Affine.h"
#include <atomic>
#include <vector>
const int width = 100;
const int height = 10;
const int gridWidth = 800;
const int gridHeight = 800;
const dim3 threadsPerBlock(16, 16);
const int blockSize = 256;
__constant__ float gap = 0.01f;
#define gpuErrchk(ans) { gpuAssert((ans), __FILE__, __LINE__); }

inline void gpuAssert(cudaError_t code, const char* file, int line, bool abort = true)
{
	if (code != cudaSuccess)
	{
		fprintf(stderr, "GPUassert: %s %s %d\n", cudaGetErrorString(code), file, line);
		if (abort) exit(code);
	}
}

#define cudaCheck(x) { cudaError_t err = x; if (err != cudaSuccess) { printf("Cuda error: %d in %s at %s:%d\n", err, #x, __FILE__, __LINE__); assert(0); } }

__global__ void SetGrid(ParticleGrid* grids)
{
	const int index = threadIdx.x + (blockIdx.x * blockDim.x);
	
	ParticleGrid& grid = grids[index];
	float xPos = -5 + ((index % gridWidth) * gap);
	float yPos = ((index / gridHeight) * gap);

	grid.gridPos.x = xPos;
	grid.gridPos.y = yPos;
	grid.index = index;
	grid.status = Empty;
	grid.predictedMoveInWaterParticleIndex = -1;
	grid.landIndex = -1;
}

__global__ void SetSpawner(ParticleGrid* grid, SpawnerPos* spawnerPos, int startIndex, int width, int i)
{
	int gridIndex = startIndex + threadIdx.x + (i * gridWidth);
	ParticleGrid& currGrid = grid[gridIndex];
	int iIndex = i * width;

	//currGrid.status = FilledWithLand;
	spawnerPos[iIndex + threadIdx.x].pos = currGrid.gridPos;
	spawnerPos[iIndex + threadIdx.x].currGridIndex = currGrid.index;
}

__global__ void SetLand(ParticleGrid* grid, Land* land, int landStartIndex, int landWidth, int i)
{

	int index = landStartIndex + threadIdx.x;
	int ogLandStartIndex = landStartIndex;
	ParticleGrid& currGrid = grid[index];
	int iIndex = i * landWidth;

	currGrid.status = FilledWithLand;
	land[iIndex + threadIdx.x].landPos = currGrid.gridPos;

}

__global__ void CheckGridPos(ParticleSand* particles, ParticleGrid* grids)
{
	const int index = threadIdx.x + (blockIdx.x * blockDim.x);
	const int startIndex = gridWidth * 500 + 400;

	const int widthVal = index % width;
	const int heightVal = index / width;
	const int heightInGrid = heightVal * gridWidth;
	

	particles[index].currGrid = &grids[startIndex + heightInGrid + widthVal];
	particles[index].pos = grids[startIndex + heightInGrid + widthVal].gridPos;

}

__global__ void SpawnerMove(ParticleGrid* grids, SpawnerPos* spawnerPos, int dir)
{
	const int index = threadIdx.x + (blockIdx.x * blockDim.x);
	const int currGridIndex = spawnerPos[index].currGridIndex;

	//printf("currGrid : %d", currGridIndex);

	const int destGridIndex = currGridIndex + dir;

	SpawnerPos& spawner = spawnerPos[index];

	spawner.currGridIndex = destGridIndex;
	spawner.pos = grids[destGridIndex].gridPos;
}

__global__ void Spawn(ParticleSand* particle, ParticleGrid* grid, SpawnerPos* spawners, int lastIndex)
{
	int index = lastIndex + threadIdx.x;

	ParticleSand& part = particle[index];
	SpawnerPos& spawner = spawners[threadIdx.x];
	ParticleGrid& gridInfo = grid[spawner.currGridIndex];

	part.pos = spawner.pos;
	part.currGrid = &grid[spawner.currGridIndex];
	gridInfo.status = FilledWithSand;
}

__global__ void SpawnLand(Land* lands, ParticleGrid* grid, SpawnerPos* spawners, int lastIndex)
{
	int index = lastIndex + threadIdx.x + (blockIdx.x * blockDim.x);

	Land& land = lands[index];
	SpawnerPos& spawner = spawners[threadIdx.x];
	ParticleGrid& gridInfo = grid[spawner.currGridIndex];

	land.landPos = spawner.pos;
	land.currGridIndex = spawner.currGridIndex;
	gridInfo.status = FilledWithLand;
}

__global__ void SpawnWater(ParticleWater* waters, ParticleGrid* grid, SpawnerPos* spawners, int lastIndex)
{
	int index = lastIndex + threadIdx.x + (blockIdx.x * blockDim.x);

	ParticleWater& water = waters[index];
	SpawnerPos& spawner = spawners[threadIdx.x];
	ParticleGrid& gridInfo = grid[spawner.currGridIndex];

	water.pos = spawner.pos;
	water.currGridIndex = spawner.currGridIndex;
	gridInfo.status = FilledWithWater;
}

__global__ void MoveWaterFromPredictedPosition(ParticleWater* particles, ParticleGrid* grids, int particleNum)
{
	const int index = threadIdx.x + (blockIdx.x * blockDim.x);

	if(index >= particleNum)
	{
		printf("over");
		return;
	}

	ParticleWater& particle = particles[index];

	if(particle.predictMoveInGridIndex != -1)
	{
		const int predictMoveInGridIndex = particle.predictMoveInGridIndex;
		ParticleGrid& predictedGrid = grids[predictMoveInGridIndex];
		ParticleGrid& currGrid = grids[particle.currGridIndex];
		
		if(predictedGrid.predictedMoveInWaterParticleIndex == index)
		{
			//move
			particle.pos = predictedGrid.gridPos;
			particle.currGridIndex = predictedGrid.index;
			particle.predictMoveInGridIndex = -1;

			predictedGrid.status = FilledWithWater;
			predictedGrid.predictedMoveInWaterParticleIndex = -1;

			currGrid.status = Empty;
		}
		/*else
		{
			const int leftIndex = predictMoveInGridIndex - 1;
			const int rightIndex = predictMoveInGridIndex + 1;
			const int upIndex = predictMoveInGridIndex - gridWidth;
			const int downIndex = predictMoveInGridIndex + gridWidth;

			ParticleGrid& leftGrid = grids[leftIndex];
			ParticleGrid& rightGrid = grids[rightIndex];
			ParticleGrid& downGrid = grids[downIndex];
			ParticleGrid& upGrid = grids[upIndex];
			int newDestGridIndex = -1;

			if(leftGrid.predictedMoveInWaterParticleIndex == -1 && leftGrid.status == Empty)
			{
				newDestGridIndex = leftIndex;
			}
			else if(rightGrid.predictedMoveInWaterParticleIndex == -1 && rightGrid.status == Empty)
			{
				newDestGridIndex = rightIndex;
			}
			else if (downGrid.predictedMoveInWaterParticleIndex == -1 && downGrid.status == Empty)
			{
				newDestGridIndex = downIndex;
			}
			else if (upGrid.predictedMoveInWaterParticleIndex == -1 && upGrid.status == Empty)
			{
				newDestGridIndex = upIndex;
			}

			if(newDestGridIndex != -1)
			{
				ParticleGrid& newDestGrid = grids[newDestGridIndex];
				
				particle.pos = newDestGrid.gridPos;
				particle.currGridIndex = newDestGrid.index;
				particle.predictMoveInGridIndex = -1;

				newDestGrid.status = FilledWithWater;
				newDestGrid.predictedMoveInWaterParticleIndex = -1;

				currGrid.status = Empty;
			}
		}*/

	}

}

__global__ void MoveDownWater(ParticleWater* particles, ParticleGrid* grids, int particleNum)
{
	const int index = threadIdx.x + (blockIdx.x * blockDim.x);
	if (index >= particleNum)
	{
		return;
	}

	ParticleWater& particle = particles[index];
	ParticleGrid& currGrid = grids[particle.currGridIndex];

	const int gridIndex = currGrid.index;
	int gridDownIndex = gridIndex - gridWidth;
	int gridLeftDownIndex = gridIndex - (gridWidth - 1);
	int gridRightDownIndex = gridIndex - (gridWidth + 1);
	int gridLeftIndex = gridIndex - 1;
	int gridRightIndex = gridIndex + 1;

	__shared__ ParticleGrid gridsInfo[256 * 2000];
	
	if (gridIndex < gridWidth)
	{
		gridDownIndex = gridIndex;
		gridLeftDownIndex = gridIndex;
		gridRightDownIndex = gridIndex;
		gridLeftIndex = gridIndex;
		gridRightIndex = gridIndex;
	}
	if (gridIndex % gridWidth == 0)
	{
		gridLeftDownIndex = gridIndex;
		gridLeftIndex = gridIndex;
	}
	if (gridIndex % gridWidth == gridWidth - 1)
	{
		gridRightDownIndex = gridIndex;
		gridRightIndex = gridIndex;
	}

	if (gridDownIndex <= 0 || gridDownIndex >= 256 * 2000 ||
		gridLeftDownIndex <= 0 || gridLeftDownIndex >= 256 * 2000 ||
		gridRightDownIndex <= 0 || gridRightDownIndex >= 256 * 2000 || 
		gridLeftIndex <= 0 || gridLeftIndex >= 256 * 2000 ||
		gridRightIndex <= 0 || gridRightIndex >= 256 * 2000)
	{
		return;
	}

	ParticleGrid& downGrid = grids[gridDownIndex];
	ParticleGrid& leftDownGrid = grids[gridLeftDownIndex];
	ParticleGrid& rightDownGrid = grids[gridRightDownIndex];
	ParticleGrid& leftGrid = grids[gridLeftIndex];
	ParticleGrid& rightGrid = grids[gridRightIndex];

	if (downGrid.status == Empty)
	{
		/*particle.pos = downGrid.gridPos;
		particle.currGridIndex = downGrid.index;

		currGrid.status = Empty;
		downGrid.status = FilledWithWater;*/
		downGrid.predictedMoveInWaterParticleIndex = index;
		particle.predictMoveInGridIndex = gridDownIndex;
	}
	else if (leftDownGrid.status == Empty)
	{
		/*particle.pos = leftDownGrid.gridPos;
		particle.currGridIndex = leftDownGrid.index;
		
		currGrid.status = Empty;
		leftDownGrid.status = FilledWithWater;*/
		leftDownGrid.predictedMoveInWaterParticleIndex = index;
		particle.predictMoveInGridIndex = gridLeftDownIndex;
	}
	else if (rightDownGrid.status == Empty)
	{
		//particle.pos = rightDownGrid.gridPos;
		//particle.currGridIndex = rightDownGrid.index;
		//
		//currGrid.status = Empty;
		//rightDownGrid.status = FilledWithWater;
		rightDownGrid.predictedMoveInWaterParticleIndex = index;
		particle.predictMoveInGridIndex = gridRightDownIndex;
	}
	else if (leftGrid.status == Empty)
	{
		
		//particle.pos = grids[gridLeftIndex].gridPos;
		//particle.currGridIndex = grids[gridLeftIndex].index;

		//currGrid.status = Empty;
		//grids[gridLeftIndex].status = FilledWithWater;

		leftGrid.predictedMoveInWaterParticleIndex = index;
		particle.predictMoveInGridIndex = gridLeftIndex;
	}
	else if (rightGrid.status == Empty)
	{
		//particle.pos = grids[gridRightIndex].gridPos;
		//particle.currGridIndex = grids[gridRightIndex].index;

		//currGrid.status = Empty;
		//grids[gridRightIndex].status = FilledWithWater;
		
		rightGrid.predictedMoveInWaterParticleIndex = index;
		particle.predictMoveInGridIndex = gridRightIndex;
	}
	else
	{
		particle.predictMoveInGridIndex = -1;
	}
}

__global__ void MoveDown(ParticleSand* particles, ParticleGrid* grids, int particleNum)
{
	const int index = threadIdx.x + (blockIdx.x * blockDim.x);
	if (index >= particleNum)
	{
		return;
	}
	
	ParticleSand& particle = particles[index];
	ParticleGrid& currGrid = *particle.currGrid;

	const int gridIndex = currGrid.index;
	int gridDownIndex = gridIndex - gridWidth;
	int gridLeftDownIndex = gridIndex - (gridWidth - 1);
	int gridRightDownIndex = gridIndex - (gridWidth + 1);

	if (gridIndex < gridWidth)
	{
		gridDownIndex = gridIndex;
		gridLeftDownIndex = gridIndex;
		gridRightDownIndex = gridIndex;
	}
	if (gridIndex % gridWidth == 0)
	{
		gridLeftDownIndex = gridIndex;
	}
	if (gridIndex % gridWidth == gridWidth - 1)
	{
		gridRightDownIndex = gridIndex;
	}

	if (gridDownIndex <= 0 || gridDownIndex >= 256 * 2000 ||
		gridLeftDownIndex <= 0 || gridLeftDownIndex >= 256 * 2000 ||
		gridRightDownIndex <= 0 || gridRightDownIndex >= 256 * 2000)
	{
		return;
	}

	ParticleGrid& downGrid = grids[gridDownIndex];
	ParticleGrid& leftDownGrid = grids[gridLeftDownIndex];
	ParticleGrid& rightDownGrid = grids[gridRightDownIndex];

	if (downGrid.status == Empty)
	{
		particle.pos = downGrid.gridPos;
		particle.currGrid = &downGrid;

		currGrid.status = Empty;
		downGrid.status = FilledWithSand;
	}
	else if (leftDownGrid.status == Empty)
	{
		particle.pos = leftDownGrid.gridPos;
		particle.currGrid = &leftDownGrid;

		currGrid.status = Empty;
		leftDownGrid.status = FilledWithSand;
	}
	else if (rightDownGrid.status == Empty)
	{
		particle.pos = rightDownGrid.gridPos;
		particle.currGrid = &rightDownGrid;

		currGrid.status = Empty;
		rightDownGrid.status = FilledWithSand;
	}
}
__global__ void DeleteLand(Land* lands, ParticleGrid* grids, SpawnerPos* spawners)
{
	int index = threadIdx.x + (blockDim.x * blockIdx.x);

	SpawnerPos& spawnerInfo = spawners[index];
	ParticleGrid& gridInfo = grids[spawnerInfo.currGridIndex];

	if(gridInfo.status == FilledWithLand)
	{
		Land& landInfo = lands[gridInfo.landIndex];

		landInfo.currGridIndex = 0;
		landInfo.landPos = grids[landInfo.currGridIndex].gridPos;
		
		gridInfo.status = Empty;
		gridInfo.landIndex = 0;
	}
}
__global__ void UpdateGrid(ParticleSand* sands, ParticleWater* waters, ParticleGrid* grids)
{
	int index = threadIdx.x + (blockDim.x * blockIdx.x);

	extern __shared__ ParticleGrid gridsInfo[];

	ParticleGrid& grid = grids[index];
	//gridsInfo
	if(grid.status == Empty)
	{
		
	}
}


void DeleteLands(Land* lands, ParticleGrid* grids, SpawnerPos* spawners)
{
	DeleteLand << <1, 256 >> > (lands, grids, spawners);
}

void SimulationUpdate(int particleNum, int gridNum, int waterNum, ParticleSand* particle, ParticleWater* water, ParticleGrid* grid)
{
	int sandGridCount = particleNum / blockSize;
	int waterGridCount = waterNum / blockSize;
	int gridGridCount = gridNum / blockSize;

	
	if(sandGridCount >= 1)
	{
		MoveDown << <sandGridCount, blockSize>> > (particle, grid, particleNum);
		gpuErrchk(cudaPeekAtLastError());
	}

	if(waterGridCount >= 1)
	{
		MoveDownWater << <waterGridCount, blockSize >> > (water, grid, waterNum);
		gpuErrchk(cudaPeekAtLastError());
		MoveWaterFromPredictedPosition << <waterGridCount, blockSize >> > (water, grid, waterNum);
		gpuErrchk(cudaPeekAtLastError());
	}

	//MoveSideWater << <waterGridCount, blockSize >> > (water, grid, waterNum);
	//gpuErrchk(cudaPeekAtLastError());
	
}
void AddWatersInSpawnerPos(ParticleWater* waters, ParticleGrid* grid, SpawnerPos* spawners, int lastIndex)
{
	SpawnWater << <1, blockSize >> > (waters, grid, spawners, lastIndex);
}
void AddSandsInContSpawnerPos(ParticleSand* particle, ParticleGrid* grid, Vector2* spawners, int lastIndex)
{
	
}

void AddSandsInSpawnerPos(ParticleSand* particle, ParticleGrid* grid, SpawnerPos* spawners, int lastIndex)
{
	Spawn << <1, blockSize >> > (particle, grid, spawners, lastIndex);
}
void AddLandsInSpawnerPos(Land* lands, ParticleGrid* grid, SpawnerPos* spawners, int lastIndex)
{
	SpawnLand << <1, 64>> > (lands, grid, spawners, lastIndex);
}

void MoveSpawner(ParticleGrid* grid, SpawnerPos* spawners, int dir, int spawnerCount)
{
	SpawnerMove << <1, spawnerCount >> > (grid, spawners, dir);
}



void SetLands(std::vector<int>& landStartRandomIndices, ParticleSand* particle, ParticleGrid* grid, Land* land)
{
	size_t vecSize = landStartRandomIndices.size();
	int landCount = 1;
	int landWidth = 50;
	int landStartIndex = gridWidth * 100 + 200;

	for (size_t i = 0; i < vecSize; ++i)
	{
		SetLand << <landCount, landWidth >> > (grid, land, landStartRandomIndices[i], landWidth, i);
	}

}
__global__ void LoadLand(Land* lands, ParticleGrid* grids, int* indices, int lastIndex)
{
	int index = threadIdx.x + (blockIdx.x * blockDim.x);
	
	if (index >= lastIndex)
	{
		printf("over");
		return;
	}
	const int gridIndex = indices[index];

	Land& land = lands[index];
	ParticleGrid& grid = grids[gridIndex];
	land.landPos = grid.gridPos;
	land.currGridIndex = grid.index;
	
	grid.status = FilledWithLand;
	grid.landIndex = index;
}

__global__ void SetContiSpawnerPos(SpawnerPos* poses, ParticleGrid* grids, int startIndex)
{
	const int index = threadIdx.x + (blockIdx.x * blockDim.x);

	SpawnerPos& pos = poses[index];

	const int gridIndex = startIndex + index * 2;

	pos.pos = grids[gridIndex].gridPos;
	pos.currGridIndex = gridIndex;
}

void LoadLands(int* landGridIndices, ParticleGrid* grid, Land* land, int landsNum)
{
	int landBlockCount = landsNum / blockSize;

	if (landBlockCount < 0)
		landBlockCount = 1;

	LoadLand << <landBlockCount, blockSize>> > (land, grid, landGridIndices, landsNum);
	gpuErrchk(cudaPeekAtLastError());
}

void Init(int particleNum, int gridNum, int spawnerNum, int contiSpawnerNum, ParticleSand* particle, ParticleGrid* grid, Land* land, SpawnerPos* spawners
, SpawnerPos* firstContiSpawner, SpawnerPos* secondContiSpawner)
{
	int particleGridCount = particleNum / blockSize;
	int gridCount = gridNum / blockSize;
	
	int spawnerStartIndex = gridWidth * 300 + 500;

	SetGrid << <gridCount, blockSize >> > (grid);
	//CheckGridPos << <particleGridCount, blockSize >> > (particle, grid);

	int spawnerWidth = sqrt(spawnerNum);
	int spwanerCount = 1;

	for (int i = 0; i < spawnerWidth; ++i)
	{
		SetSpawner << <spwanerCount, spawnerWidth >> > (grid, spawners, spawnerStartIndex, spawnerWidth, i);
	}
	int contiSpawnerGridCount = contiSpawnerNum / blockSize;

	const int firstContiSpawnerStartIndex = gridWidth * 600 + 200;
	const int secondContiSpawnerStartIndex = firstContiSpawnerStartIndex + 1;
	
	SetContiSpawnerPos << <contiSpawnerGridCount, blockSize >> > (firstContiSpawner, grid, firstContiSpawnerStartIndex);
	SetContiSpawnerPos << <contiSpawnerGridCount, blockSize >> > (secondContiSpawner, grid, secondContiSpawnerStartIndex);
	//gpuErrchk(cudaPeekAtLastError());
}