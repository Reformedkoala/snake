#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <unistd.h>
#include "Snake.cpp"
#include "Apple.cpp"
#include "Constants.h"
#include "Functions.cpp"

using namespace std;


int main(int argc, char *argv[]){
    //Seeding Random and initializing snake vector and apple
    srand(time(0));
    vector<Snake> snake;
    Apple apple;
    
    //Initializing SDL, IMG, and TTF
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();
    
    //Creating Window and renderer
    SDL_Window *window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, BOARDWIDTH*32, BOARDHEIGHT*32, SDL_WINDOW_SHOWN);
    if(NULL == window){cout << "failed to initialize window" << endl;}
    SDL_Renderer *render = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(render, 0, 0, 0, 0);
    
    //Creating event 
    SDL_Event event;
    
    if(!introScreen(render, event)){
        SDL_DestroyRenderer(render);
        SDL_DestroyWindow(window);
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return 0;
    }
    
    //Creating texture for snake and apple (might move apple to not contain it's own texture)
    SDL_Texture* snakeTex = IMG_LoadTexture(render, "Media/Snake.png");
    SDL_Texture* appleTex = IMG_LoadTexture(render, "Media/Apple.png");
    
    //Initialize direction and food, starts food at 1, technically could start it larger, but that would require dealing with not starting each link at 0,0
    int direction = 3;
    int food = 1;
    Snake tempSnake;
    snake.push_back(tempSnake);
    
    //Randomizes our apple position
    changeposition(snake.at(0), apple);
    
    //main bool for gameRunning
    bool gameRunning = true;
    
    //main game loop
    int start;
    int end;
    float elapsedMS;
    int frameCounter=0;
    
    while(gameRunning){
        //Performance counter to eventuall use to control fps
        start = SDL_GetPerformanceCounter();
        SDL_RenderClear(render);
        
        
        // game  state check involving collision and food, simple math and checking of collision
        
        if(checkCollision(snake)){
            if(replayScreen(render, event, food)){
                gameRunning = true;
                snake.clear();
                direction = 3;
                food = 1;
                Snake tempSnake;
                snake.push_back(tempSnake);
                changeposition(snake.at(0), apple);
                continue;
            }else{
                gameRunning = false;
                continue;
            }
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
            changeposition(snake.at(0), apple);
            
        }
        
        
        
        //Main user input function that controls snake movement and control interrupts
        if (SDL_PollEvent(&event)){
            //Input handling
            switch(event.type){
                case SDL_KEYDOWN:
                switch(event.key.keysym.sym){
                    case SDLK_UP:
                    direction = 1;
                    break;
                    case SDLK_LEFT:
                    direction = 4;
                    break;
                    case SDLK_DOWN:
                    direction = 2;
                    break;
                    case SDLK_RIGHT:
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
        
        //Updates snake position and links
        if(frameCounter % 4 ==0){
            updateSnake(snake, direction);
        }
        //Renders apple every loop
        SDL_RenderCopy(render, appleTex, NULL, &apple.destrect);
        
        //Loop to go over snake body
        for (unsigned int i = 0; i < snake.size(); i++){
            SDL_RenderCopy(render, snakeTex, NULL, &snake.at(i).destrect);
        }
        //Logic for dealing with fps, need to work on this because it's a little fast
        end = SDL_GetPerformanceCounter();
        elapsedMS = (end-start)/(float)SDL_GetPerformanceFrequency()*1000.0f;
        
        SDL_Delay(floor(15.666f - elapsedMS));
        cout << floor(15.666f - elapsedMS) << endl;
        //Presents the render
        SDL_RenderPresent(render);
        frameCounter++;
    }
    
    //Clears everything that was used
    SDL_DestroyTexture(snakeTex);
    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    
    return 0;
}