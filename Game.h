#pragma once
#include "../CMUgraphicsLib/CMUgraphics.h"
#include "../UI/Toolbar.h"
#include "../UI/BudgetBar.h"

class Animal;
class Chick;
class Cow;
class Wolf;

class Game
{
private:
    window* pWind;
    Toolbar* gameToolbar;
    Budgetbar* gameBudgetbar;
    int remainingTime = 0;
    int currentLevel = 1;
    int goal = 10;
    int animalscount = 0;
    int Animalsbuying = 0;
    int waterbuying = 0;

public:
    // ── Budget ──────────────────────────────────────────
    int  budget = 30000;
    bool buyWater = false;

    // ── Animal Lists ────────────────────────────────────
    Chick* chickList[100] = {};
    Cow* cowList[100] = {};
    Wolf* wolfList[100] = {};
    int chickCount = 0;
    int cowCount = 0;
    int wolfCount = 0;

    // ── Generic Animal List ──────────────────────────────
    Animal** pAnimalList = nullptr;
    int      animalCount = 0;

    // ── Animal Type Enum ────────────────────────────────
    enum AnimalType { ANIMAL_NONE, COW, CHICK, WOLF };
    AnimalType currentAnimal = ANIMAL_NONE;

    // ── Items ────────────────────────────────────────────
    struct Item {
        point  pos;
        string type;
    };
    Item* ItemList[100] = {};
    int   ItemCount = 0;

    // ── Food Areas ───────────────────────────────────────
    struct FoodArea {
        int x, y, amount;
    };
    vector<FoodArea> foodAreas;

    // ── Flags & Timers ───────────────────────────────────
    bool isPaused = false;
    bool wolfCreated = false;
    bool milk_show = false;
    bool egg_show = false;
    int  timer = 0;

    // ── NEW: Food Collision Flag ─────────────────────────
    bool animalOnFood = false;  // true when any animal is on a food area

    // ── Constructor / Destructor ─────────────────────────
    Game();
    ~Game();

    // ── Core Loop ────────────────────────────────────────
    void letsgo();
    void UpdateGame();

    // ── Wolf ─────────────────────────────────────────────
    void trySpawnWolf();
    void spawnWolf();

    // ── Window / UI ──────────────────────────────────────
    window* CreateWind(int, int, int, int) const;
    window* getWind()                      const;
    void createToolbar();
    void createBudgetbar();
    void clearBudget()            const;
    void printBudget(string msg)  const;
    void clearStatusBar()         const;
    void printMessage(string msg) const;
    void drawField()              const;
    void drawWarehouse()          const;
    void drawItems();
    void drawFoodArea();
    void checkAnimalEatFood();  // ADD this
    // ── Helpers ──────────────────────────────────────────
    clicktype getMouseClick(int& x, int& y) const;
    string    getSrting()                   const;
    void setCurrentAnimal(AnimalType type);
    void updateTimer();
    void initLevel();
    void AddAnimal(Animal* pAni);
    void eggadd();
    void milkadd();
    int elapsedSeconds = 0;  // counts real seconds, updated by updateTimer()
    // ── NEW: Food Collision ──────────────────────────────
    void checkAnimalOnFood();        // checks if animal overlaps food area
    void flashField(color c);        // repaints field with given color
};