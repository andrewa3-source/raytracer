# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++17 

#Target
TARGET = cameratest
SOURCES = cameratest.cpp external/tiny_obj_loader.cc

all: $(TARGET)

# Rule to compile the program
$(TARGET): $(SOURCES)
	$(CXX) -o $@ $^ $(CXXFLAGS)

# Clean up build files
clean:
	rm -f $(TARGET)
