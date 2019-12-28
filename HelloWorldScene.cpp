/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "gamescene.h"
#include "ui/CocosGUI.h"
#include "menu.h"
USING_NS_CC;
struct CHESS
{
	int changenum=0;
};
struct CHESS record[8][8];
char gamestate ='b';
int flag = 0;
int board[8][8] = {0};
int dx[] = {-1,-1,-1,1,1,1,0,0};
int dy[] = {-1,0,1,-1,0,1,1,-1};
int bn = 0;
int wn = 0;
ValueVector val;
ValueVector cord;
int allb = 0;
int allw = 0;
Scene* HelloWorld::createScene()
{
	auto scene = Scene::create();
	auto layer = HelloWorld::create();
	scene->addChild(layer);
	return scene;
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
	
    if ( !Layer::init() )
    {
        return false;
    }
	for (int i = 0;i < 8;i++) {
		for (int j = 0;j < 8;j++) {
			if (board[i][j] != 0) {
				board[i][j] = 0;
			}
			log("%d", board[i][j]);
		}
	}
	if (gamestate != 'b') {
		gamestate = 'b';
	}
	if (allb != 0) {
		allb = 0;
	}
	if (allw != 0) {
		allw = 0;
	}
	for (int i = 0;i < 8;i++) {
		for (int j = 0;j < 8;j++) {
			if (record[i][j].changenum != 0) {
				record[i][j].changenum = 0;
			}
		}
	}
	auto listener1 = EventListenerTouchOneByOne::create();
	listener1->onTouchBegan=[this](Touch* touch, Event* event)
	{
		int x = touch->getLocation().x;
		int y = touch->getLocation().y;	
		log("%d %d", x, y);
		int bnum = 0;
		int wnum = 0;
		allb = 0;
		allw = 0;
		switch (gamestate)
		{
		case 'b':
			for (int i = 1;i <= 8;i++) {
				for (int j = 1;j <= 8;j++) {
					if (abs(x - (i * 50 + 275)) < 24 && abs(y - (j * 50 + 174)) < 24) {
						int getx = i * 50 + 275;
						int gety = j * 50 + 174;
						if (board[8 - j][i - 1] != 3) {
							gamestate = 'b';
						}
						if (board[8 - j][i - 1] == 3) {
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
								val.push_back(Value((8 - j) * 8 + (i - 1)));
							}
							board[8 - j][i - 1] = 1;
							change(8 - j, i - 1);
							for (int i = 0;i < 8;i++) {
								for (int j = 0;j < 8;j++) {
									if (board[i][j] == 3) {
										board[i][j] = 0;
									}
								}
							}
							wn = search2();
							gamestate = 'w';
							if (wn == 0)
							{
								gamestate = 'b';
								bn=search();
								if (bn == 0 && wn == 0) {
									for (int i = 0;i < 8;i++) {
										for (int j = 0;j < 8;j++) {
											if (board[i][j] == 1)
												bnum++;
											if (board[i][j] == 2)
												wnum++;
										}
									}
									if (bnum > wnum) {
										auto blackwin = Sprite::create("ResultBlack.png");
										blackwin->setPosition(Vec2(100, 100));
										// add the sprite as a child to this layer
										this->addChild(blackwin, 0);
									}
									if (bnum < wnum) {
										auto whitewin = Sprite::create("ResultWhite.png");
										whitewin->setPosition(Vec2(100, 100));
										// add the sprite as a child to this layer
										this->addChild(whitewin, 0);	
									}
								}
								log("pass");
							}
						}		
					}
				}
			}break;
		case 'w':
			for (int i = 1;i <= 8;i++) {
				for (int j = 1;j <= 8;j++) {
					if (abs(x - (i * 50 + 275)) < 24 && abs(y - (j * 50 + 174)) < 24) {
						int getwx = i * 50 + 275;
						int getwy = j * 50 + 174;
						if (board[8 - j][i] != 3) {
							gamestate = 'w';
						}
						if (board[8 - j][i - 1] == 3) {
							clear();
							auto sprite2 = Sprite::create("wb1.png");
							if (sprite2 == nullptr)
							{
								problemLoading("'HelloWorld.png'");
							}
							else
							{
								// position the sprite on the center of the screen
								sprite2->setPosition(Vec2(getwx, getwy));

								// add the sprite as a child to this layer
								this->addChild(sprite2, 0, (8 - j) * 8 + (i - 1));
								val.push_back(Value((8 - j) * 8 + (i - 1)));
							}
							board[8 - j][i - 1] = 2;
							change2(8 - j, i - 1);
							for (int i = 0;i < 8;i++) {
								for (int j = 0;j < 8;j++) {
									if (board[i][j] == 3) {
										board[i][j] = 0;
									}
								}
							}
							bn = search();
							gamestate = 'b';
							if (bn == 0)
							{
				             	gamestate = 'w';
								wn=search2();
								if (bn == 0 && wn == 0) {
									for (int i = 0;i < 8;i++) {
										for (int j = 0;j < 8;j++) {
											if (board[i][j] == 1)
												bnum++;
											if (board[i][j] == 2)
												wnum++;
										}
									}
									if (bnum > wnum) {
										auto blackwin = Sprite::create("ResultBlack.png");
										blackwin->setPosition(Vec2(100, 100));
										// add the sprite as a child to this layer
										this->addChild(blackwin, 0);
									}
									if (bnum < wnum) {
										auto whitewin = Sprite::create("ResultWhite.png");
										whitewin->setPosition(Vec2(100, 100));
										// add the sprite as a child to this layer
										this->addChild(whitewin, 0);
									}
								}
								log("pass");
							}
						}				
					}
				}
			}break;
		}
		for (int i = 0;i < 8;i++) {
			for (int j = 0;j < 8;j++) {
				if (board[i][j] == 1)
					allb++;
				if (board[i][j] == 2)
					allw++;
			}
		}
		log("%d %d", allb, allw);
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
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
        float y = origin.y + closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x,y));
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
        label->setPosition(Vec2(origin.x + visibleSize.width/2,
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
        sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

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
		initblack->setPosition(Vec2(5*50+275,5*50+174));
		// add the sprite as a child to this layer
		this->addChild(initblack, 1,3*8+4);
		board[3][4] = 1;
	}
	auto initwhite = Sprite::create("ChessmanWhite1.png");
	if (sprite == nullptr)
	{
		problemLoading("'HelloWorld.png'");
	}
	else
	{
		// position the sprite on the center of the screen
		initwhite->setPosition(Vec2(4*50+275,5*50+174));
		// run it 
		// add the sprite as a child to this layer
		this->addChild(initwhite,1,3*8+3);
		board[3][3] = 2;
		
	}
	auto initblack2 = Sprite::create("ChessmanBlack1.png");
	if (initblack == nullptr)
	{
		problemLoading("'HelloWorld.png'");
	}
	else
	{
		// position the sprite on the center of the screen
		initblack2->setPosition(Vec2(4*50+275,4*50+174));
		// add the sprite as a child to this layer
		this->addChild(initblack2, 1,4*8+3);
		board[4][3] = 1;
		
	}
	auto initwhite2 = Sprite::create("ChessmanWhite1.png");
	if (sprite == nullptr)
	{
		problemLoading("'HelloWorld.png'");
	}
	else
	{
		// position the sprite on the center of the screen
		
		initwhite2->setPosition(Vec2(5*50+275,4*50+174));
		// add the sprite as a child to this layer
		this->addChild(initwhite2,1,4*8+4);
		board[4][4] = 2;
	}
	search();
	auto button = Button::create("huiqi.png");
	button->setPosition(Vec2(415, 159));
	button->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
		
		switch (type)
		{
		case ui::Widget::TouchEventType::BEGAN:
			break;
		case ui::Widget::TouchEventType::ENDED:
			int a1 = val.back().asInt();
			val.pop_back();
			if (board[a1 / 8][a1 % 8] == 1) {
				turnback((a1 / 8), (a1 % 8));
				clear();
				for (int i = 0;i < 8;i++) {
					for (int j = 0;j < 8;j++) {
						if (board[i][j] == 3) {
							board[i][j] = 0;
						}
					}
				}
				board[a1 / 8][a1 % 8] = 0;
				search();
				removeChildByTag(a1);
				gamestate = 'b';
			}
			if (board[a1 / 8][a1 % 8] == 2) {
				turnback2((a1 / 8), (a1 % 8));
				clear();
				for (int i = 0;i < 8;i++) {
					for (int j = 0;j < 8;j++) {
						if (board[i][j] == 3) {
							board[i][j] = 0;
						}
					}
				}
				board[a1 / 8][a1 % 8] = 0;
				search2();
				removeChildByTag(a1);
				gamestate = 'w';
			}
			allb = 0;
			allw = 0;
			for (int i = 0;i < 8;i++) {
				for (int j = 0;j < 8;j++) {
					if (board[i][j] == 1)
						allb++;
					if (board[i][j] == 2)
						allw++;
				}
			}
			break;
		}
	});
	this->addChild(button);
	auto button2 = Button::create("lose.png");
	button2->setPosition(Vec2(495, 159));
	button2->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
		switch (type)
		{
		case ui::Widget::TouchEventType::BEGAN:
			break;
		case ui::Widget::TouchEventType::ENDED:
			gamestate = 'over';
			int white = 0;
			int black = 0;
			for (int i = 0;i < 8;i++) {
				for (int j = 0;j < 8;j++) {
					if (board[i][j] == 1)
						black++;
					if (board[i][j] == 2)
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
			if (board[i][j] == 1)
				allb++;
			if (board[i][j] == 2)
				allw++;
		}
	}
	black = LabelTTF::create("","Arial",24);
	black->setAnchorPoint(Vec2(0,1));
	black->setPosition(Vec2(214, 458));
	this->addChild(black);
	white=LabelTTF::create("", "Arial", 24);
	white->setAnchorPoint(Vec2(0, 1));
	white->setPosition(Vec2(214, 255));
	this->addChild(white);
	schedule(schedule_selector(HelloWorld::update));

 return true;
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);
}
int HelloWorld::judge(int a,int b) {
	int count = 0;
	for (int k = 0;k < 8;k++) {
		int tx = a + dx[k];
		int ty = b + dy[k];
		int num = 0;
		while (tx >= 0 && ty >= 0 && tx < 8 && ty < 8 && board[tx][ty]== 2) {
			tx += dx[k];
			ty += dy[k];		
			num++;
		}
		if (num >= 1 && board[tx][ty] == 0&& tx >= 0 && ty >= 0 && tx < 8 && ty < 8) {
			board[tx][ty] = 3;
			auto space3 = Sprite::create("ChessmanFocus3.png");
			space3->setPosition(Vec2((ty+1) * 50 + 275, (8-tx) * 50 + 174));
			// add the sprite as a child to this layer
			this->addChild(space3,0,tx*8+ty);
			count++;
		}
	}
	
	if (count > 0)
		return 1;
	else
		return 0;
}
int HelloWorld::judge2(int a, int b) {
	int count = 0;
	for (int k = 0;k < 8;k++) {
		int tx = a + dx[k];
		int ty = b + dy[k];	
		int num = 0;
		while (tx >= 0 && ty >= 0 && tx < 8 && ty < 8 && board[tx][ty] == 1) {
			tx += dx[k];
			ty += dy[k];		
			num++;			
		}
		if (num >= 1 && board[tx][ty] == 0 && tx >= 0 && ty >= 0 && tx < 8 && ty < 8) {
			board[tx][ty] = 3;
			auto space4 = Sprite::create("ChessmanFocus3.png");
			space4->setPosition(Vec2((ty+1) * 50 + 275, (8 - tx) * 50 + 174));
			// add the sprite as a child to this layer
			this->addChild(space4, 0,tx*8+ty);
			count++;
		}
	}	
	if (count > 0)
		return 1;
	else
		return 0;
}
int HelloWorld::search() {
	int coc=0;
	for (int i = 0;i < 8;i++) {
		for (int j = 0;j < 8;j++) {
			if (board[i][j]==1&&judge(i,j)) {
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
int HelloWorld::search2() {
	int coc = 0;
	for (int i = 0;i < 8;i++) {
		for (int j = 0;j < 8;j++) {
			if (board[i][j] == 2&&judge2(i,j)) {
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
void HelloWorld::change(int a,int b) {	
	for (int k = 0;k < 8;k++) {
		int x = a + dx[k];
		int y = b + dy[k];		
		int numb = 0;
        while (x < 8 && y < 8 && x >= 0 && y >= 0 && board[x][y] == 2) {			
			x += dx[k];
			y += dy[k];	
			numb++;
			if (x < 8 && y < 8 && x >= 0 && y >= 0 && board[x][y] == 1&&numb>=1) {
				int mx = x - dx[k];
				int my = y - dy[k];
				record[a][b].changenum += numb;
				log("%d", record[a][b].changenum);
				for (;numb>=1;numb--) {
					auto temp = getChildByTag(mx * 8 + my);
					cord.push_back(Value(mx * 8 + my));
					CCAnimation *animation = CCAnimation::create();
					animation->addSpriteFrameWithFileName("wb2.png");
					animation->addSpriteFrameWithFileName("wb3.png");
					animation->addSpriteFrameWithFileName("wb4.png");
					animation->addSpriteFrameWithFileName("wb5.png");
					animation->setDelayPerUnit(0.1f);//设置动画的间隔时间  
					animation->setRestoreOriginalFrame(false);//是否返回第一帧  
					temp->runAction(CCAnimate::create(animation));
					board[mx][my] = 1;
					mx -= dx[k];
					my -= dy[k];
				}
			}		
		}
	}
}
void HelloWorld::change2(int a, int b) {
	for (int k = 0;k < 8;k++) {
		int x = a + dx[k];
		int y = b + dy[k];
		int numb = 0;
		while (x < 8 && y < 8 && x >= 0 && y >= 0 && board[x][y] == 1) {
			x += dx[k];
			y += dy[k];
			numb++;
			if (x < 8 && y < 8 && x >= 0 && y >= 0 && board[x][y] == 2 && numb >= 1) {
				int mx = x - dx[k];
				int my = y - dy[k];
				record[a][b].changenum += numb;
				log("%d", record[a][b].changenum);
				for (;numb >= 1;numb--) {
					auto temp = getChildByTag(mx * 8 + my);
					cord.push_back(Value(mx * 8 + my));
					CCAnimation *animation = CCAnimation::create();
					animation->addSpriteFrameWithFileName("wb4.png");
					animation->addSpriteFrameWithFileName("wb3.png");
					animation->addSpriteFrameWithFileName("wb2.png");
					animation->addSpriteFrameWithFileName("wb1.png");
					animation->setDelayPerUnit(0.1f);//设置动画的间隔时间  
					animation->setRestoreOriginalFrame(false);//是否返回第一帧  
					temp->runAction(CCAnimate::create(animation));
					board[mx][my] = 2;
					mx -= dx[k];
					my -= dy[k];
				}
			}
		}
	}
}

void HelloWorld::clear() {
	for (int i=0;i < 8;i++) {
		for (int j=0;j < 8;j++) {
			if (board[i][j] == 3) {
				removeChildByTag(i * 8 + j);			
			}
		}
	}
}
void HelloWorld::turnback(int a, int b) {
	while (record[a][b].changenum>0) {
		int test = cord.back().asInt();
		cord.pop_back();
		auto temp = getChildByTag((test/8) * 8 + (test%8));
		CCAnimation *animation = CCAnimation::create();
		animation->addSpriteFrameWithFileName("wb4.png");
		animation->addSpriteFrameWithFileName("wb3.png");
		animation->addSpriteFrameWithFileName("wb2.png");
		animation->addSpriteFrameWithFileName("wb1.png");
		animation->setDelayPerUnit(0.1f);//设置动画的间隔时间  
		animation->setRestoreOriginalFrame(false);//是否返回第一帧  
		temp->runAction(CCAnimate::create(animation));
		board[test / 8][test % 8] = 2;
		record[a][b].changenum--;
	}
}
void HelloWorld::turnback2(int a, int b) {
	while (record[a][b].changenum>0) {
		int test = cord.back().asInt();
		cord.pop_back();
		auto temp = getChildByTag((test / 8) * 8 + (test % 8));
		CCAnimation *animation = CCAnimation::create();
		animation->addSpriteFrameWithFileName("wb2.png");
		animation->addSpriteFrameWithFileName("wb3.png");
		animation->addSpriteFrameWithFileName("wb4.png");
		animation->addSpriteFrameWithFileName("wb5.png");
		animation->setDelayPerUnit(0.1f);//设置动画的间隔时间  
		animation->setRestoreOriginalFrame(false);//是否返回第一帧  
		temp->runAction(CCAnimate::create(animation));
		board[test / 8][test % 8] = 1;
		record[a][b].changenum--;
	}
}
void HelloWorld::update(float dt) {
	
	black->setString(String::createWithFormat("%d", allb)->getCString());
	white->setString(String::createWithFormat("%d", allw)->getCString());
}