#include "Pet.hpp"
#include <fstream>
#include "./Include/json.hpp"
#include <iostream>
#include <format>

Pet::Pet() {
    loadPet("oneko");

}

void Pet::draw(sf::RenderWindow& win) {
    auto it = m_animations.find(m_state);
    if (it == m_animations.end()) {
        return;
    }

    auto& anim = it->second;
    if (anim.frames.empty()) {
        return;
    }

    sf::Sprite sprite(anim.frames.at(anim.frameIndex));
    sprite.setPosition(m_position);
    sprite.setScale({ m_scale, m_scale });

    win.draw(sprite);
}

void Pet::setState(PetState s) {
    if (s == m_state) return;
    m_state = s;
    m_animationTimer = 0.f;
    m_animations[m_state].frameIndex = 0;
}

void Pet::update(float dt) {
    auto& anim = m_animations.at(m_state);
    if (anim.frames.empty() || anim.fps <= 0.f) return;

    m_animationTimer += dt;
    float timePerFrame = 1.0f / anim.fps;

    if (m_animationTimer >= timePerFrame) {
        m_animationTimer -= timePerFrame;
        ++anim.frameIndex;

        if (anim.frameIndex >= anim.frames.size()) {
            anim.frameIndex = anim.loop ? 0 : anim.frames.size() - 1;
        }
    }
    //  std::cout << anim.frameIndex << '\n';
}

PetState Pet::stringToPetState(const std::string& stateStr) {
    static const std::unordered_map<std::string, PetState> stateMap = {
        {"idle", PetState::Idle},
        {"walk", PetState::Walk},
        {"sleep", PetState::Sleep},
        {"border", PetState::Border},
        {"shocked", PetState::Shocked}
    };

    auto it = stateMap.find(stateStr);
    return (it != stateMap.end()) ? it->second : PetState::Idle;
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
    // Print current working directory

    std::ifstream file(jsonPath);
    if (!file.is_open()) {
        return false;
    }

    nlohmann::json jsonData;
    try {
        file >> jsonData;
    }
    catch (const nlohmann::json::parse_error& e) {
        return false;
    }

    // Load settings
    if (jsonData.contains("settings") && jsonData["settings"].is_object()) {
        m_scale = jsonData["settings"].value("scale", 1.0f);
    }

    // Load animations
    if (!jsonData.contains("animations") || !jsonData["animations"].is_object()) {
        return false;
    }

    int loadedAnimations = 0;
    for (auto& [key, animData] : jsonData["animations"].items()) {

        PetState state = stringToPetState(key);
        Animation newAnim;
        newAnim.fps = animData.value("fps", 8.f);
        newAnim.loop = animData.value("loop", true);
        newAnim.frameIndex = 0;

        if (!animData.contains("frames") || !animData["frames"].is_array()) {
            continue;
        }

        int loadedFrames = 0;
        for (const auto& framePath : animData["frames"]) {
            const std::string fullPath = textureBasePath + framePath.get<std::string>();

            sf::Texture texture;
            if (texture.loadFromFile(fullPath)) {
                newAnim.frames.push_back(std::move(texture));
                loadedFrames++;
            }
            else {
                std::cerr << "Failed to load texture: " << fullPath << '\n';
            }
        }

        if (!newAnim.frames.empty()) {
            m_animations[state] = std::move(newAnim);
            ++loadedAnimations;
        }
        else {
        }
    }

    bool success = !m_animations.empty();

    if (!success) {
    }

    return success;
}

sf::Sprite Pet::getCurrentSprite() const
{
    auto it = m_animations.find(m_state);
    if (it == m_animations.end() || it->second.frames.empty()) {
        throw std::runtime_error("no texture loaded, check your pet folder!");  // no sprite / texture loaded
    }

    const auto& anim = it->second;
    sf::Sprite sprite(anim.frames[anim.frameIndex]);
    sprite.setPosition(m_position);
    sprite.setScale({ m_scale, m_scale });
    return sprite;
}