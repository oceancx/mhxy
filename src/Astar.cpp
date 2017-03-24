#include "Astar.h"
#include<fstream>

Astar::Astar()
{

}

Astar::~Astar()
{

}

Astar::MapNode* Astar::NewNode(int x, int y)
{
	MapNode*tmpP2 = new MapNode();
	tmpP2->x = x;
	tmpP2->y = y;
	tmpP2->F = 0;
	tmpP2->H = 0;
	tmpP2->G = 0;
	tmpP2->parant = nullptr;
	return tmpP2;
}


void Astar::PrintMap(std::string filename, int w,int h,int** map)
{
	std::ofstream fout(filename);
	for (int i = 0; i<h; i++) {
		for (int j = 0; j<w; j++) {
			if (map[j][i] == 0) {
				fout << '0';
				// printf("0");
			}
			else if (map[j][i] == 1) {
				fout << '1';
				// printf("1");
			}
			else if (map[j][i] == 33) {
				fout << '*';
				// printf("1");
				map[j][i] == 0;
			}
			
		}
		fout << '\n';
	}
	fout.close();
}

std::list<Astar::MapNode*>::iterator Astar::PopLowestF() {
	// std::list<MapNode*>::iterator begin,end;
	auto begin = mOpenList.begin();
	auto end = mOpenList.end();
	std::list<MapNode*>::iterator res = end;

	int min_f = 1 << 28;

	while (begin != end) {
		if (min_f > (*begin)->F) {
			min_f = (*begin)->F;
			res = begin;
		}
		begin++;
	}
	if (res != mOpenList.end()) {
		// printf("LowestF: %d\n", (*res)->F );
	}

	return res;
}

bool Astar::IsInCloseList(int x, int y)
{
	for (auto element : mCloseList) {
		if (x == element->x && y == element->y) {
			// printf("%d %d\n", x,y );
			return true;
		}
	}
	return false;
}

std::list<Astar::MapNode*>::iterator Astar::FindInOpenList(int x, int y)
{
	std::list<Astar::MapNode*>::iterator cur, end;
	cur = mOpenList.begin();
	end = mOpenList.end();
	while (cur != end) {
		if ((*cur)->x == x && (*cur)->y == y) {
			return cur;
		}
		cur++;
	}
	return end;
}

bool Astar::IsInOpenList(int x, int y)
{
	for (auto element : mOpenList) {
		if (x == element->x && y == element->y) {
			// printf("%d %d\n", x,y );
			return true;
		}
	}
	return false;
}

bool Astar::PathFinding(int sx, int sy, int ex, int ey, int width, int height,
	int** map, std::list<GameMap::Pos>& posList)
{
	// 	13 ，35
	// 115，20
	// if (mPs != nullptr) {
	// 	delete mPs;
	// 	mPs = nullptr;
	// }
	// if (mPe != nullptr) {
	// 	delete mPe;
	// 	mPe = nullptr;
	// }

	// if(map ==nullptr || posList.empty())return false;
	mOpenList.clear();
	mCloseList.clear();
	

	mPs = NewNode(sx, sy);
	mPe = NewNode(ex, ey);
	mOpenList.push_back(mPs);
	//map[mPs->x][mPs->y] = 4;

	while (!mOpenList.empty()) {
		//找到最低价的 弹出
		auto tmp_iterator = PopLowestF();

		MapNode* cur_node = *tmp_iterator;

	//	printf("x %d y %d \n", (*tmp_iterator)->x, (*tmp_iterator)->y);
		if (tmp_iterator != mOpenList.end()) {
	//	 printf("%d %d \n", (cur_node)->x,(cur_node)->y );
			mOpenList.erase(tmp_iterator);
//			printf("after %d %d \n", (cur_node)->x, (cur_node)->y);
		// printf("mOpenList Size:%ld\n", mOpenList.size() );
		}
		else {
			//OpneList为空，说明路径没有找到
			break;
		}
		mCloseList.push_back(cur_node);
		//mCloseList.push_back(cur_node);
	//	map[(cur_node)->x][(cur_node)->y] = 4;
		if ((cur_node)->x == mPe->x && (cur_node)->y == mPe->y) {
			//找到了路径 从cur_node这里开始回溯
			//std::list<GameMap::Pos>* posList = new std::list<GameMap::Pos>();
			GameMap::Pos tmpPos;
			MapNode* p = cur_node;
			do {
				//printf("x: %d y: %d\n", p->x , p->y );
				map[p->x][p->y] = 33;
				// MapNode* curP = p;
				tmpPos.x = p->x;
				tmpPos.y = p->y;
				posList.push_front(tmpPos);

				if (p->y == mPs->y && p->x == mPs->x) {
					break;
				}
				p = p->parant;
				// delete curP;
			} while (p != nullptr);

			PrintMap("x.ttt",width, height, map);

			mOpenList.clear();
			mCloseList.clear();
			return true;
		}

		for (int i = 0; i<8; i++) {
			int new_x = (cur_node)->x + dir_x[i];
			int new_y = (cur_node)->y + dir_y[i];

			int new_G_dist = i<4 ? 10 : 14;

			if (map[new_x][new_y] == 1 || IsInCloseList(new_x, new_y) ) {
				continue;
			}
			else if (!IsInOpenList(new_x, new_y)) {
				MapNode* newP = NewNode(new_x, new_y);

				newP->G = (cur_node)->G + new_G_dist;
				newP->H = abs(new_x - mPe->x) * 10 + abs(new_y - mPe->y) * 10;
				newP->F = newP->H + newP->G;
				newP->parant = (cur_node);
				// printf("new F:%d=G(%d)+H(%d)\n",newP->F ,newP->G,newP->H );
				mOpenList.push_back(newP);
			}
			else {
				std::list<MapNode*>::iterator iter = FindInOpenList(new_x, new_y);
				if (iter != mOpenList.end()) {
					if ((cur_node)->G + new_G_dist < (*iter)->G) {
						(*iter)->G = (cur_node)->G + new_G_dist;
						(*iter)->F = (*iter)->G + (*iter)->H;
						(*iter)->parant = cur_node;
					}
				}
			}
		}
	}

	PrintMap("x.ttt", width, height, map);
	return false;
}