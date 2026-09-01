CXX = g++
CXXFLAGS = -O3 -std=c++11 -Iinclude

SRCS = src/main.cpp src/io.cpp src/router.cpp
TARGET = build/router.exe

all: $(TARGET)

$(TARGET): $(SRCS) | build_dir
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

build_dir:
	@if not exist build mkdir build

run: $(TARGET)
	@echo "Running test: $(TEST)..."
	./$(TARGET) data/$(TEST).grid data/$(TEST).nl build/$(TEST)

clean:
	@if exist build rmdir /s /q build