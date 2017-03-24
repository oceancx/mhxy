#pragma once
#include<list>
class GameMap
{
public:
	struct Pos
	{
		int x;
		int y;
	};

	GameMap(int width,int height,int cell_row,int cell_col,int** cells );
	bool Move(int sx, int sy, int ex, int ey, std::list<Pos>& posList);
	~GameMap();
private :
	int mWidth;
	int mHeight;
	int** mMap;
	
};

