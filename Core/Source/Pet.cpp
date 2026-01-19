#include "Pet.hpp"
#include <fstream>
#include "./Include/json.hpp"
#include <iostream>
#include <format>
#include <cmath>
#include "./Include/utils.hpp"


Pet::Pet() {
}


std::string directionToString(RunningDirection dir) {
    switch (dir) {
        case RunningDirection::Up:        return "up";
        case RunningDirection::UpRight:   return "up_right";
        case RunningDirection::Right:     return "right";
        case RunningDirection::DownRight: return "down_right";
        case RunningDirection::Down:      return "down";
        case RunningDirection::DownLeft:  return "down_left";
        case RunningDirection::Left:      return "left";
        case RunningDirection::UpLeft:    return "up_left";
        default: return "down";
    }
}

std::string stateToString(PetState state) {
    switch (state) {
        case PetState::Idle:    return "idle";
        case PetState::Running: return "running"; // base
        case PetState::Sleep:   return "sleep";
        case PetState::Border:  return "border";
        case PetState::Shocked: return "shocked";
        default: return "idle";
    }
}

std::string Pet::getCurrentAnimKey() const {
    std::string base = stateToString(m_state);

    if (m_state == PetState::Running) {
        return base + "_" + directionToString(m_currDirection);
    }

    return base;
}


void Pet::draw(sf::RenderWindow& win) {
    std::string animKey = getCurrentAnimKey();
    auto it = m_animations.find(animKey);

    if (it == m_animations.end()) {
        it = m_animations.find(stateToString(m_state));
        if (it == m_animations.end()) return;
    }

    auto& anim = it->second;
    if (anim.frames.empty()) return;

    if (anim.frameIndex >= anim.frames.size()) anim.frameIndex = 0;

    sf::Sprite sprite(anim.frames.at(anim.frameIndex));
    sprite.setPosition(m_position);
    sprite.setScale({ m_scale, m_scale });


    win.draw(sprite);
}

void Pet::setState(PetState s) {
    if (s == m_state) return;
    m_state = s;
    m_animationTimer = 0.f;

    std::string animKey = getCurrentAnimKey();
    if (m_animations.count(animKey)) {
        m_animations[animKey].frameIndex = 0;
    }
}

RunningDirection Pet::getDirectionFromAngle(float angle) {
    while (angle < 0.f) angle += 360.f;
    while (angle >= 360.f) angle -= 360.f;

    if (angle >= 337.5f || angle < 22.5f)   return RunningDirection::Right;
    if (angle >= 22.5f && angle < 67.5f)    return RunningDirection::DownRight;
    if (angle >= 67.5f && angle < 112.5f)   return RunningDirection::Down;
    if (angle >= 112.5f && angle < 157.5f)  return RunningDirection::DownLeft;
    if (angle >= 157.5f && angle < 202.5f)  return RunningDirection::Left;
    if (angle >= 202.5f && angle < 247.5f)  return RunningDirection::UpLeft;
    if (angle >= 247.5f && angle < 292.5f)  return RunningDirection::Up;
    if (angle >= 292.5f && angle < 337.5f)  return RunningDirection::UpRight;

    return RunningDirection::Down;
}
void Pet::update(float dt) {
    std::string animKey = getCurrentAnimKey();
    if (m_animations.find(animKey) == m_animations.end()) {
        animKey = stateToString(m_state);
    }

    if (m_animations.count(animKey)) {
        auto& anim = m_animations[animKey];
        if (!anim.frames.empty() && anim.fps > 0.f) {
            m_animationTimer += dt;
            float timePerFrame = 1.0f / anim.fps;
            if (m_animationTimer >= timePerFrame) {
                m_animationTimer -= timePerFrame;
                ++anim.frameIndex;
                if (anim.frameIndex >= anim.frames.size()) {
                    anim.frameIndex = anim.loop ? 0 : anim.frames.size() - 1;
                }
            }
        }
    }

    sf::Vector2f m_mousePosition = sf::Vector2f(sf::Mouse::getPosition());
    float distance = utils::distanceOf<float>(m_position, m_mousePosition);

    float triggerDist = 100.0f;
    float stopDist = 15.0f; // jittering fix - 15 px

    switch (m_state) {
        case PetState::Idle:
            if (distance > triggerDist) {
                setState(PetState::Shocked);
                m_idleTimer.restart();
            }
            break;

        case PetState::Shocked:
            if (m_idleTimer.getElapsedTime().asSeconds() >= 1.f) {
                setState(PetState::Running);
            }
            break;

        case PetState::Running:
            if (distance <= stopDist) {
                setState(PetState::Idle);
            }
            else {
                sf::Vector2f diff = m_mousePosition - m_position;
                sf::Vector2f normalizedDir = diff / distance;

                // move pet
                m_position += (normalizedDir * m_speed) * dt;

                float angleRad = std::atan2(diff.y, diff.x);
                float angleDeg = angleRad * 180.f / 3.14159265f;
                m_currDirection = getDirectionFromAngle(angleDeg);
            }
            break;

        default:
            break;
    }
}

bool Pet::loadPet(const std::string& petName) {
    const std::string basePath{ "pets/" + petName + "/" };
    const std::string jsonPath{ basePath + petName + ".json" };

    m_animations.clear();
    m_state = PetState::Idle;
    m_animationTimer = 0.f;
    m_scale = 1.f;

    std::cout << "Loading pet from: " << basePath << '\n';
    return loadFromJson(jsonPath, basePath);
}

bool Pet::loadFromJson(const std::string& jsonPath, const std::string& textureBasePath) {
    std::ifstream file(jsonPath);
    if (!file.is_open()) return false;

    nlohmann::json jsonData;
    try {
        file >> jsonData;
    }
    catch (const nlohmann::json::parse_error& e) {
        std::cerr << "JSON Error: " << e.what() << '\n';
        return false;
    }

    if (jsonData.contains("settings")) {
        m_scale = jsonData["settings"].value("scale", 1.0f);
    }

    if (!jsonData.contains("animations")) return false;

    for (auto& [key, animData] : jsonData["animations"].items()) {

        Animation newAnim;
        newAnim.fps = animData.value("fps", 8.f);
        newAnim.loop = animData.value("loop", true);
        newAnim.frameIndex = 0;

        if (animData.contains("frames")) {
            for (const auto& framePath : animData["frames"]) {
                std::string fullPath = textureBasePath + framePath.get<std::string>();
                sf::Texture texture;
                if (texture.loadFromFile(fullPath)) {
                    newAnim.frames.push_back(std::move(texture));
                }
                else {
                    std::cerr << "Failed to load: " << fullPath << '\n';
                }
            }
        }

        if (!newAnim.frames.empty()) {
            m_animations[key] = std::move(newAnim);
        }
    }

    return !m_animations.empty();
}

std::optional<sf::Sprite> Pet::getCurrentSprite() const {
    std::string key = getCurrentAnimKey();
    if (m_animations.find(key) == m_animations.end()) {
        key = stateToString(m_state);
    }

    if (m_animations.count(key) && !m_animations.at(key).frames.empty()) {
        const auto& anim = m_animations.at(key);
        sf::Sprite sprite(anim.frames[anim.frameIndex]);
        sprite.setPosition(m_position);
        sprite.setScale({ m_scale, m_scale });
        return sprite;
    }

    return {};
}

float Pet::getDistanceToCursor() const {
    return utils::distanceOf(m_position, sf::Vector2f(sf::Mouse::getPosition()));
}

sf::Vector2f Pet::getPosition() const {
    return m_position;
}