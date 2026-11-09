CXX ?= g++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra

deepcore: src/*.cpp src/*.h
	$(CXX) $(CXXFLAGS) -o deepcore src/*.cpp

clean:
	rm -f deepcore
