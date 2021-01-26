// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include "Player.h"
#include "Pawn.h"
#include "Enemy.h"
#include <time.h>
#include <stdlib.h>

#define WIDTH 26
#define HEIGHT 26
#define FLOORCOST 1
#define DESERTCOST 2
#define WATERCOST 3
#define SWAMPCOST 4
#define MTNCOST 6

using namespace std;

// used by A*
int startX;
int startY;

int goalX;
int goalY;
int coinX;
int coinY;

static int t1x = 12;
static int t1y = 0;
static int t2x = 12;
static int t2y = 25;

class Tile {
public:
	Tile();
	char tileType;
	double getCost();
	
	// used by A*
	int x;
	int y;
	int treeState;
	int parentX;
	int parentY;
	double gDist;
	int inPath;
};

char map[WIDTH][HEIGHT] = {
	{'.','.','.','.','.','.','.','.','.','.','.','.','^','^',',',',',',',',',',',',',',',',',',',',',',',','},
	{'.','.','.','.','.','.','.','.','.','.','.','.','^','^',',',',',',',',',',',',',',',',',',',',',',',','},
	{'.','.','.','.','.','.','.','.','.','.','.','.','^','^',',',',',',',',',',',',',',',',',',',',',',',','},
	{'.','.','.','.','.','.','.','.','.','.','.','.','^','^',',',',',',',',',',',',',',',',',',',',',',',','},
	{'.','.','.','.','.','.','.','.','.','.','.','.','^','^',',',',',',',',',',',',',',',',',',',',',',',','},
	{'.','.','.','.','.','.','.','.','.','.','.','.','^','^',',',',',',',',',',',',',',',',',',',',',',',','},
	{'.','.','.','.','.','.','.','.','.','.','.','.','^','^',',',',',',',',',',',',',',',',',',',',',',',','},
	{'.','.','.','.','.','.','.','.','.','.','.','.','^','^',',',',',',',',',',',',',',',',',',',',',',',','},
	{'.','.','.','.','.','.','.','.','.','.','.','.','^','^',',',',',',',',',',',',',',',',',',',',',',',','},
	{'.','.','.','.','.','.','.','.','.','.','.','.','^','^',',',',',',',',',',',',',',',',',',',',',',',','},
	{'~','~','.','.','.','.','.','.','.','.','.','.','^','^',',',',',',',',',',',',',',',',',',',',','~','~'},
	{'~','~','.','.','.','.','.','.','.','.','.','.','^','^',',',',',',',',',',',',',',',',',',',',','~','~'},
	{'T','~','~','*','*','*','*','*','*','*','*','*','^','^','*','*','*','*','*','*','*','*','*','~','~','T'},
	{'~','~',',',',',',',',',',',',',',',',',',',',','^','^','.','.','.','.','.','.','.','.','.','.','~','~'},
	{'~','~',',',',',',',',',',',',',',',',',',',',','^','^','.','.','.','.','.','.','.','.','.','.','~','~'},
	{',',',',',',',',',',',',',',',',',',',',',',',','^','^','.','.','.','.','.','.','.','.','.','.','.','.'},
	{',',',',',',',',',',',',',',',',',',',',',',',','^','^','.','.','.','.','.','.','.','.','.','.','.','.'},
	{',',',',',',',',',',',',',',',',',',',',',',',','^','^','.','.','.','.','.','.','.','.','.','.','.','.'},
	{',',',',',',',',',',',',',',',',',',',',',',',','^','^','.','.','.','.','.','.','.','.','.','.','.','.'},
	{',',',',',',',',',',',',',',',',',',',',',',',','^','^','.','.','.','.','.','.','.','.','.','.','.','.'},
	{',',',',',',',',',',',',',',',',',',',',',',',','^','^','.','.','.','.','.','.','.','.','.','.','.','.'},
	{',',',',',',',',',',',',',',',',',',',',',',',','^','^','.','.','.','.','.','.','.','.','.','.','.','.'},
	{',',',',',',',',',',',',',',',',',',',',',',',','^','^','.','.','.','.','.','.','.','.','.','.','.','.'},
	{',',',',',',',',',',',',',',',',',',',',',',',','^','^','.','.','.','.','.','.','.','.','.','.','.','.'},
	{',',',',',',',',',',',',',',',',',',',',',',',','^','^','.','.','.','.','.','.','.','.','.','.','.','.'},
	{',',',',',',',',',',',',',',',',',',',',',',',','^','^','.','.','.','.','.','.','.','.','.','.','.','.'} 
};

// create copy of our map so we can edit it and still know original tiles.
char drawMap[WIDTH][HEIGHT];

bool isInDesert(int x, int y) {
	if (map[x][y] == ',') {
		return true;
	}

	return false;
}

bool isInFloor(int x, int y) {
	if (x > 12 && y > 13) {
		return true;
	}

	if (x < 12 && y < 12) {
		return true;
	}

	return false;
}

bool inSameArea(int x, int y) {
	if (goalX < 12 && x < 12 && goalY < 12 && y < 12) {
		return true;
	}

	if (goalX > 12 && x > 12 && goalY > 13 && y > 13) {
		return true;
	}

	return false;
}

int distance(int x, int y, int x1, int y1) {
	return abs(x - x1) + abs(y - y1);
}

bool isInWater(int x, int y) {
	if (map[x][y] == '~') {
		return true;
	}
	return false;
}

bool isInMtn(int x, int y) {
	if (map[x][y] == '^') {
		return true;
	}

	return false;
}

bool isInSwamp(int x, int y) {
	if (map[x][y] == '*') {
		return true;
	}

	return false;
}

bool isOnTele(int x, int y) {
	if (map[x][y] == 'T') {
		return true;

	}

	return false;
}

void makeMTNWaypoint(int x, int y, int &x2, int &y2, int &x3, int &y3) {
	y3 = (goalY < 12) ? 11 : 14;

	if (isInMtn(x, y)) {
		x2 = x;
		y2 = y;
		x3 = x;
		return;
	}
	// just get out of desert as fast as possible
	if (isInDesert(x, y)) {
		x2 = x;
		x3 = x;
		y2 = (y < 12) ? 11 : 14;
		return;
	}
	// on floor so get as close to the coin as possible since other side is desert.
	x2 = goalX;
	x3 = x2;
	y2 = (y < 12) ? 11 : 14;
}

void makeWaterWaypoint(int x, int y, int &x2, int &y2) {
	if (isInWater(x, y)) {
		x2 = x;
		y2 = y;
		return;
	}

	int distanceToTopWater = 0;
	int distanceToBottomWater = 0;

	if (y < 12) {
		distanceToTopWater = (x < 12) ? distance(x, y, 11, 2) : distance(x, y, 13, 2);
		distanceToBottomWater = (x < 12) ? distance(x, y, 9, 0) : distance(x,y,15,0);
	}
	else {
		distanceToTopWater = (x < 12) ? distance(x, y, 11, 23) : distance(x, y, 13, 23);
		distanceToBottomWater = (x < 12) ? distance(x, y, 9, 25) : distance(x, y, 15, 25);
	}

	if (distanceToTopWater < distanceToBottomWater) {
		// make the waypoint at the top thing.
		x2 = (x < 12) ? 11 : 13;
		y2 = (y < 12) ? 2 : 23;
	}
	else {
		x2 = (x < 12) ? 9 : 15;
		y2 = (y < 12) ? 0 : 25;
	}
}

void makeSwampWaypoint(int x, int y, int&x2, int &y2) {
	if (isInSwamp(x, y)) {
		x2 = x;
		y2 = y;
		return;
	}

	if (isInDesert(x, y)) {
		// get out as fast as possible
		x2 = (x < 12) ? 11 : 13;
		y2 = y;
		return;
	}

	//on floor so get as close to coin as possible
	y2 = goalY;
	x2 = (x < 12) ? 11 : 13;


}

int GetTileType(int x, int y) {
	char t = map[x][y];
	switch (t) {
	case '.':
		return FLOORCOST;
		break;

	case 'T':
		return FLOORCOST;
		break;

	case '^':
		return  MTNCOST;
		break;

	case '*':
		return SWAMPCOST;
		break;

	case '~':
		return WATERCOST;
		break;
	case ',':
		return DESERTCOST;
		break;
	default:
		return FLOORCOST;
		break;
	}
}

double heuristic(int x, int y) {
	int h = 0;
	if (x == 19 && y == 11) {
		h = 0;
	}
	// check if mountain matters.
	if ((y <= 13 && goalY > 13) || (y >= 12 && goalY < 12)){
	//if ((y <= 12 && goalY > 13) || (y >= 13 && goalY < 12)) {

		int distanceToClosestTele = 0;
		int distanceFromExitTele = 0;
		int distanceAcrossMountain = 0;

		distanceFromExitTele = (y < 12) ? abs(goalY - t2y) + abs(goalX - t2x) : abs(goalY - t1y) + abs(goalX - t1x);

		if (isOnTele(x, y)) {
			// on the teleporter, just return distance out of teleporter.
			h += 2 * WATERCOST;
			h += (distanceFromExitTele - 2) * GetTileType(goalX, goalY);
			return h;
		}

		

		distanceToClosestTele = (y < 12) ? abs(t1y - y) + abs(t1x - x) : abs(t2y - y) + abs(t2x - x);
		distanceFromExitTele = (y < 12) ? abs(goalY - t2y) + abs(goalX - t2x) : abs(goalY - t1y) + abs(goalX - t1x);

		int closestTeleX = 12;
		int closestTeleY = (y < 12) ? 0 : 25;

		int tCost = 0;
		int WaterX;
		int WaterY;
		makeWaterWaypoint(x, y, WaterX, WaterY);
		tCost += distance(x, y, WaterX, WaterY)*GetTileType(x, y);
		if (x == WaterX && y == WaterY) {
			// this happens when we are standing in the water already
			tCost += (distance(WaterX,WaterY, closestTeleX, closestTeleY) - 1) *WATERCOST;
		}
		else {
			tCost += 2 * WATERCOST;
		}

		// always have two water waiting on the other end, so always add it.
		tCost += 2 * WATERCOST;

		tCost += (distanceFromExitTele - 2)*GetTileType(goalX, goalY);

		// done calculating cost through teleporter. Now do the cost through the mountain to compare.

		int mCost = 0;
		int mtnX;
		int mtnY;
		int mtnX2;
		int mtnY2;
		makeMTNWaypoint(x, y, mtnX, mtnY, mtnX2, mtnY2);
		mCost += (distance(x, y, mtnX, mtnY))*GetTileType(x, y);
		mCost += (distance(mtnX, mtnY, mtnX2, mtnY2) - 1) * MTNCOST;
		mCost += (distance(mtnX2, mtnY2, goalX, goalY)) * GetTileType(goalX, goalY);

		// compare costs to see which to add to heuristic
		if (mCost < tCost) {
			//cout << "X: " << x << " Y: " << y << " MTN: " << mCost << endl;
			h += mCost;
		}
		else {
			//cout << "X: " << x << " Y: " << y << " Tele: " << tCost << endl;
			h += tCost;
		}
		//think i can just return h here.
		return h;
	}
	// dont have to go through mountains, but need to figure out if we need to go through the swamp.
	if ((x < 12 && goalX > 12) || (x > 12 && goalX < 12)) {
		
		// we didn't go through the teleporter since we are on opposite sides horizontally and the above mtn stuff didn't happen.
		// so we will move through the swamp at some point.

		int swampX, swampY;
		makeSwampWaypoint(x, y, swampX, swampY);
		h += distance(x, y, swampX, swampY)*GetTileType(x, y);
		h += SWAMPCOST;
		h += (distance(swampX, swampY, goalX, goalY) - 1) * GetTileType(goalX, goalY);

		//cout << "X: " << x << " Y: " << y << " Swamp: " << h << endl;

		return h;
	}

	if (isOnTele(x, y)) {
		// on exit teleporter
		int teleWPX = 0;
		int teleWPY = y;
		if (goalX < 12) {
			teleWPX = 9;
		}
		else {
			teleWPX = 15;
		}

		h += (distance(x, y, teleWPX, teleWPY)-1)*WATERCOST;

	}

	// must have taken the teleporter, should just be able to return the distance to the coin;
	h += distance(x, y, goalX, goalY) * GetTileType(goalX, goalY);
	//cout << "X: " << x << " Y: " << y << " nuthin: " << h << endl;
	return h;
}

// first attempt
/*
double heuristic(int x, int y) {
	cout << x << " " << y;
	// heuristic here
	int manhattan = abs(goalX - x) + abs(goalY - y);
	//return manhattan;

	int h = 0;
	bool takeTeleporter = false;
	bool throughMtns = false;
	int distanceToClosestTele = 0;
	int distanceFromExitTele = 0;

	// handle mtns
	//if (goalY == 12 || goalY == 13) {
		// goal is in mountains.
//	}
	if ((y < 12 && goalY >= 12) || (y > 13 && goalY <= 13)) {
		
		// goal is accross the mountains from us. So we need to account for that by checking teleporter and by adding in for the mountaints or water.
		if (x != 12 && (y != t1y || y != t2y)) {
			distanceToClosestTele = (y < 12) ? abs(t1y - y) + abs(t1x - x) : abs(t2y - y) + abs(t2x - x);
			distanceFromExitTele = (y < 12) ? abs(goalY - t2y) + abs(goalX - t2x) : abs(goalY - t1y) + abs(goalX - t1x);
		}
		else {
			distanceFromExitTele = (y == t1y) ? abs(goalY - t2y) + abs(goalX - t2x) : abs(goalY - t1y) + abs(goalX - t1x);

		}
		if (manhattan < (distanceFromExitTele + distanceToClosestTele)) {
			// just go through the mountains.
			h += 2 * MTNCOST;
			throughMtns = true;
		}
		else {
			// go through the teleporter
			h += 2 * WATERCOST;
			//h += distanceToClosestTele + distanceFromExitTele;
			takeTeleporter = true;
		}

	}

	// handle swamp
	if (takeTeleporter == false && ((x < 12 && goalX > 12) || (x > 12 && goalX < 12))) {
		// gonna have to cross the swamp, so add one swamp tile cost.
		h += SWAMPCOST;
	}

	// handle desert
	if (isInDesert(goalX, goalY)) {
		cout << "GOAL IN DESERT ";
		// gonna have to go into the desert at some point.
		if (isInDesert(x, y) && ((goalX < 12) == (x < 12))) {
			//in same area so jsut take the manhattan * desertcost
			h += manhattan * DESERTCOST;
		}
		else {
			// not in the same area so do some calculations to get an estimate.
			if (takeTeleporter) {
				if (isInDesert(x,y)) {
					// we are in desert so add the cost to the teleporter * desert
					h += (distanceToClosestTele + distanceFromExitTele - 4) * DESERTCOST;
				}
				else {
					// just the exit end is desert so just do that part.
					h += (distanceFromExitTele - 2)*DESERTCOST + distanceToClosestTele;
				}
			}
			else {
				// not taking teleporter.
				if (isInDesert(x,y)) {
					// in desert already
					if (throughMtns) {
						// going through mtns so get the closest tile to mtns distance.
						int tmpDistance = (y < 12) ? abs(y - 11) : abs(y - 14);
						int tmpDistanceOut = (goalY < 12) ? abs(goalY - 11) : abs(goalY - 14);
						h += tmpDistance*DESERTCOST + tmpDistanceOut*DESERTCOST;
					}
					else {
						// must be going through swamp, so get the distance to that.
						int tmpDistance = abs(x - 12) + abs(goalX - 12) + abs(goalY-y);
						h += tmpDistance * DESERTCOST;
					}
					
				}
				else {
					// not in desert already, but goal is so just do the out stuff.
					if (throughMtns) {
						int tmpDistanceOut = (goalY < 12) ? abs(goalY - 11) : abs(goalY - 14);
						h += tmpDistanceOut * DESERTCOST;
					}
					else {
						//goin through swamp
						int tmpDistance = abs(goalX - 12);
						h += tmpDistance;
					}
				}
			}
		}
	}
	else if (isInDesert(x,y)) {
		// we are in desert, but the goal isn't.
		if (takeTeleporter) {
			// taking teleporter so add distance to tele
			h += distanceToClosestTele * DESERTCOST;
		}
		else if (throughMtns) {
			int tmpD = (y < 12) ? abs(y - 11) : abs(y - 14);
			h += tmpD * DESERTCOST;
		}
		else {
			//gonna go through swamp
			int tmpD = abs(x - 12);
			h += tmpD;
		}
	}

	//handle floor
	if (isInFloor(goalX, goalY)) {
		// goal is on floor tile.
		if (isInFloor(x, y) && ((goalX < 12) == (x < 12))) {
			//in same area so just add manhattan
			h += manhattan;
		}
		else if(isInFloor(x,y)) {
			//goal is in floor that isn't our floor
			if (takeTeleporter) {
				h += distanceFromExitTele + distanceToClosestTele;
			}

			if (throughMtns) {
				h += (y < 12) ? abs(x - goalX) + abs(y - 11) + abs(goalY - 14) : abs(x - goalX) + abs(y - 14) + abs(goalY - 11);
			}
		}
		
	}
	else if (isInFloor(x, y)) {
		// we are in floor, but goal isn't.
		if (takeTeleporter) {
			//h += distanceToClosestTele;
		} 
		if (throughMtns) {
			h += manhattan;
		}
	}

	cout << " " << h << endl;
	return h;

}
*/
// tile class methods
Tile::Tile(){}

double Tile::getCost(){
	switch (tileType) {
	case '.':
		return FLOORCOST;
		break;

	case 'T':
		return FLOORCOST;
		break;

	case '^':
		return  MTNCOST;
		break;

	case '*':
		return SWAMPCOST;
		break;

	case '~':
		return WATERCOST;
		break;
	case ',':
		return DESERTCOST;
		break;
	case 'C':
		return 10000;
		break;
	}
}

// creates the starting graph used by A*
void makeGraph(Tile graph[WIDTH][HEIGHT]) {
	for (int j = 0; j < WIDTH; j++) {
		for (int i = 0; i < HEIGHT; i++) {

				Tile t;
				graph[i][j] = t;
				graph[i][j].tileType = map[i][j];
				graph[i][j].treeState = 0;
				graph[i][j].x = i;
				graph[i][j].y = j;
				graph[i][j].parentX = -1; // no parents at start
				graph[i][j].parentY = -1;
				graph[i][j].inPath = 0; // not in path at start.
\
		}
	}
}
// returns tile in fringe with lowest f = g+h
Tile getBest(Tile graph[WIDTH][HEIGHT]) {
	double bestH = 99999;
	Tile bestTile;
	bestTile.x = -1; // in case nothing was in fringe (fail)
	int bestX = 0;
	int bestY = 0;
	int oldH = bestH;

	for (int i = 0; i < WIDTH; i++) {
		for (int j = 0; j < HEIGHT; j++) {
			if (graph[i][j].treeState == 1) {
				// fringe node
				double h = graph[i][j].gDist + heuristic(i, j);
				//if (i == 20 && j == 24) {
					//cout << "COST IM LOOKIN FOR: GDIST: " << graph[i][j].gDist << " H: " << h << endl;
				//}
				//double h = heuristic(i, j);
				if (h < bestH) {
					oldH = bestH;
					bestH = h;
					bestTile = graph[i][j];
					bestX = i;
					bestY = j;
					
				}
			}
		}
	}

	//cout << "CHOSE NEW BEST TILE: X: " << bestX << " Y: " << bestY << endl;
	//cout << "OldH: " << oldH << " NewH: " << bestH << endl << endl;

	return bestTile;
}

// checks adjacent tiles and updates their gDist variable to the shortest possible path currently.
void checkAdjacent(Tile graph[WIDTH][HEIGHT], int parentX, int parentY, int childX, int childY) {
	// already in tree check
	if (graph[childX][childY].treeState == 2) {
		return;
	}

	//if unexplored, add to fring with path from parent and distance from start to parent and cost of entering this node
	if (graph[childX][childY].treeState == 0) {
		graph[childX][childY].treeState = 1;
		graph[childX][childY].gDist = graph[parentX][parentY].gDist + graph[childX][childY].getCost();
		graph[childX][childY].parentX = parentX;
		graph[childX][childY].parentY = parentY;

		return;
	}

	// if fringe, reevaluate its new path
	if (graph[childX][childY].treeState == 1) {
		if (graph[parentX][parentY].gDist + graph[childX][childY].getCost() < graph[childX][childY].gDist) {
			// shorter path through parent, change path and cost.
			graph[childX][childY].gDist = graph[parentX][parentY].gDist + graph[childX][childY].getCost();
			graph[childX][childY].parentX = parentX;
			graph[childX][childY].parentY = parentY;
		}

		return;
	}
}

// currently returns path cost, want to change to maybe a queue to give to the Enemy so he can just make a move?
double finalPath(Tile graph[WIDTH][HEIGHT], Enemy * e) {
	double cost = 0;

	// start at goal (which will be either the player or next to the coin)
	int x = goalX;
	int y = goalY;

	e->clearMoveVectors();
	// loop until start (Enemy location) reached
	while (x != startX || y != startY) {
		// add node to path, add to cost
		graph[x][y].inPath = 1;
		cost += graph[x][y].getCost();

		// send this move to the enemy so he knows his path.
		
		e->addMoveToVector(x, y);

		// work backward to parent
		int tempX = graph[x][y].parentX;
		int tempy = graph[x][y].parentY;
		x = tempX;
		y = tempy;
	}
	graph[startX][startY].inPath = 1;
	return cost;

}

// used for debugging
void printGraph(Tile graph[WIDTH][HEIGHT]) {
	for (int j = 0; j < WIDTH; j++) {
		for (int i = 0; i < HEIGHT; i++) {
			cout << graph[i][j].tileType;
			cout << graph[i][j].treeState;
			if (graph[i][j].inPath) {
				cout << "*";
			}
			else {
				cout << " ";
			}
			cout << " ";
		}
		cout << "\n";
	}
}

// A* method. returns whether search was successful.
int search(Tile graph[WIDTH][HEIGHT]) {
	int goalFound = 0;

	// Add start state to tree, path cost = 0
	graph[startX][startY].treeState = 1;
	graph[startX][startY].gDist = 0;

	// Loop until goal added to tree
	while (!goalFound) {

		// Get the best tile in the fringe
		Tile bestTile = getBest(graph);
		if (bestTile.x == -1) {
			// The default tile was returned, which means the fringe was empty.
			// This means the search has failed.
			cout << "Search failed!!!!!!\n";
			printGraph(graph);
			return 0;
		}

		// Otherwise, add that best tile to the tree (removing it from fringe)
		int x = bestTile.x;
		int y = bestTile.y;
		graph[x][y].treeState = 2;


		// If it is a goal, done!
		if (x == goalX && y == goalY) {
			goalFound = 1;
			//cout << "Found the goal!!!!!\n";

			// Compute the path taken and its cost, printing the explored graph,
			// the path  cost, and the number of tiles explored (which should be
			// as small as possible!)
			//double cost = finalPath(graph);
			//printGraph(graph);

			return 1;
		}

		// Otherwise, we look at the 4 adjacent tiles to the one just added
		// to the tree (making sure each  is in the graph!) and either add it
		// to the tree or recheck its path.

		// Special cases for teleport pads
		if (x == t1x && y == t1y) {
			checkAdjacent(graph, x, y, t2x, t2y);
			//cout << "Teleport checked\n";
		}
		if (x == t2x && y == t2y) {
			checkAdjacent(graph, x, y, t1x, t1y);
			// << "Teleport checked\n";
		}

		if (x > 0) { // Tile to left
			checkAdjacent(graph, x, y, x - 1, y);
		}
		if (x < WIDTH - 1) { // Tile to right
			checkAdjacent(graph, x, y, x + 1, y);
		}
		if (y > 0) { // Tile above
			checkAdjacent(graph, x, y, x, y - 1);
		}
		if (y < HEIGHT - 1) { // Tile below
			checkAdjacent(graph, x, y, x, y + 1);
		}

	}

	return 1;
}

Player * spawnPlayer(){
	
	return new Player(0, 0);

}

Enemy * spawnEnemy() {
	startX = 25;
	startY = 25;

	return new Enemy(startX, startY);
}

void RespawnEnemy(Enemy * e) {

	int newX = rand() % 25 + 1;
	int newY = rand() % 25 + 1;

	do {
		newX = rand() % 25 + 1;
		newY = rand() % 25 + 1;

	} while (newX == coinX && newY == coinY);

	e->Respawn(newX, newY);
}

// this only gets called when a new coin is spawned (old is picked up) so just alter a spot to be a C
void SpawnCoin() {

	// change old coin back to terrain
	drawMap[coinX][coinY] = map[coinY][coinX];


	// randomly create spawn location of next coin.

	// for now im just setting one.
	//coinX = 0;
	//coinY = 12;

	//coinX = 13;
	//coinY = 4;

	int leftOrRight = rand() % 2 + 1;
	int botOrTop = rand() % 2 + 1;

	if (leftOrRight == 1) {
		// left side spawn
		coinX = rand() % 8 + 1;
	}
	else {
		coinX = rand() % 9 + 16;
	}

	if (botOrTop == 1) {
		// bottom spawn
		coinY = rand() % 10 + 15;

	}
	else {
		coinY = rand() % 10 + 1;
	}
	//coinX = 3;
	//coinY = 6;
	
	
	//THIS TEST CASE DOESNT WORK AS EXPECTED
	//coinX = 20;
	//coinY = 6;

	//coinX = 8;
	//coinY = 17;

	//coinY = 10;
	//coinX = 7;

	//coinX = 17;
	//coinY = 4;

	//coinX = 4;
	//coinY = 19;

	drawMap[coinY][coinX] = 'C';
}

void printMap() {
	
	for (int j = 0; j < WIDTH; j++) {
		for (int i = 0; i < HEIGHT; i++) {
			cout << drawMap[j][i] << ' ';
		}

		cout << '\n';
	}
}

void calculateGoal(int playerX, int playerY, int eX, int eY) {
	// simple check to see if the manhattan for the player is closer than the manhattan for the coin.
	int manPlayer = abs(playerX - eX) + abs(playerY - eY);
	int manCoin = abs(coinX - eX) + abs(coinY - eY);

	if (manPlayer < manCoin) {

		

		goalX = playerX;
		goalY = playerY;
	}
	else {

		// check to see which position next to coin we want.
		if (playerX < coinX) {
			//want left side
			goalX = coinX - 1;
		}
		else if (playerX > coinX) {
			goalX = coinX + 1;
		}
		else {
			goalX = coinX;
		}

		if (playerY < coinY) {
			goalY = coinY - 1;
		}
		else if (playerY > coinY) {
			goalY = coinY + 1;
		}
		else {
			goalY = coinY;
		}
	
	}
}

void UpdateMap(int oldX, int oldY, Pawn * c) {
	drawMap[oldY][oldX] = map[oldX][oldY];
	drawMap[c->getPosY()][c->getPosX()] = c->getSymbol();
	//update coin location in case enemy walked over it and a new one wasn't spawned.
	drawMap[coinY][coinX] = 'C';
	cout << flush;
	//system("CLS");
	system("clear");
	printMap();
}

bool inCombat(int pX, int pY, int eX, int eY) {
	int manhattan = abs(eX - pX) + abs(eY - pY);

	if (manhattan < 2) {
		return true;
	}
	
	if (pX == t1x && pY == t1y && eX == t2x && eY == t2y) {
		//cout << "TELEPORTER KOMBATTTTTT!!!" << endl;
		return true;
	}

	if (pX == t2x && pX == t2y && eX == t1x && eY == t1y) {
		//cout << "TELEPORTER KOMBATTTTTT!!!" << endl;;
		return true;
	}

	return false;
}

void setCooldown(Pawn * p) {
	switch (map[p->getPosX()][p->getPosY()])
	{
	case '.':
		p->setMoveCooldown(FLOORCOST);
		break;
	case ',':
		p->setMoveCooldown(DESERTCOST);
		break;
	case '*':
		p->setMoveCooldown(SWAMPCOST);
		break;
	case '^':
		p->setMoveCooldown(MTNCOST);
		break;
	case '~':
		p->setMoveCooldown(WATERCOST);
		break;
	}
}

int main()
{
	//initialize randomness
	srand(time(NULL));

	Tile graph[WIDTH][HEIGHT];

	// create copy of our map so we can edit it and still know original tiles.

	for (int j = 0; j < WIDTH; j++) {
		for (int i = 0; i < HEIGHT; i++) {
			drawMap[j][i] = map[i][j];
		}
	}

	Player * p = spawnPlayer();

	// spawns enemy and sets appropriate start variables for the a*
	Enemy * e = spawnEnemy();

	drawMap[e->getPosY()][e->getPosX()] = 'E';

	SpawnCoin();

	calculateGoal(p->getPosX(), p->getPosY(), e->getPosX(), e->getPosY());

	makeGraph(graph);
	search(graph);
	finalPath(graph, e);
	
	drawMap[p->getPosY()][p->getPosX()] = 'P';
	printMap();
	//printGraph(graph);

	bool gameOver = false;

	int currentTurn = 0;

	bool win = false;
	
	while (!gameOver) {

		//cache locations at beginning of frame.
		int pX = p->getPosX();
		int pY = p->getPosY();
		int eX = e->getPosX();
		int eY = e->getPosY();

		// check if we should be in combat.
		if (inCombat(pX, pY, eX, eY)) {
			// we don't care whose turn it is, so we set it to the player in case they win.
			currentTurn = 0;

			cout << "\nPlayer HP: " << p->getHealth() << " Enemy HP: " << e->getHealth() << endl;

			char pChoice = p->chooseCombat();
			char eChoice = e->chooseCombat(pChoice);

			if (pChoice == 'a') {
				// attack
				cout << "The player attacked\n";
				if (eChoice == 'g') {
					cout << "The enemy guarded and counterattacked\n";
					p->TakeDamage();
				}
				else if (eChoice == 't') {
					cout << "The enemy attempted to throw, but was hit.\n";
					e->TakeDamage();
				}
				else {
					cout << "The enemy also attacked, you both take damage\n";
					e->TakeDamage();
					p->TakeDamage();
				}
			}
			else if (pChoice == 'g') {
				// guard
				cout << "The player guarded\n";
				if (eChoice == 'a') {
					cout << "The enemy attacked and you countered.\n";
					e->TakeDamage();
				}
				else if (eChoice == 't') {
					cout << "The enemy threw you\n";
					p->TakeDamage();
				}
				else {
					cout << "You both guarded and took no damage.\n";
				}
			}
			else if (pChoice == 't') {
				// throw
				cout << "The player attempted to throw the enemy\n";
				if (eChoice == 'g') {
					cout << "You grabbed the enemy while he was guarding\n";
					e->TakeDamage();
				}
				else if (eChoice == 'a') {
					cout << "The enemy hit you while you attempted to throw him.\n";
					p->TakeDamage();
				}
				else {
					cout << "You both threw each other and took damage (somehow?)\n";
					e->TakeDamage();
					p->TakeDamage();
				}
			}

			/*while (e->getHealth() != 0) {
				e->TakeDamage();
			}*/

			if (p->getHealth() <= 0) {
				cout << "You have died\n";
				gameOver = true;
			}

			if (e->getHealth() <= 0) {
				// kill enemy
				RespawnEnemy(e);
				UpdateMap(eX, eY, e);
				startX = e->getPosX();
				startY = e->getPosY();
				makeGraph(graph);
				search(graph);
				finalPath(graph, e);
			}
		}
		else {
			// not in combat 

			// check current turn: 0 = player, 1 = AI
			if (currentTurn == 0) {
				// player stuff

				if (p->getMoveCooldown() <= 0) {
					cout << "\nInput W,A,S,D to move in that direction: ";
					char in;
					cin >> in;
					p->setPlayerNextMove(in);
					p->Move();
					
					if (p->getPosX() == coinX && p->getPosY() == coinY) {
						p->collectCoin();
						if (p->getCoinsCollected() == 3) {
							//cout << "You win!\n";
							win = true;
							gameOver = true;
							
						}
						SpawnCoin();
					}

					setCooldown(p);
					
				}
				else {
					p->Move();
					cout << "The player cannot move for another " << p->getMoveCooldown() << " turns\n";
				}
				currentTurn = 1;
				UpdateMap(pX, pY, p);
				
			}
			else if(currentTurn == 1) {
				// AI STUFF

				// check if we should redo our A*
				int tmpGoalX = goalX;
				int tmpGoalY = goalY;

				calculateGoal(pX, pY, eX, eY);

				if (tmpGoalX != goalX || tmpGoalY != goalY) {
					// goal has changed so we need to redo A*.
					startX = e->getPosX();
					startY = e->getPosY();
					makeGraph(graph);

					search(graph);
					finalPath(graph, e);
				}

				if (e->getMoveCooldown() <= 0) {
					e->Move();
					setCooldown(e);
					UpdateMap(eX, eY, e);

				}
				else {
					e->Move();
					cout << "The enemy cannot move for another " << e->getMoveCooldown() << " turns\n";
				}
				currentTurn = 0;
				

			}
			else {
				cout << "\nSOMETHING WENT HORRIBLY WRONG\n";
			}

			
		}
	}
	cout << (win == true ? "You win!\n" : "You lost!\n");
	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
