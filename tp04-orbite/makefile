CXXFLAGS += -I/usr/X11/include -I/opt/local/include -W -Wall -Wno-unused-parameter -Wno-deprecated-declarations -std=c++11
LDFLAGS += -L/usr/X11/lib -L/opt/local/lib

SRC = graph.cpp main.cpp fctsUtiles.cpp
all : main.exe
main.exe : $(SRC:.cpp=.o)
	$(CXX) $(LDFLAGS) -o$@ $(SRC:.cpp=.o) -lglut -lGLU -lGL -lGLEW

SOL = main_solution.cpp fctsUtiles.cpp
sol : main_solution.exe
main_solution.exe : $(SOL:.cpp=.o)
	$(CXX) $(LDFLAGS) -o $@ $(SOL:.cpp=.o) -lglut -lGLU -lGL -lGLEW

clean:
	-\rm *.o *.exe

fctsUtiles.o: fctsUtiles.cpp fctsUtiles.h
main.o: main.cpp varglob.h float3.h matrice.h fctsUtiles.h graph.h
main_solution.o: main_solution.cpp varglob.h float3.h matrice.h \
 fctsUtiles.h
