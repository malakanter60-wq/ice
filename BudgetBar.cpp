#include "Budgetbar.h"
#include "../Config/GameConfig.h"
#include "../Core/Game.h"
#include "../Entities/Animal.h"
#include <iostream>
using namespace std;
class Game;


BudgetbarIcon::BudgetbarIcon(Game* r_pGame, point r_point, int r_width, int r_height, string img_path) : Drawable(r_pGame, r_point, r_width, r_height)
{
	image_path = img_path;
}

void BudgetbarIcon::draw() const
{
	//draw image of this object
	window* pWind = pGame->getWind();
	pWind->DrawImage(image_path, RefPoint.x, RefPoint.y, width, height);
}

ChickIcon::ChickIcon(Game* r_pGame, point r_point, int r_width, int r_height, string img_path)
	: BudgetbarIcon(r_pGame, r_point, r_width, r_height, img_path)
{
	
}
void ChickIcon::onClick()
{
	if (pGame->budget < 100) {
		pGame->printBudget("Not enough money!");
		return;
	}

	pGame->budget -= 100;
	pGame->printBudget("BUDGET = $" + to_string(pGame->budget));

	point p;
	std::mt19937 gen(std::random_device{}());
	p.x = std::uniform_int_distribution<int>(range_min_x, range_max_x)(gen);
	p.y = std::uniform_int_distribution<int>(range_min_y, range_max_y)(gen);

	// FIX: ONLY chickList
	if (pGame->chickCount < 100) {
		Chick* newChick = new Chick(pGame, p, 50, 50, "images\\chick.jpg");
		pGame->chickList[pGame->chickCount] = newChick;
		pGame->chickCount++;
		newChick->draw();
	}
}

CowIcon::CowIcon(Game* r_pGame, point r_point, int r_width, int r_height, string img_path)
	: BudgetbarIcon(r_pGame, r_point, r_width, r_height, img_path)
{
}

void CowIcon::onClick()
{
	if (pGame->budget < 250) {
		pGame->printBudget("Not enough money!");
		return;
	}

	pGame->budget -= 250;
	pGame->printBudget("BUDGET = $" + to_string(pGame->budget));

	point p;
	std::mt19937 gen(std::random_device{}());
	p.x = std::uniform_int_distribution<int>(range_min_x, range_max_x)(gen);
	p.y = std::uniform_int_distribution<int>(range_min_y, range_max_y)(gen);

	// FIX: ONLY cowList, never AddAnimal or pAnimalList
	if (pGame->cowCount < 100) {
		Cow* newCow = new Cow(pGame, p, 50, 50, "images\\cow.jpg");
		pGame->cowList[pGame->cowCount] = newCow;
		pGame->cowCount++;
		newCow->draw();
	}
}
WolfIcon::WolfIcon(Game* r_pGame, point r_point, int r_width, int r_height, string img_path)
	: BudgetbarIcon(r_pGame, r_point, r_width, r_height, img_path)
{
	
}

void WolfIcon::onClick()
{
	if (pGame->budget < 370) {
		pGame->clearBudget();
		pGame->printBudget("Not enough money!");
		return;
	}

	pGame->budget -= 370;
	pGame->clearBudget();
	pGame->printBudget("BUDGET = $" + to_string(pGame->budget));

	point p;
	std::mt19937 gen(std::random_device{}());
	p.x = std::uniform_int_distribution<int>(range_min_x, range_max_x)(gen);
	p.y = std::uniform_int_distribution<int>(range_min_y, range_max_y)(gen);

	Wolf* newWolf = new Wolf(pGame, p, 50, 50, "images\\wolf.jpg");
	pGame->wolfList[pGame->wolfCount++] = newWolf;   // ADD to game list
	newWolf->draw();

	cout << "Wolf added at (" << p.x << ", " << p.y << ")" << endl;
}
WaterIcon::WaterIcon(Game* r_pGame, point r_point, int r_width, int r_height, string img_path)
	: BudgetbarIcon(r_pGame, r_point, r_width, r_height, img_path)
{

}

void WaterIcon::onClick()
{
	cout << "Water Icon Clicked" << endl;

	if (pGame->budget >= 50)
	{
		pGame->budget -= 50;
		pGame->buyWater = true;

		pGame->clearBudget();
		string budget_string = "BUDGET = $" + to_string(pGame->budget);
		pGame->printBudget(budget_string);
	}
	else
	{
		pGame->printBudget("Not enough money!");
	}
}


Budgetbar::Budgetbar(Game* r_pGame, point r_point, int r_width, int r_height) : Drawable(r_pGame, r_point, r_width, r_height)
{
	//First prepare List of images for each icon
	//To control the order of these images in the menu, reoder them in enum ICONS above	
	iconsImages[ICON_CHICK] = "images\\chick.jpg";
	iconsImages[ICON_COW] = "images\\cow.jpg";
	iconsImages[ICON_WOLF] = "images\\wolf.jpg";
	iconsImages[ICON_WATER] = "images\\water.jpg";
	

	point p;
	p.x = 0;
	p.y = config.toolBarHeight;

	iconsList = new BudgetbarIcon * [ANIMAL_COUNT];

	//For each icon in the tool bar create an object 
	iconsList[ICON_CHICK] = new ChickIcon(pGame, p, config.iconWidth, config.toolBarHeight, iconsImages[ICON_CHICK]);
	p.x += config.iconWidth;
	iconsList[ICON_COW] = new CowIcon(pGame, p, config.iconWidth, config.toolBarHeight, iconsImages[ICON_COW]);
	p.x += config.iconWidth;
	iconsList[ICON_WOLF] = new WolfIcon(pGame, p, config.iconWidth, config.toolBarHeight, iconsImages[ICON_WOLF]);
	p.x += config.iconWidth;
	iconsList[ICON_WATER] = new WaterIcon(pGame, p, config.iconWidth, config.toolBarHeight, iconsImages[ICON_WATER]);

}

Budgetbar::~Budgetbar()
{
	for (int i = 0; i < ANIMAL_COUNT; i++)
		delete iconsList[i];
	delete iconsList;
}

void Budgetbar::draw() const
{
	for (int i = 0; i < ANIMAL_COUNT; i++)
		iconsList[i]->draw();
	window* pWind = pGame->getWind();
	pWind->SetPen(BLACK, 3);
	pWind->DrawLine(0, 2*config.toolBarHeight, pWind->GetWidth(), 2*config.toolBarHeight);
}

bool Budgetbar::handleClick(int x, int y)
{
	int clickedIconIndex = (x / config.iconWidth);

	if (clickedIconIndex >= ANIMAL_COUNT)
		return false;

	iconsList[clickedIconIndex]->onClick();    //click outside toolbar boundaries
	return false;

}