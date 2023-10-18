//
//  Model.cpp
//  HexagonSnake
//
//  Created by SanHeo on 2014. 7. 27..
//
//

#include "UnitBehaviorMonsterFrost.h"

#include "GameUnit/Animation/UnitAnimationMonsterFrost.h"

USING_NS_CC;

UnitBehaviorMonsterFrost* UnitBehaviorMonsterFrost::create(E_PLACE ePlace)
{
    UnitBehaviorMonsterFrost *pRet = new (std::nothrow) UnitBehaviorMonsterFrost();
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

UnitBehaviorMonsterFrost::UnitBehaviorMonsterFrost() :
_eState(E_STATE::IDLE)
{
    
}

UnitBehaviorMonsterFrost::~UnitBehaviorMonsterFrost()
{

}

bool UnitBehaviorMonsterFrost::init()
{
    if ( !UnitBehavior::init())
    {
        return false;
    }
    
    //
    _unitMonster = UnitAnimationMonsterFrost::create();
    _unitMonster->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _unitMonster->setPosition(Vec2::ZERO);
    addChild(_unitMonster);
    
    //
    auto size = _unitMonster->getContentSize();
    setContentSize(size);
    setIgnoreAnchorPointForPosition(false);
    setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    
    // attack effect
    {
        for ( int i = 1; i <= 3; i++ )
        {
            auto animationFront = Animation::create();
            animationFront->setDelayPerUnit(0.05);
            
            auto animationBack = Animation::create();
            animationBack->setDelayPerUnit(0.05);
            
            for ( int j = 1; j <= 4; j++ )
            {
                auto spriteAniFront = Sprite::create(MafUtils::format("Assets/character_hero/normal/effect_hit/ef_hit_%d_%d_%d.png", i, 1, j).c_str());
                spriteAniFront->getTexture()->setTexParameters(texParams);
                animationFront->addSpriteFrame(spriteAniFront->getSpriteFrame());
                
                auto spriteAniBack = Sprite::create(MafUtils::format("Assets/character_hero/normal/effect_hit/ef_hit_%d_%d_%d.png", i, 2, j).c_str());
                spriteAniBack->getTexture()->setTexParameters(texParams);
                animationBack->addSpriteFrame(spriteAniBack->getSpriteFrame());
            }
            _listAnimateAttackEffectFront.insert(i, Animate::create(animationFront));
            _listAnimateAttackEffectBack.insert(i, Animate::create(animationBack));
            
            //
            auto spriteFront = Sprite::create();
            spriteFront->setVisible(false);
            addChild(spriteFront);
            _listSpriteAttackEffectFront.insert(i, spriteFront);
            
            auto spriteBack = Sprite::create();
            spriteBack->setVisible(false);
            addChild(spriteBack);
            _listSpriteAttackEffectBack.insert(i, spriteBack);
            
            if ( i == 1 || i == 3 )
            {
                spriteFront->setScale(0.7);
                spriteBack->setScale(0.7);
            }
        }
    }
    
    return true;
}

void UnitBehaviorMonsterFrost::onEnter()
{
    UnitBehavior::onEnter();
    
    //
    onIdle();
}
  
#pragma mark -
UnitBehaviorMonsterFrost::E_STATE UnitBehaviorMonsterFrost::getState()
{
    return _eState;
}

#pragma mark - event
void UnitBehaviorMonsterFrost::onIdle()
{
    _eState = E_STATE::IDLE;
    _unitMonster->animationIdle();
}

void UnitBehaviorMonsterFrost::onAngry(const std::function<void(void)>& callbackFinish)
{
    _eState = E_STATE::ANGRY;
    _unitMonster->animationAngry(CallFunc::create([=](){
        
        if ( callbackFinish != nullptr )
        {
            callbackFinish();
        }
    }));
}

void UnitBehaviorMonsterFrost::onWalk()
{
    _eState = E_STATE::WALK;
    _unitMonster->animationAngryWalk();
}

void UnitBehaviorMonsterFrost::onDamage(std::string damage, bool critical, int hitMask)
{
    auto size = getContentSize();
    
    _unitMonster->stopAllActions();
    _unitMonster->setPosition(Vec2::ZERO);
    _unitMonster->runAction(Shake::create(0.2, 4));
    
    //
    MafUtils::convertNumberToShort(damage);
    
    //
    auto labelDamage = ui::CText::create(damage, GAME_FONT_AL, 36);
    labelDamage->setAnchorPoint(Vec2(0.5,0));
    labelDamage->setPosition(Vec2(size.width / 2 + random(-10, 10), 100));
    labelDamage->setOpacity(0);
    labelDamage->setScale(0.8);
    addChild(labelDamage);
    
    //
    if ( critical == false )
    {
        labelDamage->setColor(Color3B(255,204,65));
    }
    else
    {
        labelDamage->setColor(Color3B(255,99,87));
        labelDamage->setFontSize(48);
    }
    
    if ( (hitMask&E_CHARACTER_HIT::HIT_FROSTSKILL_CRI) == E_CHARACTER_HIT::HIT_FROSTSKILL_CRI )
    {
        if ( (hitMask&E_CHARACTER_HIT::HIT_FROSTSKILL_CRI) == E_CHARACTER_HIT::HIT_FROSTSKILL_CRI )
        {
            labelDamage->setColor(Color3B(255,99,87));
            labelDamage->setFontSize(48);
        }
        
        if ( (hitMask&E_CHARACTER_HIT::HIT_FROSTSKILL_CRI) == E_CHARACTER_HIT::HIT_FROSTSKILL_CRI )
        {
            auto uiIcon = Sprite::create("Assets/ui/frost_prison/critical_icon.png");
            uiIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
            uiIcon->setPosition(Vec2(0, labelDamage->getContentSize().height / 2));
            uiIcon->setName("UI_ICON");
            uiIcon->getTexture()->setTexParameters(texParams);
            labelDamage->addChild(uiIcon);
        }
    }
    
    //
    if ( critical == false )
    {
        auto action1_1 = FadeIn::create(0.4);
        auto action1_2 = DelayTime::create(0.2);
        auto action1_3 = FadeOut::create(0.1);
        auto seq1 = Sequence::create(action1_1, action1_2, action1_3, NULL);
        
        auto action2_1 = ScaleTo::create(0.4, 1.2);
        auto action2_2 = DelayTime::create(0.2);
        auto action2_3 = ScaleTo::create(0.1, 0.8);
        auto seq2 = Sequence::create(action2_1, action2_2, action2_3, NULL);
        
        auto action3_1 = MoveBy::create(0.4, Vec2(0, 30));
        auto action3_2 = DelayTime::create(0.2);
        auto action3_3 = MoveBy::create(0.1, Vec2(0, 10));
        auto seq3 = Sequence::create(action3_1, action3_2, action3_3, RemoveSelf::create(), NULL);
        
        auto spawn = Spawn::create(seq1, seq2, seq3, NULL);
        labelDamage->runAction(spawn);
    }
    else
    {
        auto action1_1 = FadeIn::create(0.4);
        auto action1_2 = DelayTime::create(0.2);
        auto action1_3 = FadeOut::create(0.1);
        auto seq1 = Sequence::create(action1_1, action1_2, action1_3, NULL);
        
        auto action2_1 = ScaleTo::create(0.4, 1.4);
        auto action2_2 = DelayTime::create(0.2);
        auto action2_3 = ScaleTo::create(0.1, 0.8);
        auto seq2 = Sequence::create(action2_1, action2_2, action2_3, NULL);
        
        auto action3_1 = MoveBy::create(0.4, Vec2(0, 40));
        auto action3_2 = DelayTime::create(0.2);
        auto action3_3 = MoveBy::create(0.1, Vec2(0, 10));
        auto seq3 = Sequence::create(action3_1, action3_2, action3_3, RemoveSelf::create(), NULL);
        
        auto spawn = Spawn::create(seq1, seq2, seq3, NULL);
        labelDamage->runAction(spawn);
    }
    
    //
    animationAttackEffect();
}

void UnitBehaviorMonsterFrost::animationAttackEffect()
{
    ssize_t randIdx = RandomHelper::random_int<int>(0, static_cast<int>(_listAnimateAttackEffectFront.size()) - 1);
    
    auto iterAnimateFront   = _listAnimateAttackEffectFront.begin();
    auto iterAnimateBack    = _listAnimateAttackEffectBack.begin();
    auto iterSpriteFront    = _listSpriteAttackEffectFront.begin();
    auto iterSpriteBack     = _listSpriteAttackEffectBack.begin();
    
    std::advance(iterAnimateFront, randIdx);
    std::advance(iterAnimateBack, randIdx);
    std::advance(iterSpriteFront, randIdx);
    std::advance(iterSpriteBack, randIdx);
    
    cocos2d::Animate* animateFront = iterAnimateFront->second;
    cocos2d::Animate* animateBack = iterAnimateBack->second;
    cocos2d::Sprite* spriteFront = iterSpriteFront->second;
    cocos2d::Sprite* spriteBack = iterSpriteBack->second;
    
    if ( animateFront == nullptr || animateBack == nullptr || spriteFront == nullptr || spriteBack == nullptr )
    {
        return;
    }
    
    if ( _unitMonster == nullptr )
    {
        return;
    }
    
    if ( spriteFront->isVisible() == true )
    {
        return;
    }
    
    Vec2 effectPos = Vec2(getContentSize().width / 2, 0);
     
    float xPos = random(effectPos.x - 75, effectPos.x + 75);
    float yPos = random(effectPos.y - 0, effectPos.y + 150);
    
    
    
    Color3B color = Color3B(255,120,0);
    int colorRand = random(0, 4);
    switch (colorRand) {
        case 0: color = Color3B(255,120,0); break;
        case 1: color = Color3B(255,214,48); break;
        case 2: color = Color3B(253,48,255); break;
        case 3: color = Color3B(0,255,36); break;
        case 4: color = Color3B(0,204,255); break;
            
        default:
            break;
    }
    
    spriteFront->setPosition(xPos, yPos);
    spriteBack->setPosition(xPos, yPos);
    spriteBack->setColor(color);
    
    
    auto fSeq = Sequence::create(Show::create(), animateFront->clone(), Hide::create(),nullptr);
    auto bSeq = Sequence::create(Show::create(), animateBack->clone(), Hide::create(),nullptr);
    spriteFront->runAction(fSeq);
    spriteBack->runAction(bSeq);
}
