//
//  Model.cpp
//  HexagonSnake
//
//  Created by SanHeo on 2014. 7. 27..
//
//

#include "UnitBehaviorMonsterFlashman.h"

USING_NS_CC;

UnitBehaviorMonsterFlashman* UnitBehaviorMonsterFlashman::create()
{
    UnitBehaviorMonsterFlashman *pRet = new (std::nothrow) UnitBehaviorMonsterFlashman();
    if ( pRet && pRet->init() )
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        return nullptr;
    }
}

UnitBehaviorMonsterFlashman::UnitBehaviorMonsterFlashman() :
_bInitUI(false),

//
_eState(E_STATE::IDLE),
_eType(E_FLASHMAN_TYPE::None),
_bBoss(false),

_uiAnimationMonster(nullptr),
_uiAnimateIdle(nullptr),

_uiHPBG(nullptr)
{
    
}

UnitBehaviorMonsterFlashman::~UnitBehaviorMonsterFlashman()
{
    if ( _uiAnimateIdle != nullptr )
    {
        _uiAnimateIdle->release();
        _uiAnimateIdle = nullptr;
    }
}

bool UnitBehaviorMonsterFlashman::init()
{
    if ( !Layer::init())
    {
        return false;
    }
    
    //
    setIgnoreAnchorPointForPosition(false);
    setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    
    //
    _uiAnimationMonster = Sprite::create();
    _uiAnimationMonster->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _uiAnimationMonster->setPosition(Vec2::ZERO);
    addChild(_uiAnimationMonster);
    
    _uiHPBG = ui::ImageView::create("Assets/ui/flashman/bsbs_mon_life_01.png");
    _uiHPBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _uiHPBG->setPositionType(ui::CText::PositionType::PERCENT);
    _uiHPBG->setPositionPercent(Vec2::ANCHOR_MIDDLE_TOP);
    _uiHPBG->setVisible(false);
    _uiAnimationMonster->addChild(_uiHPBG);
    {
        auto uiHP = ui::CText::create("0", GAME_FONT, 20);
        uiHP->setPositionType(ui::CText::PositionType::PERCENT);
        uiHP->setPositionPercent(Vec2::ANCHOR_MIDDLE);
        uiHP->setName("UI_TEXT");
        uiHP->enableOutline(Color4B::BLACK, 2);
        _uiHPBG->addChild(uiHP);
    }
    
    return true;
}

void UnitBehaviorMonsterFlashman::onEnter()
{
    Layer::onEnter();
    
    if ( _bInitUI == false )
    {
        //
        onIdle();
        
        _bInitUI = true;
    }
    
    //
    drawHP();
}

void UnitBehaviorMonsterFlashman::drawHP()
{
    if ( _bBoss == false )
    {
        _uiHPBG->setVisible(false);
        return;
    }
    
    _uiHPBG->setVisible(true);
    
    auto uiHP = _uiHPBG->getChildByName<ui::CText*>("UI_TEXT");
    uiHP->setString(std::to_string(_hp));
}
  
#pragma mark -
UnitBehaviorMonsterFlashman::E_STATE UnitBehaviorMonsterFlashman::getState()
{
    return _eState;
}

E_FLASHMAN_TYPE UnitBehaviorMonsterFlashman::getType()
{
    return _eType;
}

void UnitBehaviorMonsterFlashman::setMotion(E_FLASHMAN_TYPE eType, bool bBoss)
{
    _eType = eType;
    _bBoss = bBoss;

    //s
    std::string pathNormal = "";
    std::string pathBoss = "";
    switch (eType) {
        case E_FLASHMAN_TYPE::Red:
            pathNormal = "Assets/ui/flashman/bsbs_mon_red_1_%d.png";
            pathBoss = "Assets/ui/flashman/bsbs_mon_red_2_%d.png";
            break;
        case E_FLASHMAN_TYPE::Green:
            pathNormal = "Assets/ui/flashman/bsbs_mon_green_1_%d.png";
            pathBoss = "Assets/ui/flashman/bsbs_mon_green_2_%d.png";
            break;
        case E_FLASHMAN_TYPE::Blue:
            pathNormal = "Assets/ui/flashman/bsbs_mon_blue_1_%d.png";
            pathBoss = "Assets/ui/flashman/bsbs_mon_blue_2_%d.png";
            break;
        case E_FLASHMAN_TYPE::Bonus:
            pathNormal = "Assets/ui/flashman/bsbs_mon_time_1_%d.png";
            pathBoss = "Assets/ui/flashman/bsbs_mon_time_1_%d.png";
            break;
            
        default:
            break;
    }
    
    std::string path = bBoss == false ? pathNormal : pathBoss;
    std::string original = MafUtils::format(path.c_str(), 1);

    _uiAnimationMonster->setTexture(original);
    
    auto size = _uiAnimationMonster->getContentSize();
    setContentSize(size);

    //
    Vector<cocos2d::SpriteFrame*> list;
    for ( int i = 1; i <= 2; i++ )
    {
        std::string strTempPath = MafUtils::format(path.c_str(), i);
        std::string fullpath = FileUtils::getInstance()->fullPathForFilename(strTempPath);
        
        if(fullpath.size() == 0)
            break;
        
        auto spriteAni = Sprite::create(strTempPath);
        if ( spriteAni == nullptr )
        {
            break;
        }
        
        spriteAni->getTexture()->setTexParameters(texParams);
        list.pushBack(spriteAni->getSpriteFrame());
    }
    _uiAnimateIdle = Animate::create(Animation::createWithSpriteFrames(list, 0.2));
    _uiAnimateIdle->retain();
}

int UnitBehaviorMonsterFlashman::getHP()
{
    return _hp;
}

void UnitBehaviorMonsterFlashman::setHP(int hp)
{
    _hp = hp;
    
    if ( _bInitUI == true )
    {
        drawHP();
    }
}

#pragma mark - event
void UnitBehaviorMonsterFlashman::onIdle()
{
    _eState = E_STATE::IDLE;
    
    _uiAnimationMonster->stopAllActions();
    _uiAnimationMonster->runAction(RepeatForever::create(_uiAnimateIdle));
}

