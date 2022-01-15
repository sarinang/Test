#include "GamePlay.hpp"
#include "GameOver.hpp"
#include "PauseGame.hpp"
#include <iostream>
#include <SFML/Window/Event.hpp>
#include "Constants.h"
#include <math.h>
#include <cstdlib>
#include <ctime>
#include <random>

GamePlay::GamePlay(std::shared_ptr<Context> &context)
    : m_context(context),
      m_score(0),
      m_snakeDirection({16.f, 0.f}),
      m_snakeDirection2({16.f, 0.f}),
      m_elapsedTime(sf::Time::Zero),
      m_isPaused(false)
{
    srand(time(nullptr));
}

GamePlay::~GamePlay()
{
}

void GamePlay::Init()
{


    m_context->m_assets->AddTexture(GRASS, "assets/textures/grass.png", true);
    m_context->m_assets->AddTexture(FOOD, "assets/textures/food.png");
    m_context->m_assets->AddTexture(WALL, "assets/textures/wall.png", true);
    m_context->m_assets->AddTexture(SNAKE, "assets/textures/snake.png");
    m_context->m_assets->AddTexture(SNAKE2, "assets/textures/snake2.png");

    m_grass.setTexture(m_context->m_assets->GetTexture(GRASS));
    m_grass.setTextureRect(m_context->m_window->getViewport(m_context->m_window->getDefaultView()));

    for (auto &wall : m_walls)
    {
        wall.setTexture(m_context->m_assets->GetTexture(WALL));
    }

    m_walls[0].setTextureRect({0, 0, (int)m_context->m_window->getSize().x, 16});
    m_walls[1].setTextureRect({0, 0, (int)m_context->m_window->getSize().x, 16});
    m_walls[1].setPosition(0, m_context->m_window->getSize().y - 16);

    m_walls[2].setTextureRect({0, 0, 16, (int)m_context->m_window->getSize().y});
    m_walls[3].setTextureRect({0, 0, 16, (int)m_context->m_window->getSize().y});
    m_walls[3].setPosition(m_context->m_window->getSize().x - 16, 0);
    m_walls[4].setTextureRect({0, 0, WALL_SIZE, 10});
    m_walls[5].setTextureRect({0, 0, 10, WALL_SIZE});
//    m_walls[4].setPosition(random(m_context->m_window->getSize().x - 100 , m_context->m_window->getSize().y - 100 ), random(m_context->m_window->getSize().x - 100 , m_context->m_window->getSize().y - 100 ));
//    m_walls[5].setPosition(random(m_context->m_window->getSize().x - 100 , m_context->m_window->getSize().y - 100 ), random(m_context->m_window->getSize().x - 100, m_context->m_window->getSize().y - 100 ));
    m_walls[4].setPosition(random(20,m_context->m_window->getSize().y - WALL_SIZE),random(20,HEIGHT - WALL_SIZE));
    m_walls[5].setPosition(random(20 , m_context->m_window->getSize().y - 20 ),random(20,HEIGHT - WALL_SIZE));
    m_food.setTexture(m_context->m_assets->GetTexture(FOOD));
    m_food.setPosition(m_context->m_window->getSize().x / 2, m_context->m_window->getSize().y / 2);

    m_food2.setTexture(m_context->m_assets->GetTexture(FOOD));
    m_food2.setPosition(m_context->m_window->getSize().x / 2 + 100.f, m_context->m_window->getSize().y / 2 + 100.f);
    m_snake.Init(m_context->m_assets->GetTexture(SNAKE),16.f);
    m_snake2.Init(m_context->m_assets->GetTexture(SNAKE2),m_context->m_window->getSize().y - 200.f);
    m_scoreText.setFont(m_context->m_assets->GetFont(MAIN_FONT));
    m_scoreText.setString("Score : " + std::to_string(m_score));
    m_scoreText.setCharacterSize(CHARACTER_SIZE);
}

void GamePlay::ProcessInput()
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
            sf::Vector2f newDirection = m_snakeDirection;
            switch (event.key.code)
            {
            case sf::Keyboard::Up:
                newDirection = {0.f, -16.f};
                break;
            case sf::Keyboard::Down:
                newDirection = {0.f, 16.f};
                break;
            case sf::Keyboard::Left:
                newDirection = {-16.f, 0.f};
                break;
            case sf::Keyboard::Right:
                newDirection = {16.f, 0.f};
                break;
            case sf::Keyboard::Escape:
                m_context->m_states->Add(std::make_unique<PauseGame>(m_context));
                break;

            default:
                break;
            }
            sf::Vector2f newDirection2 = m_snakeDirection2;
            switch (event.key.code)
            {
                case sf::Keyboard::W:
                    newDirection2 = {0.f, -16.f};
                    break;
                case sf::Keyboard::S:
                    newDirection2 = {0.f, 16.f};
                    break;
                case sf::Keyboard::A:
                    newDirection2 = {-16.f, 0.f};
                    break;
                case sf::Keyboard::D:
                    newDirection2 = {16.f, 0.f};
                    break;
                case sf::Keyboard::P:
                    m_context->m_states->Add(std::make_unique<PauseGame>(m_context));
                    break;
                default:
                    break;
            }

            if (abs(m_snakeDirection.x) != abs(newDirection.x) ||
                abs(m_snakeDirection.y) != abs(newDirection.y))
            {
                m_snakeDirection = newDirection;
            }

            if (abs(m_snakeDirection2.x) != abs(newDirection2.x) ||
                abs(m_snakeDirection2.y) != abs(newDirection2.y))
            {
                m_snakeDirection2 = newDirection2;
            }
        }
    }
}

void GamePlay::Update(sf::Time deltaTime)
{
    if(!m_isPaused)
    {
        m_elapsedTime += deltaTime;

        double time=ELAPSED_TIME_NORMAL;
        
        if (m_elapsedTime.asSeconds() > time)
        {
            for (auto &wall : m_walls)
            {
                if (m_snake.IsOn(wall) || m_snake2.IsOn(wall))
                {
                    m_context->m_states->Add(std::make_unique<GameOver>(m_context), true);
                    break;
                }
            }

            if (m_snake.IsOn(m_food))
            {
                m_snake.Grow(m_snakeDirection);

                int x = 0, y = 0;
                x = std::clamp<int>(rand() % m_context->m_window->getSize().x, 16, m_context->m_window->getSize().x - 2 * 16);
                y = std::clamp<int>(rand() % m_context->m_window->getSize().y, 16, m_context->m_window->getSize().y - 2 * 16);

                m_food.setPosition(x, y);
                m_score += 1;
                m_scoreText.setString("Score : " + std::to_string(m_score));
            } else if(m_snake.IsOn(m_food2)){
                m_snake.Grow(m_snakeDirection);

                int x = 0, y = 0;
                x = std::clamp<int>(rand() % m_context->m_window->getSize().x, 16, m_context->m_window->getSize().x - 2 * 16);
                y = std::clamp<int>(rand() % m_context->m_window->getSize().y, 16, m_context->m_window->getSize().y - 2 * 16);

                m_food2.setPosition(x, y);
                m_score += 1;
                m_scoreText.setString("Score : " + std::to_string(m_score));
            }
            else
            {
                m_snake.Move(m_snakeDirection);
            }

            if (m_snake2.IsOn(m_food))
            {
                m_snake2.Grow(m_snakeDirection2);

                int x = 0, y = 0;
                x = std::clamp<int>(rand() % m_context->m_window->getSize().x, 16, m_context->m_window->getSize().x - 2 * 16);
                y = std::clamp<int>(rand() % m_context->m_window->getSize().y, 16, m_context->m_window->getSize().y - 2 * 16);

                m_food.setPosition(x, y);
                m_score += 1;
                m_scoreText.setString("Score : " + std::to_string(m_score));
            } else if(m_snake2.IsOn(m_food2)){

                m_snake2.Grow(m_snakeDirection2);

                int x = 0, y = 0;
                x = std::clamp<int>(rand() % m_context->m_window->getSize().x, 16, m_context->m_window->getSize().x - 2 * 16);
                y = std::clamp<int>(rand() % m_context->m_window->getSize().y, 16, m_context->m_window->getSize().y - 2 * 16);

                m_food2.setPosition(x, y);
                m_score += 1;
                m_scoreText.setString("Score : " + std::to_string(m_score));
            }
            else
            {
                m_snake2.Move(m_snakeDirection2);
            }

            if (m_snake.IsSelfIntersecting() || m_snake.IsIntesectingWithOther(m_snake2))
            {
                m_context->m_states->Add(std::make_unique<GameOver>(m_context), true);
            }

            if (m_snake2.IsSelfIntersecting() || m_snake2.IsIntesectingWithOther(m_snake))
            {
                m_context->m_states->Add(std::make_unique<GameOver>(m_context), true);
            }

            m_elapsedTime = sf::Time::Zero;
        }
    }
}

void GamePlay::Draw()
{
    m_context->m_window->clear();
    m_context->m_window->draw(m_grass);

    for (auto &wall : m_walls)
    {
        m_context->m_window->draw(wall);
    }
    m_context->m_window->draw(m_food);
    m_context->m_window->draw(m_food2);
    m_context->m_window->draw(m_snake);
    m_context->m_window->draw(m_snake2);
    m_context->m_window->draw(m_scoreText);

    m_context->m_window->display();
}

void GamePlay::Pause()
{
    m_isPaused = true;
}

void GamePlay::Start()
{
    m_isPaused = false;
}
