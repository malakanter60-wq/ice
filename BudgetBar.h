#pragma once
#include "../Core/Drawable.h"
#include "../Entities/Animal.h"
#include "../Config/GameConfig.h"
#include <random>


const int range_min_x = 50;
const int range_max_x = config.windWidth - 50;
const int range_min_y = (config.toolBarHeight * 2) + 50;
const int range_max_y = config.windHeight - config.statusBarHeight - 50;


class BudgetbarIcon : public Drawable
{
public:
    string image_path;
    BudgetbarIcon(Game* r_pGame, point r_point, int r_width, int r_height, string img_path);
    virtual void draw()    const override;
    virtual void onClick() = 0;
};

class ChickIcon : public BudgetbarIcon
{
public:
    // FIX: removed local chickList and count
    // animals go directly into pGame->chickList
    ChickIcon(Game* r_pGame, point r_point, int r_width, int r_height, string img_path);
    virtual void onClick() override;
};

class CowIcon : public BudgetbarIcon
{
public:
    // FIX: removed local cowList and count
    CowIcon(Game* r_pGame, point r_point, int r_width, int r_height, string img_path);
    virtual void onClick() override;
};

class WolfIcon : public BudgetbarIcon
{
public:
    // FIX: removed local wolfList and count
    WolfIcon(Game* r_pGame, point r_point, int r_width, int r_height, string img_path);
    virtual void onClick() override;
};

class WaterIcon : public BudgetbarIcon
{
public:
    WaterIcon(Game* r_pGame, point r_point, int r_width, int r_height, string img_path);
    virtual void onClick() override;
};

enum ANIMAL_ICONS
{
    ICON_CHICK,
    ICON_COW,
    ICON_WOLF,
    ICON_WATER,
    ANIMAL_COUNT   
};

class Budgetbar : public Drawable
{
private:
    BudgetbarIcon** iconsList;
    string iconsImages[ANIMAL_COUNT];

public:
    Budgetbar(Game* r_pGame, point r_point, int r_width, int r_height);
    ~Budgetbar();
    void draw()          const override;
    bool handleClick(int x, int y);
};