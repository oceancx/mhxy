#include "GameMap.h"
#include "Astar.h"
#include <iostream>

#include <fstream>

GameMap::GameMap(int width, int height, int cell_row, int cell_col, int** cells)
{
	mWidth = width / 20;
	mHeight = height / 20;
	mMap = new int*[mWidth];
	for (int i = 0; i < mWidth; i++) 
	{
		mMap[i] = new int[mHeight];
	}

	int x, y;
	x = 0;
	y = 0;
	for (int i = 0; i < cell_row; i++) 
	{
		for (int j = 0; j < cell_col; j++)
		{
			y = 12 * i;	//确定y的开始坐标
			for (int p = 0; p < 12; p++)
			{
				x = 16 * j;	//确定x的开始坐标
				for(int q = 0;q < 16;q++)
				{	
					mMap[x++][y] = cells[i*cell_col+j][p*16+q];
				}
				y++;
			}
		}
	}

	/*
	std::ofstream outfile("a.map");
	for (int y = 0; y < mHeight; y++) 
	{
		for (int x = 0; x < mWidth; x++) {
			//printf("%d", mMap[x][y]);
			outfile << mMap[x][y];
		}
		outfile << '\n';
		//printf("\n");
	}
	outfile.close();
	*/
}


bool GameMap::Move(int sx, int sy, int ex, int ey,std::list<GameMap::Pos>& posList)
{
	Astar astar;
	bool res =	astar.PathFinding(sx, sy, ex, ey, mWidth, mHeight,mMap, posList);
	for(auto p : posList)
	{
		printf("pos x:%d y:%d\n", p.x, p.y);
	}
	return res;
}


GameMap::~GameMap()
{
	if (mMap != nullptr) {
		for (int x = 0; x < mWidth; x++) {
			if (mMap[x] != nullptr) {
				delete[] mMap[x];
				mMap[x] = nullptr;
			}
		}
		delete[] mMap;
		mMap= nullptr;
	}
}
