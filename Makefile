CXX = g++
CXXFLAGS = -std=c++11 -g -mtune=native -march=native -Wall -lpthread
assembly= -masm=intel -fverbose-asm -Wa,-alhnsd,-L

all:
	$(CXX) $(CXXFLAGS) -Ofast 2048.cpp -o 2048 $(assembly) > 2048.s

debug:
	$(CXX) $(CXXFLAGS) -O0 -g 2048.cpp -o 2048 $(assembly) > 2048.s

clean:
	rm -rf 2048

profile: clean all
	valgrind -v --tool=callgrind --dump-instr=yes --collect-jumps=yes --collect-systime=yes --collect-bus=yes --cache-sim=yes --branch-sim=yes ./bin/2048

