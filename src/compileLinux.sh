#!/bin/sh
g++ src/game.cpp src/glad.c src/main.cpp src/resource_manager.cpp src/shader.cpp src/sprite_renderer.cpp src/stb_image.c src/texture.cpp src/block.cpp src/player.cpp src/text_renderer.cpp src/imgui/imgui.cpp src/imgui/imgui_widgets.cpp src/imgui/imgui_tables.cpp src/imgui/imgui_draw.cpp src/imgui/imgui_demo.cpp src/imgui/imgui_impl_glfw.cpp src/imgui/imgui_impl_opengl3.cpp -I./include -L./lib -lglfw3 -lfreetype -llibbox2d.dll -lrt -lm -ldl -lX11 -lpthread -lxcb -lXau -lXdmcp -o teste
echo "Compilation terminated."
