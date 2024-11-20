#include <stdio.h>
#include <math.h>
#include <SFML/Graphics.hpp>
#include "AGsytems.hpp"
#include "AGobjects.hpp"

#define SCREENHEIGHT 600


namespace characters {
	using namespace elements;
	using namespace systems;
	systemFunctions system;

	#define tickFrequency 500
	#define MICRO 1000000

	//object base class that includes important data
	class object{
		
		private:
			float yVelo1, objectY1;


		public:	
			float objectX = 0, objectY = SCREENHEIGHT - 150;
			float yVelo = 0, xVelo = 0;
			bool grounded = false;
			float gravity;
			float objectHeight = 111;
			float objectWidth = 222;
			//xLower, xUpper, yLower, YUpper
			int bounds[4] = {0,0,0,0};
			int newGroundLevel = 0;
			bool collided = false;


			


			//constructor with correct vals
			object(float gravityVal){
				gravity = gravityVal;

			}
			void setHeight(float w, float h){
				objectWidth = w;
				objectHeight = h;
			}
			//changes the value of grounded depending on if the object is on a surface
			void onsurface(Platform array[],int arraySize){
				
				//checks to see if the object is overlapping a platform
				for (int i = 0; i < arraySize; i++)
				{
					if (system.Overlap(bounds, array[i].bounds)) 
					{
						newGroundLevel = array[i].bounds[2];
						collided = true;
						break;
					}
					else
					{
						collided = false;
					}
				}

				//sets the value of grounded if the object is on the bottom of the map or if it has collided with a box

				if (objectY >= SCREENHEIGHT - objectHeight) //if object is on the bottom of the map
				{
					objectY = SCREENHEIGHT - objectHeight;
					grounded = true;
				}
				else if(collided) //if the object collided
				{
					grounded = true;
					objectY = newGroundLevel - objectHeight;
				}
				else{
					grounded = false;
				}
				
			}	
			
			//updates the objects position 
			void move(int tic, Platform array[], int arraySize){
				setBounds();


				objectY += yVelo * tic / MICRO;
				objectX += xVelo * tic / MICRO;
				
				if (!grounded){
					fall(tic);
				}
				
				onsurface(array,arraySize);

				if (grounded){
					yVelo = 0;
				}
			}
			
			//linearly decreases yVelo
			void fall(int tic){
				yVelo += gravity * tic / MICRO;
			}

			void setBounds(){
				bounds[0] = objectX;                //lowerX
				bounds[1] =  objectX + objectWidth; //UpperX
				bounds[2] = objectY;                 //lowerY
				bounds[3] = objectY + objectHeight;  //UpperY
			}
	};

	class character : public object {
		private:
			sf::RectangleShape icon;
			sf::RectangleShape sword;
			float iconWidth = 100, iconHeight = 150;
			float swordWidth = 75, swordHeight = 20;

		public:
			bool attackL = false, attackR = false;
			int swordPos[2]; // {tip X, tip Y}

			character(float gravityVal) : 
			icon(sf::Vector2f(0,0)), sword(sf::Vector2f(0,0)), object(gravityVal)
			{
				icon.setSize(sf::Vector2f(iconWidth,iconHeight));
				sword.setSize(sf::Vector2f(swordWidth,swordHeight));
				sword.setFillColor(sf::Color::Red);
				setHeight(iconWidth, iconHeight);
			}

			void paint(sf::RenderTarget* display)
			{
				
				icon.setPosition(objectX, objectY);
				(*display).draw(icon);

				if (attackR){
					sword.setPosition(objectX + iconWidth, objectY + iconHeight/2);
					(*display).draw(sword);
					setSwordPos('R'); //sets the position of the swords tip
					
				}
				else if (attackL){
					sword.setPosition(objectX + - swordWidth, objectY + iconHeight/2);
					(*display).draw(sword);
					setSwordPos('L'); //sets the position of the swords tip
				}
				else{
					setSwordPos('N');
				}				
			}
			void jump()
			{
				if (grounded){
					yVelo = -200;
					objectY -= 3;
					collided = false;
				}
			}
			void moveRight()
			{
				xVelo = 100;
			}
			void moveLeft()
			{
				xVelo = -100;
			}
			void stopMoving()
			{
				xVelo = 0;
			}
			void setSwordPos(char direction){
				if (direction == 'R'){
					swordPos[0] = objectX + iconWidth + swordWidth;
					swordPos[1] = objectY + iconHeight/2 + swordHeight / 2;
				}
				else if(direction == 'L'){
					swordPos[0] = objectX - swordWidth;
					swordPos[1] = objectY + iconHeight/2 + swordHeight / 2;
				}
				else if(direction == 'N'){
					swordPos[0] = 0;
					swordPos[1] = 0;
				}
				else{
					printf("sword direction is incorrect");
				}
			}
	};

}

