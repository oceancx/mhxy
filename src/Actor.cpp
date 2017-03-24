#include "Actor.h"
#include <vector>

std::vector<Actor::MapBox> calcMapBoxes(int pos_x, int pos_y,int  dest_x,int  dest_y,
	int mRow,int  mCol,int** mMap);

Actor::Actor(int x,int y,int row,int col,int **map)
{
	pos_x = x;
	pos_y = y;
	dest_x = pos_x;
	dest_y = pos_y;
	state = STAY;
	moveBoxes.clear();
	mMap = map;
	mRow = row;
	mCol = col;
}


Actor::~Actor()
{
}

void Actor::move(int x,int y) 
{
	x = x / 20 * 20 + 10;
	y = y / 20 * 20 + 10;

	dest_x = x;
	dest_y = y;
	if (dest_x == pos_x && dest_y == pos_y) 
	{
		return;
	}
	else
	{
		state = MOVE;
		/*根据坐标得到MapBox*/
		moveBoxes=calcMapBoxes(pos_x, pos_y, dest_x, dest_y, mRow, mCol, mMap);
	}
}

void Actor::update(float dt)
{
	
}

std::vector<Actor::MapBox> calcMapBoxes(int sx, int sy, int  dx, int  dy,
	int row, int  col, int** map)
{
	return{};
}