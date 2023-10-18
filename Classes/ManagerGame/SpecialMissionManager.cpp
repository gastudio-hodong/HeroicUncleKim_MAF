//
//  SpecialMissionManager.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/08/29.
//

#include "SpecialMissionManager.h"

#include "Common/ConfigKey.h"

#include "GameScene/HelloWorldScene.h"

#include "GameUIPopup/Common/MenuPopup.h"
#include "GameUIPopup/Other/BattlePrison/PopupPrison.h"
#include "GameUIPopup/Other/Settings/PopupReviewMarket.h"
#include "GameUIPopup/Other/SubFunction/PopupSubFunction.h"
#include "GameUIPopup/Other/Costume/PopupCostumeDefault.h"
#include "GameUIPopup/Other/Friend/PopupFriend.h"
#include "GameUIPopup/Other/NewRaid/PopupRaidDefault.h"

#include "ManagerGame/UserInfoManager.h"
#include "ManagerGame/ItemsManager.h"
#include "ManagerGame/AccountManager.h"
#include "ManagerGame/SaveManager.h"
#include "ManagerGame/NewRaidManager.h"
#include "ManagerGame/CostumeManager.h"
#include "ManagerGame/PrisonManager.h"
#include "ManagerGame/PetNewManager.h"
#include "ManagerGame/WeaponManager.h"

USING_NS_CC;

SpecialMissionManager::SpecialMissionManager():
_nSpecialMissionClear(0),
_nSpecialMissionComplete(0),
_callbackReward(nullptr)
{
    _listSpecialMission.clear();
}
SpecialMissionManager::~SpecialMissionManager(void)
{
    _listSpecialMission.clear();
}

bool SpecialMissionManager::init()
{
    setLoadMission();
    _nSpecialMissionClear = std::atoi(UserDefault::getInstance()->getStringForKey(KEY_SPECIAL_CLEAR, "0").c_str());
    _nSpecialMissionComplete = std::atoi(UserDefault::getInstance()->getStringForKey(KEY_SPECIAL_COMPLETE, "0").c_str());
    return true;
}

#pragma mark- Load
void SpecialMissionManager::setLoadMission()
{
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::SPECIAL_MISSION);
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)strText.c_str());
    if( jsonParser.HasParseError() == true )
    {
        return;
    }
    
    _listSpecialMission.clear();
    std::string strCurType = "";
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];
        
        if ( MafNative::IsLessThanCurrVersion(jsonValue["version"].GetString()) == false )
            continue;
        
        const int nIdx = jsonValue["idx"].GetInt();
        const int nShortcuts = jsonValue["shortcuts"].GetInt();
        const bool isServerReward = (bool)jsonValue["is_server"].GetInt();
        const std::string strContents = jsonValue["contents_key"].GetString();
        const std::string strReward = jsonValue["reward"].GetString();
        
        auto obj = InfoSpecialMission::create();
        obj->setIdx(nIdx);
        obj->setContent(strContents);
        obj->setShortcutType((E_SHORTCUTS)nShortcuts);
        obj->setIsServerReward(isServerReward);
        obj->setListMissionReward(ItemsManager::getInstance()->getConvertArray(strReward));
        
        _listSpecialMission.pushBack(obj);
    }
}
Vector<InfoSpecialMission*> SpecialMissionManager::getListSpecialMission()
{
    return _listSpecialMission;
}
InfoSpecialMission* SpecialMissionManager::getInfoSpecialMission(const int idx)
{
    InfoSpecialMission* result = nullptr;
    
    for(auto obj : _listSpecialMission)
    {
        if(obj->getIdx() == idx)
            return obj;
    }
    
    return result;
}
#pragma mark - SpecialMission
void SpecialMissionManager::setSpecialMissionClear(const int value, const bool bSave)
{
    _nSpecialMissionClear = value;
    if (bSave) {
        UserDefault::getInstance()->setStringForKey(KEY_SPECIAL_CLEAR, MafAes256::Encrypt(MafUtils::toString(_nSpecialMissionClear)));
    }
}
const int SpecialMissionManager::getSpecialMissionClear()const
{
    return _nSpecialMissionClear;
}

void SpecialMissionManager::setSpecialMissionComplete(const int value, const bool bSave)
{
    _nSpecialMissionComplete = value;
    if (bSave) {
        UserDefault::getInstance()->setStringForKey(KEY_SPECIAL_COMPLETE, MafAes256::Encrypt(MafUtils::toString(_nSpecialMissionComplete)));
    }
}
const int SpecialMissionManager::getSpecialMissionComplete() const
{
    return _nSpecialMissionComplete;
}
void SpecialMissionManager::clearSpecialMission(const int missionIdx)
{
    int mission = getSpecialMissionClear()+1;
    if (mission != missionIdx)
    {
        return;
    }
    
    setSpecialMissionComplete(1);
    
    auto sceneLayer = UtilsScene::getInstance()->getRunningLayer();
    auto sceneTemp = dynamic_cast<HelloWorldScene *>(sceneLayer);
    if ( sceneTemp != nullptr )
    {
        sceneTemp->CompleteSpecialMission(true);
        sceneTemp->reDrawSpecialQuest();
    }
}

void SpecialMissionManager::checkSpeicalMission(const E_PLACE ePlace)
{
    int mission = getSpecialMissionClear()+1;
    if (mission > _listSpecialMission.size()) {

        return;
    }
    
    auto sceneLayer = UtilsScene::getInstance()->getRunningLayer();
    auto sceneTemp = dynamic_cast<HelloWorldScene *>(sceneLayer);
    if ( sceneTemp != nullptr )
    {
        sceneTemp->CompleteSpecialMission(getSpecialMissionComplete());
    }
    
    
    bool clear = isCompleteSpeicalMission(ePlace);
    if (clear)
    {
        clearSpecialMission(mission);
    }
}

const bool SpecialMissionManager::isCompleteSpeicalMission(const E_PLACE ePlace)const
{
    bool clear = false;
    int count = 0;
    
    auto userInfo = UserInfoManager::getInstance();
    int mission = getSpecialMissionClear()+1;
    switch (mission) {
        case 1:
            if(userInfo->getQuestLevel(ePlace ,E_QUEST::QUEST_1) >= 10)
                clear = true;
            break;
        case 2:
            if(userInfo->getQuestLevel(ePlace, E_QUEST::QUEST_2) >= 20)
                clear = true;
            break;
        case 3:
            if(userInfo->getQuestLevel(ePlace, E_QUEST::QUEST_3) >= 20)
                clear = true;
            break;
        case 4:
            if(WeaponManager::getInstance()->getEquip() >= E_WEAPON::WEAPON_3)
                clear = true;
            break;
        case 5:
            if(userInfo->getQuestLevel(ePlace, E_QUEST::QUEST_4) >= 10)
                clear = true;
            break;
        case 6:
            if(userInfo->getQuestLevel(ePlace, E_QUEST::QUEST_5) >= 5)
                clear = true;
            break;
        case 7:
            if(WeaponManager::getInstance()->getEquip() >= E_WEAPON::WEAPON_5)
                clear = true;
            break;
        case 8:
            if(userInfo->getQuestLevel(ePlace, E_QUEST::QUEST_6) >= 1)
                clear = true;
            break;
        case 9:
            if(userInfo->getQuestLevel(ePlace, E_QUEST::QUEST_5) >= 15)
                clear = true;
            break;
        case 10:
            if(userInfo->getHighFloor(ePlace) >= 25)
                clear = true;
            break;
        case 11:
            if(userInfo->getQuestLevel(ePlace, E_QUEST::QUEST_2) >= 30)
                clear = true;
            break;
        case 12:
        if(userInfo->getQuestLevel(ePlace, E_QUEST::QUEST_4) >= 50)
                clear = true;
            break;
        case 13:
            if(WeaponManager::getInstance()->getEquip() >= E_WEAPON::WEAPON_7)
                clear = true;
            break;
        case 14:
            if(userInfo->getQuestLevel(ePlace, E_QUEST::QUEST_5) >= 20)
                clear = true;
            break;
        case 15:
            if(userInfo->getQuestLevel(ePlace, E_QUEST::QUEST_6) >= 15)
                clear = true;
            break;
        case 16:
            if(userInfo->getQuestLevel(ePlace, E_QUEST::QUEST_7) >= 5)
                clear = true;
            break;
        case 17:
            if(WeaponManager::getInstance()->getEquip() >= E_WEAPON::WEAPON_9)
                clear = true;
            break;
        case 18:
            if(userInfo->getQuestLevel(ePlace, E_QUEST::QUEST_6) >= 30)
                clear = true;
            break;
        case 19:
            if(userInfo->getHighFloor(ePlace) >= 50)
                clear = true;
            break;
        case 20:
            if(userInfo->getQuestLevel(ePlace, E_QUEST::QUEST_7) >= 10)
                clear = true;
            break;
        case 21:
            if(userInfo->getQuestLevel(ePlace, E_QUEST::QUEST_8) >= 5)
                clear = true;
            break;
        case 22:
            if(userInfo->getQuestLevel(ePlace, E_QUEST::QUEST_3) >= 100)
                clear = true;
            break;
        case 23:
            if(userInfo->getQuestLevel(ePlace, E_QUEST::QUEST_5) >= 50)
                clear = true;
            break;
        case 24:
            if(WeaponManager::getInstance()->getEquip() >= E_WEAPON::WEAPON_10)
                clear = true;
            break;
        case 25:
            if(userInfo->getQuestLevel(ePlace, E_QUEST::QUEST_7) >= 20)
                clear = true;
            break;
        case 26:
            if(WeaponManager::getInstance()->getEquip() >= E_WEAPON::WEAPON_11)
                clear = true;
            break;
        case 27:
            if(userInfo->getQuestLevel(ePlace, E_QUEST::QUEST_7) >= 25)
                 clear = true;
            break;
        case 28:
            if(userInfo->getHighFloor(ePlace) >= 80)
                clear = true;
            break;
        case 29:
            if(userInfo->getQuestLevel(ePlace, E_QUEST::QUEST_6) >= 50)
                clear = true;
            break;
        case 30:
            if(userInfo->getQuestLevel(ePlace, E_QUEST::QUEST_8) >= 10)
                clear = true;
            break;
        case 31:
            if(userInfo->getQuestLevel(ePlace, E_QUEST::QUEST_6) >= 55)
                clear = true;
            break;
        case 32:
            if(userInfo->getQuestLevel(ePlace, E_QUEST::QUEST_9) >= 1)
                clear = true;
            break;

        case 33:
            if(userInfo->getHighFloor(ePlace) >= 101)
                clear = true;
            break;
        case 34:
            if(userInfo->getRevival(ePlace) >= 2 || userInfo->getHighFloor(ePlace) >= 120)
                clear = true;
            break;
        case 35:
            if(userInfo->getArtifactLevel(ePlace, E_ARTIFACT::ARTI_1_EVOL1) >= 20)
                clear = true;
            break;
        case 36:
            if(PetNewManager::getInstance()->getPetLevel(E_PET::CHICK_1) >= 2)
                clear = true;
            break;
        case 37:
            if(userInfo->getArtifactLevel(ePlace, E_ARTIFACT::ARTI_3_EVOL1) >= 10)
                clear = true;
            break;
        case 38:
            break;
        case 39:
            if(CostumeManager::getInstance()->getExist(E_COSTUME::HEAD, E_COSTUME_IDX::IDX_10))
                clear = true;
            break;
        case 40:
            if(userInfo->getArtifactLevel(ePlace, E_ARTIFACT::ARTI_8_EVOL1) >= 20)
                clear = true;
            break;
        case 41:
            if(userInfo->getArtifactLevel(ePlace, E_ARTIFACT::ARTI_4_EVOL1) >= 20)
                clear = true;
            break;
        case 42:
            if(PrisonManager::getInstance()->getDungeonKill() >= 50)
                clear = true;
            break;
        case 43:
        {
            count = 0;
            for (int i=1; i<=C_COUNT_COSTUME_TYPE; i++) {
                count += CostumeManager::getInstance()->getExistTypeCount((E_COSTUME)i);
            }
            if(count >= 3)
                clear = true;
        }
            break;
        case 44:
            if(userInfo->getQuestLevel(ePlace, E_QUEST::QUEST_7) >= 1)
                clear = true;
            break;
        case 45:
            if(userInfo->getArtifactLevel(ePlace, E_ARTIFACT::ARTI_4_EVOL1) >= 50)
                clear = true;
            break;
        case 46:
            if(userInfo->getArtifactLevel(ePlace, E_ARTIFACT::ARTI_1_EVOL1) >= 100)
                clear = true;
            break;
        case 47:
            if(WeaponManager::getInstance()->getEquip() >= E_WEAPON::WEAPON_12)
                clear = true;
            break;
        case 48:
            if(userInfo->getQuestLevel(ePlace, E_QUEST::QUEST_9) >= 5)
                clear = true;
            break;
        case 49:
            if(userInfo->getQuestLevel(ePlace, E_QUEST::QUEST_10) >= 1)
                clear = true;
            break;
        case 50://지감 10회
            break;
        case 51:
            if(userInfo->getArtifactLevel(ePlace, E_ARTIFACT::ARTI_3_EVOL1) >= 50)
                clear = true;
            break;
        case 52:
            if(userInfo->getArtifactLevel(ePlace, E_ARTIFACT::ARTI_8_EVOL1) >= 50)
                clear = true;
            break;
        case 53:
            if(userInfo->getQuestLevel(ePlace, E_QUEST::QUEST_9) >= 30)
                clear = true;
            break;
        case 54:
            if(WeaponManager::getInstance()->getEquip() >= E_WEAPON::WEAPON_15)
                clear = true;
            break;
        case 55:
            if(userInfo->getArtifactLevel(ePlace, E_ARTIFACT::ARTI_1_EVOL1) >= 250)
                clear = true;
            break;
        case 56:
            if(userInfo->getArtifactLevel(ePlace, E_ARTIFACT::ARTI_4_EVOL1) >= 100)
                clear = true;
            break;
        case 57:
            if(userInfo->getQuestLevel(ePlace, E_QUEST::QUEST_11) >= 5)
                clear = true;
            break;
        case 58:
            if(userInfo->getArtifactLevel(ePlace, E_ARTIFACT::ARTI_3_EVOL1) >= 100)
                clear = true;
            break;
        case 59:
            if(userInfo->getQuestLevel(ePlace, E_QUEST::QUEST_12) >= 10)
                clear = true;
            break;
        case 60:
            if(userInfo->getArtifactLevel(ePlace, E_ARTIFACT::ARTI_8_EVOL1) >= 100)
                clear = true;
            break;
        case 61:
            if(userInfo->getQuestLevel(ePlace, E_QUEST::QUEST_13) >= 20)
                clear = true;
            break;
        case 62:
            if(userInfo->getQuestLevel(ePlace, E_QUEST::QUEST_14) >= 10)
                clear = true;
            break;
        case 63:
            if(WeaponManager::getInstance()->getEquip() >= E_WEAPON::WEAPON_20)
                clear = true;
            break;
        case 64:
            if(userInfo->getArtifactLevel(ePlace, E_ARTIFACT::ARTI_5) >= 10)
                clear = true;
            break;
        case 65:
            if(userInfo->getArtifactLevel(ePlace, E_ARTIFACT::ARTI_7) >= 10)
                clear = true;
            break;
        case 66:
            if(userInfo->getArtifactLevel(ePlace, E_ARTIFACT::ARTI_6) >= 10)
                clear = true;
            break;
        case 67:
            if(userInfo->getArtifactLevel(ePlace, E_ARTIFACT::ARTI_2) >= 30)
                clear = true;
            break;
        case 68:
            if(userInfo->getQuestLevel(ePlace, E_QUEST::QUEST_16) >= 5)
                clear = true;
            break;
        case 69:
            if (WeaponManager::getInstance()->getEquip() >= E_WEAPON::WEAPON_25)
                clear = true;
            break;
        case 70:
            if(PrisonManager::getInstance()->getDungeonKill() >= 200)
                clear = true;
            break;
        case 71:
            if(userInfo->getFloor(ePlace) >= 200)
                clear = true;
            break;
        case 72:
            {
                count = 0;
                for (int i=1; i<=C_COUNT_COSTUME_TYPE; i++) {
                    count += CostumeManager::getInstance()->getExistTypeCount((E_COSTUME)i);
                }
                if(count >= 4)
                    clear = true;
            }
            break;
        case 73:
            if(userInfo->getArtifactLevel(ePlace, E_ARTIFACT::ARTI_6) >= 40)
                clear = true;
            break;
        case 74:
            if(userInfo->getArtifactLevel(ePlace, E_ARTIFACT::ARTI_2) >= 45)
                clear = true;
            break;
        case 75:
            if(userInfo->getFloor(ePlace) >= 300)
                clear = true;
            break;
        case 76:
            if(userInfo->getQuestLevel(ePlace, E_QUEST::QUEST_18) >= 30)
                clear = true;
            break;
        case 77:
            if (WeaponManager::getInstance()->getEquip() >= E_WEAPON::WEAPON_30)
                clear = true;
            break;
        case 78:
            if(userInfo->getArtifactLevel(ePlace, E_ARTIFACT::ARTI_1_EVOL1) >= 350)
                clear = true;
            break;
        case 79:
            if(userInfo->getArtifactLevel(ePlace, E_ARTIFACT::ARTI_3_EVOL1) >= 300)
                clear = true;
            break;
        case 80://지감 20회
            break;
        case 81:
            if(userInfo->getQuestLevel(ePlace, E_QUEST::QUEST_19) >= 50)
                clear = true;
            break;
        case 82:
            if(userInfo->getQuestLevel(ePlace, E_QUEST::QUEST_20) >= 70)
                clear = true;
            break;
        case 83:
            if (WeaponManager::getInstance()->getEquipPermanent() >= E_WEAPON::WEAPON_2 )
                clear = true;
            break;
        case 84:
            break;
        case 85:
            break;
        case 86:
            if(userInfo->getQuestLevel(ePlace, E_QUEST::QUEST_21) >= 30)
                clear = true;
            break;
        case 87:
            if(userInfo->getArtifactLevel(ePlace, E_ARTIFACT::ARTI_8_EVOL1) >= 400)
                clear = true;
            break;
        case 88:
            if(userInfo->getArtifactLevel(ePlace, E_ARTIFACT::ARTI_4_EVOL1) >= 400)
                clear = true;
            break;
        case 89:
            if(userInfo->getQuestLevel(ePlace, E_QUEST::QUEST_22) >= 30)
                clear = true;
            break;
        case 90:
            if (WeaponManager::getInstance()->getEquip() >= E_WEAPON::WEAPON_40)
                clear = true;
            break;
        case 91://레이드1회
            break;
        case 92:
            if (WeaponManager::getInstance()->getEquipPermanent() >= E_WEAPON::WEAPON_3)
                clear = true;
            break;
        case 93:
            break;
        case 94:
            if(userInfo->getFloor(ePlace) >= 500)
                clear = true;
            break;
        case 95:
            if(userInfo->getQuestLevel(ePlace, E_QUEST::QUEST_23) >= 5)
                clear = true;
            break;
        case 96://지감 25회
            break;
        case 97:
           if ( userInfo->getFloor(ePlace) >= 601 )
                clear = true;
            break;
        case 98:
            if(PetNewManager::getInstance()->getPetLevel(E_PET::HAMSTER_2) >= 3)
                clear = true;
            break;
        case 99:
            if (WeaponManager::getInstance()->getEquipPermanent() >= E_WEAPON::WEAPON_10)
                clear = true;
            break;
        case 100:
            if(userInfo->getArtifactLevel(ePlace, E_ARTIFACT::ARTI_89_DISCOUNT_1) >= 1)
                clear = true;
            break;
        case 101:
            if(userInfo->getArtifactLevel(ePlace, E_ARTIFACT::ARTI_1_EVOL1) >= 800)
                clear = true;
            break;
        case 102:
            if (WeaponManager::getInstance()->getEquipPermanent() >= E_WEAPON::WEAPON_20)
                clear = true;
            break;
        case 103:
            if(userInfo->getFloor(ePlace) >= 800)
                 clear = true;
            break;
        case 104:
            if (WeaponManager::getInstance()->getEquip() >= E_WEAPON::WEAPON_46)
                clear = true;
            break;
        case 105:
            if (WeaponManager::getInstance()->getEquip() >= E_WEAPON::WEAPON_51)
                clear = true;
            break;
        case 106:
            if(PrisonManager::getInstance()->getDungeonKill() >= 500)
                clear = true;
            break;
        case 107:
            if(userInfo->getArtifactLevel(ePlace, E_ARTIFACT::ARTI_3_EVOL1) >= 2000)
                clear = true;
            break;
        case 108:
            if(PetNewManager::getInstance()->getPetLevel(E_PET::PRINCESS_4) >= 2)
                clear = true;
            break;
        case 109:
            if(userInfo->getArtifactLevel(ePlace, E_ARTIFACT::ARTI_8_EVOL1) >= 2000)
                clear = true;
            break;
        case 110:
            if (WeaponManager::getInstance()->getEquip() >= E_WEAPON::WEAPON_56)
                clear = true;
            break;
        case 111:
            if (WeaponManager::getInstance()->getEquipPermanent() >= E_WEAPON::WEAPON_25)
                clear = true;
            break;
        case 112:
            if(userInfo->getFloor(ePlace) >= 1500)
                clear = true;
            break;
        case 113:
            if(userInfo->getArtifactLevel(ePlace, E_ARTIFACT::ARTI_1_EVOL1) >= 10000)
                clear = true;
            break;
        case 114:
            if(PetNewManager::getInstance()->getPetLevel(E_PET::PRINCESS_4) >= 5)
                clear = true;
            break;
        case 115://getEquipPermanent
            if (WeaponManager::getInstance()->getEquip() >= E_WEAPON::WEAPON_59)
                clear = true;
            break;
        case 116:
            if (WeaponManager::getInstance()->getEquipPermanent() >= E_WEAPON::WEAPON_40)
                clear = true;
            break;
        case 117:
            if(userInfo->getArtifactLevel(ePlace, E_ARTIFACT::ARTI_3_EVOL1) >= 10000)
                clear = true;
            break;
        case 118:
            if (WeaponManager::getInstance()->getEquip() >= E_WEAPON::WEAPON_66)
                clear = true;
            break;
        case 119:
            if (WeaponManager::getInstance()->getEquipPermanent() >= E_WEAPON::WEAPON_50)
                clear = true;
            break;
        case 120:
            if(userInfo->getArtifactLevel(ePlace, E_ARTIFACT::ARTI_8_EVOL1) >= 99999)
                clear = true;
            break;
        case 121:
            if(PetNewManager::getInstance()->getPetLevel(E_PET::PRINCESS_4) >= 10)
                clear = true;
            break;
        case 122:
            if(userInfo->getArtifactLevel(ePlace, E_ARTIFACT::ARTI_4_EVOL1) >= 99999)
                clear = true;
            break;
        case 123:
            if (WeaponManager::getInstance()->getEquip() >= E_WEAPON::WEAPON_66)
                clear = true;
            break;
        case 124:
            if(PrisonManager::getInstance()->getDungeonKill() >= 1500)
                clear = true;
            break;
        case 125:
            if (WeaponManager::getInstance()->getEquip() >= E_WEAPON::WEAPON_76_MINE)
                clear = true;
            break;
        case 126:
            if(userInfo->getArtifactLevel(ePlace, E_ARTIFACT::ARTI_13_EVOL2) >= 50000)
                clear = true;
            break;
        case 127:
            if(userInfo->getArtifactLevel(ePlace, E_ARTIFACT::ARTI_14_EVOL2) >= 50000)
                clear = true;
            break;
        case 128:
            if (WeaponManager::getInstance()->getEquipPermanent() >= E_WEAPON::WEAPON_60)
                clear = true;
            break;
        case 129:
            if (WeaponManager::getInstance()->getEquip() >= E_WEAPON::WEAPON_78_L)
                clear = true;
            break;
        case 130:
            if(userInfo->getArtifactLevel(ePlace, E_ARTIFACT::ARTI_18_EVOL3) >= 50000)
                clear = true;
            break;
        case 131:
            if(PrisonManager::getInstance()->getDungeonKill() >= 3000)
                clear = true;
            break;
        case 132:
            if(userInfo->getArtifactLevel(ePlace, E_ARTIFACT::ARTI_19_EVOL3) >= 65000)
                clear = true;
            break;
        case 133:
            if(userInfo->getArtifactLevel(ePlace, E_ARTIFACT::ARTI_21_EVOL3) >= 80000)
                clear = true;
            break;
        case 134:
            if (WeaponManager::getInstance()->getEquip() >= E_WEAPON::WEAPON_82_L)
                clear = true;
            break;
        case 135:
            if(userInfo->getArtifactLevel(ePlace, E_ARTIFACT::ARTI_92_DISCOUNT_4) >= 1)
                clear = true;
            break;
        case 136:
            if(userInfo->getArtifactLevel(ePlace, E_ARTIFACT::ARTI_25_EVOL4) >= 5000)
                clear = true;
            break;
        case 137:
            if(userInfo->getArtifactLevel(ePlace, E_ARTIFACT::ARTI_27_CROSS_EVOL4) >= 3500)
                clear = true;
            break;
        case 138:
            if (WeaponManager::getInstance()->getEquip() >= E_WEAPON::WEAPON_90_L)
                clear = true;
            break;
        case 139:
            if(userInfo->getArtifactLevel(ePlace, E_ARTIFACT::ARTI_93_DISCOUNT_5) >= 1)
                clear = true;
            break;
        case 140:
            if ( userInfo->getArtifactLevel(ePlace, E_ARTIFACT::ARTI_28_EVOL5) >= 1000)
                clear = true;
            break;
        case 141:
            if ( userInfo->getArtifactLevel(ePlace, E_ARTIFACT::ARTI_31_EVOL5) >= 2500)
               clear = true;
            break;
        case 142:
            if (WeaponManager::getInstance()->getEquip() >= E_WEAPON::WEAPON_93)
                clear = true;
            break;
        case 143:
            if ( userInfo->getArtifactLevel(ePlace, E_ARTIFACT::ARTI_29_EVOL5) >= 18000 )
                clear = true;
            break;
        case 144:
            if (WeaponManager::getInstance()->getEquip() >= E_WEAPON::WEAPON_100_L)
                clear = true;
            break;
        case 145:
            if ( userInfo->getArtifactLevel(ePlace, E_ARTIFACT::ARTI_28_EVOL5) >= 30000 )
                clear = true;
            break;
        case 146:
            if ( userInfo->getArtifactLevel(ePlace, E_ARTIFACT::ARTI_33_CROSS_EVOL5) >= 100 )
                clear = true;
            break;
        case 147:
            if ( PrisonManager::getInstance()->getDungeonKill() >= 5000 )
                clear = true;
            break;
        case 148:
            if (WeaponManager::getInstance()->getEquip() >= E_WEAPON::WEAPON_104_L)
                clear = true;
            break;
        case 149:
            if ( userInfo->getArtifactLevel(ePlace, E_ARTIFACT::ARTI_35_EVOL6) >= 7000 )
                clear = true;
            break;
        case 150:
            if ( userInfo->getArtifactLevel(ePlace, E_ARTIFACT::ARTI_37_EVOL6) >= 3500 )
                clear = true;
            break;
        case 151:
            if (WeaponManager::getInstance()->getEquipPermanent() >= E_WEAPON::WEAPON_90_L)
                clear = true;
            break;
        case 152:
            if ( PrisonManager::getInstance()->getDungeonKill() >= 7000 )
                clear = true;
            break;
        case 153:
            if (WeaponManager::getInstance()->getEquip() >= E_WEAPON::WEAPON_114)
                clear = true;
            break;
        case 154:
            if (WeaponManager::getInstance()->getEquip() >= E_WEAPON::WEAPON_116_L)
                clear = true;
            break;
        case 155:
            if (WeaponManager::getInstance()->getEquip() >= E_WEAPON::WEAPON_119_L)
                clear = true;
            break;
        case 156:
             if (WeaponManager::getInstance()->getEquip() >= E_WEAPON::WEAPON_122_L)
                 clear = true;
             break;
        case 157:
             if (WeaponManager::getInstance()->getEquip() >= E_WEAPON::WEAPON_124_L)
                 clear = true;
             break;
        case 158:
            if (WeaponManager::getInstance()->getEquipPermanent() >= E_WEAPON::WEAPON_100_L)
                clear = true;
            break;
        case 159:
            if ( PrisonManager::getInstance()->getDungeonKill() >= 8000 )
                clear = true;
            break;
        case 160:
            if (WeaponManager::getInstance()->getEquip() >= E_WEAPON::WEAPON_126_L)
                clear = true;
            break;
        case 161:
            if (WeaponManager::getInstance()->getEquip() >= E_WEAPON::WEAPON_128_L)
                clear = true;
            break;
        case 162:
            if (WeaponManager::getInstance()->getEquip() >= E_WEAPON::WEAPON_130_L)
                clear = true;
            break;
        case 163:
        {
            auto obj = NewRaidManager::getInstance()->getStat(1);
            if(obj->getCurrentLv() >= 5)
                clear = true;
        }
            break;
        case 164:
            if ( userInfo->getPetBafometzHave() == true )
                clear = true;
            break;
        case 165:
            //vip 1레벨 달성
            break;
        case 166:
            if(userInfo->getHighFloor(E_PLACE::DG_DEVIL_DOM) >= 10000)
                clear = true;
            break;
        default:
            break;
    }
    
    return clear;
}

#pragma mark -net
void SpecialMissionManager::requestMissionReward(const std::function<void(bool, int)>& pCallback, int idx)
{
    _callbackReward = pCallback;
    
    auto objMission = getInfoSpecialMission(idx);
    if(objMission->isServerReward())
    {
        std::string url = "/moneyhero/specialmission/reward";
        auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
        req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
        req->setData("_index", objMission->getIdx());
        
        MafResponseNew cb = [&, idx](void* http,network::HttpResponse* response,const char *data){
            std::string dd = data;
            responseMissionReward(response, dd, idx);
        };
        req->send(cb);
    }
    else
    {
        ItemsManager::getInstance()->addItems(objMission->getListMissionReward());
        
        missionRewardAfter(objMission->getIdx());
        if(_callbackReward != nullptr)
            _callbackReward(true, 1);
        return;
    }
}

void SpecialMissionManager::responseMissionReward(cocos2d::network::HttpResponse* response,std::string &data, const int idx)
{
    bool bError = false;
    if ( response == nullptr || response->isSucceed() == false || response->getResponseCode() != 200 )
    {
        bError = true;
    }
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)data.c_str());
    if(jsonParser.HasParseError())
    {
        bError = true;
    }
    
    if ( bError == true )
    {
        if(_callbackReward != nullptr)
        {
            _callbackReward(false, 0);

            _callbackReward = nullptr;
        }
        return;
    }
    
    int result = jsonParser["_result"].GetInt();
    if ( result != 1 )
    {
        if(result == 0)
            missionRewardAfter(idx);
        if(_callbackReward != nullptr)
        {
            _callbackReward(false, result);

            _callbackReward = nullptr;
        }
        return;
    }
    
    std::string strReward = jsonParser["_reward"].GetString();
    std::string strItems = jsonParser["_items"].GetString();
    
    ItemsManager::getInstance()->addItems(strReward);
    ItemsManager::getInstance()->setItemsServer(strItems);
    
    missionRewardAfter(idx);
    
    if(_callbackReward != nullptr)
    {
        _callbackReward(true, result);

        _callbackReward = nullptr;
    }
}
void SpecialMissionManager::missionRewardAfter(const int missionIdx)
{
    setSpecialMissionClear(missionIdx);
    setSpecialMissionComplete(0);
    
    std::string strEventName = MafUtils::format(kAnalEventSpecialMission, missionIdx);
    MafAnalyticsManager::LogEvent(strEventName.c_str(),kRepeatFalse);
    
    // maket review
    int nReviewMarekt = UserDefault::getInstance()->getIntegerForKey(KEY_REVIEW_MARKET, 0);
    if ( nReviewMarekt == 0 && missionIdx == 24 )
    {
        auto popup = PopupReviewMarket::create();
        popup->show();
    }
    HelloWorldScene::getInstance()->reDrawSpecialQuest();
}

E_SHORTCUTS SpecialMissionManager::onShortCut(int idx)
{
    E_SHORTCUTS result = E_SHORTCUTS::SHORTCUT_NONE;
    auto objMission = SpecialMissionManager::getInstance()->getInfoSpecialMission(idx);
    if(objMission == nullptr)
        return result;
    
    //
    auto tutorial = TutorialManager::getInstance();
    tutorial->clickShortcutButton();
    
    //
    result = objMission->getShortcutType();
    switch (result)
    {
        case SHORTCUT_NONE:
        {
            return result;
        }
            break;

        case SHORTCUT_QUEST:
        {
            auto sceneGameLayer = UtilsScene::getInstance()->getRunningLayer();
            if ( dynamic_cast<HelloWorldScene *>(sceneGameLayer) != nullptr )
            {
                MafNode::MafMenuItemSprite* item = (MafNode::MafMenuItemSprite*)dynamic_cast<HelloWorldScene *>(sceneGameLayer)->_menu_btn->getChildByName("QUEST");
                item->setName("QUEST");
                dynamic_cast<HelloWorldScene *>(sceneGameLayer)->onButtonTouched(item);
            }
            break;
        }
        case SHORTCUT_EQUIP:
        {
            auto sceneGameLayer = UtilsScene::getInstance()->getRunningLayer();
            if ( dynamic_cast<HelloWorldScene *>(sceneGameLayer) != nullptr )
            {
                MafNode::MafMenuItemSprite* item = (MafNode::MafMenuItemSprite*)dynamic_cast<HelloWorldScene *>(sceneGameLayer)->_menu_btn->getChildByName("EQUIP");
                item->setName("EQUIP");
                dynamic_cast<HelloWorldScene *>(sceneGameLayer)->onButtonTouched(item);
            }
            break;
        }
        case SHORTCUT_PET:
        case SHORTCUT_PETEQUIP:
        {

            auto sceneGameLayer = UtilsScene::getInstance()->getRunningLayer();
            if ( dynamic_cast<HelloWorldScene *>(sceneGameLayer) != nullptr )
            {
                MafNode::MafMenuItemSprite* item = (MafNode::MafMenuItemSprite*)dynamic_cast<HelloWorldScene *>(sceneGameLayer)->_menu_btn->getChildByName("PET");
                item->setName("PET");
                dynamic_cast<HelloWorldScene *>(sceneGameLayer)->onButtonTouched(item);
            }
            break;
        }
        case SHORTCUT_ARTIFACT_NORMAL:
        {
            auto sceneGameLayer = UtilsScene::getInstance()->getRunningLayer();
            if ( dynamic_cast<HelloWorldScene *>(sceneGameLayer) != nullptr )
            {
                MafNode::MafMenuItemSprite* item = (MafNode::MafMenuItemSprite*)dynamic_cast<HelloWorldScene *>(sceneGameLayer)->_menu_btn->getChildByName("ARTIFACT");
                item->setName("ARTIFACT");
                dynamic_cast<HelloWorldScene *>(sceneGameLayer)->onButtonTouched(item);

            }
            if ( dynamic_cast<HelloWorldScene *>(sceneGameLayer) != nullptr )
            {
                //dynamic_cast<HelloWorldScene *>(sceneGameLayer)->createArtifactTable(1);
                auto item = dynamic_cast<HelloWorldScene *>(sceneGameLayer)->_cellArtifacTop;
                item->onClickTab(item->getChildByTag(1000)->getChildByTag(1));
                dynamic_cast<HelloWorldScene *>(sceneGameLayer)->_scroll[2]->setContentOffset(Vec2(0,0));

            }
            break;
        }
        case SHORTCUT_ARTIFACT_SPECIAL:
        {
            auto sceneGameLayer = UtilsScene::getInstance()->getRunningLayer();
            if ( dynamic_cast<HelloWorldScene *>(sceneGameLayer) != nullptr )
            {
                MafNode::MafMenuItemSprite* item = (MafNode::MafMenuItemSprite*)dynamic_cast<HelloWorldScene *>(sceneGameLayer)->_menu_btn->getChildByName("ARTIFACT");
                item->setName("ARTIFACT");
                dynamic_cast<HelloWorldScene *>(sceneGameLayer)->onButtonTouched(item);

            }
            if ( dynamic_cast<HelloWorldScene *>(sceneGameLayer) != nullptr )
            {
                //dynamic_cast<HelloWorldScene *>(sceneGameLayer)->createArtifactTable(1);
                auto item = dynamic_cast<HelloWorldScene *>(sceneGameLayer)->_cellArtifacTop;
                item->onClickTab(item->getChildByTag(1000)->getChildByTag(0));

            }
            break;
        }
        case SHORTCUT_RELIC:
        {
            auto sceneGameLayer = UtilsScene::getInstance()->getRunningLayer();
            if ( dynamic_cast<HelloWorldScene *>(sceneGameLayer) != nullptr )
            {
                MafNode::MafMenuItemSprite* item = (MafNode::MafMenuItemSprite*)dynamic_cast<HelloWorldScene *>(sceneGameLayer)->_menu_btn->getChildByName("ARTIFACT");
                item->setName("ARTIFACT");

                dynamic_cast<HelloWorldScene *>(sceneGameLayer)->onButtonTouched(item);

            }
            if ( dynamic_cast<HelloWorldScene *>(sceneGameLayer) != nullptr )
            {
                //dynamic_cast<HelloWorldScene *>(sceneGameLayer)->createArtifactTable(1);
                auto item = dynamic_cast<HelloWorldScene *>(sceneGameLayer)->_cellArtifacTop;
                item->onClickTab(item->getChildByTag(1000)->getChildByTag(1000));

            }
            break;
        }
        case SHORTCUT_SHOP:
        {
            auto popup = PopupSubFunction::create(PopupSubFunction::E_TYPE::SHOPITEM);
            popup->show();
            break;
        }
        case SHORTCUT_PRESTIGE:
        {
            auto sceneGameLayer = UtilsScene::getInstance()->getRunningLayer();
            if ( dynamic_cast<HelloWorldScene *>(sceneGameLayer) != nullptr )
            {
                dynamic_cast<HelloWorldScene *>(sceneGameLayer)->onClickoRevive();
            }
            break;
        }
        case SHORTCUT_MINE:
        {
            if ( WeaponManager::getInstance()->getEquipHigh() >= E_WEAPON::WEAPON_76_MINE )
            {
                auto popup = PopupSubFunction::create(PopupSubFunction::E_TYPE::MINE);
                popup->show();
            }
            else
            {
                auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_12"));
                popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
                popup->show();
            }
            break;
        }
        case SHORTCUT_BUILDING:
        {
            auto popup = PopupSubFunction::create(PopupSubFunction::E_TYPE::TOWN);
            popup->show();
            break;
        }
        case SHORTCUT_MYHOME:
        {
            auto popup = PopupSubFunction::create(PopupSubFunction::E_TYPE::MYHOME);
            popup->show();
            break;
        }
        case SHORTCUT_MENU:
        {
            auto popup = MenuPopup::create();
            popup->show();
            break;
        }
        case SHORTCUT_GIFT:
        {
            auto sceneGameLayer = UtilsScene::getInstance()->getRunningLayer();
            if ( dynamic_cast<HelloWorldScene *>(sceneGameLayer) != nullptr )
            {
                auto item = MafNode::MafMenuItem::create();
                item->setName("GIFT");

                dynamic_cast<HelloWorldScene *>(sceneGameLayer)->onButtonTouched(item);
            }
            break;
        }
        case SHORTCUT_ATTEND:
        {
            auto sceneGameLayer = UtilsScene::getInstance()->getRunningLayer();
            if ( dynamic_cast<HelloWorldScene *>(sceneGameLayer) != nullptr )
            {
                auto item = MafNode::MafMenuItem::create();
                item->setName("ATTEND");

                dynamic_cast<HelloWorldScene *>(sceneGameLayer)->onButtonTouched(item);
            }
            break;
        }
        case SHORTCUT_COSTUME:
        {
            if ( AccountManager::getInstance()->getUserIdx() == 0 )
            {
                auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_1"));
                popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
                popup->show();
                return result;
            }

            auto popup = PopupCostumeDefault::create();
            popup->show();
            break;
        }
        case SHORTCUT_PRISON:
        {
            auto sceneGameLayer = UtilsScene::getInstance()->getRunningLayer();
            if ( dynamic_cast<HelloWorldScene *>(sceneGameLayer) != nullptr )
            {
                auto item = MafNode::MafMenuItem::create();
                item->setName("DUNGEON_INPUT");

                dynamic_cast<HelloWorldScene *>(sceneGameLayer)->onButtonTouched(item);
                
                auto popup = PopupPrison::create();
                popup->show();
            }
            break;
        }
        case SHORTCUT_RAID:
        {
            bool bEnter = false;

            int nRevival = UserInfoManager::getInstance()->getRevival(E_PLACE::DG_NORMAL);
            if ( nRevival > 1 )
            {
                bEnter = true;
            }

            if ( UserInfoManager::getInstance()->getHighFloor(E_PLACE::DG_NORMAL) >= 1000 || UserInfoManager::getInstance()->getFloor(E_PLACE::DG_NORMAL) >= 1000 )
            {
                bEnter = true;
            }

            if ( bEnter == false )
            {
                auto popup = PopupDefault::create(GAME_TEXT("t_ui_new_raid_title"), GAME_TEXT("t_ui_new_raid_condition_1"));
                popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
                popup->show();
                return result;
            }

            auto popup = PopupRaidDefault::create();
            popup->show();

            break;
        }
        case SHORTCUT_FRIENDS:
        {
            if ( AccountManager::getInstance()->getUserIdx() == 0 )
            {
                auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_1"));
                popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
                popup->show();
                return result;
            }

            auto popup = PopupFriend::create();
            popup->show();
            break;
        }
        case SHORTCUT_DEVILDOM:
        {
            if ( UserInfoManager::getInstance()->getHighFloor(E_PLACE::DG_NORMAL) < DEVIL_JOIN_FLOOR )
            {
                auto popup = PopupDefault::create(GAME_TEXT("t_ui_dungeon_devil"), GAME_TEXT("t_ui_error_28"));
                popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
                popup->show();
                return SHORTCUT_NONE;
            }
            
            //
            SaveManager::saveAllData();
            
            bool bPlay = PrisonManager::getInstance()->isDungeonPlay();
            int nEnterAutoCount = PrisonManager::getInstance()->getEnterAutoCount();
            if ( bPlay == true || nEnterAutoCount > 0 )
            {
                auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_prison_close_check"));
                popup->addButton(GAME_TEXT("t_ui_cancel"), true, nullptr);
                popup->addButton(GAME_TEXT("t_ui_confirm"), false, [=](Ref* sender){
                    UtilsScene::getInstance()->sceneReplace(UtilsScene::eType::scene_dungeon_devil);
                });
                popup->show();
            }
            else
                UtilsScene::getInstance()->sceneReplace(UtilsScene::eType::scene_dungeon_devil);
        }
            break;
        default:
            break;
    }
    
    return result;
}
