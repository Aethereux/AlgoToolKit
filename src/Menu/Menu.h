#pragma once
#include "imgui.h"

class Menu {
public:
    Menu(const Menu&) = delete;
    Menu& operator=(const Menu&) = delete;

    static Menu& GetInstance() {
        static Menu instance;
        return instance;
    }

    void Render();

private:
    Menu() = default;
    ~Menu() = default;
};