#include <SFML/Graphics.hpp>
#include <SFML\Audio.hpp>
#include <Windows.h>
#include "Player.h"
#include "Bullet.h"
#include "background.h"
#include <vector>
#include <sstream>
#include "Menu.h"
#include <cstdlib>

using namespace sf;
using namespace std;

static const float VIEW_HEIGHT = 864.0f;
static const float VIEW_WIDHT = 564.0f;

float detalTime = 0.0f;       // ตั้งค่าความเร็วให้ตัวละครเดิน 
float totaltime = 0;		  // ตั้งค่าความเร็วให้ตัวละครเดิน 	
float speed_player = 150.0f;  // ตั้งค่าความเร็วให้ตัวละครเดิน
int   animationFrame = 0; // แฟรมของตัวละคร ที่ 0 

int hit = 0, hit_again = 0;  // rand HP

///////////////////////////////// VIEW //////////////////////////////////////////

//ฟังชั่น ในการเซ้ทค่า display ให้คงที่ตลอดหลังจากปรับขยายหรือหดหน้า
void ResizeView(const sf::RenderWindow& window, sf::View& view)
{
	// aspectRatio คือการเซ้ทค่าของหน้าจอให้คงที่ตลอดหลังจากมีการขยายหน้าจอ เป็นต้น
	float aspectRatio = float(window.getSize().x) / float(window.getSize().y);
	view.setSize(VIEW_HEIGHT * aspectRatio, VIEW_WIDHT * aspectRatio);  // การให้มันคงที่ค่าในแกน x ภาพหน้าจะเหมือนเดิม
}

///////////////////////////////// SHOT //////////////////////////////////////////

/*class Bullett
{
public:
		
	sf::RectangleShape shot;
	float maxSpeed;
	sf::Vector2f curveVelocity;
	sf::Vector2f position;
	

	Bullett(float radius = 30.0f)   // เหมือนคำสั่งใน .cpp 
		: curveVelocity(0.0f, 0.0f), maxSpeed(10.0f)

	{			
		this->shot.setSize(sf::Vector2f(radius,radius));
				
	}

};*/

///////////////////////////////// MAIN /////////////////////////////////////////
int main()
{	
	int  Scorecurrent = 0;

	sf::Font word;
	if (!word.loadFromFile("font/Jelly Crazies.ttf")); {

	}
	
	sf::Text score;
	score.setFont(word);
	score.setCharacterSize(10);
	score.setStyle(sf::Text::Regular);
	score.setPosition(400, 50);
	score.setString("SCORE . ");

	sf::Text scoreCurrent;
	scoreCurrent.setFont(word);
	scoreCurrent.setCharacterSize(10);
	scoreCurrent.setStyle(sf::Text::Regular);
	scoreCurrent.setPosition(500, 50);
	

	///////////////////////////////////////////////////////////////////////////
	int screen = 0; // หน้าจอตอนให้ตัวละครเดินขึ้นมา
	sf::Clock clock;
	srand(time(NULL));

	///////////////////////////////// DISPLAY ///////////////////////////////////////
	
	sf::RenderWindow window(sf::VideoMode(564.0f, 864.0f), "Game_pp" );  //ขนาดหน้าจอ "ชื่อหัวเกม"

	///////////////////////////////// MENU ///////////////////////////////////////

	Menu menu(window.getSize().x,window.getSize().y);

	///////////////////////////////// VIEW //////////////////////////////////////////
	sf::View viewCharacter;
	viewCharacter.setSize(sf::Vector2f(564.0f, 864.0f));
	viewCharacter.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));   // ให้พอดีขนาดหน้าจอ
	//viewCharacter.zoom(1.2f);  // ขยายภาพพื้นหลัง
	//sf::View view(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(564.0f, 864.0f));   // ทำให้การปรับหน้าจอภาพให้มีขนาดตงที่

	/////////////////////////////// BACKGROUND /////////////////////////////////////

	// ลองใส่พื้นหลัง
	//sf::RectangleShape background(sf::Vector2f(564.0f, 864.0f)); // ให้รูปใหญ่เท่าไหร่ ก็เลือกขนาดเอา
	sf::Texture backgroundTexture[2];
	backgroundTexture[0].loadFromFile("./sprite/background_game.png");
	//backgroundTexture[1].loadFromFile("./sprite/background_game2.jpg");
	std::vector<Background>backgrounds;
	backgrounds.push_back(Background(&backgroundTexture[0], 110.0f));
	//backgrounds.push_back(Background(&backgroundTexture[1], 80.0f));


	///////////////////////////////// ITEM //////////////////////////////////////////
	// Item
	sf::RectangleShape Heart(sf::Vector2f(42.0f, 43.0f));
	sf::Texture Heartlove;
	Heartlove.loadFromFile("./sprite/Heart.png");
	Heart.setTexture(&Heartlove);
	Heart.setPosition({ 270,340 });
	//Heart.setOrigin(Heart.getSize() / 2.0f);

	///////////////////////////////// HP //////////////////////////////////////////
	//Hp

	sf::RectangleShape HP(sf::Vector2f(230.0f, 70.0f));
	sf::Texture HPplayer;
	HPplayer.loadFromFile("sprite/HP/HP_1_100p.png");
	HP.setTexture(&HPplayer);
	HP.setPosition({ 50,30 });
	
	///////////////////////////////// SHAPE //////////////////////////////////////////
	// เป็นของวงกลม
	sf::CircleShape shape(70,5);  // (ขนาด,มุมที่ให้เกิด เช่นอันนี้เป็น 5 มุม)
	shape.setPosition({ 450,400 });  // ตำแหน่งวงกลม (x,y)
	shape.setFillColor(sf::Color::White);  //สีของรูป
	
	///////////////////////////////// PLAYER //////////////////////////////////////////
	
	sf::Texture playerTexture;													
	playerTexture.loadFromFile("sprite/playermainful.png");
	// animation ใน class		//การแบ่งสโคปของรูปภาพ (4,4)	//ความเร็วในการรันเฟรม (0.5f)	// ความเร็วของตัวละคร (100.0f)
	Player player(&playerTexture, sf::Vector2u(4, 6), 0.2f, 150.0f);

	///////////////////////////////// ENEMY //////////////////////////////////////////

	sf::Vector2f playerCenter;  // เอามาจาก code ด้านล่าง
	sf::RectangleShape enemy(sf::Vector2f(120.0f, 100.0f));  // ของ monepic
	sf::Texture enemies_photo;
	enemies_photo.loadFromFile("sprite/monepicWHITE.png");
	enemy.setTexture(&enemies_photo);
	enemy.setPosition({200,100});
	
	std::vector<RectangleShape> enemies;
	enemies.push_back(RectangleShape(enemy));
	int enemySpawnTimer = 0;


	///////////////////////////////// SHOT //////////////////////////////////////////

	sf::Clock shotclock;   // delay กระสุน
	float shotDelay = shotclock.getElapsedTime().asSeconds();
	sf::RectangleShape shoot(sf::Vector2f(30.0f, 30.0f));  // ขนาดกระสุน
	sf::Texture shootz_photo;
	shootz_photo.loadFromFile("sprite/ninjaweaponWHITE.png");
	shoot.setTexture(&shootz_photo);
	std::vector<RectangleShape> shoots;
	shoots.push_back(RectangleShape(shoot));


	///////////////////////////////// SHOT //////////////////////////////////////////

	/*Bullett shot_shot;
	std::vector<Bullett> bullets;
	bullets.push_back(Bullett(shot_shot));

	sf::Vector2f playerCenter;
	sf::Vector2f mousePosition;
	sf::Vector2f aimDirection;
	sf::Vector2f aimDirectionNormalize;
	window.setFramerateLimit(60);

	// delay กระสุน
	sf::Clock shotclock;
	float shotDelay = shotclock.getElapsedTime().asSeconds();

	// ใส่รูป weapon 
	sf::Texture shottexture;
	shottexture.loadFromFile("sprite/weapon.png");
	// shot_shot ก่อนแล้ว เข้าไปในกรอบชื่อ shot แล้วใส่รูปเข้าไป 
	shot_shot.shot.setTexture(&shottexture);
	*/

	///////////////////////////////// SOUND //////////////////////////////////////////
	
	sf::Music start_sound; // เพลงเริ่มเกม
	if (!start_sound.openFromFile("sound/songstart.ogg"))
	{
		printf("ERROR\n");
	}
	start_sound.play(); //เปิดเพลง

	///////////////////////////////// SHOT SOUND //////////////////////////////////////////

	sf::SoundBuffer soundeffect;
	soundeffect.loadFromFile("sound/Soundshot.wav");
	sf::Sound shooteffect;
	shooteffect.setBuffer(soundeffect);

	///////////////////////////////// pickup SOUND //////////////////////////////////////////

	sf::SoundBuffer soundpick;
	soundpick.loadFromFile("sound/pickup_item.wav");
	sf::Sound Pickupsound;
	Pickupsound.setBuffer(soundpick);


	//******************************************************************************

	while (window.isOpen())
	{
		shotDelay = shotclock.getElapsedTime().asSeconds();     
		window.setFramerateLimit(60);
		
		
			detalTime = clock.restart().asSeconds(); // คำสั่งหน่วงให้น้องเดินช้า
			
			sf::Event evnt;
			while (window.pollEvent(evnt))	// while นี้ทำให้สามารถใช้เม้ากดเต็มจอหรือใช้ขยาย กว้างยาวได้
			{
				switch (evnt.type)
				{

	   /////////////////////////////  MENU //////////////////////////////////////////

				case sf::Event::KeyReleased:
					switch (evnt.key.code)
					{
					
					case sf::Keyboard::Up:
						menu.MoveUp();
						break;

					case sf::Keyboard::Down:
						menu.MoveDown();
						break;

					case sf::Keyboard::Return:
						switch (menu.GetPressedItem())
						{
						case 0:    // การรับค่าหน้า menu เข้ามา 
					
							printf("Play button has been pressed\n");			
							break;

						case 1:
							printf("Options button has been pressed\n");
							break;

						case 2:  // [2] array ที่ 2 จะให้ออกจากการทำงานทั้งหมด
							window.close();
							break;
						}
					}

					break;

				case sf::Event::Closed:

					window.close();
					break;

				case sf::Event::Resized:

					ResizeView(window, viewCharacter);
					break;

				}
			}

			///////////////////////////////// LOOP BG //////////////////////////////////////////	

			for (Background& background : backgrounds) {
				background.Update(detalTime);
			}

			for (Background& background : backgrounds) {   // ลูปให้รูปวน 
				background.Draw(window);
			}

			/// ///////////////////////////////////////////////////////////////////////////////

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{
				window.close();  // กด esc เพื่อออกจากโปรแกรม
			}

			//if(player.GetCollider().CheckCollision();   ทำเช็คชนกับตัวไอเท็ม

			//////////////////////// 	Update shot  /////////////////////////////////
		
		/*	playerCenter = sf::Vector2f(player.getPosition().x-15, player.getPosition().y+10); // ตำแหน่งกระสุนออกจากตัว

			mousePosition = sf::Vector2f(Mouse::getPosition(window));
			
			aimDirection = mousePosition - playerCenter;  // ทิศทางการยิง
			aimDirectionNormalize = aimDirection / sqrt(pow(aimDirection.x, 2) + pow(aimDirection.y, 2));
			
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)&&shotDelay>0.25)    // Mouse ยิง
			{
				shot_shot.shot.setPosition(playerCenter);
				shot_shot.curveVelocity = aimDirectionNormalize * shot_shot.maxSpeed;
				bullets.push_back(Bullett(shot_shot));
				shooteffect.play();
				shotDelay = shotclock.restart().asSeconds();

			}

			
			for (size_t i = 0; i < bullets.size(); i++)
			{
			
				bullets[i].shot.move(bullets[i].curveVelocity);

			}

			for (size_t i = 0; i < bullets.size(); i++)   // ลูปการวาดกระสุน 
			{
				if (i != 0) 
				{	
					window.draw(bullets[i].shot);
				}
				
				
			}

			*/

			/////////////////////////////  20/11/20  Update shoot //////////////////////////////////////////

			playerCenter = sf::Vector2f(player.getPosition().x - 15, player.getPosition().y + 10); // ตำแหน่งกระสุนออกจากตัว

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)
				/*Mouse::isButtonPressed(Mouse::Left)*/ & shotDelay > 0.25)
			{
				shoot.setPosition(playerCenter);
				shoots.push_back(RectangleShape(shoot));
				shotDelay = shotclock.restart().asSeconds();
				shooteffect.play();

			}

			for (size_t i = 0; i < shoots.size(); i++)
			{
				shoots[i].move(0.0f, -10.0f);

				if (shoots[i].getPosition().y <= 0)
				{
					shoots.erase(shoots.begin()+i);
				}
			}


			/////////////////////////////  spawn monster  //////////////////////////////////////////

			if (enemySpawnTimer < 30)   // การ spawn mon  (กำหนดลิมิตการสปอน)
			{
				enemySpawnTimer++;
			}

			
			int rand_x = (rand() % int(window.getSize().x - enemy.getSize().x));

			if (enemySpawnTimer >= 70)
			{
				enemy.setPosition(Vector2f(rand_x, -20.0f));
				enemies.push_back(RectangleShape(enemy));
				enemySpawnTimer=0;
			}

			enemySpawnTimer++;

			for (size_t i = 0; i < enemies.size(); i++)
			{
				enemies[i].move(0.0f, 1.5f);

				if (enemies[i].getPosition().y > window.getSize().y )  // สปอน มอนให้เกิด
				{
					enemies.erase(enemies.begin() + i);
				}

				window.draw(enemies[i]);     // วาดศัตรุออกมา
			}


			for (size_t i = 0; i < shoots.size(); i++)
			{
				window.draw(shoots[i]);   // วาดกระสุนออกมาให้ยิง
			}


			//////////////////// collision mon - shoot //////////////////////////////////////////


			for (size_t i = 0; i < shoots.size(); i++)
			{
				for (size_t k = 0; k < enemies.size(); k++)
				{
					if (shoots[i].getGlobalBounds().intersects(enemies[k].getGlobalBounds()))
					{
						Pickupsound.play();
						shoots.erase(shoots.begin() + i);
						enemies.erase(enemies.begin() + k);
						Scorecurrent += 5;
						
						break;
					}
									
				}
			}
			
			/////////////////////////////  DRAW //////////////////////////////////////////

			stringstream scoreShow;
			scoreShow << Scorecurrent;
			scoreCurrent.setString(scoreShow.str().c_str());    // วาด คะแนนตอนยิงได้
				
			window.draw(score);
			window.draw(scoreCurrent);
			menu.draw(window);
			window.draw(Heart);
			window.draw(HP);
			player.Draw(window);	
			player.Update(detalTime);

			window.display();
			window.clear(sf::Color::Transparent);
			
		}
	
	return 0;

}



