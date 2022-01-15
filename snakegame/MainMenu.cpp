#include "MainMenu.hpp"
#include "GamePlay.hpp"
#include "Constants.h"
#include "Settings.h"

#include <SFML/Window/Event.hpp>

MainMenu::MainMenu(std::shared_ptr<Context> &context)
    : m_context(context), m_isPlayButtonSelected(true),
      m_isPlayButtonPressed(false), m_isExitButtonSelected(false),
      m_isExitButtonPressed(false) , m_isSettingButtonSelected(false),m_isSettingButtonPressed(false)
{
}

MainMenu::~MainMenu()
{
}

void MainMenu::Init()
{
    m_context->m_assets->AddFont(MAIN_FONT, "assets/fonts/Pacifico-Regular.ttf");

    // Title
    m_gameTitle.setFont(m_context->m_assets->GetFont(MAIN_FONT));
    m_gameTitle.setString("Snake Game");
    m_gameTitle.setOrigin(m_gameTitle.getLocalBounds().width / 2,
                          m_gameTitle.getLocalBounds().height / 2);
    m_gameTitle.setPosition(m_context->m_window->getSize().x / 2,
                            m_context->m_window->getSize().y / 2 - 150.f);

    // Play Button
    m_playButton.setFont(m_context->m_assets->GetFont(MAIN_FONT));
    m_playButton.setString("Play");
    m_playButton.setOrigin(m_playButton.getLocalBounds().width / 2,
                           m_playButton.getLocalBounds().height / 2);
    m_playButton.setPosition(m_context->m_window->getSize().x / 2,
                             m_context->m_window->getSize().y / 2 - 25.f);
    m_playButton.setCharacterSize(CHARACTER_SIZE);

    // Exit Button
    m_settingButton.setFont(m_context->m_assets->GetFont(MAIN_FONT));
    m_settingButton.setString("Settings");
    m_settingButton.setOrigin(m_settingButton.getLocalBounds().width / 2,
                              m_settingButton.getLocalBounds().height / 2);
    m_settingButton.setPosition(m_context->m_window->getSize().x / 2 - 10.f,
                             m_context->m_window->getSize().y / 2 + 25.f);
    m_settingButton.setCharacterSize(CHARACTER_SIZE);

    m_exitButton.setFont(m_context->m_assets->GetFont(MAIN_FONT));
    m_exitButton.setString("Exit");
    m_exitButton.setOrigin(m_exitButton.getLocalBounds().width / 2,
                           m_exitButton.getLocalBounds().height / 2);
    m_exitButton.setPosition(m_context->m_window->getSize().x / 2,
                             m_context->m_window->getSize().y / 2 + 75.f);
    m_exitButton.setCharacterSize(CHARACTER_SIZE);
}

void MainMenu::ProcessInput()
{
    sf::Event event;
    while (m_context->m_window->pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            m_context->m_window->close();
        }
        else if (event.type == sf::Event::KeyPressed)
        {
            switch (event.key.code)
            {
            case sf::Keyboard::Up:
            {

                if(m_isExitButtonSelected){
                    m_isSettingButtonSelected = true;
                    m_isExitButtonSelected = false;
                }
                else if (m_isSettingButtonSelected)
                {
                    m_isPlayButtonSelected = true;
                    m_isSettingButtonSelected = false;
                }
                break;
            }
            case sf::Keyboard::Down:
            {
                if(m_isPlayButtonSelected){
                    m_isSettingButtonSelected = true;
                    m_isPlayButtonSelected = false;
                }
                else if (m_isSettingButtonSelected)
                {
                    m_isExitButtonSelected = true;
                    m_isSettingButtonSelected = false;
                }
                break;
            }
            case sf::Keyboard::Return:
            {
                m_isPlayButtonPressed = false;
                m_isExitButtonPressed = false;
                m_isSettingButtonPressed = false;
                if (m_isPlayButtonSelected)
                {
                    m_isPlayButtonPressed = true;
                }
                else if(m_isSettingButtonSelected)
                {
                    m_isSettingButtonPressed = true;
                } else{

                    m_isExitButtonPressed = true;
                }

                break;
            }
            default:
            {
                break;
            }
            }
        }
    }
}

void MainMenu::Update(sf::Time deltaTime)
{
    if(m_isPlayButtonSelected)
    {
        m_playButton.setFillColor(sf::Color::Black);
        m_settingButton.setFillColor(sf::Color::White);
        m_exitButton.setFillColor(sf::Color::White);
    }
    else if(m_isSettingButtonSelected)
    {
        m_settingButton.setFillColor(sf::Color::Black);
        m_exitButton.setFillColor(sf::Color::White);
        m_playButton.setFillColor(sf::Color::White);
    } else{
        m_settingButton.setFillColor(sf::Color::White);
        m_exitButton.setFillColor(sf::Color::Black);
        m_playButton.setFillColor(sf::Color::White);
    }


    if(m_isPlayButtonPressed)
    {
        m_context->m_states->Add(std::make_unique<GamePlay>(m_context), true);
    } else if(m_isSettingButtonPressed){
        m_context->m_states->Add(std::make_unique<Settings>(m_context), true);
    }
    else if(m_isExitButtonPressed)
    {
        m_context->m_window->close();
    }
    
}

void MainMenu::Draw()
{
    m_context->m_window->clear(sf::Color::Blue);
    m_context->m_window->draw(m_gameTitle);
    m_context->m_window->draw(m_playButton);
    m_context->m_window->draw(m_exitButton);
    m_context->m_window->draw(m_settingButton);
    m_context->m_window->display();
}
