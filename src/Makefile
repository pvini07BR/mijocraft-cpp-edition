#OBJS specifies which files to compile as part of the project
OBJS = src/game.cpp src/glad.c src/main.cpp src/resource_manager.cpp src/shader.cpp src/sprite_renderer.cpp src/stb_image.c src/texture.cpp src/block.cpp src/player.cpp src/text_renderer.cpp src/imgui/imgui.cpp src/imgui/imgui_widgets.cpp src/imgui/imgui_tables.cpp src/imgui/imgui_draw.cpp src/imgui/imgui_demo.cpp src/imgui/imgui_impl_glfw.cpp src/imgui/imgui_impl_opengl3.cpp

#CC specifies which compiler we're using
CC = g++

#INCLUDE_PATHS specifies the additional include paths we'll need
INCLUDE_PATHS = -I./include

#LIBRARY_PATHS specifies the additional library paths we'll need
LIBRARY_PATHS = -L./lib

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
# -Wl,-subsystem,windows gets rid of the console window
COMPILER_FLAGS = -w -Wl,-subsystem,windows

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lglfw3dll -lbox2d.dll -lfreetype

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = build/teste.exe

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) --std c++20 -g $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)