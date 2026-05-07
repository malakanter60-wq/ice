#include "Game.h"
#include "../Config/GameConfig.h"
#include "../Entities/Animal.h"
#include <iostream>

#include <chrono>    
using namespace std;
using namespace chrono; 
Game::Game()
{
    pWind = CreateWind(config.windWidth, config.windHeight, config.wx, config.wy);
    if (!pWind) return;

    createToolbar();
    createBudgetbar();

    currentLevel = 1;
    initLevel();
    drawField();
    drawWarehouse();
    clearStatusBar();

    for (int i = 0; i < 100; i++) {
        chickList[i] = nullptr;
        cowList[i] = nullptr;
        wolfList[i] = nullptr;
        ItemList[i] = nullptr;
    }

    pAnimalList = new Animal * [100];
    for (int i = 0; i < 100; i++)
        pAnimalList[i] = nullptr;

    animalCount = 0;

    string budget_string = "BUDGET = $" + to_string(budget);
    printBudget(budget_string);
}

Game::~Game()
{
    delete pWind;
    delete gameToolbar;
    delete gameBudgetbar;

    for (int i = 0; i < chickCount; i++)
        if (chickList[i]) delete chickList[i];
    for (int i = 0; i < cowCount; i++)
        if (cowList[i]) delete cowList[i];
    for (int i = 0; i < wolfCount; i++)
        if (wolfList[i]) delete wolfList[i];

    delete[] pAnimalList;
}

clicktype Game::getMouseClick(int& x, int& y) const
{
    return pWind->WaitMouseClick(x, y);
}

string Game::getSrting() const
{
    string Label;
    char Key;
    keytype ktype;
    pWind->FlushKeyQueue();
    while (1)
    {
        ktype = pWind->WaitKeyPress(Key);
        if (ktype == ESCAPE) return "";
        if (Key == 13)       return Label;
        if (Key == 8) {
            if (Label.size() > 0)
                Label.resize(Label.size() - 1);
        }
        else {
            Label += Key;
        }
        printMessage(Label);
    }
}

window* Game::CreateWind(int w, int h, int x, int y) const
{
    window* pW = new window(w, h, x, y);
    pW->SetBrush(WHITESMOKE);
    pW->SetPen(WHITESMOKE);
    pW->DrawRectangle(0, 0, w, h);
    return pW;
}

void Game::createToolbar()
{
    point p = { 0, 0 };
    gameToolbar = new Toolbar(this, p, 0, config.toolBarHeight);
    gameToolbar->draw();
}

void Game::createBudgetbar()
{
    point p = { 0, config.toolBarHeight };
    gameBudgetbar = new Budgetbar(this, p, 0, config.toolBarHeight);
    gameBudgetbar->draw();
}

void Game::clearBudget() const
{
    pWind->SetPen(WHITESMOKE, 1);
    pWind->SetBrush(WHITESMOKE);
    pWind->DrawRectangle(config.windWidth - 500, config.toolBarHeight,
        config.windWidth, 2 * config.toolBarHeight);
}

void Game::printBudget(string msg) const
{
    clearBudget();
    pWind->SetPen(BLACK, 50);
    pWind->SetFont(24, BOLD, BY_NAME, "Arial");
    pWind->DrawString(config.windWidth - 200, config.toolBarHeight + 10, msg);
    pWind->DrawString(400, config.toolBarHeight + 10, "Animal Cost: $" + to_string(Animalsbuying));
    pWind->DrawString(700, config.toolBarHeight + 10, "Water Cost: $" + to_string(waterbuying));
}

void Game::clearStatusBar() const
{
    pWind->SetPen(WHITESMOKE, 1);
    pWind->SetBrush(WHITESMOKE);
    pWind->DrawRectangle(0, config.windHeight - config.statusBarHeight,
        config.windWidth, config.windHeight);
}

void Game::printMessage(string msg) const
{
    clearStatusBar();
    pWind->SetPen(BLACK, 50);
    pWind->SetFont(24, BOLD, BY_NAME, "Arial");
    int b = config.windHeight - (int)(0.85 * config.statusBarHeight);
    int spacing = config.windWidth / 4;
    pWind->DrawString(spacing * 0, b, "Time: " + to_string(remainingTime));
    pWind->DrawString(spacing * 1, b, "Goal: " + to_string(goal));
    pWind->DrawString(spacing * 2, b, "Level: " + to_string(currentLevel));
    pWind->DrawString(spacing * 3, b, "Animals: " + to_string(animalscount));
}

window* Game::getWind() const { return pWind; }

void Game::initLevel()
{
    if (currentLevel == 1) remainingTime = 120;
    else if (currentLevel == 2) remainingTime = 90;
    else                        remainingTime = 60;
}


void Game::updateTimer()
{
    static auto lastSecond = chrono::steady_clock::now();

    auto now = chrono::steady_clock::now();
    auto elapsed = chrono::duration_cast<chrono::seconds>(now - lastSecond).count();

    if (elapsed >= 1)
    {
        lastSecond = chrono::steady_clock::now();
        remainingTime--;
        elapsedSeconds++;  // ADD: count up every real second
    }

    if (remainingTime <= 0) {
        remainingTime = 0;
        printMessage("Game Over");
    }
    else {
        printMessage("");
    }
}
void Game::drawFoodArea()
{
    pWind->SetBrush(LIGHTGOLDENRODYELLOW);
    pWind->SetPen(DARKGREEN, 5);
    pWind->DrawRectangle(0, 2 * config.toolBarHeight,
        config.windWidth,
        config.windHeight - config.statusBarHeight);
}

void Game::drawField() const
{
    int left = 0, top = 2 * config.toolBarHeight;
    int right = config.windWidth, bottom = config.windHeight - config.statusBarHeight;
    pWind->SetBrush(SANDYBROWN);
    pWind->SetPen(DARKGREEN, 2);
    pWind->DrawRectangle(left, top, right, bottom);
    pWind->SetPen(BLACK, 4);
    pWind->DrawRectangle(left + 2, top + 2, right - 2, bottom - 2);
}

void Game::drawWarehouse() const
{
    int x = config.windWidth - 150;
    int y = config.windHeight - config.statusBarHeight - 150;
    pWind->DrawImage("images\\warehouse.jpg", x, y, 120, 120);
}

void Game::AddAnimal(Animal* pAni)
{
    if (!pAni) return;
    if (animalCount < 100)
        pAnimalList[animalCount++] = pAni;
}

// NEW: checks if any animal is on a food area and flashes field color
void Game::checkAnimalOnFood()
{
    bool onFood = false;

    // Check chicks over food
    for (int i = 0; i < chickCount && !onFood; i++) {
        if (!chickList[i]) continue;
        point pos = chickList[i]->getPos();
        for (auto& f : foodAreas) {
            if (pos.x + 50 >= f.x && pos.x <= f.x + 100 &&
                pos.y + 50 >= f.y && pos.y <= f.y + 100) {
                onFood = true;
                break;
            }
        }
    }

    // Check cows over food
    for (int i = 0; i < cowCount && !onFood; i++) {
        if (!cowList[i]) continue;
        point pos = cowList[i]->getPos();
        for (auto& f : foodAreas) {
            if (pos.x + 50 >= f.x && pos.x <= f.x + 100 &&
                pos.y + 50 >= f.y && pos.y <= f.y + 100) {
                onFood = true;
                break;
            }
        }
    }

    if (onFood != animalOnFood) {
        animalOnFood = onFood;
        // Animal ON food  → restore SANDYBROWN so food is visible
        // Animal OFF food → keep ORANGE as walking trail
        flashField(animalOnFood ? SANDYBROWN : ORANGE);
    }
}
// NEW: repaints the field with given color, then redraws food + warehouse on top
void Game::flashField(color c)
{
    int fieldTop = 2 * config.toolBarHeight;
    int fieldBottom = config.windHeight - config.statusBarHeight;
    int fieldRight = config.windWidth;

    // Paint field background
    pWind->SetPen(c, 1);
    pWind->SetBrush(c);
    pWind->DrawRectangle(0, fieldTop, fieldRight, fieldBottom);

    // Redraw border on top
    pWind->SetPen(BLACK, 4);
    pWind->DrawRectangle(2, fieldTop + 2, fieldRight - 2, fieldBottom - 2);

    // Redraw warehouse ON TOP so it never gets painted over
    drawWarehouse();

    // Redraw food ON TOP
    for (auto& f : foodAreas)
        pWind->DrawImage("images\\food5.jpg", f.x, f.y, 100, 100);
}
void Game::checkAnimalEatFood()
{
    for (int i = 0; i < cowCount; i++)
    {
        if (!cowList[i]) continue;
        point pos = cowList[i]->getPos();

        for (int j = 0; j < foodAreas.size(); j++)
        {
            // Check if cow overlaps food area
            if (pos.x + 50 >= foodAreas[j].x &&
                pos.x <= foodAreas[j].x + 100 &&
                pos.y + 50 >= foodAreas[j].y &&
                pos.y <= foodAreas[j].y + 100)
            {
                // Erase the food image with ORANGE
                pWind->SetPen(ORANGE, 1);
                pWind->SetBrush(ORANGE);
                pWind->DrawRectangle(foodAreas[j].x, foodAreas[j].y,
                    foodAreas[j].x + 100,
                    foodAreas[j].y + 100);

                // Remove food from list
                foodAreas.erase(foodAreas.begin() + j);

                // Trigger milk production
                cowList[i]->milk();

                // Draw milk item on screen
                for (int k = 0; k < ItemCount; k++)
                    if (ItemList[k] && ItemList[k]->type == "milk")
                        pWind->DrawImage("images\\milk.jpg",
                            ItemList[k]->pos.x,
                            ItemList[k]->pos.y, 30, 30);

                break; // one food per cow per frame
            }
        }
    }

    for (int i = 0; i < chickCount; i++)
    {
        if (!chickList[i]) continue;
        point pos = chickList[i]->getPos();

        for (int j = 0; j < foodAreas.size(); j++)
        {
            // Check if chick overlaps food area
            if (pos.x + 50 >= foodAreas[j].x &&
                pos.x <= foodAreas[j].x + 100 &&
                pos.y + 50 >= foodAreas[j].y &&
                pos.y <= foodAreas[j].y + 100)
            {
                // Erase the food image with ORANGE
                pWind->SetPen(ORANGE, 1);
                pWind->SetBrush(ORANGE);
                pWind->DrawRectangle(foodAreas[j].x, foodAreas[j].y,
                    foodAreas[j].x + 100,
                    foodAreas[j].y + 100);

                // Remove food from list
                foodAreas.erase(foodAreas.begin() + j);

                // Trigger egg production
                chickList[i]->egg();

                // Draw egg item on screen
                for (int k = 0; k < ItemCount; k++)
                    if (ItemList[k] && ItemList[k]->type == "egg")
                        pWind->DrawImage("images\\egg.jpg",
                            ItemList[k]->pos.x,
                            ItemList[k]->pos.y, 30, 30);

                break; // one food per chick per frame
            }
        }
    }
}
void Game::UpdateGame()
{
    updateTimer();
    trySpawnWolf();

    
    cout << "chickCount = " << chickCount << endl;
    cout << "cowCount = " << cowCount << endl;

    for (int i = 0; i < chickCount; i++)
        if (chickList[i]) chickList[i]->moveStep();

    for (int i = 0; i < cowCount; i++)
        if (cowList[i]) cowList[i]->moveStep();

    for (int i = 0; i < wolfCount; i++)
        if (wolfList[i]) wolfList[i]->moveStep();

    for (int i = 0; i < animalCount; i++)
        if (pAnimalList[i]) pAnimalList[i]->moveStep();

    drawWarehouse();

    for (auto& f : foodAreas)
        pWind->DrawImage("images\\food5.jpg", f.x, f.y, 100, 100);

    checkAnimalOnFood();
}

void Game::spawnWolf()
{
    point p;
    int yStart = 2 * config.toolBarHeight + 10;
    int yEnd = config.windHeight - config.statusBarHeight - 60;
    p.x = rand() % (config.windWidth - 60);
    p.y = yStart + rand() % (yEnd - yStart);

    Wolf* pWolf = new Wolf(this, p, 50, 50, "images\\wolf.jpg");
    AddAnimal(pWolf);
    printMessage("WOLF ATTACK!!!");
}

void Game::trySpawnWolf()
{
    bool isSpawnTime = (remainingTime == 90 || remainingTime == 20);

    if (currentLevel == 1) {
        if (isSpawnTime && !wolfCreated) {
            spawnWolf();
            wolfCreated = true;
        }
    }
    if (!isSpawnTime) wolfCreated = false;

    if (currentLevel == 2 && remainingTime % 15 == 0 && !wolfCreated) {
        spawnWolf();
        wolfCreated = true;
    }
}

void Game::eggadd()
{
    if (timer > 0 && timer % 10 == 0 && !egg_show)
    {
        for (int i = 0; i < chickCount; i++)
            if (chickList[i]) chickList[i]->egg();

        for (int i = 0; i < ItemCount; i++)
            if (ItemList[i] && ItemList[i]->type == "egg")
                pWind->DrawImage("images\\egg.jpg",
                    ItemList[i]->pos.x, ItemList[i]->pos.y, 30, 30);

        egg_show = true;
    }
}

void Game::milkadd()
{
    if (timer > 0 && timer % 15 == 0 && !milk_show)
    {
        for (int i = 0; i < cowCount; i++)
            if (cowList[i]) cowList[i]->milk();

        for (int i = 0; i < ItemCount; i++)
            if (ItemList[i] && ItemList[i]->type == "milk")
                pWind->DrawImage("images\\milk.jpg",
                    ItemList[i]->pos.x, ItemList[i]->pos.y, 30, 30);

        milk_show = true;
    }
}

void Game::drawItems()
{
    
}

#include <chrono>  // ADD at top of Game.cpp

void Game::letsgo()
{
    int x = 0, y = 0;
    bool isExit = false;
    pWind->ChangeTitle("- - - - - - - - - - Farm Frenzy (CIE101-project) - - - - - - - - - -");

    // Draw field once
    pWind->SetPen(ORANGE, 2);
    pWind->SetBrush(ORANGE);
    pWind->DrawRectangle(0, 2 * config.toolBarHeight,
        config.windWidth,
        config.windHeight - config.statusBarHeight);
    pWind->SetPen(BLACK, 4);
    pWind->DrawRectangle(2, 2 * config.toolBarHeight + 2,
        config.windWidth - 2,
        config.windHeight - config.statusBarHeight - 2);

    drawWarehouse();
    gameToolbar->draw();
    gameBudgetbar->draw();
    printBudget("BUDGET = $" + to_string(budget));

    // Precise timer
    auto lastTime = chrono::high_resolution_clock::now();
    const int FRAME_TIME_MS = 50;  // exactly 50ms per frame

    do {
        auto currentTime = chrono::high_resolution_clock::now();
        auto elapsed = chrono::duration_cast<chrono::milliseconds>(currentTime - lastTime).count();

        if (elapsed >= FRAME_TIME_MS)
        {
            lastTime = currentTime;

            // Step 1: move animals
            if (!isPaused)
                UpdateGame();

            // Step 2: handle clicks
            if (pWind->GetButtonState(LEFT_BUTTON, x, y))
            {
                if (y >= 0 && y < config.toolBarHeight)
                    isExit = gameToolbar->handleClick(x, y);
                else if (y >= config.toolBarHeight && y < 2 * config.toolBarHeight)
                    isExit = gameBudgetbar->handleClick(x, y);

                // Handle water placement
                if (!isPaused &&
                    buyWater &&
                    y >= 2 * config.toolBarHeight &&
                    y < config.windHeight - config.statusBarHeight)
                {
                    FoodArea f = { x, y, 100 };
                    foodAreas.push_back(f);
                    buyWater = false;
                }
            }

            if (!isPaused)
            {
                timer++;

                // Step 3: redraw UI
                gameToolbar->draw();
                gameBudgetbar->draw();

                // Step 4: redraw warehouse
                drawWarehouse();

                // Step 5: redraw food on top
                for (auto& f : foodAreas)
                    pWind->DrawImage("images\\food5.jpg", f.x, f.y, 100, 100);

                // Step 6: redraw eggs and milk LAST
                for (int i = 0; i < ItemCount; i++) {
                    if (!ItemList[i]) continue;
                    if (ItemList[i]->type == "egg")
                        pWind->DrawImage("images\\egg.jpg",
                            ItemList[i]->pos.x,
                            ItemList[i]->pos.y, 30, 30);
                    else if (ItemList[i]->type == "milk")
                        pWind->DrawImage("images\\milk.jpg",
                            ItemList[i]->pos.x,
                            ItemList[i]->pos.y, 30, 30);
                }
            }
        }
        else {
            Sleep(1);  // don't burn CPU while waiting
        }

    } while (!isExit);
}