#include "Constants.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

struct Apple{
    SDL_Rect srcrect;
    SDL_Rect destrect;
    SDL_Texture* appleTex;
    
    //Need to implement checking snake position as well, but can do that later
    void changeposition(Snake &snake){
        srcrect.x = destrect.x;
        srcrect.y = destrect.y;
        int tempx = (rand()%BOARDWIDTH)*TEXTUREWIDTH;
        int tempy = (rand()%BOARDHEIGHT)*TEXTUREHEIGHT;
        while(tempx ==  srcrect.x){
            tempx = rand()%BOARDWIDTH; 
        }
        while(tempy ==  srcrect.y){
            tempy= rand()%BOARDWIDTH; 
            
        }
        destrect.x = tempx;
        destrect.y = tempy;
    };
    
    Apple(){
        appleTex = nullptr;
        srcrect.x = 0;
        srcrect.y = 0;
        srcrect.w = TEXTUREWIDTH;
        srcrect.h = TEXTUREHEIGHT;
        destrect.x = 0;
        destrect.y = 0;
        destrect.w = TEXTUREWIDTH;
        destrect.h = TEXTUREHEIGHT;
    }
    
    ~Apple(){
        SDL_DestroyTexture(appleTex);
    }
    
};

