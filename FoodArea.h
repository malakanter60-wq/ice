#pragma once
#include "../Core/Drawable.h"
#include "../Config/GameConfig.h"

class FoodArea : public Drawable
{
private:
    int foodAmount;       // كمية الأكل المتبقية
public:
    FoodArea(Game* r_pGame, point r_point, int r_width, int r_height, int initialFood);
    void draw() const override;      
    void decreaseFood();                 
    int getFoodAmount() const;         
};