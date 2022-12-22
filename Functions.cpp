#include <vector>
#include <random>
#include <ctime>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <unistd.h>
#include "Constants.h"

#pragma once

using namespace std;


//function to update position of the snake
void updatePosition(Snake &snake){
    snake.srcrect.x = snake.destrect.x;
    snake.srcrect.y = snake.destrect.y;
    switch(snake.direction){
        case 1:
        snake.destrect.y -= TEXTUREHEIGHT;
        break;
        case 2:
        snake.destrect.y += TEXTUREHEIGHT;
        break;
        case 3:
        snake.destrect.x += TEXTUREWIDTH;
        break;
        case 4:
        snake.destrect.x -= TEXTUREWIDTH;
        break;
    }
}


//Updating the each link in the snakes body and moving accordingly
void updateSnake(vector<Snake> &snake, int direction){
    int prevDirection = snake.at(0).direction;
    snake.at(0).direction = direction;
    updatePosition(snake.at(0));
    int tempDirection;
    for(unsigned int i = 1; i < snake.size(); i++){
        tempDirection = snake.at(i).direction;
        snake.at(i).direction = prevDirection;
        updatePosition(snake.at(i));
        prevDirection = tempDirection;
    }
}


//Checking collision of the snake on walls or itself
bool checkCollision(vector<Snake> &snake){
    if(snake.at(0).srcrect.x < 0 || snake.at(0).srcrect.x > BOARDWIDTH*32){
        return true;
    }
    
    if(snake.at(0).srcrect.y < 0 || snake.at(0).srcrect.y > BOARDHEIGHT*32){
        return true;
    }
    
    for(unsigned int i = 1; i < snake.size(); i++){
        if(snake.at(0).destrect.x == snake.at(i).destrect.x && snake.at(0).destrect.y == snake.at(i).destrect.y){
            return true;
        }
    }
    
    return false;
}


//Check food function to check if the snake has ran into the food
bool checkFood(Snake &snake, Apple &apple){
    if(snake.destrect.x == apple.destrect.x && snake.destrect.y == apple.destrect.y){
        return true;
    }
    return false;
}

//Check entire snake position, but can implement that later
//function to change position of the apple
void changeposition(Snake &snake, Apple &apple){
    apple.srcrect.x = apple.destrect.x;
    apple.srcrect.y = apple.destrect.y;
    int tempx = (rand()%BOARDWIDTH)*TEXTUREWIDTH;
    int tempy = (rand()%BOARDHEIGHT)*TEXTUREHEIGHT;
    while(tempx ==  apple.srcrect.x){
        tempx = (rand()%BOARDWIDTH)*TEXTUREWIDTH; 
    }
    while(tempy ==  apple.srcrect.y){
        tempy = (rand()%BOARDHEIGHT)*TEXTUREHEIGHT; 
    }
    apple.destrect.x = tempx;
    apple.destrect.y = tempy;
};


SDL_Texture* textToTexture(SDL_Renderer *render, const char * input, SDL_Rect &textRect, int x, int y){
    //Creating the intro text (May abstract this for score too)
    TTF_Font* font = TTF_OpenFont("Media/8bitOperatorPlus8-Regular.ttf", 24);
    SDL_Color White ={255, 255, 255};
    SDL_Surface* text = TTF_RenderText_Solid(font, input, White);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(render, text);
    
    textRect.x = x - (text->w/2);
    textRect.y = y - (text->h/2);
    textRect.w = text->w;
    textRect.h = text->h;
    return textTexture;
}


bool introScreen(SDL_Renderer *render, SDL_Event &event){
    SDL_Rect introRect;
    SDL_Texture* introTexture = textToTexture(render, "Press Enter to start the game", introRect, ((BOARDWIDTH*32) / 2), ((BOARDHEIGHT*32) / 2)); 
    
    //Playing introScreen loop that prompts the user for enter
    while(1){
        SDL_RenderClear(render);
        SDL_RenderCopy(render, introTexture, NULL, &introRect);
        //Allows the game to start once user presses enter
        if(SDL_PollEvent(&event) && SDL_KEYDOWN == event.type && SDLK_RETURN == event.key.keysym.sym){
            SDL_DestroyTexture(introTexture);
            return 1;
        }
        //Quits the game and deallocates everything to deal with garbage
        if(SDL_PollEvent(&event) && SDL_QUIT == event.type){
            SDL_DestroyTexture(introTexture);
            return 0;
        }
        SDL_RenderPresent(render);
        
    }
}


bool replayScreen(SDL_Renderer *render, SDL_Event &event, int food){
    //Creating the intro text (May abstract this for score too)
    SDL_Rect replayRect;
    SDL_Texture* replayTexture = textToTexture(render, "GAME OVER Press Enter to try again", replayRect, ((BOARDWIDTH*32) / 2), ((BOARDHEIGHT*32) / 2));
    
    SDL_Rect scoreRect;
    string score = to_string(food);
    SDL_Texture* scoreTexture = textToTexture(render, score.c_str(), scoreRect, (((BOARDWIDTH*32) / 2)+25), (((BOARDHEIGHT*32) / 2) - 50));
    
    SDL_Rect labelRect;
    SDL_Texture* labelTexture = textToTexture(render, "Score: ", labelRect, (((BOARDWIDTH*32) / 2)-25), (((BOARDHEIGHT*32) / 2) - 50));
    
    //Playing introScreen loop that prompts the user for enter
    while(1){
        SDL_RenderClear(render);
        SDL_RenderCopy(render, replayTexture, NULL, &replayRect);
        SDL_RenderCopy(render, scoreTexture, NULL, &scoreRect);
        SDL_RenderCopy(render, labelTexture, NULL, &labelRect);
        //Allows the game to start once user presses enter
        if(SDL_PollEvent(&event) && SDL_KEYDOWN == event.type && SDLK_RETURN == event.key.keysym.sym){
            SDL_DestroyTexture(replayTexture);
            return 1;
        }
        //Quits the game and deallocates everything to deal with garbage
        if(SDL_PollEvent(&event) && SDL_QUIT == event.type){
            SDL_DestroyTexture(replayTexture);
            SDL_RenderClear(render);
            return 0;
        }
        SDL_RenderPresent(render);
        
    }
}