#include "inc/SDL.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#undef main

// Direction enum declaration
enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

// Constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int TILE_SIZE = 20;
const int TARGET_FRAME_RATE = 60;   // Frames per second
const int MOVEMENT_DELAY = 100;     // Milliseconds delay between movements

// Snake structure
struct SnakeSegment {
    int x, y;
};

// Function prototypes
void spawnFood();
void update();
void render();
void handleInput();

// Global variables
SDL_Window* window;
SDL_Renderer* renderer;
std::vector<SnakeSegment> snake;
SnakeSegment food;
Direction snakeDirection = Direction::RIGHT; // Initialize the direction
int score = 0;

int main(int argc, char* args[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Create window and renderer
    window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Initialize random seed
    std::srand(static_cast<unsigned>(std::time(0)));

    // Initialize snake
    snake.push_back({SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2});

    // Initial food spawn
    spawnFood();

    // Main game loop
    bool quit = false;
    SDL_Event e;

    while (!quit) {
        // Handle events
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            handleInput();
        }

        // Update game state
        update();

        // Render graphics
        render();

        // Introduce a delay to control the frame rate
        SDL_Delay(MOVEMENT_DELAY);
    }

    // Cleanup and exit
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

void spawnFood() {
    int maxX = (SCREEN_WIDTH - 2 * TILE_SIZE) / TILE_SIZE;  // Maximum X index within walls
    int maxY = (SCREEN_HEIGHT - 2 * TILE_SIZE) / TILE_SIZE; // Maximum Y index within walls

    food.x = TILE_SIZE + rand() % maxX * TILE_SIZE; // Adjusted to consider walls
    food.y = TILE_SIZE + rand() % maxY * TILE_SIZE; // Adjusted to consider walls
}

void update() {
    // Move the snake
    SnakeSegment head = snake.front();
    switch (snakeDirection) {
        case Direction::UP:
            head.y -= TILE_SIZE;
            break;
        case Direction::DOWN:
            head.y += TILE_SIZE;
            break;
        case Direction::LEFT:
            head.x -= TILE_SIZE;
            break;
        case Direction::RIGHT:
            head.x += TILE_SIZE;
            break;
    }

    // Check for collisions with the wall
    if (head.x < TILE_SIZE || head.x >= SCREEN_WIDTH - TILE_SIZE ||
        head.y < TILE_SIZE || head.y >= SCREEN_HEIGHT - TILE_SIZE) {
        // Game over
        std::cout << "Game Over! Your score: " << score << std::endl;
        SDL_Quit();
        exit(0);
    }

    // Check for collisions with food
    if (head.x == food.x && head.y == food.y) {
        // Snake ate the food
        score += 10;
        spawnFood();
    } else {
        // Remove the tail segment
        snake.pop_back();
    }

    // Check for collisions with itself
    for (const auto& segment : snake) {
        if (head.x == segment.x && head.y == segment.y) {
            // Game over
            std::cout << "Game Over! Your score: " << score << std::endl;
            SDL_Quit();
            exit(0);
        }
    }

    // Add the new head to the snake
    snake.insert(snake.begin(), head);
}

void render() {
    // Clear the screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Render walls (borders)
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect topWall = {0, 0, SCREEN_WIDTH, TILE_SIZE};
    SDL_Rect bottomWall = {0, SCREEN_HEIGHT - TILE_SIZE, SCREEN_WIDTH, TILE_SIZE};
    SDL_Rect leftWall = {0, 0, TILE_SIZE, SCREEN_HEIGHT};
    SDL_Rect rightWall = {SCREEN_WIDTH - TILE_SIZE, 0, TILE_SIZE, SCREEN_HEIGHT};
    SDL_RenderFillRect(renderer, &topWall);
    SDL_RenderFillRect(renderer, &bottomWall);
    SDL_RenderFillRect(renderer, &leftWall);
    SDL_RenderFillRect(renderer, &rightWall);

    // Render snake
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    for (const auto& segment : snake) {
        SDL_Rect rect = {segment.x, segment.y, TILE_SIZE, TILE_SIZE};
        SDL_RenderFillRect(renderer, &rect);
    }

    // Render food
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect foodRect = {food.x, food.y, TILE_SIZE, TILE_SIZE};
    SDL_RenderFillRect(renderer, &foodRect);

    // Render score
    SDL_Color textColor = {255, 255, 255, 255};
    std::string scoreText = "Score: " + std::to_string(score);

    SDL_Surface* surface = SDL_CreateRGBSurface(0, 100, 20, 32, 0, 0, 0, 0);
    SDL_FillRect(surface, nullptr, SDL_MapRGB(surface->format, 0, 0, 0));

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect scoreRect = {10, 10, 100, 20};
    SDL_RenderCopy(renderer, texture, nullptr, &scoreRect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);

    // Present the renderer
    SDL_RenderPresent(renderer);
}

void handleInput() {
    const Uint8* currentKeyStates = SDL_GetKeyboardState(nullptr);

    if (currentKeyStates[SDL_SCANCODE_UP] && snakeDirection != Direction::DOWN) {
        snakeDirection = Direction::UP;
    } else if (currentKeyStates[SDL_SCANCODE_DOWN] && snakeDirection != Direction::UP) {
        snakeDirection = Direction::DOWN;
    } else if (currentKeyStates[SDL_SCANCODE_LEFT] && snakeDirection != Direction::RIGHT) {
        snakeDirection = Direction::LEFT;
    } else if (currentKeyStates[SDL_SCANCODE_RIGHT] && snakeDirection != Direction::LEFT) {
        snakeDirection = Direction::RIGHT;
    }
}
