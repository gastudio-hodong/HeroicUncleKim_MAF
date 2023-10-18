//
//  TutorialManager.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 18/11/2019.
//

#include "TutorialManager.h"

#include "ManagerGame/UserInfoManager.h"

#include "GameScene/HelloWorldScene.h"

#include "ManagerGame/CostumeManager.h"
#include "ManagerGame/SpecialMissionManager.h"
#include "ManagerGame/WeaponManager.h"

USING_NS_CC;

TutorialManager::TutorialManager(void) :
_nowTutorialProgress(PROGRESS_NONE,SUB_NONE),
_tutorialObject(nullptr),
_tutorialParent(nullptr),
_nowLayer(nullptr)
{

}

TutorialManager::~TutorialManager(void)
{
    
}
bool TutorialManager::init()
{
    initTutorialOpenData();
    return true;
}
#pragma mark - start,end
void TutorialManager::initTutorialOpenData()
{
    
    {//스페셜 미션
        
        if(SpecialMissionManager::getInstance()->getSpecialMissionClear() >= 9)
        {
            setOpenData(E_STEPWISE_OPEN_TYPE::ST_SPECIAL_MISSION, 9, true);
        }

        if(SpecialMissionManager::getInstance()->getSpecialMissionClear() >= 10)
        {
            setOpenData(E_STEPWISE_OPEN_TYPE::ST_SPECIAL_MISSION, 10, true);
        }
        
        if(SpecialMissionManager::getInstance()->getSpecialMissionClear() >= 17)
        {
            setOpenData(E_STEPWISE_OPEN_TYPE::ST_SPECIAL_MISSION, 17, true);
        }
        
        if(SpecialMissionManager::getInstance()->getSpecialMissionClear() >= 18)
        {
            setOpenData(E_STEPWISE_OPEN_TYPE::ST_SPECIAL_MISSION, 18, true);
        }
        
        if(SpecialMissionManager::getInstance()->getSpecialMissionClear() >= 22)
        {
            setOpenData(E_STEPWISE_OPEN_TYPE::ST_SPECIAL_MISSION, 22, true);
        }
        if(SpecialMissionManager::getInstance()->getSpecialMissionClear() >= 27)
        {
            setOpenData(E_STEPWISE_OPEN_TYPE::ST_SPECIAL_MISSION, 27, true);
        }
        if(SpecialMissionManager::getInstance()->getSpecialMissionClear() >= 83)
        {
            setOpenData(E_STEPWISE_OPEN_TYPE::ST_SPECIAL_MISSION, 83, true);
        }
    }
    
    {//층
        if(UserInfoManager::getInstance()->getHighFloor(E_PLACE::DG_NORMAL) >= 25)
        {
            setOpenData(E_STEPWISE_OPEN_TYPE::ST_FLOOR, 25, true);
        }
        
        if(SpecialMissionManager::getInstance()->getSpecialMissionClear() >= 50)
        {
            setOpenData(E_STEPWISE_OPEN_TYPE::ST_FLOOR, 50, true);
        }
        
        if(UserInfoManager::getInstance()->getHighFloor(E_PLACE::DG_NORMAL) >= 75)
        {
            setOpenData(E_STEPWISE_OPEN_TYPE::ST_FLOOR, 75, true);
        }
        
        if(UserInfoManager::getInstance()->getHighFloor(E_PLACE::DG_NORMAL) >= 100)
        {
            setOpenData(E_STEPWISE_OPEN_TYPE::ST_FLOOR, 100, true);
        }
        
        if(UserInfoManager::getInstance()->getHighFloor(E_PLACE::DG_NORMAL) >= 750)
        {
            setOpenData(E_STEPWISE_OPEN_TYPE::ST_FLOOR, 750, true);
        }
        
    }
    
    {//버튼 잠금
        
        if(UserInfoManager::getInstance()->getHighFloor(E_PLACE::DG_NORMAL) >= 120)
        {
            setOpenData(E_STEPWISE_OPEN_TYPE::ST_SPECIAL_MISSION, 34, true);
            setOpenData(E_STEPWISE_OPEN_TYPE::ST_SPECIAL_MISSION, 35, true);
            setOpenData(E_STEPWISE_OPEN_TYPE::ST_SPECIAL_MISSION, 38, true);
        }
        else
        {
            if(SpecialMissionManager::getInstance()->getSpecialMissionClear() >= 34)//보물, 마을
            {
                setOpenData(E_STEPWISE_OPEN_TYPE::ST_SPECIAL_MISSION, 34, true);
            }
            
            if(SpecialMissionManager::getInstance()->getSpecialMissionClear() >= 35)//펫
            {
                setOpenData(E_STEPWISE_OPEN_TYPE::ST_SPECIAL_MISSION, 35, true);
            }
            
            if(SpecialMissionManager::getInstance()->getSpecialMissionClear() >= 38)//코스튬
            {
                setOpenData(E_STEPWISE_OPEN_TYPE::ST_SPECIAL_MISSION, 38, true);
            }
        }
    }
    
    {//기타
        if(WeaponManager::getInstance()->getEquipHigh() >= E_WEAPON::WEAPON_76_MINE)
        {
            setOpenData("WEAPON_76", true);
        }
    }
    
}
void TutorialManager::startTutorial()
{
    _nowTutorialProgress = std::pair<PROGRESS_MAIN, PROGRESS_SUB>(PROGRESS_QUEST, SUB_SHORTCUT_BUTTON);
}
void TutorialManager::endTutorial()
{
    _nowTutorialProgress = std::pair<PROGRESS_MAIN, PROGRESS_SUB>(PROGRESS_END, SUB_NONE);
    _tutorialParent = nullptr;
}
#pragma mark - effect

void TutorialManager::attachEffect(EFFECT eff, std::pair<PROGRESS_MAIN, PROGRESS_SUB> progress, Node* button, float time)
{
    if ( progress != _nowTutorialProgress )
    {
        return;
    }
    if( _tutorialParent == button && (progress.second != SUB_REWARD_BUTTON || progress.second != SUB_SHORTCUT_BUTTON))
    {
        return;
    }
    if(_tutorialObject != nullptr )
    {
        if(_nowLayer ==  UtilsScene::getInstance()->getRunningLayer())
        {
            _tutorialObject->removeFromParent();
            _tutorialObject->release();
        }
        _tutorialObject = nullptr;
    }
    
    _nowLayer = UtilsScene::getInstance()->getRunningLayer();
  
    std::string strImagePath = "";
    std::string strname = "";
    float delay = 0;
    int imageAmount = 0;
    bool flip = false;
    if(eff == EFFECT_FINGER)
    {
        if(MafRemoteConfig::GetRemoteValueInt("tutorial_hand_color")==1)
            strImagePath = "Assets/ui/tutorial/guide_finger1_%d.png";
        else
            strImagePath = "Assets/ui/tutorial/guide_finger2_%d.png";
        strname = "FINGER";
        delay = 0.15f;
        imageAmount = 4;
    }
    else if(eff == EFFECT_FLITTER)
    {
        strImagePath = "Assets/ui/tutorial/menu_btn_bg_tutorial_new_0%d.png";
        strname = "FLITTER";
        delay = 0.1f;
        imageAmount = 7;
    }
    else if(eff == EFFECT_FLITTER_BIG)
    {
        strImagePath = "Assets/ui/tutorial/menu_btn_bg2_tutorial_new_0%d.png";
        strname = "FLITTER_BIG";
        delay = 0.1f;
        imageAmount = 7;
    }
    
    Animation* aniButton = Animation::create();
    aniButton->setDelayPerUnit(delay);
    
    for ( int i = 1; i <= imageAmount; i++ )
    {
        aniButton->addSpriteFrameWithFile(MafUtils::format(strImagePath.c_str(),i));
    }
    
        
    Animate* animate = Animate::create(aniButton);
    RepeatForever *repeat = RepeatForever::create(animate);
    
    Sprite* sprButton = Sprite::create(MafUtils::format(strImagePath.c_str(),1));
    sprButton->setPosition(button->getContentSize().width/2+10,button->getContentSize().height/2);
    sprButton->setName(strname);
    sprButton->runAction(repeat);
    sprButton->setFlippedX(flip);
    if(button->getChildByName(strname) == nullptr)
    {
        button->addChild(sprButton,2);
    }
    else
    {
        sprButton = (Sprite*)button->getChildByName(strname);
    }
    
    
    _tutorialObject = sprButton;
    _tutorialObject->retain();
    _tutorialParent = button;

    
    
}
void TutorialManager::removeAllEffect(std::pair<PROGRESS_MAIN, PROGRESS_SUB> progress)
{
    if(_tutorialObject != nullptr )
    {
        if(_nowLayer ==  UtilsScene::getInstance()->getRunningLayer())
        {
            _tutorialObject->removeFromParent();
            _tutorialObject->release();
        }
        _tutorialObject = nullptr;
    }
    
    _nowLayer = UtilsScene::getInstance()->getRunningLayer();
}

#pragma mark - get, set

void TutorialManager::refreshTutorialProgress()
{
    //퀘스트
    if(SpecialMissionManager::getInstance()->getSpecialMissionClear() == 0 || SpecialMissionManager::getInstance()->getSpecialMissionClear() == 1||
       SpecialMissionManager::getInstance()->getSpecialMissionClear() == 2 || SpecialMissionManager::getInstance()->getSpecialMissionClear() == 4||
       SpecialMissionManager::getInstance()->getSpecialMissionClear() == 5 || SpecialMissionManager::getInstance()->getSpecialMissionClear() == 7||
       SpecialMissionManager::getInstance()->getSpecialMissionClear() == 8 || SpecialMissionManager::getInstance()->getSpecialMissionClear() == 10||
       SpecialMissionManager::getInstance()->getSpecialMissionClear() == 11)
        _nowTutorialProgress.first = PROGRESS_MAIN::PROGRESS_QUEST;
    //무기
    else if( SpecialMissionManager::getInstance()->getSpecialMissionClear() == 3 || SpecialMissionManager::getInstance()->getSpecialMissionClear() == 6 ||
            SpecialMissionManager::getInstance()->getSpecialMissionClear() == 82 || SpecialMissionManager::getInstance()->getSpecialMissionClear() == 91)
            _nowTutorialProgress.first = PROGRESS_MAIN::PROGRESS_WEAPON;
    //기타
    else if( SpecialMissionManager::getInstance()->getSpecialMissionClear() == 34 || SpecialMissionManager::getInstance()->getSpecialMissionClear() == 36)
        _nowTutorialProgress.first = PROGRESS_MAIN::PROGRESS_ARTIFACT;
    
    else if( SpecialMissionManager::getInstance()->getSpecialMissionClear() == 35 )
        _nowTutorialProgress.first = PROGRESS_MAIN::PROGRESS_PET;
    
    else if( SpecialMissionManager::getInstance()->getSpecialMissionClear() == 37 )
        _nowTutorialProgress.first = PROGRESS_MAIN::PROGRESS_PRISON;
    
    else if( SpecialMissionManager::getInstance()->getSpecialMissionClear() == 38 )
        _nowTutorialProgress.first = PROGRESS_MAIN::PROGRESS_PRISON;
    
    else if(SpecialMissionManager::getInstance()->getSpecialMissionClear() == 9 || SpecialMissionManager::getInstance()->getSpecialMissionClear() == 18 ||
            SpecialMissionManager::getInstance()->getSpecialMissionClear() == 27 )
        _nowTutorialProgress.first = PROGRESS_MAIN::PROGRESS_FLOOR;
    
    else if(SpecialMissionManager::getInstance()->getSpecialMissionClear() >= 90)
        _nowTutorialProgress.first = PROGRESS_MAIN::PROGRESS_RAID;
    
    else if(SpecialMissionManager::getInstance()->getSpecialMissionClear() >= 98)
        _nowTutorialProgress.first = PROGRESS_MAIN::PROGRESS_END;
    
    
    else
        _nowTutorialProgress.first = PROGRESS_MAIN::PROGRESS_NONE;
    
    _nowTutorialProgress.second = SUB_SHORTCUT_BUTTON;
    saveTutorialProgress();
}
void TutorialManager::clickShortcutButton()
{
    _nowTutorialProgress.second = PROGRESS_SUB::SUB_REWARD_BUTTON;
}

std::pair<PROGRESS_MAIN, PROGRESS_SUB> TutorialManager::getTutorialProgress()
{
    return _nowTutorialProgress;
}

void TutorialManager::saveTutorialProgress()
{
    std::stringstream str;
    std::string key;
    
    key = KEY_TUTORIAL_PROGRESS;
    str << _nowTutorialProgress.first << "_" << _nowTutorialProgress.second;
    std::string nowkey = key;
    UserDefault::getInstance()->setStringForKey(nowkey.c_str(), MafAes256::Encrypt(str.str()));
}

void TutorialManager::loadTutorialProgress()
{
    std::vector<std::string> vec;
    
    auto save = UserDefault::getInstance()->getStringForKey(KEY_TUTORIAL_PROGRESS);
    save = MafAes256::Decrypt(save);
    
    vec = MafUtils::split(save, '_');
    if( vec.empty() == false )
    {
        _nowTutorialProgress = std::pair<PROGRESS_MAIN, PROGRESS_SUB>((PROGRESS_MAIN)atoi(vec.at(0).c_str()),(PROGRESS_SUB)atoi(vec.at(1).c_str()));
    }
}

void TutorialManager::setPosition(Vec2 position)
{
    if ( _tutorialObject != nullptr )
    {
        _tutorialObject->setPosition(position);
    }
}

#pragma mark - draw

void TutorialManager::drawTutorial(bool refresh)
{
    PROGRESS_MAIN main;
    PROGRESS_SUB sub;

    PROGRESS_MAIN main_before;
    PROGRESS_SUB sub_before;

    main = _nowTutorialProgress.first;
    sub = _nowTutorialProgress.second;

    main_before = main;
    sub_before = sub;

    if(SpecialMissionManager::getInstance()->getSpecialMissionClear() >= 98)
    {
        main = PROGRESS_END;
        sub = SUB_NONE;
    }
    else
    {
        switch ( main_before )
        {
            case PROGRESS_NONE:
            {
            }
            case PROGRESS_FLOOR:
            {
                if(SpecialMissionManager::getInstance()->getSpecialMissionClear() == 9)
                {
                    if(UserInfoManager::getInstance()->getFloor(E_PLACE::DG_NORMAL) < 25)
                    {
                        sub = SUB_NONE;
                        main = PROGRESS_FLOOR;
                        break;
                    }
                    
                    sub = SUB_REWARD_BUTTON;
                    main = PROGRESS_FLOOR;
                    break;
                }
            }
            case PROGRESS_QUEST:
            {
                
                if(SpecialMissionManager::getInstance()->getSpecialMissionClear() == 0)
                {
                    if ( UserInfoManager::getInstance()->getQuestLevel(E_PLACE::DG_NORMAL,QUEST_1) < 10 )
                    {
                        sub = SUB_OBJECT_1;
                        main = PROGRESS_QUEST;
                        break;
                    }
                    sub = SUB_REWARD_BUTTON;
                    main = PROGRESS_QUEST;
                    break;
                }
                
                else if(SpecialMissionManager::getInstance()->getSpecialMissionClear() == 1)
                {
                    if ( sub == PROGRESS_SUB::SUB_SHORTCUT_BUTTON )
                    {
                        main = PROGRESS_QUEST;
                        break;
                    }

                    if ( UserInfoManager::getInstance()->getQuestLevel(E_PLACE::DG_NORMAL,QUEST_2) < 20 )
                    {
                        sub = SUB_OBJECT_1;
                        main = PROGRESS_QUEST;
                        break;
                    }
                    sub = SUB_REWARD_BUTTON;
                    main = PROGRESS_QUEST;
                    break;
                }
                else if(SpecialMissionManager::getInstance()->getSpecialMissionClear() == 2)
                {
                    if ( sub == PROGRESS_SUB::SUB_SHORTCUT_BUTTON )
                    {
                        main = PROGRESS_QUEST;
                        break;
                    }

                    if ( UserInfoManager::getInstance()->getQuestLevel(E_PLACE::DG_NORMAL,QUEST_3) < 20 )
                    {
                        sub = SUB_OBJECT_1;
                        main = PROGRESS_QUEST;
                        break;
                    }
                    sub = SUB_REWARD_BUTTON;
                    main = PROGRESS_QUEST;
                    break;
                }
                else if(SpecialMissionManager::getInstance()->getSpecialMissionClear() == 4)
                {
                    if ( sub == PROGRESS_SUB::SUB_SHORTCUT_BUTTON )
                    {
                        main = PROGRESS_QUEST;
                        break;
                    }

                    if ( UserInfoManager::getInstance()->getQuestLevel(E_PLACE::DG_NORMAL,QUEST_4) < 10 )
                    {
                        sub = SUB_OBJECT_1;
                        main = PROGRESS_QUEST;
                        break;
                    }
                    sub = SUB_REWARD_BUTTON;
                    main = PROGRESS_QUEST;
                    break;
                }
                else if(SpecialMissionManager::getInstance()->getSpecialMissionClear() == 5)
                {
                    if ( sub == PROGRESS_SUB::SUB_SHORTCUT_BUTTON )
                    {
                        main = PROGRESS_QUEST;
                        break;
                    }

                    if ( UserInfoManager::getInstance()->getQuestLevel(E_PLACE::DG_NORMAL,QUEST_5) < 5 )
                    {
                        sub = SUB_OBJECT_1;
                        main = PROGRESS_QUEST;
                        break;
                    }
                    sub = SUB_REWARD_BUTTON;
                    main = PROGRESS_QUEST;
                    break;
                }
                else if(SpecialMissionManager::getInstance()->getSpecialMissionClear() == 7)
                {
                    if ( sub == PROGRESS_SUB::SUB_SHORTCUT_BUTTON )
                    {
                        main = PROGRESS_QUEST;
                        break;
                    }

                    if ( UserInfoManager::getInstance()->getQuestLevel(E_PLACE::DG_NORMAL,QUEST_6) < 1 )
                    {
                        sub = SUB_OBJECT_1;
                        main = PROGRESS_QUEST;
                        break;
                    }
                    sub = SUB_REWARD_BUTTON;
                    main = PROGRESS_QUEST;
                    break;
                }
                
                else if(SpecialMissionManager::getInstance()->getSpecialMissionClear() == 8)
                {
                    if ( sub == PROGRESS_SUB::SUB_SHORTCUT_BUTTON )
                    {
                        main = PROGRESS_QUEST;
                        break;
                    }

                    if ( UserInfoManager::getInstance()->getQuestLevel(E_PLACE::DG_NORMAL,QUEST_5) < 15 )
                    {
                        sub = SUB_OBJECT_1;
                        main = PROGRESS_QUEST;
                        break;
                    }
                    sub = SUB_REWARD_BUTTON;
                    main = PROGRESS_QUEST;
                    break;
                }
                else if(SpecialMissionManager::getInstance()->getSpecialMissionClear() == 10)
                {
                    if ( sub == PROGRESS_SUB::SUB_SHORTCUT_BUTTON )
                    {
                        main = PROGRESS_QUEST;
                        break;
                    }

                    if ( UserInfoManager::getInstance()->getQuestLevel(E_PLACE::DG_NORMAL,QUEST_2) < 30 )
                    {
                        sub = SUB_OBJECT_1;
                        main = PROGRESS_QUEST;
                        break;
                    }
                    sub = SUB_REWARD_BUTTON;
                    main = PROGRESS_QUEST;
                    break;
                }
                else if(SpecialMissionManager::getInstance()->getSpecialMissionClear() == 11)
                {
                    if ( sub == PROGRESS_SUB::SUB_SHORTCUT_BUTTON )
                    {
                        main = PROGRESS_QUEST;
                        break;
                    }

                    if ( UserInfoManager::getInstance()->getQuestLevel(E_PLACE::DG_NORMAL,QUEST_4) < 50 )
                    {
                        sub = SUB_OBJECT_1;
                        main = PROGRESS_QUEST;
                        break;
                    }
                    sub = SUB_REWARD_BUTTON;
                    main = PROGRESS_QUEST;
                    break;
                }
                
            }
            case PROGRESS_WEAPON:
            {
                if(SpecialMissionManager::getInstance()->getSpecialMissionClear() == 3)
                {
                    if( WeaponManager::getInstance()->getEquip() <= E_WEAPON::WEAPON_1 && WeaponManager::getInstance()->getLevel(WeaponManager::getInstance()->getEquip()) < 5)
                    {
                        sub = SUB_OBJECT_1;
                        main = PROGRESS_WEAPON;
                        break;
                    }
                    else if( WeaponManager::getInstance()->getEquip() <= E_WEAPON::WEAPON_1 && WeaponManager::getInstance()->getLevel(WeaponManager::getInstance()->getEquip()) >= 5)
                    {
                        sub = SUB_OBJECT_2;
                        main = PROGRESS_WEAPON;
                        break;
                    }
                    else if( WeaponManager::getInstance()->getEquip() <= E_WEAPON::WEAPON_2 && WeaponManager::getInstance()->getLevel(WeaponManager::getInstance()->getEquip()) < 5)
                    {
                        sub = SUB_OBJECT_2;
                        main = PROGRESS_WEAPON;
                        break;
                    }
                    else if( WeaponManager::getInstance()->getEquip() <= E_WEAPON::WEAPON_2 && WeaponManager::getInstance()->getLevel(WeaponManager::getInstance()->getEquip()) >= 5)
                    {
                        sub = SUB_OBJECT_3;
                        main = PROGRESS_WEAPON;
                        break;
                    }
                    sub = SUB_REWARD_BUTTON;
                    main = PROGRESS_WEAPON;
                    break;
                }
                if(SpecialMissionManager::getInstance()->getSpecialMissionClear() == 6)
                {
                    if( WeaponManager::getInstance()->getEquip() <= E_WEAPON::WEAPON_3 && WeaponManager::getInstance()->getLevel(WeaponManager::getInstance()->getEquip()) < 5)
                    {
                        sub = SUB_OBJECT_1;
                        main = PROGRESS_WEAPON;
                        break;
                    }
                    else if( WeaponManager::getInstance()->getEquip() <= E_WEAPON::WEAPON_3 && WeaponManager::getInstance()->getLevel(WeaponManager::getInstance()->getEquip()) >= 5)
                    {
                        sub = SUB_OBJECT_2;
                        main = PROGRESS_WEAPON;
                        break;
                    }
                    else if( WeaponManager::getInstance()->getEquip() <= E_WEAPON::WEAPON_4 && WeaponManager::getInstance()->getLevel(WeaponManager::getInstance()->getEquip()) < 5)
                    {
                        sub = SUB_OBJECT_2;
                        main = PROGRESS_WEAPON;
                        break;
                    }
                    else if( WeaponManager::getInstance()->getEquip() <= E_WEAPON::WEAPON_4 && WeaponManager::getInstance()->getLevel(WeaponManager::getInstance()->getEquip()) >= 5)
                    {
                        sub = SUB_OBJECT_3;
                        main = PROGRESS_WEAPON;
                        break;
                    }
                    sub = SUB_REWARD_BUTTON;
                    main = PROGRESS_WEAPON;
                    break;
                }
                if(SpecialMissionManager::getInstance()->getSpecialMissionClear() == 82 )
                {
                    if(WeaponManager::getInstance()->getEquipPermanent() < 2)
                    {
                        sub = SUB_OBJECT_1;
                        main = PROGRESS_WEAPON;
                        break;
                    }
                    sub = SUB_REWARD_BUTTON;
                    main = PROGRESS_WEAPON;
                    break;
                }
                if(SpecialMissionManager::getInstance()->getSpecialMissionClear() == 91)
                {
                    if(WeaponManager::getInstance()->getEquipPermanent() < 3)
                    {
                        sub = SUB_OBJECT_1;
                        main = PROGRESS_WEAPON;
                        break;
                    }
                    sub = SUB_REWARD_BUTTON;
                    main = PROGRESS_WEAPON;
                    break;
                }
                
            }
            case PROGRESS_ARTIFACT:
            {
                if(SpecialMissionManager::getInstance()->getSpecialMissionClear() == 34)
                {
                    if(UserInfoManager::getInstance()->getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_1_EVOL1) < 20)
                    {
                        sub = SUB_OBJECT_1;
                        main = PROGRESS_ARTIFACT;
                        break;
                    }
                    sub = SUB_REWARD_BUTTON;
                    main = PROGRESS_ARTIFACT;
                    break;
                }
                else if(SpecialMissionManager::getInstance()->getSpecialMissionClear() == 36)
                {
                    if(UserInfoManager::getInstance()->getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_3_EVOL1) < 10)
                    {
                        sub = SUB_OBJECT_1;
                        main = PROGRESS_ARTIFACT;
                        break;
                    }
                    sub = SUB_REWARD_BUTTON;
                    main = PROGRESS_ARTIFACT;
                    break;
                }
            }
            case PROGRESS_PET:
            {
                if(SpecialMissionManager::getInstance()->getSpecialMissionClear() == 35)
                {
                    if(SpecialMissionManager::getInstance()->getSpecialMissionComplete() != true)
                    {
                        sub = SUB_OBJECT_1;
                        main = PROGRESS_PET;
                        break;
                    }
                    sub = SUB_REWARD_BUTTON;
                    main = PROGRESS_PET;
                    break;
                }
            }
            case PROGRESS_COSTUME:
            {
                if(SpecialMissionManager::getInstance()->getSpecialMissionClear() == 38)
                {
                    if(CostumeManager::getInstance()->isExist(E_COSTUME::HEAD, E_COSTUME_IDX::IDX_10) == false)
                    {
                        sub = SUB_OBJECT_1;
                        main = PROGRESS_COSTUME;
                        break;
                    }
                    sub = SUB_REWARD_BUTTON;
                    main = PROGRESS_COSTUME;
                    break;
                }
            }
            case PROGRESS_PRISON:
            {
                if(SpecialMissionManager::getInstance()->getSpecialMissionClear() == 37)
                {
                    if(SpecialMissionManager::getInstance()->getSpecialMissionComplete() != true)
                    {
                        sub = SUB_OBJECT_1;
                        main = PROGRESS_PRISON;
                        break;
                    }
                    sub = SUB_REWARD_BUTTON;
                    main = PROGRESS_PRISON;
                    break;
                }
            }
            case PROGRESS_RAID:
            {
                if(SpecialMissionManager::getInstance()->getSpecialMissionClear() == 90)
                {
                    if(SpecialMissionManager::getInstance()->getSpecialMissionComplete() != true)
                    {
                        sub = SUB_OBJECT_1;
                        main = PROGRESS_RAID;
                        break;
                    }
                    sub = SUB_REWARD_BUTTON;
                    main = PROGRESS_RAID;
                    break;
                }
            }
            default:
            break;

        }
    }

    if( _nowTutorialProgress != std::pair<PROGRESS_MAIN, PROGRESS_SUB>(main, sub))
    {
        _nowTutorialProgress = std::pair<PROGRESS_MAIN, PROGRESS_SUB>(main, sub);
        saveTutorialProgress();
    }

    auto layer = UtilsScene::getInstance()->getRunningLayer();

    if (std::pair<PROGRESS_MAIN, PROGRESS_SUB>(main_before, sub_before) != std::pair<PROGRESS_MAIN, PROGRESS_SUB>(main, sub) )
    {
        refresh = true;
    }
    
    if( refresh == true)
    {
        if ( dynamic_cast<HelloWorldScene *>(layer) != nullptr )
        {
            dynamic_cast<HelloWorldScene *>(layer)->redrawTutorialObject();
        }
        
        // refresh
        RefreshManager::getInstance()->refreshUI(E_REFRESH::TUTORIAL);
    }
    
}

void TutorialManager::setFirstReviver(bool active)
{
    _firstReviver = active;
}
bool TutorialManager::getFirstReviver()
{
    return _firstReviver;
}

void TutorialManager::setOpenData(E_STEPWISE_OPEN_TYPE type, int value, bool active)
{
    std::string key = "";
    
    switch (type) {
        case E_STEPWISE_OPEN_TYPE::ST_SPECIAL_MISSION:
        {
            key = "SP_" + MafUtils::toString(value);
        }
            break;
        case E_STEPWISE_OPEN_TYPE::ST_FLOOR:
        {
            key = "FLOOR_" + MafUtils::toString(value);;
        }
            break;
        default:
            break;
    }
    auto iter = _mapTutorialOpenData.find(key);
    
    if(iter == _mapTutorialOpenData.end())
    {
        _mapTutorialOpenData.insert(std::make_pair(key, active));
        return;
    }
    
    iter->second = active;
}

void TutorialManager::setOpenData(std::string key, bool active)
{
    
    auto iter = _mapTutorialOpenData.find(key);
    
    if(iter == _mapTutorialOpenData.end())
    {
        _mapTutorialOpenData.insert(std::make_pair(key, active));
        return;
    }
    
    iter->second = active;
}

bool TutorialManager::getOpenData(E_STEPWISE_OPEN_TYPE type, int value)
{
    
    std::string key = "";
    
    switch (type) {
        case E_STEPWISE_OPEN_TYPE::ST_SPECIAL_MISSION:
        {
            key = "SP_" + MafUtils::toString(value);
        }
            break;
        case E_STEPWISE_OPEN_TYPE::ST_FLOOR:
        {
            key = "FLOOR_" + MafUtils::toString(value);;
        }
            break;
        default:
            break;
    }
    auto iter = _mapTutorialOpenData.find(key);
    
    if(iter == _mapTutorialOpenData.end())
        return false;
    
    return iter->second;
}

bool TutorialManager::getOpenData(std::string key)
{
    
    auto iter = _mapTutorialOpenData.find(key);
    
    if(iter == _mapTutorialOpenData.end())
        return false;
    
    return iter->second;
}
