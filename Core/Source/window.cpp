#include "SFML/Graphics.hpp"
#include "window.hpp"
#include <iostream>

#ifdef _WIN32
#include <dwmapi.h>
#include <windows.h>
#endif

#ifdef __linux__
#include <X11/Xlib.h>
#include <X11/extensions/shape.h>
#endif



namespace Core {

    Window::~Window() {
    }

    void Window::create() {
        m_window.create(sf::VideoMode({ m_windowVars.width, m_windowVars.height }), m_windowVars.title, sf::Style::None);
        m_window.setFramerateLimit(m_frameRateLimit);

        // transparent window

#ifdef _WIN32

#pragma comment(lib, "dwmapi.lib")

        MARGINS margins;
        margins.cxLeftWidth = -1;
        margins.cxRightWidth = -1;
        margins.cyTopHeight = -1;
        margins.cyBottomHeight = -1;

        DwmExtendFrameIntoClientArea(m_window.getNativeHandle(), &margins);

        // make window unclickeable
        long cur_style = GetWindowLong(m_window.getNativeHandle(), GWL_EXSTYLE);
        SetWindowLong(m_window.getNativeHandle(), GWL_EXSTYLE, cur_style | WS_EX_TRANSPARENT | WS_EX_LAYERED);

        SetWindowPos(m_window.getNativeHandle(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_ASYNCWINDOWPOS | SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
#endif


#ifdef __linux__
        Display* display = XOpenDisplay(NULL);
        Window w = m_window.getNativeHandle();

        XShapeCombineRectangles(display, w, ShapeInput, 0, 0, NULL, 0, ShapeSet, YXBanded);

        XFlush(display);
        XCloseDisplay(display);

#endif
    }

    void Window::update() {
        while (const std::optional<sf::Event> event = m_window.pollEvent()) {

            if (event->is<sf::Event::Closed>()) {
                m_window.close();
            }

        }

    }

    bool Window::shouldClose() const {
        return !m_window.isOpen();
    }
}