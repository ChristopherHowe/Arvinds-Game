//preprocessor directives
#include <stdio.h>
#include <SFML/Graphics.hpp>
#include "AGCharacters.hpp"
#include "CPPGUI.hpp"
#include "AGobjects.hpp"

//macros
#define SCREENWIDTH 800
#define SCREENHEIGHT 600
#define LEVELSIZE 10

//namespace definitions
using namespace characters;
using namespace systems;
using namespace GUI;
using namespace elements;


//implicit definitions
void loadLevel(int levelIndex, int pMap[][2],int tMap[][2], int* pSize, int* tSize);

//main function
int main(){
	
	//struct with all system functions
	systemFunctions system;
	
	//time variables
	sf::Clock clock;
	sf::Clock tickClock;
	double averageFPS;
	sf::Time endTime;

	//generates window
	sf::RenderWindow window(sf::VideoMode(SCREENWIDTH, SCREENHEIGHT), "Arvinds Game");

	//mouse variables
	int mouseX, mouseY;
	bool click = false;	
	int mVX, mVY; //mouse speed variables

	//changable constants;
	float gravity = 300;
	//game state variables;
	std::string screen = "menu";
	
	//level definition
	int platformMap[LEVELSIZE][2] = {{231,530},{482,432},{269,256},{559,189},{356,73},{664,540},{118,252}};
	int platformMapSize = 7;
	int targetMap[LEVELSIZE][2] = {{31,110},{750,60},{730,280},{190,380},{450,280}};
	int targetMapSize = 5;

	//class instances
		//game objects
	character John(gravity);
	Map map1(platformMap, platformMapSize,targetMap, targetMapSize);
	
		//start menu objects
	Slider gravitySlider(SCREENWIDTH/2,400,0,1000);
	Button startButton( SCREENWIDTH/2 ,50, "start");
	Button playAgain(SCREENWIDTH/2 ,300,"Play again?",'c',250);	

    while (window.isOpen()){
		int tick = system.getTick(&tickClock);
		double fps = system.getFPS(tick);
		averageFPS = system.getAverageFPS(clock, tick, fps, averageFPS);

		//Event Loop
		sf::Event event;
        while (window.pollEvent(event)){
			switch(event.type){
				
				//closes window if x is pressed
				case sf::Event::Closed:
					window.close();
					break;

				//events control for in game

				if (screen == "game"){
					//all control inputs
					case sf::Event::KeyPressed:	
						switch(event.key.code){
							case sf::Keyboard::W:
								John.jump();
								break;
							case sf::Keyboard::D:
								John.moveRight();
								break;
							case sf::Keyboard::A:
								John.moveLeft();
								break;
							case sf::Keyboard::K:
								John.attackL = true;
								break;
							case sf::Keyboard::L:
								John.attackR = true;
								break;
						}
						break;
					case sf::Event::KeyReleased:
						switch(event.key.code)
						{
							case sf::Keyboard::D:
								John.stopMoving();
								break;
							case sf::Keyboard::A:
								John.stopMoving();
								break;
							case sf::Keyboard::K:
								John.attackL = false;
								break;
							case sf::Keyboard::L:
								John.attackR = false;
								break;
						}
						break;
						
				}

				//gets mouse events stored in click, mouseX, and mouseY	
				case sf::Event::MouseButtonPressed:
					click  = true;
					break;
				
				case sf::Event::MouseButtonReleased:
					click = false;
					break;
				
				case sf::Event::MouseMoved:	
					
					//gets speed of mouse since last frame
					//v = final - initial
					mVX =   event.mouseMove.x - mouseX;
					mVY =   event.mouseMove.y - mouseY;

					//sets mouse position variables;
					mouseX = event.mouseMove.x;
					mouseY = event.mouseMove.y;				
			}
		}
		//start of update
        window.clear();

		//in game

		if (screen == "game"){
				//logic update
				John.move(tick, map1.platformArray, platformMapSize);
				map1.scoreUpdate(John.swordPos);

				//end game
				if(map1.score == targetMapSize){
					 endTime = clock.getElapsedTime();
					 screen = "end";

				}
				
				//score and frames per second
				char scoreText[9];
				sprintf(scoreText, "Score: %d" ,map1.score);
				char fpsText[15];
				sprintf(fpsText, "FPS: %.2lf",averageFPS);

				//painting
				system.printText(fpsText, 20, SCREENWIDTH-150, 0, &window);
				system.printText(scoreText, 30, 0, 0, &window);
				system.displayTime(30, 400, 0, clock, &window);
				map1.paint(&window);
				John.paint(&window);
		}

		//menu screen

		else if (screen == "menu"){
				//logic update
				if(startButton.isClicked(mouseX,mouseY,click)){
					screen = "game";
					clock.restart();
					averageFPS = 0;
				}
				John.gravity = gravitySlider.moveHandle(mouseX, mouseY, click,mVX);

				//painting
				system.printText("Gravity", 60, 275, 300, &window);
				gravitySlider.paint(&window);
				startButton.paint(&window);



		}
		else if (screen == "end"){
				char scoreText[9];
				sprintf(scoreText, "Score: %d" ,map1.score);
				char fpsText[15];
				sprintf(fpsText, "FPS: %.2lf",averageFPS);
				
				system.printText(fpsText, 20, SCREENWIDTH-150, 0, &window);
				system.printText(scoreText, 30, 0, 0, &window);
				system.printEndTime(30, 400, 0, endTime, &window);
				
				map1.paint(&window);
				John.paint(&window);
				
				drawPopup(300,200,50,SCREENWIDTH,SCREENHEIGHT,&window);
				system.printText2("You Win!",60, SCREENWIDTH/2 , 200,'g','c', &window);	
				playAgain.paint(&window);
				if (playAgain.isClicked(mouseX,mouseY,click)){
					map1.resetPositions(platformMap,targetMap);
					screen = "menu";
					map1.score = 0;
					John.objectX = 0, John.objectY = SCREENHEIGHT - John.objectHeight;
				}

		}
		
		//pushes update
        window.display();
	}
	return 0;
}