#include "Settings.h"
#include "Constants.h"
#include "MainMenu.hpp"
#include <SFML/Window/Event.hpp>

void Settings::Init()
{
    m_context->m_assets->AddFont(MAIN_FONT, "assets/fonts/Pacifico-Regular.ttf");





    m_music.setFont(m_context->m_assets->GetFont(MAIN_FONT));
    if(m_context->isMusicOn){
        m_music.setString("Music : On");
    } else{
        m_music.setString("Music : Off");
    }
    m_music.setOrigin(m_music.getLocalBounds().width / 2,
                      m_music.getLocalBounds().height / 2);
    m_music.setPosition(m_context->m_window->getSize().x / 2,
                             m_context->m_window->getSize().y / 2 - 25.f);
    m_music.setCharacterSize(CHARACTER_SIZE);

    // Exit Button
    
    m_gobackToMainMenu.setFont(m_context->m_assets->GetFont(MAIN_FONT));
    m_gobackToMainMenu.setString("Go To MainMenu");
    m_gobackToMainMenu.setOrigin(m_gobackToMainMenu.getLocalBounds().width / 2,
                                 m_gobackToMainMenu.getLocalBounds().height / 2);
    m_gobackToMainMenu.setPosition(m_context->m_window->getSize().x / 2,
                             m_context->m_window->getSize().y / 2 + 75.f);
    m_gobackToMainMenu.setCharacterSize(CHARACTER_SIZE);
}
void Settings::ProcessInput() {
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

                    if(m_ishardnessSelected){
                        m_isMusicSelected = true;
                        m_ishardnessSelected = false;
                    }
                    else if (m_isGoBackSelected)
                    {
                        m_ishardnessSelected = true;
                        m_isGoBackSelected = false;
                    }
                    break;
                }
                case sf::Keyboard::Down:
                {
                    if(m_ishardnessSelected){
                        m_isGoBackSelected = true;
                        m_ishardnessSelected = false;
                    }
                    else if (m_isMusicSelected)
                    {
                        m_ishardnessSelected = true;
                        m_isMusicSelected = false;
                    }
                    break;
                }
                case sf::Keyboard::Return:
                {
                    m_isGoBackPressed = false;
                    m_ishardnessPressed = false;
                    m_isMusicPressed = false;
                    if (m_isMusicSelected)
                    {
                        m_isMusicPressed = true;
                    }
                    else if(m_ishardnessSelected)
                    {
                        m_ishardnessPressed = true;
                    } else{

                        m_isGoBackPressed = true;
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

void Settings::Update(sf::Time deltaTime) {
    if(m_isMusicSelected)
    {
        m_music.setFillColor(sf::Color::Black);
        m_hardness.setFillColor(sf::Color::White);
        m_gobackToMainMenu.setFillColor(sf::Color::White);
    }
    else if(m_ishardnessSelected)
    {
        m_hardness.setFillColor(sf::Color::Black);
        m_music.setFillColor(sf::Color::White);
        m_gobackToMainMenu.setFillColor(sf::Color::White);
    } else{
        m_music.setFillColor(sf::Color::White);
        m_gobackToMainMenu.setFillColor(sf::Color::Black);
        m_hardness.setFillColor(sf::Color::White);
    }


    if(m_isMusicPressed)
    {
        isMusicOn = !isMusicOn;
        if(isMusicOn){
            m_context->isMusicOn = true;
            m_music.setString("Music : On");
        } else if(!isMusicOn){
            m_context->isMusicOn = false;
            m_music.setString("Music : Off");
        }

    } else if(m_ishardnessPressed){
            if(isHard){
               isHard = false;
               m_context->isHard = false;
               m_hardness.setString("Difficulty : Normal");
            } else{
                isHard = true;
                m_context -> isHard = true;
                m_hardness.setString("Difficulty : Hard");
            }
    }
    else if(m_isGoBackPressed)
    {
        m_context->m_states->Add(std::make_unique<MainMenu>(m_context), true);
    }

    m_ishardnessPressed = false;
    m_isMusicPressed = false;
    m_isGoBackPressed = false;
}

void Settings::Draw() {


    m_context->m_window->clear(sf::Color::Blue);
    m_context->m_window->draw(m_music);
    m_context->m_window->draw(m_hardness);
    m_context->m_window->draw(m_gobackToMainMenu);
    m_context->m_window->display();
}

Settings::Settings(std::shared_ptr<Context> &context): m_context(context), m_isMusicSelected(true),
                                                       m_isGoBackSelected(false), m_isMusicPressed(false),
                                                       m_isGoBackPressed(false) , m_ishardnessPressed(false), m_ishardnessSelected(false) {



}

Settings::~Settings(){

}