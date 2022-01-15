#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Audio.hpp>
#include "Game.hpp"
#include "MainMenu.hpp"
#include "Constants.h"
Game::Game() : m_context(std::make_shared<Context>())
{
    m_context->m_window->create(sf::VideoMode(WIDTH, HEIGHT), "Snake Game", sf::Style::Close);
    m_context->m_states->Add(std::make_unique<MainMenu>(m_context));
}

Game::~Game()
{
}

void Game::Run()
{
    sf::CircleShape shape(CIRCLE_SHAPE_SIZE);
    shape.setFillColor(sf::Color::Green);

    sf::Clock clock;
    sf::Time timeSinceLastFrame = sf::Time::Zero;


    sf::Music music;
    if(!music.openFromFile("assets/music/music.ogg")){
        return;
    }
    music.play();
    while (m_context->m_window->isOpen())
    {
        if(!m_context->isMusicOn && music.getStatus() == sf::SoundSource::Playing){
            music.pause();
        } else if(m_context->isMusicOn && music.getStatus() == sf::SoundSource::Paused){
            music.play();
        }
        timeSinceLastFrame += clock.restart();

        while (timeSinceLastFrame > TIME_PER_FRAME)
        {
            timeSinceLastFrame -= TIME_PER_FRAME;

            m_context->m_states->ProcessStateChange();
            m_context->m_states->GetCurrent()->ProcessInput();
            m_context->m_states->GetCurrent()->Update(TIME_PER_FRAME);
            m_context->m_states->GetCurrent()->Draw();
        }
    }
}