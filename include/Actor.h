#pragma once
#define MOVE 1
#define STAY 2
#include<vector>
class Actor
{
public:
	Actor():pos_x(0), pos_y(0) {};
	Actor(int x,int y,int row,int col,int**map);
	~Actor();
	void move(int x, int y);
	
	void update(float dt);

	//地图格子
	struct MapBox
	{
		int x, y;
		MapBox* next;
	};

	int pos_x, pos_y; //当前坐标
	std::vector<MapBox> moveBoxes;
	int dest_x, dest_y;
	int state;
	int** mMap;
	int mRow, mCol;

};

