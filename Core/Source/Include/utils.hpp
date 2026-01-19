#include <cmath>
#include <SFML/System.hpp>
#include <concepts>
#include <chrono>
namespace utils {

    template <typename T>
    float distanceOf(sf::Vector2<T> x, sf::Vector2<T> y) {
        return std::sqrtf(
            powf((y.x - x.x), 2) + powf((y.y - x.y), 2)
        );
    }

    template <typename T>
    float angleTo_d(const sf::Vector2<T>& from, const sf::Vector2<T>& to) {
        auto fromf = sf::Vector2f{ from }; auto tof = sf::Vector2f{ to };
        if (fromf == sf::Vector2f({ 0,0 }) || tof == sf::Vector2f({ 0,0 })) { return 0.f; }

        return from.angleTo(to).asDegrees();


    };
}