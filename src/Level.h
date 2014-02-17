#ifndef LEVEL_H
#define LEVEL_H

#include<vector>
#include<iostream>

class Level
{
    public:
        Level();
        ~Level();
        std::vector<std::vector<int>> tiles;
        bool isSolid(int,int);
        int height;
        int width;
        int pixelHeight;
        int pixelWidth;
    private:
        
};

#endif