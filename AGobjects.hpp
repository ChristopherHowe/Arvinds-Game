#include <stdio.h>
#include <SFML/Graphics.hpp>
#include "AGsytems.hpp"

#ifndef ELEMENTS
#define ELEMENTS

namespace elements{
    using namespace systems;
	systemFunctions system;

    class Platform{
        private:
            sf::RectangleShape body;
            int paintX, paintY;
            int platformWidth, platformHeight;

        public:
            int lowerX, upperX, lowerY, upperY;
            int bounds[4];



            Platform(int x = 0, int y= 0, int width = 100, int height =  25): 
            body(sf::Vector2f(0,0))
            {
                platformWidth = width, platformHeight = height;
                body.setSize(sf::Vector2f(platformWidth, platformHeight));
                body.setFillColor(systems::Grey);
                body.setPosition(x,y);
            }
            void setBounds(){
                bounds[0] = paintX;
                bounds[1] = paintX + platformWidth;
                bounds[2] = paintY;
                bounds[3]=  paintY +platformHeight;
            }
            void paint(sf::RenderTarget* display){
                (*display).draw(body);
            }
            void setPos(int x, int y) //called in map constructor to set the paint location for an individual platform
            {
                paintX = x, paintY = y;
                //defines colision bounds
                lowerX = x, upperX = x + platformWidth, lowerY = y, upperY = y + platformHeight;
                body.setPosition(x,y);
                setBounds();
            }


    };

    class Target{
        private:
            sf::CircleShape body;
            int paintX, paintY;
            int targetRadius;
        public:
            int bounds[4];

            Target(int x = 0, int y = 0, float radius = 25): 
            body(radius,15)
            {
                paintX = x, paintY = y;
                targetRadius = radius;
                body.setRadius(radius);
                body.setFillColor(sf::Color::Green);
                body.setPosition(x,y);
            }
            void setBounds()
            {
                bounds[0] = paintX;
                bounds[1] = paintX + targetRadius * 2;
                bounds[2] = paintY;
                bounds[3]=  paintY + targetRadius * 2;
            }
            void paint(sf::RenderTarget* display)
            {
                (*display).draw(body);
            }
            void setPos(int x, int y) //called in map constructor to set the paint location for an individual platform
            {
                paintX = x, paintY = y;
                //defines colision bounds
                body.setPosition(x,y);
                setBounds();
            }
            void setColor(char color)
            {
                switch(color)
                {
                    case 'R':
                        body.setFillColor(sf::Color::Red);
                        break;
                }
            }
    };

    class Map{
        private:
            int numberOfPlatforms;
            int numberOfTargets;

        public:
            Platform platformArray[15];
            Target targetArray[15];
            int score = 0;

            Map(int platformMap[][2], int platformMapSize, int targetMap[][2], int targetMapSize)
            {
                numberOfPlatforms = platformMapSize;
                numberOfTargets = targetMapSize;
                resetPositions(platformMap, targetMap);
                     
            }
            void paint(sf::RenderTarget* display){
                for (int i = 0; i < numberOfPlatforms; i++){
                    platformArray[i].paint(display);
                }
                
                for (int i = 0; i < numberOfTargets; i++){
                    targetArray[i].paint(display);
                }        
            }
            void scoreUpdate(int elimPos[2]){
                for (int i = 0; i < numberOfTargets; i++)
                {
                    if (system.inBox(elimPos[0],elimPos[1],targetArray[i].bounds))
                    {
                        targetArray[i].setPos(-200,-200);
                        score ++;
                    }
                }
            }
            void resetPositions(int platformMap[][2], int targetMap[][2]){
                for (int i = 0; i < numberOfPlatforms; i++ ){
                    platformArray[i].setPos(platformMap[i][0],platformMap[i][1]);
                } 
                for (int i = 0; i < numberOfTargets; i++ ){
                    targetArray[i].setPos(targetMap[i][0],targetMap[i][1]);
                }        
            }
    };


}

#endif
