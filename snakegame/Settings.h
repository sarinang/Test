#pragma once

#include <memory>

#include <SFML/Graphics/Text.hpp>

#include "State.hpp"
#include "Game.hpp"


#ifndef SNAKE_SETTINGS_H
#define SNAKE_SETTINGS_H


class Settings : public Engine::State {

private:
    std::shared_ptr<Context> m_context;
    sf::Text m_music;
    sf::Text m_hardness;
    sf::Text m_gobackToMainMenu;



    bool m_isMusicSelected;
    bool m_isMusicPressed;
    bool isMusicOn = true;

    bool m_ishardnessSelected;
    bool m_ishardnessPressed;
    bool isHard = false;

    bool m_isGoBackSelected;
    bool m_isGoBackPressed;

public:

    Settings(std::shared_ptr<Context> &context);
    ~Settings();

    void Init() override;
    void ProcessInput() override;
    void Update(sf::Time deltaTime) override;
    void Draw() override;
};


#endif //SNAKE_SETTINGS_H
