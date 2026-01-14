#include "App.hpp"
#include "SFML/System/Clock.hpp"  // Add this for sf::Clock
#include <iostream>
#include <SFML/OpenGL.hpp>
namespace App {

    Application::Application(const Core::WindowVariables& vars)
        : m_windowVars{ vars }
    {
        m_pet.setState(PetState::Idle);
    }

    void Application::run() {

        m_window.create();

        m_window.getRenderWindow().setActive(true);

        if (!m_pet.loadPet("oneko")) {
            std::cerr << "Fatal: Failed to load pet!\n";
            return;
        }

        m_pet.setPosition({ 30, 30 });
        m_pet.setState(PetState::Walk);

        auto& renderWindow{ m_window.getRenderWindow() };
        sf::Clock clock;

        while (!m_window.shouldClose()) {
            float dt = clock.restart().asSeconds();
            //std::cout << 1.f / dt << '\n';

            m_window.update();
            m_pet.update(dt);

            renderWindow.clear(sf::Color::Transparent);
            m_pet.draw(renderWindow);
            renderWindow.display();

        }
    }

};