//
//  Model.cpp
//  HexagonSnake
//
//  Created by SanHeo on 2014. 7. 27..
//
//

#include "UnitBehaviorMonster.h"

#include "ManagerGame/UserInfoManager.h"
#include "ManagerGame/UserInfoStackManager.h"
#include "ManagerGame/UserInfoConfigManager.h"
#include "ManagerGame/PetNewManager.h"
#include "ManagerGame/SaveManager.h"
#include "ManagerGame/DataManager.h"
#include "ManagerGame/CostumeManager.h"
#include "ManagerGame/PrisonManager.h"
#include "ManagerGame/ItemsMoneyManager.h"
#include "ManagerGame/MonsterManager.h"

USING_NS_CC;

UnitBehaviorMonster* UnitBehaviorMonster::create(E_PLACE ePlace)
{
    UnitBehaviorMonster *pRet = new (std::nothrow) UnitBehaviorMonster();
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

UnitBehaviorMonster::UnitBehaviorMonster() :
_eBoss(E_BOSS::NORMAL),
_nIdx(1),

_bDie(false),
_bBuffRecommend(false),

_hp("0"),
_hpMax("0"),

_spritePartDefault(nullptr),
_progressHpBG(nullptr),
_progressHpBar(nullptr),
_progressHpLabel(nullptr)
{
    
}

UnitBehaviorMonster::~UnitBehaviorMonster()
{
    
}

bool UnitBehaviorMonster::init()
{
    if ( !UnitBehavior::init() )
    {
        return false;
    }
    
    //
    initPart();
    
    return true;
}

void UnitBehaviorMonster::initPart()
{
    _layerCharacter = Layer::create();
    _layerCharacter->setContentSize(Size(232, 232));
    _layerCharacter->setIgnoreAnchorPointForPosition(false);
    _layerCharacter->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _layerCharacter->setPosition(Vec2::ZERO);
    addChild(_layerCharacter);
    
    // SKIN : DEFAULT
    _spritePartDefault = Sprite::create();
    _spritePartDefault->setAnchorPoint(Vec2::ZERO);
    _spritePartDefault->setPosition(Vec2::ZERO);
    _layerCharacter->addChild(_spritePartDefault);
    
    //
    {
        _progressHpBG = Sprite::create("Assets/battle/hp_bg.png");
        _progressHpBG->setAnchorPoint(Vec2(0.5,0));
        _layerCharacter->addChild(_progressHpBG);
        
        
        _progressHpBar = ProgressTimer::create(Sprite::create("Assets/battle/hp_hp.png"));
        _progressHpBar->setType(ProgressTimer::Type::BAR);
        _progressHpBar->setPercentage(100);
        _progressHpBar->setMidpoint(Vec2::ANCHOR_MIDDLE_LEFT);
        _progressHpBar->setBarChangeRate(Vec2(1,0));
        _progressHpBar->setPosition(_progressHpBG->getContentSize().width / 2, _progressHpBG->getContentSize().height / 2);
        _progressHpBG->addChild(_progressHpBar);
        
        //
        _progressHpLabel = MafNode::createBmFont("0", GAME_BM_FONT_AL, 16);
        _progressHpLabel->setPosition(_progressHpBG->getContentSize()/2);
        _progressHpLabel->setColor(Color3B(255,223,255));
        _progressHpBG->addChild(_progressHpLabel);
    }
    
    // 데미지 라벨
    _listLabelDamage.clear();
    for ( int i = 0; i < 30; i++)
    {
        auto labelDamage = MafNode::createBmFont("", GAME_BM_FONT_AL, 24);
        labelDamage->setAnchorPoint(Vec2(0.5,0));
        labelDamage->setVisible(false);
        labelDamage->setCascadeOpacityEnabled(true);
        addChild(labelDamage, 100);
        
        _listLabelDamage.pushBack(labelDamage);
    }
}
 
#pragma mark - Monster : set, get
bool UnitBehaviorMonster::isDie()
{
    return _bDie;
}

bool UnitBehaviorMonster::isBuffRecommend()
{
    return _bBuffRecommend;
}

void UnitBehaviorMonster::setBuffRecommend(bool rec)
{
    _bBuffRecommend = rec;
}

std::string UnitBehaviorMonster::getMonsterHpMax()
{
    return _hpMax;
}

std::string UnitBehaviorMonster::getMonsterHp()
{
    return _hp;
}

UnitBehaviorMonster::E_BOSS UnitBehaviorMonster::getMonsterType()
{
    return _eBoss;
}

int UnitBehaviorMonster::getMonsterIdx()
{
    return _nIdx;
}

Sprite* UnitBehaviorMonster::getMonsterSprite()
{
    return _spritePartDefault;
}

Sprite* UnitBehaviorMonster::getMonsterHPSprite()
{
    return _progressHpBG;
}

#pragma mark - Monster : character 모양 및 위치
void UnitBehaviorMonster::setMotion(E_BOSS eBosstype)
{
    _bDie = false;
    _eBoss = E_BOSS::NORMAL;
    
    //
    int floor = UserInfoManager::getInstance()->getFloor(_ePlace);
    int floorCount = UserInfoManager::getInstance()->getFloorCount(_ePlace);
    
    // 보스마다 scale이 달라져서 초기 세팅
    if ( _ePlace == E_PLACE::DG_DEVIL_DOM )
        _chaScale = DevilScale;
    else
        _chaScale = NormalScale;
    
    //
    if ( _ePlace == E_PLACE::DG_PRISON )
    {
        _eBoss = eBosstype;
    }
    else if ( _ePlace == E_PLACE::DG_NORMAL )
    {
        if ( floorCount == C_MAX_FLOOR_COUNT )
        {
            _eBoss = E_BOSS::BOSS;
            if ( floor % 100 == 0 )
            {
                if ( floor >= 9001 )            _eBoss = E_BOSS::BOSS_9000;
                else if ( floor >= 8001 )       _eBoss = E_BOSS::BOSS_8000;
                else if ( floor >= 7001 )       _eBoss = E_BOSS::BOSS_7000;
                else if ( floor >= 6001 )       _eBoss = E_BOSS::BOSS_6000;
                else if ( floor >= 5001 )       _eBoss = E_BOSS::BOSS_AFTERLIFE3;
                else if ( floor >= 4001 )       _eBoss = E_BOSS::BOSS_AFTERLIFE2;
                else if ( floor >= 3001 )       _eBoss = E_BOSS::BOSS_AFTERLIFE1;
                else if ( floor >= 2001 )       _eBoss = E_BOSS::BOSS_HEAVEN;
                else if ( floor >= 1001 )       _eBoss = E_BOSS::BOSS_HELL;
                else                            _eBoss = E_BOSS::BOSS_DRAGON;
                
                _chaScale = 1;
            }
            else if ( floor % 10 == 0 )
            {
                _eBoss = E_BOSS::BOSS_10;
            }
        }
    }
    else if(_ePlace == E_PLACE::DG_DEVIL_DOM)
    {
        if ( floorCount == C_MAX_FLOOR_COUNT )
        {
            _eBoss = E_BOSS::BOSS;
            if ( floor % 1000 == 0 )
            {
                if ( floor == 1000 )            _eBoss = E_BOSS::DEVIL_BOSS_1;
                else if ( floor == 2000 )       _eBoss = E_BOSS::DEVIL_BOSS_2;
                else if ( floor == 3000 )       _eBoss = E_BOSS::DEVIL_BOSS_3;
                else if ( floor == 4000 )       _eBoss = E_BOSS::DEVIL_BOSS_4;
                else if ( floor == 5000 )       _eBoss = E_BOSS::DEVIL_BOSS_5;
                else if ( floor == 6000 )       _eBoss = E_BOSS::DEVIL_BOSS_6;
                else if ( floor == 7000 )       _eBoss = E_BOSS::DEVIL_BOSS_7;
                else if ( floor == 8000 )       _eBoss = E_BOSS::DEVIL_BOSS_8;
                else if ( floor == 9000 )       _eBoss = E_BOSS::DEVIL_BOSS_9;
                else if ( floor == 10000 )      _eBoss = E_BOSS::DEVIL_BOSS_10;
            }
            else if ( floor % 100 == 0 )
            {
                _eBoss = E_BOSS::BOSS_100; //시즌1에는 100층보스를 잘못둠.시즌2만 변경함
            }
            else if ( floor % 10 == 0 )
            {
                _eBoss = E_BOSS::BOSS_10;
            }
        }
    }
    
    
    //
    std::string strMonsterPath = "";;
    if ( _ePlace == E_PLACE::DG_PRISON )
    {
        if ( _eBoss == E_BOSS::PRISON_NORMAL )
        {
            strMonsterPath = "Assets/character_mon/prison/boss_" + MafUtils::toString(random(1, 6)) + ".png";
            
            if ( DataManager::isEventChristmas() == true )         strMonsterPath = "Assets/character_mon/prison/event_boss_" + MafUtils::toString(random(1, 5)) + ".png";
            else if ( DataManager::isEvent5th() == true )          strMonsterPath = "Assets/character_mon/prison/4th_boss_" + MafUtils::toString(random(1, 6)) + ".png";
            else if ( DataManager::isEventSummer() == true )       strMonsterPath = "Assets/character_mon/prison/19summer_boss_0" + MafUtils::toString(random(1, 7)) + ".png";
            else if ( DataManager::isEventThanksgiving() == true ) strMonsterPath = "Assets/character_mon/prison/21moon_boss_" + MafUtils::toString(random(1, 5)) + ".png";
            else if ( DataManager::isEventHalloween() == true )    strMonsterPath = "Assets/character_mon/prison/20halloween_boss_0" + MafUtils::toString(random(1, 8)) + ".png";
            else if ( DataManager::isNewYear() == true )    strMonsterPath = "Assets/character_mon/prison/22new_boss_" + MafUtils::toString(random(1, 7)) + ".png";
        }
        else if ( _eBoss == E_BOSS::PRISON_TREASURE )
        {
            strMonsterPath = "Assets/character_mon/prison/treasure_1.png";
            
            if ( DataManager::isEventChristmas() == true )         strMonsterPath = "Assets/character_mon/prison/event_treasure.png";
            else if ( DataManager::isEvent5th() == true )          strMonsterPath = "Assets/character_mon/prison/4th_treasure.png";
            else if ( DataManager::isEventSummer() == true )       strMonsterPath = "Assets/character_mon/prison/19summer_treasure.png";
            else if ( DataManager::isEventThanksgiving() == true ) strMonsterPath = "Assets/character_mon/prison/21moon_treasure.png";
            else if ( DataManager::isEventHalloween() == true )    strMonsterPath = "Assets/character_mon/prison/20halloween_treasure.png";
            else if ( DataManager::isNewYear() == true )    strMonsterPath = "Assets/character_mon/prison/21new_treasure.png";
        }
        else if ( _eBoss == E_BOSS::PRISON_NORMAL_FIRE )
        {
            strMonsterPath = "Assets/character_mon/prison/boss_" + MafUtils::toString(random(7, 11)) + ".png";
        }
        else if ( _eBoss == E_BOSS::PRISON_TREASURE_FIRE )
        {
            strMonsterPath = "Assets/character_mon/prison/treasure_2.png";
        }
    }
    else if ( _ePlace == E_PLACE::DG_DEVIL_DOM )
    {
        int rand = random(1, 15);
        int val = floor / 1000;
        
        if ( floor % 1000 == 0 && floorCount == C_MAX_FLOOR_COUNT)
        {
            strMonsterPath = MafUtils::format("Assets/character_mon/devil_boss/boss_%.2d.png",val);
        }
        else
        {
            if ( floor % 1000 == 0)
                val -= 1;
            rand += val * 15;
            strMonsterPath = MafUtils::format("Assets/character_mon/devil/monster_%.3d.png",rand);
        }
    }
    else
    {
        if ( floor % 100 == 0 && floorCount==C_MAX_FLOOR_COUNT )
        {
            int val = floor / 1000;
            if ( floor % 1000 != 0 )
                val++;
            
            if(val<1)      val = 1;
            if(val>10)     val = 10;
            
            if(floor>=30001)         val = 16;
            else if(floor>=20001)    val = 15;
            else if(floor>=18001)    val = 14;
            else if(floor>=16001)    val = 13;
            else if(floor>=14001)    val = 12;
            else if(floor>=12001)    val = 11;
            
            _nIdx = val;
            strMonsterPath = MafUtils::format("Assets/character_mon/normal_boss/monster_boss_%d.png", val);
        }
        else
        {
            int rand = random(1, 15);
            
            if(floor>=30001)         rand = random(211, 225);
            else if(floor>=20001)    rand = random(196, 210);
            else if(floor>=18001)    rand = random(181, 195);
            else if(floor>=16001)    rand = random(166, 180);
            else if(floor>=14001)    rand = random(151, 165);
            else if(floor>=12001)    rand = random(136, 150);
            else if(floor>=10001)    rand = random(1, 15);
            else if(floor>=9001)     rand = random(121, 135);
            else if(floor>=8001)     rand = random(106, 120);
            else if(floor>=7001)     rand = random(91, 105);
            else if(floor>=6001)     rand = random(76, 90);
            else if(floor>=5001)     rand = random(46, 75);
            else if(floor>=4001)     rand = random(61, 75);
            else if(floor>=3001)     rand = random(61, 75);
            else if(floor>=2001)     rand = random(31, 45);
            else if(floor>=1001)     rand = random(16, 30);

            _nIdx = rand;
            strMonsterPath = MafUtils::format("Assets/character_mon/normal/monster_%d.png", rand);
        }
    }
    
    //
    _spritePartDefault->setVisible(true);
    _spritePartDefault->setOpacity(255);
    _spritePartDefault->setTexture(strMonsterPath);
    _spritePartDefault->getTexture()->setTexParameters(texParams);
    _spritePartDefault->setScale(_chaScale);
    if ( _spritePartDefault->getNumberOfRunningActions() != 0 )
    {
        _spritePartDefault->stopAllActions();
    }
    
    if ( _ePlace == E_PLACE::DG_NORMAL )
    {
        if ( _eBoss == E_BOSS::BOSS || _eBoss == E_BOSS::BOSS_10 )
            _spritePartDefault->setScale(_chaScale*1.3);
    }
}

void UnitBehaviorMonster::setPositionMonster(Node *parent, float height)
{
    Size size = parent->getContentSize();
    
    float posX = size.width + _spritePartDefault->getBoundingBox().size.width;
    float posY = size.height - height * 0.90;
    if ( _ePlace ==E_PLACE::DG_NORMAL )
    {
        double heightPlus = UserInfoManager::getInstance()->_fHeightIphoneX * (180.0 / 700.0);
        posY = size.height - height + 215 + heightPlus;
    }
    else if( _ePlace == E_PLACE::DG_PRISON )
        posY = size.height - height*0.80;
    
    //
    setPosition(Vec2(posX, posY));
    
    //
    if ( _ePlace == E_PLACE::DG_PRISON )
    {
        _spritePartDefault->setPosition(Vec2::ZERO);
    }
    else if ( _ePlace == E_PLACE::DG_NORMAL )
    {
        if ( _eBoss == E_BOSS::NORMAL || _eBoss == E_BOSS::BOSS || _eBoss == E_BOSS::BOSS_10 )
        {
            _spritePartDefault->setPosition(Vec2::ZERO);
        }
        else
        {
            float newHeight = -30;
            float width = -130;
            _spritePartDefault->setPosition(Vec2(width, newHeight));
        }
    }
    else if ( _ePlace == E_PLACE::DG_DEVIL_DOM )
    {
        _spritePartDefault->setPosition(Vec2::ZERO);
    }
 
    
    //
    if ( _ePlace == E_PLACE::DG_PRISON )
    {
        _progressHpBG->setPosition(_spritePartDefault->getPositionX() + _spritePartDefault->getBoundingBox().size.width / 2, 10);
    }
    else if ( _ePlace == E_PLACE::DG_NORMAL )
    {
        if ( _eBoss == E_BOSS::NORMAL || _eBoss == E_BOSS::BOSS || _eBoss == E_BOSS::BOSS_10 )
        {
            _progressHpBG->setPosition(_spritePartDefault->getPositionX() + _spritePartDefault->getBoundingBox().size.width / 2, 10);
        }
        else
        {
            _progressHpBG->setPosition(160, 10);
        }
    }
    else if (_ePlace == E_PLACE::DG_DEVIL_DOM )
    {
        if ( _eBoss == E_BOSS::DEVIL_BOSS_1 )
        {
            _progressHpBG->setPosition(_spritePartDefault->getPositionX() + 60, 10);
        }
        else if ( _eBoss == E_BOSS::DEVIL_BOSS_10 )
        {
            _progressHpBG->setPosition(160, 10);
        }
        else
        {
            _progressHpBG->setPosition(_spritePartDefault->getPositionX() + _spritePartDefault->getBoundingBox().size.width / 2, 10);
        }
    }
}

void UnitBehaviorMonster::setMonsterHp(std::string hp, std::string hpMax)
{
    if ( _ePlace == E_PLACE::DG_DEVIL_DOM )
    {
        _hpMax = DataManager::GetMonsterHpDevil();
        
        if(_eBoss == E_BOSS::BOSS) //boss
            _hpMax = MafUtils::bigMulNum(_hpMax, MafUtils::toString(C_BOSS_HP));
        else if(_eBoss == E_BOSS::BOSS_10)
            _hpMax.append("0"); //10배
        else if(_eBoss == E_BOSS::BOSS_100)
            _hpMax.append("0"); //10배
        else if(_eBoss >= E_BOSS::DEVIL_BOSS_1)
            _hpMax = MafUtils::bigMulNum(_hpMax, MafUtils::toString(C_BOSS_D_HP));
        
        std::string savedHp = UserDefault::getInstance()->getStringForKey(KEY_D_MONSTER_HP,"0");
        if ( savedHp == "0" || MafUtils::compareStr(savedHp, _hpMax) )
        {
            //
            _hp = _hpMax;
            
            //
            UserInfoManager::getInstance()->setHitCount(_ePlace, 0);
            UserInfoManager::getInstance()->setHitCountDevilHedgehog(0);
            UserInfoManager::getInstance()->setHitCountDevilFlash(0);
        }
        else
        {
            _hp = savedHp;
        }
        
        //
        UserDefault::getInstance()->setStringForKey(KEY_D_MONSTER_HP, _hp);
    }
    else if(_ePlace == E_PLACE::DG_NORMAL)
    {
        setBuffRecommend(false);
        _hpMax = DataManager::getInstance()->GetMonsterHp(_ePlace);
        
        if(_eBoss == E_BOSS::BOSS) //boss
            _hpMax = MafUtils::bigMulNum(_hpMax, MafUtils::toString(C_BOSS_HP));
        else if(_eBoss == E_BOSS::BOSS_10)
            _hpMax.append("0"); //10배
        else if(_eBoss >= E_BOSS::BOSS_DRAGON && _eBoss <= E_BOSS::BOSS_9000)
            _hpMax = MafUtils::bigMulNum(_hpMax, MafUtils::toString(C_BOSS_D_HP));
        
        std::string savedHp = UserDefault::getInstance()->getStringForKey(KEY_MONSTER_HP,"0");
        if ( savedHp == "0" || MafUtils::compareStr(savedHp, _hpMax))
        {
            //
            _hp = _hpMax;
            
            //
            UserInfoManager::getInstance()->setHitCount(_ePlace, 0);
            UserInfoManager::getInstance()->setHongHitCount(_ePlace, 0);
            
            //
            if( UserInfoManager::getInstance()->isFloorCheck(1000, false) )
            {
                if( CostumeManager::getInstance()->isEquip(E_COSTUME::CAPE,E_COSTUME_IDX::IDX_5))
                {
                    auto temp = MafUtils::bigMulNum(_hp, "5");
                    temp.pop_back();
                    _hp = temp;
                }
            }
            
            if( UserInfoManager::getInstance()->isFloorCheck(1001, true) )
            {
                if( CostumeManager::getInstance()->isEquip(E_COSTUME::CAPE,E_COSTUME_IDX::IDX_4))
                {
                    auto temp = MafUtils::bigMulNum(_hp, "5");
                    temp.pop_back();
                    _hp = temp;
                }
            }
            
            if ( PetNewManager::getInstance()->getPet(E_PET::HANDSOME_8))
            {
                int attackPer =  PetNewManager::getInstance()->getPetEffect(E_PET::HANDSOME_8, PetNewManager::getInstance()->getPetLevel(E_PET::HANDSOME_8));
                
                int totalPer = 100 - attackPer;
                int reinforceLevel = PetNewManager::getInstance()->getPetReinforce(E_PET::HANDSOME_8);
                double reinforcePer = PetNewManager::getInstance()->getPetReinforceEffect(E_PET::HANDSOME_8, reinforceLevel);
                if(reinforcePer>0)
                {
                    totalPer = totalPer*reinforcePer;
                }
                _hp = MafUtils::bigMulNum(_hp, MafUtils::toString(totalPer));
                _hp.pop_back();
                _hp.pop_back();
                
                int kill = PetNewManager::getInstance()->getPetEquipEffect(802, 0);
                if ( kill >= random(1, 100) && (UserInfoManager::getInstance()->getFloor(E_PLACE::DG_NORMAL)%100 != 0) )
                {
                    _hp = "0";
                }
            }
        }
        else
        {
            _hp = savedHp;
        }
          
        //
        UserDefault::getInstance()->setStringForKey(KEY_MONSTER_HP,_hp);
    }
    else if(_ePlace == E_PLACE::DG_PRISON)
    {
        _hp = hp;
        _hpMax = hpMax;
        
        if ( MafUtils::compareStr(_hp, _hpMax) )
        {
            //
            UserInfoManager::getInstance()->setHitCount(_ePlace, 0);
            UserInfoManager::getInstance()->setHongHitCount(_ePlace, 0);
            
            //
            if ( PetNewManager::getInstance()->getPet(E_PET::HANDSOME_8) )
            {
                int attackPer =  PetNewManager::getInstance()->getPetEffect(E_PET::HANDSOME_8, PetNewManager::getInstance()->getPetLevel(E_PET::HANDSOME_8));
                int totalPer = 100 - attackPer;
                int reinforceLevel = PetNewManager::getInstance()->getPetReinforce(E_PET::HANDSOME_8);
                double reinforcePer = PetNewManager::getInstance()->getPetReinforceEffect(E_PET::HANDSOME_8, reinforceLevel);
                if(reinforcePer>0)
                {
                    totalPer = totalPer*reinforcePer;
                }
                _hp = MafUtils::bigMulNum(_hp, MafUtils::toString(totalPer));
                if ( _hp.length() > 2 )
                {
                    _hp.pop_back();
                    _hp.pop_back();
                }
                else
                {
                    _hp = "0";
                }
                
            }
        }
    }
    
    //
    std::string strHp = _hp;
    strHp = MafUtils::convertNumberToShort(strHp);
    
    _progressHpLabel->setString(strHp);
    _progressHpBar->setPercentage(100);
    _progressHpBG->setVisible(true);
    _progressHpBG->setOpacity(255);
}

#pragma mark - Monster : action default
void UnitBehaviorMonster::actionMove()
{
    double nTimeOriginal = UserInfoManager::getInstance()->getUserSpeed(_ePlace, false);
    double nTimeDelay = nTimeOriginal;
//    double nTimeMove1 = nTimeDelay * 4 * 4;
    double nTimeMove2 = nTimeDelay * 4 * 2;
    
    Size size = getContentSize();
    
    if ( _ePlace == E_PLACE::DG_PRISON )
    {
        // 이용시간 30초 > 15초로 변경에 따른 이동속도 증가
        nTimeMove2 *= 0.25;
        
        auto des = Vec2(size.width*0.50 -30, getPosition().y);
        
        MoveTo *move = MoveTo::create(nTimeMove2, Vec2(des));

        Sequence *seq = Sequence::create(move, nullptr);
        runAction(seq);
    }
    else
    {
        auto des = Vec2(size.width*0.50 + 10, getPosition().y);
        
        MoveTo *move = MoveTo::create(nTimeMove2, Vec2(des));
        if ( UserInfoManager::getInstance()->getFloorCount(_ePlace) == 1 )
        {
            Sequence *seq = Sequence::create(DelayTime::create(nTimeMove2), move, nullptr);
            runAction(seq);
        }
        else
        {
            runAction(move);
        }
    }
}

void UnitBehaviorMonster::actionHit(E_CHARACTER eCharac, int hitMask, std::string damage, bool critical)
{
    if ( _bDie )
    {
        return;
    }
        
    
    //
    int floor = UserInfoManager::getInstance()->getFloor(_ePlace);
    
    //
    if ( _eBoss != E_BOSS::PRISON_TREASURE )
    {
        int numSound = random(1, 15); // 20% 확률
        if(numSound==1)         SoundManager::SoundEffectStart(SOUND_MONSTER_HIT_1);
        else if(numSound==2)    SoundManager::SoundEffectStart(SOUND_MONSTER_HIT_2);
        else if(numSound==3)    SoundManager::SoundEffectStart(SOUND_MONSTER_HIT_3);
    }
    
    if( UserInfoManager::getInstance()->getHitCount(_ePlace) <= 1 )
        UserInfoManager::getInstance()->setOneKill(_ePlace, true);
    else
        UserInfoManager::getInstance()->setOneKill(_ePlace, false);
    
    //
    _hp = MafUtils::bigSubNum(_hp, damage);
    
    //
    if (_ePlace == E_PLACE::DG_PRISON)
    {
        if ( CostumeManager::getInstance()->isEquip(E_COSTUME::CAPE,E_COSTUME_IDX::IDX_33)){
            int tempfloor = (PrisonManager::getInstance()->getDungeonKill()) % 25;
            if (1 <= tempfloor && tempfloor <= 5) {
                _hp[0] = '0';
            }
        }
    }
    else if (_ePlace == E_PLACE::DG_NORMAL)
    {
        if ( CostumeManager::getInstance()->isEquip(E_COSTUME::CAPE,E_COSTUME_IDX::IDX_33))
        {
            if (1 == floor % 5)
            {
                _hp[0] = '0';
            }
        }
        
        if ( CostumeManager::getInstance()->isEquip(E_COSTUME::HEAD,E_COSTUME_IDX::IDX_19))
        {
            if ( floor > 100 )
            {
                int tempfloor = floor % 100;
                int count = 0;
                if ( CostumeManager::getInstance()->isEquip(E_COSTUME::HEAD,E_COSTUME_IDX::IDX_33))
                    count = 40;
                else
                    count = 20;
                
                if (1 <= tempfloor && tempfloor <= count)
                {
                    _hp[0] = '0';
                }
            }
        }
    }
    
    if ( _hp[0] == '0' )
    {
        _hp = "0";
        _progressHpBar->setPercentage(0);
        
        if ( _ePlace == E_PLACE::DG_DEVIL_DOM )
        {
            UserDefault::getInstance()->setStringForKey(KEY_D_MONSTER_HP,"0");
        }
        else if ( _ePlace == E_PLACE::DG_NORMAL )
        {
            UserDefault::getInstance()->setStringForKey(KEY_MONSTER_HP,"0");
            
            int monsterHitCount = UserInfoManager::getInstance()->getHitCount(_ePlace);
            if ( monsterHitCount <= 1 )
            {
                if(_eBoss > E_BOSS::NORMAL)
                {
                    auto data = SaveManager::getHistoryDataInt(E_HISTORY_DATA::K_ONE_SHOT);
                    int oneKill = UserInfoManager::getInstance()->getFloor(_ePlace);
                    if(oneKill > data)
                    {
                        SaveManager::setHistoryData(E_HISTORY_DATA::K_ONE_SHOT, oneKill);
                    }
                }
            }
            
            if(_eBoss == E_BOSS::BOSS_HEAVEN)
                MafGooglePlay::IncrementAchievement(ACHIEVE_GOD_1000KILL, 1, 1000);
            else if(_eBoss == E_BOSS::BOSS_HELL && floor >= 2000 && monsterHitCount <= 1)
                MafGooglePlay::UnlockAchievement(ACHIEVE_2000_ONEKILL, 1, 1);
            else if(_eBoss == E_BOSS::BOSS_DRAGON && floor >= 1000 && monsterHitCount <= 1)
                MafGooglePlay::UnlockAchievement(ACHIEVE_1000_DRAGON, 1, 1);
            else if(_eBoss == E_BOSS::BOSS_DRAGON && monsterHitCount <= 1)
                MafGooglePlay::UnlockAchievement(ACHIEVE_100_DRAGON, 1, 1);
        }
        
        //
        actionDie();
    }
    else
    {
        if (_ePlace == E_PLACE::DG_NORMAL)
        {
            UserDefault::getInstance()->setStringForKey(KEY_MONSTER_HP,_hp);
        }
        else if (_ePlace == E_PLACE::DG_DEVIL_DOM)
        {
            UserDefault::getInstance()->setStringForKey(KEY_D_MONSTER_HP,_hp);
        }
            
        
        std::string tempMax = _hpMax;
        tempMax.pop_back(); //10%
        std::string total = "0";
        int per =0;

        for(int i=0; i <10; i++){ //10%
            auto tmp = MafUtils::bigAddNum(total, tempMax);
            if(!MafUtils::compareStr(_hp, tmp)){
                break;
            }
            total = MafUtils::bigAddNum(total, tempMax);
            per += 10;
        }
        
        tempMax.pop_back();
        for(int i=0; i <10; i++){ //1%
            auto tmp = MafUtils::bigAddNum(total, tempMax);
            if(!MafUtils::compareStr(_hp, tmp)){
                break;
            }
            total = MafUtils::bigAddNum(total, tempMax);
            per += 1;
        }
        _progressHpBar->setPercentage(per);
        std::string str = _hp;
        _progressHpLabel->setString(MafUtils::convertNumberToShort(str));
        
        if ( eCharac == E_CHARACTER::CHA_HERO )
        {
            int level = UserInfoManager::getInstance()->getArtifactLevel(_ePlace, E_ARTIFACT::ARTI_7);
            bool bBlend = true;
            if ( level >= 5 )
            {
                if ( random(0, 1) == 0 )
                {
                    bBlend = true;
                }
                else
                {
                    bBlend = false;
                }
            }
            
            if ( bBlend )
            {
                CallFunc *call = CallFunc::create([this](){
                    auto blendFunc = (BlendFunc) { GL_ONE, GL_ONE };//GL_ONE, GL_ONE_MINUS_SRC_ALPHA
                    _spritePartDefault->setBlendFunc(blendFunc);
                });
                CallFunc *call1 = CallFunc::create([this](){
                    auto blendFunc = (BlendFunc) { GL_ONE, GL_ONE_MINUS_SRC_ALPHA };//GL_ONE, GL_ONE_MINUS_SRC_ALPHA
                    _spritePartDefault->setBlendFunc(blendFunc);
                });
                Sequence *sseq = Sequence::create(call,DelayTime::create(0.1f),call1, nullptr);
                auto hitMove = MoveBy::create(0.05f, Vec2(10,0));
                auto moveSeq = Sequence::create(hitMove, hitMove->reverse(), nullptr);
                _spritePartDefault->runAction(Spawn::create(sseq, moveSeq, nullptr));
                
            }
        }
    }
    
    Label* lbDamage = nullptr;
    if ( UserInfoConfigManager::getInstance()->isFunction(E_FUNCTION_ACTIVE::DAMAGE) == true )
    {
        lbDamage = _listLabelDamage.front();
        if(lbDamage == nullptr)
            return;
        
        lbDamage->retain();
        
        _listLabelDamage.eraseObject(lbDamage);
        _listLabelDamage.pushBack(lbDamage);
        
        lbDamage->release();
         
    }
    
    if ( lbDamage == nullptr )
    {
        return;
    }
    
    if ( lbDamage->getChildByName("UI_ICON") != nullptr )
    {
        lbDamage->removeChildByName("UI_ICON");
    }
    
    int fontSize = 36;
    
    float width = _spritePartDefault->getBoundingBox().size.width;
    
    float wPer = 0.5f;
    if((_eBoss >= E_BOSS::BOSS_DRAGON && _eBoss <= E_BOSS::BOSS_9000)){
        wPer = 0.25f;
    }
    else if(_eBoss == E_BOSS::DEVIL_BOSS_1){
        wPer = 0.10f;
    }
    
    if (eCharac == E_CHARACTER::CHA_HERO)
    {
        if(_ePlace==E_PLACE::DG_DEVIL_DOM)
            lbDamage->setPosition(Vec2(width*wPer, 100));
        else
            lbDamage->setPosition(Vec2(width*wPer, 90));
        
        if ( critical )
        {
            lbDamage->setColor(Color3B(255,99,87));
            fontSize = 48;
        }
        else
        {
            lbDamage->setColor(Color3B(255,204,65));
        }
        
        if ( (hitMask&E_CHARACTER_HIT::HIT_DEATHBLOW) == E_CHARACTER_HIT::HIT_DEATHBLOW || (hitMask&E_CHARACTER_HIT::HIT_FROSTSKILL_CRI) == E_CHARACTER_HIT::HIT_FROSTSKILL_CRI )
        {
            if ( (hitMask&E_CHARACTER_HIT::HIT_DEATHBLOW) == E_CHARACTER_HIT::HIT_DEATHBLOW )
            {
                lbDamage->setPosition(lbDamage->getPosition() + Vec2::UP * 100);
                lbDamage->setColor(Color3B(50, 255, 150));
                fontSize = 60;
            }
            else if ( (hitMask&E_CHARACTER_HIT::HIT_FROSTSKILL_CRI) == E_CHARACTER_HIT::HIT_FROSTSKILL_CRI )
            {
                lbDamage->setColor(Color3B(255,99,87));
                fontSize = 48;
            }
            
            if ( (hitMask&E_CHARACTER_HIT::HIT_FROSTSKILL_CRI) == E_CHARACTER_HIT::HIT_FROSTSKILL_CRI )
            {
                auto uiIcon = Sprite::create("Assets/ui/frost_prison/critical_icon.png");
                uiIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
                uiIcon->setPosition(Vec2(0, lbDamage->getContentSize().height / 2));
                uiIcon->setName("UI_ICON");
                uiIcon->getTexture()->setTexParameters(texParams);
                lbDamage->addChild(uiIcon);
            }
        }
        else if ( (hitMask&E_CHARACTER_HIT::HIT_FROSTSKILL_50005) == E_CHARACTER_HIT::HIT_FROSTSKILL_50005 || (hitMask&E_CHARACTER_HIT::HIT_FROSTSKILL_50006) == E_CHARACTER_HIT::HIT_FROSTSKILL_50006 )
        {
            lbDamage->setPosition(lbDamage->getPosition() + Vec2::UP * 100);
            lbDamage->setColor(Color3B(255 ,235, 0));
            fontSize = 60;
        }
    }
    else if (eCharac == E_CHARACTER::CHA_SHADOW)
    {
        lbDamage->setPosition(Vec2(width*wPer+130, 50));
        lbDamage->setColor(Color3B(150,150,150));
        fontSize = 28;
        if(critical)
            fontSize = 32;
    }
    else if (eCharac == E_CHARACTER::CHA_GAVEL)
    {
        lbDamage->setPosition(Vec2(width*wPer, 200));
        lbDamage->setColor(Color3B(144,255,0));
        fontSize = 60;
    }
    else if (eCharac == E_CHARACTER::CHA_PET_CHICK)
    {
        lbDamage->setPosition(Vec2(width*wPer, 30));
        lbDamage->setColor(Color3B::WHITE);
        if(critical) fontSize = 48;
    }
    else if (eCharac == E_CHARACTER::CHA_PET_CHICK_EVOLUTION)
    {
        lbDamage->setPosition(Vec2(width*wPer, 40));
        lbDamage->setColor(Color3B::WHITE);
        if(critical) fontSize = 48;
    }
    else if(eCharac == E_CHARACTER::CHA_PET_DRAGON)
    {
        lbDamage->setPosition(Vec2(width*wPer, 200));
        lbDamage->setColor(Color3B::RED);
        fontSize = 56;
    }
    else if(eCharac == E_CHARACTER::CHA_PET_HONG )
    {
        lbDamage->setPosition(Vec2(width*wPer,  170));
        lbDamage->setColor(Color3B(219,153,255));
        fontSize = 56;
    }
    else if(eCharac == E_CHARACTER::CHA_D_PET_PENGUIN )
    {
        lbDamage->setPosition(Vec2(width*wPer,  140));
        lbDamage->setColor(Color3B::WHITE);
        fontSize = 48;
    }
    else if(eCharac == E_CHARACTER::CHA_D_PET_HEDGEHOG)
    {
        lbDamage->setPosition(Vec2(width*wPer,  50));
        lbDamage->setColor(Color3B::RED);
        fontSize = 48;
    }
    else if(eCharac == E_CHARACTER::CHA_D_PET_FLASH )
    {
        lbDamage->setPosition(Vec2(width*wPer,  200));
        lbDamage->setColor(Color3B::RED);
        fontSize = 56;
    }
    
    
    MafNode::setNewScale(*lbDamage, fontSize);
    MafUtils::convertNumberToShort(damage);
    if(damage == "0")
    {
        damage = "";
    }
    lbDamage->setString(damage.c_str());
    lbDamage->setOpacity(255);
    lbDamage->setVisible(true);
    
    MoveBy *move =  nullptr;
    bool bActionMove = false;
    if ( critical )
    {
        bActionMove = true;
        move = MoveBy::create(0.25f, Vec2(0,50));
    }
    
    if ( !bActionMove )
    {
        if(eCharac == E_CHARACTER::CHA_PET_HONG || eCharac == E_CHARACTER::CHA_D_PET_HEDGEHOG){
            move = MoveBy::create(1.0f, Vec2(0,0));
        }else{
            move = MoveBy::create(0.2f, Vec2(0,0));
        }
    }
    
    FadeOut *fade = FadeOut::create(0.1f);
    Sequence *seq = Sequence::create(move,fade,Hide::create(), nullptr);
    EaseIn *ease = EaseIn::create(seq, 0.5f);
    lbDamage->runAction(ease);
}

void UnitBehaviorMonster::actionDie()
{
    if ( _bDie )
        return;
    
    //
    _bDie = true;
    
    //
    if(_eBoss==E_BOSS::BOSS_DRAGON)
    {
        SoundManager::SoundEffectStart(SOUND_DIE_DRAGON);
    }
    else if(_eBoss != E_BOSS::PRISON_TREASURE)
    {
        int numSound = random(1, 3);
        if(numSound==1)         SoundManager::SoundEffectStart(SOUND_DIE_1);
        else if(numSound==2)    SoundManager::SoundEffectStart(SOUND_DIE_2);
        else if(numSound==3)    SoundManager::SoundEffectStart(SOUND_DIE_3);
    }
    
    //
    int floor = UserInfoManager::getInstance()->getFloor(_ePlace);
    int floorCount = UserInfoManager::getInstance()->getFloorCount(_ePlace);
    
    
    
    //
    if ( _ePlace == E_PLACE::DG_DEVIL_DOM )
    {
        auto parent = getParent();
        if ( _eBoss >= E_BOSS::BOSS )
        {
            auto key = Sprite::create("Assets/icon/icon_gold_2.png");//key
            key->setPosition(Vec2(_layerCharacter->getPosition().x, _layerCharacter->getPosition().y+10));
            key->setAnchorPoint(Vec2(0,0));
            parent->addChild(key,getLocalZOrder());
            
            Sequence *seq = Sequence::create(FadeTo::create(0.5f, 50),RemoveSelf::create(), nullptr);
            key->runAction(seq);
        }
        
        if ( _eBoss >= E_BOSS::BOSS_10 )
        {
            auto jewel = Sprite::create("Assets/icon/icon_jewel.png");
            jewel->setPosition(Vec2(_layerCharacter->getPosition().x+50, _layerCharacter->getPosition().y+10));
            jewel->setAnchorPoint(Vec2(0,0));
            parent->addChild(jewel,getLocalZOrder());
            
            Sequence *seq = Sequence::create(FadeTo::create(0.5f, 50),RemoveSelf::create(), nullptr);
            jewel->runAction(seq);
        }
        
        if ( _eBoss == E_BOSS::BOSS)
        {
            ItemsMoneyManager::getInstance()->setDevilKey(C_DEVIL_BONUS_KEY);
        }
        else if(_eBoss == E_BOSS::BOSS_10)
        {
            ItemsMoneyManager::getInstance()->setDevilKey(C_DEVIL_BONUS_SEMI_KEY);
            ItemsMoneyManager::getInstance()->setGem(MafRemoteConfig::GetRemoteValueInt("BONUS_10F_GEM"));
        }
        else if(_eBoss >= E_BOSS::BOSS_100)
        {
            ItemsMoneyManager::getInstance()->setDevilKey(C_DEVIL_BONUS_DRG_KEY);
            ItemsMoneyManager::getInstance()->setGem(MafRemoteConfig::GetRemoteValueInt("BONUS_100F_GEM"));
        }
    }
    else if(_ePlace == E_PLACE::DG_NORMAL)
    {
        SoundManager::SoundEffectStart(SOUND_DIE_MONEY);

        Size visibleSize = Director::getInstance()->getVisibleSize();
        auto parent = getParent();

        auto coin = Sprite::create("Assets/icon/menu_coin.png");
        coin->setPosition(Vec2(getPosition().x, getPosition().y));
        coin->setAnchorPoint(Vec2(0,0));
        parent->addChild(coin,getLocalZOrder());

        ccBezierConfig config;
        float ranWidth = random(0.0f, visibleSize.width/2);
        float ranHeight = random(visibleSize.height*0.75, visibleSize.height*0.9);
        config.controlPoint_1 = Vec2(ranWidth, ranHeight);

        ranWidth = random(0.0f, visibleSize.width/2);
        ranHeight = random(visibleSize.height*0.75, visibleSize.height*0.9);
        config.controlPoint_2 = Vec2(ranWidth, ranHeight);

        config.endPosition    = Vec2(visibleSize.width*0.03, visibleSize.height*0.6);
        BezierTo *bezier = BezierTo::create(0.5f, config);
        Spawn *spawn = Spawn::create(bezier,FadeTo::create(0.5f, 100), nullptr);
        Sequence *seq = Sequence::create(DelayTime::create(0.05f),spawn,RemoveSelf::create(), nullptr);
        coin->runAction(seq);

        if(_eBoss >= E_BOSS::BOSS )
        {
            auto key = Sprite::create("Assets/icon/menu_key.png");
            key->setPosition(Vec2(getPosition().x, getPosition().y));
            key->setAnchorPoint(Vec2(0,0));
            parent->addChild(key,getLocalZOrder());

            Spawn *spawn = Spawn::create(FadeTo::create(0.5f, 100), nullptr);
            Sequence *seq = Sequence::create(spawn,RemoveSelf::create(), nullptr);
            key->runAction(seq);
        }

        if ( _eBoss >= E_BOSS::BOSS_10 )
        {
            auto jewel = Sprite::create("Assets/icon/icon_jewel.png");
            jewel->setPosition(Vec2(getPosition().x+50, getPosition().y));
            jewel->setAnchorPoint(Vec2(0,0));
            parent->addChild(jewel,getLocalZOrder());

            Spawn *spawn = Spawn::create(FadeTo::create(0.5f, 100), nullptr);
            Sequence *seq = Sequence::create(spawn,RemoveSelf::create(), nullptr);
            jewel->runAction(seq);
        }

        std::string gold = UserInfoManager::getInstance()->getEarnGoldFinal(_ePlace, E_EARNGOLD::MON_KILL);
        if ( CostumeManager::getInstance()->isEquip(E_COSTUME::BODY,E_COSTUME_IDX::IDX_19) )
        {
            if ( UserInfoManager::getInstance()->getHitCount(_ePlace) <= 1 )
            {
                gold = MafUtils::bigMulNum(gold, "4");
            }
        }

        //
        ItemsMoneyManager::getInstance()->setGold(gold);
        RefreshManager::getInstance()->refreshCurrencyUI(E_ITEMS::GOLD, gold);
        
        int bonusKey = 0;
        int bonusGem = 0;
        if ( _eBoss == E_BOSS::BOSS ){
            bonusKey = MafRemoteConfig::GetRemoteValueInt("BONUS_1F_KEY");
            if ( CostumeManager::getInstance()->isEquip(E_COSTUME::HAND,E_COSTUME_IDX::IDX_5) )
                bonusKey *= 2;
        }
        else if(_eBoss == E_BOSS::BOSS_10){
            bonusKey = MafRemoteConfig::GetRemoteValueInt("BONUS_10F_KEY");
            bonusGem = MafRemoteConfig::GetRemoteValueInt("BONUS_10F_GEM");
            if ( CostumeManager::getInstance()->isEquip(E_COSTUME::HAND,E_COSTUME_IDX::IDX_5) )
                bonusKey *= 2;
        }
        else if( _eBoss >= E_BOSS::BOSS_DRAGON ){
            bonusKey = MafRemoteConfig::GetRemoteValueInt("BONUS_100F_KEY");
            bonusGem = MafRemoteConfig::GetRemoteValueInt("BONUS_100F_GEM");
            if (CostumeManager::getInstance()->isEquip(E_COSTUME::HAND,E_COSTUME_IDX::IDX_5) )
                bonusKey *= 2;
            if (CostumeManager::getInstance()->isEquip(E_COSTUME::HAND,E_COSTUME_IDX::IDX_39) )
                bonusGem += 1;
        }
        
        if(bonusKey >0)
            ItemsMoneyManager::getInstance()->setKey(bonusKey);
        if(bonusGem >0)
            ItemsMoneyManager::getInstance()->setGem(bonusGem);

        if(_eBoss >= E_BOSS::BOSS_DRAGON)
        {
            if ( CostumeManager::getInstance()->isEquip(E_COSTUME::FOOT,E_COSTUME_IDX::IDX_12) )
            {
                UserInfoStackManager::getInstance()->setDragonBuff(UserInfoStackManager::getInstance()->getDragonBuff()+1);
            }
        }

        if ( _eBoss != NORMAL && _eBoss != BOSS && _eBoss != BOSS_10 )
        {
            if ( CostumeManager::getInstance()->isEquip(E_COSTUME::BODY,E_COSTUME_IDX::IDX_11) )
            {
                if (floor % 500 == 0)
                {
                    UserInfoManager::getInstance()->addHighArtifact(_ePlace, E_ARTIFACT_TYPE::ARTI_TYPE_ATK, 1);
                }
            }
            if ( CostumeManager::getInstance()->isEquip(E_COSTUME::FOOT,E_COSTUME_IDX::IDX_14) )
            {
                UserInfoStackManager::getInstance()->setHellBuff(UserInfoStackManager::getInstance()->getHellBuff()+1);
            }
        }
        
        // 몬스터 정수
        int nMonsterSoulRate = ParameterManager::getInstance()->getParameterInteger("monster_soul_rate");
        if ( floorCount == 5 && random(1, 100) <= nMonsterSoulRate )
        {
            int nMonsterSoulIdx = random(1, C_COUNT_MONSTER);
            MonsterManager::getInstance()->addSoulCount(nMonsterSoulIdx, 1);
            MonsterManager::getInstance()->saveData();
            
            //
            RefreshReddotManager::getInstance()->setValue(E_REFRESH_REDDOT::DEX_MONSTER_LEVELUP, MonsterManager::getInstance()->isSoulLevelComplete());
            RefreshReddotManager::getInstance()->refreshUI(E_REFRESH_REDDOT::DEX_MONSTER_LEVELUP);

            //
            auto coin = Sprite::create("Assets/ui/bonsikdex/get_essence_1.png");
            coin->setPosition(Vec2(getPosition().x, getPosition().y));
            coin->setAnchorPoint(Vec2(0,0));
            parent->addChild(coin,getLocalZOrder());

            ccBezierConfig config;
            config.controlPoint_1 = Vec2(visibleSize.width*0.50, parent->getContentSize().height - 300);
            config.controlPoint_2 = Vec2(visibleSize.width*0.52, parent->getContentSize().height - 300);
            config.endPosition    = Vec2(visibleSize.width*0.55, parent->getContentSize().height - 550);
            
            
            auto actionBezier = BezierTo::create(0.5f, config);
 
            
            auto aniframeRun = Animation::create();
            aniframeRun->addSpriteFrameWithFile("Assets/ui/bonsikdex/get_essence_1.png");
            aniframeRun->addSpriteFrameWithFile("Assets/ui/bonsikdex/get_essence_2.png");
            aniframeRun->addSpriteFrameWithFile("Assets/ui/bonsikdex/get_essence_3.png");
            aniframeRun->addSpriteFrameWithFile("Assets/ui/bonsikdex/get_essence_4.png");
            aniframeRun->addSpriteFrameWithFile("Assets/ui/bonsikdex/get_essence_5.png");
            aniframeRun->addSpriteFrameWithFile("Assets/ui/bonsikdex/get_essence_6.png");
            aniframeRun->addSpriteFrameWithFile("Assets/ui/bonsikdex/get_essence_7.png");
            aniframeRun->addSpriteFrameWithFile("Assets/ui/bonsikdex/get_essence_8.png");
            aniframeRun->addSpriteFrameWithFile("Assets/ui/bonsikdex/get_essence_9.png");
            aniframeRun->setDelayPerUnit(0.1f);

            Sequence *seq = Sequence::create(DelayTime::create(0.05f), actionBezier, Animate::create(aniframeRun), RemoveSelf::create(), nullptr);
            coin->runAction(seq);
        }
    }
    
    
    //
    _spritePartDefault->runAction(FadeOut::create(0.05f));
    _progressHpBG->setVisible(false);
     
    if ( _ePlace == E_PLACE::DG_NORMAL || _ePlace == E_PLACE::DG_DEVIL_DOM )
    {
        if( floorCount >= C_MAX_FLOOR_COUNT )
        {
            floor++;
            floorCount = 1;
            
            UserInfoManager::getInstance()->setFloor(_ePlace, floor);
            UserInfoManager::getInstance()->setFloorCount(_ePlace, floorCount,false);
            
            //치팅감지
            UserInfoManager::getInstance()->setCheatSensorCount(UserInfoManager::getInstance()->getCheatSensorCount() + 1);
            
            if(UserInfoManager::getInstance()->isCheatSensorOn())
            {
                UserInfoManager::getInstance()->checkCheating();
            }
        }
        else
        {
            floorCount++;
            UserInfoManager::getInstance()->setFloorCount(_ePlace, floorCount);
        }
    }
}

