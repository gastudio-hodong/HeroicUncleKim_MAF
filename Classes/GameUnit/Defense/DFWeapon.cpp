//
//  DFWeapon.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/07/07.
//

#include "DFWeapon.h"

#include "Common/ObjectPool/MafPool.h"
#include "Common/ObjectPool/PoolableObjectComponent.h"

#include "GameUnit/Defense/DFBattleManager.h"

#include "GameObject/Grid/InfoGridTileData.h"
#include "GameObject/Grid/Grid.h"
#include "GameObject/Defense/InfoDefenseWeapon.h"
#include "GameObject/Defense/InfoDefenseBullet.h"
#include "GameObject/Defense/InfoDefenseTrench.h"
#include "GameObject/Defense/InfoDefenseCharacter.h"

#include "GameUnit/Defense/DFBullet.h"
#include "GameUnit/Defense/DFEffect.h"

#include "ManagerGame/DefenseManager.h"

USING_NS_CC;

DFWeapon* DFWeapon::create()
{
    DFWeapon* pRet = new(std::nothrow) DFWeapon();
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

DFWeapon::DFWeapon() :
_num(0),
_weaponData(nullptr),
_grid(nullptr),
_battleManager(nullptr),
_muzzleFX(nullptr),
_weapon(nullptr)
{
    
}

DFWeapon::~DFWeapon()
{
    DefenseManager::getInstance()->_onUpdateWeapon.Clear(this);
}

bool DFWeapon::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    DefenseManager::getInstance()->_onUpdateWeapon += new MafFunction<void(void)>(CC_CALLBACK_0(DFWeapon::activateObject, this), this);
    
    setIgnoreAnchorPointForPosition(false);
    setContentSize(Size(177, 177));
    updateWeapon();
    
    return true;
}
void DFWeapon::update(float dt)
{
}
void DFWeapon::activateObject()
{
    stopAllActions();
    updateWeapon();
    
    if(_weaponData == nullptr)
        return;
    
    if(auto trench = DefenseManager::getInstance()->getInfoTrench(_num); trench->getCharacterIdx() > 0)
    {
        auto action1 = CallFunc::create(CC_CALLBACK_0(DFWeapon::shotBullet, this));
        
        double speed = _weaponData->getBaseDFAttackSpeed();
        if(auto characterData = trench->getInfoDefenseCharacter())
            speed = characterData->getBaseDFAttackSpeed() / 100 * _weaponData->getBaseDFAttackSpeed();
        
        auto seq = Sequence::create(action1, DelayTime::create(1.f / speed), nullptr);
        runAction(RepeatForever::create(seq));
    }
}
#pragma mark -get set
void DFWeapon::setGrid(Grid* grid)
{
    _grid = grid;
}
void DFWeapon::setBattleManager(DFBattleManager* battleManager)
{
    _battleManager = battleManager;
}
void DFWeapon::setNum(int num)
{
    _num = num;
}

#pragma mark -update
void DFWeapon::updateWeapon()
{
    if(_weapon == nullptr)
    {
        _weapon = Sprite::create();
        _weapon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        _weapon->setPosition(Vec2(getContentSize()/2));
        addChild(_weapon);
    }
    _weapon->stopAllActions();
    _weapon->setTexture("");
    _weaponData = DefenseManager::getInstance()->getEquipWeaponDataByTrench(_num);
    if(_weaponData == nullptr)
        return;
    
    std::string imgPath = MafUtils::format("Assets/ui/defense/Defense_gun_lv%d_00.png", _weaponData->getIndex());
    _weapon->setTexture(imgPath);
}

#pragma mark -Utils
void DFWeapon::shotBullet()
{
    auto bulletData = DefenseManager::getInstance()->getBulletData(_weaponData->getBulletType());
    
    auto characterData = DefenseManager::getInstance()->getCharacterData(_num);
    if(_battleManager->getActivateMonsterCount() <= 0)
    {
        if(bulletData->getAttackType() != E_DF_AttackType::NONE_TARGETING)
            return;
    }
           
    if(bulletData->getBulletValue() > 1)
    {
        shotBulletByShotGun();
        return;
    }
    
    auto bullet = _battleManager->spawnBullet();
    if(bullet == nullptr)
        return;

    auto trench = getParent();
    if(auto bulletData = DefenseManager::getInstance()->getBulletData(_weaponData->getBulletType()))
    {
        bullet->setBattleManager(_battleManager);
        bullet->setBulletData(bulletData);
        bullet->setBulletDamage(_weaponData->getDFAttack() + characterData->getDFAttack());
        bullet->setPosition(trench->getPosition() + Vec2::RIGHT * _weapon->getPosition().x + Vec2::RIGHT * _weapon->getContentSize().width/2);
        bullet->setWeaponType(_weaponData->getWeaponType());
        
        if(bulletData->getAttackType() == E_DF_AttackType::NONE_TARGETING)
        {
            bullet->setTargetLocation(Vec2(750, trench->getPositionY()));
        }
    }


    if(auto poolComp = (PoolableObjectComponent*)bullet->getComponent("Poolable"))
    {
        poolComp->ActivateObject();
        playMuzzleEffect(bullet);
    }
    
    playShotAnimation();
    playCartridgeEffect(bullet);
}
void DFWeapon::shotBulletByShotGun()
{
    auto bulletData = DefenseManager::getInstance()->getBulletData(_weaponData->getBulletType());
    auto characterData = DefenseManager::getInstance()->getCharacterData(_num);
    float angle = 1.8f;
    float anglePiece = (angle * 2) / ((float)bulletData->getBulletValue() - 1.f);
    
    DFBullet* lastBullet = nullptr;
    for(int i = 0; i < bulletData->getBulletValue(); ++i)
    {
        auto bullet = _battleManager->spawnBullet();
        if(bullet == nullptr)
            return;
        auto trench = getParent();
        
        Vec2 bulletLocation = trench->getPosition() + Vec2::RIGHT * _weapon->getPosition().x + Vec2::RIGHT * _weapon->getContentSize().width/2;
        Vec2 targetLocation = Vec2(750, trench->getPosition().y);
        
        
        targetLocation += Vec2(1, angle) * 300;
        
        bullet->setBattleManager(_battleManager);
        bullet->setBulletData(DefenseManager::getInstance()->getBulletData(_weaponData->getBulletType()));
        bullet->setBulletDamage(_weaponData->getDFAttack() + characterData->getDFAttack());
        bullet->setPosition(bulletLocation);
        bullet->setTargetLocation(targetLocation);
        bullet->setWeaponType(_weaponData->getWeaponType());

        if(auto poolComp = (PoolableObjectComponent*)bullet->getComponent("Poolable"))
        {
            poolComp->ActivateObject();
        }
        
        angle -= anglePiece;
        lastBullet = bullet;
    }
    
    playMuzzleEffect(lastBullet);
    playShotAnimation();
    playCartridgeEffect(lastBullet);
}

void DFWeapon::playShotAnimation()
{
    updateWeapon();
    _weapon->stopAllActions();
    
    int count = 0;
    auto anim = Animation::create();
    for(int i = 0; i <= 100; ++i)
    {
        std::string strPath = MafUtils::format("Assets/ui/defense/Defense_gun_lv%d_%02d.png", _weaponData->getIndex(), i);
        std::string fullpath = FileUtils::getInstance()->fullPathForFilename(strPath);
        
        if (fullpath.size() == 0)
            break;
        
        auto sprite = Sprite::create(strPath);
        sprite->getTexture()->setTexParameters(texParams);
        anim->addSpriteFrame(sprite->getSpriteFrame());
        count++;
    }
    auto sprite = Sprite::create(MafUtils::format("Assets/ui/defense/Defense_gun_lv%d_%02d.png", _weaponData->getIndex(), 0));
    sprite->getTexture()->setTexParameters(texParams);
    anim->addSpriteFrame(sprite->getSpriteFrame());
    
    double speed = _weaponData->getBaseDFAttackSpeed();
    if(auto trench = DefenseManager::getInstance()->getInfoTrench(_num); trench->getCharacterIdx() > 0)
    {
        if(auto characterData = trench->getInfoDefenseCharacter())
            speed = characterData->getBaseDFAttackSpeed() / 100 * _weaponData->getBaseDFAttackSpeed();
    }
    
    float delay = 0.15f / speed / (float)count;
    
    if(delay <= 0.05f)
        delay = 0.05f;
    anim->setDelayPerUnit(delay);
    auto animate = Animate::create(anim);
    _weapon->runAction(animate);
}
void DFWeapon::playMuzzleEffect(DFBullet* bullet)
{
    auto bulletData = DefenseManager::getInstance()->getBulletData(_weaponData->getBulletType());
    if(bulletData == nullptr)
        return;
    
    auto trench = getParent();
    
    std::string muzzlePath = "";
    int rand = 0;
    switch(bulletData->getMuzzleEffectType())
    {
        case E_DF_MuzzleEffectType::SMALL_TYPE:
        {
            rand = random(1,3);
        }break;
        case E_DF_MuzzleEffectType::STLONG_TYPE:
        {
            rand = random(4,6);
        }break;
        case E_DF_MuzzleEffectType::BIG_STLONG_TYPE:
        {
            rand = 7;
        }break;
        case E_DF_MuzzleEffectType::IMAGE_NUM_TYPE:
        {
            rand = bulletData->getImageNum();
        }break;
        default: break;
    }
    
    muzzlePath = MafUtils::format("Assets/ui/defense/Gun_effect_%02d_01.png",rand);
    
    if(auto muzzleEffect = _battleManager->spawnEffectObject())
    {
        muzzleEffect->setPosition(trench->getPosition() + Vec2::RIGHT * _weapon->getPosition().x + Vec2::RIGHT * _weapon->getContentSize().width/2);
        muzzleEffect->setPath("Assets/ui/defense/Gun_effect_%02d_%02d.png");
        muzzleEffect->setNum(rand);
        if(auto poolCompFX = (PoolableObjectComponent*)muzzleEffect->getComponent("Poolable"))
        {
            poolCompFX->ActivateObject();
        }
    }
}
void DFWeapon::playCartridgeEffect(DFBullet* bullet)
{
    auto bulletData = DefenseManager::getInstance()->getBulletData(_weaponData->getBulletType());
    if(bulletData == nullptr)
        return;
    
    auto trench = getParent();
    
    std::string fxPath = "";
    int rand = 0;
    switch(bulletData->getCartridgeType())
    {
        case E_DF_CartridgeType::NONE:
        {
            return;
        }break;
        case E_DF_CartridgeType::BULLET:
        {
            rand = random(1,3);
            fxPath = "Assets/ui/defense/Defense_cartridge_%02d_%02d.png";
        }break;
        default: return; break;
    }
    
    if(auto cartridgeEffect = _battleManager->spawnEffectObject())
    {
        cartridgeEffect->setPosition(trench->getPosition() + Vec2::RIGHT * _weapon->getPosition().x);
        cartridgeEffect->setPath(fxPath);
        cartridgeEffect->setNum(rand);
        cartridgeEffect->setPlayScale(1.5f);
        cartridgeEffect->setPlaySpeed(0.5f);
        if(auto poolCompFX = (PoolableObjectComponent*)cartridgeEffect->getComponent("Poolable"))
        {
            poolCompFX->ActivateObject();
        }
    }
}
