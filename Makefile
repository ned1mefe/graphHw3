# filepath: /home/sunny_unix/graphHw3/Makefile
CXX = g++
CXXFLAGS = -std=c++17 -g -Wall -I. -I/usr/include/GL -I/usr/include/freetype2 -DGL_SILENCE_DEPRECATION -DGLM_ENABLE_EXPERIMENTAL
LDFLAGS = -lglfw -lpng -lpthread -lX11 -ldl -lXrandr -lGLEW -lGL -lfreetype

SRCS = main.cpp game_state.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = tetrisGL

all: $(TARGET)

$(TARGET): $(OBJS)
    $(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)

%.o: %.cpp
    $(CXX) $(CXXFLAGS) -c $< -o $@

clean:
    rm -f $(OBJS) $(TARGET)