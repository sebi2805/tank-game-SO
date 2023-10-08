CXXFLAGS = -Wall
LDFLAGS = -lncurses

all: TankGame

TankGame: main.o ./models/Tank.o ./models/Projectile.o
	$(CXX) $(CXXFLAGS) -o TankGame main.o ./models/Tank.o ./models/Projectile.o $(LDFLAGS)

main.o: main.cpp ./models/Tank.h ./models/Projectile.h
	$(CXX) $(CXXFLAGS) -c main.cpp

src/models/Tank.o: ./models/Tank.cpp src/models/Tank.h
	$(CXX) $(CXXFLAGS) -c ./models/Tank.cpp -o Tank.o

src/models/Projectile.o: ./models/Projectile.cpp src/models/Projectile.h
	$(CXX) $(CXXFLAGS) -c ./models/Projectile.cpp -o Projectile.o

clean:
	rm -f TankGame *.o src/models/*.o
