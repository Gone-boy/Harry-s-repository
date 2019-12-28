#pragma once
#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
using namespace cocos2d::ui;
class game : public cocos2d::Scene
{
	cocos2d::ValueVector list;
public:
	static cocos2d::Scene* createScene();
	int judge(int a,int b);
	int judge2(int a,int b);
	int search();
	int search2();
	void change(int a,int b);
	void change2(int a,int b);
	void clear();
	int score();
	void turnback(int a, int b);
	void turnback2(int a, int b);
	virtual bool init();
	virtual void update(float dt);
	cocos2d::LabelTTF *black2;
	cocos2d::LabelTTF *white2;
	// a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);
	// implement the "static create()" method manually
	CREATE_FUNC(game);
};

#endif // __GAME_SCENE_H__

