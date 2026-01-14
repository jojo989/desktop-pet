#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <vector>
#include <string>
#include <filesystem>

enum class PetState {
    Idle,
    Walk,
    Sleep,
    Border,
    Shocked
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
    sf::Sprite getCurrentSprite() const;
    bool loadPet(const std::string& petName);

private:
    std::unordered_map<PetState, Animation> m_animations;
    float m_scale{ 1.f };
    sf::Vector2f m_position{ 400.f, 300.f };

    PetState m_state{ PetState::Idle };
    float m_animationTimer{ 0.f };

    PetState stringToPetState(const std::string& stateStr);
    bool loadFromJson(const std::string& jsonPath, const std::string& textureBasePath);
};