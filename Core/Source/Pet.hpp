#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <vector>
#include <string>
#include <filesystem>

enum class PetState {
    Idle,
    Running,
    Sleep,
    Border,
    Shocked
};

enum class RunningDirection {
    Right,
    DownRight,
    Down,
    DownLeft,
    Left,
    UpLeft,
    Up,
    UpRight
};

struct Animation {
    std::vector<sf::Texture> frames;
    float fps{ 8.f };
    bool loop{ true };
    size_t frameIndex{ 0 };
};

class Pet {
public:
    Pet();
    ~Pet() = default;

    void update(float dt);
    void draw(sf::RenderWindow& win);
    void setState(PetState s);
    void setPosition(const sf::Vector2f& pos) { m_position = pos; }
    void setSpeed(float speed) { m_speed = speed * 100; }
    void setScale(float scale) { m_scale = scale; };

    std::optional<sf::Sprite> getCurrentSprite() const;
    float getDistanceToCursor() const;
    sf::Vector2f getPosition() const;
    bool loadPet(const std::string& petName);

private:
    std::unordered_map<std::string, Animation> m_animations;

    PetState m_state{ PetState::Idle };
    RunningDirection m_currDirection{ RunningDirection::Down };

    sf::Vector2f m_position{};
    sf::Vector2f m_mousePosition{};
    sf::Vector2f m_futurePosition{};

    sf::Clock m_idleTimer;

    float m_scale{ 1.f };
    float m_speed{ 500.f };
    float m_animationTimer{ 0.f };
    float m_distanceToCursor{};

    bool loadFromJson(const std::string& jsonPath, const std::string& textureBasePath);

    // Helpers
    std::string getCurrentAnimKey() const;
    RunningDirection getDirectionFromAngle(float angleDeg);
};