#include <stdio.h>
#include <SFML/Graphics.hpp>

#ifndef SYSTEM
#define SYSTEM


namespace systems
{
	sf::Color Grey(100,100,100);
	#define MICRO 1000000


	struct systemFunctions
	{
		//returns whether or not a cursor is within the box defined by upper and lower bounds
		bool inBox(float cursorX,float cursorY, int lowerX, int upperX, int lowerY, int upperY)
		{
			if (cursorX <= upperX && cursorX >= lowerX && cursorY <= upperY && cursorY >= lowerY){
				return true;    
			}
			else{
				return false;
			}
		}
		//determines if a point is within a bounding box
		bool inBox(int cursorX, int cursorY, int bounds[])
		{
			if (cursorX <= bounds[1] && cursorX >= bounds[0] && cursorY <= bounds[3] && cursorY >= bounds[2])
			{
				return true;    
			}
			else
			{
				return false;
			}
		}
		//returns whether or not two boxes overlap
		bool Overlap(int bound1[4], int bound2[4])
		{
			if (inBox(bound1[0], bound1[2], bound2)) // if lowerX and lower Y are within bound 2
			{
				return true;
			}
			else if (inBox(bound1[1], bound1[2], bound2)) // if upper X and lower Y are within bound 2
			{
				return true;
			}
			else if(inBox(bound1[0], bound1[3], bound2)) //if lower X and upper Y are within bound 2
			{
				return true;
			}
			else if(inBox(bound1[1], bound1[3], bound2)) // if upper X and upper Y are wthin bound 2
			{
				return true;
			}
			else if(inBox(bound2[0], bound2[2], bound1)) // if lower X and lower Y are wthin bound 1
			{
				return true;
			}
			else if (inBox(bound2[1], bound2[2], bound1)) // if upper X and lower Y are within bound 1
			{
				return true;
			}
			else if(inBox(bound2[0], bound2[3], bound1)) //if lower X and upper Y are within bound 1
			{
				return true;
			}
			else if(inBox(bound2[1], bound2[3], bound1)) // if upper X and upper Y are wthin bound 1
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		//creates a text surface that can be printed to the screen
		void printText(std::string inputText, int size, float printX, float printY, sf::RenderTarget* display)
		{
			
			//declares and loads a font, returns an error message if the font isnt loaded
			sf::Font font;
			if(!font.loadFromFile("assets/courier-prime.bold.ttf")){
				printf("font not loaded");
			}
			
			//declares a text surface and sets its attributes
			sf::Text text;
			text.setFont(font);
			text.setCharacterSize(size);
			text.setString(inputText);
			text.setPosition(printX, printY);

			//draws the text to the screen
			(*display).draw(text);
		}
		void printText2(std::string inputText, int size, float printX, float printY, char color, char orientation,  sf::RenderTarget* display)
		{
			
			//declares and loads a font, returns an error message if the font isnt loaded
			sf::Font font;
			if(!font.loadFromFile("assets/courier-prime.bold.ttf")){
				printf("font not loaded");
			}
			
			//declares a text surface and sets its attributes
			sf::Text text;
			text.setFont(font);
			text.setCharacterSize(size);
			text.setString(inputText);
			
			int textWidth = text.getLocalBounds().width;

			switch(orientation)
			{
				case 'c':
					text.setPosition(printX - textWidth / 2, printY);
					break;
				default:
					text.setPosition(printX, printY);
					break;
			}

			switch(color)
			{
				case 'g':
					text.setFillColor(sf::Color::Green);
					break;
				case 'b':
					text.setFillColor(sf::Color::Blue);
					break;
				case 'r':
					text.setFillColor(sf::Color::Red);
					break;
				default:
					text.setFillColor(sf::Color::White);
					break;

			}

			//draws the text to the screen
			(*display).draw(text);
		}
		//gets how much time has elapsed since the last window update
		float getTick(sf::Clock* clock)
		{
			//restarts the clock
			sf::Time t1 = (*clock).restart();
			//converts time type to int microseconds
			int tick = t1.asMicroseconds();
			//returns the int
			return tick;
		}
		//gets the frames per second depending on how long it takes for one loop to occur
		double getFPS(int tick)
		{
			double fpsTick = tick;
			return 1 / (fpsTick/1000000);
		}
		//gets the average number of frames per second that have occured since the game clock has restart
		double getAverageFPS(sf::Clock gameClock, int tick, double fps, double average)
		{
			sf::Time gameTime = gameClock.getElapsedTime();
			double gameMicroSeconds = gameTime.asMicroseconds();
			double tickTimeRatio = tick / gameMicroSeconds;
			average = average * (1 - tickTimeRatio) + fps * tickTimeRatio;
			return average;
		}
		//prints the bounds of two objects to see if they are overlapping
		void printBounds(int bound1[], int bound2[])
		{
			printf("bounds are: {");
			for(int i = 0; i < 4; i++){
				printf("%d,", bound1[i]);
			}
			printf("}, {");
			for(int i = 0; i < 4; i++){
				printf("%d,", bound2[i]);
			}
			printf("}\n");
		}
		//displays the amount of time that has passed since the clock has been reset
		void displayTime(int size, int x, int y, sf::Clock gameClock, sf::RenderTarget* display)
		{
			sf::Time gameTime = gameClock.getElapsedTime();
			int gameSeconds = gameTime.asSeconds();
			int gameMinutes = gameSeconds / 60;
			gameSeconds = gameSeconds % 60;
			char timeString[8];
			
			if(gameSeconds < 10)
			{
				sprintf(timeString,"%d:0%d\n",gameMinutes,gameSeconds);
			}
			else
			{
				sprintf(timeString,"%d:%d\n",gameMinutes,gameSeconds);
			}
			
			printText(timeString,size,x,y,display); 
		}
		void printEndTime(int size, int x, int y, sf::Time endTime, sf::RenderTarget* display)
		{
			int endSeconds = endTime.asSeconds();
			int endMinutes = endSeconds / 60;
			endSeconds = endSeconds % 60;
			char timeString[8];
			
			if(endSeconds < 10)
			{
				sprintf(timeString,"%d:0%d\n",endMinutes,endSeconds);
			}
			else
			{
				sprintf(timeString,"%d:%d\n",endMinutes,endSeconds);
			}
			
			printText(timeString,size,x,y,display); 
		}
	};

}

#endif
