#include "SFML/Graphics.hpp"
#include "window.hpp"
#include <iostream>
namespace Core {

    Window::~Window() {
    }

    void Window::create() {
        m_window.create(sf::VideoMode({ m_windowVars.width, m_windowVars.height }), m_windowVars.title, sf::Style::None);
                // m_window.setFramerateLimit(m_frameRateLimit);
    }

    void Window::update() {
        const std::optional<sf::Event> event = m_window.pollEvent();
        if (!event) { return; }
        if (event->is<sf::Event::Closed>()) {
            m_window.close();
        }
    }

    bool Window::shouldClose() const {
        return !m_window.isOpen();
    }
}