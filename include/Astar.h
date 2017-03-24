#pragma once
#include "GameMap.h"
#include <string>
#include <list>
class Astar
{
public:
	struct MapNode
	{
		int x, y;
		int F, G, H;
		MapNode* parant;
	};


	Astar();
	~Astar();

	
	
	MapNode *mPs, *mPe;
	std::list<MapNode*> mOpenList;
	std::list<MapNode*> mCloseList;
	//¡ü ¡ú ¡ý ¡û ¨J ¨K ¨L ¨I
	int dir_x[8] = { 0, 1, 0, -1, 1, 1, -1, -1 };
	int dir_y[8] = { -1, 0, 1, 0,-1, 1, 1,-1 };

	
	MapNode* NewNode(int x, int y);
	void PrintMap(std::string filename, int w, int h, int** mMap);
	bool IsInCloseList(int x, int y);
	bool IsInOpenList(int x, int y);
	std::list<MapNode*>::iterator FindInOpenList(int x, int y);
	std::list<MapNode*>::iterator PopLowestF();
	
	bool PathFinding(int sx, int sy, int ex, int ey, int width, int height, int** map,
		std::list<GameMap::Pos>& posList);
		


};

