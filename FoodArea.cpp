#include "FoodArea.h"
#include "../Core/Game.h"
#include <iostream>
using namespace std;

FoodArea::FoodArea(Game* r_pGame, point r_point, int r_width, int r_height, int initialFood)
    : Drawable(r_pGame, r_point, r_width, r_height), foodAmount(initialFood)
{
}

void FoodArea::draw() const
{
    window* pWind = pGame->getWind();
  \
    pWind->SetBrush(GREEN);
    pWind->SetPen(BLACK, 2);
    pWind->DrawRectangle(RefPoint.x, RefPoint.y, RefPoint.x + width, RefPoint.y + height);

    pWind->SetFont(20, BOLD, BY_NAME, "Arial");
    pWind->DrawString(RefPoint.x + width / 4, RefPoint.y + height / 4, "Food: " + to_string(foodAmount));
}

void FoodArea::decreaseFood()
{
    if (foodAmount > 0) foodAmount--;
}

int FoodArea::getFoodAmount() const
{
    return foodAmount;
}