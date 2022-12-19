#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "Snake.cpp"
#include "Apple.cpp"
#include "Constants.h"
#include <unistd.h>

using namespace std;


void updateSnake(vector<Snake> &snake, int direction){
    int prevDirection = snake.at(0).direction;
    snake.at(0).direction = direction;
    snake.at(0).updatePosition();
    int tempDirection;
    for(int i = 1; i < snake.size(); i++){
        tempDirection = snake.at(i).direction;
        snake.at(i).direction = prevDirection;
        snake.at(i).updatePosition();
        prevDirection = tempDirection;
    }
}


bool checkCollision(vector<Snake> &snake){
    if(snake.at(0).destrect.x < 0 || snake.at(0).destrect.x > BOARDWIDTH*32){
        return true;
    }
    
    if(snake.at(0).destrect.y < 0 || snake.at(0).destrect.y > BOARDHEIGHT*32){
        return true;
    }
    
    for(int i = 1; i < snake.size(); i++){
        if(snake.at(0).destrect.x == snake.at(i).destrect.x && snake.at(0).destrect.y == snake.at(i).destrect.y){
            return true;
        }
    }
    
    return false;
}


bool checkFood(Snake &snake, Apple &apple){
    if(snake.destrect.x == apple.destrect.x && snake.destrect.y == apple.destrect.y){
        return true;
    }
    return false;
}


int main(int argc, char *argv[]){
    srand(time(0));
    vector<Snake> snake;
    Apple apple;
    
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();
    SDL_Window *window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, BOARDWIDTH*32, BOARDHEIGHT*32, SDL_WINDOW_SHOWN);
    if(NULL == window){cout << "failed to initialize window" << endl;}
    SDL_Renderer *render = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(render, 0, 0, 0, 0);
    SDL_Texture* snakeTex = IMG_LoadTexture(render, "Media/Snake.png");
    apple.appleTex = IMG_LoadTexture(render, "Media/Apple.png");
    SDL_Event event;
    
    TTF_Font* font = TTF_OpenFont("Media/8bitOperatorPlus8-Regular.ttf", 24);
    SDL_Color White ={255, 255, 255};
    SDL_Surface* text = TTF_RenderText_Solid(font, "Press Enter to start the game", White);
    SDL_Texture* introTexture = SDL_CreateTextureFromSurface(render, text);
    SDL_Rect introRect;
    introRect.x = ((BOARDWIDTH*32) / 2) - (text->w/2);
    introRect.y = ((BOARDHEIGHT*32) / 2) - (text->h/2);
    introRect.w = text->w;
    introRect.h = text->h;
    
    int direction = 3;
    int food = 1;
    for(int i = 0; i < food; i++){
        Snake tempSnake;
        snake.push_back(tempSnake);
    }
    
    apple.changeposition(snake.at(0));
    
    while(1){
        SDL_RenderClear(render);
        SDL_RenderCopy(render, introTexture, NULL, &introRect);
        if(SDL_PollEvent(&event) && SDL_KEYDOWN == event.type && SDLK_RETURN == event.key.keysym.sym){
            break;
        }
        if(SDL_PollEvent(&event) && SDL_QUIT == event.type){
            TTF_Quit();
            SDL_DestroyTexture(introTexture);
            SDL_DestroyTexture(snakeTex);
            SDL_DestroyRenderer(render);
            SDL_DestroyWindow(window);
            SDL_Quit();
            return 0;
        }
        SDL_RenderPresent(render);
        
    }
    bool gameRunning = true;
    while(gameRunning){
        int start = SDL_GetPerformanceCounter();
        SDL_RenderClear(render);
        // game  state check
        if(checkCollision(snake)){
            gameRunning = false;
        }
        if(checkFood(snake.at(0), apple)){
            food++;
            Snake tempSnake;
            tempSnake.destrect.x = snake.at(snake.size()-1).destrect.x;
            tempSnake.destrect.y = snake.at(snake.size()-1).destrect.y;
            tempSnake.direction = snake.at(snake.size()-1).direction;
            switch(tempSnake.direction){
                case 1:
                tempSnake.destrect.y += TEXTUREHEIGHT;
                break;
                case 2:
                tempSnake.destrect.y -= TEXTUREHEIGHT;
                break;
                case 3:
                tempSnake.destrect.x -= TEXTUREWIDTH;
                break;
                case 4:
                tempSnake.destrect.x += TEXTUREWIDTH;
                break;
            }
            snake.push_back(tempSnake);
            apple.changeposition(snake.at(0));
            
        }
        
        if (SDL_PollEvent(&event)){
            //Input handling
            switch(event.type){
                case SDL_KEYDOWN:
                switch(event.key.keysym.sym){
                    case SDLK_UP:
                    cout << "UP ARROW DOWN" << endl;
                    direction = 1;
                    break;
                    case SDLK_LEFT:
                    cout << "LEFT ARROW DOWN" << endl;
                    direction = 4;
                    break;
                    case SDLK_DOWN:
                    cout << "DOWN ARROW DOWN" << endl;
                    direction = 2;
                    break;
                    case SDLK_RIGHT:
                    cout << "RIGHT ARROW DOWN" << endl;
                    direction = 3;
                    break;
                }
                break;
                case SDL_KEYUP:
                break;
                case SDL_QUIT:
                gameRunning = false;
                break;
            }
        }
        updateSnake(snake, direction);
        SDL_RenderCopy(render, apple.appleTex, NULL, &apple.destrect);
        for (int i = 0; i < snake.size(); i++){
            SDL_RenderCopy(render, snakeTex, NULL, &snake.at(i).destrect);
        }
        //event handling end
        SDL_RenderPresent(render);
        int end = SDL_GetPerformanceCounter();
        float elapsedMS = (end-start)/(float)SDL_GetPerformanceFrequency()*1000.0f;
        SDL_Delay(floor(60.0f - elapsedMS));
    }
    
    TTF_Quit();
    SDL_DestroyTexture(introTexture);
    SDL_DestroyTexture(snakeTex);
    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}