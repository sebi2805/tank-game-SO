CXXFLAGS = -Wall
LDFLAGS = -lncurses

all: TankGame

TankGame: main.o Game.o ./models/Tank.o ./models/Projectile.o
	$(CXX) $(CXXFLAGS) -o TankGame main.o Game.o ./models/Tank.o ./models/Projectile.o $(LDFLAGS)

main.o: main.cpp ./models/Tank.h ./models/Projectile.h Game.h
	$(CXX) $(CXXFLAGS) -c main.cpp

Game.o: Game.cpp Game.h
	$(CXX) $(CXXFLAGS) -c Game.cpp

./models/Tank.o: ./models/Tank.cpp ./models/Tank.h
	$(CXX) $(CXXFLAGS) -c ./models/Tank.cpp -o ./models/Tank.o

./models/Projectile.o: ./models/Projectile.cpp ./models/Projectile.h
	$(CXX) $(CXXFLAGS) -c ./models/Projectile.cpp -o ./models/Projectile.o

clean:
	rm -f TankGame *.o ./models/*.o
