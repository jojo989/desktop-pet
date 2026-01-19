#include <iostream>
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "SFML/System.hpp"
#include "App.hpp"

int main(int argc, char** argv) {
    const auto screen{ sf::VideoMode::getDesktopMode() };
    App::Application mainWindow{ {"SFML UWU", static_cast<uint16_t>(screen.size.x - 1), static_cast<uint16_t>(screen.size.y - 1)} };
/*

if (argc > 2 || argc < 1) {
        throw std::runtime_error("you can only use 1 argument, which is the name of the pet!");
    }

*/


    std::cout << "argv: " << argv[1] << '\n';
    mainWindow.m_pet.loadPet(std::string{ argv[1] });
    mainWindow.m_pet.setSpeed(std::stof(argv[2]));
    mainWindow.m_pet.setScale(std::stof(argv[3]));





    mainWindow.run();

    // mainWindow.m_window.getRenderWindow().draw(mainWindow.m_pet.getCurrentSprite());

    auto currentDir{ std::filesystem::current_path() };
    std::cout << currentDir;



    return 0;
}
