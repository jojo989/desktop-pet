#include "App.hpp"
#include "SFML/System/Clock.hpp"
#include <iostream>
#include <SFML/OpenGL.hpp>
#include "../../Core/Source/Include/utils.hpp"
namespace App {

    Application::Application(const Core::WindowVariables& vars)
        : m_windowVars{ vars }
    {

    }

    void Application::run() {

        m_window.create();

        m_window.getRenderWindow().setActive(true);

        m_pet.setPosition({ 800, 800 });

        auto& renderWindow{ m_window.getRenderWindow() };
        sf::Clock clock;
/*
        sf::Font font("arial.ttf");
        sf::Text text(font);
        text.setPosition({ 500.f, 500.f });
        text.setScale({ 1.f, 1.f });
*/


        while (!m_window.shouldClose()) {
            float dt = clock.restart().asSeconds();
            //std::cout << 1.f / dt << '\n';

            m_window.update();
            m_pet.update(dt);

            //draw things!
            renderWindow.clear(sf::Color(0, 0, 0, 0));
            m_pet.draw(renderWindow);
            renderWindow.display();

        }
    }

};