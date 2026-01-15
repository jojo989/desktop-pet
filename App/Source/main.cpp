#include <iostream>
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "SFML/System.hpp"
#include "App.hpp"

int main(int argc, char** argv) {
    App::Application mainWindow{ {"SFML UWU", 500, 500} };
/*

if (argc > 2 || argc < 1) {
        throw std::runtime_error("you can only use 1 argument, which is the name of the pet!");
    }

*/


    std::cout << "argc: " << argv[0] << '\n';
    mainWindow.m_pet.loadPet(std::string{ argv[1] });



    mainWindow.run();

    // mainWindow.m_window.getRenderWindow().draw(mainWindow.m_pet.getCurrentSprite());

    auto currentDir{ std::filesystem::current_path() };
    std::cout << currentDir;



    return 0;
}
