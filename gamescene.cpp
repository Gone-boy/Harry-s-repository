#include "gamescene.h"
#include "SimpleAudioEngine.h"
#include "ui/CocosGUI.h"
#include "HelloWorldScene.h"
#include "menu.h"
using namespace cocos2d::ui;
USING_NS_CC;
struct AICHESS
{
	int changenum = 0;
};
struct AICHESS record2[8][8];
int board2[8][8] = { 0 };
char gamestate2 = 'b';
int dx2[] = { -1,-1,-1,1,1,1,0,0 };
int dy2[] = { -1,0,1,-1,0,1,1,-1 };
int bnum = 0;
int wnum = 0;
int allb2 = 0;
int allw2 = 0;
ValueVector val2;
ValueVector cord2;
Scene* game::createScene()
{
	return game::create();
}
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}
bool game::init()
{
	//////////////////////////////
	// 1. super init first
	
	if (!Scene::init())
	{
		return false;
	}
	for (int i = 0;i < 8;i++) {
		for (int j = 0;j < 8;j++) {
			if (board2[i][j] != 0) {
				board2[i][j] = 0;
			}
			log("%d", board2[i][j]);
		}
	}
	if (gamestate2 != 'b') {
		gamestate2 = 'b';
	}
	if (allb2 != 0) {
		allb2 = 0;
	}
	if (allw2 != 0) {
		allw2 = 0;
	}
	for (int i = 0;i < 8;i++) {
		for (int j = 0;j < 8;j++) {
			if (record2[i][j].changenum != 0) {
				record2[i][j].changenum = 0;
			}
		}
	}
	auto listener1 = EventListenerTouchOneByOne::create();
	listener1->onTouchBegan = [this](Touch* touch, Event* event)
	{
		int x = touch->getLocation().x;
		int y = touch->getLocation().y;
		int bnumber = 0;
		int wnumber = 0;
		if (gamestate2 == 'b') {
			for (int i = 1;i <= 8;i++) {
				for (int j = 1;j <= 8;j++) {
					if (abs(x - (i * 50 + 275)) < 24 && abs(y - (j * 50 + 174)) < 24) {
						int getx = i * 50 + 275;
						int gety = j * 50 + 174;
						if (board2[8 - j][i - 1] != 3) {
							gamestate2 = 'b';
						}
						if (board2[8 - j][i - 1] == 3) {
							clear();
							auto sprite2 = Sprite::create("wb5.png");
							if (sprite2 == nullptr)
							{
								problemLoading("'HelloWorld.png'");
							}
							else
							{
								// position the sprite on the center of the screen
								sprite2->setPosition(Vec2(getx, gety));

								// add the sprite as a child to this layer

								this->addChild(sprite2, 0, (8 - j) * 8 + (i - 1));
								val2.push_back(Value((8 - j) * 8 + (i - 1)));
							}
							board2[8 - j][i - 1] = 1;
							change(8 - j, i - 1);
							for (int i = 0;i < 8;i++) {
								for (int j = 0;j < 8;j++) {
									if (board2[i][j] == 3) {
										board2[i][j] = 0;
									}
								}
							}
							wnum = search2();
							gamestate2 = 'w';
							if (wnum == 0)
							{
								gamestate2 = 'b';
								bnum = search();
								if (bnum == 0 && wnum == 0) {
									for (int i = 0;i < 8;i++) {
										for (int j = 0;j < 8;j++) {
											if (board2[i][j] == 1)
												bnumber++;
											if (board2[i][j] == 2)
												wnumber++;
										}
									}
									if (bnumber > wnumber) {
										auto blackwin = Sprite::create("ResultBlack.png");
										blackwin->setPosition(Vec2(100, 100));
										// add the sprite as a child to this layer
										this->addChild(blackwin, 0);
									}
									if (bnumber < wnumber) {
										auto whitewin = Sprite::create("ResultWhite.png");
										whitewin->setPosition(Vec2(100, 100));
										// add the sprite as a child to this layer
										this->addChild(whitewin, 0);
									}
								}
								log("white pass");
							}
							allb2 = 0;
							allw2 = 0;
							for (int i = 0;i < 8;i++) {
								for (int j = 0;j < 8;j++) {
									if (board2[i][j] == 1)
										allb2++;
									if (board2[i][j] == 2)
										allw2++;
								}
							}
						}
					}
				}
			}
		}
		if (gamestate2 == 'w') {
			auto delaytime = DelayTime::create(0.5f);
			auto func = CallFunc::create([this]() {
				int got = score();
				int gotx = got / 8;
				int goty = got % 8;
				clear();
				auto sprite2 = Sprite::create("wb1.png");
				if (sprite2 == nullptr)
				{
					problemLoading("'HelloWorld.png'");
				}
				else
				{
					// position the sprite on the center of the screen
					sprite2->setPosition(Vec2((goty + 1) * 50 + 275, (8 - gotx) * 50 + 174));

					// add the sprite as a child to this layer

					this->addChild(sprite2, 0, gotx * 8 + goty);
					val2.push_back(Value(gotx * 8 + goty));
				}
				board2[gotx][goty] = 2;
				change2(gotx, goty);
				for (int i = 0;i < 8;i++) {
					for (int j = 0;j < 8;j++) {
						if (board2[i][j] == 3) {
							board2[i][j] = 0;
						}
					}
				}
				bnum = search();
				gamestate2 = 'b';
				if (bnum == 0)
				{
					int number1 = 0;
					int number2 = 0;
					gamestate2 = 'w';
					wnum = search2();
					if (bnum == 0 && wnum == 0) {
						for (int i = 0;i < 8;i++) {
							for (int j = 0;j < 8;j++) {
								if (board2[i][j] == 1)
									number1++;
								if (board2[i][j] == 2)
									number2++;
							}
						}
						if (number1 > number2) {
							auto blackwin = Sprite::create("ResultBlack.png");
							blackwin->setPosition(Vec2(100, 100));
							// add the sprite as a child to this layer
							this->addChild(blackwin, 0);
						}
						if (number1 < number2) {
							auto whitewin = Sprite::create("ResultWhite.png");
							whitewin->setPosition(Vec2(100, 100));
							// add the sprite as a child to this layer
							this->addChild(whitewin, 0);
						}
					}
					log("black pass");
				}
				allb2 = 0;
				allw2 = 0;
				for (int i = 0;i < 8;i++) {
					for (int j = 0;j < 8;j++) {
						if (board2[i][j] == 1)
							allb2++;
						if (board2[i][j] == 2)
							allw2++;
					}
				}
		     });
			auto seq = Sequence::create(delaytime, func, nullptr);
			this->runAction(seq);
		}
		int allbnum = 0;
		int allwnum = 0;
		for (int i = 0;i < 8;i++) {
			for (int j = 0;j < 8;j++) {
				if (board2[i][j] == 1)
					allbnum++;
				if (board2[i][j] == 2)
					allwnum++;
			}
		}	
		
		return true;
	};

	listener1->onTouchEnded = [](Touch* touch, Event* event)
	{

	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, this);

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	/////////////////////////////
	// 2. add a menu item with "X" image, which is clicked to quit the program
	//    you may modify it.

	// add a "close" icon to exit the progress. it's an autorelease object
	auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(game::menuCloseCallback, this));

	if (closeItem == nullptr ||
		closeItem->getContentSize().width <= 0 ||
		closeItem->getContentSize().height <= 0)
	{
		problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
	}
	else
	{
		float x = origin.x + visibleSize.width - closeItem->getContentSize().width / 2;
		float y = origin.y + closeItem->getContentSize().height / 2;
		closeItem->setPosition(Vec2(x, y));
	}

	// create menu, it's an autorelease object
	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	/////////////////////////////
	// 3. add your codes below...

	// add a label shows "Hello World"
	// create and initialize a label

	auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
	if (label == nullptr)
	{
		problemLoading("'fonts/Marker Felt.ttf'");
	}
	else
	{
		// position the label on the center of the screen
		label->setPosition(Vec2(origin.x + visibleSize.width / 2,
			origin.y + visibleSize.height - label->getContentSize().height));

		// add the label as a child to this layer
		this->addChild(label, 1);
	}

	// add "HelloWorld" splash screen"

	auto sprite = Sprite::create("MainWnd1.png");
	if (sprite == nullptr)
	{
		problemLoading("'HelloWorld.png'");
	}
	else
	{
		// position the sprite on the center of the screen
		sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

		// add the sprite as a child to this layer
		this->addChild(sprite, 0);
	}
	auto initblack = Sprite::create("ChessmanBlack1.png");
	if (initblack == nullptr)
	{
		problemLoading("'HelloWorld.png'");
	}
	else
	{
		// position the sprite on the center of the screen
		initblack->setPosition(Vec2(5 * 50 + 275, 5 * 50 + 174));
		// add the sprite as a child to this layer
		this->addChild(initblack, 1, 3 * 8 + 4);
		board2[3][4] = 1;
	}
	auto initwhite = Sprite::create("ChessmanWhite1.png");
	if (sprite == nullptr)
	{
		problemLoading("'HelloWorld.png'");
	}
	else
	{
		// position the sprite on the center of the screen
		initwhite->setPosition(Vec2(4 * 50 + 275, 5 * 50 + 174));
		// run it 
		// add the sprite as a child to this layer
		this->addChild(initwhite, 1, 3 * 8 + 3);
		board2[3][3] = 2;
	}
	auto initblack2 = Sprite::create("ChessmanBlack1.png");
	if (initblack == nullptr)
	{
		problemLoading("'HelloWorld.png'");
	}
	else
	{
		// position the sprite on the center of the screen
		initblack2->setPosition(Vec2(4 * 50 + 275, 4 * 50 + 174));
		// add the sprite as a child to this layer
		this->addChild(initblack2, 1, 4 * 8 + 3);
		board2[4][3] = 1;
	}
	auto initwhite2 = Sprite::create("ChessmanWhite1.png");
	if (sprite == nullptr)
	{
		problemLoading("'HelloWorld.png'");
	}
	else
	{
		// position the sprite on the center of the screen

		initwhite2->setPosition(Vec2(5 * 50 + 275, 4 * 50 + 174));
		// add the sprite as a child to this layer
		this->addChild(initwhite2, 1, 4 * 8 + 4);
		board2[4][4] = 2;
	}
	search();
	auto button = Button::create("huiqi.png");
	button->setPosition(Vec2(415,159));
	button->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
		switch (type)
		{
		case ui::Widget::TouchEventType::BEGAN:
			break;
		case ui::Widget::TouchEventType::ENDED:
			int a2 = val2.back().asInt();
			if (board2[a2 / 8][a2 % 8] == 2) {
				turnback2((a2 / 8), (a2 % 8));
				clear();
				for (int i = 0;i < 8;i++) {
					for (int j = 0;j < 8;j++) {
						if (board2[i][j] == 3) {
							board2[i][j] = 0;
						}
					}
				}
				board2[a2 / 8][a2 % 8] = 0;
				search2();
				removeChildByTag(a2);
				val2.pop_back();
				int a3 = val2.back().asInt();
				turnback((a3 / 8), (a3 % 8));
				clear();
				for (int i = 0;i < 8;i++) {
					for (int j = 0;j < 8;j++) {
						if (board2[i][j] == 3) {
							board2[i][j] = 0;
						}
					}
				}
				board2[a3 / 8][a3 % 8] = 0;
				search();
				removeChildByTag(a3);
				val2.pop_back();
				gamestate2 = 'b';
			}
			if (board2[a2 / 8][a2 % 8] == 1) {
				turnback((a2 / 8), (a2 % 8));
				clear();
				for (int i = 0;i < 8;i++) {
					for (int j = 0;j < 8;j++) {
						if (board2[i][j] == 3) {
							board2[i][j] = 0;
						}
					}
				}
				board2[a2 / 8][a2 % 8] = 0;
				search();
				removeChildByTag(a2);
				val2.pop_back();
				gamestate2 = 'b';
			}
			allb2 = 0;
			allw2 = 0;
			for (int i = 0;i < 8;i++) {
				for (int j = 0;j < 8;j++) {
					if (board2[i][j] == 1)
						allb2++;
					if (board2[i][j] == 2)
						allw2++;
				}
			}
			break;
		}
	});
	this->addChild(button);
	auto button2= Button::create("lose.png");
	button2->setPosition(Vec2(495, 159));
	button2->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
		switch (type)
		{
		case ui::Widget::TouchEventType::BEGAN:
			break;
		case ui::Widget::TouchEventType::ENDED:
			gamestate2 = 'over';
			int white = 0;
			int black = 0;
			for (int i=0;i < 8;i++) {
				for (int j=0;j < 8;j++) {
					if (board2[i][j] == 1)
						black++;
					if (board2[i][j] == 2)
						white++;
				}
			}
			if (black > white) {
				auto blackwin = Sprite::create("ResultBlack.png");
				blackwin->setPosition(Vec2(100, 100));
				// add the sprite as a child to this layer
				this->addChild(blackwin, 0);
			}
			if (black < white) {
				auto whitewin = Sprite::create("ResultWhite.png");
				whitewin->setPosition(Vec2(100, 100));
				// add the sprite as a child to this layer
				this->addChild(whitewin, 0);
			}
			if (black == white) {
				auto whitewin = Sprite::create("ResultPeace.png");
				whitewin->setPosition(Vec2(100, 100));
				// add the sprite as a child to this layer
				this->addChild(whitewin, 0);
			}
			break;
		}
	});
	this->addChild(button2);
	auto button3 = Button::create("exit.jpg");
	button3->setPosition(Vec2(575, 159));
	button3->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
		switch (type)
		{
		case ui::Widget::TouchEventType::BEGAN:
			break;
		case ui::Widget::TouchEventType::ENDED:
			Director::getInstance()->replaceScene(nunu::createScene());
			break;
		}
	});
	this->addChild(button3);
	for (int i = 0;i < 8;i++) {
		for (int j = 0;j < 8;j++) {
			if (board2[i][j] == 1)
				allb2++;
			if (board2[i][j] == 2)
				allw2++;
		}
	}
	black2 = LabelTTF::create("", "Arial", 24);
	black2->setAnchorPoint(Vec2(0, 1));
	black2->setPosition(Vec2(214, 458));
	this->addChild(black2);
	white2 = LabelTTF::create("", "Arial", 24);
	white2->setAnchorPoint(Vec2(0, 1));
	white2->setPosition(Vec2(214, 255));
	this->addChild(white2);
	schedule(schedule_selector(game::update));
	return true;
}
void game::menuCloseCallback(Ref* pSender)
{
	//Close the cocos2d-x game scene and quit the application
	Director::getInstance()->end();
	/*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

	//EventCustom customEndEvent("game_scene_close_event");
	//_eventDispatcher->dispatchEvent(&customEndEvent);
}
int game::judge(int a, int b) {
	int count = 0;
	for (int k = 0;k < 8;k++) {
		int tx = a + dx2[k];
		int ty = b + dy2[k];
		int num = 0;
		while (tx >= 0 && ty >= 0 && tx < 8 && ty < 8 && board2[tx][ty] == 2) {
			tx += dx2[k];
			ty += dy2[k];
			num++;
		}
		if (num >= 1 && board2[tx][ty] == 0 && tx >= 0 && ty >= 0 && tx < 8 && ty < 8) {
			board2[tx][ty] = 3;
			auto space3 = Sprite::create("ChessmanFocus3.png");
			space3->setPosition(Vec2((ty + 1) * 50 + 275, (8 - tx) * 50 + 174));
			// add the sprite as a child to this layer
			this->addChild(space3, 0, tx * 8 + ty);
			count++;
		}
	}
	if (count > 0)
		return 1;
	else
		return 0;
}
int game::judge2(int a, int b) {
	int count = 0;
	for (int k = 0;k < 8;k++) {
		int tx = a + dx2[k];
		int ty = b + dy2[k];
		int num = 0;
		while (tx >= 0 && ty >= 0 && tx < 8 && ty < 8 && board2[tx][ty] == 1) {
			tx += dx2[k];
			ty += dy2[k];
			num++;
		}
		if (num >= 1 && board2[tx][ty] == 0 && tx >= 0 && ty >= 0 && tx < 8 && ty < 8) {
			board2[tx][ty] = 3;
			auto space4 = Sprite::create("ChessmanFocus3.png");
			space4->setPosition(Vec2((ty + 1) * 50 + 275, (8 - tx) * 50 + 174));
			// add the sprite as a child to this layer
			this->addChild(space4, 0, tx * 8 + ty);
			count++;
		}
	}
	if (count > 0)
		return 1;
	else
		return 0;
}
int game::search() {
	int coc = 0;
	for (int i = 0;i < 8;i++) {
		for (int j = 0;j < 8;j++) {
			if (board2[i][j] == 1 && judge(i, j)) {
				judge(i, j);
				coc++;
			}
		}
	}
	if (coc > 0)
		return 1;
	else
		return 0;
}
int game::search2() {
	int coc = 0;
	for (int i = 0;i < 8;i++) {
		for (int j = 0;j < 8;j++) {
			if (board2[i][j] == 2 && judge2(i, j)) {
				judge2(i, j);
				coc++;
			}
		}
	}
	if (coc > 0)
		return 1;
	else
		return 0;
}
void game::change(int a, int b) {
	for (int k = 0;k < 8;k++) {
		int x = a + dx2[k];
		int y = b + dy2[k];
		int numb = 0;
		while (x < 8 && y < 8 && x >= 0 && y >= 0 && board2[x][y] == 2) {
			x += dx2[k];
			y += dy2[k];
			numb++;
			if (x < 8 && y < 8 && x >= 0 && y >= 0 && board2[x][y] == 1 && numb >= 1) {
				int mx = x - dx2[k];
				int my = y - dy2[k];
				record2[a][b].changenum += numb;
				log("%d", record2[a][b].changenum);
				for (;numb >= 1;numb--) {
					auto temp = getChildByTag(mx * 8 + my);
					cord2.push_back(Value(mx * 8 + my));
					CCAnimation *animation = CCAnimation::create();
					animation->addSpriteFrameWithFileName("wb2.png");
					animation->addSpriteFrameWithFileName("wb3.png");
					animation->addSpriteFrameWithFileName("wb4.png");
					animation->addSpriteFrameWithFileName("wb5.png");
					animation->setDelayPerUnit(0.1f);//设置动画的间隔时间  
					animation->setRestoreOriginalFrame(false);//是否返回第一帧  
					temp->runAction(CCAnimate::create(animation));
					board2[mx][my] = 1;
					mx -= dx2[k];
					my -= dy2[k];
				}
			}
		}
	}
}
void game::change2(int a, int b) {
	for (int k = 0;k < 8;k++) {
		int x = a + dx2[k];
		int y = b + dy2[k];
		int numb = 0;
		while (x < 8 && y < 8 && x >= 0 && y >= 0 && board2[x][y] == 1) {
			x += dx2[k];
			y += dy2[k];
			numb++;
			if (x < 8 && y < 8 && x >= 0 && y >= 0 && board2[x][y] == 2 && numb >= 1) {
				int mx = x - dx2[k];
				int my = y - dy2[k];
				record2[a][b].changenum += numb;
				log("%d", record2[a][b].changenum);
				for (;numb >= 1;numb--) {
					auto temp = getChildByTag(mx * 8 + my);
					CCAnimation *animation = CCAnimation::create();
					cord2.push_back(Value(mx * 8 + my));
					animation->addSpriteFrameWithFileName("wb4.png");
					animation->addSpriteFrameWithFileName("wb3.png");
					animation->addSpriteFrameWithFileName("wb2.png");
					animation->addSpriteFrameWithFileName("wb1.png");
					animation->setDelayPerUnit(0.1f);//设置动画的间隔时间  
					animation->setRestoreOriginalFrame(false);//是否返回第一帧  
					temp->runAction(CCAnimate::create(animation));
					board2[mx][my] = 2;
					mx -= dx2[k];
					my -= dy2[k];
				}
			}
		}
	}
}
void game::clear() {
	for (int i = 0;i < 8;i++) {
		for (int j = 0;j < 8;j++) {
			if (board2[i][j] == 3) {
				removeChildByTag(i * 8 + j);
			}
		}
	}
}
int game::score() {
	int score[72] = { 0 };
	int maxscore = 0;
	int getx = 0;
	int gety = 0;
	for (int i = 0;i < 8;i++) {
		for (int j = 0;j < 8;j++) {
			if (board2[i][j] == 3) {
				int allscore = 0;
				if (j == 0||j==7) {
					allscore = allscore + 5;
				}
				if (i == 0 || i == 7)
					allscore = allscore + 5;
				for (int k = 0;k < 8;k++) {
					int tx = i + dx2[k];
					int ty = j + dy2[k];
					int num2 = 0;
					int divscore = 0;
					while (board2[tx][ty] == 1 && tx >= 0 && tx < 8 && ty >= 0 && ty < 8) {
						tx += dx2[k];
						ty += dy2[k];
						num2++;
						if (board2[tx][ty] == 2)
						{
							if (num2 == 1)
								divscore = 5;
							if (num2 == 2)
								divscore = 10;
							if (num2 == 3)
								divscore = 15;
							if (num2 == 4)
								divscore = 20;
							if (num2 == 5)
								divscore = 25;
							if (num2 == 6)
								divscore = 30;
							allscore += divscore;
						}
					}	
				}
				score[i * 8 + j] = allscore;
				if (i == 0) {
					if (j == 0)
						score[i * 8 + j] = 250;
					if (j == 7)
						score[i * 8 + j] = 250;
					if (j == 1)
						score[i * 8 + j] = 2;
					if (j == 6)
						score[i * 8 + j] = 2;
				}
				if (i == 7) {
					if (j == 0)
						score[i * 8 + j] = 250;
					if (j == 7)
						score[i * 8 + j] = 250;
					if (j == 1)
						score[i * 8 + j] = 2;
					if (j == 6)
						score[i * 8 + j] = 2;
				}
				if (i == 1) {
					if (j == 0)
						score[i * 8 + j] = 2;
					if (j == 1)
						score[i * 8 + j] = 2;
					if (j == 6)
						score[i * 8 + j] = 2;
					if (j == 7)
						score[i * 8 + j] = 2;
				}
				if (i == 6) {
					if (j == 0)
						score[i * 8 + j] = 2;
					if (j == 1)
						score[i * 8 + j] = 2;
					if (j == 6)
						score[i * 8 + j] = 2;
					if (j == 7)
						score[i * 8 + j] = 2;
				}
			}
		}
	}
	for (int x = 0;x < 8;x++) {
		for (int y = 0;y < 8;y++) {
			if (board2[x][y]==3&&score[x * 8 + y]>maxscore) {
				getx = x;
				gety = y;
				maxscore = score[x * 8 + y];
			}
		}
	}
	return getx * 8 + gety;
}
void game::turnback(int a, int b) {
	while (record2[a][b].changenum>0) {
		int test = cord2.back().asInt();
		cord2.pop_back();
		auto temp = getChildByTag((test / 8) * 8 + (test % 8));
		CCAnimation *animation = CCAnimation::create();
		animation->addSpriteFrameWithFileName("wb4.png");
		animation->addSpriteFrameWithFileName("wb3.png");
		animation->addSpriteFrameWithFileName("wb2.png");
		animation->addSpriteFrameWithFileName("wb1.png");
		animation->setDelayPerUnit(0.1f);//设置动画的间隔时间  
		animation->setRestoreOriginalFrame(false);//是否返回第一帧  
		temp->runAction(CCAnimate::create(animation));
		board2[test / 8][test % 8] = 2;
		record2[a][b].changenum--;
	}
}
void game::turnback2(int a, int b) {
	while (record2[a][b].changenum>0) {
		int test = cord2.back().asInt();
		cord2.pop_back();
		auto temp = getChildByTag((test / 8) * 8 + (test % 8));
		CCAnimation *animation = CCAnimation::create();
		animation->addSpriteFrameWithFileName("wb2.png");
		animation->addSpriteFrameWithFileName("wb3.png");
		animation->addSpriteFrameWithFileName("wb4.png");
		animation->addSpriteFrameWithFileName("wb5.png");
		animation->setDelayPerUnit(0.1f);//设置动画的间隔时间  
		animation->setRestoreOriginalFrame(false);//是否返回第一帧  
		temp->runAction(CCAnimate::create(animation));
		board2[test / 8][test % 8] = 1;
		record2[a][b].changenum--;
	}
}
void game::update(float dt) {
	black2->setString(String::createWithFormat("%d", allb2)->getCString());
	white2->setString(String::createWithFormat("%d", allw2)->getCString());
}
