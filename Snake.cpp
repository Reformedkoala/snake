#include "Constants.h"
#pragma once

// direction mapping up:1,down:2,right:3,left:4
//Snake struct containing related values, may move updatePosition outside of struct
struct Snake{
    SDL_Rect srcrect;
    SDL_Rect destrect;
    int direction;
    
    Snake(){
        srcrect.x = 0;
        srcrect.y = 0;
        srcrect.w = TEXTUREWIDTH;
        srcrect.h = TEXTUREHEIGHT;
        destrect.x = 0;
        destrect.y = 0;
        destrect.w = TEXTUREWIDTH;
        destrect.h = TEXTUREHEIGHT;
    }
};

