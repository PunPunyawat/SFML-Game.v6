#include "weapon.h"

weapon::weapon(sf::Texture* texture, sf::Vector2u imageCount, float switchTime, sf::Vector2f position) :
	Animation(texture,imageCount,switchTime)
{
	this->body.setScale(sf::Vector2f(1, 1));
	this->body.setPosition(position);
	this->body.setTexture(*texture);
}

weapon::~weapon()
{

}

void weapon::Update(float deltaTime)
{
	//std::cout << deltaTime << std::endl;
	this->move(deltaTime);
	Animation.Update(5, deltaTime);
	this->body.setTextureRect(Animation.uvRect);
}

void weapon::Draw(sf::RenderWindow& window)
{
	window.draw(body);
}

void weapon::setActive(bool status)
{
	this->active = status;
	if (status == true) {
		startPosition = this->body.getPosition().x;
	}
}

bool weapon::isActive()
{
	return this->active;
}

void weapon::setPosition(sf::Vector2f posi)
{
	this->body.setPosition(posi);
}

sf::Vector2f weapon::getPosition()
{
	return this->body.getPosition();
}

void weapon::setY(double y)
{
	this->body.setPosition(sf::Vector2f(this->body.getPosition().x, this->body.getPosition().y + y));
}

double weapon::getDeltaDistance()
{
	return this->body.getPosition().x - this->startPosition;
}

void weapon::setEffect(int state)
{
	if (this->effect == 1 && state == 0) {
		this->speed = this->baseSpeed;
	}
	else if (this->effect == 0 && state == 1) {
		this->speed = 0.5;
	}
	this->effect = state;
}

int weapon::getEffect()
{
	return this->effect;
}

void weapon::move(float deltaTime)
{

	if (this->body.getPosition().x > 900)
	{
		this->setActive(false);
	}
	else
	{
		//std::cout << "Move" << std::endl;
		this->body.move(sf::Vector2f(this->speed, 0));
	}
}
