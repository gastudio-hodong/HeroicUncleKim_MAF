//
//  LayerAdventureBossBattle.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 09/01/2020.
//

#include "LayerAdventureBossBattle.h"

#include "Common/ConfigGameNormal.h"

#include "GameUnit/UnitBehaviorHeroFrost.h"
#include "GameUnit/UnitBehaviorMonsterFrost.h"

#include "ManagerGame/AdventureManager.h"

USING_NS_CC;

LayerAdventureBossBattle* LayerAdventureBossBattle::create(cocos2d::Size size)
{
    LayerAdventureBossBattle* pRet = new(std::nothrow) LayerAdventureBossBattle();
    if ( pRet && pRet->init(size) )
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

LayerAdventureBossBattle::LayerAdventureBossBattle(void) :
_bInitEnter(false),

_objInfoBoss(nullptr),
_bulletItemIdx(0),
_bulletItemCount(0),

_callbackAttackFinish(nullptr),

//
_uiContentsLobby(nullptr)

{
    
}

LayerAdventureBossBattle::~LayerAdventureBossBattle(void)
{
    CC_SAFE_RELEASE_NULL(_objInfoBoss);
}

bool LayerAdventureBossBattle::init(cocos2d::Size size)
{
    if ( !ui::Layout::init() )
    {
        return false;
    }
    
    //
    setContentSize(size);
    setIgnoreAnchorPointForPosition(false);
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    setClippingEnabled(true);
    setTouchEnabled(true);
    
    // init
    initVar();
    initUi();
    
    return true;
}

void LayerAdventureBossBattle::onEnter()
{
    ui::Layout::onEnter();
 
    if ( _bInitEnter == false )
    {
        _bInitEnter = true;
        
        uiContainer();
        uiLobby();
    }
    
    //
    drawLobby();
}

#pragma mark - init
void LayerAdventureBossBattle::initVar()
{
    auto bossIdx = AdventureManager::getInstance()->getBossIdx();
    
    _objInfoBoss = AdventureManager::getInstance()->getInfoBoss(bossIdx);
    _objInfoBoss->retain();
}

void LayerAdventureBossBattle::initUi()
{
    
}

#pragma mark - ui
void LayerAdventureBossBattle::uiContainer()
{
    auto size = getContentSize();
    
    // ui default
    _uiContentsLobby = ui::Layout::create();
    _uiContentsLobby->setContentSize(size);
    _uiContentsLobby->setAnchorPoint(Vec2::ZERO);
    _uiContentsLobby->setPosition(Vec2::ZERO);
    addChild(_uiContentsLobby);
  
}

void LayerAdventureBossBattle::uiLobby()
{
    auto layout = _uiContentsLobby;
    auto size = layout->getContentSize();
    
    auto uiBG = ui::ImageView::create(_objInfoBoss->getIconBattleBGPath());
    uiBG->setPosition(Vec2(size.width / 2, size.height / 2));
    ((ui::Scale9Sprite*)uiBG->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
    uiBG->setScale(4);
    layout->addChild(uiBG);
    
    auto uiBoss = ui::ImageView::create(_objInfoBoss->getIconBattlePath());
    uiBoss->setPosition(Vec2(size.width / 2, size.height / 2));
    ((ui::Scale9Sprite*)uiBoss->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
    uiBoss->setScale(4);
    uiBoss->runAction(RepeatForever::create(Sequence::create(MoveBy::create(2.5, Vec2(0, 20)), MoveBy::create(0.5, Vec2(0, -20)), NULL)));
    layout->addChild(uiBoss);
    
    auto uiDeck = ui::Layout::create();
    uiDeck->setContentSize(Size(size.width, size.height));
    uiDeck->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    uiDeck->setPosition(Vec2(size.width / 2, 0));
    uiDeck->setName("UI_DECK");
    layout->addChild(uiDeck);
    {
        auto uiCannon = ui::ImageView::create("Assets/ui/adventure/boss/hit_shell_cannon_1_1.png");
        uiCannon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        uiCannon->setPosition(Vec2(uiDeck->getContentSize().width / 2, 0));
        ((ui::Scale9Sprite*)uiCannon->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
        uiCannon->setScale(4);
        uiCannon->setName("UI_CANNON");
        uiDeck->addChild(uiCannon);
        
        auto uiCannonPart1 = ui::ImageView::create("Assets/ui/adventure/boss/hit_shell_part_1.png");
        uiCannonPart1->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        uiCannonPart1->setPosition(Vec2(uiDeck->getContentSize().width / 2 + 2, 36));
        uiDeck->addChild(uiCannonPart1);
        
        auto uiCannonPart2 = ui::ImageView::create("Assets/ui/adventure/boss/hit_shell_part_2.png");
        uiCannonPart2->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        uiCannonPart2->setPosition(Vec2(uiDeck->getContentSize().width / 2 + 2, 36));
        uiDeck->addChild(uiCannonPart2, 10);
        
        auto uiBullets = ui::Layout::create();
        uiBullets->setContentSize(uiCannonPart1->getContentSize());
        uiBullets->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        uiBullets->setPosition(Vec2(uiCannonPart1->getPositionX(), uiCannonPart1->getPositionY() + uiCannonPart1->getContentSize().height / 2));
        uiBullets->setName("UI_BULLETS");
        uiDeck->addChild(uiBullets);
        {
            for ( int i = 0; i < 6; i++ )
            {
                auto uiBullet = ui::ImageView::create(MafUtils::format("Assets/ui/adventure/boss/hit_shell_%02d.png", i+1));
                uiBullet->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
                uiBullet->setPosition(Vec2(uiBullets->getContentSize().width / 2, uiBullets->getContentSize().height / 2));
                uiBullet->setRotation(360 - (72 * i));
                uiBullets->addChild(uiBullet);
            }
        }
    }
}

#pragma mark - ui : draw
void LayerAdventureBossBattle::drawLobby()
{
    auto layout = _uiContentsLobby;
}

#pragma mark - ui : draw
void LayerAdventureBossBattle::animationCannon()
{
    auto layout = _uiContentsLobby;
    auto size = layout->getContentSize();
     
    auto uiDeck = utils::findChild<ui::Layout*>(layout, "UI_DECK");
    auto uiCannon = utils::findChild<ui::ImageView*>(layout, "UI_CANNON");
    
    {
        auto action1 = DelayTime::create(0.4);
        auto action2 = Shake::create(0.4, 6);
        layout->runAction(Sequence::create(action1, action2, NULL));
    }
    
    {
        auto action1 = ScaleTo::create(0.1, 1.1);
        auto action2 = DelayTime::create(0.4 + 0.2);
        auto action3 = ScaleTo::create(0.1, 1.0);
        uiDeck->runAction(Sequence::create(action1, action2, action3, NULL));
    }
    
    {
        auto animation = Animation::create();
        animation->setDelayPerUnit(0.1);
        for ( int i = 1; i <= 6; i++ )
        {
            auto spriteAni = Sprite::create(MafUtils::format("Assets/ui/adventure/boss/hit_shell_cannon_1_%d.png", i).c_str());
            spriteAni->getTexture()->setTexParameters(texParams);
            animation->addSpriteFrame(spriteAni->getSpriteFrame());
        }
        
        auto action1 = Animate::create(animation);
        auto action2 = CallFunc::create([=](){
            
            bool bWeakness = false;
            auto damage = 100 * _bulletItemCount;
            
            if ( _bulletItemIdx == 121 || _objInfoBoss->getWeakness1() == _bulletItemIdx || _objInfoBoss->getWeakness2() == _bulletItemIdx )
            {
                bWeakness = true;
                damage *= 2;
            }
            
            auto uiDamage = ui::CText::create(std::to_string(damage), GAME_FONT_AL, 65);
            uiDamage->setPosition(Vec2(this->getContentSize().width / 2 + 100, this->getContentSize().height - 200));
            uiDamage->setTextColor(Color4B::WHITE);
            uiDamage->runAction(Sequence::create(MoveBy::create(0.4, Vec2(20, 50)), FadeOut::create(0.2), RemoveSelf::create(), NULL));
            this->addChild(uiDamage);
            if ( bWeakness == true )
            {
                uiDamage->setTextColor(Color4B::RED);
            }
            
            if ( _callbackAttackFinish != nullptr )
            {
                _callbackAttackFinish();
            }
        });
        uiCannon->getVirtualRenderer()->stopAllActions();
        uiCannon->getVirtualRenderer()->runAction(Sequence::create(action1, action2, NULL));
    }
    
    {
        auto animation = Animation::create();
        animation->setDelayPerUnit(0.1);
        for ( int i = 1; i <= 5; i++ )
        {
            auto spriteAni = Sprite::create(MafUtils::format("Assets/ui/adventure/boss/hit_shell_cannon_2_%d.png", i).c_str());
            spriteAni->getTexture()->setTexParameters(texParams);
            animation->addSpriteFrame(spriteAni->getSpriteFrame());
        }
        
        auto uiEffect = Sprite::create("Assets/ui/adventure/boss/hit_shell_cannon_2_1.png");
        uiEffect->setPosition(Vec2(size.width / 2, size.height - 150));
        uiEffect->runAction(Sequence::create(DelayTime::create(0.5), Show::create(), Animate::create(animation), RemoveSelf::create(), NULL));
        uiEffect->getTexture()->setTexParameters(texParams);
        uiEffect->setScale(4);
        uiEffect->setVisible(false);
        layout->addChild(uiEffect);
        
    }
}

void LayerAdventureBossBattle::animationBulletChange()
{
    auto layout = _uiContentsLobby;
    
    auto uiBullets = utils::findChild<ui::Layout*>(layout, "UI_BULLETS");
    
    auto bulletPosition = getBulletPosition();
    
    auto action1 = RotateTo::create(0.2, 72 * bulletPosition + 10);
    auto action2 = RotateTo::create(0.1, 72 * bulletPosition);
    uiBullets->stopAllActions();
    uiBullets->runAction(Sequence::create(action1, action2, NULL));
}

#pragma mark - ui : click

#pragma mark - callback

#pragma mark - schedule

#pragma mark - set, get
void LayerAdventureBossBattle::setCallbackAttackFinish(const std::function<void(void)>& callback)
{
    _callbackAttackFinish = callback;
}

void LayerAdventureBossBattle::setBulletItemIdx(int itemIdx)
{
    _bulletItemIdx = itemIdx;
}

void LayerAdventureBossBattle::setBulletItemCount(int itemCount)
{
    _bulletItemCount = itemCount;
}

int LayerAdventureBossBattle::getBulletPosition()
{
    int pos = 0;
    switch (_bulletItemIdx) {
        case 117:       pos = 0; break;
        case 118:       pos = 1; break;
        case 119:       pos = 2; break;
        case 120:       pos = 3; break;
        case 121:       pos = 4; break;
            
        default:
            break;
    }
    
    return pos;
}

#pragma mark - game
void LayerAdventureBossBattle::onBulletChange()
{
    animationBulletChange();
}

void LayerAdventureBossBattle::onAttack()
{
    animationCannon();
}
