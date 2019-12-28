#pragma once
#ifndef __MENU_H__
#define __MENU_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
using namespace cocos2d::ui;
class nunu : public cocos2d::Scene {
public :
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(nunu);
};
#endif // __Menu_H__
