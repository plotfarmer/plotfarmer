#ifndef PLAYER_H
#define PLAYER_H

#include<SDL2/SDL_image.h>
#include<SDL2/SDL.h>
#include<vector>
#include<iterator>
#include<iostream>
#include"Spritesheet.h"
#include"Level.h"
#include"Geometry.h"
#include"Constants.h"




class Player{
    
    public:
        Player(SDL_Renderer*, Level* l, vec2di *c);
        ~Player();
        void render();
        void update(long);
        void setDirection(int d);
        void stop();
        void jump();
            
    private:
        //functions:
        bool isColliding();
        void handleCollision();
        void updateBounding();
        bool didIHitMyHead();
        void setCamera();
        //fields
        SDL_Rect dstRect;
        SDL_Renderer* gameRenderer;
        SDL_Rect* getCurrentRectangle();
        Level* level;
        Spritesheet* spritesheet;
        vec2df vel;
        vec2di pos;
        vec2df remainder;
        vec2di *camera;
        int height;
        int width;
        short direction;
        rectangle bounding;
        bool running;
        bool inJump;
        int acc_counter;
    };

#endif
