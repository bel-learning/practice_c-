#CXX=g++
#CXX=i586-mingw32msvc-g++
CXX=i686-w64-mingw32-g++
#LD=g++
#LD=i586-mingw32msvc-g++
LD=i686-w64-mingw32-g++

CFLAGS=-Wall -pedantic -Wno-long-long -O2

all: expr.exe

expr.exe: main.o expr.o tree.o
	$(LD) $^ -lgdi32 -mconsole -static-libgcc -static-libstdc++ -o $@
	
tree.o: tree.cpp tree.h
	$(CXX) $(CFLAGS) -c $<
	
expr.o: expr.cpp tree.h expr.h
	$(CXX) $(CFLAGS) -c $<
	
main.o: main.cpp tree.h expr.h
	$(CXX) $(CFLAGS) -c $<
	
clean:
	rm -rf expr.o tree.o main.o expr.exe
	