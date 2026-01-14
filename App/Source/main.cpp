#include <iostream>
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "SFML/System.hpp"
#include "App.hpp"

int main() {


    App::Application mainWindow{ {"SFML UWU", 1280, 720} };

    mainWindow.run();

   // mainWindow.m_window.getRenderWindow().draw(mainWindow.m_pet.getCurrentSprite());

    auto currentDir{ std::filesystem::current_path() };
    std::cout << currentDir;



    return 0;
}
