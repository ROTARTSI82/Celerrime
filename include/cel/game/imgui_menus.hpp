/**
 * @file imgui_menus.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-08-10
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#ifndef CEL_GAME_IMGUI_MENUS_HPP
#define CEL_GAME_IMGUI_MENUS_HPP

#include <GL/glew.h>

#include <GLFW/glfw3.h>

namespace cel {
    class app_t;

    class imgui_menu {
    public:
        imgui_menu(app_t *parent);
        ~imgui_menu();

        void new_frame();
        void draw();
    private:
        app_t *parent;
        bool show_demo = false;
        
        void debug_tools();
    };
}

#endif