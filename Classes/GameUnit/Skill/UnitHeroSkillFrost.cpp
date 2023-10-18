//
//  Model.cpp
//  HexagonSnake
//
//  Created by SanHeo on 2014. 7. 27..
//
//

#include "UnitHeroSkillFrost.h"

#include "Common/Observer/IBattleInfo.h"

#include "ManagerGame/UserInfoManager.h"

#include "ManagerGame/PrisonFrostManager.h"
#include "ManagerGame/PetNewManager.h"

USING_NS_CC;

UnitHeroSkillFrost* UnitHeroSkillFrost::create()
{
    UnitHeroSkillFrost *pRet = new (std::nothrow) UnitHeroSkillFrost();
    if ( pRet )
    {
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

UnitHeroSkillFrost::UnitHeroSkillFrost() :

_battle(nullptr),

_skillCasting(false),
_skillIdx(0),

_animatePartDefaultAttack(nullptr),
_spritePartDefault(nullptr)
{

}

UnitHeroSkillFrost::~UnitHeroSkillFrost()
{
    
}

bool UnitHeroSkillFrost::init()
{
    if ( !LayerColor::initWithColor(Color4B(0,0,0, 0)))
    {
        return false;
    }
    
    setContentSize(Size(getContentSize().width, 600));
    setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    
    
    // update
    scheduleUpdate();
    
    return true;
} 
 
void UnitHeroSkillFrost::update(float dt)
{
    // coolTime 감소
    int64_t hitCount = UserInfoManager::getInstance()->getHitCount(E_PLACE::DG_NORMAL);
    if ( hitCount >= 50 )
    {
        PrisonFrostManager::getInstance()->update(dt);
    }
    
    if ( _skillCasting == true )
    {
        return;
    }
    
    // active
    auto listSkill = PrisonFrostManager::getInstance()->getListSkill();
    
    // sort
    std::sort(listSkill.begin(), listSkill.end(), [](InfoFrostSkill *a, InfoFrostSkill *b) -> bool{
        double coolTimeA = PrisonFrostManager::getInstance()->getSkillCoolTime(a->getIdx());
        double coolTimeB = PrisonFrostManager::getInstance()->getSkillCoolTime(b->getIdx());
        return coolTimeA < coolTimeB;
    });
    
    for ( auto& objSkill : listSkill )
    {
        int idx = objSkill->getIdx();
        std::string type = objSkill->getType();
        std::transform(type.begin(), type.end(), type.begin(), tolower);
        
        int step = PrisonFrostManager::getInstance()->getSkillStep(idx);
        if ( step == -1 )
        {
            continue;
        }
        
        if ( type.compare("active") != 0 )
        {
            continue;
        }
        
        if ( _skillCasting == true )
        {
            continue;
        }
        
        double coolTime = PrisonFrostManager::getInstance()->getSkillCoolTime(idx);
        if ( coolTime <= 0 )
        {
            _skillCasting = true;
            _skillIdx = idx;
            PrisonFrostManager::getInstance()->setSkillUse(idx);
            PrisonFrostManager::getInstance()->setSkillCoolTime(idx, objSkill->getCoolTime());
            
            switch (idx) {
                case 50004: skill50004(); break;
                case 50005: skill50005(); break;
                case 50006: skill50006(); break;
                    
                default:
                    break;
            }
            break;
        }
    }
}

void UnitHeroSkillFrost::setBattleInfo(IBattleInfo* battle)
{
    _battle = battle;
}

cocos2d::Animate* UnitHeroSkillFrost::getSkillAnimate(std::string path, double delayPerUnit)
{
    Vector<cocos2d::SpriteFrame*> list;
    
    int count = 0;
    
    for ( int i = 1; i <= 100; i++ )
    {
        std::string strTempPath = MafUtils::format(path.c_str(), i);
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
    
    if ( list.size() <= 0 )
    {
        return nullptr;
    }
    else
    {
        auto animation = Animation::createWithSpriteFrames(list, delayPerUnit);
        auto animate = Animate::create(animation);
        return animate;
    }
}

void UnitHeroSkillFrost::skill50004()
{
    cocos2d::Animate* animateCut = getSkillAnimate("Assets/ui/frost_prison/skill/skill_buff_cutscene%02d.png", 0.1);
    
    auto action1 = animateCut;
    auto action2 = DelayTime::create(0.25f);
    auto action3 = CallFunc::create([=](){
        
        int idx = 50004;
        
        int step = PrisonFrostManager::getInstance()->getSkillStep(idx);
        
        auto objSkill = PrisonFrostManager::getInstance()->getInfoSkill(idx, step);
        if ( objSkill != nullptr )
        {
            PrisonFrostManager::getInstance()->setSkillDuration(idx, objSkill->getDuration());
        }
    });
    auto actionFinish = CallFunc::create(CC_CALLBACK_0(UnitHeroSkillFrost::skillFinish, this));
    auto seq = Sequence::create(action1, action2, action3, actionFinish, RemoveSelf::create(), NULL);
    
    auto skill = Sprite::create();
    skill->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    skill->setPosition(Vec2::ZERO);
    skill->setScale(3);
    skill->getTexture()->setTexParameters(texParams);
    skill->runAction(seq);
    addChild(skill);
}

void UnitHeroSkillFrost::skill50005()
{
    cocos2d::Animate* animateCut = getSkillAnimate("Assets/ui/frost_prison/skill/skill1_cutscene%02d.png", 0.1);
    cocos2d::Animate* animateActionBefore = getSkillAnimate("Assets/ui/frost_prison/skill/skill1_effect1_%02d.png", 0.1);
    cocos2d::Animate* animateActionAfter = getSkillAnimate("Assets/ui/frost_prison/skill/skill1_effect2_%02d.png", 0.1);
    
    auto action1 = animateCut;
    auto action2 = DelayTime::create(0.25f);
    auto action3 = animateActionBefore;
    auto action4 = CallFunc::create([=](){
        
        int idx = 50005;
        int level = PrisonFrostManager::getInstance()->getSkillLevel(idx);
        
        double effect = PrisonFrostManager::getInstance()->getSkillEffect(idx, level);
        if ( effect > 0 )
        {
            bool bCritical   = UserInfoManager::getInstance()->getCriticalRate(E_PLACE::DG_NORMAL, true, false);
            std::string damage = UserInfoManager::getInstance()->getAttackDamage(E_PLACE::DG_NORMAL, E_CHARACTER::CHA_HERO, bCritical, false);
            
            // 펫 장비
            {
                double timePrincessBuff = _battle->getUnitHero()->getPrincessBuffTime();
                if ( timePrincessBuff > 0 )
                {
                    auto princessEquip = PetNewManager::getInstance()->getPetEquipEffect(400, 1);
                    damage = MafUtils::bigMulNum(damage, MafUtils::doubleToString(princessEquip*100));
                    damage.pop_back();
                    damage.pop_back();
                }
                
                if ( UserInfoManager::getInstance()->getPetBuffPriencess(E_PLACE::DG_NORMAL) == E_PRINCESS_BUFF::BUFF_ALL && timePrincessBuff > 0 )
                {
                    auto princessEquip = PetNewManager::getInstance()->getPetEquipEffect(403, 1);
                    damage = MafUtils::bigMulNum(damage, MafUtils::doubleToString(princessEquip*100));
                    damage.pop_back();
                    damage.pop_back();
                }
                
                int nCount = 10 + PetNewManager::getInstance()->getPetEvolutionEffect1(E_PET::HONG_6, PetNewManager::getInstance()->getPetEvolution(E_PET::HONG_6));
                if ( UserInfoManager::getInstance()->getHongHitCount(E_PLACE::DG_NORMAL) == nCount )
                {
                    auto HongBuff = PetNewManager::getInstance()->getPetEquipEffect(601, 1);
                    damage = MafUtils::bigMulNum(damage, MafUtils::doubleToString(HongBuff*100));
                    damage.pop_back();
                    damage.pop_back();
                }
            }
            
            //
            std::string temp = MafUtils::bigMulNum(damage, MafUtils::doubleToString(effect));
            temp.pop_back();
            temp.pop_back();
            damage = MafUtils::bigAddNum(damage, temp);
            
            int hitMask = E_CHARACTER_HIT::HIT_NORMAL;
            hitMask = hitMask | E_CHARACTER_HIT::HIT_FROSTSKILL_50005;
            
            auto enemy = _battle->getUnitMonster();
            enemy->actionHit(E_CHARACTER::CHA_HERO, hitMask, damage, bCritical);
            
            //
            auto sceneLayer = UtilsScene::getInstance()->getRunningLayer();
            sceneLayer->runAction(Shake::create(0.2, 5));
        }
        
        //
        SoundManager::SoundEffectStart(SOUND_FROST_ATTACK_SKILL_20005);
    });
    auto action5 = animateActionAfter;
    auto actionFinish = CallFunc::create(CC_CALLBACK_0(UnitHeroSkillFrost::skillFinish, this));
    auto seq = Sequence::create(action1, action2, action3, action4, action5, actionFinish, RemoveSelf::create(), NULL);
    
    auto skill = Sprite::create();
    skill->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    skill->setPosition(Vec2::ZERO);
    skill->setScale(3);
    skill->getTexture()->setTexParameters(texParams);
    skill->runAction(seq);
    addChild(skill);
}

void UnitHeroSkillFrost::skill50006()
{
    cocos2d::Animate* animateCut = getSkillAnimate("Assets/ui/frost_prison/skill/skill2_cutscene%02d.png", 0.1);
    cocos2d::Animate* animateActionBefore = getSkillAnimate("Assets/ui/frost_prison/skill/skill2_effect1_%02d.png", 0.1);
    cocos2d::Animate* animateActionAfter = getSkillAnimate("Assets/ui/frost_prison/skill/skill2_effect2_%02d.png", 0.1);
    auto monster = _battle->getUnitMonster();
    
    auto sizeMonster = monster->getLayerCharacter()->getBoundingBox().size;
    auto posMonster = monster->getPosition();
    
    auto action1_1 = animateCut;
    auto action1_2 = DelayTime::create(0.25f);
    auto seq1 = Sequence::create(action1_1, action1_2, RemoveSelf::create(), NULL);
    
    auto skill1 = Sprite::create();
    skill1->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    skill1->setPosition(Vec2::ZERO);
    skill1->setScale(3);
    skill1->getTexture()->setTexParameters(texParams);
    skill1->runAction(seq1);
    addChild(skill1);
    
    auto action2_1 = DelayTime::create(seq1->getDuration());
    auto action2_2 = animateActionBefore;
    auto action2_3 = CallFunc::create([=](){
        
        int idx = 50006;
        int level = PrisonFrostManager::getInstance()->getSkillLevel(idx);
        
        double effect = PrisonFrostManager::getInstance()->getSkillEffect(idx, level);
        if ( effect > 0 )
        {
            bool bCritical   = UserInfoManager::getInstance()->getCriticalRate(E_PLACE::DG_NORMAL, true, false);
            std::string damage = UserInfoManager::getInstance()->getAttackDamage(E_PLACE::DG_NORMAL, E_CHARACTER::CHA_HERO, bCritical, false);
            
            // 펫 장비
            {
                double timePrincessBuff = _battle->getUnitHero()->getPrincessBuffTime();
                if ( timePrincessBuff > 0 )
                {
                    auto princessEquip = PetNewManager::getInstance()->getPetEquipEffect(400, 1);
                    damage = MafUtils::bigMulNum(damage, MafUtils::doubleToString(princessEquip*100));
                    damage.pop_back();
                    damage.pop_back();
                }
                
                if ( UserInfoManager::getInstance()->getPetBuffPriencess(E_PLACE::DG_NORMAL) == E_PRINCESS_BUFF::BUFF_ALL && timePrincessBuff > 0 )
                {
                    auto princessEquip = PetNewManager::getInstance()->getPetEquipEffect(403, 1);
                    damage = MafUtils::bigMulNum(damage, MafUtils::doubleToString(princessEquip*100));
                    damage.pop_back();
                    damage.pop_back();
                }
                
                int nCount = 10 + PetNewManager::getInstance()->getPetEvolutionEffect1(E_PET::HONG_6, PetNewManager::getInstance()->getPetEvolution(E_PET::HONG_6));
                if ( UserInfoManager::getInstance()->getHongHitCount(E_PLACE::DG_NORMAL) == nCount )
                {
                    auto HongBuff = PetNewManager::getInstance()->getPetEquipEffect(601, 1);
                    damage = MafUtils::bigMulNum(damage, MafUtils::doubleToString(HongBuff*100));
                    damage.pop_back();
                    damage.pop_back();
                }
            }
            
            //
            std::string temp = MafUtils::bigMulNum(damage, MafUtils::doubleToString(effect));
            temp.pop_back();
            temp.pop_back();
            damage = MafUtils::bigAddNum(damage, temp);
            
            int hitMask = E_CHARACTER_HIT::HIT_NORMAL;
            hitMask = hitMask | E_CHARACTER_HIT::HIT_FROSTSKILL_50006;
            
            auto enemy = _battle->getUnitMonster();
            enemy->actionHit(E_CHARACTER::CHA_HERO, hitMask, damage, bCritical);
            
            //
            auto sceneLayer = UtilsScene::getInstance()->getRunningLayer();
            sceneLayer->runAction(Shake::create(0.2, 5));
        }
        
        //
        SoundManager::SoundEffectStart(SOUND_FROST_ATTACK_SKILL_20006);
    });
    auto action2_4 = animateActionAfter;
    auto actionFinish = CallFunc::create(CC_CALLBACK_0(UnitHeroSkillFrost::skillFinish, this));
    auto seq2 = Sequence::create(action2_1, action2_2, action2_3, action2_4, actionFinish, RemoveSelf::create(), NULL);
    
    auto skill2 = Sprite::create();
    skill2->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    skill2->setPosition(Vec2(posMonster.x + sizeMonster.width / 2, -120));
    skill2->setScale(3);
    skill2->getTexture()->setTexParameters(texParams);
    skill2->runAction(seq2);
    addChild(skill2);
}

void UnitHeroSkillFrost::skillFinish()
{
    _skillCasting = false;
    _skillIdx = 0;
    PrisonFrostManager::getInstance()->setSkillUse(0);
}
