//preprocessor directives
#include "AGsytems.hpp"
#include <SFML/Graphics.hpp>
#include <cstring>

//function prototypes
float getAverageWidth(char inputString[]);

namespace GUI{

    using namespace systems;
    systemFunctions system;

    class Slider{
		private:
			//defines all surfaces for the slider
			//handle
			sf::RectangleShape activeSquare;
			//background
			sf::RectangleShape passiveElement;
			//blackbar
			sf::RectangleShape measurement;
			
			float val, valMax, valMin;
			int paintX, paintY;
			int AX,AY,PX,PY,MX,MY;
			bool handleGrabbed = true;
			int activeX;
		
		public:
			//construction parameters, all are optional except first x and y
			Slider(int drawX, int drawY, float minVal = 0, float maxVal = 100, int aX = 25, int aY = 75, int pX = 300, int pY = 100, int mX = 300, int mY = 10):
				//member instanization list
				activeSquare{sf::Vector2f(aX, aY)}, passiveElement{sf::Vector2f(pX, pY)}, measurement{sf::Vector2f(mX, mY)} 
			{
				//sets colors 
				passiveElement.setFillColor(Grey);
				measurement.setFillColor(sf::Color::Black);

				//sets initial value
				val = (maxVal-minVal)/2 + minVal;

				//assigns paramters to fields
				paintX = drawX - pX/2, paintY = drawY;
				AX = aX, AY = aY, PX = pX, PY = pY, MX = mX, MY = mY;
				valMin = minVal, valMax = maxVal;
				
				activeX = paintX + MX*(val/maxVal) - AX/2;
				
			}

			float moveHandle(int CursorX, int CursorY, bool click, int mouseXVelo){
				
				//determines if the handle is grabbed and sets the handdlegrabbed variable
				if (system.inBox(CursorX, CursorY, activeX, activeX + AX, paintY+(PY-AY)/2, AY + paintY) && click){
					handleGrabbed = true;
				}
				else{
						handleGrabbed = false;
				}

				if (handleGrabbed){			
					activeX += mouseXVelo/1.25;
					val = (activeX - paintX + AX/2) * valMax / MX;
				}

				return val;
			}

			void paint(sf::RenderTarget* Window){

				passiveElement.setPosition(paintX, paintY);
				measurement.setPosition(paintX, paintY+PY/2-MY/2);
				activeSquare.setPosition(activeX, paintY + (PY-AY)/2);

				(*Window).draw(passiveElement);
				(*Window).draw(measurement);
				(*Window).draw(activeSquare);
			}
			void setPos(int newX, int newY){
				paintX = newX;
				paintY = newY;
			}

	};

	class Button{
		
		private:
			sf::RectangleShape square;
			int width, height;
			bool pressed;
            std::string buttonText;
            int paintX, paintY;
			sf::Text text;
			sf::Font font;
		
		public:
			Button(int x = 0, int y = 0, std::string inputText = "Button", char orientation = 'c', int w = 300, int h = 60): 
			square (sf::Vector2f(w,h)), text(), font(){
				width = w, height = h;
				buttonText = inputText;

				//determines where to paint the button depending on if the user wants the button centered
				switch(orientation){ 
					case 'c':
						paintX = x - w/2;
						paintY = y;
						break;
					default:
						paintX = x;
						paintY = y;
						break;
				}
				

				//changes properties of the button box
				square.setPosition(paintX,paintY);

				//converting std::string to char list
				char buffer[20];
				strcpy(buffer, buttonText.c_str());
				text.setString(buffer);	

				//font for button
				if(!font.loadFromFile("assets/courier-prime.bold.ttf")){
					printf("font not loaded");
				}
	
				//Sets up the text so it can be adjusted to fit the button
				float fontCharacterSize = height;
				text.setFont(font);
				text.setCharacterSize(fontCharacterSize);
				text.setFillColor(sf::Color::Black);

				//resizes the font so that it is equal to the height of the button box
				sf::FloatRect bounds = text.getLocalBounds();
				float fontOffset = bounds.height / height; //equal to the ratio between the actual Height of the font(litteH) divided by the whitespace above the font(bigH)
				fontCharacterSize = height / fontOffset;
				
				text.setCharacterSize(fontCharacterSize);
				
				//shrinks the text if it is larger than the box
				bounds = text.getLocalBounds();				
				if (bounds.width > width){
					float HperWratio = bounds.height / bounds.width; // ratio between littleH and the width of the text
					float littleH = width * HperWratio;              //actual dimension of the text height
					fontCharacterSize =  littleH / fontOffset;       //BigH aka the size the text is told to change to after including the new height
					text.setCharacterSize(fontCharacterSize);        //updates the size of the text
					bounds = text.getLocalBounds();                  //updates the size of the bounds so the new bounds can be used in the code
				}

				//corrects the position of the text to account for whitespace at top, the inputted paint coordinates, and centers the text
				float fontShift = fontCharacterSize * (1 - fontOffset);                     //accounts for the whitespace above the font
				float centerYShift = (height - bounds.height) / 2;                          //centers the text in the y;
				float centerXshift = (width - bounds.width) / 2;                            //centers the text in the x;
				
				text.setPosition(paintX + centerXshift, paintY + centerYShift - fontShift); // updates the position of the text				
			}


		void paint(sf::RenderTarget* display){
			(*display).draw(square);     
            (*display).draw(text);
		}

		bool isClicked(int mouseX, int mouseY, bool click){
			if (system.inBox(mouseX, mouseY, paintX, paintX + width, paintY, paintY + height) && click){
				return true;
			}
			else{
				return false;
			}
		}
	};
	void drawPopup(int width, int height, int border, int sWidth, int sHeight, sf::RenderTarget* display){
		sf::RectangleShape endCard(sf::Vector2f(width,height));
		sf::RectangleShape endCardBorder(sf::Vector2f(width + border, height + border));
		
		endCard.setPosition(sWidth/2 - width/2, sHeight/2 - height/2);
		endCardBorder.setPosition(sWidth/2 - (width + border)/2, sHeight/2 - (height+border)/2);

		endCard.setFillColor({150,150,150});
		endCardBorder.setFillColor({75,75,75});

		(*display).draw(endCardBorder);
		(*display).draw(endCard);
	}

}

//gets the average width of one character depending on the font
float getAverageWidth(char inputString[]){
	sf::Font font;
	if(!font.loadFromFile("assets/courier-prime.bold.ttf")){
					printf("font not loaded");
				}
	sf::Text text;
	text.setFont(font);
	text.setCharacterSize(50);
	
	int sumOfWidths = 0;

	for(int i  = 0; i < strlen(inputString); i++){
		text.setString(inputString[i]);
		float width = text.getLocalBounds().width;
		sumOfWidths += width;
	}
	printf("string length is %ld\n", strlen(inputString));
	float averageWidth = sumOfWidths / strlen(inputString);
	return averageWidth;
}


	
