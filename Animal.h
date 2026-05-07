#pragma once
#include "../Core/Drawable.h"
#include <chrono>

class Animal : public Drawable
{
private:
    string image_path;
protected:
    point curr_pos;
    point curr_vel;
    int speed;  // ADD: store speed separately so bounce preserves magnitude

public:
    Animal(Game* r_pGame, point r_point, int r_width, int r_height, string img_path);
    virtual void draw() const override;
    virtual void moveStep() = 0;
    string getImagePath() const { return image_path; }
    virtual int getType() = 0;
    point getPos() const { return RefPoint; }  // FIX: was returning curr_pos which never updates, RefPoint is what actually moves
};


class Chick : public Animal
{
    bool showEgg = false;
    int  lastEggDrop = 0;  // stores game timer value when egg was last dropped
    //int  eggTimer = 0;    // MUST be here

public:
    Chick(Game* r_pGame, point r_point, int r_width, int r_height, string img_path);
    virtual void draw() const override;
    void moveStep() override;
    virtual int getType() override { return 1; }
    void egg();
};

class Cow : public Animal
{
    bool showMilk = false;
    int  lastMilkDrop = 0;  // stores game timer value when milk was last dropped
   // int  milkTimer = 0;   // MUST be here
public:
    Cow(Game* r_pGame, point r_point, int r_width, int r_height, string img_path);
    virtual void draw() const override;
    void moveStep() override;
    virtual int getType() override { return 2; }
    void milk();
};
class Wolf : public Animal
{
public:
    Wolf(Game* r_pGame, point r_point, int r_width, int r_height, string img_path);
    void moveStep() override;
    virtual int getType() override { return 3; }
};

class Egg : public Drawable
{
private:
    int quantity;

public:
    Egg(Game* r_pGame, point r_pos = { 900, 100 }, int r_qty = 0);
    void draw() const override;
    void addQuantity(int qty);
    int getQuantity() const { return quantity; }
    void sellAll();
};

class Milk : public Drawable
{
private:
    int quantity;

public:
    Milk(Game* r_pGame, point r_pos = { 800, 100 }, int r_qty = 0);
    void draw() const override;
    void addQuantity(int qty);
    int getQuantity() const { return quantity; }
    void sellAll();
};