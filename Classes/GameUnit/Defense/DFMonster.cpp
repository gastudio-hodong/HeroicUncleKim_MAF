//
//  DefenseMonster.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/07/06.
//

#include "DFMonster.h"

#include "Common/ObjectPool/PoolableObjectComponent.h"
#include "GameUnit/Defense/DFBattleManager.h"

#include "GameUnit/Defense/DFBullet.h"
#include "GameUnit/Defense/DFEffect.h"
#include "GameUnit/Defense/DFHPBar.h"

#include "GameObject/Grid/InfoGridTileData.h"
#include "GameObject/Grid/Grid.h"
#include "GameObject/Defense/InfoDefenseMap.h"
#include "GameObject/Defense/InfoDefenseBullet.h"
#include "GameObject/Defense/InfoDefenseMonster.h"
#include "GameObject/Defense/InfoDefenseFloor.h"

#include "ManagerGame/DefenseManager.h"
USING_NS_CC;

DFMonster* DFMonster::create()
{
    DFMonster* pRet = new(std::nothrow) DFMonster();
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

DFMonster::DFMonster() :
_isDead(false),
_speed(0),
_hp(0),
_defensivePower(0),
_currentPos(IntPoint::ZeroValue),
_type(E_DF_MonsterType::NONE),
_tramcar(nullptr),
_tramcarFront(nullptr),
_deadEffect(nullptr),
_monster(nullptr),
_hpBar(nullptr)
{
}

DFMonster::~DFMonster()
{
}

bool DFMonster::init()
{
    if ( !DFObject::init() )
    {
        return false;
    }
    setContentSize(Size(84, 84));
    
    return true;
}
void DFMonster::update(float dt)
{
    DFObject::update(dt);
    movement(dt);
}
void DFMonster::activateObject()
{
    DFObject::activateObject();
    if(auto floorData = DefenseManager::getInstance()->getCurrentFloorData(); int mapNumber = floorData->getMapNumber())
    {
        if(auto mapData = DefenseManager::getInstance()->getMapData(mapNumber))
        {
            _path = mapData->getPath();
        }
    }
    
    _isDead = false;
    moveCurrentPos();
    _grid = _battleManager->getGrid();
    
    setPosition(_grid->getTileLocationFromGridIndex(_currentPos));
    
    if(_hpBar == nullptr)
    {
        _hpBar = _battleManager->getHPBar();
        _hpBar->setOwnerMonster(this);
    }
    if(auto poolComp = (PoolableObjectComponent*)_hpBar->getComponent("Poolable"))
    {
        poolComp->ActivateObject();
    }
    _hpBar->setVisible(true);
    
    updateMonsterStatus();
    updateTramcar();
    updateMonster();
}
void DFMonster::deactivateObject()
{
    _isDead = true;
    _type = E_DF_MonsterType::NONE;
    _speed = 0;
    _defensivePower = 0;
    _hp = 0;
    _hpMax = 0;
    

    
    DFObject::deactivateObject();
}
#pragma mark -get set
void DFMonster::setMonsterType(E_DF_MonsterType type)
{
    _type = type;
}
int DFMonster::getPathLeftSize()
{
    return _path.size();
}
bool DFMonster::isDead()
{
    return _isDead;
}
E_DF_MonsterType DFMonster::getMonsterType()
{
    return _type;
}
double DFMonster::getMaxHP()
{
    return _hpMax;
}
double DFMonster::getCurrentHP()
{
    return _hp;
}
Sprite* DFMonster::getMonsterSprite()
{
    return _monster;
}
#pragma mark -update
void DFMonster::updateTramcar()
{
    if(_tramcar == nullptr)
    {
        _tramcar = Sprite::create();
        _tramcar->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        _tramcar->setPosition(Vec2(getContentSize().width/2, 0));
        addChild(_tramcar, 0);
    }
    if(_tramcarFront == nullptr)
    {
        _tramcarFront = Sprite::create();
        _tramcarFront->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        _tramcarFront->setPosition(Vec2(getContentSize().width/2, 0));
        addChild(_tramcarFront, 3);
    }
    
    std::string tramcarPath = "Assets/ui/defense/Defense_tramcar_lv1.png";
    std::string tramcarFrontPath = "Assets/ui/defense/Defense_tramcar_lv1_front.png";
    switch(_type)
    {
        case E_DF_MonsterType::NORMAL :
        {
            tramcarPath = "Assets/ui/defense/Defense_tramcar_lv1.png";
            tramcarFrontPath = "Assets/ui/defense/Defense_tramcar_lv1_front.png";
        }break;
        case E_DF_MonsterType::RUNNER :
        {
            tramcarPath = "Assets/ui/defense/Defense_tramcar_lv2.png";
            tramcarFrontPath = "Assets/ui/defense/Defense_tramcar_lv2_front.png";
        }break;
        case E_DF_MonsterType::TANKER :
        {
            tramcarPath = "Assets/ui/defense/Defense_tramcar_lv3.png";
            tramcarFrontPath = "Assets/ui/defense/Defense_tramcar_lv3_front.png";
        }break;
        case E_DF_MonsterType::BOSS :
        {
            tramcarPath = "Assets/ui/defense/Defense_tramcar_lv4.png";
            tramcarFrontPath = "Assets/ui/defense/Defense_tramcar_lv4_front.png";
        }break;
        default: break;
    }
    _tramcar->setTexture(tramcarPath);
    _tramcarFront->setTexture(tramcarFrontPath);
    _tramcar->runAction(FadeIn::create(0.1f));
    _tramcarFront->runAction(FadeIn::create(0.1f));
}
void DFMonster::updateMonster()
{
    if(_monster == nullptr)
    {
        _monster = Sprite::create();
        _monster->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        _monster->setPosition(getContentSize().width/2, 8);
        addChild(_monster, 2);
    }
    _monster->stopAllActions();
    _monster->setTexture(MafUtils::format("Assets/character_mon/normal/monster_%d.png", random(1, 200)));
    _monster->getTexture()->setTexParameters(texParams);
    
    switch(_type)
    {
        case E_DF_MonsterType::NORMAL :
        {
            _monster->setScale(2);
        }break;
        case E_DF_MonsterType::RUNNER :
        {
            _monster->setScale(2);
        }break;
        case E_DF_MonsterType::TANKER :
        {
            _monster->setScale(2);
        }break;
        case E_DF_MonsterType::BOSS :
        {
            _monster->setScale(3);
        }break;
        default: break;
    }
    
    auto action1 = ScaleBy::create(0.5f, 1, 1.3f);
    _monster->runAction(RepeatForever::create(Sequence::create(action1, action1->reverse(), nullptr)));
    _monster->runAction(FadeIn::create(0.1f));
    
}
void DFMonster::updateMonsterStatus()
{
    if(auto floorData = DefenseManager::getInstance()->getCurrentFloorData())
    {
        int monsterIdx = 1;
        switch(_type)
        {
            case E_DF_MonsterType::RUNNER: monsterIdx = floorData->getRunnerIdx(); break;
            case E_DF_MonsterType::TANKER: monsterIdx = floorData->getTankerIdx(); break;
            case E_DF_MonsterType::BOSS: monsterIdx = floorData->getBossIdx(); break;
            default: monsterIdx = 1; break;
        }
        if(auto data = DefenseManager::getInstance()->getMonsterData(monsterIdx))
        {
            _hpMax = _hp = data->getHpCoefficient() * floorData->getMonsterHp() * DefenseManager::getInstance()->getCurrentFloor();
            _defensivePower = data->getDefensive();
            _speed = data->getSpeed() * 100;
        }
    }
    
    _hpBar->updateHpBar();
}
#pragma mark -utill
void DFMonster::onHit(double damage, int hitEffectNumber, Vec2 hitLocation, Vec2 hitSize, bool isExplosion)
{
    playHitEffect(hitEffectNumber, hitLocation, hitSize, isExplosion);
    
    double tmpDefensive = DefenseManager::getInstance()->getCurrentFloor() * _defensivePower * 0.1f;
    damage -= tmpDefensive;
    if(damage <= 0)
        damage = 0;
    
    _hp -= damage;
    
    _hpBar->updatePercentage();
    if(_hp <= 0)
    {
        _battleManager->addMonsterKillCount();
        deadMonster();
        return;
    }
}

void DFMonster::movement(float dt)
{
    if(getPosition().distance(_nextLocation) <= 20)
    {
        moveCurrentPos();
        return;
    }
    
    Vec2 dir = MafMath::getSsfeNormal(_nextLocation - getPosition());
    
    IntPoint a = IntPoint(dir.x, dir.y);
    Vec2 location = getPosition() + dir * _speed * dt;
    
    
    setPosition(location);
    if(_hpBar)
        _hpBar->movement(dt);
}
void DFMonster::moveCurrentPos()
{
    if(_grid == nullptr || _isDead)
        return;
    
    if(_path.size() > 1)
    {
        _currentPos = _path[0];
        _nextLocation = _grid->getTileLocationFromGridIndex(_path[1]);
        _path.erase(_path.begin());
    }
    else
    {
        DefenseManager::getInstance()->FailStage();
    }
}
void DFMonster::deadMonster()
{
    _isDead = true;
    if(_deadEffect == nullptr)
    {
        _deadEffect = Sprite::create();
        _deadEffect->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        _deadEffect->setPosition(getContentSize()/2);
        addChild(_deadEffect, 3);
    }
    
    if(auto poolComp = (PoolableObjectComponent*)_hpBar->getComponent("Poolable"))
    {
        poolComp->DeactivateObject();
    }
    
    auto anim = Animation::create();
    anim->setDelayPerUnit(0.05f);
    for(int i = 0; i <= 6; ++i)
    {
        std::string strPath = MafUtils::format("Assets/ui/defense/Die_effect%02d_%02d.png", 1, i);
        std::string fullpath = FileUtils::getInstance()->fullPathForFilename(strPath);
        
        if (fullpath.size() == 0)
            break;
        
        anim->addSpriteFrameWithFile(strPath);
    }
    auto animate = Animate::create(anim);
    _deadEffect->runAction(Sequence::create(Show::create(), animate, Hide::create(), nullptr));
    
    _tramcar->runAction(FadeOut::create(0.35f));
    _tramcarFront->runAction(FadeOut::create(0.35f));
    _monster->runAction(FadeOut::create(0.35f));
    runAction(Sequence::create(DelayTime::create(0.35f), CallFunc::create(CC_CALLBACK_0(DFMonster::deactivateObject, this)), nullptr));
}

void DFMonster::playHitEffect(int hitEffectNumber, Vec2 hitLocation, Vec2 hitSize, bool isExplosion)
{
    auto action1 = TintTo::create(0.1f, 255, 88, 88);
    auto action2 = TintTo::create(0.1f, 255, 255, 255);
    
    _monster->runAction(Sequence::create(action1, action2,  nullptr));
    
    if(auto hitEffect = _battleManager->spawnEffectObject())
    {
        Vec2 hitBox = Vec2::ZERO;
        Vec2 HitEffectLocation =  Vec2::ZERO;
        
        if(hitLocation.equals(Vec2::ZERO))
        {
            HitEffectLocation = getPosition() + Vec2::LEFT * (_tramcar->getContentSize().width/2 - 25);
            HitEffectLocation.x = random(HitEffectLocation.x, getPositionX() + 20);
            HitEffectLocation.y = random(HitEffectLocation.y - 20, HitEffectLocation.y + 20);
            
        }
        else
        {
            float x = getPositionX() - _tramcar->getContentSize().width/2;
            hitBox = Vec2(20, hitSize.y);
            HitEffectLocation = Vec2(random(x + hitBox.x / 2, x + hitBox.x * 2),
                                          random(hitLocation.y - hitBox.y, hitLocation.y + hitBox.y));
        }
        
        if(isExplosion)
        {
            HitEffectLocation = hitLocation;
        }
        
        hitEffect->setPath("Assets/ui/defense/Hit_effect_boom%02d_%02d.png");
        
        hitEffect->setPosition(HitEffectLocation);
        hitEffect->setNum(hitEffectNumber);
        if(auto poolComp = (PoolableObjectComponent*)hitEffect->getComponent("Poolable"))
        {
            poolComp->ActivateObject();
        }
    }
}

#pragma mark -override
void DFMonster::CollisionBegin(IMafColider *Other)
{
    DFObject::CollisionBegin(Other);
    
    if(_isDead)
        return;

    
    auto bullet = dynamic_cast<DFBullet*>(Other);
    
    if(bullet == nullptr || _battleManager == nullptr)
        return;
    
    //playHitEffect(bullet);
    
    if(random(1, 100) < 30 )
    {
        //deadMonster();
    }
    //monster->deactivateObject();
}
Rect DFMonster::GetBoundingBox()
{
    Rect result = getBoundingBox();
    result.size = _tramcar->getBoundingBox().size;
    result.origin += Vec2::UP * fabs(_tramcar->getContentSize().height - getContentSize().height)/2;
    return result;
}
