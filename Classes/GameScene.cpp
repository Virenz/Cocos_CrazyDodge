#include "GameScene.h"
#include "GameOverScene.h"

USING_NS_CC;

static int CIRCLE = 0;
static int SQUARE = 1;
static int RECT_SHU = 2;
static int RECT_HENG = 3;
static double LIMIT_SPEED = 1000;

Size WIN_SIZE = Size(0, 0);
Vec2 ORIGIN = Vec2(0, 0);

#define MUSIC "Music/bgMusic.wav"


Scene* GameScene::createScene()
{
    auto scene = Scene::createWithPhysics();
	auto layer = GameScene::create();
	scene->addChild(layer);
	
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	scene->getPhysicsWorld()->setGravity(Vec2(0, 0));
	
	return scene;
}


bool GameScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
	
	// ��ȡ��Ļ��С
	WIN_SIZE = Director::getInstance()->getVisibleSize();
	ORIGIN = Director::getInstance()->getVisibleOrigin();

	this->resetMusic();
	this->addUI();
	this->addEdge();
	this->addPlayer();
	this->addEnemy();
	this->addTouchEvent();
	this->addContactEvent();
    
	this->score = 0.0;
	this->firstTouch = true;
	this->isOver = false;

    return true;
}


// �������
void GameScene::resetMusic()
{
	bool state = UserDefault::getInstance()->getBoolForKey("Music", true);
	if (state == true)
	{
		SimpleAudioEngine::getInstance()->stopBackgroundMusic();
		SimpleAudioEngine::getInstance()->playBackgroundMusic(MUSIC, true);
	}
}


// ��ӽ���Ԫ��
void GameScene::addUI()
{
	auto bg = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("GameUI/GameUI.ExportJson");
	this->addChild(bg);
	
	this->lbScore = (TextBMFont*)ui::Helper::seekWidgetByName(bg, "Score");
}


// ��ӱ߽�
void GameScene::addEdge()
{
	this->edge = Sprite::create();
	auto body = PhysicsBody::createEdgeBox(WIN_SIZE, PhysicsMaterial(100.0, 1.0, 0.0), 5.0f);
	this->edge->setPhysicsBody(body);
	this->edge->setPosition(WIN_SIZE / 2);
	this->addChild(this->edge);

	this->edge->getPhysicsBody()->setCategoryBitmask(4);
	this->edge->getPhysicsBody()->setCollisionBitmask(2);
	this->edge->getPhysicsBody()->setContactTestBitmask(1);
}


// ������
void GameScene::addPlayer()
{
	this->player = Sprite::create("Player.png");
	auto body = PhysicsBody::createCircle(15.0f, PhysicsMaterial(0.0, 0.0, 0.0));
	this->player->setPhysicsBody(body);
	this->player->setPosition(WIN_SIZE / 2);
	this->addChild(this->player);

	this->player->getPhysicsBody()->setCategoryBitmask(1);
	this->player->getPhysicsBody()->setCollisionBitmask(0);
	this->player->getPhysicsBody()->setContactTestBitmask(6);
}


// ��ӵ���
void GameScene::addEnemy()
{
	this->enemy.clear();

	// Բ��
	auto circle = Sprite::create("Circle.png");
	auto circle_body = PhysicsBody::createCircle(20, PhysicsMaterial(0.0, 1.0, 0.0));
	circle->setPhysicsBody(circle_body);
	circle->setPosition(100, WIN_SIZE.height - 100);
	this->enemy.pushBack(circle);

	// ������
	auto square = Sprite::create("Square.png");
	auto square_body = PhysicsBody::createBox(Size(50, 50), PhysicsMaterial(0.0, 1.0, 0.0));
	square->setPhysicsBody(square_body);
	square->setPosition(WIN_SIZE.width - 100, WIN_SIZE.height - 100);
	this->enemy.pushBack(square);

	// ������:��
	auto rect_shu = Sprite::create("Rect_Shu.png");
	auto rect_shu_body = PhysicsBody::createBox(Size(20, 50), PhysicsMaterial(0.0, 1.0, 0.0));
	rect_shu->setPhysicsBody(rect_shu_body);
	rect_shu->setPosition(100, 100);
	this->enemy.pushBack(rect_shu);

	// ������:��
	auto rect_heng = Sprite::create("Rect_Heng.png");
	auto rect_heng_body = PhysicsBody::createBox(Size(60, 20), PhysicsMaterial(0.0, 1.0, 0.0));
	rect_heng->setPhysicsBody(rect_heng_body);
	rect_heng->setPosition(WIN_SIZE.width - 100, 100);
	this->enemy.pushBack(rect_heng);

	for (int i = 0; i < this->enemy.size(); i++)
	{
		Sprite* e = this->enemy.at(i);
		e->setScale(0.2f);
		e->getPhysicsBody()->setCategoryBitmask(2);
		e->getPhysicsBody()->setCollisionBitmask(6);
		e->getPhysicsBody()->setContactTestBitmask(1);
		this->addChild(e, 0, i);
	}
}


// ��Ӵ����¼�
void GameScene::addTouchEvent()
{
	auto dispatcher = this->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}


// �����ײ���
void GameScene::addContactEvent()
{
	auto dispatcher = this->getEventDispatcher();
	auto listener = EventListenerPhysicsContact::create();
	listener->onContactBegin = CC_CALLBACK_1(GameScene::onContactBegin, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}


// ��ʼ��������Ϸ��ʼ
bool GameScene::onTouchBegan(Touch *touch, Event *unused_event)
{
	//CCLOG("TouchBegan");
	if (this->firstTouch == true) 
	{
		this->firstTouch = false;
		for (int i = 0; i < this->enemy.size(); i++) 
		{
			Sprite* e = this->enemy.at(i);
			if (i==0) {	// Բ
				e->getPhysicsBody()->setVelocity(Vect(100, -100));
			}else if (i==1) { // ������
				e->getPhysicsBody()->setVelocity(Vect(-100, -100));
			}else if (i==2) { // ������
				e->getPhysicsBody()->setVelocity(Vect(100, 100));
			}else if (i==3) { // ���κ�
				e->getPhysicsBody()->setVelocity(Vect(-100, 100));
			}
		}
		scheduleUpdate();
	}
	return true;
}


// �����ƶ�
void GameScene::onTouchMoved(Touch *touch, Event *unused_event)
{
	//CCLOG("TouchMoved");
	Vec2 delta = touch->getDelta();
	this->player->setPosition(this->player->getPosition() + delta);
}


// ��ײ���
bool GameScene::onContactBegin(PhysicsContact& contact)
{
	this->isOver = true;
	return true;
}


// ��ʱ��ˢ��
void GameScene::update(float dt)
{
	if (this->isOver == true)
	{
		this->unscheduleUpdate();
		this->turnToGameOverScene();
		return;
	}

	this->score += dt;

	char str[50];
	sprintf(str, "%.3f", this->score);
	this->lbScore->setString(str);

	for (int i = 0; i < this->enemy.size(); i++) 
	{
		Sprite* e = this->enemy.at(i);
		
		// ��С
		float scale = e->getScale() + dt / 100;
		if (scale >= 1.5f) scale = 1.5f;
		e->setScale(scale);

		// �ٶ�
		double minSpeed = this->score * 10 + 100;
		Vec2 speed = e->getPhysicsBody()->getVelocity();
		float length = sqrt(speed.x*speed.x + speed.y*speed.y);

		if (minSpeed >= LIMIT_SPEED) minSpeed = LIMIT_SPEED;
		if (length < minSpeed)
		{
			float d = minSpeed / length;
			speed *= d;
		}
		if (length > LIMIT_SPEED)
		{
			float d = LIMIT_SPEED / length;
			speed *= d;
		}
		if (fabs(speed.x) <= 10.0) speed.x = (speed.x < 0.0f) ? -100.0f : 100.0f;
		if (fabs(speed.y) <= 10.0) speed.y = (speed.y < 0.0f) ? -100.0f : 100.0f;

		e->getPhysicsBody()->setVelocity(Vect(speed.x, speed.y));

		// λ��
		Vect pos = e->getPosition();
		if (this->isOutScreen(pos) == true)
		{
			pos = pos.getClampPoint(Vec2(50, 50), Vec2(WIN_SIZE.width - 50, WIN_SIZE.height - 50));
		}
		e->setPosition(pos);
	}
}


// �Ƿ񳬳���Ļ
bool GameScene::isOutScreen(Vec2 pos)
{
	return (pos.x<0 || pos.x>WIN_SIZE.width || pos.y<0 || pos.y>WIN_SIZE.height);
}


// ��Ϸ����
void GameScene::turnToGameOverScene()
{
	// ��������
	UserDefault::getInstance()->setDoubleForKey("Score", this->score);
	UserDefault::getInstance()->flush();

	auto scene = GameOverScene::createScene();
	auto tt = TransitionFlipX::create(0.5, scene);
	Director::getInstance()->replaceScene(tt);
}

