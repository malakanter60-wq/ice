#include "Toolbar.h"
#include "../Config/GameConfig.h"
#include "../Core/Game.h"
#include <fstream>
#include "../Entities/Animal.h"

ToolbarIcon::ToolbarIcon(Game* r_pGame, point r_point, int r_width, int r_height, string img_path) : Drawable(r_pGame, r_point, r_width, r_height)
{
	image_path = img_path;
}

void ToolbarIcon::draw() const
{
	//draw image of this object
	window* pWind = pGame->getWind();
	pWind->DrawImage(image_path, RefPoint.x, RefPoint.y, width, height);
}

RestartIcon::RestartIcon(Game* r_pGame, point r_point, int r_width, int r_height, string img_path) : ToolbarIcon(r_pGame, r_point, r_width, r_height, img_path)
{}

void RestartIcon::onClick()
{
	// Reset all animals
	for (int i = 0; i < pGame->chickCount; i++) {
		delete pGame->chickList[i];
		pGame->chickList[i] = nullptr;
	}
	pGame->chickCount = 0;

	for (int i = 0; i < pGame->cowCount; i++) {
		delete pGame->cowList[i];
		pGame->cowList[i] = nullptr;
	}
	pGame->cowCount = 0;

	for (int i = 0; i < pGame->wolfCount; i++) {
		delete pGame->wolfList[i];
		pGame->wolfList[i] = nullptr;
	}
	pGame->wolfCount = 0;

	// Reset items
	for (int i = 0; i < pGame->ItemCount; i++) {
		delete pGame->ItemList[i];
		pGame->ItemList[i] = nullptr;
	}
	pGame->ItemCount = 0;

	// Reset budget and timer
	pGame->budget = 30000;
	pGame->timer = 0;
	pGame->foodAreas.clear();

	// Redraw clean field
	pGame->getWind()->SetPen(ORANGE, 2);
	pGame->getWind()->SetBrush(ORANGE);
	pGame->getWind()->DrawRectangle(0, 2 * config.toolBarHeight,
		config.windWidth,
		config.windHeight - config.statusBarHeight);
	pGame->drawWarehouse();
	pGame->printBudget("BUDGET = $" + to_string(pGame->budget));
	pGame->printMessage("Game Restarted!");
}

ExitIcon::ExitIcon(Game* r_pGame, point r_point, int r_width, int r_height, string img_path) : ToolbarIcon(r_pGame, r_point, r_width, r_height, img_path)
{}

void ExitIcon::onClick()
{
	// Properly exits the game
	exit(0);
}
PauseIcon::PauseIcon(Game* r_pGame, point r_point, int r_width, int r_height, string img_path) : ToolbarIcon(r_pGame, r_point, r_width, r_height, img_path)
{
}
void PauseIcon::onClick()
{
	pGame->isPaused = true;
	pGame->printMessage("Game Paused - Click Resume to continue");

	// Wait until resume is clicked
	window* pWind = pGame->getWind();
	int x, y;
	while (pGame->isPaused)
	{
		if (pWind->GetButtonState(LEFT_BUTTON, x, y))
		{
			// Check if resume icon area is clicked
			if (y >= 0 && y < config.toolBarHeight)
			{
				int clickedIndex = x / config.iconWidth;
				if (clickedIndex == ICON_RESUME)
				{
					pGame->isPaused = false;
					pGame->printMessage("Game Resumed!");
				}
			}
		}
		Sleep(50);
	}
}


ResumeIcon::ResumeIcon(Game* r_pGame, point r_point, int r_width, int r_height, string img_path) : ToolbarIcon(r_pGame, r_point, r_width, r_height, img_path)
{
}
void ResumeIcon::onClick()
{
	pGame->isPaused = false;
	pGame->printMessage("Game Resumed!");
}
SaveIcon::SaveIcon(Game* r_pGame, point r_point, int r_width, int r_height, string img_path)
	: ToolbarIcon(r_pGame, r_point, r_width, r_height, img_path)
{
}

void SaveIcon::onClick()
{
	// Save to a text file
	ofstream file("savegame.txt");
	if (!file) {
		pGame->printMessage("Save Failed!");
		return;
	}

	file << pGame->budget << "\n";
	file << pGame->timer << "\n";
	file << pGame->chickCount << "\n";
	file << pGame->cowCount << "\n";

	// Save chick positions
	for (int i = 0; i < pGame->chickCount; i++) {
		if (pGame->chickList[i]) {
			file << pGame->chickList[i]->getPos().x << " "
				<< pGame->chickList[i]->getPos().y << "\n";
		}
	}

	// Save cow positions
	for (int i = 0; i < pGame->cowCount; i++) {
		if (pGame->cowList[i]) {
			file << pGame->cowList[i]->getPos().x << " "
				<< pGame->cowList[i]->getPos().y << "\n";
		}
	}

	file.close();
	pGame->printMessage("Game Saved!");
}

LoadIcon1::LoadIcon1(Game* r_pGame, point r_point, int r_width, int r_height, string img_path)
	: ToolbarIcon(r_pGame, r_point, r_width, r_height, img_path)
{
}

void LoadIcon1::onClick()
{
	ifstream file("savegame.txt");
	if (!file) {
		pGame->printMessage("No Save File Found!");
		return;
	}

	// Clear existing animals first
	for (int i = 0; i < pGame->chickCount; i++) {
		delete pGame->chickList[i];
		pGame->chickList[i] = nullptr;
	}
	pGame->chickCount = 0;

	for (int i = 0; i < pGame->cowCount; i++) {
		delete pGame->cowList[i];
		pGame->cowList[i] = nullptr;
	}
	pGame->cowCount = 0;

	// Load saved data
	int savedChickCount, savedCowCount;
	file >> pGame->budget;
	file >> pGame->timer;
	file >> savedChickCount;
	file >> savedCowCount;

	// Restore chicks
	for (int i = 0; i < savedChickCount; i++) {
		point p;
		file >> p.x >> p.y;
		Chick* c = new Chick(pGame, p, 50, 50, "images\\chick.jpg");
		pGame->chickList[pGame->chickCount++] = c;
		c->draw();
	}

	// Restore cows
	for (int i = 0; i < savedCowCount; i++) {
		point p;
		file >> p.x >> p.y;
		Cow* c = new Cow(pGame, p, 50, 50, "images\\cow.jpg");
		pGame->cowList[pGame->cowCount++] = c;
		c->draw();
	}

	file.close();
	pGame->printBudget("BUDGET = $" + to_string(pGame->budget));
	pGame->printMessage("Game Loaded!");
}
Toolbar::Toolbar(Game* r_pGame, point r_point, int r_width, int r_height) : Drawable(r_pGame, r_point, r_width, r_height)
{
	//First prepare List of images for each icon
	//To control the order of these images in the menu, reoder them in enum ICONS above	
	iconsImages[ICON_RESTART] = "images\\RESTART.jpg";
	iconsImages[ICON_EXIT] = "images\\EXIT.jpg";
	iconsImages[ICON_RESUME] = "images\\resume.jpg";
	iconsImages[ICON_SAVE] = "images\\save.jpg";
	iconsImages[ICON_LOAD] = "images\\load.jpg";
	iconsImages[ICON_PAUSE] = "images\\pause.jpg";
	point p;
	p.x = 0;
	p.y = 0;

	iconsList = new ToolbarIcon * [ICON_COUNT];

	//For each icon in the tool bar create an object 
	iconsList[ICON_RESTART] = new RestartIcon(pGame, p, config.iconWidth, config.toolBarHeight, iconsImages[ICON_RESTART]);
	p.x += config.iconWidth;
	iconsList[ICON_EXIT] = new ExitIcon(pGame, p, config.iconWidth, config.toolBarHeight, iconsImages[ICON_EXIT]);
	p.x += config.iconWidth;
	iconsList[ICON_PAUSE] = new PauseIcon(pGame, p, config.iconWidth, config.toolBarHeight, iconsImages[ICON_PAUSE]);
	p.x += config.iconWidth;
	iconsList[ICON_LOAD] = new LoadIcon1(pGame, p, config.iconWidth, config.toolBarHeight, iconsImages[ICON_LOAD]);
	p.x += config.iconWidth;
	iconsList[ICON_SAVE] = new SaveIcon(pGame, p, config.iconWidth, config.toolBarHeight, iconsImages[ICON_SAVE]);
	p.x += config.iconWidth;
	iconsList[ICON_RESUME] = new ResumeIcon(pGame, p, config.iconWidth, config.toolBarHeight, iconsImages[ICON_RESUME]);
}

Toolbar::~Toolbar()
{
	for (int i = 0; i < ICON_COUNT; i++)
		delete iconsList[i];
	delete iconsList;
}

void Toolbar::draw() const
{
	for (int i = 0; i < ICON_COUNT; i++)
		iconsList[i]->draw();
	window* pWind = pGame->getWind();
	pWind->SetPen(BLACK, 3);
	pWind->DrawLine(0, config.toolBarHeight, pWind->GetWidth(), config.toolBarHeight);
}

bool Toolbar::handleClick(int x, int y)
{
	if (x > ICON_COUNT * config.iconWidth)	//click outside toolbar boundaries
		return false;


	//Check whick icon was clicked
	//==> This assumes that menu icons are lined up horizontally <==
	//Divide x co-ord of the point clicked by the icon width (int division)
	//if division result is 0 ==> first icon is clicked, if 1 ==> 2nd icon and so on

	int clickedIconIndex = (x / config.iconWidth);
	iconsList[clickedIconIndex]->onClick();	//execute onClick action of clicled icon

	if (clickedIconIndex == ICON_EXIT) return true;

	return false;

}



