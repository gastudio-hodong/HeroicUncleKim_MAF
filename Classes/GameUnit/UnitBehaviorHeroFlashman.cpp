//
//  Model.cpp
//  HexagonSnake
//
//  Created by SanHeo on 2014. 7. 27..
//
//

#include "UnitBehaviorHeroFlashman.h"

#include "ManagerGame/UserInfoManager.h"

#include "GameUnit/Animation/UnitAnimationHero.h"
#include "GameUnit/UnitBehaviorMonsterFrost.h"

#include "ManagerGame/DataManager.h"
#include "ManagerGame/CostumeManager.h"
#include "ManagerGame/CostumeSkinManager.h"
#include "ManagerGame/WeaponManager.h"

USING_NS_CC;

UnitBehaviorHeroFlashman* UnitBehaviorHeroFlashman::create(E_PLACE ePlace)
{
    UnitBehaviorHeroFlashman *pRet = new (std::nothrow) UnitBehaviorHeroFlashman();
    if ( pRet )
    {
        pRet->setPlace(ePlace);
        pRet->init();
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        return nullptr;
    }
}

UnitBehaviorHeroFlashman::UnitBehaviorHeroFlashman() :
_unitHero(nullptr),

_callbackAttack(nullptr)
{
    
}

UnitBehaviorHeroFlashman::~UnitBehaviorHeroFlashman()
{

}

bool UnitBehaviorHeroFlashman::init()
{
    if ( !UnitBehavior::init())
    {
        return false;
    }
    
    //
    _unitHero = UnitAnimationHero::create();
    _unitHero->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _unitHero->setPosition(Vec2::ZERO);
    addChild(_unitHero);
    
    //
    auto size = _unitHero->getContentSize();
    setContentSize(size);
    setIgnoreAnchorPointForPosition(false);
    setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    
    return true;
}

void UnitBehaviorHeroFlashman::onEnter()
{
    UnitBehavior::onEnter();
    
    //
    setMotion();
    onWalk();
}
  
#pragma mark -
void UnitBehaviorHeroFlashman::setMotion(int skinDummy/* = 0*/)
{
    //
    double delayUnitWalk = UserInfoManager::getInstance()->getUserSpeed(_ePlace, true);
    double delayUnitAttack = UserInfoManager::getInstance()->getAttackSpeed(_ePlace);
    
    delayUnitWalk = 0.125f;
    delayUnitAttack = 0.05f;
    _unitHero->setDelayUnitWalk(delayUnitWalk);
    _unitHero->setDelayUnitAttack(delayUnitAttack);
    
    
    //
    UnitAnimationHero::E_TYPE eType = UnitAnimationHero::E_TYPE::DEFAULT;
    if ( skinDummy != 0 || CostumeSkinManager::getInstance()->getCostumeSkinEquip() != 0 )
    {
        eType = UnitAnimationHero::E_TYPE::SKIN;
    }
    else
    {
        eType = UnitAnimationHero::E_TYPE::HERO;
    }
    
    //
    if ( eType == UnitAnimationHero::E_TYPE::SKIN )
    {
        int skin = CostumeSkinManager::getInstance()->getCostumeSkinEquip();
        if ( skinDummy != 0 )
            skin = skinDummy;
        
        _unitHero->setMotionSkin(skin);
    }
    else
    {
        int equipAura = CostumeManager::getInstance()->getEquip(E_COSTUME::SETITEM);
        int weaponEquip = WeaponManager::getInstance()->getEquipVisual();
        if ( _ePlace == E_PLACE::DG_PRISON )
        {
            weaponEquip = E_WEAPON::WEAPON_1;
        }
        
        int body = (int)CostumeManager::getInstance()->getEquip(E_COSTUME::BODY);
        int head = (int)CostumeManager::getInstance()->getEquip(E_COSTUME::HEAD);
        int hand = (int)CostumeManager::getInstance()->getEquip(E_COSTUME::HAND);
        int foot = (int)CostumeManager::getInstance()->getEquip(E_COSTUME::FOOT);
        int cape = (int)CostumeManager::getInstance()->getEquip(E_COSTUME::CAPE);
        
        _unitHero->setMotionCostume(weaponEquip, body, head, hand, foot, cape);
        
        //
        if ( equipAura == E_COSTUME_IDX::IDX_1_DEFAULT )
        {
            _unitHero->setAura(false, Color3B::WHITE);
        }
        else
        {
            _unitHero->setAura(true, DataManager::GetAuraColor((E_COSTUME_IDX)equipAura));
        }
    }
}

void UnitBehaviorHeroFlashman::setCallbackAttack(const std::function<void(void)>& callbackAttack)
{
    _callbackAttack = callbackAttack;
}

#pragma mark - event
void UnitBehaviorHeroFlashman::onWalk()
{
    _unitHero->stopAllActions();
    _unitHero->animationWalk();
}

void UnitBehaviorHeroFlashman::onAttack()
{
    //
    _unitHero->stopAllActions();
    _unitHero->animationAttack(false);
    
    //
    if ( _callbackAttack != nullptr )
    {
        _callbackAttack();
    }
}

#pragma mark -
