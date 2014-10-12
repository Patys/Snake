#include <SFML/Graphics.hpp>
#include <vector>
#include <ctime>
#include <iostream>

enum GAME_STATE {PLAYING, MENU};
enum DIRECTION {NORTH, SOUTH, EAST, WEST};

struct snake_segment {
  int x, y; //position
  snake_segment(int x, int y): x(x), y(y) {}
};

struct food{
  int x, y; //position
  int points; //points for player
  food(int x, int y, int points): x(x), y(y), points(points) {}
};

std::vector<snake_segment> snake_segments;

void init(){
  snake_segments.push_back(snake_segment(1,3));
}

int main(){
  sf::RenderWindow window(sf::VideoMode(800,600), "Snake");
  srand(time(0));
  init();
  
  sf::Texture tileset;
  tileset.loadFromFile("data/graphic.png");
  
  sf::Sprite spr_snake_seg(tileset);
  spr_snake_seg.setTextureRect(sf::IntRect(0,0,32,32));

  sf::Sprite spr_food(tileset);
  spr_food.setTextureRect(sf::IntRect(32,0,32,32));

  GAME_STATE game_state;
  game_state = GAME_STATE::MENU;

  DIRECTION dir = DIRECTION::SOUTH;

  food foods(1,1,5);

  sf::Clock game_clock;
  int score = 0;

  while(window.isOpen()){
     sf::Event event;
     while (window.pollEvent(event)){
       //Close window : exit
       if (event.type == sf::Event::Closed)
	 window.close();
     }
     if(game_state == GAME_STATE::PLAYING){
       if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
	 if(dir != DIRECTION::SOUTH)
	   dir = DIRECTION::NORTH;
       }
       if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
	 if(dir != DIRECTION::NORTH)
	   dir = DIRECTION::SOUTH;
       }
       if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
	 if(dir != DIRECTION::EAST)
	   dir = DIRECTION::WEST; 
       }
       if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
	 if(dir != DIRECTION::WEST)
	   dir = DIRECTION::EAST;
       }
       
       /*if(snake_segments[0].x + dir_x < 0 || snake_segments[0].x + dir_y > 20 ||
	  snake_segments[0].y + dir_y < 0 || snake_segments[0].y + dir_y > 20){
	 game_state = GAME_STATE::MENU;
	 }*/
       
       if(game_clock.getElapsedTime() > sf::milliseconds(500)){

	 if(foods.x == snake_segments.front().x &&
	    foods.y == snake_segments.front().y){
	   score += foods.points;

	   snake_segments.push_back(snake_segment(0,0));

	   foods.x = rand()%10+1;
	   foods.y = rand()%10+1;
	 }
	 
	 for(size_t i = snake_segments.size(); i-- > 1;){
	     snake_segments[i].x = snake_segments[i-1].x;
	     snake_segments[i].y = snake_segments[i-1].y;
	 }
	 
	 switch(dir){
	 case DIRECTION::NORTH:
	   snake_segments.front().y -= 1;
	   break;
	 case DIRECTION::SOUTH:
	   snake_segments.front().y += 1;
	   break;
	 case DIRECTION::WEST:
	   snake_segments.front().x -= 1;
	   break;
	 case DIRECTION::EAST:
	   snake_segments.front().x += 1;
	 }
	 game_clock.restart();
       }


       //DRAWING
       window.clear();
       for(size_t i = 0; i < snake_segments.size(); i++){
	 spr_snake_seg.setPosition(snake_segments[i].x * 32, snake_segments[i].y * 32);
	 window.draw(spr_snake_seg);
       }
       spr_food.setPosition(foods.x * 32, foods.y * 32);
       window.draw(spr_food);
       window.display();
     } //if:GAME_STATE::PLAY
     if(game_state == GAME_STATE::MENU){
       if(sf::Keyboard::isKeyPressed(sf::Keyboard::E)){
	 game_state = GAME_STATE::PLAYING;
       }

       window.clear();
       window.display();
     } //if:GAME_STATE::MENU
  }
}
