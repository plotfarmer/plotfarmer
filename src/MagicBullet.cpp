#include"MagicBullet.h"

MagicBullet::MagicBullet(int x, int y, SDL_Renderer *r, vec2di &c, Level &l, int direction):
super(x,y,r,c,false),
alive(true),
spritesheet("sprites/ball.png",1,1,1,r),
timer(0),
level(l)
{
    if (direction==DIRECTIONRIGHT)
    
    vel.x = SPEEDLIMIT;
    else vel.x = - SPEEDLIMIT;
    vel.y = 0;
}

void MagicBullet::render()
{
    if (isDead()) return;
    vec2di renderpos;
    renderpos.x = (interppos.x)+roundf(interpremainder.x);
    renderpos.y = (interppos.y)+roundf(interpremainder.y);

    SDL_Rect dstRec = {
        renderpos.x-camera.x,
        renderpos.y-camera.y,
        spritesheet.singleWidth*SCALE,
        spritesheet.singleHeight*SCALE
        };
    SDL_RenderCopy(gameRenderer,
        spritesheet.sprites,
        NULL,
        &(dstRec));
}

void MagicBullet::update()
{
    super::update();
    ++timer;
    
    //check wether a collision with the level occured.
    int offset = (vel.x > 0) ? spritesheet.singleWidth*SCALE : 0;
    int TileX = ((pos.x+offset)/SCALEDBLOCK);
    int TileY0 = ((pos.y)/SCALEDBLOCK);
    int TileY1 = ((pos.y+spritesheet.singleHeight*SCALE)/SCALEDBLOCK);
    if (level.isSolid(TileY0,TileX))
    {
        level.tiles[TileY0][TileX]=-1;
        alive = false;
    }
    if (level.isSolid(TileY1,TileX))
    {
        level.tiles[TileY1][TileX]=-1;
        alive = false;
    }
}

bool MagicBullet::isDead()
{
    return (!alive || timer > BULLET_LIFETIME);
}

MagicBullet::~MagicBullet()
{
}
