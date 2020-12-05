#include "Bullet.h"

Bullet::Bullet(sf::Texture* texture, int* effect)
{
    this->effect = effect;
    for (int i = 0; i < fire_num; i++) {
        weapon Weapon_shot(texture, sf::Vector2u(4, 6), 0.3f, sf::Vector2f(100, 100));
        shot.push_back(Weapon_shot);
    }
}

Bullet::~Bullet()
{
}

void Bullet::Draw(sf::RenderWindow& window)
{
    deltaTime = clock.restart().asSeconds();
    this->action();
    for (int i = 0; i < fire_num; i++)
    {
        if (shot[i].isActive() == true)
        {
            //std::cout << *this->effect << std::endl;
            
            shot[i].Update(deltaTime);
            shot[i].Draw(window);

            if (shot[i].getDeltaDistance() > 200 && shot[i].getEffect() == 1) {
                shot[i].setEffect(0);
                shot[i + 1].setEffect(0);
                shot[i + 2].setEffect(0);
                shot[i].setY(-50);
                shot[i+1].setY(+50);
                i += 3;
            }
        }
    }
}

bool Bullet::canAttack()
{
    for (int i = 0; i < fire_num; i++)
    {
        if (this->shot[i].isActive() == false) {
            return true;
        }
    }
    return false;
}

void Bullet::action()
{
    if ((*this->evnt).type == sf::Event::EventType::TextEntered
        and (*this->evnt).text.unicode == ' '
        and this->isSpacebarPrees == false)
    {
        //std::cout << "set isSpacebarPrees: true" << std::endl;
        this->isSpacebarPrees = true;
        for (int i = 0; i < fire_num; i++)
        {
            if (this->shot[i].isActive() == false) {
                //this->fires[i].setPosition(sf::Vector2f(300.0f,300.0f)); => fixed : 31/10/20 12.20AM.
                
                //std::cout << "fire [" << i << "]" << std::endl;
                //std::cout << fires[i].getPosition().x << std::endl;
                switch (*this->effect)
                {
                case 0:
                    shot[i].setPosition(this->positionPlayer);
                    this->shot[i].setActive(true);
                    break;
                case 1:
                    shot[i].setEffect(*this->effect);
                    shot[i + 1].setEffect(*this->effect);
                    shot[i + 2].setEffect(*this->effect);
                    shot[i].setPosition(sf::Vector2f(this->positionPlayer.x, this->positionPlayer.y));
                    shot[i + 1].setPosition(sf::Vector2f(this->positionPlayer.x, this->positionPlayer.y));
                    shot[i + 2].setPosition(sf::Vector2f(this->positionPlayer.x, this->positionPlayer.y));
                    this->shot[i].setActive(true);
                    this->shot[i + 1].setActive(true);
                    this->shot[i + 2].setActive(true);
                    break;
                case 2:
                    shot[i].setPosition(sf::Vector2f(this->positionPlayer.x, this->positionPlayer.y-50));
                    shot[i + 1].setPosition(sf::Vector2f(this->positionPlayer.x+30, this->positionPlayer.y));
                    shot[i + 2].setPosition(sf::Vector2f(this->positionPlayer.x, this->positionPlayer.y+50));
                    this->shot[i].setActive(true);
                    this->shot[i + 1].setActive(true);
                    this->shot[i + 2].setActive(true);
                    break;
                default:
                    break;
                }
                break;
            }
        }
    }
    if ((*this->evnt).type == sf::Event::EventType::KeyReleased && this->isSpacebarPrees == true)
    {
        this->isSpacebarPrees = false;
       // std::cout << "set isSpacebarPrees: false" << std::endl;
    }
    
}