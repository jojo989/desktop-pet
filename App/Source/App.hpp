#include "../../Core/Source/window.hpp"
#include "../../Core/Source/Pet.hpp"
namespace App {

    class Application {
    public:
        Application(const Core::WindowVariables& vars);
        void run();
        Core::WindowVariables m_windowVars{};
        Pet m_pet{};
        Core::Window m_window{ m_windowVars };
    };


};