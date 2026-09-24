#include "Game.hpp"
#include <algorithm>
#include <cmath>
#include <cstdlib>

namespace {
sf::Color pc(Power p){ if(p==Power::Fire)return{255,90,35}; if(p==Power::Water)return{60,180,255}; if(p==Power::Silver)return{90,220,90}; return{190,150,255}; }
float clampf(float v,float a,float b){return std::max(a,std::min(b,v));}
void label(sf::RenderWindow&w,const sf::Font&f,const std::string&s,float x,float y,unsigned n,sf::Color c){sf::Text t(s,f,n);t.setPosition(x,y);t.setFillColor(c);w.draw(t);}
}

Enemy::Enemy(Type t,sf::Vector2f p):type(t){pos=p;maxHp=hp=35+(static_cast<int>(t)%7)*12;contactDamage=8+(static_cast<int>(t)%5)*3;if(t==Type::DeadKnight||t==Type::DarkKnight)hp=maxHp=120;if(t==Type::LavaGolem||t==Type::MiniGolem)hp=maxHp=150;}
sf::FloatRect Enemy::bounds()const{float s=(type==Type::LavaGolem||type==Type::MiniGolem||type==Type::DeadKnight||type==Type::DarkKnight)?58:40;return{pos.x-s/2,pos.y-s/2,s,s};}
void Enemy::hit(float d){hp-=d;}
void Enemy::update(float dt,sf::Vector2f pl,std::vector<Projectile>&shots){
 phase+=dt;attackTimer-=dt;float dx=pl.x-pos.x,dy=pl.y-pos.y;
 bool flyer=type==Type::Bat||type==Type::GiantBat;
 bool ranged=type==Type::Piranha||type==Type::CarnivorousPlant||type==Type::Medusa||type==Type::GoblinArcher||type==Type::DarkMage||type==Type::GuardianMage||type==Type::InkSpirit;
 if(flyer){pos.y+=std::sin(phase*3)*25*dt;if(std::abs(dx)<900)pos.x+=(dx>0?1:-1)*70*dt;}
 else if(ranged&&std::abs(dx)<750&&attackTimer<=0){Projectile q;q.enemy=true;q.pos=pos;q.vel={dx>0?250.f:-250.f,dy*.18f};q.damage=15;shots.push_back(q);attackTimer=1.4f;}
 else if(std::abs(dx)<650)pos.x+=clampf(dx,-1,1)*(45+(static_cast<int>(type)%4)*12)*dt;
 if((type==Type::Ghost||type==Type::Specter||type==Type::LivingBook)&&std::abs(dy)<160)pos.y+=std::sin(phase*2)*18*dt;
}
void Enemy::draw(sf::RenderWindow&w,sf::Vector2f cam)const{
 sf::Vector2f p=pos-cam;sf::RectangleShape r({bounds().width,bounds().height});r.setOrigin(r.getSize()/2.f);r.setPosition(p);
 sf::Color c{150,70,70};
 if(type==Type::Slime)c={100,210,130}; else if(type==Type::Bat||type==Type::GiantBat)c={100,70,150}; else if(type==Type::Salamander||type==Type::LavaGolem)c={235,90,35}; else if(type==Type::Piranha)c={220,80,110}; else if(type==Type::Medusa)c={130,180,230}; else if(type==Type::CarnivorousPlant)c={70,170,70}; else if(type==Type::GiantInsect)c={180,130,40}; else if(type==Type::Ghost||type==Type::Specter)c={190,190,255}; else if(type==Type::Goblin||type==Type::GoblinArcher||type==Type::ShieldGoblin)c={90,170,70}; else if(type==Type::LivingBook)c={170,70,100}; else if(type==Type::DarkMage||type==Type::GuardianMage)c={75,50,120}; else if(type==Type::InkSpirit)c={35,35,55}; else if(type==Type::RockWorm||type==Type::Miner)c={120,100,80}; else c={70,75,95};
 r.setFillColor(c);r.setOutlineThickness(2);r.setOutlineColor(sf::Color::Black);w.draw(r);
 sf::CircleShape e(4);e.setFillColor(sf::Color::White);e.setPosition(p.x-12,p.y-10);w.draw(e);e.setPosition(p.x+4,p.y-10);w.draw(e);
}

Player::Player(){reset({120,520});}
void Player::reset(sf::Vector2f p){pos=p;vel={0,0};hp=maxHp=100;dead=false;grounded=false;ghost=false;invuln=0;shootCooldown=0;specialCooldown=0;}
sf::FloatRect Player::bounds()const{return{pos.x-20,pos.y-30,40,60};}
void Player::damage(float d){if(invuln>0||dead)return;hp-=d;invuln=.8f;if(hp<=0){hp=0;dead=true;}}
void Player::setPower(Power p){if(unlocked[(int)p])power=p;}
void Player::special(std::vector<Projectile>&s,std::vector<Particle>&ps){
 if(specialCooldown>0||dead)return;specialCooldown=5;
 if(power==Power::Spirit){ghost=true;ghostTimer=1.5;}
 else if(power==Power::Fire){for(int i=0;i<16;i++){float a=6.28318f*i/16;Projectile q;q.pos=pos;q.vel={std::cos(a)*280.f,std::sin(a)*280.f};q.damage=24;q.power=power;s.push_back(q);}}
 else if(power==Power::Water){Projectile q;q.pos={pos.x+35,pos.y};q.vel={360,0};q.damage=35;q.radius=16;q.life=3;q.power=power;s.push_back(q);}
 else for(int i=-2;i<=2;i++){Projectile q;q.pos={pos.x+25,pos.y+i*15.f};q.vel={220,0};q.damage=28;q.radius=10;q.power=power;s.push_back(q);}
 for(int i=0;i<12;i++){Particle p;p.pos=pos;p.vel={(float)(std::rand()%240-120),(float)(std::rand()%240-120)};p.power=power;ps.push_back(p);}
}
void Player::update(float dt,const std::vector<sf::FloatRect>&solids,std::vector<Projectile>&shots){
 if(dead)return;shootCooldown-=dt;specialCooldown-=dt;invuln-=dt;
 if(ghostTimer>0){ghostTimer-=dt;if(ghostTimer<=0)ghost=false;}
 float move=0;if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))move--;if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))move++;
 vel.x=move*260; if((sf::Keyboard::isKeyPressed(sf::Keyboard::W)||sf::Keyboard::isKeyPressed(sf::Keyboard::Space))&&grounded){vel.y=-520;grounded=false;}
 vel.y+=1200*dt;pos.x+=vel.x*dt;auto b=bounds();for(auto&s:solids)if(b.intersects(s)){if(vel.x>0)pos.x=s.left-20;else if(vel.x<0)pos.x=s.left+s.width+20;}
 pos.y+=vel.y*dt;grounded=false;b=bounds();for(auto&s:solids)if(b.intersects(s)){if(vel.y>0){pos.y=s.top-30;vel.y=0;grounded=true;}else{pos.y=s.top+s.height+30;vel.y=0;}}
 if(pos.y>760)damage(999);
 if(sf::Keyboard::isKeyPressed(sf::Keyboard::J)&&shootCooldown<=0){Projectile q;q.pos={pos.x+24,pos.y-5};q.vel={move<0?-520.f:520.f,0};q.damage=power==Power::Fire?18:12;q.power=power;shots.push_back(q);shootCooldown=.25;}
}
void Player::draw(sf::RenderWindow&w,sf::Vector2f cam)const{
 auto p=pos-cam;sf::CircleShape b(25);b.setPosition(p.x-25,p.y-5);b.setFillColor(pc(power));b.setOutlineThickness(3);b.setOutlineColor(sf::Color::White);w.draw(b);
 sf::RectangleShape t({34,28});t.setPosition(p.x-17,p.y-18);t.setFillColor({225,205,180});w.draw(t);sf::CircleShape h(18);h.setPosition(p.x-18,p.y-45);h.setFillColor({238,205,175});w.draw(h);
}

Boss::Boss(sf::Vector2f p):pos(p){}
sf::FloatRect Boss::bounds()const{return{pos.x-80,pos.y-100,160,200};}
void Boss::hit(float d){hp=std::max(0.f,hp-d);phase=hp<maxHp*.5?2:1;}
void Boss::update(float dt,sf::Vector2f pl,std::vector<Projectile>&s,std::vector<Particle>&ps){
 timer+=dt;float dx=pl.x-pos.x;if(std::abs(dx)>180)pos.x+=(dx>0?1:-1)*(phase==2?95.f:65.f)*dt;
 if(timer>1.4){timer=0;Projectile q;q.enemy=true;q.pos={pos.x,pos.y-50};q.vel={dx>0?180.f:-180.f,-80};q.damage=22;q.radius=13;s.push_back(q);
 if(phase==2)for(int i=-2;i<=2;i++){Projectile r;r.enemy=true;r.pos={pos.x+i*35,pos.y-100};r.vel={i*50.f,260};r.damage=18;s.push_back(r);}
 for(int i=0;i<10;i++){Particle p;p.pos=pos;p.vel={(float)(std::rand()%300-150),(float)(std::rand()%300-150)};ps.push_back(p);}}
}
void Boss::draw(sf::RenderWindow&w,sf::Vector2f cam)const{
 auto p=pos-cam;sf::CircleShape b(78);b.setPosition(p.x-78,p.y-55);b.setFillColor({105,105,115});b.setOutlineThickness(5);b.setOutlineColor({55,55,65});w.draw(b);
 sf::RectangleShape a({35,120});a.setPosition(p.x-105,p.y-25);a.setFillColor({85,85,95});w.draw(a);a.setPosition(p.x+70,p.y-25);w.draw(a);
 sf::CircleShape e(11);e.setFillColor({255,70,50});e.setPosition(p.x-35,p.y-45);w.draw(e);e.setPosition(p.x+15,p.y-45);w.draw(e);
}

Game::Game():window(sf::VideoMode(1280,720),"Mazmorra Slime"){window.setFramerateLimit(60);loadFont();resetLevel(1);}
void Game::loadFont(){const char*ps[]={"assets/fonts/DejaVuSans.ttf","C:/Windows/Fonts/arial.ttf","/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf","/usr/share/fonts/truetype/liberation2/LiberationSans-Regular.ttf"};for(auto p:ps)if(font.loadFromFile(p)){fontLoaded=true;break;}}
void Game::buildLevel(){worldWidth=3400+level*180;solids.push_back({0,620,worldWidth,100});for(int i=0;i<12;i++)solids.push_back({250+i*270+(level%3)*40,500-(i%3)*80,180,25});for(int i=0;i<7;i++)solids.push_back({700+i*420,380-(i%2)*100,130,22});}
void Game::spawnEnemies(){
 using T=Enemy::Type;std::vector<T>ts;
 switch(level){case 1:ts={T::Slime,T::Bat};break;case 2:ts={T::LavaGolem,T::Salamander};break;case 3:ts={T::Piranha,T::Medusa};break;case 4:ts={T::CarnivorousPlant,T::GiantInsect};break;case 5:ts={T::Ghost,T::Specter,T::DeadKnight};break;case 6:ts={T::Goblin,T::GoblinArcher,T::ShieldGoblin};break;case 7:ts={T::GiantBat,T::RockWorm,T::Miner};break;case 8:ts={T::LivingBook,T::DarkMage,T::InkSpirit};break;default:ts={T::DarkKnight,T::MiniGolem,T::GuardianMage};}
 if(level<10)for(int i=0;i<10+level;i++)enemies.emplace_back(ts[i%ts.size()],{500.f+i*260.f,450.f-(i%3)*80.f});else boss=std::make_unique<Boss>(sf::Vector2f(worldWidth-500,500));
 player.unlocked[0]=true;if(level>=3)player.unlocked[1]=true;
if(level>=5)player.unlocked[2]=true;
if(level>=7)player.unlocked[3]=true;
}
void Game::resetLevel(int n){level=n;gameOver=victory=false;boss.reset();enemies.clear();projectiles.clear();particles.clear();solids.clear();player.reset(spawn);buildLevel();spawnEnemies();}
void Game::nextLevel(){if(level<10)resetLevel(level+1);else victory=true;}
bool Game::levelComplete()const{return level==10?(boss&&!boss->alive()):(enemies.empty()&&player.position().x>worldWidth-180);}
void Game::update(float dt){
 if(gameOver||victory){if(sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))resetLevel(1);return;}
 if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))player.setPower(Power::Spirit);if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))player.setPower(Power::Fire);if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))player.setPower(Power::Water);if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num4))player.setPower(Power::Silver);
 static bool prevK=false;bool k=sf::Keyboard::isKeyPressed(sf::Keyboard::K);if(k&&!prevK)player.special(projectiles,particles);prevK=k;
 player.update(dt,solids,projectiles);for(auto&e:enemies)e.update(dt,player.position(),projectiles);if(boss)boss->update(dt,player.position(),projectiles,particles);
 for(auto&p:projectiles){p.pos+=p.vel*dt;p.life-=dt;if(p.life>0&&!p.enemy){for(auto&e:enemies)if(e.alive()&&e.bounds().contains(p.pos)){e.hit(p.damage);p.life=0;}if(boss&&boss->alive()&&boss->bounds().contains(p.pos)){boss->hit(p.damage);p.life=0;}}else if(p.enemy&&p.life>0&&player.bounds().contains(p.pos)){player.damage(p.damage);p.life=0;}}
 projectiles.erase(std::remove_if(projectiles.begin(),projectiles.end(),[](auto&p){return p.life<=0;}),projectiles.end());enemies.erase(std::remove_if(enemies.begin(),enemies.end(),[](auto&e){return !e.alive();}),enemies.end());
 for(auto&p:particles){p.pos+=p.vel*dt;p.life-=dt;}particles.erase(std::remove_if(particles.begin(),particles.end(),[](auto&p){return p.life<=0;}),particles.end());
 if(player.dead)gameOver=true;if(levelComplete())nextLevel();
}
void Game::drawHud(){if(!fontLoaded)return;sf::RectangleShape b({300,24});b.setPosition(20,20);b.setFillColor({45,45,50});window.draw(b);b.setSize({300.f*player.hp/player.maxHp,24});b.setFillColor({70,210,90});window.draw(b);label(window,font,"HP "+std::to_string((int)player.hp)+"/100",28,18,16,sf::Color::White);label(window,font,"Nivel "+std::to_string(level)+"/10",20,55,22,sf::Color::White);const char*n[]={"ESPIRITU","FUEGO","AGUA","PLATA"};for(int i=0;i<4;i++)label(window,font,std::to_string(i+1)+" "+n[i],20+i*150,90,16,player.unlocked[i]?pc((Power)i):sf::Color(100,100,100));label(window,font,"A/D mover  W/SPACE saltar  J ataque  K especial  1-4 poderes  ESC salir",20,675,16,sf::Color(220,220,220));if(boss&&boss->alive()){sf::RectangleShape q({600,22});q.setPosition(340,20);q.setFillColor({55,55,55});window.draw(q);q.setSize({600.f*boss->hp/boss->maxHp,22});q.setFillColor({190,70,60});window.draw(q);label(window,font,"GOLEM ANCESTRAL",500,45,18,sf::Color::White);}}
void Game::draw(){
 sf::Color bg={18,20,30};if(level==2)bg={45,25,25};if(level==3)bg={20,45,65};if(level==4)bg={25,55,30};if(level==5)bg={25,25,50};if(level==8)bg={35,20,45};window.clear(bg);
 float cam=clampf(player.position().x-420,0.f,std::max(0.f,worldWidth-1280.f));for(auto&s:solids){sf::RectangleShape r({s.width,s.height});r.setPosition(s.left-cam,s.top);r.setFillColor({65,60,65});r.setOutlineThickness(2);r.setOutlineColor({100,100,115});window.draw(r);}
 for(auto&e:enemies)e.draw(window,{cam,0});if(boss)boss->draw(window,{cam,0});
 for(auto&p:projectiles){sf::CircleShape r(p.radius);r.setPosition(p.pos.x-cam-p.radius,p.pos.y-p.radius);r.setFillColor(p.enemy?sf::Color(255,70,70):pc(p.power));window.draw(r);}
 for(auto&p:particles){sf::CircleShape r(p.size);r.setPosition(p.pos.x-cam-p.size,p.pos.y-p.size);r.setFillColor(pc(p.power));window.draw(r);}
 player.draw(window,{cam,0});drawHud();
 if(fontLoaded&&(gameOver||victory)){sf::RectangleShape s({1280,720});s.setFillColor({0,0,0,180});window.draw(s);label(window,font,victory?"¡VICTORIA!":"GAME OVER",450,280,52,victory?sf::Color(100,240,140):sf::Color(240,90,80));label(window,font,"Pulsa ENTER para comenzar de nuevo",410,350,22,sf::Color::White);}
}
void Game::run(){while(window.isOpen()){sf::Event e;while(window.pollEvent(e)){if(e.type==sf::Event::Closed)window.close();if(e.type==sf::Event::KeyPressed&&e.key.code==sf::Keyboard::Escape)window.close();}float dt=std::min(clock.restart().asSeconds(),.033f);update(dt);draw();window.display();}}
