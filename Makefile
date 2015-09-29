CXX = g++
CXXFLAGS = -std=c++11 -g -mtune=native -march=native -Wall -lpthread
assembly= -masm=intel -fverbose-asm -Wa,-alhnsd,-L

all:
	$(CXX) $(CXXFLAGS) -Ofast 2048.cpp -o 2048 $(assembly) > 2048.s
	$(CXX) $(CXXFLAGS) -Ofast benchmark.cpp -o bench $(assembly) > bench.s

debug:
	$(CXX) $(CXXFLAGS) -O0 2048.cpp -o 2048 $(assembly) > 2048.s
	$(CXX) $(CXXFLAGS) -O0 benchmark.cpp -o bench $(assembly) > bench.s

clean:
	rm -rf 2048 bench *.s *out*

profile: clean all
	valgrind -v --log-fd=1 --tool=callgrind --dump-instr=yes --dump-line=yes --collect-jumps=yes --collect-systime=yes --collect-bus=yes --cache-sim=yes --branch-sim=yes --simulate-hwpref=yes --cacheuse=yes ./2048

leakcheck: clean all
	valgrind -v --log-fd=1 --leak-check=full --show-leak-kinds=all --trace-children=yes ./2048

