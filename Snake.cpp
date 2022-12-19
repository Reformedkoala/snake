#include "Constants.h"
// direction mapping up:1,down:2,right:3,left:4
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
    
    ~Snake(){}
    
    void updatePosition(){
        srcrect.x = destrect.x;
        srcrect.y = destrect.y;
        switch(this->direction){
            case 1:
            destrect.y -= TEXTUREHEIGHT;
            break;
            case 2:
            destrect.y += TEXTUREHEIGHT;
            break;
            case 3:
            destrect.x += TEXTUREWIDTH;
            break;
            case 4:
            destrect.x -= TEXTUREWIDTH;
            break;
        }
    }
    
};
