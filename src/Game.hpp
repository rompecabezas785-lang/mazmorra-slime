#pragma once
#include <SFML/Graphics.hpp>
#include <array>
#include <memory>
#include <string>
#include <vector>

enum class Power { Spirit=0, Fire=1, Water=2, Plant=3 };

struct Projectile { sf::Vector2f pos{},vel{}; float damage=10,radius=7,life=2; Power power=Power::Spirit; bool enemy=false; };
struct Particle { sf::Vector2f pos{},vel{}; float life=.6f,size=4; Power power=Power::Spirit; };

class Enemy {
public:
 enum class Type { Slime,Bat,Salamander,LavaGolem,Piranha,Medusa,CarnivorousPlant,GiantInsect,Ghost,Specter,DeadKnight,Goblin,GoblinArcher,ShieldGoblin,GiantBat,RockWorm,Miner,LivingBook,DarkMage,InkSpirit,DarkKnight,MiniGolem,GuardianMage };
 Enemy(Type,sf::Vector2f); void update(float,sf::Vector2f,std::vector<Projectile>&); void draw(sf::RenderWindow&,sf::Vector2f) const; sf::FloatRect bounds() const; void hit(float); bool alive() const{return hp>0;}
 float hp,maxHp,contactDamage; Type type;
private: sf::Vector2f pos,vel; float attackTimer=0,phase=0;
};

class Player {
public:
 Player(); void reset(sf::Vector2f); void update(float,const std::vector<sf::FloatRect>&,std::vector<Projectile>&); void draw(sf::RenderWindow&,sf::Vector2f) const; sf::FloatRect bounds() const; void damage(float); void setPower(Power); void special(std::vector<Projectile>&,std::vector<Particle>&); sf::Vector2f position() const{return pos;}
 float hp=100,maxHp=100,specialCooldown=0; Power power=Power::Spirit; std::array<bool,4> unlocked{{true,false,false,false}}; bool dead=false;
private: sf::Vector2f pos,vel; bool grounded=false,ghost=false; float shootCooldown=0,invuln=0,ghostTimer=0;
};

class Boss {
public:
 explicit Boss(sf::Vector2f); void update(float,sf::Vector2f,std::vector<Projectile>&,std::vector<Particle>&); void draw(sf::RenderWindow&,sf::Vector2f) const; sf::FloatRect bounds() const; void hit(float); bool alive() const{return hp>0;}
 float hp=700,maxHp=700;
private: sf::Vector2f pos; float timer=0; int phase=1;
};

class Game {
public: Game(); void run();
private:
 void resetLevel(int); void nextLevel(); void update(float); void draw(); void buildLevel(); void spawnEnemies(); void drawHud(); void loadFont(); bool levelComplete() const;
 sf::RenderWindow window; sf::Font font; bool fontLoaded=false; Player player; std::unique_ptr<Boss> boss; std::vector<Enemy> enemies; std::vector<Projectile> projectiles; std::vector<Particle> particles; std::vector<sf::FloatRect> solids,hazards; int level=1; float worldWidth=3600; sf::Vector2f spawn{120,520}; bool victory=false,gameOver=false; sf::Clock clock;
};
