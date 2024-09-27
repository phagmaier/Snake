#include <iostream>
#include "raylib.h"
#include <utility>
#include <random>
#include <ctime>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>

#define BLOCKSIZE 10
#define SCREENH 600
#define SCREENW 600

using snakeVec = std::vector<std::pair<int,int>>;

enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

std::unordered_map<Direction, std::pair<int, int>> dirDic =
{
    {UP, {0, -BLOCKSIZE}},
    {DOWN, {0, BLOCKSIZE}},
    {LEFT, {-BLOCKSIZE, 0}},
    {RIGHT, {BLOCKSIZE, 0}}
};

Direction get_dir(Direction curr) {
    int key = GetKeyPressed();

    if (key == KEY_RIGHT && curr != LEFT) {
        return RIGHT;
    }
    if (key == KEY_LEFT && curr != RIGHT) {
        return LEFT;
    }
    if (key == KEY_UP && curr != DOWN) {
        return UP;
    }
    if (key == KEY_DOWN && curr != UP) {
        return DOWN;
    }
    return curr;
}

snakeVec init_position(){
    snakeVec vec;
    for (int i = 0; i < SCREENH; i += BLOCKSIZE) {
        for (int x = 0; x < SCREENW; x += BLOCKSIZE) {
            vec.push_back({x, i});  
        }
    }
    return vec;
}


void delete_element(snakeVec &vec, const std::pair<int,int> &val){
  auto it = std::remove(vec.begin(), vec.end(), val);
    vec.erase(it, vec.end());
}

void random_food(snakeVec &vec, std::pair<int,int> &food){
    static std::random_device rd;   // Non-deterministic random number generator
    static std::mt19937 gen(rd());  // Mersenne Twister engine
    std::uniform_int_distribution<> dis(0, vec.size()- 1);
    std::pair<int,int> tmp = food;
    food = vec[dis(gen)];
    delete_element(vec, food);
    vec.push_back(tmp);
}

void drawfood(const std::pair<int,int> &food){
  DrawRectangle(food.first, food.second, BLOCKSIZE,BLOCKSIZE,RED);
}

void drawSnake(const snakeVec &snake){
  for (std::pair<int,int> segment: snake){
    DrawRectangle(segment.first, segment.second, BLOCKSIZE, BLOCKSIZE, BLACK);
  }
}

bool update_snake(snakeVec &snake, Direction dir, std::pair<int,int> &food, snakeVec &free_squares, int &score){
  std::pair<int, int> head = snake.front();
  std::pair<int, int> change = dirDic[dir];
  head.first += change.first;
  head.second += change.second;
  bool over = false;
  if (head.first >= SCREENW || head.first < 0 || head.second >= SCREENH || head.second < 0) {
    return true;
  }
  for (const auto& segment : snake) {
    if (head == segment){
      over = true;
    } 
  }
  snake.insert(snake.begin(), head);
  if (head == food) {
    random_food(free_squares, food);  // Generate new food
    ++score;
  } 
  else {
    snake.pop_back(); // Remove the last segment of the snake (tail) unless it eats the food
  }

  for (const auto& segment : snake) {
    DrawRectangle(segment.first, segment.second, BLOCKSIZE, BLOCKSIZE, BLACK);
  }
  return over;
}

void resetGame(snakeVec &snake, Direction &dir, std::pair<int,int> &food, snakeVec &free_vec, int &score){
  snake.clear();
  snake.push_back({0,0});
  dir = DOWN;
  food = {300,300};
  free_vec.clear();
  free_vec = init_position();
  delete_element(free_vec, {0,0});
  delete_element(free_vec, food);
  score = 0;
}


int main(){
    int counter = 0;
    bool gameOver = false;
    int score = 0;
    std::pair<int, int> food = {300,300};
    Direction dir = DOWN;
    snakeVec snake = {{0,0}};
    snakeVec free_vec = init_position();
    delete_element(free_vec, {0,0});
    delete_element(free_vec, food);


    InitWindow(SCREENW, SCREENH, "SNAKE");
    SetTargetFPS(60);

    Rectangle newGameBtn = { SCREENW / 2 - 150, SCREENH / 2 + 100, 400, 100 };    
    
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        if (!gameOver) {
          dir = get_dir(dir);
          if (counter == 2){
            gameOver = update_snake(snake, dir, food, free_vec,score);
            counter = 0;
          }
          drawSnake(snake);
          drawfood(food);
          DrawText(("Score: " + std::to_string(score)).c_str(), 10, 10, 20, GREEN);
          ++counter;
        } 
        
        else {
          DrawText("Game Over!", SCREENW / 2 - 80, SCREENH / 2, 30, RED);
          counter = 0;

          // Draw the button background with a gradient effect
          Color lightGray = Color{200, 200, 200, 255};
          Color darkGray = Color{100, 100, 100, 255};
          DrawRectangleGradientV(newGameBtn.x, newGameBtn.y, newGameBtn.width, newGameBtn.height, lightGray, darkGray);
          DrawRectangleLinesEx(newGameBtn, 2, BLACK);

          // Draw button text
          DrawText("PRESS ENTER FOR NEW GAME", newGameBtn.x + 25, newGameBtn.y + 35, 20, BLACK);

          // Check for Enter key press to restart the game
          if (IsKeyPressed(KEY_ENTER)) {
              resetGame(snake, dir, food, free_vec, score);
              gameOver = false;
          }
        }
        
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

