CXX := g++
CXXFLAGS := -std=c++11 -O3 -pedantic -g 
INCLUDES := -Iinclude
SRCDIR := src
OBJDIR := obj

# List all the source files here
SOURCES := $(wildcard $(SRCDIR)/*.cpp) $(wildcard $(SRCDIR)/lights/*.cpp) $(wildcard $(SRCDIR)/objects/*.cpp) $(wildcard $(SRCDIR)/kdtree/*.cpp)
# Generate the object file names by replacing the source directory with the object directory and extension .cpp with .o
OBJECTS := $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SOURCES))

# The final binary
TARGET := raytracer

# Default target
all: $(TARGET)

# Link the target with all the object files
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $@

# Compile each source file to an object file
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Clean up the build
clean:
	rm -rf $(OBJDIR) $(TARGET) *.ppm

.PHONY: all clean