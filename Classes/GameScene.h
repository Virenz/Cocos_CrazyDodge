#ifndef __GameScene_H__
#define __GameScene_H__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "editor-support/cocostudio/CCSGUIReader.h"


USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;
using namespace cocostudio;
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

class GameScene : public cocos2d::Layer
{
public:
    static Scene* createScene();
    virtual bool init();  
    void menuCloseCallback(Ref* pSender);
	CREATE_FUNC(GameScene);

	void resetMusic();
	void addUI();
	void addEdge();
	void addPlayer();
	void addEnemy();
	void addContactEvent();
	void addTouchEvent();

	bool onTouchBegan(Touch *touch, Event *unused_event);
	void onTouchMoved(Touch *touch, Event *unused_event);
	bool onContactBegin(PhysicsContact& contact);

	void update(float dt);

	bool isOutScreen(Vec2 pos);

	void turnToGameOverScene();

private:
	Sprite* edge;			// �߽�
	Sprite* player;			// ���
	Vector<Sprite*> enemy;	// ����

	TextBMFont* lbScore;	// ��ʾ����
	double score;			// ����
	bool firstTouch;		// �Ƿ�ʼ��Ϸ
	bool isOver;			// ��Ϸ�Ƿ����

};

#endif // __GameScene_H__
