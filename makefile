LIBS=-lsfml-graphics -lsfml-window -lsfml-system

all:
	@echo "** Building the game"
	g++ arvindsGame.cpp $(LIBS) -o game
	i686-w64-mingw32-g++ arvindsGame.cpp $(LIBS) -o game.exe


        
