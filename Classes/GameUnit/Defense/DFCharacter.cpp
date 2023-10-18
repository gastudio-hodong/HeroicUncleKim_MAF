//
//  DFCharacter.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/07/07.
//

#include "DFCharacter.h"

#include "GameObject/Grid/InfoGridTileData.h"
#include "GameObject/Grid/Grid.h"
#include "GameObject/Defense/InfoDefenseMap.h"
#include "GameObject/Defense/InfoDefenseCharacter.h"
#include "GameObject/Defense/InfoDefenseTrench.h"

#include "ManagerGame/DefenseManager.h"
USING_NS_CC;

DFCharacter* DFCharacter::create()
{
    DFCharacter* pRet = new(std::nothrow) DFCharacter();
    if ( pRet && pRet->init() )
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

DFCharacter::DFCharacter() :
_num(0),
_grid(nullptr),
_character(nullptr)
{
    
}

DFCharacter::~DFCharacter()
{
    DefenseManager::getInstance()->_onUpdateCharacter.Clear(this);
}

bool DFCharacter::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    DefenseManager::getInstance()->_onUpdateCharacter += new MafFunction<void(void)>(CC_CALLBACK_0(DFCharacter::activateObject, this), this);
    
    setIgnoreAnchorPointForPosition(false);
    setContentSize(Size(177, 177));
    updateCharacter();
    
    return true;
}
void DFCharacter::update(float dt)
{
}
void DFCharacter::activateObject()
{
    updateCharacter();
}
#pragma mark -get set
void DFCharacter::setGrid(Grid* grid)
{
    _grid = grid;
}

void DFCharacter::setNum(int num)
{
    _num = num;
}
#pragma mark -update

#pragma mark -update
void DFCharacter::updateCharacter()
{
    if(_character == nullptr)
    {
        _character = Sprite::create();
        _character->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        _character->setPosition(getContentSize().width/2 - 15, 45);
        addChild(_character);
    }
    _character->setTexture("");
    _character->stopAllActions();
    
    auto trenchData = DefenseManager::getInstance()->getInfoTrench(_num);
    if(trenchData == nullptr || trenchData->getCharacterIdx() == 0)
        return;
    
    auto characterIdx = trenchData->getCharacterIdx();
    
    _character->setTexture(MafUtils::format("Assets/ui/defense/Defense_mole_lv%d_%02d.png", characterIdx, 0));
    
    auto anim = Animation::create();
    for(int i = 0; i < 2; ++i)
    {
        std::string strPath = MafUtils::format("Assets/ui/defense/Defense_mole_lv%d_%02d.png", characterIdx, i);
        
        auto sprite = Sprite::create(strPath);
        sprite->getTexture()->setTexParameters(texParams);
        anim->addSpriteFrame(sprite->getSpriteFrame());
    }
    
    anim->setDelayPerUnit(0.3f);
    auto animate = Animate::create(anim);
    _character->runAction(RepeatForever::create(animate));
}

