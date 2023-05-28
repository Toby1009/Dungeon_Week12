/************************************************************************
File:   source.cpp
Team: Team_36
Author:
		鍾賢廣，ea5878158@gmail.com
Modifier:
		賴祐吉，cheeryuchi@gmail.com
		黃欣云，windyhuang6@gmail.com
		陳俊宇，JYCReports@gmail.com
		邱嘉興，tbcey74123@gmail.com
		楊鈞安 B11115033
Comment:
		基本輸入方向移動功能，w s a d 移動腳色上下左右，空白改變腳色站立之地板字元，到T上可以增加經驗
		ESC是離開畫面。同時更新圖版上的資訊。

************************************************************************/

#include "main.h"
#include "Hero.h"
#include "Item.h"
#include "Creature.h"
#include "Ghost.h"
#include "Wolf.h"
#include "BloodPack.h"
#include "Trigger.h"
#include <vector>
#include <fstream>
Hero	 gHero(2, 2);


// Constent value
const char GWALL = 'O';
const char GNOTHING = ' ';

int GWIDTH = -1;
int GHEIGHT = -1;
const int MIN_SIZE = 4;
const double gTimeLog = 0.033;

// Distance
const int gDistance = 4;

// 用來儲存版面資料
char** gBoard;

// 定義六種輸入指令與對應陣列index
enum ValidInput
{
	EW = 0,
	ES = 1,
	EA = 2,
	ED = 3,
	ESPACE = 4,
	EESC = 5,
	ESave = 6,
	ELoad = 7,
	INVALID,
};


//Creature gCreature;

// function declare
// 偵測輸入狀態
void keyUpdate(bool key[]);
// 初始化版面
void setupBoard(int rowN, int colN);
// 輸出版面
void draw(void);
// 輸出說明資訊
void drawInfo(void);

// 程式主驅動函式
void update(bool key[]);

void saveMap();
void loadMap();
//Item vector
std::vector<Item*> gItems;
//creature vector
std::vector<Creature*> gCreature;

int main(int argc, char** argv)
{
	srand(time(NULL));

	// 用來管理鍵盤輸入狀態的陣列
	bool gKeyState[ValidInput::INVALID];
	for (int i = 0; i < ValidInput::INVALID; i++) {
		gKeyState[i] = false;
	}

	while (true) {
		std::cout << "Enter Width: ";
		std::cin >> GWIDTH;
		std::cout << "Enter Height: ";
		std::cin >> GHEIGHT;
		system("CLS");
		if (GHEIGHT < MIN_SIZE || GWIDTH < MIN_SIZE) {
			std::cout << "Illegal, both has to be larger than " << MIN_SIZE << std::endl;
		}
		else {
			break;
		}
	}

	// Setup a clear dungeon
	setupBoard(GHEIGHT, GWIDTH);

	// Draw the bord and information
	draw();
	drawInfo();

	// Variable for game loop
	clock_t startT, endT;
	startT = clock();
	endT = clock();

	// Run the game loop
	while (!gKeyState[ValidInput::EESC]) {
		// Compute the time lap
		double timeFrame = (double)(endT - startT) / CLOCKS_PER_SEC;

		// Execute the game loop
		if (timeFrame >= gTimeLog) {
			update(gKeyState);
			startT = clock();
		}

		// Update the key
		keyUpdate(gKeyState);
		endT = clock();

		if (gKeyState[ValidInput::ESave]) {
			saveMap();
			gKeyState[ValidInput::ESave] = false;
		}
		else if (gKeyState[ValidInput::ELoad]) {
			loadMap();
			gKeyState[ValidInput::ELoad] = false;
		}
	}

	system("pause");
	return 0;
}

//******************************************************************
//
// * 偵測輸入狀態
//==================================================================
void keyUpdate(bool key[])
//==================================================================
{
	for (int i = 0; i < ValidInput::INVALID; i++) {
		key[i] = false;
	}
	char input = _getch();
	switch (input) {
	case 'w':
		key[ValidInput::EW] = true;
		break;
	case 's':
		key[ValidInput::ES] = true;
		break;
	case 'a':
		key[ValidInput::EA] = true;
		break;
	case 'd':
		key[ValidInput::ED] = true;
		break;
	case ' ':
		key[ValidInput::ESPACE] = true;
		break;
	case 27:
		key[ValidInput::EESC] = true;
		break;
	case '1':
		key[ValidInput::ESave] = true;
		break;
	case '2':
		key[ValidInput::ELoad] = true;
		break;
	default:
		break;
	}
}

//******************************************************************
//
// * 判斷位置是否為空
//==================================================================
bool isPositionValid(Position& pos)
//==================================================================
{
	// Check whether it is an empty space
	if (gBoard[pos.y][pos.x] != GNOTHING)
		return false;

	return true;
}

//******************************************************************
//
// * clip algroithm
//==================================================================
float clip(float n, float minimun, float maximum)
//==================================================================
{
	return std::max(minimun, std::min(n, maximum));
}

//******************************************************************
//
// * 利用 call-by-referce 將計算得到的方向回傳
//==================================================================
bool canSee(Position cPos, Position hPos, Position& dir)
//==================================================================
{
	// the dir_x and dir_y value are call-by-refernce
	dir.x = (int)clip((float)(hPos.x - cPos.x), -1.f, 1.f); // clip the value between -1 ~ 1
	dir.y = (int)clip((float)(hPos.y - cPos.y), -1.f, 1.f);
	int count = 0;
	do {
		// spot the target position
		if (cPos + dir * count == hPos) {
			return true;
		}
		count++;
	} while (count < gDistance); // check the range in 4 units
	return false;
}

const int HORIZONTAL = 1, VERTICAL = 2;

//Intent:choose orientation
//Pre:width height
//Pos:return origentation
int choose_orientation(int width, int height) {
	if (width < height) {
		return HORIZONTAL;
	}
	else if (height < width) {
		return VERTICAL;
	}
	else {
		return (rand() % 2 == 0) ? HORIZONTAL : VERTICAL;
	}
}
//Intent:divide the board
//Pre:board,x,y,width,heigh,orientation
//Pos:divid the board
void divide(char** grid, int x, int y, int width, int height, int orientation) {
	if (width <= 2 || height <= 2) {
		return;
	}

	bool horizontal = (orientation == HORIZONTAL);

	// where will the wall be drawn from?
	int wx = x + (horizontal ? 0 : rand() % (width - 2));
	int wy = y + (horizontal ? rand() % (height - 2) : 0);

	// where will the passage through the wall exist?
	int px = wx + (horizontal ? rand() % width : 0);
	int py = wy + (horizontal ? 0 : rand() % height);

	// what direction will the wall be drawn?
	int dx = horizontal ? 1 : 0;
	int dy = horizontal ? 0 : 1;

	// how long will the wall be?
	int length = horizontal ? width : height;

	// what direction is perpendicular to the wall?
	char dir = 'O';

	//divid and draw
	for (int i = 0; i < length; i++) {
		if ((wx != px || wy != py)) {
			grid[wy][wx] = dir;
		}
		wx += dx;
		wy += dy;
		if (wx < 0 || wx >= GWIDTH || wy < 0 || wy >= GHEIGHT)
		{
			break;
		}
	}

	int nx, ny, w, h;
	nx = x;
	ny = y;
	w = horizontal ? width : wx - x + 1;
	h = horizontal ? wy - y + 1 : height - (wy + 1) + y;
	divide(grid, nx, ny, w, h, choose_orientation(w, h));

	nx = horizontal ? x : wx + 1;
	ny = horizontal ? wy + 1 : y;
	w = horizontal ? width : x + width - (wx + 1);
	h = horizontal ? height - (wy + 1) + y : wy - y + 1;
	divide(grid, nx, ny, w, h, choose_orientation(w, h));
}
//******************************************************************
//
// * Setup the board
//==================================================================
void setupBoard(int rowN, int colN)
//==================================================================
{
	// Allocate & init game board using 2d dynamic array

	gBoard = new char* [rowN];
	for (int i = 0; i < rowN; i++) {
		gBoard[i] = new char[colN];
		for (int j = 0; j < colN; j++) {
			if (i == 0 || i == rowN - 1 || j == 0 || j == colN - 1)
				gBoard[i][j] = GWALL;
			else
				gBoard[i][j] = GNOTHING;
		}
	}

	/*Please implement your code here*/
	divide(gBoard, 0, 0, colN, rowN, choose_orientation(colN, rowN));
	/************************************************************************/

	// Setup for (random) position of all elements and implementation of game board using 2d dynamic array
	// Elements has to be randomly generated within canvas

	std::vector<std::vector<bool>> validFlags(GHEIGHT);
	for (int i = 0; i < GHEIGHT; i++) {
		validFlags[i].resize(GWIDTH);
		for (int j = 0; j < GWIDTH; j++) {
			validFlags[i][j] = gBoard[i][j] == GWALL ? false : true;
		}
	}

	auto getRandomPos = [&rowN, &colN]() {
		return Position((int)(rand() % colN), (int)(rand() % rowN));
	};

	auto getValidRandomPos = [&validFlags, &getRandomPos]() {
		while (true) {
			Position pos = getRandomPos();
			if (validFlags[pos.y][pos.x]) {
				return pos;
			}
		}
	};

	Position hPos = getValidRandomPos();
	validFlags[hPos.y][hPos.x] = false;
	gHero.setPos(hPos);
	//creatures set
	Ghost* ghost = new Ghost();
	Position gPos = getValidRandomPos();
	validFlags[gPos.y][gPos.x] = false;
	ghost->setPos(gPos);
	gCreature.push_back(ghost);
	
	Wolf* wolf = new Wolf();
	Position wPos = getValidRandomPos();
	validFlags[wPos.y][wPos.x] = false;
	wolf->setPos(wPos);
	gCreature.push_back(wolf);
	//imem set
	for (int i = 0; i < 2; i++) {
		Trigger* trigger = new Trigger();
		Position tpos = getValidRandomPos();
		validFlags[tpos.y][tpos.x] = false;
		trigger->setPos(tpos);
		gItems.push_back(trigger);
	}
	BloodPack* bloodPack = new BloodPack();
	Position bPos = getValidRandomPos();
	validFlags[bPos.y][bPos.x] = false;
	bloodPack->setPos(bPos);
	gItems.push_back(bloodPack);
}

//******************************************************************
//
// * Draw the board
//==================================================================
void draw()
//==================================================================
{
	// Add the hero into the board
	// Setup and implementation of drawing board using 2d vector

	std::vector<std::vector<char>> drawBoard(GHEIGHT);

	for (int i = 0; i < GHEIGHT; i++) {
		drawBoard[i].resize(GWIDTH);
		for (int j = 0; j < GWIDTH; j++) {
			drawBoard[i][j] = gBoard[i][j];
		}
	}

	// Draw two triggers using for loop on drawBoard

	for (int i = 0; i < gItems.size(); i++) {
		Position t = gItems[i]->getPos();
		drawBoard[t.y][t.x] = gItems[i]->getIcon();
	}
	//Draw two creatures using for loop on drawBoard
	for (const auto& c : gCreature)
	{
		Position cPos = c->getPos();
		drawBoard[cPos.y][cPos.x] = c->getIcon();
	}

	// Update the hero information
	Position h = gHero.getPos();
	drawBoard[h.y][h.x] = gHero.getIcon();

	// Draw the board
	for (int i = 0; i < GHEIGHT; i++) {
		for (int j = 0; j < GWIDTH; j++) {
			std::cout << drawBoard[i][j]; //  output
		}
		std::cout << "\n";
	}
}

//******************************************************************
//
// * Output information
//==================================================================
void drawInfo(void)
//==================================================================
{
	std::cout << "The hero is level " << gHero.getLevel() << "(" << gHero.getExp() << "/" << gHero.getMaxExp() << " to level up)" << std::endl;
	std::cout << "The hero has " << gHero.getHP() << " hp" << std::endl;
	std::cout << "Use wsad key to move Hero " << gHero.getIcon() << std::endl;
	std::cout << "Every time you step on a trigger T, the hero gets 10 exp." << std::endl;
	std::cout << "(ESC) Exit (1) Save (2) Load" << std::endl;
}

//******************************************************************
//
// * Update the board state
//==================================================================
void update(bool key[])
//==================================================================
{
	// 清除版面
	system("CLS");

	Position delta;

	// 是否有input
	bool hasInput = false;
	if (key[ValidInput::EW]) {
		delta -= Position(0, 1);
		hasInput = true;
	}
	else if (key[ValidInput::ES]) {
		delta += Position(0, 1);
		hasInput = true;
	}
	else if (key[ValidInput::EA]) {
		delta = delta - Position(1, 0);
		hasInput = true;
	}
	else if (key[ValidInput::ED]) {
		delta = delta + Position(1, 0);
		hasInput = true;
	}
	else {
		bool allInvalid = true;
		for (int i = 0; i < ValidInput::INVALID; i++) {
			if (key[i]) {
				allInvalid = false;
				break;
			}
		}
		if (allInvalid)
			std::cout << "invalid input\n";
	}
	if (hasInput) {
		gHero.move(delta);
	}

	// Manipulate update of two triggers using while loop

	for (int i = 0; i < gItems.size(); i++) {
		gItems[i]->update(gHero);
	}
	for (int i = 0; i < gCreature.size(); i++) {
		gCreature[i]->update(gHero);
	}
	draw();
	drawInfo();
}

//******************************************************************
//
// * Save the map status
//==================================================================
void saveMap() {
	// Save dungen to file
	system("CLS");
	std::cout << "Input file name for saving or Exit to leave saving." << std::endl;
	std::cout << "Input: ";
	std::string input;
	std::cin >> input;
	if (input.compare("Exit") == 0)
		return;

	std::ofstream oStream(input + ".txt");
	//save board
	oStream << GWIDTH << " " << GHEIGHT << std::endl;

	for (int i = 0; i < GHEIGHT; i++) {
		for (int j = 0; j < GWIDTH; j++) {
			oStream << gBoard[i][j];
		}
		oStream << std::endl;
	}
	oStream << std::endl;
	//save hero
	oStream << gHero.getPos() << " " << gHero.getHP() << " " << gHero.getLevel() << " " << gHero.getExp() << " " << gHero.getMaxExp() << std::endl;
	//save creature
	oStream << gCreature.size() << std::endl;
	for (int i = 0; i < gCreature.size(); i++) {
		oStream << gCreature[i]->getType()<<" ";
		oStream << gCreature[i]->getPos() << std::endl;
	}
	//save item
	oStream << gItems.size() << std::endl;
	for (int i = 0; i < gItems.size(); i++) {
		oStream << gItems[i]->getType() << " ";
		oStream << gItems[i]->getPos() << std::endl;
	}

	oStream.close();
}

//******************************************************************
//
// * Load the map status
//==================================================================
void loadMap() {
	// Load from file and reset the dungen
	system("CLS");
	std::cout << "Input file name for loading or Exit to leave loading." << std::endl;
	std::cout << "Input: ";
	std::string input;
	std::cin >> input;
	if (input.compare("Exit") == 0)
		return;

	std::ifstream iStream(input + ".txt");
	if (!iStream.is_open())
		return;

	// reset dungen
	for (int i = 0; i < GHEIGHT; i++) {
		delete[] gBoard[i];
	}
	delete[] gBoard;

	iStream >> GWIDTH >> GHEIGHT;

	char _c;
	iStream.get(_c);
	//load board
	gBoard = new char* [GHEIGHT];
	for (int i = 0; i < GHEIGHT; i++) {
		gBoard[i] = new char[GWIDTH];
		for (int j = 0; j < GWIDTH; j++) {
			iStream.get(gBoard[i][j]);
		}
		iStream.get(_c);
	}
	//load hero
	Position pos;
	int hp, level, exp, maxExp;
	iStream >> pos >> hp >> level >> exp >> maxExp;

	gHero.setPos(pos);
	gHero.setHP(hp);
	gHero.setLevel(level);
	gHero.setExp(exp);
	gHero.SetMaxExp(maxExp);
	//load creature
	int creatureN;
	iStream >> creatureN;
	for (int i = 0; i < creatureN; i++) {
		int type;
		iStream >> type;
		Creature* creature = new Creature();
		if (type == 1)
		{
			creature = new Wolf();
		}
		else if (type == 2)
		{
			creature = new Ghost();
		}
		iStream >> pos;
		creature->setPos(pos);
		gCreature.push_back(creature);
	}
	//load item
	int itemN;
	iStream >> itemN;
	for (int i = 0; i < itemN; i++) {
		int type;
		iStream >> type;
		Item* item = new Trigger();
		if (type == 0)
		{
			item = new Trigger();
		}
		else if (type == 1)
		{
			item = new BloodPack();
		}
		iStream >> pos;
		item->setPos(pos);
		gItems.push_back(item);
	}

	iStream.close();
}