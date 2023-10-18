//
//  Model.cpp
//  HexagonSnake
//
//  Created by SanHeo on 2014. 7. 27..
//
//

#include "UnitBehaviorHeroFrost.h"

#include "ManagerGame/UserInfoManager.h"

#include "GameUnit/Animation/UnitAnimationHero.h"
#include "GameUnit/UnitBehaviorMonsterFrost.h"

#include "ManagerGame/DataManager.h"
#include "ManagerGame/CostumeManager.h"
#include "ManagerGame/CostumeSkinManager.h"
#include "ManagerGame/PrisonFrostManager.h"
#include "ManagerGame/WeaponManager.h"

USING_NS_CC;

UnitBehaviorHeroFrost* UnitBehaviorHeroFrost::create(E_PLACE ePlace)
{
    UnitBehaviorHeroFrost *pRet = new (std::nothrow) UnitBehaviorHeroFrost();
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

UnitBehaviorHeroFrost::UnitBehaviorHeroFrost() :
_unitHero(nullptr),
_unitMonster(nullptr),

_callbackDamage(nullptr)
{
    
}

UnitBehaviorHeroFrost::~UnitBehaviorHeroFrost()
{

}

bool UnitBehaviorHeroFrost::init()
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

void UnitBehaviorHeroFrost::onEnter()
{
    UnitBehavior::onEnter();
    
    //
    setMotion();
    onWalk();
}
  
#pragma mark -
void UnitBehaviorHeroFrost::setMotion()
{
    //
    double delayUnitWalk = UserInfoManager::getInstance()->getUserSpeed(_ePlace, true);
    double delayUnitAttack = UserInfoManager::getInstance()->getAttackSpeed(_ePlace);
    
    delayUnitWalk = 0.125f;
    _unitHero->setDelayUnitWalk(delayUnitWalk);
    _unitHero->setDelayUnitAttack(delayUnitAttack);
    
    
    //
    UnitAnimationHero::E_TYPE eType = UnitAnimationHero::E_TYPE::DEFAULT;
    if ( _ePlace == E_PLACE::DG_NORMAL && UserInfoManager::getInstance()->getBuffTime(E_BUFF_TYPE::BUFF_HERO_BONUS) > 0 && UserInfoManager::getInstance()->getBuffVisible(E_BUFF_TYPE::BUFF_HERO_BONUS) == true )
    {
        eType = UnitAnimationHero::E_TYPE::HERO;
    }
    else if ( CostumeSkinManager::getInstance()->getCostumeSkinEquip() != 0 )
    {
        eType = UnitAnimationHero::E_TYPE::SKIN;
    }
    
    //
    if ( eType == UnitAnimationHero::E_TYPE::DEFAULT )
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
    else if ( eType == UnitAnimationHero::E_TYPE::SKIN )
    {
        int skin = CostumeSkinManager::getInstance()->getCostumeSkinEquip();
        
        _unitHero->setMotionSkin(skin);
    }
}

void UnitBehaviorHeroFrost::setMonster(UnitBehaviorMonsterFrost* unitMonster)
{
    _unitMonster = unitMonster;
}

#pragma mark - event
void UnitBehaviorHeroFrost::onWalk()
{
    _unitHero->stopAllActions();
    _unitHero->animationWalk();
}

void UnitBehaviorHeroFrost::onAttack(const std::function<void(std::string)>& callbackDamage)
{
    _callbackDamage = callbackDamage;
    
    //
    _unitHero->stopAllActions();
    _unitHero->animationAttack(true);
    
    //
    auto perAttack = UserInfoManager::getInstance()->getAttackSpeed(E_PLACE::DG_NORMAL);
    auto action1 = CallFunc::create([=](){
        
        //
        bool critical   = UserInfoManager::getInstance()->getCriticalRate(_ePlace, true, true);
        
        //
        std::string damage = UserInfoManager::getInstance()->getAttackDamage(_ePlace, E_CHARACTER::CHA_HERO, critical, false);
        damage = getAttackSkillFrostDamage(damage);
        
        //
        double effectBonus = PrisonFrostManager::getInstance()->getAdsEffect();
        damage = MafUtils::bigMulNum(damage, MafUtils::doubleToString(effectBonus));
        
        //
        int hitMask = E_CHARACTER_HIT::HIT_NORMAL;
        if ( _isFrostSkillCri == true )
        {
            hitMask = hitMask | E_CHARACTER_HIT::HIT_FROSTSKILL_CRI;
            _isFrostSkillCri = false;
        }
        _unitMonster->onDamage(damage, critical, hitMask);
        
        //
        if ( _callbackDamage != nullptr )
        {
            _callbackDamage(damage);
        }
    });
    auto action2 = DelayTime::create(perAttack);
    auto seq = Sequence::create(action1, action2, NULL);
    _unitHero->runAction(RepeatForever::create(seq));
}
  


#pragma mark -
std::string UnitBehaviorHeroFrost::getAttackSkillFrostDamage(std::string &tmpAtk)
{
    auto dmg = tmpAtk;
    
    double rate = PrisonFrostManager::getInstance()->getSkillEffect(50002, PrisonFrostManager::getInstance()->getSkillLevel(50002));
    if ( random(0.f, 100.f) > rate )
        return dmg;

    double effect = PrisonFrostManager::getInstance()->getSkillEffect(50001, PrisonFrostManager::getInstance()->getSkillLevel(50001));
    if ( effect > 0 )
    {
        std::string temp = MafUtils::bigMulNum(dmg, MafUtils::doubleToString(effect));
        temp.pop_back();
        temp.pop_back();
        dmg = MafUtils::bigAddNum(dmg,temp);
    }
    
    _isFrostSkillCri = true;

    return dmg;
}
