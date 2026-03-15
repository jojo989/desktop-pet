#include "SFML/Window/Window.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Window/WindowEnums.hpp"
#include "SFML/Window/WindowHandle.hpp"
#include "window.hpp"
#include <iostream>

#ifdef _WIN32
#include <dwmapi.h>
#include <windows.h>
#endif

#ifdef __linux__
#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <X11/extensions/Xfixes.h>
#include <X11/extensions/shape.h>

#undef None
#endif

namespace Core {

Window::~Window() {}

void Window::create() {
    m_window.create(sf::VideoMode({ m_windowVars.width, m_windowVars.height }),
                    "caca", sf::Style::None);
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
    SetWindowLong(m_window.getNativeHandle(), GWL_EXSTYLE,
                  cur_style | WS_EX_TRANSPARENT | WS_EX_LAYERED);

    SetWindowPos(m_window.getNativeHandle(), HWND_TOPMOST, 0, 0, 0, 0,
                 SWP_NOACTIVATE | SWP_ASYNCWINDOWPOS | SWP_NOACTIVATE |
                     SWP_NOMOVE | SWP_NOSIZE);
#endif

#ifdef __linux__
    Display* display = XOpenDisplay(NULL);
    sf::WindowHandle w = m_window.getNativeHandle();

    XserverRegion region = XFixesCreateRegion(display, NULL, 0);
    XFixesSetWindowShapeRegion(display, w, ShapeInput, 0, 0, region);
    XFixesDestroyRegion(display, region);

    Atom property = XInternAtom(display, "_NET_WM_WINDOW_OPACITY", False);
    unsigned long opacity = 0xffffffff;

    XChangeProperty(display, w, property, XA_CARDINAL, 32, PropModeReplace,
                    (unsigned char*)&opacity, 1);

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
} // namespace Core