all: tetrisGL

tetrisGL: main.cpp GameState.cpp
    g++ -std=c++17 main.cpp GameState.cpp -o tetrisGL -g -lglfw -lpng -lpthread -lX11 -ldl -lXrandr -lGLEW -lGL -DGL_SILENCE_DEPRECATION -DGLM_ENABLE_EXPERIMENTAL -I. `pkg-config --cflags --libs freetype2`

clean:
    rm -f tetrisGL