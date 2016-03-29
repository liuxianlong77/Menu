#include "MenuSelect.h"
#include "DBMenu.h"
namespace Src{
    using namespace Help;
    SpriteInfo bg[5] = {
        { "bg0.png", cocos2d::Rect(0,0,-1.f,-1.f), Anchor::CE, CC::Vec2::ZERO},
        { "bg1.png", cocos2d::Rect(0,0,-1.f,-1.f), Anchor::CE, CC::Vec2::ZERO},
        { "bg2.png", cocos2d::Rect(0,0,-1.f,-1.f), Anchor::CE, CC::Vec2::ZERO},
        { "bg3.png", cocos2d::Rect(0,0,-1.f,-1.f), Anchor::CE, CC::Vec2::ZERO},
        { "bg4.png", cocos2d::Rect(0,0,-1.f,-1.f), Anchor::CE, CC::Vec2::ZERO},
    };
    
    AnimationInfo animation[5] = {
         { "fish_19.png", 9, 1, 9, true, 0.5/9.f, true },
         { "fish_20.png", 6, 1, 6, true, 0.5/6.f, true },
         { "fish_21.png", 11, 1, 11, true, 0.5/11.f, true },
         { "fish_22.png", 3, 1, 3, true, 0.5/3.f, true },
         { "fish_23.png", 12, 1, 12, true, 0.5/12.f, true },
    };
    
    ButtonInfo3 butInfo = {
        "prop_bg2.png", 1, true, defaultRect, Anchor::CE, CC::Vec2::ZERO
    };
 }
static float diff = 230.f;
static float startPos = 0;
static float maxScale = 2.5;
static bool  isMove ;
static bool  canMove = false;
enum Z_Order
{
    Z_menu, Z_bg, Z_animal
};

SelectItem::SelectItem(int index, std::function<void(Ref*)> selectCall)
{
    mIndex = index;
    auto bg = Help::LoadSprite(Src::bg[index]);
    bg->setScale(0.1);
    this->addChild(bg,Z_bg);
    
    auto animal = Help::LoadSprite(Src::animation[index]);
    animal->setScale(0.4);
    auto animate = CC::Animate::create(Help::LoadAnimation(Src::animation[index]));
    animal->runAction(CC::RepeatForever::create(animate));
    this->addChild(animal,Z_animal);
    
    auto but = Help::LoadButton(Src::butInfo);
    but->setCallback(selectCall);
    auto menu = DBMenu::create();
    menu->addChild(but);
    this->addChild(menu,Z_menu);
    this->setContentSize(but->getContentSize());
}

MenuSelect::MenuSelect()
{
    mCurrentItem = 0;
    for (int i = 0; i < 5; i++) {
        auto item =new SelectItem(i,[this,i](Ref*){
            selectLevel(i);
        });
        mItems.push_back(item);
        item->setPosition(CC::Vec2(startPos + diff*i, 0));
        this->addChild(item);
    }
    mItems[mCurrentItem]->setScale(maxScale);
    
    Help::AddOneByOneTouch(this->getEventDispatcher(), this, true, [this](CC::Touch* touch, CC::Event* event){
        for (auto& item : mItems) {
            float positionx = this->getPositionX() - item->getContentSize().width/2 + item->getPositionX();
            float positiony = this->getPositionY() - item->getContentSize().height/2 + item->getPositionY();
            CC::Rect itemBox = CC::Rect(positionx , positiony,item->getContentSize().width,item->getContentSize().height);
            canMove = itemBox.containsPoint(touch->getLocation());
            if (canMove) {
                break;
            }
        }
        if (!canMove) {
            return  false;
        }
        isMove = false;
        originalPosition = touch->getLocation();
        return true;
        
    }, [this](CC::Touch* touch, CC::Event* event){
        CC::Vec2 touchPosition = touch->getLocation();
        CC::Vec2 distance = touchPosition - originalPosition;
        this->setPosition(CC::Vec2(this->getPositionX()+distance.x, this->getPositionY()));
        originalPosition = touchPosition;
        updateScale();
        isMove = true;
        
    }, [this](CC::Touch* touch, CC::Event* event){
        updatePosition();
        updateScale();
        
    });
}

void MenuSelect::selectLevel(int index)
{
    if (isMove) return;
    if (mCurrentItem != index){
        this->setPositionX(this->getPositionX() - (this->getPositionX()+mItems[index]->getPositionX() - WIDTH/2));
        updateScale();
    }
    else{
        /*selectLevel*/
        CCLOG("111111111");
    }
    
}

void MenuSelect::updateScale()
{
    for (auto& item : mItems) {
        float distance = std::abs(this->getPositionX()+item->getPositionX() - WIDTH/2);
        float scaleValue = 1 + (52 / distance);
        if(scaleValue > maxScale)
        {
            scaleValue = maxScale;
        }
        item->setScale(scaleValue);
    }
}

void MenuSelect::updatePosition()
{
    int index = 0;
    float minDistance = std::abs(this->getPositionX()+mItems[0]->getPositionX() - WIDTH/2);
    for (auto & item : mItems) {
        if (minDistance > std::abs(this->getPositionX()+item->getPositionX() - WIDTH/2)) {
            minDistance = std::abs(this->getPositionX()+item->getPositionX() - WIDTH/2);
            index++;
        }
    }
    this->setPositionX(this->getPositionX() - (this->getPositionX()+mItems[index]->getPositionX() - WIDTH/2));
    mCurrentItem = index;
}
