#pragma once

#include <SFML/Graphics.hpp>
#include <utility>
#include <glm/glm.hpp>
#include <algorithm>

#include "ConfigData.hpp"

class Joystick {
protected:
    glm::vec2 m_direction{};
    float m_magnitude = 0;
public:
    inline glm::vec2 GetDirection() const {
	return m_direction;
    }
    inline float GetMagnitude() const {
	return m_magnitude;
    }
    virtual void Update(const sf::Event & event) = 0;
    virtual Joystick * Clone() = 0;
    virtual ~Joystick() {}
};

struct Circle {
    glm::vec2 center;
    float radius;
};

class MouseJoystickProxy : public Joystick {
    Circle m_circle;
    size_t m_yields;
    float m_sensitivity;
public:
    MouseJoystickProxy();
    MouseJoystickProxy * Clone() override;
    void Update(const sf::Event & event) override;
    size_t Yield();
};

class GamepadJoystick : public Joystick {
public:
    void Update(const sf::Event & event) override;
    GamepadJoystick * Clone() override;
};

class ButtonSet {
protected:
    bool m_pausePressed, m_weaponPressed;
public:
    bool PausePressed() const;
    bool WeaponPressed() const;
    virtual void Update(const sf::Event & event) = 0;
    virtual ~ButtonSet() {}
};

class KeyboardButtonSet : public ButtonSet {
    sf::Keyboard::Key m_pauseMapping, m_weaponMapping;
public:
    KeyboardButtonSet(const ConfigData::ControlsConf::KeyboardMapping & mapping);
    void Update(const sf::Event & event) override;
};

class GamepadButtonSet : public ButtonSet {
    unsigned m_pauseMapping, m_weaponMapping;
public:
    GamepadButtonSet(const ConfigData::ControlsConf::GamepadMapping & mapping);
    void Update(const sf::Event & event) override;
};
