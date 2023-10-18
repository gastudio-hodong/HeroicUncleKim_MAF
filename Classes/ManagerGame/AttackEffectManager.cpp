//
//  AttackEffectManager.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/10/20.
//

#include "AttackEffectManager.h"

#include "ManagerGame/CostumeSkinManager.h"

USING_NS_CC;

SkinAttackEffect::SkinAttackEffect(int idx, Animate* anim, std::string path)
:
_idx(idx),
strBasePath(path),
_isActive(false),

_anim(anim),
_sprFront(nullptr),
_nowScene(nullptr)
{
}

void SkinAttackEffect::init()
{
    _sprFront = Sprite::create(strBasePath);
    _sprFront->setVisible(false);
    _sprFront->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _sprFront->getTexture()->setTexParameters(texParams);
    _sprFront->setScale(4);
    _sprFront->retain();
}

SkinAttackEffect::~SkinAttackEffect()
{
    if(_anim != nullptr)
        _anim->release();
    if(_sprFront != nullptr)
        _sprFront->release();
}

int SkinAttackEffect::getIdx()
{
    return _idx;
}

Animate* SkinAttackEffect::getAnim()
{
    return _anim;
}

Sprite* SkinAttackEffect::getSprite()
{
    return _sprFront;
}

Node* SkinAttackEffect::getNowScene()
{
    return _nowScene;
}

std::string SkinAttackEffect::getPath()
{
    return strBasePath;
}

bool SkinAttackEffect::isActive()
{
    return _isActive;
}

void SkinAttackEffect::setActive(bool active)
{
    _isActive = active;
}

void SkinAttackEffect::setSprite(Sprite* base)
{
    _sprFront = base;
}

void SkinAttackEffect::setNowScene(Node* scene)
{
    _nowScene = scene;
    
    if(_sprFront->getParent() != nullptr)
    {
        _sprFront->removeFromParent();
    }
    _nowScene->addChild(_sprFront, E_ZORER::HERO);
    
}

AttackEffectManager::AttackEffectManager():
_isActive(false)
{
    _vecEffect.clear();
}

AttackEffectManager::~AttackEffectManager()
{
    
    for(auto effect : _vecEffect)
        delete effect;
    
    _vecEffect.clear();
}

bool AttackEffectManager::init()
{
    {
        //스킨별 공격 이펙트
        auto listSkin = CostumeSkinManager::getInstance()->getListCostumeSkin();
        for (int i = 0; i < listSkin.size(); ++i)
        {
            auto objSkin = listSkin.at(i);
            
            auto animation = Animation::create();
            animation->setDelayPerUnit(0.05);
            
            std::string strBasePath = "";
            bool hasEffect = false;
            for(int j = 1; j <= 100; ++j)
            {
                std::string strPath = MafUtils::format("Assets/character_hero/skin_effect/skin_%d_attack_effect_%02d.png", objSkin->getIdx(), j);
                std::string fullpath = FileUtils::getInstance()->fullPathForFilename(strPath);
                
                if (fullpath.size() == 0)
                    break;
                
                if ( j == 1 )
                {
                    strBasePath = strPath;
                    hasEffect = true;
                }
                
                animation->addSpriteFrameWithFile(strPath);
            }
            
            if ( hasEffect == true )
            {
                auto vecFrames = animation->getFrames();
                for(auto frame : vecFrames)
                {
                    frame->getSpriteFrame()->getTexture()->setTexParameters(texParams);
                }
                
                auto animate = Animate::create(animation);
                animate->retain();
                
                addEffect(objSkin->getIdx(), animate, strBasePath);
            }
        }
        
    }
    
    return true;
}

std::vector<SkinAttackEffect*>AttackEffectManager::getAllEffect()
{
    return _vecEffect;
}

SkinAttackEffect* AttackEffectManager::getAttackEffectForIdx(int idx)
{
    SkinAttackEffect* result = nullptr;
    
    for(auto effect : _vecEffect)
    {
        auto objSkin = CostumeSkinManager::getInstance()->getInfoSkin(idx);
        if ( objSkin == nullptr )
            break;
        
        if ( effect->getIdx() == idx )
            result = effect;
    }
    
    return result;
}

void AttackEffectManager::addEffect(int idx, Animate* anim, std::string path)
{
    auto effect = new SkinAttackEffect(idx, anim, path);
    effect->init();
    _vecEffect.push_back(effect);
}

void AttackEffectManager::addEffect(SkinAttackEffect* effect)
{
    
    _vecEffect.push_back(effect);
}

void AttackEffectManager::playSkinEffect(int idx, Vec2 pos)
{
    if(_isActive == false)
        return;
    
    SkinAttackEffect* effect = getAttackEffectForIdx(idx);
    
    if(effect == nullptr)
        return;
    
    auto scene = UtilsScene::getInstance()->getRunningLayer();
    
    if(effect->getNowScene() != scene)
    {
        effect->setNowScene(scene);
        return;
    }
    
    Sprite* sprBase = effect->getSprite();
    if(sprBase == nullptr)
    {
        sprBase = Sprite::create(effect->getPath());
        sprBase->setVisible(false);
        sprBase->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        sprBase->getTexture()->setTexParameters(texParams);
        sprBase->setScale(4);
        sprBase->retain();
        effect->setSprite(sprBase);
        scene->addChild(sprBase, E_ZORER::HERO);
        effect->setNowScene(scene);
        effect->setActive(false);
        
        return;
    }
    
    if(effect->isActive())
        return;
    
    sprBase->setPosition(pos);
    
    auto funcStart = CallFunc::create([=](){
        
        effect->setActive(true);
    });
    
    auto funcEnd = CallFunc::create([=](){
        
        effect->setActive(false);
    });
    
    auto fSeq = Sequence::create(funcStart, Show::create(), effect->getAnim()->clone(), Hide::create(), funcEnd, nullptr);
    
    
    sprBase->runAction(fSeq);
    
}

bool AttackEffectManager::hasSkinEffect(int idx)
{
    bool result = false;
    
    for(auto effect : _vecEffect)
    {
        auto objSkin = CostumeSkinManager::getInstance()->getInfoSkin(idx);
        if ( objSkin == nullptr )
            break;
        
        if ( effect->getIdx() == idx )
            result = true;
    }
    
    return result;
}

void AttackEffectManager::stopAllEffect()
{
    _isActive = false;
    
    for(auto effect : _vecEffect)
    {
        
        auto base = effect->getSprite();
        if(base != nullptr)
        {
            if(base->getNumberOfRunningActions() > 0)
                base->stopAllActions();
            base->setVisible(false);
            
            if(base->getParent() != nullptr)
                base->removeFromParent();
        }
        effect->setActive(false);
        
    }
}

void AttackEffectManager::reactivationEffect()
{
    _isActive = true;
}
