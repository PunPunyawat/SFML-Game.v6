#include "background.h"

Background::Background(sf::Texture* texture, float speed)
{
	this->speed = speed;
	this->size = sf::Vector2f(564.0f, 864.0f); // ขนาดหน้าจอ

	body[0].setSize(size);
	body[1].setSize(size);
	body[0].setTexture(texture);
	body[1].setTexture(texture);

	/*if (speed > 0) // ลงข้างล่าง
	{
		body[1].setPosition(0.0f, +size.y ); 
	}*/
		
	//else 
	//{
		body[1].setPosition(0.0f, -size.y); 
	//}
}

void Background::Update(float deltaTime)
{
	for (int i = 0; i < 2; i++)
	{
		position = body[i].getPosition();

		body[i].move(0, speed * deltaTime);  // ให้ภาพพื้นหลังเลื่อน แกน y

		/*if (position.y < -size.y && speed> 0) // รูปแรกเคลื่อนที่ไปข้างบน
		{
			body[i].setPosition(position.x, size.y );
		}*/
		 if (position.y > size.y && speed>0)  // เซ้ทกลับมาภาพแรก
		{
			body[i].setPosition(position.x,-size.y + 4.0f);  // + 4.5f ให้รูปมันติดกัน
		}
	}

}

void Background::Draw(sf::RenderWindow& window)
{
	window.draw(body[0]);
	window.draw(body[1]);
}
