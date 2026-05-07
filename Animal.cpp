#include "Animal.h"
#include "../Config/GameConfig.h"
#include "../Core/Game.h"
#include "../CMUgraphicsLib/CMUgraphics.h"
#include <iostream>
#include <chrono>
using namespace chrono;
using namespace std;

Animal::Animal(Game* r_pGame, point r_point, int r_width, int r_height, string img_path)
    : Drawable(r_pGame, r_point, r_width, r_height)
{
    image_path = img_path;
    curr_pos = r_point;
    speed = (rand() % 3) + 1;
    curr_vel.x = ((rand() % 2) == 0 ? 1 : -1) * speed;
    curr_vel.y = ((rand() % 2) == 0 ? 1 : -1) * speed;
}

void Animal::draw() const
{
    window* pWind = pGame->getWind();
    pWind->DrawImage(image_path, RefPoint.x, RefPoint.y, width, height);
}

// ─────────────────────────────────────────
//  CHICK
// ─────────────────────────────────────────
Chick::Chick(Game* r_pGame, point r_point, int r_width, int r_height, string img_path)
    : Animal(r_pGame, r_point, r_width, r_height, img_path)
{
}

void Chick::draw() const
{
    Animal::draw();
    // FIX: removed showEgg draw here — egg is drawn in letsgo loop from ItemList
}

void Chick::egg()
{
    if (pGame->ItemCount >= 100) return;

    Game::Item* item = new Game::Item();
    // Save position where chick IS RIGHT NOW
    item->pos.x = RefPoint.x + width / 2 - 15;
    item->pos.y = RefPoint.y + height / 2 - 15;
    item->type = "egg";
    pGame->ItemList[pGame->ItemCount++] = item;
}

void Chick::moveStep()
{
    window* pWind = pGame->getWind();

    // Erase old position
    pWind->SetPen(ORANGE, 1);
    pWind->SetBrush(ORANGE);
    pWind->DrawRectangle(RefPoint.x, RefPoint.y,
        RefPoint.x + width,
        RefPoint.y + height);

    // Restore items erased
    for (int i = 0; i < pGame->ItemCount; i++) {
        if (!pGame->ItemList[i]) continue;
        int ix = pGame->ItemList[i]->pos.x;
        int iy = pGame->ItemList[i]->pos.y;
        if (ix + 30 >= RefPoint.x && ix <= RefPoint.x + width &&
            iy + 30 >= RefPoint.y && iy <= RefPoint.y + height) {
            if (pGame->ItemList[i]->type == "egg")
                pWind->DrawImage("images\\egg.jpg", ix, iy, 30, 30);
            else if (pGame->ItemList[i]->type == "milk")
                pWind->DrawImage("images\\milk.jpg", ix, iy, 30, 30);
        }
    }

    // Restore warehouse
    int wx = config.windWidth - 150;
    int wy = config.windHeight - config.statusBarHeight - 150;
    if (RefPoint.x + width >= wx && RefPoint.x <= wx + 120 &&
        RefPoint.y + height >= wy && RefPoint.y <= wy + 120)
        pGame->drawWarehouse();

    // Move
    RefPoint.x += curr_vel.x;
    RefPoint.y += curr_vel.y;

    // Bounce
    if (RefPoint.x < 0) {
        RefPoint.x = 0; curr_vel.x = abs(curr_vel.x);
    }
    else if (RefPoint.x > config.windWidth - width) {
        RefPoint.x = config.windWidth - width; curr_vel.x = -abs(curr_vel.x);
    }
    if (RefPoint.y < config.toolBarHeight * 2) {
        RefPoint.y = config.toolBarHeight * 2; curr_vel.y = abs(curr_vel.y);
    }
    else if (RefPoint.y > config.windHeight - config.statusBarHeight - height) {
        RefPoint.y = config.windHeight - config.statusBarHeight - height;
        curr_vel.y = -abs(curr_vel.y);
    }

    // FIX: drop egg every 20 real seconds using game's elapsedSeconds
    if (pGame->elapsedSeconds - lastEggDrop >= 20)
    {
        lastEggDrop = pGame->elapsedSeconds;
        egg();
    }

    draw();
}



Cow::Cow(Game* r_pGame, point r_point, int r_width, int r_height, string img_path)
    : Animal(r_pGame, r_point, r_width, r_height, img_path)
{
}

void Cow::draw() const
{
    Animal::draw();
    // FIX: removed showMilk draw here — milk is drawn in letsgo loop from ItemList
}
void Cow::milk()
{
    if (pGame->ItemCount >= 100) return;

    Game::Item* item = new Game::Item();
    // Save position where cow IS RIGHT NOW
    item->pos.x = RefPoint.x + width / 2 - 15;
    item->pos.y = RefPoint.y + height / 2 - 15;
    item->type = "milk";
    pGame->ItemList[pGame->ItemCount++] = item;
}
void Cow::moveStep()
{
    window* pWind = pGame->getWind();

    // Erase old position
    pWind->SetPen(ORANGE, 1);
    pWind->SetBrush(ORANGE);
    pWind->DrawRectangle(RefPoint.x, RefPoint.y,
        RefPoint.x + width,
        RefPoint.y + height);

    // Restore items erased
    for (int i = 0; i < pGame->ItemCount; i++) {
        if (!pGame->ItemList[i]) continue;
        int ix = pGame->ItemList[i]->pos.x;
        int iy = pGame->ItemList[i]->pos.y;
        if (ix + 30 >= RefPoint.x && ix <= RefPoint.x + width &&
            iy + 30 >= RefPoint.y && iy <= RefPoint.y + height) {
            if (pGame->ItemList[i]->type == "egg")
                pWind->DrawImage("images\\egg.jpg", ix, iy, 30, 30);
            else if (pGame->ItemList[i]->type == "milk")
                pWind->DrawImage("images\\milk.jpg", ix, iy, 30, 30);
        }
    }

    // Restore warehouse
    int wx = config.windWidth - 150;
    int wy = config.windHeight - config.statusBarHeight - 150;
    if (RefPoint.x + width >= wx && RefPoint.x <= wx + 120 &&
        RefPoint.y + height >= wy && RefPoint.y <= wy + 120)
        pGame->drawWarehouse();

    // Move
    RefPoint.x += curr_vel.x;
    RefPoint.y += curr_vel.y;

    // Bounce
    if (RefPoint.x < 0) {
        RefPoint.x = 0; curr_vel.x = abs(curr_vel.x);
    }
    else if (RefPoint.x > config.windWidth - width) {
        RefPoint.x = config.windWidth - width; curr_vel.x = -abs(curr_vel.x);
    }
    if (RefPoint.y < config.toolBarHeight * 2) {
        RefPoint.y = config.toolBarHeight * 2; curr_vel.y = abs(curr_vel.y);
    }
    else if (RefPoint.y > config.windHeight - config.statusBarHeight - height) {
        RefPoint.y = config.windHeight - config.statusBarHeight - height;
        curr_vel.y = -abs(curr_vel.y);
    }

    // FIX: drop milk every 20 real seconds using game's elapsedSeconds
    if (pGame->elapsedSeconds - lastMilkDrop >= 20)
    {
        lastMilkDrop = pGame->elapsedSeconds;
        milk();
    }

    draw();
}

// ─────────────────────────────────────────
//  WOLF
// ─────────────────────────────────────────
Wolf::Wolf(Game* r_pGame, point r_point, int r_width, int r_height, string img_path)
    : Animal(r_pGame, r_point, r_width, r_height, img_path)
{
}

void Wolf::moveStep()
{
    window* pWind = pGame->getWind();

    // Erase old position
    pWind->SetPen(ORANGE, 1);
    pWind->SetBrush(ORANGE);
    pWind->DrawRectangle(RefPoint.x, RefPoint.y,
        RefPoint.x + width,
        RefPoint.y + height);

    // Restore warehouse if walked over it
    int wx = config.windWidth - 150;
    int wy = config.windHeight - config.statusBarHeight - 150;
    if (RefPoint.x + width >= wx && RefPoint.x <= wx + 120 &&
        RefPoint.y + height >= wy && RefPoint.y <= wy + 120)
        pGame->drawWarehouse();

    // Move
    RefPoint.x += curr_vel.x;
    RefPoint.y += curr_vel.y;

    // Bounce
    if (RefPoint.x < 0) {
        RefPoint.x = 0;
        curr_vel.x = abs(curr_vel.x);
    }
    else if (RefPoint.x > config.windWidth - width) {
        RefPoint.x = config.windWidth - width;
        curr_vel.x = -abs(curr_vel.x);
    }
    if (RefPoint.y < config.toolBarHeight * 2) {
        RefPoint.y = config.toolBarHeight * 2;
        curr_vel.y = abs(curr_vel.y);
    }
    else if (RefPoint.y > config.windHeight - config.statusBarHeight - height) {
        RefPoint.y = config.windHeight - config.statusBarHeight - height;
        curr_vel.y = -abs(curr_vel.y);
    }

    draw();
}