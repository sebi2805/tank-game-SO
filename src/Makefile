CXXFLAGS = -Wall
LDFLAGS = -lncurses

all: TankGame

TankGame: main.o ./models/Tank.o ./models/Projectile.o Game.o
	g++ -Wall -o TankGame main.o ./models/Tank.o Game.o ./models/Projectile.o -lncurses -lrt -lpthread

main.o: main.cpp ./models/Tank.h ./models/Projectile.h game_functions.h
	$(CXX) $(CXXFLAGS) -c main.cpp

Game.o: game_functions.cpp game_functions.h
	$(CXX) $(CXXFLAGS) -c game_functions.cpp -o Game.o

./models/Tank.o: ./models/Tank.cpp ./models/Tank.h
	$(CXX) $(CXXFLAGS) -c ./models/Tank.cpp -o ./models/Tank.o

./models/Projectile.o: ./models/Projectile.cpp ./models/Projectile.h
	$(CXX) $(CXXFLAGS) -c ./models/Projectile.cpp -o ./models/Projectile.o

clean:
	rm -f TankGame *.o ./models/*.o
