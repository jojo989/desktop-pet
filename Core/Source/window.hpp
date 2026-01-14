#pragma once
#include "SFML/Window.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include <string.h>
#include <cstdint>

namespace Core {

    struct WindowVariables {
        std::string title;
        uint16_t width;
        uint16_t height;
    };



    class Window {
    public:
        Window(const WindowVariables& vars)
            : m_windowVars{ vars }
        {
            m_window.setFramerateLimit(m_frameRateLimit);
        }
        ~Window();

        void create();
        void update();
        void setFrameRateLimit();

        bool shouldClose() const;

        sf::RenderWindow& getRenderWindow() { return m_window; }

        const WindowVariables& m_windowVars;

    private:
        sf::RenderWindow m_window;
        uint16_t m_frameRateLimit{ 30 };

    };

};