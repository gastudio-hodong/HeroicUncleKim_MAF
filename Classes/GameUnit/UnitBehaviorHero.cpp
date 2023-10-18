//
//  Model.cpp
//  HexagonSnake
//
//  Created by SanHeo on 2014. 7. 27..
//
//

#include "UnitBehaviorHero.h"

#include "GameUnit/UnitBehaviorHeroAnother.h"
#include "GameUnit/UnitBehaviorMonster.h"
#include "GameUnit/Shadow/UnitHeroShadow.h"

#include "ManagerGame/UserInfoManager.h"
#include "ManagerGame/UserInfoStackManager.h"
#include "ManagerGame/UserInfoConfigManager.h"
#include "ManagerGame/AttackEffectManager.h"
#include "ManagerGame/PetNewManager.h"
#include "ManagerGame/SaveManager.h"
#include "ManagerGame/DataManager.h"
#include "ManagerGame/CostumeManager.h"
#include "ManagerGame/CostumeSkinManager.h"
#include "ManagerGame/InfiniteArtifactManager.h"
#include "ManagerGame/WeaponSpiritManager.h"
#include "ManagerGame/PrisonFrostManager.h"
#include "ManagerGame/WeaponManager.h"

USING_NS_CC;

UnitBehaviorHero* UnitBehaviorHero::create(E_PLACE ePlace)
{
    UnitBehaviorHero *pRet = new (std::nothrow) UnitBehaviorHero();
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

UnitBehaviorHero::UnitBehaviorHero() :
_eState(E_HERO_STATE::MOVE),
_eSkin(E_HERO_SKIN::DEFAULT),
_nTalkCount(0),
_bChangeParts(false),
_bOnekill(false),
_bShowSkinAttackEffect(true),
_bShowWeaponAttackEffect(true),
_bDeathBlow(false),
_isDeathBlow(false),
_isDeathBlowActive(false),
_nDeathBlowTime(0),
_isFrostSkillCri(false),
_nTimePrincessBuff(0),
_nTimeGatekeeperBuff(0),
_bSkinUse(true),

_enemy(nullptr),
_shadow(nullptr),
_spriteSpiritBuffFx(nullptr),
_fxSpiritBuff(nullptr),


_onAttackCallback(nullptr),
_onMoveCallback(nullptr),
_onUpstairsCallback(nullptr),
_onAttackCountCallback(nullptr),

_spriteTalk(nullptr),
_spriteRing(nullptr),
_spriteItemFeverFx(nullptr),

_animateBuffAttack(nullptr),
_animateBuffSpeed(nullptr),
_animateBuffCritical(nullptr),
_animateBuffAttackSpeed(nullptr),
_animateBuffAll(nullptr),
_animateBuffBack(nullptr),
_spriteBuff(nullptr),
_spriteBuffBack(nullptr),

_spritePartSkin(nullptr),
_spritePartSkinMoveEffect(nullptr),
_spritePartSkinAttackEffect(nullptr),
_spriteFrostSkillShape(nullptr)
{

}


UnitBehaviorHero::~UnitBehaviorHero()
{
    if ( _onAttackCallback != nullptr )     _onAttackCallback->release();
    if ( _onMoveCallback != nullptr )       _onMoveCallback->release();
    if ( _onUpstairsCallback != nullptr )   _onUpstairsCallback->release();
    if ( _onAttackCountCallback != nullptr )   _onAttackCountCallback->release();
    
    _onAttackCallback = nullptr;
    _onMoveCallback = nullptr;
    _onUpstairsCallback = nullptr;
    _onAttackCountCallback = nullptr;
    
    //
    _vecSequence.clear();
    if ( _animateBuffAttack != nullptr )        _animateBuffAttack->release();
    if ( _animateBuffSpeed != nullptr )         _animateBuffSpeed->release();
    if ( _animateBuffCritical != nullptr )      _animateBuffCritical->release();
    if ( _animateBuffAttackSpeed != nullptr )   _animateBuffAttackSpeed->release();
    if ( _animateBuffAll != nullptr )           _animateBuffAll->release();
    if ( _animateBuffBack != nullptr )          _animateBuffBack->release();
    
    
    for(int i = 0; i < _vecAtkEffectAnimInfo.size(); ++i)
    {
        if(_vecAtkEffectAnimInfo[i].fAnim != nullptr) _vecAtkEffectAnimInfo[i].fAnim->release();
        if(_vecAtkEffectAnimInfo[i].bAnim != nullptr) _vecAtkEffectAnimInfo[i].bAnim->release();
    }
    
}

bool UnitBehaviorHero::init()
{
    if ( !UnitBehavior::init() )
    {
        return false;
    }
    
    //
    _bShowSkinAttackEffect = UserInfoConfigManager::getInstance()->isFunction(E_FUNCTION_ACTIVE::ATTACK_SKIN);
    _bShowWeaponAttackEffect = UserInfoConfigManager::getInstance()->isFunction(E_FUNCTION_ACTIVE::ATTACK_WEAPON);
    
    // 코스튬 효과 적용
    int specialReborn = SaveManager::getHistoryDataInt(E_HISTORY_DATA::K_REVIVAL_2) + SaveManager::getHistoryDataInt(E_HISTORY_DATA::K_REVIVAL_3);
    UserInfoStackManager::getInstance()->setSetItemEffectIdx25Count(specialReborn);
    
    //
    if ( _ePlace == E_PLACE::DG_DEVIL_DOM )
    {//마왕성, 마왕성 스텟
        _chaScale = DevilScale;
        initPartDevil();
    }
    else
    {
        _chaScale = NormalScale;
        initPart();
        initShadow();
    }
    
    // 대화 박스
    
    setTalkShow(UserInfoConfigManager::getInstance()->isFunction(E_FUNCTION_ACTIVE::SPEECH));
    setTalkAuto();
    
    //
    if(InfiniteArtifactManager::getInstance()->isActive() && _ePlace != E_PLACE::DG_DEVIL_DOM)
    {
        scheduleUpdate();
        _isDeathBlowActive = true;
    }
    return true;
}

void UnitBehaviorHero::update(float dt)
{
    _nDeathBlowTime += dt;
    
    if(_nDeathBlowTime >= getDeathBlowCoolDown() && _bDeathBlow == false)
    {
        _bDeathBlow = true;
    }
}

void UnitBehaviorHero::initPart()
{
    //
    _layerCharacter = LayerColor::create();
    _layerCharacter->setContentSize(Size(232, 232));
    _layerCharacter->setIgnoreAnchorPointForPosition(false);
    _layerCharacter->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    _layerCharacter->setPosition(Vec2::ZERO);
    addChild(_layerCharacter);
    
    _lySpiritBuff = Layer::create();
    _lySpiritBuff->setContentSize(Size(232, 232));
    _lySpiritBuff->setIgnoreAnchorPointForPosition(false);
    _lySpiritBuff->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    _lySpiritBuff->setPosition(Vec2::ZERO);
    addChild(_lySpiritBuff);
     
    
    // SKIN : DEFAULT, DEFAULT_EVENT
    {
        E_HERO listPart[] = {E_HERO::BODY, E_HERO::HEAD, E_HERO::HAND, E_HERO::FOOT, E_HERO::CAPE, E_HERO::WEAPON};
        for ( int i = 0; i < sizeof(listPart) / sizeof(E_HERO); i++ )
        {
            _spritePartDefault[listPart[i]] = Sprite::create();
            _spritePartDefault[listPart[i]]->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
            _spritePartDefault[listPart[i]]->setPosition(_layerCharacter->getContentSize().width, 0);
            _spritePartDefault[listPart[i]]->getTexture()->setTexParameters(texParams);
            _layerCharacter->addChild(_spritePartDefault[listPart[i]], listPart[i]);
            
            if ( listPart[i] != E_HERO::BODY )
            {
                _spritePartDefault[listPart[i]]->setScale(_chaScale);
            }
        }
        
        // 배경 그림자
        _spritePartDefault[E_HERO::SHADOW2] = Sprite::create("Assets/character_hero/normal/shadow2.png");
        _spritePartDefault[E_HERO::SHADOW2]->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        _spritePartDefault[E_HERO::SHADOW2]->setPosition(_layerCharacter->getContentSize().width / 4, 0);
        _layerCharacter->addChild(_spritePartDefault[E_HERO::SHADOW2], E_HERO::SHADOW2);
        
        // 바닥 그림자
        _spritePartDefault[E_HERO::SHADOW1] = Sprite::create("Assets/character_hero/normal/shadow.png");
        _spritePartDefault[E_HERO::SHADOW1]->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        _spritePartDefault[E_HERO::SHADOW1]->setPosition(_layerCharacter->getContentSize().width / 2, 0);
        _layerCharacter->addChild(_spritePartDefault[E_HERO::SHADOW1], E_HERO::SHADOW1);
        
        // 아우라
        auto ani = Animation::create();
        ani->setDelayPerUnit(0.1f);
        ani->addSpriteFrameWithFile("Assets/character_hero/normal/effect/effect_aura_1.png");
        ani->addSpriteFrameWithFile("Assets/character_hero/normal/effect/effect_aura_2.png");
        ani->addSpriteFrameWithFile("Assets/character_hero/normal/effect/effect_aura_3.png");
        ani->addSpriteFrameWithFile("Assets/character_hero/normal/effect/effect_aura_4.png");
        
        _spritePartDefault[E_HERO::AURA] = Sprite::create("Assets/character_hero/normal/effect/effect_aura_1.png");
        _spritePartDefault[E_HERO::AURA]->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        _spritePartDefault[E_HERO::AURA]->setPosition(_layerCharacter->getContentSize().width / 2 - 60, 0);
        _spritePartDefault[E_HERO::AURA]->runAction(RepeatForever::create(Animate::create(ani)));
        _spritePartDefault[E_HERO::AURA]->setVisible(false);
        _layerCharacter->addChild(_spritePartDefault[E_HERO::AURA], E_HERO::AURA);
        {// 아우라 미니
            auto spriteAura = Sprite::create("Assets/character_hero/normal/effect/effect_aura_1.png");
            spriteAura->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            spriteAura->setPosition(_spritePartDefault[E_HERO::AURA]->getContentSize().width / 2, 10);
            spriteAura->setScale(0.8);
            spriteAura->setOpacity(200);
            spriteAura->runAction(RepeatForever::create(Animate::create(ani->clone())));
            _spritePartDefault[E_HERO::AURA]->addChild(spriteAura);
        }
        
        // 머리 링
        _spriteRing = Sprite::create("Assets/character_hero/normal/ring_bong.png");
        _spriteRing->setPosition(55, 165);
        _spriteRing->setVisible(false);
        _layerCharacter->addChild(_spriteRing);
    }
    
    // SKIN : PREMIUM, HERO
    {
        _spritePartSkin = Sprite::create();
        _spritePartSkin->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        _spritePartSkin->setPosition(_layerCharacter->getContentSize().width, 0);
        _layerCharacter->addChild(_spritePartSkin);
        
        _spritePartSkinMoveEffect = Sprite::create();
        _spritePartSkinMoveEffect->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        _spritePartSkinMoveEffect->setPosition(_layerCharacter->getContentSize().width / 2, -60);
        _layerCharacter->addChild(_spritePartSkinMoveEffect);
        
        _spritePartSkinAttackEffect = Sprite::create();
        _spritePartSkinAttackEffect->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        _spritePartSkinAttackEffect->setPosition(_layerCharacter->getContentSize().width / 2, -60);
        _layerCharacter->addChild(_spritePartSkinAttackEffect);
    }
    
    //타격 이펙트
    {
        _vecAtkEffectInfo.clear();
        
        for(int i = 1; i <= 3; ++i)
        {
            auto frontAnim = Animation::create();
            frontAnim->setDelayPerUnit(0.05);
            auto backAnim = Animation::create();
            backAnim->setDelayPerUnit(0.05);
            Sprite* fSpr;
            Sprite* bSpr;
            for(int j = 1; j <= 4; ++j)
            {
                std::string sprPath = "Assets/character_hero/normal/effect_hit/ef_hit_%d_%d_%d.png";
                std::string sprFrontPath = MafUtils::format(sprPath.c_str(), i, 1, j);
                std::string sprBackPath = MafUtils::format(sprPath.c_str(), i, 2, j);
                
                if(j == 1)
                {
                    fSpr = Sprite::create(sprFrontPath);
                    bSpr = Sprite::create(sprBackPath);
                    
                    fSpr->setVisible(false);
                    fSpr->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                    
                    bSpr->setVisible(false);
                    bSpr->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                    if( i == 1 || i == 3)
                    {
                        fSpr->setScale(0.7f);
                        bSpr->setScale(0.7f);
                    }
                    
                    _layerCharacter->addChild(fSpr, E_ZORER::HERO);
                    _layerCharacter->addChild(bSpr, E_ZORER::HERO);
                }
                
                frontAnim->addSpriteFrameWithFile(sprFrontPath);
                backAnim->addSpriteFrameWithFile(sprBackPath);

            }
            auto fAnim = Animate::create(frontAnim);
            fAnim->retain();
            
            auto bAnim = Animate::create(backAnim);
            bAnim->retain();
            
            _vecAtkEffectAnimInfo.push_back(tagAtkEffectAnimtInfo(fAnim, bAnim, fSpr, bSpr));
        }
            
            
        _vecAtkEffectInfo.push_back(tagAtkEffectDefaultInfo(Color3B(255,120,0)));
        _vecAtkEffectInfo.push_back(tagAtkEffectDefaultInfo(Color3B(255,214,48)));
        _vecAtkEffectInfo.push_back(tagAtkEffectDefaultInfo(Color3B(253,48,255)));
        _vecAtkEffectInfo.push_back(tagAtkEffectDefaultInfo(Color3B(0,255,36)));
        _vecAtkEffectInfo.push_back(tagAtkEffectDefaultInfo(Color3B(0,204,255)));
        
        
    }

    //
    {
        auto animation = Animation::create();
        animation->setDelayPerUnit(0.1);
        for ( int i = 1; i <= 6; i++ )
        {
            auto spriteAni = Sprite::create(MafUtils::format("Assets/ui/frost_prison/skill/skill_buff_effect%02d.png",i).c_str());
            spriteAni->getTexture()->setTexParameters(texParams);
            animation->addSpriteFrame(spriteAni->getSpriteFrame());
        }
        
        auto action1 = DelayTime::create(0.5);
        auto action2 = CallFuncN::create([=](Node* sender){
            
            auto effect = PrisonFrostManager::getInstance()->getSkillBuffEffect(50004, PrisonFrostManager::getInstance()->getSkillLevel(50004));
            if ( effect > 0 )
            {
                sender->setVisible(true);
            }
            else
            {
                sender->setVisible(false);
            }
        });
        auto seq = Sequence::create(action1, action2, NULL);
        
        _spriteFrostSkillShape = Sprite::create("Assets/ui/frost_prison/skill/skill_buff_effect01.png");
        _spriteFrostSkillShape->setPosition(Vec2(58, 48));
        _spriteFrostSkillShape->getTexture()->setTexParameters(texParams);
        _spriteFrostSkillShape->runAction(RepeatForever::create(Animate::create(animation)));
        _spriteFrostSkillShape->runAction(RepeatForever::create(seq));
        _spriteFrostSkillShape->setVisible(false);
        _layerCharacter->addChild(_spriteFrostSkillShape, -10);
        
    }
}

void UnitBehaviorHero::initPartDevil()
{
    //
    _layerCharacter = Layer::create();
    _layerCharacter->setContentSize(Size(180, 180));
    _layerCharacter->setIgnoreAnchorPointForPosition(false);
    _layerCharacter->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    _layerCharacter->setPosition(Vec2::ZERO);
    addChild(_layerCharacter);
    
    // SKIN : DEFAULT, DEFAULT_EVENT
    {
        /*마왕성 오더 순서 변경 필요 무기-몸-망토-머리*/
        E_HERO listPart[] = {E_HERO::RIDER, E_HERO::HEAD, E_HERO::CAPE, E_HERO::BODY, E_HERO::WEAPON};
        for ( int i = 0; i < sizeof(listPart) / sizeof(E_HERO); i++ )
        {
            _spritePartDefault[listPart[i]] = Sprite::create();
            _spritePartDefault[listPart[i]]->setAnchorPoint(Vec2::ZERO);
            _spritePartDefault[listPart[i]]->setPosition(Vec2::ZERO);
            _spritePartDefault[listPart[i]]->getTexture()->setTexParameters(texParams);
            _spritePartDefault[listPart[i]]->setScale(_chaScale);
            _layerCharacter->addChild(_spritePartDefault[listPart[i]], i);
            
            if ( listPart[i] == E_HERO::RIDER )
            {
                _spritePartDefault[listPart[i]]->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
                _spritePartDefault[listPart[i]]->setPosition(_layerCharacter->getContentSize().width, 0);
            }
        }
        
        // 바닥 그림자
        _spritePartDefault[E_HERO::SHADOW1] = Sprite::create("Assets/character_hero/devil/hero_shadow.png");
        _spritePartDefault[E_HERO::SHADOW1]->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        _spritePartDefault[E_HERO::SHADOW1]->setPosition(_layerCharacter->getContentSize().width / 2, 0);
        _spritePartDefault[E_HERO::SHADOW1]->setScale(_chaScale);
        _layerCharacter->addChild(_spritePartDefault[E_HERO::SHADOW1], E_HERO::SHADOW1);
    }
}

void UnitBehaviorHero::initShadow()
{
    _shadow = UnitHeroShadow::create();
    _shadow->setPosition(_shadow->getContentSize().width * 0.5, 0);
    addChild(_shadow);
}

#pragma mark - Hero : schedule
void UnitBehaviorHero::scheduleTalk(float dt)
{
    std::string strTalk = "";
    std::string strBGPath = "Assets/ui/textbox.png";
    
    _nTalkCount = ++_nTalkCount % 4;
    if ( _nTalkCount == 0 )
    {
        strTalk = GAME_TALKTIP();
        strBGPath = "Assets/ui/textbox_tip.png";
    }
    
    if ( strTalk.length() == 0 )
    {
        strTalk = GAME_TALK();
        strBGPath = "Assets/ui/textbox.png";
    }
    
    if ( _spriteTalk != nullptr )
    {
        _spriteTalk->setTexture(strBGPath);
        
        auto label = (Label*)_spriteTalk->getChildByTag(10);
        label->setString(strTalk);
    }
}

#pragma mark - Hero : set, get
void UnitBehaviorHero::setAttackCallBackListener(CallFunc *call)
{
    if ( _onAttackCallback != nullptr )
        _onAttackCallback->release();
    
    _onAttackCallback = call;
    _onAttackCallback->retain();
}

void UnitBehaviorHero::setMoveCallBackListener(CallFunc *call)
{
    if ( _onMoveCallback != nullptr )
        _onMoveCallback->release();
    
    _onMoveCallback = call;
    _onMoveCallback->retain();
}

void UnitBehaviorHero::setUpstairsCallBackListener(CallFunc *call)
{
    if ( _onUpstairsCallback != nullptr )
        _onUpstairsCallback->release();
    
    _onUpstairsCallback = call;
    _onUpstairsCallback->retain();
}

void UnitBehaviorHero::setAttackCountCallBackListener(CallFunc *call)
{
    if ( _onAttackCountCallback != nullptr )
        _onAttackCountCallback->release();
    
    _onAttackCountCallback = call;
    _onAttackCountCallback->retain();
}

void UnitBehaviorHero::setShowSkinAttackEffect(bool active)
{
    _bShowSkinAttackEffect = active;
}

void UnitBehaviorHero::setShowWeaponAttackEffect(bool active)
{
    _bShowWeaponAttackEffect = active;
}

void UnitBehaviorHero::setTalk(std::string talk)
{
    auto sch = Director::getInstance()->getScheduler();
    if(sch->isScheduled(schedule_selector(UnitBehaviorHero::scheduleTalk), this))
        sch->unschedule(schedule_selector(UnitBehaviorHero::scheduleTalk), this);
    
    if ( _spriteTalk != nullptr )
    {
        auto label = (Label*)_spriteTalk->getChildByTag(10);
        label->setString(talk);
    }
}

void UnitBehaviorHero::setTalkAuto()
{
    auto sch = Director::getInstance()->getScheduler();
    if ( !sch->isScheduled(schedule_selector(UnitBehaviorHero::scheduleTalk), this) )
    {
        sch->schedule(schedule_selector(UnitBehaviorHero::scheduleTalk), this, 5 , kRepeatForever, 0.0f, false);
    }
}

void UnitBehaviorHero::setTalkShow(bool value)
{
    if ( _spriteTalk == nullptr )
    {
        _spriteTalk = Sprite::create("Assets/ui/textbox.png");
        _spriteTalk->setAnchorPoint(Vec2(1,0));
        _spriteTalk->setPosition(10, _layerCharacter->getContentSize().height - 80);
        _layerCharacter->addChild(_spriteTalk);
        if ( _ePlace == E_PLACE::DG_DEVIL_DOM )
        {
            _spriteTalk->setPosition(10, _layerCharacter->getContentSize().height-30);
        }
        else if ( _ePlace == E_PLACE::DG_NORMAL )
        {
            _spriteTalk->setPosition(10, _layerCharacter->getContentSize().height - 30);
        }


        auto labelTalk = Label::createWithTTF(GAME_TALK(), GAME_FONT, 24);
        labelTalk->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
        labelTalk->setPosition(_spriteTalk->getContentSize().width / 2, _spriteTalk->getContentSize().height );
        labelTalk->setDimensions(_spriteTalk->getContentSize().width - 15, _spriteTalk->getContentSize().height - 15);
        labelTalk->setOverflow(Label::Overflow::SHRINK);
        labelTalk->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
        labelTalk->setColor(COLOR_TALK);
        labelTalk->setTag(10);
        _spriteTalk->addChild(labelTalk);
    }

    _spriteTalk->setVisible(value);
}

void UnitBehaviorHero::setSpiritBuff(bool value)
{
    auto objBuff = WeaponSpiritManager::getInstance()->getInfoWeaponSpiritBuff();
    
    if(value == false)
    {
        _lySpiritBuff->setVisible(false);
    }
    
    if(objBuff == nullptr)
        return;
    
    if ( _fxSpiritBuff == nullptr )
    {
        _fxSpiritBuff = ParticleSystemQuad::create("Assets/Particle/particle_spirit_buff_2.plist");
        _fxSpiritBuff->setScale(0.5f);
        _fxSpiritBuff->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        _fxSpiritBuff->setPosition(55, _lySpiritBuff->getContentSize().height + 20);
        _fxSpiritBuff->setStartColor(Color4F(objBuff->getEffectColor()));
        _fxSpiritBuff->setVisible(false);
        _lySpiritBuff->addChild(_fxSpiritBuff);
        
    }
    if(_spriteSpiritBuffFx == nullptr)
    {
        _spriteSpiritBuffFx = Sprite::create(MafUtils::format("Assets/ui/spirit/buf_spirit_%04d.png", objBuff->getIdx()));
        _spriteSpiritBuffFx->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        _spriteSpiritBuffFx->setScale(0.9f);
        _spriteSpiritBuffFx->setPosition(55, _lySpiritBuff->getContentSize().height - 40);
        _spriteSpiritBuffFx->setVisible(false);
        _lySpiritBuff->addChild(_spriteSpiritBuffFx);
        {
            auto action1 = MoveBy::create(1.f,Vec2(0, 5));
            auto action2 = action1->reverse();
            auto seq = Sequence::create(action1,action2, NULL);
            _spriteSpiritBuffFx->runAction(RepeatForever::create(seq));
        }
    }
    
    _spriteSpiritBuffFx->setTexture(MafUtils::format("Assets/ui/spirit/buf_spirit_%04d.png", objBuff->getIdx()));
    _fxSpiritBuff->setStartColor(Color4F(objBuff->getEffectColor()));
    
    if(value == true)
    {
        _lySpiritBuff->setVisible(true);
        
        if(_fxSpiritBuff->isVisible() == false)
        {
            auto sprFx = Sprite::create("Assets/ui/spirit/buf_spirit_on_1_1.png");
            sprFx->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            sprFx->setPosition(_spriteSpiritBuffFx->getPosition() + Vec2::UP * _spriteSpiritBuffFx->getContentSize().height/2);
            sprFx->setColor(Color3B(objBuff->getEffectColor()));
            _lySpiritBuff->addChild(sprFx);
            
            auto anim = Animation::create();
            anim->setDelayPerUnit(0.05f);
            
            for(int i = 0; i < 4; ++i)
            {
                std::string sprPath = MafUtils::format("Assets/ui/spirit/buf_spirit_on_1_%d.png", i+1);
                anim->addSpriteFrameWithFile(sprPath);
            }
            auto animate = Animate::create(anim);
            
            auto anim2 = Animation::create();
            anim2->setDelayPerUnit(0.05f);
            for(int i = 4; i < 8; ++i)
            {
                std::string sprPath = MafUtils::format("Assets/ui/spirit/buf_spirit_on_1_%d.png", i+1);
                anim2->addSpriteFrameWithFile(sprPath);
            }
            auto animate2 = Animate::create(anim2);
            
            auto sequence = Sequence::create(animate, CallFunc::create([=](){
                    _fxSpiritBuff->setVisible(true);
                    _spriteSpiritBuffFx->setVisible(true);
            }), animate2, RemoveSelf::create(), nullptr);
            sprFx->runAction(sequence);
        }
    }
    else
    {
        _fxSpiritBuff->setVisible(value);
        _spriteSpiritBuffFx->setVisible(value);
    }
}

void UnitBehaviorHero::setShowRing()
{
    if ( _spriteRing != nullptr )
    {
        _spriteRing->setVisible(true);
    }
}

void UnitBehaviorHero::setHideRing()
{
    if ( _spriteRing != nullptr)
    {
        _spriteRing->setVisible(false);
    }
}

void UnitBehaviorHero::setSkinUse(bool bSkinUse)
{
    _bSkinUse = bSkinUse;
}

std::string UnitBehaviorHero::getAttackDeathBlowDamage(std::string &tmpAtk)
{
    E_PLACE ePlace = E_PLACE::DG_NORMAL;
    auto dmg = tmpAtk;
     
    auto per = DataManager::GetArtifactEffect(E_ARTIFACT::ARTI_103_BERSERKER_SPRIT, UserInfoManager::getInstance()->getArtifactLevel(ePlace, E_ARTIFACT::ARTI_103_BERSERKER_SPRIT), UserInfoManager::getInstance()->getArtifactReinforce(ePlace, ARTI_103_BERSERKER_SPRIT));
    auto criPer = random(0.f, 100.f);
    if ( criPer > per )
        return dmg;

    per = DataManager::GetArtifactEffect(E_ARTIFACT::ARTI_104_BERSERKER_AMULET, UserInfoManager::getInstance()->getArtifactLevel(ePlace, E_ARTIFACT::ARTI_104_BERSERKER_AMULET), UserInfoManager::getInstance()->getArtifactReinforce(ePlace, ARTI_104_BERSERKER_AMULET));
    if ( per > 0 )
    {
        std::string temp = MafUtils::bigMulNum(dmg, MafUtils::doubleToString(per));
        temp.pop_back();
        temp.pop_back();
        dmg = MafUtils::bigAddNum(dmg,temp);
    }

    _bDeathBlow = false;
    _isDeathBlow = true;
    _nDeathBlowTime = 0;

    return dmg;
 }

float UnitBehaviorHero::getDeathBlowCoolDown()
{
    E_PLACE ePlace = E_PLACE::DG_NORMAL;
    
    float coolDown = 5000;
    coolDown -= (UserInfoManager::getInstance()->getArtifactLevel(ePlace, E_ARTIFACT::ARTI_103_BERSERKER_SPRIT) -1) * 115;
    if(coolDown < 1000)
    {
        coolDown = 0;
        
        return coolDown;
    }
    
    //ms -> s 변환
    coolDown /= 1000;
    return coolDown;
}

bool UnitBehaviorHero::isDeathBlowActive()
{
    return _isDeathBlowActive;
}

void UnitBehaviorHero::actionDeathBlowActive()
{
    if(!_isDeathBlowActive)
    {
        scheduleUpdate();
        _isDeathBlowActive = true;
    }
}

std::string UnitBehaviorHero::getAttackSkillFrostDamage(std::string &tmpAtk)
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

#pragma mark - Hero : character 모양 및 위치
void UnitBehaviorHero::setChangeParts()
{
    _bChangeParts = true;
}

void UnitBehaviorHero::setMotion(int nSkinDummy, int nWeaponDummy)
{
    
    //
    _vecSequence.clear();
    
    
    //
    double attckPer = UserInfoManager::getInstance()->getAttackSpeed(_ePlace);
    attckPer *= 0.25;
    
    int weaponEquip = WeaponManager::getInstance()->getEquipVisual();
    if ( _ePlace == E_PLACE::DG_PRISON )
    {
        weaponEquip = E_WEAPON::WEAPON_1;
    }
    if ( nWeaponDummy != 0 )
    {
        weaponEquip = nWeaponDummy;
    }
    
    //
    _eSkin = E_HERO_SKIN::DEFAULT;
    if ( _bSkinUse == true )
    {
        if ( _ePlace == E_PLACE::DG_NORMAL && UserInfoManager::getInstance()->getBuffTime(E_BUFF_TYPE::BUFF_HERO_BONUS) > 0 && UserInfoManager::getInstance()->getBuffVisible(E_BUFF_TYPE::BUFF_HERO_BONUS) == true )
        {
            _eSkin = E_HERO_SKIN::HERO;
        }
        else if ( CostumeSkinManager::getInstance()->getCostumeSkinEquip() != 0 )
        {
            _eSkin = E_HERO_SKIN::PREMIUM;
        }
        else if ( nSkinDummy != 0 )
        {
            _eSkin = E_HERO_SKIN::PREMIUM;
        }
    }
    
    
    
    //
    _spritePartDefault[E_HERO::SHADOW1]->setVisible(false);
    _spritePartDefault[E_HERO::AURA]->setVisible(false);
    _spritePartDefault[E_HERO::BODY]->setVisible(false);
    _spritePartDefault[E_HERO::HEAD]->setVisible(false);
    _spritePartDefault[E_HERO::HAND]->setVisible(false);
    _spritePartDefault[E_HERO::FOOT]->setVisible(false);
    _spritePartDefault[E_HERO::CAPE]->setVisible(false);
    _spritePartDefault[E_HERO::WEAPON]->setVisible(false);
    _spritePartSkin->setVisible(false);
    _spritePartSkinMoveEffect->setVisible(false);
    _spritePartSkinAttackEffect->setVisible(false);
    
    //
    if ( _eSkin == E_HERO_SKIN::DEFAULT || _eSkin == E_HERO_SKIN::DEFAULT_SUMMER )
    {// SKIN : DEFAULT, DEFAULT_EVENT
        std::string strPathAttack = "";
        std::string strPathWalk = "";
        
        //attack weapon
        {
            strPathAttack = MafUtils::format("Assets/character_hero/normal/weapon/weapon_%.2d_attack_%s.png", weaponEquip, "%d");
            strPathWalk = MafUtils::format("Assets/character_hero/normal/weapon/weapon_%.2d_walk_%s.png", weaponEquip, "%d");
            if ( _eSkin == E_HERO_SKIN::DEFAULT_SUMMER )
            {
                strPathAttack = "Assets/character_hero/event/weapon/19summer_weapon_01_attack_%d.png";
                strPathWalk = "Assets/character_hero/event/weapon/19summer_weapon_01_walk_%d.png";
            }
            _animatePartDefaultAttack[E_HERO::WEAPON] = getMotionDefault(strPathAttack, attckPer);
            _vecSequence.pushBack(_animatePartDefaultAttack[E_HERO::WEAPON]);
            
            _animatePartDefaultMove[E_HERO::WEAPON] = getMotionDefault(strPathWalk, 0.125f);
            _vecSequence.pushBack(_animatePartDefaultMove[E_HERO::WEAPON]);
        }
        
        //attack body
        {
            int nIdx = (int)CostumeManager::getInstance()->getEquip(E_COSTUME::BODY);// _uCostumeCurrentEquip[E_COSTUME::BODY];

            strPathAttack = MafUtils::format("Assets/character_hero/normal/body/body_%.2d_attack_%s.png", nIdx, "%d");
            strPathWalk = MafUtils::format("Assets/character_hero/normal/body/body_%.2d_walk_%s.png", nIdx , "%d");
            if ( _eSkin == E_HERO_SKIN::DEFAULT_SUMMER )
            {
                strPathAttack = "Assets/character_hero/event/body/19summer_body_01_attack_%d.png";
                strPathWalk = "Assets/character_hero/event/body/19summer_body_01_walk_%d.png";
            }
            _animatePartDefaultAttack[E_HERO::BODY] = getMotionDefault(strPathAttack, attckPer);
            _vecSequence.pushBack(_animatePartDefaultAttack[E_HERO::BODY]);
            
            _animatePartDefaultMove[E_HERO::BODY] = getMotionDefault(strPathWalk, 0.125f);
            _vecSequence.pushBack(_animatePartDefaultMove[E_HERO::BODY]);
        }
        
        //attack head
        {
            int nIdx = (int)CostumeManager::getInstance()->getEquip(E_COSTUME::HEAD);

            strPathAttack = MafUtils::format("Assets/character_hero/normal/head/head_%.2d_attack_%s.png", nIdx, "%d");
            strPathWalk = MafUtils::format("Assets/character_hero/normal/head/head_%.2d_walk_%s.png", nIdx, "%d");
            if ( _eSkin == E_HERO_SKIN::DEFAULT_SUMMER )
            {
                strPathAttack = "Assets/character_hero/event/head/19summer_head_01_attack_%d.png";
                strPathWalk = "Assets/character_hero/event/head/19summer_head_01_walk_%d.png";
            }
            _animatePartDefaultAttack[E_HERO::HEAD] = getMotionDefault(strPathAttack, attckPer);
            _vecSequence.pushBack(_animatePartDefaultAttack[E_HERO::HEAD]);
            
            _animatePartDefaultMove[E_HERO::HEAD] = getMotionDefault(strPathWalk, 0.125f);
            _vecSequence.pushBack(_animatePartDefaultMove[E_HERO::HEAD]);
        }
        
        //attack hand
        {
            int nIdx = (int)CostumeManager::getInstance()->getEquip(E_COSTUME::HAND);

            strPathAttack = MafUtils::format("Assets/character_hero/normal/hand/hand_%.2d_attack_%s.png", nIdx, "%d");
            strPathWalk = MafUtils::format("Assets/character_hero/normal/hand/hand_%.2d_walk_%s.png", nIdx, "%d");
            if ( _eSkin == E_HERO_SKIN::DEFAULT_SUMMER )
            {
                strPathAttack = "Assets/character_hero/event/hand/19summer_hand_01_attack_%d.png";
                strPathWalk = "Assets/character_hero/event/hand/19summer_hand_01_walk_%d.png";
            }
            _animatePartDefaultAttack[E_HERO::HAND] = getMotionDefault(strPathAttack, attckPer);
            _vecSequence.pushBack(_animatePartDefaultAttack[E_HERO::HAND]);
            
            _animatePartDefaultMove[E_HERO::HAND] = getMotionDefault(strPathWalk, 0.125f);
            _vecSequence.pushBack(_animatePartDefaultMove[E_HERO::HAND]);
        }
       
        //attack foot
        {
            int nIdx = (int)CostumeManager::getInstance()->getEquip(E_COSTUME::FOOT);

            strPathAttack = MafUtils::format("Assets/character_hero/normal/foot/foot_%.2d_attack_%s.png", nIdx, "%d");
            strPathWalk = MafUtils::format("Assets/character_hero/normal/foot/foot_%.2d_walk_%s.png", nIdx, "%d");
            if ( _eSkin == E_HERO_SKIN::DEFAULT_SUMMER )
            {
                strPathAttack = "Assets/character_hero/event/foot/19summer_foot_01_attack_%d.png";
                strPathWalk = "Assets/character_hero/event/foot/19summer_foot_01_walk_%d.png";
            }
            _animatePartDefaultAttack[E_HERO::FOOT] = getMotionDefault(strPathAttack, attckPer);
            _vecSequence.pushBack(_animatePartDefaultAttack[E_HERO::FOOT]);
            
            _animatePartDefaultMove[E_HERO::FOOT] = getMotionDefault(strPathWalk, 0.125f);
            _vecSequence.pushBack(_animatePartDefaultMove[E_HERO::FOOT]);
        }
        
        //attack cape
        {
            int nIdx = (int)CostumeManager::getInstance()->getEquip(E_COSTUME::CAPE);

            strPathAttack = MafUtils::format("Assets/character_hero/normal/cape/cape_%.2d_attack_%s.png", nIdx, "%d");
            strPathWalk = MafUtils::format("Assets/character_hero/normal/cape/cape_%.2d_walk_%s.png", nIdx, "%d");
            if ( _eSkin == E_HERO_SKIN::DEFAULT_SUMMER )
            {
                strPathAttack = "Assets/character_hero/event/cape/19summer_cape_01_attack_%d.png";
                strPathWalk = "Assets/character_hero/event/cape/19summer_cape_01_walk_%d.png";
            }
            _animatePartDefaultAttack[E_HERO::CAPE] = getMotionDefault(strPathAttack, attckPer);
            _vecSequence.pushBack(_animatePartDefaultAttack[E_HERO::CAPE]);
            
            _animatePartDefaultMove[E_HERO::CAPE] = getMotionDefault(strPathWalk, 0.125f);
            _vecSequence.pushBack(_animatePartDefaultMove[E_HERO::CAPE]);
        }
         
        //
        _spritePartDefault[E_HERO::SHADOW1]->setVisible(true);
        _spritePartDefault[E_HERO::AURA]->setVisible(true);
        _spritePartDefault[E_HERO::BODY]->setVisible(true);
        _spritePartDefault[E_HERO::HEAD]->setVisible(true);
        _spritePartDefault[E_HERO::HAND]->setVisible(true);
        _spritePartDefault[E_HERO::FOOT]->setVisible(true);
        _spritePartDefault[E_HERO::CAPE]->setVisible(true);
        _spritePartDefault[E_HERO::WEAPON]->setVisible(true);
        // 아우라
        {
            int eAuraEquip = CostumeManager::getInstance()->getEquip(E_COSTUME::SETITEM);
            if ( eAuraEquip == E_COSTUME_IDX::IDX_1_DEFAULT )
            {
                _spritePartDefault[E_HERO::AURA]->setVisible(false);
            }
            else
            {
                _spritePartDefault[E_HERO::AURA]->setVisible(true);
                _spritePartDefault[E_HERO::AURA]->setColor(DataManager::GetAuraColor((E_COSTUME_IDX)eAuraEquip));
            }
        }
    }
    else if ( _eSkin == E_HERO_SKIN::HERO || _eSkin == E_HERO_SKIN::PREMIUM )
    {// SKIN : PREMIUM, HERO
        std::string strPathAttack = "";
        std::string strPathWalk = "";
        std::string strPathAttackEffect = "";
        std::string strPathWalkEffect = "";
        
        _animatePartSkinMove = nullptr;
        _animatePartSkinAttack = nullptr;
        _animatePartSkinMoveEffect = nullptr;
        _animatePartSkinAttackEffect = nullptr;
        if ( _eSkin == E_HERO_SKIN::HERO )
        {
            strPathAttack = "Assets/character_hero/skin/hero_buff_attack_%d.png";
            strPathWalk = "Assets/character_hero/skin/hero_buff_walk_%d.png";
            strPathWalkEffect = "Assets/character_hero/skin/hero_buff_walk_effect_%d.png";
            
            _animatePartSkinAttack = getMotionDefault(strPathAttack, attckPer);
            _vecSequence.pushBack(_animatePartSkinAttack);
            
            _animatePartSkinMove = getMotionDefault(strPathWalk, 0.125f);
            _vecSequence.pushBack(_animatePartSkinMove);
            
            _animatePartSkinMoveEffect = getMotionDefault(strPathWalkEffect, 0.125f);
            if ( _animatePartSkinMoveEffect != nullptr )
            {
                _vecSequence.pushBack(_animatePartSkinMoveEffect);
            }
            
        }
        else
        {
            int nSkinEquip = CostumeSkinManager::getInstance()->getCostumeSkinEquip();
            if ( nSkinDummy != 0 )
            {
                nSkinEquip = nSkinDummy;
            }
            
            auto objSkin = CostumeSkinManager::getInstance()->getInfoSkin(nSkinEquip);
            if ( objSkin == nullptr )
            {
                CostumeSkinManager::getInstance()->setCostumeSkinEquip(0);
                
                if(nSkinDummy == nSkinEquip)
                    setMotion(0, nWeaponDummy);
                else
                    setMotion(nSkinDummy, nWeaponDummy);
                
                return;
            }
            
            strPathAttack = MafUtils::format("Assets/character_hero/skin/skin_%02d_attack_%s.png", nSkinEquip, "%d");
            strPathWalk = MafUtils::format("Assets/character_hero/skin/skin_%02d_walk_%s.png", nSkinEquip, "%d");
            strPathAttackEffect = MafUtils::format("Assets/character_hero/skin/skin_%02d_attack_effect_%s.png", nSkinEquip, "%d");
            strPathWalkEffect = MafUtils::format("Assets/character_hero/skin/skin_%02d_walk_effect_%s.png", nSkinEquip, "%d");
            
            _animatePartSkinAttack = getMotionDefault(strPathAttack, attckPer);
            _vecSequence.pushBack(_animatePartSkinAttack);
            
            _animatePartSkinMove = getMotionDefault(strPathWalk, 0.125f);
            _vecSequence.pushBack(_animatePartSkinMove);
            
            _animatePartSkinAttackEffect = getMotionDefault(strPathAttackEffect, attckPer);
            if ( _animatePartSkinAttackEffect != nullptr )
            {
                _vecSequence.pushBack(_animatePartSkinAttackEffect);
            }
                
            _animatePartSkinMoveEffect = getMotionDefault(strPathWalkEffect, 0.125f);
            if ( _animatePartSkinMoveEffect != nullptr )
            {
                _vecSequence.pushBack(_animatePartSkinMoveEffect);
            }
        }
        
        
        //
        _spritePartSkin->setVisible(true);
        _spritePartSkinMoveEffect->setVisible(true);
        _spritePartSkinAttackEffect->setVisible(true);
    }
 
    
}


void UnitBehaviorHero::setMotionDevil(const bool isNotRiderMotion)
{
    //
    _vecSequence.clear();
    
    
    double attckPer = UserInfoManager::getInstance()->getAttackSpeed(_ePlace);
    attckPer*=0.25;
    
    //
    _eSkin = E_HERO_SKIN::DEFAULT;
    
    //
    _spritePartDefault[E_HERO::SHADOW1]->setVisible(false);
    _spritePartDefault[E_HERO::RIDER]->setVisible(false);
    _spritePartDefault[E_HERO::BODY]->setVisible(false);
    _spritePartDefault[E_HERO::HEAD]->setVisible(false);
    _spritePartDefault[E_HERO::CAPE]->setVisible(false);
    _spritePartDefault[E_HERO::WEAPON]->setVisible(false);
    
    // 라이더
    int time = UserInfoManager::getInstance()->getBuffTime(E_BUFF_TYPE::BUFF_D_MOVE_SPD);
    if (!isNotRiderMotion && time > 0 )
    {
        _bRider = true;
    }
    else
    {
        _bRider = false;
    }
    
    //
    std::string strPathAttack = "";
    std::string strPathWalk = "";
    
    // weapon
    {
        int idx = UserInfoManager::getInstance()->getEquipDevil(E_EQUIP_TYPE::E_D_WEAPON);
        strPathAttack = MafUtils::format("Assets/character_hero/devil/weapon/weapon_%.2d_attack_%s.png", idx, "%d");
        strPathWalk = MafUtils::format("Assets/character_hero/devil/weapon/weapon_%.2d_walk_%s.png", idx, "%d");
        
        _animatePartDefaultAttack[E_HERO::WEAPON] = getMotionDefault(strPathAttack, attckPer);
        _vecSequence.pushBack(_animatePartDefaultAttack[E_HERO::WEAPON]);
        
        _animatePartDefaultMove[E_HERO::WEAPON] = getMotionDefault(strPathWalk, 0.125f);
        _vecSequence.pushBack(_animatePartDefaultMove[E_HERO::WEAPON]);
    }
    
    // head
    {
        int idx = UserInfoManager::getInstance()->getEquipDevil(E_EQUIP_TYPE::E_D_HEAD);
        strPathAttack = MafUtils::format("Assets/character_hero/devil/head/head_%.2d_attack_%s.png", idx, "%d");
        strPathWalk = MafUtils::format("Assets/character_hero/devil/head/head_%.2d_walk_%s.png", idx, "%d");
        
        _animatePartDefaultAttack[E_HERO::HEAD] = getMotionDefault(strPathAttack, attckPer);
        _vecSequence.pushBack(_animatePartDefaultAttack[E_HERO::HEAD]);
        
        _animatePartDefaultMove[E_HERO::HEAD] = getMotionDefault(strPathWalk, 0.125f);
        _vecSequence.pushBack(_animatePartDefaultMove[E_HERO::HEAD]);
    }
    
    // body
    {
        int idx = UserInfoManager::getInstance()->getEquipDevil(E_EQUIP_TYPE::E_D_BODY);
        strPathAttack = MafUtils::format("Assets/character_hero/devil/body/body_%.2d_attack_%s.png", idx, "%d");
        strPathWalk = MafUtils::format("Assets/character_hero/devil/body/body_%.2d_walk_%s.png", idx, "%d");
        if ( _bRider == true )
        {
            strPathWalk = MafUtils::format("Assets/character_hero/devil/body/body_%.2d_run_%s.png", idx, "%d");
        }
        
        _animatePartDefaultAttack[E_HERO::BODY] = getMotionDefault(strPathAttack, attckPer);
        _vecSequence.pushBack(_animatePartDefaultAttack[E_HERO::BODY]);
        
        _animatePartDefaultMove[E_HERO::BODY] = getMotionDefault(strPathWalk, 0.125f);
        _vecSequence.pushBack(_animatePartDefaultMove[E_HERO::BODY]);
    }
    
    // cape
    {
        int idx = UserInfoManager::getInstance()->getEquipDevil(E_EQUIP_TYPE::E_D_CAPE);
        strPathAttack = MafUtils::format("Assets/character_hero/devil/cape/cape_%.2d_attack_%s.png", idx, "%d");
        strPathWalk = MafUtils::format("Assets/character_hero/devil/cape/cape_%.2d_walk_%s.png", idx, "%d");
        
        _animatePartDefaultAttack[E_HERO::CAPE] = getMotionDefault(strPathAttack, attckPer);
        _vecSequence.pushBack(_animatePartDefaultAttack[E_HERO::CAPE]);
        
        _animatePartDefaultMove[E_HERO::CAPE] = getMotionDefault(strPathWalk, 0.125f);
        _vecSequence.pushBack(_animatePartDefaultMove[E_HERO::CAPE]);
    }
    
    // rider
    {
        strPathAttack = "Assets/character_pet/devil/rider/wolf_wait_%d.png";
        strPathWalk = "Assets/character_pet/devil/rider/wolf_run_%d.png";
        
        _animatePartDefaultAttack[E_HERO::RIDER] = getMotionDefault(strPathAttack, 0.0625f);
        _vecSequence.pushBack(_animatePartDefaultAttack[E_HERO::RIDER]);
        
        _animatePartDefaultMove[E_HERO::RIDER] = getMotionDefault(strPathWalk, 0.0625f);
        _vecSequence.pushBack(_animatePartDefaultMove[E_HERO::RIDER]);
    }
    
    
    //
    if ( _bRider == true )
    {
        _spritePartDefault[E_HERO::RIDER]->setVisible(true);
    }
    else
    {
        _spritePartDefault[E_HERO::RIDER]->setVisible(false);
    }
    _spritePartDefault[E_HERO::SHADOW1]->setVisible(false);
    _spritePartDefault[E_HERO::WEAPON]->setVisible(false);
    _spritePartDefault[E_HERO::BODY]->setVisible(true);
    _spritePartDefault[E_HERO::HEAD]->setVisible(true);
    _spritePartDefault[E_HERO::CAPE]->setVisible(true);
}

Animate* UnitBehaviorHero::getMotionDefault(std::string strPath, double delay)
{
    Vector<cocos2d::SpriteFrame*> list;
    
    int count = 0;
    
    for ( int i = 1; i <= 100; i++ )
    {
        std::string strTempPath = MafUtils::format(strPath.c_str(), i);
        std::string fullpath = FileUtils::getInstance()->fullPathForFilename(strTempPath);
        
        if(fullpath.size() == 0)
            break;
        
        auto spriteAni = Sprite::create(strTempPath);
        if ( spriteAni == nullptr )
        {
            return nullptr;
        }
        
        spriteAni->getTexture()->setTexParameters(texParams);
        list.pushBack(spriteAni->getSpriteFrame());
        count = i;
    }
    
    delay = delay / (count / 4);
    
    if ( list.size() <= 0 )
    {
        return nullptr;
    }
    else
    {
        auto animation = Animation::createWithSpriteFrames(list, delay);
        auto animate = Animate::create(animation);
        return animate;
    }
}

void UnitBehaviorHero::setPositionHero(Node *parent, float height)
{
    Size size = parent->getContentSize();
    
    Vec2 startPosition = Vec2::ZERO;
    if ( _ePlace == E_PLACE::DG_DEVIL_DOM )
    {
        startPosition = Vec2(size.width*0.5 + _layerCharacter->getContentSize().width*0.3, size.height- height*0.92);
        
        if ( UserInfoManager::getInstance()->getFloorCount(_ePlace) == 1 )
            startPosition = Vec2(-_layerCharacter->getContentSize().width, size.height - height*0.92);
    }
    else if(_ePlace ==E_PLACE::DG_PRISON )
    {
        startPosition = Vec2(size.width*0.5 + _layerCharacter->getContentSize().width*0.45, 140);
    }
    else if(_ePlace ==E_PLACE::DG_NORMAL )
    {
        double heightPlus = UserInfoManager::getInstance()->_fHeightIphoneX * (180.0 / 700.0);
        startPosition = Vec2(size.width*0.5 + _layerCharacter->getContentSize().width*0.45, size.height- height + 215 + heightPlus);
        
        if ( UserInfoManager::getInstance()->getFloorCount(_ePlace) == 1 )
            startPosition = Vec2(-_layerCharacter->getContentSize().width, size.height - height + 215 + heightPlus);
    }
    else
    {
        startPosition = Vec2(size.width*0.5 + _layerCharacter->getContentSize().width*0.45, size.height- height*0.92);
    }
        
    //
    setPosition(startPosition);
}

void UnitBehaviorHero::setPositionDevilHeroChange(bool bMove)
{
    if( _bRider )
    {
        if ( bMove )
        {// move
            float h = 20;
            _spritePartDefault[E_HERO::BODY]->setPosition(-7*_chaScale, 7*_chaScale+h);
            _spritePartDefault[E_HERO::HEAD]->setPosition(-7*_chaScale, 7*_chaScale+h);
            _spritePartDefault[E_HERO::CAPE]->setPosition(-7*_chaScale, 7*_chaScale+h);
            _spritePartDefault[E_HERO::RIDER]->setPosition(_layerCharacter->getContentSize().width, h);
        }
        else
        {// attack
            _spritePartDefault[E_HERO::BODY]->setPosition(0,0);
            _spritePartDefault[E_HERO::HEAD]->setPosition(0,0);
            _spritePartDefault[E_HERO::CAPE]->setPosition(0,0);
            _spritePartDefault[E_HERO::RIDER]->setPosition(_layerCharacter->getContentSize().width-1*_chaScale,20*_chaScale);
        }
    }
    else
    {
        _spritePartDefault[E_HERO::BODY]->setPosition(0,0);
        _spritePartDefault[E_HERO::HEAD]->setPosition(0,0);
        _spritePartDefault[E_HERO::CAPE]->setPosition(0,0);
    }
}

#pragma mark - Hero : pet buff
void UnitBehaviorHero::onPrincessBuff(E_PRINCESS_BUFF type,float time)
{
    if ( type == E_PRINCESS_BUFF::BUFF_NONE )
        return;
    
    SoundManager::SoundEffectStart(SOUND_BUFF);
    
    _nTimePrincessBuff = time;
    UserInfoManager::getInstance()->setPetBuffPriencess(_ePlace, type);
    
    // 버프 배경
    if ( _animateBuffBack == nullptr )
    {
        auto animation = Animation::create();
        for(int i=1; i<=4; i++){
            std::string name = MafUtils::format("Assets/character_hero/normal/effect/ef_princess_%d.png",i);
            animation->addSpriteFrameWithFile(name);
        }
        animation->setDelayPerUnit(0.1f);
        _animateBuffBack = Animate::create(animation);
        _animateBuffBack->retain();
    }
    
    if ( _spriteBuffBack == nullptr )
    {
        _spriteBuffBack = Sprite::create();
        _spriteBuffBack->setAnchorPoint(Vec2(1,0));
        _spriteBuffBack->setPosition(Vec2(0,0));
        addChild(_spriteBuffBack);
    }
    
    auto seq = Sequence::create(DelayTime::create(0.1f), Show::create(), DelayTime::create(0.3f), Hide::create(), NULL);
    _spriteBuffBack->stopAllActions();
    _spriteBuffBack->runAction(_animateBuffBack);
    _spriteBuffBack->runAction(seq);
    
    
    // 버프 아이콘
    Animate* animate = nullptr;
    switch (type) {
        case E_PRINCESS_BUFF::BUFF_ATTACK:
            animate = _animateBuffAttack;
            break;
        case E_PRINCESS_BUFF::BUFF_MOVE:
            animate = _animateBuffSpeed;
            break;
        case E_PRINCESS_BUFF::BUFF_CRITICAL:
            animate = _animateBuffCritical;
            break;
        case E_PRINCESS_BUFF::BUFF_ALL:
            animate = _animateBuffAll;
            break;
        default: break;
    }
    
    if ( animate == nullptr )
    {
        std::string name = "";
        auto animation = Animation::create();
        for(int i=1; i <=4; i++){
            if(type == E_PRINCESS_BUFF::BUFF_ATTACK)            name = MafUtils::format("Assets/character_pet/normal/effect/effect_princess_attack_%d.png",i);
            else if(type == E_PRINCESS_BUFF::BUFF_MOVE)         name = MafUtils::format("Assets/character_pet/normal/effect/effect_princess_move_%d.png",i);
            else if(type == E_PRINCESS_BUFF::BUFF_CRITICAL)     name = MafUtils::format("Assets/character_pet/normal/effect/effect_princess_cri_%d.png",i);
            else if(type == E_PRINCESS_BUFF::BUFF_ALL)          name = MafUtils::format("Assets/character_pet/normal/effect/effect_princess_all_%d.png",i);
            
            animation->addSpriteFrameWithFile(name);
        }
        animation->setDelayPerUnit(0.1f);
        animate = Animate::create(animation);
        animate->retain();
        
        switch (type) {
            case E_PRINCESS_BUFF::BUFF_ATTACK:
                _animateBuffAttack = animate;
                break;
            case E_PRINCESS_BUFF::BUFF_MOVE:
                _animateBuffSpeed = animate;
                break;
            case E_PRINCESS_BUFF::BUFF_CRITICAL:
                _animateBuffCritical = animate;
                break;
            case E_PRINCESS_BUFF::BUFF_ALL:
                _animateBuffAll = animate;
                break;
            default: break;
        }
    }
    
    if ( _spriteBuff == nullptr )
    {
        _spriteBuff = Sprite::create();
        _spriteBuff->setAnchorPoint(Vec2(1,0));
        _spriteBuff->setPosition(0,0);
        _spriteBuff->setVisible(false);
        addChild(_spriteBuff,11);
    }
    
    auto action1_2 = DelayTime::create(0.1f);
    auto action1_3 = Show::create();
    auto seq1 = Sequence::create(action1_2, action1_3, NULL);
    _spriteBuff->stopAllActions();
    _spriteBuff->runAction(RepeatForever::create(animate));
    _spriteBuff->runAction(seq1);
    
    auto action2_1 = DelayTime::create(_nTimePrincessBuff);
    auto action2_2 = CallFunc::create([=](){
        
        _spriteBuff->stopAllActions();
        
        _nTimePrincessBuff = 0;
        UserInfoManager::getInstance()->setPetBuffPriencess(_ePlace, E_PRINCESS_BUFF::BUFF_NONE);
    });
    auto action2_3 = Hide::create();
    auto seq2 = Sequence::create(action2_1, action2_2, action2_3, NULL);
    _spriteBuff->runAction(seq2);
}

void UnitBehaviorHero::onGatekeeperBuff(E_GATEKEEPER_BUFF type,float time)
{
    if ( type == E_GATEKEEPER_BUFF::BUFF_NONE_GATEKEEPER )
    {
        return;
    }
    
    _nTimeGatekeeperBuff = time;
    UserInfoManager::getInstance()->setPetBuffGatekeeper(_ePlace, type);
    
    Animate* animate = nullptr;
    switch (type) {
        case E_GATEKEEPER_BUFF::BUFF_ATTACK_GATEKEEPER:
            animate = _animateBuffAttack;
            break;
        case E_GATEKEEPER_BUFF::BUFF_MOVESPPED_GATEKEEPER:
            animate = _animateBuffSpeed;
            break;
        case E_GATEKEEPER_BUFF::BUFF_CRITICAL_GATEKEEPER:
            animate = _animateBuffCritical;
            break;
        case E_GATEKEEPER_BUFF::BUFF_ATTACKSPPED_GATEKEEPER:
            animate = _animateBuffAttackSpeed;
            break;
        default: break;
    }

    if ( animate == nullptr )
    {
        std::string name = "";
        auto animation = Animation::create();
        for(int i=1; i <=4; i++){
            if(type == E_GATEKEEPER_BUFF::BUFF_ATTACK_GATEKEEPER)           name = MafUtils::format("Assets/character_pet/devil/effect/pet_03_effect_4_%d.png",i);
            else if(type == E_GATEKEEPER_BUFF::BUFF_MOVESPPED_GATEKEEPER)   name = MafUtils::format("Assets/character_pet/devil/effect/pet_03_effect_1_%d.png",i);
            else if(type == E_GATEKEEPER_BUFF::BUFF_CRITICAL_GATEKEEPER)    name = MafUtils::format("Assets/character_pet/devil/effect/pet_03_effect_3_%d.png",i);
            else if(type == E_GATEKEEPER_BUFF::BUFF_ATTACKSPPED_GATEKEEPER) name = MafUtils::format("Assets/character_pet/devil/effect/pet_03_effect_2_%d.png",i);
            
            animation->addSpriteFrameWithFile(name);
        }
        animation->setDelayPerUnit(0.1f);
        animate = Animate::create(animation);
        animate->retain();
        
        switch (type) {
            case E_GATEKEEPER_BUFF::BUFF_ATTACK_GATEKEEPER:
                _animateBuffAttack = animate;
                break;
            case E_GATEKEEPER_BUFF::BUFF_MOVESPPED_GATEKEEPER:
                _animateBuffSpeed = animate;
                break;
            case E_GATEKEEPER_BUFF::BUFF_CRITICAL_GATEKEEPER:
                _animateBuffCritical = animate;
                break;
            case E_GATEKEEPER_BUFF::BUFF_ATTACKSPPED_GATEKEEPER:
                _animateBuffAttackSpeed = animate;
                break;
            default: break;
        }
    }
    
    if ( _spriteBuff == nullptr )
    {
        _spriteBuff = Sprite::create();
        _spriteBuff->setAnchorPoint(Vec2(1,0));
        _spriteBuff->setPosition(0,0);
        _spriteBuff->setScale(_chaScale);
        _spriteBuff->setVisible(false);
        addChild(_spriteBuff,11);
    }
    
    auto action1_2 = DelayTime::create(0.1f);
    auto action1_3 = Show::create();
    auto seq1 = Sequence::create(action1_2, action1_3, NULL);
    _spriteBuff->runAction(RepeatForever::create(animate));
    _spriteBuff->runAction(seq1);

    auto action2_1 = DelayTime::create(_nTimeGatekeeperBuff);
    auto action2_2 = CallFunc::create([=](){
        
        _spriteBuff->stopAllActions();
        
        _nTimeGatekeeperBuff = 0;
        UserInfoManager::getInstance()->setPetBuffGatekeeper(_ePlace, E_GATEKEEPER_BUFF::BUFF_NONE_GATEKEEPER);
    });
    auto action2_3 = Hide::create();
    auto seq2 = Sequence::create(action2_1, action2_2, action2_3, NULL);
    _spriteBuff->runAction(seq2);
}

void UnitBehaviorHero::updatePrincessBuff(float dt)
{
    _nTimePrincessBuff -= dt;
    
    if ( _nTimePrincessBuff <= 0 )
    {
        _nTimePrincessBuff = 0;
        UserInfoManager::getInstance()->setPetBuffPriencess(_ePlace, E_PRINCESS_BUFF::BUFF_NONE);
        
        if ( _spriteBuff != nullptr )
        {
            _spriteBuff->stopAllActions();
            _spriteBuff->removeFromParent();
            _spriteBuff = nullptr;
        }
        
        auto sch = Director::getInstance()->getScheduler();
        if( sch->isScheduled(schedule_selector(UnitBehaviorHero::updatePrincessBuff), this) )
        {
            sch->unschedule(schedule_selector(UnitBehaviorHero::updatePrincessBuff), this);
        }
    }
}

void UnitBehaviorHero::updateGatekeeperBuff(float dt)
{
    _nTimeGatekeeperBuff -= dt;
    
    if ( _nTimeGatekeeperBuff <= 0 )
    {
        _nTimeGatekeeperBuff = 0;
        UserInfoManager::getInstance()->setPetBuffGatekeeper(_ePlace, E_GATEKEEPER_BUFF::BUFF_NONE_GATEKEEPER);
        
        if ( _spriteBuff != nullptr )
        {
            _spriteBuff->stopAllActions();
            _spriteBuff->removeFromParent();
            _spriteBuff = nullptr;
        }
    
        auto sch = Director::getInstance()->getScheduler();
        if( sch->isScheduled(schedule_selector(UnitBehaviorHero::updateGatekeeperBuff), this) )
        {
            sch->unschedule(schedule_selector(UnitBehaviorHero::updateGatekeeperBuff), this);
        }
    }
}

double UnitBehaviorHero::getPrincessBuffTime()
{
    return _nTimePrincessBuff;
}
double UnitBehaviorHero::getGatekeeperBuffTime()
{
    return _nTimeGatekeeperBuff;
}

#pragma mark - Hero : action default
void UnitBehaviorHero::actionMoveUpstairs()
{
    SoundManager::SoundEffectStart(SOUND_RUN);
    
    //
    _eState = E_HERO_STATE::MOVE;
    
    //
    if ( _bChangeParts )
    {
        _bChangeParts = false;
        if ( _ePlace == E_PLACE::DG_DEVIL_DOM )
        {
            setMotionDevil();
        }
        else
        {
            setMotion();
        }
    }
    
    //
    if ( _ePlace == E_PLACE::DG_DEVIL_DOM )
    {
        setPositionDevilHeroChange(true);
    }
    
    //
    float nTimeOriginal = UserInfoManager::getInstance()->getUserSpeed(_ePlace, false);
    float nTimeDelay = nTimeOriginal;
    float nTimeMove = nTimeDelay * 4 * 2;
    
    if ( _ePlace == E_PLACE::DG_PRISON )
    {
        // 이용시간 30초 > 15초로 변경에 따른 이동속도 증가
        nTimeMove *= 0.25f;
    }
    
    //
    Size size = getContentSize();
    
    auto action = MoveTo::create(nTimeMove, Vec2(size.width + _layerCharacter->getContentSize().width, getPositionY()));
    auto seq = Sequence::create(action, _onUpstairsCallback, nullptr);
    this->runAction(seq);
    
    //
    if ( _ePlace == E_PLACE::DG_DEVIL_DOM )
    {
        _animatePartDefaultMove[E_HERO::RIDER]->getAnimation()->setDelayPerUnit(nTimeDelay);
        _animatePartDefaultMove[E_HERO::BODY]->getAnimation()->setDelayPerUnit(nTimeDelay);
        _animatePartDefaultMove[E_HERO::HEAD]->getAnimation()->setDelayPerUnit(nTimeDelay);
        _animatePartDefaultMove[E_HERO::CAPE]->getAnimation()->setDelayPerUnit(nTimeDelay);
        _animatePartDefaultMove[E_HERO::WEAPON]->getAnimation()->setDelayPerUnit(nTimeDelay);
        
        Repeat *repeat = nullptr;
        
        repeat = Repeat::create(_animatePartDefaultMove[E_HERO::RIDER]->clone(), 2);
        _spritePartDefault[E_HERO::RIDER]->runAction(repeat);
        
        repeat = Repeat::create(_animatePartDefaultMove[E_HERO::BODY]->clone(), 4);
        _spritePartDefault[E_HERO::BODY]->runAction(repeat);
         
        repeat = Repeat::create(_animatePartDefaultMove[E_HERO::HEAD]->clone(), 4);
        _spritePartDefault[E_HERO::HEAD]->runAction(repeat);
        
        repeat = Repeat::create(_animatePartDefaultMove[E_HERO::CAPE]->clone(), 4);
        _spritePartDefault[E_HERO::CAPE]->runAction(repeat);
        
        repeat = Repeat::create(_animatePartDefaultMove[E_HERO::WEAPON]->clone(), 4);
        _spritePartDefault[E_HERO::WEAPON]->runAction(repeat);
        
        if ( _bRider )
        {
            _spritePartDefault[E_HERO::SHADOW1]->setVisible(false);
            _spritePartDefault[E_HERO::RIDER]->setVisible(true);
            _spritePartDefault[E_HERO::WEAPON]->setVisible(false);
        }
        else
        {
            _spritePartDefault[E_HERO::SHADOW1]->setVisible(true);
            _spritePartDefault[E_HERO::RIDER]->setVisible(false);
            _spritePartDefault[E_HERO::WEAPON]->setVisible(true);
        }
    }
    else
    {
        if ( _eSkin == E_HERO_SKIN::DEFAULT || _eSkin == E_HERO_SKIN::DEFAULT_SUMMER )
        {// SKIN : DEFAULT, DEFAULT_EVENT
            _animatePartDefaultMove[E_HERO::BODY]->getAnimation()->setDelayPerUnit(nTimeDelay);
            _animatePartDefaultMove[E_HERO::HEAD]->getAnimation()->setDelayPerUnit(nTimeDelay);
            _animatePartDefaultMove[E_HERO::HAND]->getAnimation()->setDelayPerUnit(nTimeDelay);
            _animatePartDefaultMove[E_HERO::FOOT]->getAnimation()->setDelayPerUnit(nTimeDelay);
            _animatePartDefaultMove[E_HERO::CAPE]->getAnimation()->setDelayPerUnit(nTimeDelay);
            _animatePartDefaultMove[E_HERO::WEAPON]->getAnimation()->setDelayPerUnit(nTimeDelay);
            
            Repeat *repeat = nullptr;
            
            repeat = Repeat::create(_animatePartDefaultMove[E_HERO::BODY]->clone(), 4);
            _spritePartDefault[E_HERO::BODY]->runAction(repeat);
             
            repeat = Repeat::create(_animatePartDefaultMove[E_HERO::HEAD]->clone(), 4);
            _spritePartDefault[E_HERO::HEAD]->runAction(repeat);
            
            repeat = Repeat::create(_animatePartDefaultMove[E_HERO::HAND]->clone(), 4);
            _spritePartDefault[E_HERO::HAND]->runAction(repeat);
            
            repeat = Repeat::create(_animatePartDefaultMove[E_HERO::FOOT]->clone(), 4);
            _spritePartDefault[E_HERO::FOOT]->runAction(repeat);
            
            repeat = Repeat::create(_animatePartDefaultMove[E_HERO::CAPE]->clone(), 4);
            _spritePartDefault[E_HERO::CAPE]->runAction(repeat);
            
            repeat = Repeat::create(_animatePartDefaultMove[E_HERO::WEAPON]->clone(), 4);
            _spritePartDefault[E_HERO::WEAPON]->runAction(repeat);
        }
        else if ( _eSkin == E_HERO_SKIN::HERO || _eSkin == E_HERO_SKIN::PREMIUM )
        {// SKIN : PREMIUM, HERO
            //
            _spritePartSkinMoveEffect->setVisible(false);
            _spritePartSkinAttackEffect->setVisible(false);
            
            //
            _animatePartSkinMove->getAnimation()->setDelayPerUnit(nTimeDelay);
            if ( _animatePartSkinMoveEffect != nullptr )
            {
                _animatePartSkinMoveEffect->getAnimation()->setDelayPerUnit(nTimeDelay);
            }
            
            
            //
            Repeat *repeat = nullptr;
            
            repeat = Repeat::create(_animatePartSkinMove->clone(), 4);
            _spritePartSkin->runAction(repeat);
            
            if ( _animatePartSkinMoveEffect != nullptr )
            {
                repeat = Repeat::create(_animatePartSkinMoveEffect->clone(), 4);
                _spritePartSkinMoveEffect->runAction(repeat);
                _spritePartSkinMoveEffect->setVisible(true);
            }
        }
        
        //
        if ( _shadow != nullptr )
        {
            _shadow->setHide();
        }
    }
    
    
    
}

void UnitBehaviorHero::actionMove()
{
    SoundManager::SoundEffectStart(SOUND_RUN);
    
    //
    _eState = E_HERO_STATE::MOVE;
    
    //
    if ( _bChangeParts )
    {
        _bChangeParts = false;
        if ( _ePlace == E_PLACE::DG_DEVIL_DOM )
        {
            setMotionDevil();
        }
        else
        {
            setMotion();
        }
    }
    
    //
    if ( _ePlace == E_PLACE::DG_DEVIL_DOM )
    {
        setPositionDevilHeroChange(true);
    }
    
    float nTimeOriginal = UserInfoManager::getInstance()->getUserSpeed(_ePlace, false);
    float nTimeDelay = nTimeOriginal;
    float nTimeMove1 = nTimeDelay * 4 * 4;
    float nTimeMove2 = nTimeDelay * 4 * 2;
    
    if ( UserInfoManager::getInstance()->getFloorCount(_ePlace) == 1 )
    {
        Size size = getContentSize();
        MoveTo* actionMove = nullptr;
        
        if(_ePlace ==E_PLACE::DG_DEVIL_DOM )
            actionMove = MoveTo::create(nTimeMove1, Vec2(size.width*0.5 + _layerCharacter->getContentSize().width*0.3, getPositionY()));
        else
            actionMove = MoveTo::create(nTimeMove1, Vec2(size.width*0.5 + _layerCharacter->getContentSize().width*0.45, getPositionY()));
        
        auto seq = Sequence::create(actionMove, _onMoveCallback, nullptr);
        this->runAction(seq);
    }
    else
    {
        auto seq = Sequence::create(DelayTime::create(nTimeMove2), _onMoveCallback, nullptr);
        this->runAction(seq);
    }
    
    if ( _ePlace == E_PLACE::DG_DEVIL_DOM )
    {
        _animatePartDefaultMove[E_HERO::RIDER]->getAnimation()->setDelayPerUnit(nTimeDelay);
        _animatePartDefaultMove[E_HERO::BODY]->getAnimation()->setDelayPerUnit(nTimeDelay);
        _animatePartDefaultMove[E_HERO::HEAD]->getAnimation()->setDelayPerUnit(nTimeDelay);
        _animatePartDefaultMove[E_HERO::CAPE]->getAnimation()->setDelayPerUnit(nTimeDelay);
        _animatePartDefaultMove[E_HERO::WEAPON]->getAnimation()->setDelayPerUnit(nTimeDelay);
        
        RepeatForever *repeat = nullptr;
        
        repeat = RepeatForever::create(_animatePartDefaultMove[E_HERO::RIDER]->clone());
        _spritePartDefault[E_HERO::RIDER]->runAction(repeat);
        
        repeat = RepeatForever::create(_animatePartDefaultMove[E_HERO::BODY]->clone());
        _spritePartDefault[E_HERO::BODY]->runAction(repeat);
        
        repeat = RepeatForever::create(_animatePartDefaultMove[E_HERO::HEAD]->clone());
        _spritePartDefault[E_HERO::HEAD]->runAction(repeat);
        
        repeat = RepeatForever::create(_animatePartDefaultMove[E_HERO::CAPE]->clone());
        _spritePartDefault[E_HERO::CAPE]->runAction(repeat);
        
        repeat = RepeatForever::create(_animatePartDefaultMove[E_HERO::WEAPON]->clone());
        _spritePartDefault[E_HERO::WEAPON]->runAction(repeat);
        
        if ( _bRider )
        {
            _spritePartDefault[E_HERO::SHADOW1]->setVisible(false);
            _spritePartDefault[E_HERO::RIDER]->setVisible(true);
            _spritePartDefault[E_HERO::WEAPON]->setVisible(false);
        }
        else
        {
            _spritePartDefault[E_HERO::SHADOW1]->setVisible(true);
            _spritePartDefault[E_HERO::RIDER]->setVisible(false);
            _spritePartDefault[E_HERO::WEAPON]->setVisible(true);
        }
    }
    else
    {
        if ( _eSkin == E_HERO_SKIN::DEFAULT || _eSkin == E_HERO_SKIN::DEFAULT_SUMMER )
        {// SKIN : DEFAULT, DEFAULT_EVENT
            _animatePartDefaultMove[E_HERO::BODY]->getAnimation()->setDelayPerUnit(nTimeDelay);
            _animatePartDefaultMove[E_HERO::HEAD]->getAnimation()->setDelayPerUnit(nTimeDelay);
            _animatePartDefaultMove[E_HERO::HAND]->getAnimation()->setDelayPerUnit(nTimeDelay);
            _animatePartDefaultMove[E_HERO::FOOT]->getAnimation()->setDelayPerUnit(nTimeDelay);
            _animatePartDefaultMove[E_HERO::CAPE]->getAnimation()->setDelayPerUnit(nTimeDelay);
            _animatePartDefaultMove[E_HERO::WEAPON]->getAnimation()->setDelayPerUnit(nTimeDelay);
            
            RepeatForever *repeat = nullptr;
            
            repeat = RepeatForever::create(_animatePartDefaultMove[E_HERO::BODY]->clone());
            _spritePartDefault[E_HERO::BODY]->runAction(repeat);
            
            repeat = RepeatForever::create(_animatePartDefaultMove[E_HERO::HEAD]->clone());
            _spritePartDefault[E_HERO::HEAD]->runAction(repeat);
            
            repeat = RepeatForever::create(_animatePartDefaultMove[E_HERO::HAND]->clone());
            _spritePartDefault[E_HERO::HAND]->runAction(repeat);
            
            repeat = RepeatForever::create(_animatePartDefaultMove[E_HERO::FOOT]->clone());
            _spritePartDefault[E_HERO::FOOT]->runAction(repeat);
            
            repeat = RepeatForever::create(_animatePartDefaultMove[E_HERO::CAPE]->clone());
            _spritePartDefault[E_HERO::CAPE]->runAction(repeat);
            
            repeat = RepeatForever::create(_animatePartDefaultMove[E_HERO::WEAPON]->clone());
            _spritePartDefault[E_HERO::WEAPON]->runAction(repeat);
        }
        else if ( _eSkin == E_HERO_SKIN::HERO || _eSkin == E_HERO_SKIN::PREMIUM )
        {// SKIN : PREMIUM, HERO
            //
            _spritePartSkinMoveEffect->setVisible(false);
            _spritePartSkinAttackEffect->setVisible(false);
            
            //
            _animatePartSkinMove->getAnimation()->setDelayPerUnit(nTimeDelay);
            if ( _animatePartSkinMoveEffect != nullptr )
            {
                _animatePartSkinMoveEffect->getAnimation()->setDelayPerUnit(nTimeDelay);
            }
            
            
            //
            RepeatForever *repeat = nullptr;
            
            repeat = RepeatForever::create(_animatePartSkinMove->clone());
            _spritePartSkin->runAction(repeat);
            
            if ( _animatePartSkinMoveEffect != nullptr )
            {
                repeat = RepeatForever::create(_animatePartSkinMoveEffect->clone());
                _spritePartSkinMoveEffect->runAction(repeat);
                _spritePartSkinMoveEffect->setVisible(true);
            }
        }
        
        //
        if ( _shadow != nullptr )
        {
            _shadow->setHide();
        }
    }
    
 
    
}

void UnitBehaviorHero::actionMoveDungeon(bool first)
{
    SoundManager::SoundEffectStart(SOUND_RUN);
    
    //
    _eState = E_HERO_STATE::MOVE;
    
    //
    if ( _bChangeParts )
    {
        _bChangeParts = false;
        if ( _ePlace == E_PLACE::DG_DEVIL_DOM )
        {
            setMotionDevil();
        }
        else
        {
            setMotion();
        }
    }
    
    
    float nTimeOriginal = UserInfoManager::getInstance()->getUserSpeed(_ePlace, false);
    float nTimeDelay = nTimeOriginal;
    float nTimeMove1 = nTimeDelay * 4 * 4;
    float nTimeMove2 = nTimeDelay * 4 * 2;
    
    // 이용시간 30초 > 15초로 변경에 따른 이동속도 증가
    nTimeMove1 *= 0.25f;
    nTimeMove2 *= 0.25f;
    
    if ( _eSkin == E_HERO_SKIN::DEFAULT || _eSkin == E_HERO_SKIN::DEFAULT_SUMMER )
    {// SKIN : DEFAULT, DEFAULT_EVENT
        _animatePartDefaultMove[E_HERO::BODY]->getAnimation()->setDelayPerUnit(nTimeDelay);
        _animatePartDefaultMove[E_HERO::HEAD]->getAnimation()->setDelayPerUnit(nTimeDelay);
        _animatePartDefaultMove[E_HERO::HAND]->getAnimation()->setDelayPerUnit(nTimeDelay);
        _animatePartDefaultMove[E_HERO::FOOT]->getAnimation()->setDelayPerUnit(nTimeDelay);
        _animatePartDefaultMove[E_HERO::CAPE]->getAnimation()->setDelayPerUnit(nTimeDelay);
        _animatePartDefaultMove[E_HERO::WEAPON]->getAnimation()->setDelayPerUnit(nTimeDelay);
        
        RepeatForever *repeat = nullptr;
        
        repeat = RepeatForever::create(_animatePartDefaultMove[E_HERO::BODY]->clone());
        _spritePartDefault[E_HERO::BODY]->runAction(repeat);
        
        repeat = RepeatForever::create(_animatePartDefaultMove[E_HERO::HEAD]->clone());
        _spritePartDefault[E_HERO::HEAD]->runAction(repeat);
        
        repeat = RepeatForever::create(_animatePartDefaultMove[E_HERO::HAND]->clone());
        _spritePartDefault[E_HERO::HAND]->runAction(repeat);
        
        repeat = RepeatForever::create(_animatePartDefaultMove[E_HERO::FOOT]->clone());
        _spritePartDefault[E_HERO::FOOT]->runAction(repeat);
        
        repeat = RepeatForever::create(_animatePartDefaultMove[E_HERO::CAPE]->clone());
        _spritePartDefault[E_HERO::CAPE]->runAction(repeat);
        
        repeat = RepeatForever::create(_animatePartDefaultMove[E_HERO::WEAPON]->clone());
        _spritePartDefault[E_HERO::WEAPON]->runAction(repeat);
    }
    else if ( _eSkin == E_HERO_SKIN::HERO || _eSkin == E_HERO_SKIN::PREMIUM )
    {// SKIN : PREMIUM, HERO(봉싱강림은 표현 안함)
        //
        _spritePartSkinMoveEffect->setVisible(false);
        _spritePartSkinAttackEffect->setVisible(false);
        
        //
        _animatePartSkinMove->getAnimation()->setDelayPerUnit(nTimeDelay);
        if ( _animatePartSkinMoveEffect != nullptr )
        {
            _animatePartSkinMoveEffect->getAnimation()->setDelayPerUnit(nTimeDelay);
        }
        
        
        //
        RepeatForever *repeat = nullptr;
        
        repeat = RepeatForever::create(_animatePartSkinMove->clone());
        _spritePartSkin->runAction(repeat);
        
        if ( _animatePartSkinMoveEffect != nullptr )
        {
            repeat = RepeatForever::create(_animatePartSkinMoveEffect->clone());
            _spritePartSkinMoveEffect->runAction(repeat);
            _spritePartSkinMoveEffect->setVisible(true);
        }
    }
    
    //
    if ( _shadow != nullptr )
    {
        _shadow->setHide();
    }
    
    
    if ( first )
    {
        Size size = getContentSize();
        MoveTo* actionMove = nullptr;
        actionMove = MoveTo::create(nTimeMove1, Vec2(size.width*0.5 + _layerCharacter->getContentSize().width*0.45, getPositionY()));
        
        auto seq = Sequence::create(actionMove, _onMoveCallback, nullptr);
        this->runAction(seq);
    }
    else
    {
        auto seq = Sequence::create(DelayTime::create(nTimeMove2), _onMoveCallback, nullptr);
        this->runAction(seq);
    }
}

void UnitBehaviorHero::actionAttack(UnitBehaviorMonster &mon)
{
    _eState = E_HERO_STATE::ATTACK;
    _enemy = &mon;
    
    //
    if ( _bChangeParts )
    {
        _bChangeParts = false;
        if ( _ePlace == E_PLACE::DG_DEVIL_DOM )
        {
            setMotionDevil();
        }
        else
        {
            setMotion();
        }
    }
    
    //
    float nTimeOriginal = UserInfoManager::getInstance()->getAttackSpeed(_ePlace);
    float nTimeDelay = nTimeOriginal * 0.25;
    
    CallFunc *func = CallFunc::create(CC_CALLBACK_0(UnitBehaviorHero::actionAttackContinue,this));
    Sequence *seq = Sequence::create(func,DelayTime::create(nTimeOriginal),nullptr);
    this->runAction(RepeatForever::create(seq));
    
    if ( _ePlace == E_PLACE::DG_DEVIL_DOM )
    {
        setPositionDevilHeroChange(false);

        //
        _animatePartDefaultAttack[E_HERO::RIDER]->getAnimation()->setDelayPerUnit(nTimeDelay);
        _animatePartDefaultAttack[E_HERO::BODY]->getAnimation()->setDelayPerUnit(nTimeDelay);
        _animatePartDefaultAttack[E_HERO::HEAD]->getAnimation()->setDelayPerUnit(nTimeDelay);
        _animatePartDefaultAttack[E_HERO::CAPE]->getAnimation()->setDelayPerUnit(nTimeDelay);
        _animatePartDefaultAttack[E_HERO::WEAPON]->getAnimation()->setDelayPerUnit(nTimeDelay);
        
        _spritePartDefault[E_HERO::RIDER]->runAction(RepeatForever::create(_animatePartDefaultAttack[E_HERO::RIDER]->clone()));
        _spritePartDefault[E_HERO::BODY]->runAction(RepeatForever::create(_animatePartDefaultAttack[E_HERO::BODY]->clone()));
        _spritePartDefault[E_HERO::HEAD]->runAction(RepeatForever::create(_animatePartDefaultAttack[E_HERO::HEAD]->clone()));
        _spritePartDefault[E_HERO::CAPE]->runAction(RepeatForever::create(_animatePartDefaultAttack[E_HERO::CAPE]->clone()));
        _spritePartDefault[E_HERO::WEAPON]->runAction(RepeatForever::create(_animatePartDefaultAttack[E_HERO::WEAPON]->clone()));

        _spritePartDefault[E_HERO::WEAPON]->setVisible(true);
        _spritePartDefault[E_HERO::SHADOW1]->setVisible(true);
    }
    else if ( _ePlace == E_PLACE::DG_NORMAL || _ePlace == E_PLACE::DG_PRISON )
    {
        if ( _eSkin == E_HERO_SKIN::DEFAULT || _eSkin == E_HERO_SKIN::DEFAULT_SUMMER )
        {// SKIN : DEFAULT, DEFAULT_EVENT
            _spritePartDefault[E_HERO::BODY]->runAction(RepeatForever::create(_animatePartDefaultAttack[E_HERO::BODY]->clone()));
            _spritePartDefault[E_HERO::HEAD]->runAction(RepeatForever::create(_animatePartDefaultAttack[E_HERO::HEAD]->clone()));
            _spritePartDefault[E_HERO::HAND]->runAction(RepeatForever::create(_animatePartDefaultAttack[E_HERO::HAND]->clone()));
            _spritePartDefault[E_HERO::FOOT]->runAction(RepeatForever::create(_animatePartDefaultAttack[E_HERO::FOOT]->clone()));
            _spritePartDefault[E_HERO::CAPE]->runAction(RepeatForever::create(_animatePartDefaultAttack[E_HERO::CAPE]->clone()));
            _spritePartDefault[E_HERO::WEAPON]->runAction(RepeatForever::create(_animatePartDefaultAttack[E_HERO::WEAPON]->clone()));
        }
        else if ( _eSkin == E_HERO_SKIN::HERO || _eSkin == E_HERO_SKIN::PREMIUM )
        {// SKIN : PREMIUM, HERO

            if ( _spritePartSkin->getNumberOfRunningActions() != 0 )                _spritePartSkin->stopAllActions();
            if ( _spritePartSkinMoveEffect->getNumberOfRunningActions() != 0 )      _spritePartSkinMoveEffect->stopAllActions();
            if ( _spritePartSkinAttackEffect->getNumberOfRunningActions() != 0 )    _spritePartSkinAttackEffect->stopAllActions();

            //
            _spritePartSkinMoveEffect->setVisible(false);
            _spritePartSkinAttackEffect->setVisible(false);

            //
            _spritePartSkin->runAction(RepeatForever::create(_animatePartSkinAttack->clone()));

            if ( _animatePartSkinAttackEffect != nullptr )
            {
                _spritePartSkinAttackEffect->runAction(RepeatForever::create(_animatePartSkinAttackEffect->clone()));
                _spritePartSkinAttackEffect->setVisible(true);
            }
        }
         

        int level =  UserInfoManager::getInstance()->getRelicLevel(_ePlace, E_RELIC::RELIC_2);
        if ( level > 0 )
        {
            if ( _shadow != nullptr )
            {
                _shadow->setShow();
                _shadow->actionAttack();
            }
        }
    }
}

void UnitBehaviorHero::actionAttackContinue()
{
    if ( _enemy->isDie() == true )
    {
        this->heroStopAllAction();
        this->runAction(_onAttackCallback);
        return;
    }
    
    //
    if ( _bChangeParts )
    {
        //
        _bChangeParts = false;
        if ( _ePlace == E_PLACE::DG_DEVIL_DOM )
        {
            setMotionDevil();
        }
        else
        {
            setMotion();
        }
        
        
        //
        heroStopAllAction();
        actionAttack(*_enemy);
        return;
    }
    
    //
    _eState = E_HERO_STATE::ATTACK;
    
    // statck
    int monsterHitCount = UserInfoManager::getInstance()->getHitCount(_ePlace);
    UserInfoManager::getInstance()->setHitCount(_ePlace, monsterHitCount + 1);
    
    if ( _ePlace == E_PLACE::DG_NORMAL )
    {
        UserInfoStackManager::getInstance()->setTotalAttackCount(UserInfoStackManager::getInstance()->getTotalAttackCount()+1);
        
        if (CostumeManager::getInstance()->isEquip(E_COSTUME::CAPE,E_COSTUME_IDX::IDX_14))
        {
            UserInfoStackManager::getInstance()->addHeroHitCount();
        }
        
        if ( MafRemoteConfig::GetRemoteValueBool("enable_buff_recommend") == true )
        {
            //1. on/off 확인 //3. 현재 카운팅 확인 //2. 최대층수 확인 //4. 한 몬스터당 1번
            //*너무 자주 호출된다 생각되면 앞에서 isBuffRecommend 확인하고 조건에 어긋날때 setBuffRecommend 바꿔서 최적화 가능.
            if ( monsterHitCount > MafRemoteConfig::GetRemoteValueInt("buff_recommend_count") )
            {
                if ( _enemy->isBuffRecommend() == false )
                {
                    if ( UserInfoManager::getInstance()->getHighFloor(E_PLACE::DG_NORMAL) <= MafRemoteConfig::GetRemoteValueInt("buff_recommend_floor") )
                    {
                        _enemy->setBuffRecommend(true);
                        this->runAction(_onAttackCountCallback);
                    }
                }
            }
        }
    }
    else if ( _ePlace == E_PLACE::DG_PRISON )
    {
        if (CostumeManager::getInstance()->isEquip(E_COSTUME::CAPE,E_COSTUME_IDX::IDX_14))
        {
            UserInfoStackManager::getInstance()->addHeroHitCount();
        }
    }
    else if ( _ePlace == E_PLACE::DG_DEVIL_DOM )
    {
        UserInfoManager::getInstance()->setHitCountDevilFlash(UserInfoManager::getInstance()->getHitCountDevilFlash()+1);
    }
    
    //
    bool bCritical   = UserInfoManager::getInstance()->getCriticalRate(_ePlace, true, false);
    std::string strAtkDamage = UserInfoManager::getInstance()->getAttackDamage(_ePlace, E_CHARACTER::CHA_HERO, bCritical, false);
    
    //
    if(_bDeathBlow)
        strAtkDamage = getAttackDeathBlowDamage(strAtkDamage);
    
    //
    if (_ePlace == E_PLACE::DG_NORMAL || _ePlace == E_PLACE::DG_PRISON )
    {
        // 서리감옥 스킬
        strAtkDamage = getAttackSkillFrostDamage(strAtkDamage);
        
        if(_bShowWeaponAttackEffect)
            actionWeaponEffect(0);
        
        if ( _bShowSkinAttackEffect && UserInfoManager::getInstance()->getOneKill(_ePlace) == false )
            actionSkinEffect();
        
        if ( _ePlace == DG_NORMAL )//펫장비 데미지 효과, 일반 던전에만 적용
        {
            if ( _nTimePrincessBuff > 0 )
            {
                auto princessEquip = PetNewManager::getInstance()->getPetEquipEffect(400, 1);
                strAtkDamage = MafUtils::bigMulNum(strAtkDamage, MafUtils::doubleToString(princessEquip*100));
                strAtkDamage.pop_back();
                strAtkDamage.pop_back();
            }
            
            if ( UserInfoManager::getInstance()->getPetBuffPriencess(_ePlace) == E_PRINCESS_BUFF::BUFF_ALL && _nTimePrincessBuff > 0 )
            {
                auto princessEquip = PetNewManager::getInstance()->getPetEquipEffect(403, 1);
                strAtkDamage = MafUtils::bigMulNum(strAtkDamage, MafUtils::doubleToString(princessEquip*100));
                strAtkDamage.pop_back();
                strAtkDamage.pop_back();
            }
            
            int nCount = 10 + PetNewManager::getInstance()->getPetEvolutionEffect1(E_PET::HONG_6, PetNewManager::getInstance()->getPetEvolution(E_PET::HONG_6));
            if ( UserInfoManager::getInstance()->getHongHitCount(_ePlace) == nCount )
            {
                auto HongBuff = PetNewManager::getInstance()->getPetEquipEffect(601, 1);
                strAtkDamage = MafUtils::bigMulNum(strAtkDamage, MafUtils::doubleToString(HongBuff*100));
                strAtkDamage.pop_back();
                strAtkDamage.pop_back();
            }
        }
        
        int hitMask = E_CHARACTER_HIT::HIT_NORMAL;
        if ( _isDeathBlow == true )
        {
            hitMask = hitMask | E_CHARACTER_HIT::HIT_DEATHBLOW;
        }
        
        if ( _isFrostSkillCri == true )
        {
            hitMask = hitMask | E_CHARACTER_HIT::HIT_FROSTSKILL_CRI;
        }
        _enemy->actionHit(E_CHARACTER::CHA_HERO, hitMask, strAtkDamage, bCritical);
        
        //
        _isFrostSkillCri = false;
        _isDeathBlow = false;
            
        
        // 위험한 가면
        double per = DataManager::GetRelicEffect(E_RELIC::RELIC_2, UserInfoManager::getInstance()->getRelicLevel(_ePlace, E_RELIC::RELIC_2));
        if (per > 0)
        {
            if ( _ePlace == E_PLACE::DG_NORMAL && CostumeManager::getInstance()->isEquip(E_COSTUME::HEAD,E_COSTUME_IDX::IDX_27))
            {
                if ( UserInfoManager::getInstance()->getRelicLevel(_ePlace, E_RELIC::RELIC_2) >= DataManager::GetRelicMaxLevel(E_RELIC::RELIC_2))
                {
                    per += per * 6.0f;
                }
            }
            
            if ( CostumeManager::getInstance()->isEquip(E_COSTUME::HAND,E_COSTUME_IDX::IDX_33))
            {
                per += per * 5.0f;
            }
            
            if ( CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_23))
            {
                per *= 10.0f;
            }
            
            strAtkDamage = MafUtils::bigMulNum(strAtkDamage, MafUtils::doubleToString(per));
            strAtkDamage.pop_back();
            strAtkDamage.pop_back();
            _enemy->actionHit(E_CHARACTER::CHA_SHADOW, E_CHARACTER_HIT::HIT_NORMAL, strAtkDamage, bCritical);
        }
    }
    else
    {
        _enemy->actionHit(E_CHARACTER::CHA_HERO, E_CHARACTER_HIT::HIT_NORMAL, strAtkDamage, bCritical);
    }
    
    
    
    
    // 사운드
    if ( bCritical == true )
    {
        int numSound = random(0, 4);
        if ( numSound == 0 )        SoundManager::SoundEffectStart(SOUND_CRI_1);
        else if ( numSound == 1 )   SoundManager::SoundEffectStart(SOUND_CRI_2);
    }
    else
    {
        if ( _ePlace == E_PLACE::DG_PRISON && DataManager::isEventSummer() == true )
        {
            int numSound = random(0, 4);
            if ( numSound == 0 )    SoundManager::SoundEffectStart(SOUND_ATTACK_EVENT);
        }
        else
        {
            int numSound = random(1, 10);
            if(numSound==1)         SoundManager::SoundEffectStart(SOUND_ATTACK_1);
            else if(numSound==2)    SoundManager::SoundEffectStart(SOUND_ATTACK_2);
            else if(numSound==3)    SoundManager::SoundEffectStart(SOUND_ATTACK_3);
            else if(numSound==4)    SoundManager::SoundEffectStart(SOUND_ATTACK_4);
            else if(numSound==5)    SoundManager::SoundEffectStart(SOUND_ATTACK_5);
        }
    }
}

void UnitBehaviorHero::actionRun()
{
    //
    _eState = E_HERO_STATE::IDLE;
    
    //
    heroStopAllAction();
    if(_bChangeParts)
    {
        _bChangeParts = false;
        setMotion();
    }
    
    if ( _ePlace == E_PLACE::DG_DEVIL_DOM )
    {
        RepeatForever *repeat = nullptr;
        
        repeat = RepeatForever::create(_animatePartDefaultMove[E_HERO::RIDER]->clone());
        _spritePartDefault[E_HERO::RIDER]->runAction(repeat);
        
        repeat = RepeatForever::create(_animatePartDefaultMove[E_HERO::BODY]->clone());
        _spritePartDefault[E_HERO::BODY]->runAction(repeat);
        
        repeat = RepeatForever::create(_animatePartDefaultMove[E_HERO::HEAD]->clone());
        _spritePartDefault[E_HERO::HEAD]->runAction(repeat);
        
        repeat = RepeatForever::create(_animatePartDefaultMove[E_HERO::CAPE]->clone());
        _spritePartDefault[E_HERO::CAPE]->runAction(repeat);
        
        repeat = RepeatForever::create(_animatePartDefaultMove[E_HERO::WEAPON]->clone());
        _spritePartDefault[E_HERO::WEAPON]->runAction(repeat);
        
        if ( _bRider )
        {
            _spritePartDefault[E_HERO::RIDER]->setVisible(true);
            _spritePartDefault[E_HERO::WEAPON]->setVisible(false);
        }
        else
        {
            _spritePartDefault[E_HERO::RIDER]->setVisible(false);
            _spritePartDefault[E_HERO::WEAPON]->setVisible(true);
        }
    }
    else
    {
        if ( _eSkin == E_HERO_SKIN::DEFAULT || _eSkin == E_HERO_SKIN::DEFAULT_SUMMER )
        {// SKIN : DEFAULT, DEFAULT_EVENT
            RepeatForever *repeat = nullptr;
            
            repeat = RepeatForever::create(_animatePartDefaultMove[E_HERO::BODY]->clone());
            _spritePartDefault[E_HERO::BODY]->runAction(repeat);
            
            repeat = RepeatForever::create(_animatePartDefaultMove[E_HERO::HEAD]->clone());
            _spritePartDefault[E_HERO::HEAD]->runAction(repeat);
            
            repeat = RepeatForever::create(_animatePartDefaultMove[E_HERO::HAND]->clone());
            _spritePartDefault[E_HERO::HAND]->runAction(repeat);
            
            repeat = RepeatForever::create(_animatePartDefaultMove[E_HERO::FOOT]->clone());
            _spritePartDefault[E_HERO::FOOT]->runAction(repeat);
            
            repeat = RepeatForever::create(_animatePartDefaultMove[E_HERO::CAPE]->clone());
            _spritePartDefault[E_HERO::CAPE]->runAction(repeat);
            
            repeat = RepeatForever::create(_animatePartDefaultMove[E_HERO::WEAPON]->clone());
            _spritePartDefault[E_HERO::WEAPON]->runAction(repeat);
        }
        else if ( _eSkin == E_HERO_SKIN::HERO || _eSkin == E_HERO_SKIN::PREMIUM )
        {// SKIN : PREMIUM, HERO
            //
            _spritePartSkinMoveEffect->setVisible(false);
            _spritePartSkinAttackEffect->setVisible(false);
            
            //
            RepeatForever *repeat = nullptr;
            
            repeat = RepeatForever::create(_animatePartSkinMove->clone());
            _spritePartSkin->runAction(repeat);
            
            if ( _animatePartSkinMoveEffect != nullptr )
            {
                repeat = RepeatForever::create(_animatePartSkinMoveEffect->clone());
                _spritePartSkinMoveEffect->runAction(repeat);
                _spritePartSkinMoveEffect->setVisible(true);
            }
        }
        

        if ( _shadow != nullptr )
        {
            _shadow->setHide();
        }
    }
}

void UnitBehaviorHero::actionWeaponEffect(float x)
{
    int nAnimRand = random(0, (int)_vecAtkEffectAnimInfo.size()-1);
    int nInfoRand = random(0, (int)_vecAtkEffectInfo.size()-1);
    
    if ( _vecAtkEffectAnimInfo[nAnimRand].sprFrontDefault->isVisible())
    {
        return;
    }
    
    Vec2 enemySize = Vec2(_enemy->getMonsterSprite()->getContentSize());
    enemySize *= _enemy->getMonsterSprite()->getScale();
    Vec2 enemyPos;
    if(_ePlace == E_PLACE::DG_PRISON)
        enemyPos = _enemy->getPosition() + Vec2::LEFT * 250;
    else
        enemyPos = _layerCharacter->convertToNodeSpace(_enemy->getPosition());
    
    float xPos = random(enemyPos.x , enemyPos.x + enemySize.x);
    float yPos = random(enemyPos.y , enemyPos.y + enemySize.y);
    
    _vecAtkEffectAnimInfo[nAnimRand].sprBackDefault->setPosition(Vec2(xPos, yPos));
    _vecAtkEffectAnimInfo[nAnimRand].sprFrontDefault->setPosition(Vec2(xPos, yPos));
    _vecAtkEffectAnimInfo[nAnimRand].sprBackDefault->setColor(_vecAtkEffectInfo[nInfoRand].color);
    
    auto fSeq = Sequence::create(Show::create(), _vecAtkEffectAnimInfo[nAnimRand].fAnim->clone(), Hide::create(),nullptr);
    auto bSeq = Sequence::create(Show::create(), _vecAtkEffectAnimInfo[nAnimRand].bAnim->clone(), Hide::create(),nullptr);
    _vecAtkEffectAnimInfo[nAnimRand].sprFrontDefault->runAction(fSeq);
    _vecAtkEffectAnimInfo[nAnimRand].sprBackDefault->runAction(bSeq);
    
}

void UnitBehaviorHero::actionSkinEffect()
{
    int idx = CostumeSkinManager::getInstance()->getCostumeSkinEquip();
    
    AttackEffectManager* effectMgr = AttackEffectManager::getInstance();
    
    if(effectMgr->hasSkinEffect(idx) == false)
        return;
    
    Vec2 enemySize = Vec2(_enemy->getMonsterSprite()->getContentSize());
    enemySize *= _enemy->getMonsterSprite()->getScale();
    
    Vec2 enemyPos = _enemy->convertToWorldSpace(_enemy->getMonsterHPSprite()->getPosition());
    enemyPos += Vec2::DOWN * 10;
    
    effectMgr->playSkinEffect(idx, enemyPos);
    
}

void UnitBehaviorHero::heroStopAllAction()
{
    this->stopAllActions();
       
    if ( _ePlace == E_PLACE::DG_DEVIL_DOM )
    {
        if ( _spritePartDefault[E_HERO::RIDER]->getNumberOfRunningActions() != 0 )  _spritePartDefault[E_HERO::RIDER]->stopAllActions();
        if ( _spritePartDefault[E_HERO::BODY]->getNumberOfRunningActions() != 0 )   _spritePartDefault[E_HERO::BODY]->stopAllActions();
        if ( _spritePartDefault[E_HERO::HEAD]->getNumberOfRunningActions() != 0 )   _spritePartDefault[E_HERO::HEAD]->stopAllActions();
        if ( _spritePartDefault[E_HERO::CAPE]->getNumberOfRunningActions() != 0 )   _spritePartDefault[E_HERO::CAPE]->stopAllActions();
        if ( _spritePartDefault[E_HERO::WEAPON]->getNumberOfRunningActions() != 0 ) _spritePartDefault[E_HERO::WEAPON]->stopAllActions();
    }
    else
    {
        if ( _spritePartDefault[E_HERO::BODY]->getNumberOfRunningActions() != 0 )       _spritePartDefault[E_HERO::BODY]->stopAllActions();
        if ( _spritePartDefault[E_HERO::HEAD]->getNumberOfRunningActions() != 0 )       _spritePartDefault[E_HERO::HEAD]->stopAllActions();
        if ( _spritePartDefault[E_HERO::HAND]->getNumberOfRunningActions() != 0 )       _spritePartDefault[E_HERO::HAND]->stopAllActions();
        if ( _spritePartDefault[E_HERO::FOOT]->getNumberOfRunningActions() != 0 )       _spritePartDefault[E_HERO::FOOT]->stopAllActions();
        if ( _spritePartDefault[E_HERO::CAPE]->getNumberOfRunningActions() != 0 )       _spritePartDefault[E_HERO::CAPE]->stopAllActions();
        if ( _spritePartDefault[E_HERO::WEAPON]->getNumberOfRunningActions() != 0 )     _spritePartDefault[E_HERO::WEAPON]->stopAllActions();

        if ( _spritePartSkin->getNumberOfRunningActions() != 0 )                _spritePartSkin->stopAllActions();
        if ( _spritePartSkinMoveEffect->getNumberOfRunningActions() != 0 )      _spritePartSkinMoveEffect->stopAllActions();
        if ( _spritePartSkinAttackEffect->getNumberOfRunningActions() != 0 )    _spritePartSkinAttackEffect->stopAllActions();
    }
}

void UnitBehaviorHero::showItemFeverFx()
{
    if(_spriteItemFeverFx == nullptr)
    {
        _spriteItemFeverFx = Sprite::create("Assets/ui/fever/fx_fever1_1.png");
        _spriteItemFeverFx->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        _spriteItemFeverFx->setPosition(_layerCharacter->getContentSize().width/2,
                                        _layerCharacter->getContentSize().height/2 - 25);
        _layerCharacter->addChild(_spriteItemFeverFx, -1);
    }
    _spriteItemFeverFx->setVisible(true);
    
    auto animation = Animation::create();
    animation->setDelayPerUnit(0.1f);
    for ( int i = 1; i <= 6; i++ )
    {
        auto spriteAni = Sprite::create(MafUtils::format("Assets/ui/fever/fx_fever1_%d.png",i).c_str());
        spriteAni->getTexture()->setTexParameters(texParams);
        animation->addSpriteFrame(spriteAni->getSpriteFrame());
    }
    
    for(auto child : _layerCharacter->getChildren())
    {
        auto pet = dynamic_cast<Pet*>(child);
        if(pet != nullptr)
        {
            child->setLocalZOrder(-2);
        }
    }
    
    _spriteItemFeverFx->runAction(RepeatForever::create(Animate::create(animation)));
}

void UnitBehaviorHero::hideItemFeverFx()
{
    _spriteItemFeverFx->stopAllActions();
    _spriteItemFeverFx->setVisible(false);
    
    for(auto child : _layerCharacter->getChildren())
    {
        auto pet = dynamic_cast<Pet*>(child);
        if(pet != nullptr)
        {
            pet->setLocalZOrder(pet->getZorderNum());
        }
    }
}

void UnitBehaviorHero::actionAttackGavel()
{
    std::string myAtk = "0";
    
    int levelRelic = UserInfoManager::getInstance()->getRelicLevel(E_PLACE::DG_NORMAL, E_RELIC::RELIC_4);
    if ( levelRelic <= 0 )
    {
        return;
    }
    
    int randAttack = random(0, 1);
    if ( randAttack == 0 )
    {
        // 봉식이 치명타 공격력 적용 (50%)
        myAtk = UserInfoManager::getInstance()->getAttackDamage(_ePlace,E_CHARACTER::CHA_HERO, true, false);
    }
    else
    {
        // 크르르 낙뢰 공격력 적용 (50%)
        myAtk = UserInfoManager::getInstance()->getAttackDamage(_ePlace,E_CHARACTER::CHA_PET_CHICK, true, false);
        
        int per = 8;
        if ( CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_2) )
        {
            per *= 10;
        }
        
        int evolutionLevel = PetNewManager::getInstance()->getPetEvolution(E_PET::CHICK_1);
        double evolutionPer = PetNewManager::getInstance()->getPetEvolutionEffect1(E_PET::CHICK_1, evolutionLevel);
        if ( evolutionPer > 0 )
        {
            per *= evolutionPer;
        }
        
        if ( CostumeManager::getInstance()->isEquip(E_COSTUME::BODY,E_COSTUME_IDX::IDX_26) )
        {
            int petLevel = PetNewManager::getInstance()->getPetLevel(E_PET::CHICK_1);
            per += per * (petLevel * 0.2f);
        }
        
        per *= PetNewManager::getInstance()->getPetEquipEffect(103, 1);
        
        myAtk = MafUtils::bigMulNum(myAtk, MafUtils::doubleToString(per * 100));
        myAtk.pop_back();
        myAtk.pop_back();
    }
    
    double perRelic = DataManager::GetRelicEffect(E_RELIC::RELIC_4, levelRelic);
    myAtk = MafUtils::bigMulNum(myAtk, MafUtils::doubleToString(perRelic));
    myAtk.pop_back();
    myAtk.pop_back();
    
    
    
    //
    auto spriteEffect = Sprite::create("Assets/character_hero/normal/effect/ef_relic_4_01.png");
    spriteEffect->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    spriteEffect->setPosition(_layerCharacter->getContentSize().width + 150, 0);
    _layerCharacter->addChild(spriteEffect, 20);
    
    auto animation = Animation::create();
    animation->setDelayPerUnit(0.1f);
    for ( int i = 1; i <= 12; i++ )
    {
        auto spriteAni = Sprite::create(MafUtils::format("Assets/character_hero/normal/effect/ef_relic_4_%02d.png",i).c_str());
        spriteAni->getTexture()->setTexParameters(texParams);
        animation->addSpriteFrame(spriteAni->getSpriteFrame());
    }
    
    auto action1 = DelayTime::create(1.0f);
    auto action2 = CallFunc::create([=](){
        
        _enemy->actionHit(E_CHARACTER::CHA_GAVEL, E_CHARACTER_HIT::HIT_NORMAL, myAtk, false);
        
    });
    auto action3 = RemoveSelf::create();
    auto seq = Sequence::create(action1, action2, action3, NULL);
    spriteEffect->runAction(Animate::create(animation));
    spriteEffect->runAction(seq);
}
