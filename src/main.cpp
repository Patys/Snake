#include <SFML/Graphics.hpp>
#include <vector>
#include <ctime>
#include <sstream>
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

int main(){
  sf::RenderWindow window(sf::VideoMode(320,320),"Snake", sf::Style::Close);
  window.setFramerateLimit(60);
  srand(time(0));
  
  sf::Texture tileset;
  tileset.loadFromFile("data/graphic.png");
  
  sf::Sprite spr_snake_seg(tileset);
  spr_snake_seg.setTextureRect(sf::IntRect(0,0,16,16));

  sf::Sprite spr_food(tileset);
  spr_food.setTextureRect(sf::IntRect(32,0,16,16));
  
  sf::Font font;
  font.loadFromFile("data/Russo_One.ttf");
  
  sf::Text txt_score;
  txt_score.setFont(font);

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
       
       if(snake_segments.front().x < 0 || snake_segments.front().x > 20 ||
	  snake_segments.front().y < 0 || snake_segments.front().y > 20){
	 game_state = GAME_STATE::MENU;
       }
       

       if(game_clock.getElapsedTime() > sf::milliseconds(400)){

	 if(foods.x == snake_segments.front().x &&
	    foods.y == snake_segments.front().y){
	   score += foods.points;

	   snake_segments.push_back(snake_segment(0,0));

	   foods.x = rand()%19+1;
	   foods.y = rand()%19+1;
	   foods.points = rand()%20+1;
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

       for(size_t i = 1; i < snake_segments.size(); i++){
	 if(snake_segments.front().x == snake_segments[i].x &&
	    snake_segments.front().y == snake_segments[i].y){
	   game_state = GAME_STATE::MENU;
	 }
       }

       /**< showing score */
       std::ostringstream _score_string;
       _score_string << score;

       txt_score.setString( _score_string.str());
       sf::FloatRect textRect = txt_score.getLocalBounds();
       txt_score.setOrigin(textRect.left + textRect.width/2.0f,
			   textRect.top  + textRect.height/2.0f);
       txt_score.setPosition(sf::Vector2f(400,textRect.top  + textRect.height/2.0f + 10));

       //DRAWING
       window.clear(sf::Color::Blue);
       for(size_t i = 0; i < snake_segments.size(); i++){
	 spr_snake_seg.setPosition(snake_segments[i].x * 16, snake_segments[i].y * 16);
	 window.draw(spr_snake_seg);
       }
       spr_food.setPosition(foods.x * 16, foods.y * 16);
       window.draw(spr_food);
       window.draw(txt_score);
       window.display();
     } //if:GAME_STATE::PLAY
     if(game_state == GAME_STATE::MENU){
       if(sf::Keyboard::isKeyPressed(sf::Keyboard::E)){
	 game_state = GAME_STATE::PLAYING;
	 snake_segments.clear();
	 snake_segments.push_back(snake_segment(5,5));
       }

       window.clear();
       window.display();
     } //if:GAME_STATE::MENU
  }
}
