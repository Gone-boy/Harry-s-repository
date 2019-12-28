#include "gamescene.h"
#include "SimpleAudioEngine.h"
#include "ui/CocosGUI.h"
#include "HelloWorldScene.h"
#include "menu.h"
using namespace cocos2d::ui;
USING_NS_CC;
Scene* nunu::createScene() {
	return nunu::create();
}
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}
bool nunu::init() {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	if (!Scene::init())
	{
		return false;
	}
	auto initpic = Sprite::create("menu.png");
	initpic->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(initpic,0);
	auto titile = Sprite::create("biaoti.png");
	titile->setPosition(Vec2(500, 550));
	this->addChild(titile,0);
	auto button = Button::create("pvp.jpg");
	button->setPosition(Vec2(500, 412));
	button->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
		switch (type)
		{
		case ui::Widget::TouchEventType::BEGAN:
			break;
		case ui::Widget::TouchEventType::ENDED:
			Director::getInstance()->replaceScene(HelloWorld::createScene());
			break;
		}
	});
	this->addChild(button);
	auto button2 = Button::create("pve.jpg");
	button2->setPosition(Vec2(500, 345));
	button2->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
		switch (type)
		{
		case ui::Widget::TouchEventType::BEGAN:
			break;
		case ui::Widget::TouchEventType::ENDED:
			Director::getInstance()->replaceScene(game::createScene());
			break;
		}
	});
	this->addChild(button2);
}
