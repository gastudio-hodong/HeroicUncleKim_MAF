//
//  PetNewManager.cpp
//  FantasyClicker-mobile
//
//  Created by MAF on 2022/05/12.
//

#include "PetNewManager.h"

#include "ManagerGame/UserInfoManager.h"
#include "ManagerGame/SaveManager.h"
#include "ManagerGame/AccountManager.h"
#include "ManagerGame/DataManager.h"
#include "ManagerGame/ItemsManager.h"
#include "ManagerGame/ItemsMoneyManager.h"
#include "ManagerGame/CostumeManager.h"
#include "ManagerGame/AchievementManager.h"
#include "ManagerGame/AdventureRelicManager.h"
#include "ManagerGame/StoryManager.h"
#include "ManagerGame/TownManager.h"
#include "ManagerGame/WeaponManager.h"
#include "ManagerGame/MineralManager.h"

USING_NS_CC;

PetNewManager::PetNewManager(void) :
_ePlace(E_PLACE::DG_NORMAL)
{
    
}

PetNewManager::~PetNewManager(void)
{
    
}

bool PetNewManager::init()
{
    for ( int i = 0; i < sizeof(_uPetLevel) / sizeof(TInteger); i++ )
    {
        _uPetLevel[i] = 0;
    }
    
    for ( int i = 0; i < sizeof(_listPetEvolution) / sizeof(TInteger); i++ )
    {
        _listPetEvolution[i] = 0;
    }
    
    for ( int i = 0; i < sizeof(_listPetEvolutionReward) / sizeof(TInteger); i++ )
    {
        _listPetEvolutionReward[i] = 0;
    }
    
    for ( int i = 0; i < sizeof(_uPetReinforce) / sizeof(TInteger); i++ )
    {
        _uPetReinforce[i] = 0;
    }
    
    for ( int i = 0; i < sizeof(_uPetSkin) / sizeof(TInteger); i++ )
    {
        _uPetSkin[i] = 0;
    }
    
    
    
    return true;
}

#pragma mark - update
void PetNewManager::update(float dt)
{
     
}

#pragma mark - data
void PetNewManager::saveData()
{
    std::stringstream str;
    
    // level
    str.str("");
    for ( int i = 1; i <= C_COUNT_PET; i++ )
    {
        str << getPetLevel((E_PET)i) << ",";
    }
    UserDefault::getInstance()->setStringForKey(KEY_PET_LEVEL, MafAes256::Encrypt(str.str()));
    
    // Evolution
    str.str("");
    for ( int i = 1; i <= C_COUNT_PET; i++ )
    {
        str << getPetEvolution((E_PET)i) << ",";
    }
    UserDefault::getInstance()->setStringForKey(KEY_PET_EVOLUTION, MafAes256::Encrypt(str.str()));
    
    // Evolution Reward
    str.str("");
    for ( int i = 1; i <= C_COUNT_PET; i++ )
    {
        str << getPetEvolutionReward((E_PET)i) << ",";
    }
    UserDefault::getInstance()->setStringForKey(KEY_PET_EVOLUTION_REWARD, MafAes256::Encrypt(str.str()));
    
    // Reinforce
    str.str("");
    for ( int i = 1; i <= C_COUNT_PET; i++ )
    {
        str << getPetReinforce((E_PET)i) << ",";
    }
    UserDefault::getInstance()->setStringForKey(KEY_PET_REINFORCE_LEVEL, MafAes256::Encrypt(str.str()));
    
    // skin equip
    str.str("");
    for ( int i = 1; i <= C_COUNT_PET; i++ )
    {
        str << getPetSkin((E_PET)i) << ",";
    }
    UserDefault::getInstance()->setStringForKey(KEY_PET_SKIN, MafAes256::Encrypt(str.str()));
    
    // skin buy
    str.str("");
    for( const auto& obj : _listPetSkinBuy )
    {
        if ( str.str().length() != 0 )
        {
            str << ",";
        }
        str << obj;
    }
    UserDefault::getInstance()->setStringForKey(KEY_PET_SKIN_BUY, MafAes256::Encrypt(str.str()));
}

void PetNewManager::loadData()
{
    std::string str = "";
    
    //
    str.clear();
    str = UserDefault::getInstance()->getStringForKey(KEY_PET_LEVEL);
    if ( !str.empty() )
    {
        str = MafAes256::Decrypt(str);
        
        for(int i=1; i <= C_COUNT_PET; i++)
        {
            auto offset = str.find(",");
            if ( offset != std::string::npos )
            {
                auto result = str.substr(0,offset);
                setPetLevel((E_PET)i, std::atoi(result.c_str()));
                
                str = str.substr(offset+1);
            }
            else
            {
                setPetLevel((E_PET)i, std::atoi(str.c_str()));
                break;
            }
        }
    }
    
    // Evolution
    str.clear();
    str = UserDefault::getInstance()->getStringForKey(KEY_PET_EVOLUTION);
    if ( !str.empty() )
    {
        str = MafAes256::Decrypt(str);
        
        for ( int i=1; i <= C_COUNT_PET; i++ )
        {
            auto offset = str.find(",");
            if ( offset != std::string::npos )
            {
                auto result = str.substr(0,offset);
                setPetEvolution((E_PET)i, std::atoi(result.c_str()));
                
                str = str.substr(offset+1);
            }
            else
            {
                setPetEvolution((E_PET)i, std::atoi(str.c_str()));
                break;
            }
        }
    }
    
    // Evolution Reward
    str.clear();
    str = UserDefault::getInstance()->getStringForKey(KEY_PET_EVOLUTION_REWARD);
    if ( !str.empty() )
    {
        str = MafAes256::Decrypt(str);
        
        for ( int i=1; i <= C_COUNT_PET; i++ )
        {
            auto offset = str.find(",");
            if ( offset != std::string::npos )
            {
                auto result = str.substr(0, offset);
                setPetEvolutionReward((E_PET)i, std::atoi(result.c_str()));
                
                str = str.substr(offset+1);
            }
            else
            {
                setPetEvolutionReward((E_PET)i, std::atoi(str.c_str()));
                break;
            }
        }
    }
    
    // Reinforce
    str.clear();
    str = UserDefault::getInstance()->getStringForKey(KEY_PET_REINFORCE_LEVEL);
    if ( !str.empty() )
    {
        str = MafAes256::Decrypt(str);
        
        for ( int i=1; i <= C_COUNT_PET; i++ )
        {
            auto offset = str.find(",");
            if ( offset != std::string::npos )
            {
                auto result = str.substr(0,offset);
                setPetReinforce((E_PET)i, std::atoi(result.c_str()));
                
                str = str.substr(offset+1);
            }
            else
            {
                setPetReinforce((E_PET)i, std::atoi(str.c_str()));
                break;
            }
        }
    }
    
    // skin equip
    str.clear();
    str = UserDefault::getInstance()->getStringForKey(KEY_PET_SKIN);
    if ( !str.empty() )
    {
        str = MafAes256::Decrypt(str);
        
        for ( int i = 1; i <= C_COUNT_PET; i++ )
        {
            auto offset = str.find(",");
            if ( offset != std::string::npos )
            {
                auto result = str.substr(0,offset);
                setPetSkin((E_PET)i, std::atoi(result.c_str()));
                
                str = str.substr(offset+1);
            }
            else
            {
                setPetSkin((E_PET)i, std::atoi(str.c_str()));
                break;
            }
        }
    }
    
    // skin buy
    str.clear();
    str = UserDefault::getInstance()->getStringForKey(KEY_PET_SKIN_BUY);
    if ( !str.empty() )
    {
        str = MafAes256::Decrypt(str);
        
        std::istringstream stream(str);
        std::string field = "";
        while ( std::getline(stream, field, ',') )
        {
            auto offset = field.find("_");
            if(offset != std::string::npos)
            {
                std::string str1 = field.substr(0,offset);
                std::string str2 = field.substr(offset+1);
                
                if ( MafUtils::isNumeric(str1) == false || MafUtils::isNumeric(str2) == false )
                {
                    continue;
                }
                setPetSkinBuy((E_PET)atoi(str1.c_str()), atoi(str2.c_str()));
            }
        }
    }
}

#pragma mark - load
void PetNewManager::setLoad()
{
    setLoadPetSkin();
    setLoadPetEquip();
    setLoadPetEquipLevel();
}

void PetNewManager::setLoadPetSkin()
{
    _listInfoPetSkin.clear();
    
    //
    std::string txt = TableManager::getInstance()->getTableData(E_TABLE::PET_SKIN);
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)(const char*)txt.c_str()).HasParseError();
    if( jsonParser.HasParseError() == true )
    {
        return;
    }
    
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];
        if ( MafNative::IsLessThanCurrVersion(jsonValue["version"].GetString()) == false )
        {
            continue;
        }
        
        E_PET ePetType = E_PET::CHICK_1;
        if ( jsonValue.HasMember("pet") == true )
        {
            ePetType = (E_PET)jsonValue["pet"].GetInt();
        }
        
        int nIdx = jsonValue["idx"].GetInt();
        std::string strNameKey = jsonValue["name_key"].GetString();
        
        int nItemIdx = jsonValue["item_idx"].GetInt();
        int nItemTypeParams = jsonValue["item_type_params"].GetInt();
        
        std::string strItemCount = "0";
        if ( jsonValue["item_count"].IsString() )
        {
            strItemCount = jsonValue["item_count"].GetString();
        }
        else
        {
            int64_t nCount = jsonValue["item_count"].GetInt64();
            strItemCount = MafUtils::toString(nCount);
        }
        
        int64_t nEventSaleTimeStart = 0;
        int64_t nEventSaleTimeEnd = 0;
        if ( jsonValue.HasMember("event_start_date") == true )
        {
            nEventSaleTimeStart = jsonValue["event_start_date"].GetInt64();
            nEventSaleTimeEnd = jsonValue["event_end_date"].GetInt64();
        }
        
        auto objPetSkin = InfoPetSkin::create();
        objPetSkin->setPetType(ePetType);
        objPetSkin->setIdx(nIdx);
        objPetSkin->setNameKey(strNameKey);
        objPetSkin->setItemIdx(nItemIdx);
        objPetSkin->setItemTypeParams(nItemTypeParams);
        objPetSkin->setItemCount(strItemCount);
        
        // event sale
        objPetSkin->setEventSaleTimeStart(nEventSaleTimeStart);
        objPetSkin->setEventSaleTimeEnd(nEventSaleTimeEnd);
        
        _listInfoPetSkin.pushBack(objPetSkin);
    }
}

void PetNewManager::setLoadPetEquip()
{
    _listInfoPetEquip.clear();
    
    //
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::PET_EQUIP_DATA);
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)strText.c_str());
    if( jsonParser.HasParseError() == true )
    {
        return;
    }
    
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];
        if ( MafNative::IsLessThanCurrVersion(jsonValue["version"].GetString()) == false )
        {
            continue;
        }
        
        auto objPetEquip = InfoPetEquip::create();
        objPetEquip->setIdx(jsonValue[("idx")].GetInt());
        objPetEquip->setType((E_PET)jsonValue[("pet")].GetInt());
        objPetEquip->setSlot(jsonValue[("slot")].GetInt());
        objPetEquip->setProperty1(jsonValue[("property_1")].GetFloat());
        objPetEquip->setProperty2(jsonValue[("property_2")].GetFloat());
        objPetEquip->setIncrease(jsonValue[("increase")].GetFloat());
        objPetEquip->setEffectType((E_PET_EQUIP_TYPE)jsonValue[("effect_type")].GetInt());
        
        std::string strName = MafUtils::format("t_pet_equip_name_%d", objPetEquip->getIdx());
        objPetEquip->setName(strName);
        
        std::string strStory = MafUtils::format("t_pet_equip_story_%d", objPetEquip->getIdx());
        objPetEquip->setStory(strStory);
        
        std::string strDesc = MafUtils::format("t_pet_equip_desc_%d", objPetEquip->getIdx());
        objPetEquip->setDesc(strDesc);
        
        _listInfoPetEquip.pushBack(objPetEquip);
    }
}

void PetNewManager::setLoadPetEquipLevel()
{
    _listInfoPetEquipLevel.clear();
    
    //
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::PET_EQUIP_AMOUNT);
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)strText.c_str());
    if( jsonParser.HasParseError() == true )
    {
        return;
    }
    
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];
        
        auto objPetEquipAmount = InfoPetEquipAmount::create();
        objPetEquipAmount->setIdx(jsonValue[("level")].GetInt());
        objPetEquipAmount->setAmount(jsonValue[("count")].GetInt());
        
        _listInfoPetEquipLevel.pushBack(objPetEquipAmount);
    }
}

#pragma mark - info
cocos2d::Vector<InfoPetSkin*> PetNewManager::getListPetSkinAll()
{
    return _listInfoPetSkin;
}

cocos2d::Vector<InfoPetSkin*> PetNewManager::getListPetSkin(E_PET ePet)
{
    Vector<InfoPetSkin*> listResult;
    
    for ( auto& objPetSkin : _listInfoPetSkin )
    {
        if ( objPetSkin->getPetType() == ePet )
        {
            listResult.pushBack(objPetSkin);
        }
    }
    
    return listResult;
}

InfoPetSkin* PetNewManager::getInfoPetSkin(E_PET ePet, int idx)
{
    for ( auto& objPetSkin : _listInfoPetSkin )
    {
        if ( objPetSkin->getPetType() == ePet && objPetSkin->getIdx() == idx )
            return objPetSkin;
    }
    
    return nullptr;
}

cocos2d::Vector<InfoPetEquip*> PetNewManager::getListPetEquipAll()
{
    return _listInfoPetEquip;
}
cocos2d::Vector<InfoPetEquip*> PetNewManager::getListPetEquip(E_PET ePet)
{
    Vector<InfoPetEquip*> listResult;
    
    for ( auto& objPetEquip : _listInfoPetEquip )
    {
        if ( objPetEquip->getType() == ePet )
        {
            listResult.pushBack(objPetEquip);
        }
    }
    
    return listResult;
}

InfoPetEquip* PetNewManager::getInfoPetEquip(int idx)
{
    for ( auto& objPetEquip : _listInfoPetEquip )
    {
        if ( objPetEquip->getIdx() == idx )
            return objPetEquip;
    }
    
    return nullptr;
}

cocos2d::Vector<InfoPetEquipAmount*> PetNewManager::getListPetEquipLevelAll()
{
    return _listInfoPetEquipLevel;
}

InfoPetEquipAmount* PetNewManager::getInfoPetEquipLevel(int level)
{
    for ( auto& objPetEquipLevel : _listInfoPetEquipLevel )
    {
        if ( objPetEquipLevel->getIdx() == level )
            return objPetEquipLevel;
    }
    
    return nullptr;
}

#pragma mark - get, set : pet
std::string PetNewManager::getPetName(E_PET ePet, bool bEvolution)
{
    std::string strName = "";
    
    switch (ePet) {
        case E_PET::CHICK_1:
        {
            if (bEvolution || getPetEvolution(E_PET::CHICK_1) > 0)
                strName.append(GAME_TEXT("t_pet_d1_title_chick_e"));
            else
                strName.append(GAME_TEXT("t_pet_d1_title_chick"));
            break;
        }
        case E_PET::HAMSTER_2:
        {
            if (bEvolution || getPetEvolution(E_PET::HAMSTER_2) > 0)
                strName.append(GAME_TEXT("t_pet_d1_title_hamster_e"));
            else
                strName.append(GAME_TEXT("t_pet_d1_title_hamster"));
            break;
        }
        case E_PET::DRAGON_3:
        {
            if (bEvolution || getPetEvolution(E_PET::DRAGON_3) > 0)
                strName.append(GAME_TEXT("t_pet_d1_title_dragon_e"));
            else
                strName.append(GAME_TEXT("t_pet_d1_title_dragon"));
            break;
        }
        case E_PET::PRINCESS_4:
        {
            if (bEvolution || getPetEvolution(E_PET::PRINCESS_4) > 0)
                strName.append(GAME_TEXT("t_pet_d1_title_princess_e"));
            else
                strName.append(GAME_TEXT("t_pet_d1_title_princess"));
            break;
        }
        case E_PET::PURIRING_5:
        {
            if (bEvolution || getPetEvolution(E_PET::PURIRING_5) > 0)
                strName.append(GAME_TEXT("t_pet_d1_title_puriring_e"));
            else
                strName.append(GAME_TEXT("t_pet_d1_title_puriring"));
            break;
        }
        case E_PET::HONG_6:
        {
            strName.append(GAME_TEXT("t_pet_d1_title_hong"));
            break;
        }
        case E_PET::MOLE_7:
        {
            strName.append(GAME_TEXT("t_pet_d1_title_mole"));
            break;
        }
        case E_PET::HANDSOME_8:
        {
            strName.append(GAME_TEXT("t_pet_d1_title_handsome"));
            break;
        }
        case E_PET::D_1_PENGUIN:
        {
            strName.append(GAME_TEXT("t_pet_d2_title_penguin"));
            break;
        }
        case E_PET::D_2_HEDGEHOG:
        {
            strName.append(GAME_TEXT("t_pet_d2_title_hedgehog"));
            break;
        }
        case E_PET::D_3_GATEKEEPER:
        {
            strName.append(GAME_TEXT("t_pet_d2_title_gatekeeper"));
            break;
        }
        case E_PET::D_4_FLASH:
        {
            strName.append(GAME_TEXT("t_pet_d2_title_flash"));
            break;
        }
            
        default:
            break;
    }
    
    return strName;
}

std::string PetNewManager::getPetExplain(E_PET ePet)
{
    std::string strExpain = "";
    switch (ePet) {
        case E_PET::CHICK_1:            strExpain.append(GAME_TEXT("t_pet_d1_contents_chick"));          break;
        case E_PET::HAMSTER_2:          strExpain.append(GAME_TEXTFORMAT("t_pet_d1_contents_hamster", Hamster::getAbilityMagnification()));        break;
        case E_PET::DRAGON_3:           strExpain.append(GAME_TEXT("t_pet_d1_contents_dragon"));         break;
        case E_PET::PRINCESS_4:         strExpain.append(GAME_TEXT("t_pet_d1_contents_princess"));       break;
        case E_PET::PURIRING_5:         strExpain.append(GAME_TEXT("t_pet_d1_contents_puriring"));       break;
        case E_PET::HONG_6:             strExpain.append(GAME_TEXT("t_pet_d1_contents_hong"));           break;
        case E_PET::MOLE_7:             strExpain.append(GAME_TEXT("t_pet_d1_contents_mole"));           break;
        case E_PET::HANDSOME_8:         strExpain.append(GAME_TEXT("t_pet_d1_contents_handsome"));       break;
        case E_PET::D_1_PENGUIN:        strExpain.append(GAME_TEXT("t_pet_d2_contents_penguin"));        break;
        case E_PET::D_2_HEDGEHOG:       strExpain.append(GAME_TEXT("t_pet_d2_contents_hedgehog"));       break;
        case E_PET::D_3_GATEKEEPER:     strExpain.append(GAME_TEXT("t_pet_d2_contents_gatekeeper"));     break;
        case E_PET::D_4_FLASH:          strExpain.append(GAME_TEXT("t_pet_d2_contents_flash"));          break;
            
        default:
            break;
    }
    
    return strExpain;
}

std::string PetNewManager::getPetUpgradeExplain(E_PET ePet)
{
    std::string strExpain = "";
    switch (ePet) {
        case E_PET::CHICK_1:            strExpain.append(GAME_TEXT("t_pet_d1_option_chick"));          break;
        case E_PET::HAMSTER_2:          strExpain.append(GAME_TEXT("t_pet_d1_option_hamster"));        break;
        case E_PET::DRAGON_3:           strExpain.append(GAME_TEXT("t_pet_d1_option_dragon"));         break;
        case E_PET::PRINCESS_4:         strExpain.append(GAME_TEXT("t_pet_d1_option_princess"));       break;
        case E_PET::PURIRING_5:         strExpain.append(GAME_TEXT("t_pet_d1_option_puriring"));       break;
        case E_PET::HONG_6:             strExpain.append(GAME_TEXT("t_pet_d1_option_hong"));           break;
        case E_PET::MOLE_7:             strExpain.append(GAME_TEXT("t_pet_d1_option_mole"));           break;
        case E_PET::HANDSOME_8:         strExpain.append(GAME_TEXT("t_pet_d1_option_handsome"));       break;
        case E_PET::D_1_PENGUIN:        strExpain.append(GAME_TEXT("t_pet_d2_option_penguin"));        break;
        case E_PET::D_2_HEDGEHOG:       strExpain.append(GAME_TEXT("t_pet_d2_option_hedgehog"));       break;
        case E_PET::D_3_GATEKEEPER:     strExpain.append(GAME_TEXT("t_pet_d2_option_gatekeeper"));     break;
        case E_PET::D_4_FLASH:          strExpain.append(GAME_TEXT("t_pet_d2_option_flash"));          break;
            
        default:
            break;
    }
    
    return strExpain;
}

std::string PetNewManager::getPetImagePath(E_PET ePet)
{
    int nIdx = 1;
    if ( getPetEvolution(ePet) > 0 )
    {
        nIdx = 2;
    }
        
    std::string strPath = MafUtils::format("Assets/icon_pet/peticon_%02d_%d.png", ePet, nIdx);
    return strPath;
}

double PetNewManager::getPetCoolTime(E_PET ePet)
{
    double nTime = 0;
    int nLevel =0;
    
    if ( ePet == E_PET::CHICK_1)
    {
        nLevel = UserInfoManager::getInstance()->getArtifactLevel(_ePlace, E_ARTIFACT::ARTI_7);
    }
    else if( ePet == E_PET::HAMSTER_2)
    {
        nLevel = getPetLevel(ePet);
    }
    else if( ePet == E_PET::PRINCESS_4)
    {
        nLevel = getPetLevel(ePet);
    }
    else if( ePet == E_PET::HONG_6)
    {
        nLevel = getPetLevel(ePet);
    }
    
    if      ( ePet == E_PET::CHICK_1 )          nTime = 2.5;
    else if ( ePet == E_PET::HAMSTER_2 )        nTime = 20;
    else if ( ePet == E_PET::PRINCESS_4 )       nTime = 2;
    else if ( ePet == E_PET::HONG_6 )           nTime = 2;
    else if ( ePet == E_PET::D_2_HEDGEHOG )     nTime = 3;
    else if ( ePet == E_PET::D_3_GATEKEEPER )   nTime = 10;
    
    if ( nLevel <= 0 )
    {
        return nTime;
    }
     
    if ( ePet == E_PET::CHICK_1)
    {
        nTime -= 0.2 * nLevel;
    }
    else if ( ePet == E_PET::HAMSTER_2 )
    {
        for ( int i = 0; i < nLevel-1; i++ )
        {
            nTime *= 0.9;
        }
    }
    
    //
    if ( ePet == E_PET::CHICK_1)
    {
        if ( CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_9) )
        {
            nLevel = UserInfoManager::getInstance()->getRelicLevel(_ePlace, E_RELIC::RELIC_1);
            nTime -= 0.0125 * nLevel;
            if (nTime < 0.25)
            {
                nTime = 0.25f;
            }
        }
        if ( CostumeManager::getInstance()->isEquip(E_COSTUME::FOOT,E_COSTUME_IDX::IDX_38) )
        {
            nTime *= 0.5f;
        }
        nTime *= getPetEquipEffect(101,  1);
    }
    else if( ePet == E_PET::HAMSTER_2)
    {
        nTime *= getPetEquipEffect(201, 1);
    }
    else if( ePet == E_PET::PRINCESS_4)
    {
        nTime *= getPetEquipEffect(402, 1);
    }
    else if ( ePet == E_PET::HONG_6)
    {
        if ( CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_34) )
        {
            nTime *= 0.5;
        }
        nTime *= getPetEquipEffect(600, 1);
    }
     
    return nTime;
}

double PetNewManager::getPetEffect(E_PET ePet, int level)
{
    //
    std::lock_guard<std::mutex> lock(_mutexEffect);
    
    double nEffect = 0;
    
    switch (ePet) {
        case E_PET::CHICK_1:
            nEffect = 0;
            nEffect += level;
            break;
        case E_PET::HAMSTER_2:
            nEffect = 20;
            for ( int i = 0; i < level-1; i++ )
            {
                nEffect *= 0.9;
            }
            break;
        case E_PET::DRAGON_3:
            nEffect = 30;
            nEffect += 5 * (level-1);
            break;
        case E_PET::PRINCESS_4:
            nEffect = 2;
            nEffect += 0.5 * (level-1);
            break;
        case E_PET::PURIRING_5:
            nEffect = 0.00005;
            nEffect += 0.00003 * (level-1);
            break;
        case E_PET::HONG_6:
            nEffect = 1;
            nEffect += 1 * (level-1);
            break;
        case E_PET::MOLE_7:
            nEffect = 1;
            nEffect += 1 * (level-1);
            break;
        case E_PET::HANDSOME_8:
            nEffect = 4;
            nEffect += 4 * (level-1);
            break;
            
        default:
            break;
    }
    
    if ( ePet == E_PET::PURIRING_5 )
    {
        nEffect *=  getPetEquipEffect(500, 1);
    }
    else if( ePet == E_PET::HONG_6 )
    {
        nEffect *= getPetEquipEffect(603, 1);
    }
    
    return nEffect;
}

std::string PetNewManager::getPetEffectPuriringKey(int nLevel, int nFloor, int nFloorCount)
{
    //
    std::lock_guard<std::mutex> lock(_mutexEffectPuriringKey);
    
    std::string keyAmount = DataManager::getInstance()->GetPrestigeKey(_ePlace, nFloor);
    
    int64_t nEffect = getPetEffect(E_PET::PURIRING_5, nLevel) * 10000000;
    keyAmount = MafUtils::bigMulNum(keyAmount, MafUtils::toString(nEffect));
    if ( keyAmount.length() > 7 )
    {
        keyAmount.pop_back();
        keyAmount.pop_back();
        keyAmount.pop_back();
        keyAmount.pop_back();
        keyAmount.pop_back();
        keyAmount.pop_back();
        keyAmount.pop_back();
    }
    else
    {
        keyAmount = "1";
    }
    
    //
    double nBuff = 1;
    if ( CostumeManager::getInstance()->isEquip(E_COSTUME::FOOT,E_COSTUME_IDX::IDX_22) )
    {
        if ( UserInfoManager::getInstance()->getBuffTime(E_BUFF_TYPE::BUFF_KEY_BONUS) > 0 )
        {
            nBuff += nBuff * 0.5;
        }
    }
    
    if ( CostumeManager::getInstance()->isEquip(E_COSTUME::CAPE,E_COSTUME_IDX::IDX_30) )
    {
        double per = TownManager::getInstance()->getTownLevel(E_TOWN::BUILDING_3) * 0.002;
        if ( per > 0 )
        {
            nBuff += nBuff * per;
        }
    }
    
    if ( CostumeManager::getInstance()->isEquip(E_COSTUME::CAPE,E_COSTUME_IDX::IDX_38) )
    {
        nBuff += nBuff * 0.2;
    }
    
    
    int nLevelEvolution = getPetEvolution(E_PET::PURIRING_5);
    double nEffectEvolution1 = getPetEvolutionEffect1(E_PET::PURIRING_5, nLevelEvolution);
    if ( nEffectEvolution1 > 0)
    {
        nBuff *= nEffectEvolution1;
    }
    
    double nEffectEvolution2 = getPetEvolutionEffect2(E_PET::PURIRING_5, nLevelEvolution);
    if ( nEffectEvolution2 > 0 )
    {
        if ( nFloor % 100 == 1 && nFloorCount == 1 )
        {
            nBuff *= nEffectEvolution2;
        }
    }
    
    if ( nFloorCount == 1 )
    {
        nBuff *= getPetEquipEffect(501, 1);
    }
    
    nBuff *= 100;
    keyAmount = MafUtils::bigMulNum(keyAmount, MafUtils::doubleToString(nBuff));
    if ( keyAmount.length() > 2 )
    {
        keyAmount.pop_back();
        keyAmount.pop_back();
    }
    else
    {
        keyAmount = "0";
    }
    
    
    return keyAmount;
}


#pragma mark - get, set : pet level
int PetNewManager::getPetLevel(E_PET ePet)
{
    int level = _uPetLevel[ePet].valueInt();
    if ( level > getPetLevelMax(ePet) )
    {
        level = getPetLevelMax(ePet);
    }
    return level;
}

int PetNewManager::getPetLevelMax(E_PET ePet)
{
    int level = 0;
    
    switch (ePet) {
        case E_PET::CHICK_1:            level = 40;    break;
        case E_PET::HAMSTER_2:          level = 16;    break;
        case E_PET::DRAGON_3:           level = 14;    break;
        case E_PET::PRINCESS_4:         level = 15;    break;
        case E_PET::PURIRING_5:         level = 16;    break;
        case E_PET::HONG_6:             level = 20;    break;
        case E_PET::MOLE_7:             level = 20;    break;
        case E_PET::HANDSOME_8:         level = 20;    break;
            
        default:
            break;
    }
    
    return level;
}

void PetNewManager::setPetLevel(E_PET ePet, int level)
{
    _uPetLevel[ePet] = level;
}

bool PetNewManager::getPet(E_PET ePet)
{
    bool bResult = false;
    if ( getPetLevel(ePet) > 0 )
    {
        bResult = true;
    }
    
    return bResult;
}

int PetNewManager::getPetAcquireFloor(E_PET ePet)
{
    int nResult = 0;
    switch (ePet) {
        case E_PET::CHICK_1:            nResult = 100;          break;
        case E_PET::HAMSTER_2:          nResult = 300;          break;
        case E_PET::DRAGON_3:           nResult = 600;          break;
        case E_PET::PRINCESS_4:         nResult = 900;          break;
        case E_PET::PURIRING_5:         nResult = 1100;         break;
        case E_PET::HONG_6:             nResult = 4500;         break;
        case E_PET::MOLE_7:             nResult = 0;            break;
        case E_PET::HANDSOME_8:         nResult = 6500;         break;
        case E_PET::D_1_PENGUIN:        nResult = 0;            break;
        case E_PET::D_2_HEDGEHOG:       nResult = 0;            break;
        case E_PET::D_3_GATEKEEPER:     nResult = 0;            break;
        case E_PET::D_4_FLASH:          nResult = 0;            break;
            
        default:
            break;
    }
    
    return nResult;
}

int PetNewManager::getPetUpgradePrice(E_PET ePet, int level)
{
    int nGem = 0;
    
    switch (ePet) {
        case E_PET::CHICK_1:            nGem = 200;    break;
        case E_PET::HAMSTER_2:          nGem = 600;    break;
        case E_PET::DRAGON_3:           nGem = 1000;    break;
        case E_PET::PRINCESS_4:         nGem = 1500;    break;
        case E_PET::PURIRING_5:         nGem = 2000;    break;
        case E_PET::HONG_6:             nGem = 1500;    break;
        case E_PET::MOLE_7:             nGem = 1500;    break;
        case E_PET::HANDSOME_8:         nGem = 1500;    break;
            
        default:
            break;
    }
    
    if ( level == 0 )
        return nGem;
    
    switch (ePet) {
        case E_PET::CHICK_1:
            nGem += 100 * level;
            break;
        case E_PET::HAMSTER_2:
            nGem += 100 * level;
            if ( nGem > 1500 )      nGem = 1500;
            break;
        case E_PET::DRAGON_3:
            nGem += 100 * level;
            if ( nGem > 2000 )      nGem = 2000;
            break;
        case E_PET::PRINCESS_4:
            nGem += 100 * level;
            if ( nGem > 2500 )      nGem = 2500;
            break;
        case E_PET::PURIRING_5:
            nGem += 100 * level;
            if ( nGem > 3000 )      nGem = 3000;
            break;
        case E_PET::HONG_6:
            nGem += 100 * level;
            if ( nGem > 2500 )      nGem = 2500;
            break;
        case E_PET::MOLE_7:
            nGem += 100 * level;
            if ( nGem > 2500 )      nGem = 2500;
            break;
        case E_PET::HANDSOME_8:
            nGem += 100 * level;
            if ( nGem > 2500 )      nGem = 2500;
            break;
        default:
            break;
    }
    
    return nGem;
}

#pragma mark - get, set : pet evolution
int PetNewManager::getPetEvolution(E_PET ePet)
{
    int level = _listPetEvolution[ePet].valueInt();
    if ( level > C_MAX_PET_EVOLUTION )
    {
        level = C_MAX_PET_EVOLUTION;
    }
    return level;
}

void PetNewManager::setPetEvolution(E_PET ePet, int level)
{
    _listPetEvolution[ePet] = level;
}

int PetNewManager::getPetEvolutionReward(E_PET ePet)
{
    return _listPetEvolutionReward[ePet].valueInt();
}

void PetNewManager::setPetEvolutionReward(E_PET ePet, int level)
{
    _listPetEvolutionReward[ePet] = level;
}

std::string PetNewManager::getPetEvolutionExplain(E_PET ePet, int nEvol)
{
    std::string strExpain = "";
    switch (ePet) {
        case E_PET::CHICK_1:
            if      (nEvol == 1)     strExpain.append(GAME_TEXT("t_pet_d1_option_evol_chick_1"));
            else if (nEvol == 2)     strExpain.append(GAME_TEXT("t_pet_d1_option_evol_chick_2"));
            else if (nEvol == 3)     strExpain.append(GAME_TEXT("t_pet_d1_option_evol_chick_2"));
            else if (nEvol == 4)     strExpain.append(GAME_TEXT("t_pet_d1_option_evol_chick_2"));
            else if (nEvol == 5)     strExpain.append(GAME_TEXT("t_pet_d1_option_evol_chick_3"));
            else if (nEvol == 6)     strExpain.append(GAME_TEXT("t_pet_d1_option_evol_chick_2"));
            else if (nEvol == 7)     strExpain.append(GAME_TEXT("t_pet_d1_option_evol_chick_2"));
            else if (nEvol == 8)     strExpain.append(GAME_TEXT("t_pet_d1_option_evol_chick_2"));
            else if (nEvol == 9)     strExpain.append(GAME_TEXT("t_pet_d1_option_evol_chick_2"));
            else if (nEvol == 10)    strExpain.append(GAME_TEXT("t_pet_d1_option_evol_chick_3"));
            break;
        case E_PET::HAMSTER_2:
            if      (nEvol == 1)     strExpain.append(GAME_TEXT("t_pet_d1_option_evol_hamster_1"));
            else if (nEvol == 2)     strExpain.append(GAME_TEXT("t_pet_d1_option_evol_hamster_2"));
            else                     strExpain.append(GAME_TEXT("t_pet_d1_option_evol_hamster_3"));
            break;
        case E_PET::DRAGON_3:
            if      (nEvol == 1)     strExpain.append(GAME_TEXT("t_pet_d1_option_evol_dragon_1"));
            else                     strExpain.append(GAME_TEXT("t_pet_d1_option_evol_dragon_2"));
            break;
        case E_PET::PRINCESS_4:
            if      (nEvol == 1)     strExpain.append(GAME_TEXT("t_pet_d1_option_evol_princess_1"));
            else if (nEvol == 2)     strExpain.append(GAME_TEXT("t_pet_d1_option_evol_princess_2"));
            else if (nEvol == 3)     strExpain.append(GAME_TEXT("t_pet_d1_option_evol_princess_3"));
            else if (nEvol == 4)     strExpain.append(GAME_TEXT("t_pet_d1_option_evol_princess_4"));
            else if (nEvol == 5)     strExpain.append(GAME_TEXT("t_pet_d1_option_evol_princess_5"));
            else if (nEvol == 6)     strExpain.append(GAME_TEXT("t_pet_d1_option_evol_princess_6"));
            else if (nEvol == 7)     strExpain.append(GAME_TEXT("t_pet_d1_option_evol_princess_7"));
            else if (nEvol == 8)     strExpain.append(GAME_TEXT("t_pet_d1_option_evol_princess_8"));
            else if (nEvol == 9)     strExpain.append(GAME_TEXT("t_pet_d1_option_evol_princess_9"));
            else if (nEvol == 10)    strExpain.append(GAME_TEXT("t_pet_d1_option_evol_princess_10"));
            break;;
        case E_PET::PURIRING_5:
            if      (nEvol == 1)     strExpain.append(GAME_TEXT("t_pet_d1_option_evol_puriring_1"));
            else                     strExpain.append(GAME_TEXT("t_pet_d1_option_evol_puriring_2"));
            break;;
        case E_PET::HONG_6:
            strExpain.append(GAME_TEXT("t_pet_d1_option_evol_hong_1"));
            break;
        case E_PET::MOLE_7:
            strExpain.append(GAME_TEXT("t_pet_d1_option_evol_mole_1"));
            break;
        case E_PET::HANDSOME_8:
            strExpain.append(GAME_TEXT("t_pet_d1_option_evol_handsome_1"));
            break;
            
        default:
            break;
    }
    
    return strExpain;
}

int PetNewManager::getPetEvolutionFloor(E_PET ePet, int nEvol)
{
    int nFloor = 0;
    
    switch (ePet)
    {
        case E_PET::CHICK_1:
            if      (nEvol == 0)     nFloor = 1201;
            else if (nEvol == 1)     nFloor = 3201;
            else if (nEvol == 2)     nFloor = 5201;
            else if (nEvol == 3)     nFloor = 7201;
            else if (nEvol == 4)     nFloor = 9201;
            else if (nEvol == 5)     nFloor = 11201;
            else if (nEvol == 6)     nFloor = 13201;
            else if (nEvol == 7)     nFloor = 15201;
            else if (nEvol == 8)     nFloor = 17201;
            else if (nEvol == 9)     nFloor = 19201;
            break;
        case E_PET::HAMSTER_2:
            if      (nEvol == 0)     nFloor = 1401;
            else if (nEvol == 1)     nFloor = 3401;
            else if (nEvol == 2)     nFloor = 5401;
            else if (nEvol == 3)     nFloor = 7401;
            else if (nEvol == 4)     nFloor = 9401;
            else if (nEvol == 5)     nFloor = 11401;
            else if (nEvol == 6)     nFloor = 13401;
            else if (nEvol == 7)     nFloor = 15401;
            else if (nEvol == 8)     nFloor = 17401;
            else if (nEvol == 9)     nFloor = 19401;
            break;
        case E_PET::DRAGON_3:
            if      (nEvol == 0)     nFloor = 1701;
            else if (nEvol == 1)     nFloor = 3701;
            else if (nEvol == 2)     nFloor = 5701;
            else if (nEvol == 3)     nFloor = 7701;
            else if (nEvol == 4)     nFloor = 9701;
            else if (nEvol == 5)     nFloor = 11701;
            else if (nEvol == 6)     nFloor = 13701;
            else if (nEvol == 7)     nFloor = 15701;
            else if (nEvol == 8)     nFloor = 17701;
            else if (nEvol == 9)     nFloor = 19701;
            break;
        case E_PET::PRINCESS_4:
            if      (nEvol == 0)     nFloor = 2001;
            else if (nEvol == 1)     nFloor = 4001;
            else if (nEvol == 2)     nFloor = 6001;
            else if (nEvol == 3)     nFloor = 8001;
            else if (nEvol == 4)     nFloor = 10001;
            else if (nEvol == 5)     nFloor = 12001;
            else if (nEvol == 6)     nFloor = 14001;
            else if (nEvol == 7)     nFloor = 16001;
            else if (nEvol == 8)     nFloor = 18001;
            else if (nEvol == 9)     nFloor = 20001;
            break;
        case E_PET::PURIRING_5:
            if      (nEvol == 0)     nFloor = 2201;
            else if (nEvol == 1)     nFloor = 4201;
            else if (nEvol == 2)     nFloor = 6201;
            else if (nEvol == 3)     nFloor = 8201;
            else if (nEvol == 4)     nFloor = 10201;
            else if (nEvol == 5)     nFloor = 12201;
            else if (nEvol == 6)     nFloor = 14201;
            else if (nEvol == 7)     nFloor = 16201;
            else if (nEvol == 8)     nFloor = 18201;
            else if (nEvol == 9)     nFloor = 20201;
            break;
        case E_PET::HONG_6:
            if      (nEvol == 0)     nFloor = 5601;
            else if (nEvol == 1)     nFloor = 7601;
            else if (nEvol == 2)     nFloor = 9601;
            else if (nEvol == 3)     nFloor = 11601;
            else if (nEvol == 4)     nFloor = 13601;
            else if (nEvol == 5)     nFloor = 15601;
            else if (nEvol == 6)     nFloor = 17601;
            else if (nEvol == 7)     nFloor = 19601;
            else if (nEvol == 8)     nFloor = 21601;
            else if (nEvol == 9)     nFloor = 23601;
            break;
        case E_PET::MOLE_7:
            if      (nEvol == 0)     nFloor = 6601;
            else if (nEvol == 1)     nFloor = 8601;
            else if (nEvol == 2)     nFloor = 10601;
            else if (nEvol == 3)     nFloor = 12601;
            else if (nEvol == 4)     nFloor = 14601;
            else if (nEvol == 5)     nFloor = 16601;
            else if (nEvol == 6)     nFloor = 18601;
            else if (nEvol == 7)     nFloor = 20601;
            else if (nEvol == 8)     nFloor = 22601;
            else if (nEvol == 9)     nFloor = 24601;
            break;
        case E_PET::HANDSOME_8:
            if      (nEvol == 0)     nFloor = 7601;
            else if (nEvol == 1)     nFloor = 9601;
            else if (nEvol == 2)     nFloor = 11601;
            else if (nEvol == 3)     nFloor = 13601;
            else if (nEvol == 4)     nFloor = 15601;
            else if (nEvol == 5)     nFloor = 17601;
            else if (nEvol == 6)     nFloor = 19601;
            else if (nEvol == 7)     nFloor = 21601;
            else if (nEvol == 8)     nFloor = 23601;
            else if (nEvol == 9)     nFloor = 25601;
            break;
        default:
            break;
    }
    
    return nFloor;
}

double PetNewManager::getPetEvolutionEffect1(E_PET ePet, int nEvol)
{
    //
    std::lock_guard<std::mutex> lock(_mutexEffectEvolution1);
    
    double nEffect = 0;
    switch (ePet)
    {
        case E_PET::CHICK_1:
            if (nEvol >= 2)  nEffect = 2;
            if (nEvol >= 3)  nEffect *= 2;
            if (nEvol >= 4)  nEffect *= 2;
            if (nEvol >= 6)  nEffect *= 2;
            if (nEvol >= 7)  nEffect *= 2;
            if (nEvol >= 8)  nEffect *= 2;
            if (nEvol >= 9)  nEffect *= 2;
            break;
        case E_PET::HAMSTER_2:
            if (nEvol >= 1)  nEffect *= 2;
            if (nEvol >= 2)  nEffect = atof(UserInfoManager::getInstance()->getMonsterBonusRate(_ePlace, false).c_str()) / 10000.0f;
            if (nEvol >= 3)  nEffect *= 2;
            if (nEvol >= 4)  nEffect *= 2;
            if (nEvol >= 5)  nEffect *= 2;
            if (nEvol >= 6)  nEffect *= 2;
            if (nEvol >= 7)  nEffect *= 2;
            if (nEvol >= 8)  nEffect *= 2;
            if (nEvol >= 9)  nEffect *= 2;
            if (nEvol >= 10) nEffect *= 2;
            break;
        case E_PET::DRAGON_3:
            if (nEvol >= 1)  nEffect = 20;
            if (nEvol >= 2)  nEffect += 1;
            if (nEvol >= 3)  nEffect += 1;
            if (nEvol >= 4)  nEffect += 1;
            if (nEvol >= 5)  nEffect += 1;
            if (nEvol >= 6)  nEffect += 1;
            if (nEvol >= 7)  nEffect += 1;
            if (nEvol >= 8)  nEffect += 1;
            if (nEvol >= 9)  nEffect += 1;
            if (nEvol >= 10) nEffect += 1;
            break;
        case E_PET::PURIRING_5:
            if (nEvol >= 2)  nEffect = 1.1;
            if (nEvol >= 3)  nEffect *= 1.1;
            if (nEvol >= 4)  nEffect *= 1.1;
            if (nEvol >= 5)  nEffect *= 1.1;
            if (nEvol >= 6)  nEffect *= 1.1;
            if (nEvol >= 7)  nEffect *= 1.1;
            if (nEvol >= 8)  nEffect *= 1.1;
            if (nEvol >= 9)  nEffect *= 1.1;
            if (nEvol >= 10) nEffect *= 1.1;
            break;
        case E_PET::HONG_6:
            if (nEvol >= 1)  nEffect = 1;
            if (nEvol >= 2)  nEffect += 1;
            if (nEvol >= 3)  nEffect += 1;
            if (nEvol >= 4)  nEffect += 1;
            if (nEvol >= 5)  nEffect += 1;
            if (nEvol >= 6)  nEffect += 1;
            if (nEvol >= 7)  nEffect += 1;
            if (nEvol >= 8)  nEffect += 1;
            if (nEvol >= 9)  nEffect += 1;
            if (nEvol >= 10) nEffect += 1;
            break;
        case E_PET::MOLE_7:
            if (nEvol >= 1)  nEffect = 0.05;
            if (nEvol >= 2)  nEffect += 0.05;
            if (nEvol >= 3)  nEffect += 0.05;
            if (nEvol >= 4)  nEffect += 0.05;
            if (nEvol >= 5)  nEffect += 0.05;
            if (nEvol >= 6)  nEffect += 0.05;
            if (nEvol >= 7)  nEffect += 0.05;
            if (nEvol >= 8)  nEffect += 0.05;
            if (nEvol >= 9)  nEffect += 0.05;
            if (nEvol >= 10) nEffect += 0.05;
            break;
        case E_PET::HANDSOME_8:
            if (nEvol >= 1)  nEffect = 2;
            if (nEvol >= 2)  nEffect *= 2;
            if (nEvol >= 3)  nEffect *= 2;
            if (nEvol >= 4)  nEffect *= 2;
            if (nEvol >= 5)  nEffect *= 2;
            if (nEvol >= 6)  nEffect *= 2;
            if (nEvol >= 7)  nEffect *= 2;
            if (nEvol >= 8)  nEffect *= 2;
            if (nEvol >= 9)  nEffect *= 2;
            if (nEvol >= 10) nEffect *= 2;
            break;
        default:
            break;
    }
    
    return nEffect;
}

double PetNewManager::getPetEvolutionEffect2(E_PET ePet, int nEvol)
{
    //
    std::lock_guard<std::mutex> lock(_mutexEffectEvolution2);
    
    double nEffect = 0;
    switch (ePet)
    {
        case E_PET::CHICK_1:
            if (nEvol >= 1)  nEffect += 10;
            if (nEvol >= 5)  nEffect += 10;
            if (nEvol >= 10) nEffect += 10;
            
            nEffect = nEffect + getPetEquipEffect(102, 0);
            break;
        case E_PET::PURIRING_5:
            if (nEvol >= 1) nEffect = 500;
        default:
            break;
    }
    
    return nEffect;
}

#pragma mark - get, set : pet reinforce
int PetNewManager::getPetReinforce(E_PET ePet)
{
    int level = _uPetReinforce[ePet].valueInt();
    if ( level > C_MAX_PET_GRADE )
    {
        level = C_MAX_PET_GRADE;
    }
    return level;
}

void PetNewManager::setPetReinforce(E_PET ePet, int level)
{
    _uPetReinforce[ePet] = level;
}

std::string PetNewManager::getPetReinforceExplain(E_PET ePet, int nLevel)
{
    std::string strExpain = "";
    double nDiff = 0;
    
    // diff
    {
        double nEffect = getPetReinforceEffect(ePet, nLevel);
        if ( nLevel == 0 || nLevel == 1 )
        {
            nDiff = nEffect;
        }
        else
        {
            double nEffectPrev = getPetReinforceEffect(ePet, nLevel - 1);
            
            if (ePet == E_PET::DRAGON_3 || ePet == E_PET::HONG_6 || ePet == E_PET::MOLE_7)
                nDiff = nEffect - nEffectPrev;
            else
                nDiff = nEffect / nEffectPrev;
        }
        
        if ( ePet == E_PET::HANDSOME_8)
        {
            nDiff = 1 - nDiff;
            nDiff *= 100;
        }
        else if ( ePet == E_PET::MOLE_7)
        {
            nDiff *= 100;
        }
    }
    
    switch (ePet) {
        case E_PET::CHICK_1:            strExpain.append(GAME_TEXTFORMAT("t_pet_d1_option_rein_chick", nDiff));         break;
        case E_PET::HAMSTER_2:          strExpain.append(GAME_TEXTFORMAT("t_pet_d1_option_rein_hamster", nDiff));       break;
        case E_PET::DRAGON_3:           strExpain.append(GAME_TEXTFORMAT("t_pet_d1_option_rein_dragon", nDiff));        break;
        case E_PET::PRINCESS_4:         strExpain.append(GAME_TEXTFORMAT("t_pet_d1_option_rein_princess", nDiff));      break;
        case E_PET::PURIRING_5:         strExpain.append(GAME_TEXTFORMAT("t_pet_d1_option_rein_puriring", nDiff));      break;
        case E_PET::HONG_6:             strExpain.append(GAME_TEXTFORMAT("t_pet_d1_option_rein_hong", nDiff));          break;
        case E_PET::MOLE_7:             strExpain.append(GAME_TEXTFORMAT("t_pet_d1_option_rein_mole", nDiff));          break;
        case E_PET::HANDSOME_8:         strExpain.append(GAME_TEXTFORMAT("t_pet_d1_option_rein_handsome", nDiff));      break;
            
        default:
            break;
    }
    
    return strExpain;
}

std::vector<int> PetNewManager::getPetReinforceUpgradePrice(E_PET ePet, int nLevel)
{
    std::vector<int> listMine;
    for ( int i = 0; i <= C_COUNT_MINE_MINERAL; i++ )
    {
        listMine.push_back(0);
    }
    
    switch (ePet) {
        case E_PET::CHICK_1:
            listMine.at(E_MINERAL::MINERAL_1)            = 7000;
            listMine.at(E_MINERAL::MINERAL_4)            = 2000;
            listMine.at(E_MINERAL::MINERAL_6)            = 300;
            listMine.at(E_MINERAL::MINERAL_7_SAPPHIRE)     = 20;
            break;
        case E_PET::HAMSTER_2:
            listMine.at(E_MINERAL::MINERAL_2)            = 7000;
            listMine.at(E_MINERAL::MINERAL_5)            = 2000;
            listMine.at(E_MINERAL::MINERAL_6)            = 300;
            listMine.at(E_MINERAL::MINERAL_7_SAPPHIRE)     = 20;
            break;
        case E_PET::DRAGON_3:
            listMine.at(E_MINERAL::MINERAL_3)            = 7000;
            listMine.at(E_MINERAL::MINERAL_4)            = 2000;
            listMine.at(E_MINERAL::MINERAL_6)            = 300;
            listMine.at(E_MINERAL::MINERAL_8_OPAL)         = 20;
            break;
        case E_PET::PRINCESS_4:
            listMine.at(E_MINERAL::MINERAL_1)            = 7000;
            listMine.at(E_MINERAL::MINERAL_5)            = 2000;
            listMine.at(E_MINERAL::MINERAL_6)            = 300;
            listMine.at(E_MINERAL::MINERAL_9_AQUA)         = 20;
            break;
        case E_PET::PURIRING_5:
            listMine.at(E_MINERAL::MINERAL_2)            = 7000;
            listMine.at(E_MINERAL::MINERAL_4)            = 2000;
            listMine.at(E_MINERAL::MINERAL_6)            = 300;
            listMine.at(E_MINERAL::MINERAL_10_EMERALD)      = 20;
            break;
        case E_PET::HONG_6:
            listMine.at(E_MINERAL::MINERAL_3)            = 7000;
            listMine.at(E_MINERAL::MINERAL_5)            = 2000;
            listMine.at(E_MINERAL::MINERAL_6)            = 300;
            listMine.at(E_MINERAL::MINERAL_11_AMETHYST)     = 20;
            break;
        case E_PET::MOLE_7:
            listMine.at(E_MINERAL::MINERAL_3)            = 7000;
            listMine.at(E_MINERAL::MINERAL_5)            = 2000;
            listMine.at(E_MINERAL::MINERAL_6)            = 300;
            listMine.at(E_MINERAL::MINERAL_11_AMETHYST)     = 20;
            break;
        case E_PET::HANDSOME_8:
            listMine.at(E_MINERAL::MINERAL_3)            = 7000;
            listMine.at(E_MINERAL::MINERAL_5)            = 2000;
            listMine.at(E_MINERAL::MINERAL_6)            = 300;
            listMine.at(E_MINERAL::MINERAL_11_AMETHYST)     = 20;
            break;
            
        default:
            break;
    }
    
    int nMul = 1;
    
    if      (nLevel == 0)     nMul = 1;
    else if (nLevel == 1)     nMul = 2;
    else if (nLevel == 2)     nMul = 4;
    else if (nLevel == 3)     nMul = 8;
    else if (nLevel == 4)     nMul = 16;
    
    for ( int i = 0; i < listMine.size(); i++ )
    {
        listMine.at(i) = listMine.at(i) * nMul;
    }
    
    return listMine;
}

double PetNewManager::getPetReinforceEffect(E_PET ePet, int nLevel)
{
    //
    std::lock_guard<std::mutex> lock(_mutexEffectReinforce);
    
    double nEffect = 0;
    if ( nLevel <= 0)
    {
        return nEffect;
    }
    
    switch (ePet) {
        case E_PET::CHICK_1:
            nEffect = pow(2, nLevel);
            break;
        case E_PET::HAMSTER_2:
            nEffect = pow(5, nLevel);
            break;
        case E_PET::DRAGON_3:
            if      (nLevel == 1)    nEffect = 1;
            else if (nLevel == 2)    nEffect = 2;
            else if (nLevel == 3)    nEffect = 3;
            else if (nLevel == 4)    nEffect = 4;
            else if (nLevel == 5)    nEffect = 4.9;
            break;
        case E_PET::PRINCESS_4:
            nEffect = pow(2, nLevel);
            break;
        case E_PET::PURIRING_5:
            nEffect = pow(1.3, nLevel);
            break;
        case E_PET::HONG_6:
            if      (nLevel == 1)    nEffect = 5;
            else if (nLevel == 2)    nEffect = 10;
            else if (nLevel == 3)    nEffect = 15;
            else if (nLevel == 4)    nEffect = 20;
            else if (nLevel == 5)    nEffect = 30;
            break;
        case E_PET::MOLE_7:
            if      (nLevel == 1)    nEffect = 0.1;
            else if (nLevel == 2)    nEffect = 0.2;
            else if (nLevel == 3)    nEffect = 0.3;
            else if (nLevel == 4)    nEffect = 0.4;
            else if (nLevel == 5)    nEffect = 0.5;
            break;
        case E_PET::HANDSOME_8:
            nEffect = pow(0.8, nLevel);
            break;
            
        default:
            break;
    }
    
    return nEffect;
}



#pragma mark - get, set : pet skin
int PetNewManager::getPetSkin(E_PET ePet)
{
    return _uPetSkin[ePet].valueInt();
}

void PetNewManager::setPetSkin(E_PET ePet, int idx)
{
    _uPetSkin[ePet] = idx;
}

bool PetNewManager::isPetSkinBuy(E_PET ePet, int idx)
{
    bool isResult = false;
    
    std::string strData = MafUtils::format("%d_%d", ePet, idx);
    if ( std::find(_listPetSkinBuy.begin(), _listPetSkinBuy.end(), strData) != _listPetSkinBuy.end() )
    {
        isResult = true;
    }
    
    if ( idx == 0 )
    {
        isResult = true;
    }
    
    return isResult;
}

void PetNewManager::setPetSkinBuy(E_PET ePet, int idx)
{
    std::string strData = MafUtils::format("%d_%d", ePet, idx);
    if ( std::find(_listPetSkinBuy.begin(), _listPetSkinBuy.end(), strData) != _listPetSkinBuy.end() )
    {
        return;
    }
    
    if ( idx == 0 )
    {
        return;
    }
    
    _listPetSkinBuy.push_back(strData);
}

#pragma mark - get, set : pet equip
int PetNewManager::getPetEquipLevel(int idx)
{
    int level = 0;
    
    auto iter = _listPetEquipLevel.find(idx);
    if ( iter != _listPetEquipLevel.end() )
    {
        level = _listPetEquipLevel[idx].valueInt();
    }
    
    return level;
}

int PetNewManager::getPetEquipLevelMax()
{
    return ParameterManager::getInstance()->getParameterInteger("pet_equip_max_level");
}

void PetNewManager::setPetEquipLevel(int idx, int level)
{
    _listPetEquipLevel[idx] = level;
}

int PetNewManager::getPetEquipAmount(int idx)
{
    int amount = 0;
    
    auto iter = _listPetEquipAmount.find(idx);
    if ( iter != _listPetEquipAmount.end() )
    {
        amount = _listPetEquipAmount[idx].valueInt();
    }
    
    return amount;
}

int PetNewManager::getPetEquipAmountNeed(int level)
{
    int amount = 0;
    
    auto objPetEquipLevel = getInfoPetEquipLevel(level);
    if ( objPetEquipLevel != nullptr )
    {
        amount = objPetEquipLevel->getAmount();
    }
    
    return amount;
}

void PetNewManager::setPetEquipAmount(int idx, int level)
{
    _listPetEquipAmount[idx] = level;
}

std::string PetNewManager::getPetEquipExplain(int idx, int levelAdd/* = 0*/)
{
    std::string explain = "";
    
    auto objPetEquip = getInfoPetEquip(idx);
    if ( objPetEquip != nullptr )
    {
        explain = objPetEquip->getDesc();
        
        double effect = getPetEquipEffect(idx, (int)objPetEquip->getEffectType(), levelAdd, true);
        if ( idx == 202 || idx == 503 || idx == 603 || idx == 703 )
        {
            double chance = objPetEquip->getProperty2() / 10;
            explain = MafUtils::format(explain.c_str(), chance, effect);
        }
        else
        {
            explain = MafUtils::format(explain.c_str(), effect);
        }
    }
    
    return explain;
}

double PetNewManager::getPetEquipEffect(int idx, int returntype, int levelAdd/* = 0*/, bool ignore/* = false*/)
{
    //
    std::lock_guard<std::mutex> lock(_mutexEffectEquip);
    
    // levelAdd - 현재 장비 레벨 보다 높은 레벨 효과 주고 싶을때,
    // ignorechance - 확률 있는 아이템의 효과를 확률 관계 없이 무조건 들고 오고 싶을 때
    int level = getPetEquipLevel(idx) + levelAdd;
    if ( level > getPetEquipLevelMax() )
    {
        level = getPetEquipLevelMax();
    }
    
    auto objPetEquip = getInfoPetEquip(idx);
    if ( objPetEquip == nullptr )
    {
        return returntype;
    }
    
    if ( ignore == false && getPet(objPetEquip->getType()) == false )
    {// 펫이 없을때 리턴
        return returntype;
    }

    double property1 = objPetEquip->getProperty1();
    double property2 = objPetEquip->getProperty2();
    double increase = objPetEquip->getIncrease();

    if ( ignore == false && property2 < 1000 )
    {//확률 무시하고 싶을때
        if(random(0, 1000) > property2)
        {
            return returntype;
        }
    }
    
    double effect = 0;
    switch (objPetEquip->getEffectType()) {
            
        case E_PET_EQUIP_TYPE::CHICK_DMG_1:
            effect = std::pow(property1, level * increase / 2) + level * increase;
            break;
        case E_PET_EQUIP_TYPE::HAMSTER_GOLD_2:
            effect = (int)(std::pow(level, increase) + property1);
            break;
        case E_PET_EQUIP_TYPE::PURIRING_KEY_3:
            effect = (std::pow(increase, level) + property1)/1.4;
            break;
        case E_PET_EQUIP_TYPE::MOLE_MINE_4:
            effect = ceil(property1 + std::pow(1.5, increase) * level);
            break;
        case E_PET_EQUIP_TYPE::DRAGON_DMG_5:
            effect = property1 + (increase * level);
            break;
        case E_PET_EQUIP_TYPE::PRINCESS_DELAY_6:
            effect = 1 - (increase * level) - property1;
            break;
        case E_PET_EQUIP_TYPE::CHICK_PROBABILITY_7:
            effect = property1 * level * increase;
            break;
        case E_PET_EQUIP_TYPE::CHICK_DELAY_8:
            effect = 1 - property1 * (increase * level);
            break;
        case E_PET_EQUIP_TYPE::MONSTER_GOLD_9:
            effect = level * increase * property1 + 1;
            break;
        case E_PET_EQUIP_TYPE::PURIRING_MONSTER_KEY_10:
            effect = std::pow(increase, level) - property1;
            break;
        case E_PET_EQUIP_TYPE::CRI_DMG_11:
        {
            int bonus = CostumeManager::getInstance()->getExistTypeCount(E_COSTUME::HEAD);
            effect = increase * level / 40 * bonus + property1;
            break;
        }
        case E_PET_EQUIP_TYPE::ALL_DMG_12:
        {
            int count = 0;
            int bonus = std::atoi(SaveManager::getHistoryData(E_HISTORY_DATA::K_PLAY_TIME).c_str());
            bonus = bonus / (60 * 60);
            while(true)
            {
                if(bonus == 0)
                {
                    break;
                }
                else if(bonus > 0)
                {
                    count++;
                }
                bonus = bonus/10;
            }
            
            effect = increase * level / 40 * count + property1;
            break;
        }
            
        default:
            break;
    }
    
    return effect;
}

int PetNewManager::getPetEquipCollectLevelAverage()
{
    int levelMin = getPetEquipLevelMax();
    
    for (auto iter = _listPetEquipLevel.begin() ; iter !=  _listPetEquipLevel.end(); iter++)
    {
        if ( iter->second < levelMin )
        {
            levelMin = iter->second.valueInt();
        }
    }
    
    return levelMin;
}

int PetNewManager::getPetEquipCollectLevelTotal()
{
    int levelTotal = 0;
    
    for (auto iter = _listPetEquipLevel.begin() ; iter !=  _listPetEquipLevel.end(); iter++)
    {
        levelTotal += iter->second;
    }
    
    return levelTotal / 10 * 10;
}

double PetNewManager::getPetEquipCollectEffect()
{
    double effect = 0;
    double levelTotal = getPetEquipCollectLevelTotal();
    
    if ( levelTotal > 0 )
    {
        if ( levelTotal <= 170 )
        {
            effect = ((levelTotal / 29) * 2) + 1;
        }
        else if ( levelTotal <= 310 )
        {
            effect = ((levelTotal / 29) - 1) * 10 * ((levelTotal / 29) - 5);
        }
        else if ( levelTotal <= 600 )
        {
            effect = std::pow(20, ((levelTotal / 29) - 9)) * 2;
        }
        else
        {
            effect = std::pow(17, ((levelTotal / 29) - 9)) * 12;
        }
    }
    
    
    //모험 유물 적용
    double advtRelicBuff = AdventureRelicManager::getInstance()->getRelicTotalEffect(E_ADVENTURE_PROPERTY_TYPE::PET_EQUIP_COLLECT_ATK_BONUS);
    int relicBonus = effect * advtRelicBuff * 0.01;
    
    effect += relicBonus;
    
    return effect;
}

double PetNewManager::getPetEquipCollectEffectBongsik()
{
    double effect = 0;
    double levelTotal = getPetEquipCollectLevelTotal();

    if ( levelTotal > 0 )
    {
        //effect = int(std::pow(1.4, levelTotal / 29) + 5) - 4; 2023년 11월 이전
        effect = std::pow(3.0, levelTotal / 29);
        effect = std::floor(effect);
    }
    
    return effect;
}

int PetNewManager::getPetEquipMaxUseCount()
{
    int result = 0;
    
    for (auto iter = _listPetEquipAmount.begin() ; iter !=  _listPetEquipAmount.end(); iter++)
    {
        if ( iter->second >= ParameterManager::getInstance()->getParameterInteger("pet_equip_max_count"))
            result++;
    }
    
    return result;
}

#pragma mark - event
int PetNewManager::onPetFree(E_PET ePet)
{
    /*
     0 : 성공
     1 : 조건 안됨
     */
    int level = PetNewManager::getInstance()->getPetLevel(ePet);
    if ( level > 0 )
    {
        return 1;
    }
    
    int floor = UserInfoManager::getInstance()->getHighFloor(_ePlace);
    int floorAcquire = PetNewManager::getInstance()->getPetAcquireFloor(ePet);

    bool bFree = false;
    if ( ePet == E_PET::MOLE_7 && WeaponManager::getInstance()->getEquipHigh() >= E_WEAPON::WEAPON_76_MINE )
    {
        bFree = true;
    }
    else if ( floorAcquire != 0 && floor > floorAcquire )
    {
        bFree = true;
    }
    
    if ( bFree == false )
    {
        return 1;
    }
    
    //
    setPetLevel(ePet, 1);
    
    //
    saveData();
    
    //
    PopupToastPet::show(ePet);
    
    //
    if(ePet == E_PET::CHICK_1)          StoryManager::getInstance()->showStory((int)E_COMIC::CLEAR_100);
    else if(ePet == E_PET::HAMSTER_2)   StoryManager::getInstance()->showStory((int)E_COMIC::CLEAR_300);
    else if(ePet == E_PET::DRAGON_3)    StoryManager::getInstance()->showStory((int)E_COMIC::CLEAR_600);
    else if(ePet == E_PET::PRINCESS_4)  StoryManager::getInstance()->showStory((int)E_COMIC::CLEAR_900);
    else if(ePet == E_PET::PURIRING_5)  StoryManager::getInstance()->showStory((int)E_COMIC::CLEAR_1100);
    else if(ePet == E_PET::HONG_6)      StoryManager::getInstance()->showStory((int)E_COMIC::CLEAR_4500);
    else if(ePet == E_PET::MOLE_7)      StoryManager::getInstance()->showStory((int)E_COMIC::MINE_JOIN);
    else if(ePet == E_PET::HANDSOME_8)  StoryManager::getInstance()->showStory((int)E_COMIC::CLEAR_6500);
    
    //
    AchievementManager::getInstance()->setAchieveNormalMission(E_NORMAL_ACHIEVE_MISSION::ACHIEVE_PET_GET, 1, true);
    
    //
    ValueMap listData;
    listData["type"] = Value((int)ePet);
    RefreshManager::getInstance()->refreshUI(E_REFRESH::PET_FREE, listData);
    
    return 0;
}

int PetNewManager::onPetLevelUP(E_PET ePet)
{
    /*
     0 : 성공
     1 : 조건 안됨
     2 : 재화 부족
     */
    int level = PetNewManager::getInstance()->getPetLevel(ePet);
    int levelMax = PetNewManager::getInstance()->getPetLevelMax(ePet);
    bool bLevelMax = level >= levelMax ? true : false;
    
    if ( bLevelMax == true )
    {
        return 1;
    }
    
    auto itemNow = ItemsMoneyManager::getInstance()->getGem();
    auto itemNeed = getPetUpgradePrice(ePet, level);
    if ( itemNow < itemNeed )
    {
        return 2;
    }
    
    //
    setPetLevel(ePet, level + 1);
    ItemsMoneyManager::getInstance()->setGem(-itemNeed);
    
    //
    saveData();
    
    //
    AchievementManager::getInstance()->setAchieveNormalMission(E_NORMAL_ACHIEVE_MISSION::ACHIEVE_PET_LEVELUP, 1, true);
    
    //
    ValueMap listData;
    listData["type"] = Value((int)ePet);
    RefreshManager::getInstance()->refreshUI(E_REFRESH::PET, listData);
    
    return 0;
}

int PetNewManager::onPetEvolution(E_PET ePet)
{
    /*
     0 : 성공
     1 : 조건 안됨
     2 : 최대 레벨
     */
    int level = getPetEvolution(ePet);
    int levelMax = C_MAX_PET_EVOLUTION;
    if ( level < 0 || level >= levelMax )
    {
        return 1;
    }
 
    bool bEvolution = false;
    
    int floor = UserInfoManager::getInstance()->getHighFloor(_ePlace);
    for (int i = level; i < C_MAX_PET_EVOLUTION; i++ )
    {
        if ( floor >= PetNewManager::getInstance()->getPetEvolutionFloor(ePet, i) )
        {
            setPetEvolution(ePet, i+1);
            bEvolution = true;
        }
    }
    
    if ( bEvolution == false )
    {
        return 1;
    }
    
    //
    saveData();
    
    //
    int evolutionLevelTotal = 0;
    for ( int i = 1; i < E_PET::D_1_PENGUIN; i++ )
    {
        evolutionLevelTotal += getPetEvolution((E_PET)i);
    }
    AchievementManager::getInstance()->setAchieveNormalMission(E_NORMAL_ACHIEVE_MISSION::ACHIEVE_PET_EVOLUTION, evolutionLevelTotal);
    
    //
    ValueMap listData;
    listData["type"] = Value((int)ePet);
    RefreshManager::getInstance()->refreshUI(E_REFRESH::PET, listData);
    
    return 0;
}

int PetNewManager::onPetEvolutionReward(E_PET ePet)
{
    /*
     0 : 성공
     1 : 조건 안됨
     */
    int evolutionLevel = getPetEvolution(ePet);
    int evolutionReward = getPetEvolutionReward(ePet) + 1;
    if ( evolutionReward > evolutionLevel )
    {
        return 1;
    }
    
    //
    setPetEvolutionReward(ePet, evolutionReward);
    
    //
    auto objItems = ItemsManager::getInstance()->getInfoItems(E_ITEMS::GEM);
    objItems->setCount(100);
    
    ItemsManager::getInstance()->addItem(objItems);
    
    // save
    saveData();
    
    //
    RefreshReddotManager::getInstance()->refreshUI(E_REFRESH_REDDOT::PET_EVOLUTION_CHICK);
    RefreshReddotManager::getInstance()->refreshUI(E_REFRESH_REDDOT::PET_EVOLUTION_HAMSTER);
    RefreshReddotManager::getInstance()->refreshUI(E_REFRESH_REDDOT::PET_EVOLUTION_DRAGON);
    RefreshReddotManager::getInstance()->refreshUI(E_REFRESH_REDDOT::PET_EVOLUTION_PRINCESS);
    RefreshReddotManager::getInstance()->refreshUI(E_REFRESH_REDDOT::PET_EVOLUTION_PURIRING);
    RefreshReddotManager::getInstance()->refreshUI(E_REFRESH_REDDOT::PET_EVOLUTION_HONG);
    RefreshReddotManager::getInstance()->refreshUI(E_REFRESH_REDDOT::PET_EVOLUTION_MOLE);
    RefreshReddotManager::getInstance()->refreshUI(E_REFRESH_REDDOT::PET_EVOLUTION_HANDSOME);
    RefreshReddotManager::getInstance()->refreshUI(E_REFRESH_REDDOT::PET_EVOLUTION_ALL);
    
    //
    ValueMap listData;
    listData["type"] = Value((int)ePet);
    RefreshManager::getInstance()->refreshUI(E_REFRESH::PET, listData);
    
    return 0;
}

int PetNewManager::onPetReinforce(E_PET ePet)
{
    /*
     0 : 성공
     1 : 조건 안됨
     2 : 보유하지 않음
     3 : 최대 레벨
     4 : 재화 부족
     */
    
    if ( getPet(ePet) == false )
    {
        return 2;
    }
    
    //
    int reinforceLevel = getPetReinforce(ePet);
    if ( reinforceLevel >= C_MAX_PET_GRADE )
    {
        return 3;
    }
    
    
    //
    bool enough = true;
    
    std::vector<int> listMatter = getPetReinforceUpgradePrice(ePet, reinforceLevel);
    for ( int i = 0; i < listMatter.size(); i++ )
    {
        E_MINERAL eMineral = (E_MINERAL)i;
        
        int mineralNeed = listMatter.at(i);
        int mineralNow = MineralManager::getInstance()->getCount(eMineral);
        if ( mineralNeed > mineralNow )
        {
            enough = false;
        }
    }
    
    if ( enough == false )
    {
        return 4;
    }
    
    for ( int i = 0; i < listMatter.size(); i++ )
    {
        E_MINERAL eMineral = (E_MINERAL)i;
        
        int mineralNeed = listMatter.at(i);
        int mineralNow = MineralManager::getInstance()->getCount(eMineral);
        if ( mineralNeed <= 0 )
        {
            continue;
        }
        
        MineralManager::getInstance()->setCount(eMineral, mineralNow-mineralNeed);
    }
    
    //
    setPetReinforce(ePet, reinforceLevel + 1);
    
    // save
    saveData();
    MineralManager::getInstance()->saveData();
    
    //
    AchievementManager::getInstance()->setAchieveNormalMission(E_NORMAL_ACHIEVE_MISSION::ACHIEVE_PET_ENHANCE, 1, true);

    //
    RefreshReddotManager::getInstance()->refreshUI(E_REFRESH_REDDOT::PET_REINFORCE_CHICK);
    RefreshReddotManager::getInstance()->refreshUI(E_REFRESH_REDDOT::PET_REINFORCE_HAMSTER);
    RefreshReddotManager::getInstance()->refreshUI(E_REFRESH_REDDOT::PET_REINFORCE_DRAGON);
    RefreshReddotManager::getInstance()->refreshUI(E_REFRESH_REDDOT::PET_REINFORCE_PRINCESS);
    RefreshReddotManager::getInstance()->refreshUI(E_REFRESH_REDDOT::PET_REINFORCE_PURIRING);
    RefreshReddotManager::getInstance()->refreshUI(E_REFRESH_REDDOT::PET_REINFORCE_HONG);
    RefreshReddotManager::getInstance()->refreshUI(E_REFRESH_REDDOT::PET_REINFORCE_MOLE);
    RefreshReddotManager::getInstance()->refreshUI(E_REFRESH_REDDOT::PET_REINFORCE_HANDSOME);
    RefreshReddotManager::getInstance()->refreshUI(E_REFRESH_REDDOT::PET_REINFORCE_ALL);
    
    //
    ValueMap listData;
    listData["type"] = Value((int)ePet);
    RefreshManager::getInstance()->refreshUI(E_REFRESH::PET, listData);
    
    if ( ePet == E_PET::MOLE_7 )
    {
        RefreshManager::getInstance()->refreshUI(E_REFRESH::MINE);
    }
    
    return 0;
}

int PetNewManager::onPetSkinEquip(E_PET ePet, int idx)
{
    /*
     0 : 성공
     1 : 조건 안됨
     */
    
    if ( isPetSkinBuy(ePet, idx) == false )
    {
        return 1;
    }
    
    //
    setPetSkin(ePet, idx);
    
    // save
    saveData();
    
    //
    ValueMap listData;
    listData["type"] = Value((int)ePet);
    RefreshManager::getInstance()->refreshUI(E_REFRESH::PET, listData);
       
       
    return 0;
}

int PetNewManager::onPetSkinBuy(E_PET ePet, int idx, bool bPopupLoading, const std::function<void(bool, int)>& callback)
{
    /*
     0 : 성공
     1 : 조건 안됨
     2 : 판매기간 아님
     */
    auto objPetSkin = getInfoPetSkin(ePet, idx);
    if ( objPetSkin == nullptr )
    {
        return 1;
    }
    
    if ( AccountManager::getInstance()->getUserIdx() == 0 )
    {
        return 1;
    }
    
    if ( objPetSkin->isEventSale() == false )
    {
        return 2;
    }
    
    //
    requestSkinBuy(ePet, objPetSkin->getIdx(), objPetSkin->getItemIdx(), objPetSkin->getItemCount(), bPopupLoading, callback);
    return 0;
}

int PetNewManager::onPetEquipLevelUP(int idx, bool bPopupLoading, const std::function<void(bool)>& callback)
{
    /*
     0 : 성공
     1 : 조건 안됨
     2 : 재화 부족
     3 : 최대 레벨
     */
    
    int level = getPetEquipLevel(idx);
    int levelMax = PetNewManager::getInstance()->getPetEquipLevelMax();
    if ( level <= 0 )
    {
        return 1;
    }
    
    bool bLevelMax = level >= levelMax ? true : false;
    if ( bLevelMax == true )
    {
        return 3;
    }
    
    int itemNeed = PetNewManager::getInstance()->getPetEquipAmountNeed(level);
    int itemNow = PetNewManager::getInstance()->getPetEquipAmount(idx);
    if ( itemNow < itemNeed )
    {
        return 2;
    }
    
    //
    requestEquipLevelUP(idx, level, bPopupLoading, callback);
    return 0;
}


#pragma mark - network
void PetNewManager::requestSkinBuy(E_PET ePet, int nSkinIdx, int nItemIdx, std::string strItemCount, bool bPopupLoading, const std::function<void(bool, int)>& callback)
{
    if ( bPopupLoading == true )
    {
        PopupLoading::show();
    }
    
    //
    _callbackSkinBuy = callback;
    
    //
    std::string url = "/v1/pet/buyCostume";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_pet", ePet);
    req->setData("_costume", nSkinIdx);
    req->setData("_item_type", nItemIdx);
    req->setData("_item_count", std::atoi(strItemCount.c_str()));
    
    MafResponseNew cb = [=](void* http,network::HttpResponse* response,const char *data){
        if ( bPopupLoading == true )
        {
            PopupLoading::hide();
        }
        
        std::string dd = data;
        responseSkinBuy(response,dd);
    };
    req->send(cb);
}

void PetNewManager::responseSkinBuy(cocos2d::network::HttpResponse* response,std::string &data)
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
    
    if ( bError == false && jsonParser.HasMember("_result") == false )
    {
        bError = true;
    }
    
    if ( bError == true )
    {
        if ( _callbackSkinBuy != nullptr )
        {
            _callbackSkinBuy(false, -1);
            _callbackSkinBuy = nullptr;
        }
        return;
    }
    
    int nResult = jsonParser["_result"].GetInt();
    if ( nResult == 1 || nResult == 4 )
    {
        E_PET ePet = (E_PET)jsonParser["_pet"].GetInt();
        int nSkinIdx = jsonParser["_costume"].GetInt();
         
        
        std::string strReward = jsonParser["_reward"].GetString();
        std::string strItems = jsonParser["_items"].GetString();
        ItemsManager::getInstance()->addItems(strReward);
        ItemsManager::getInstance()->setItemsServer(strItems);
        
        //
        setPetSkinBuy(ePet, nSkinIdx);
        setPetSkin(ePet, nSkinIdx);
        
        //
        saveData();
        
        //
        bool bStory = true;
        if ( nSkinIdx == 1 )
        {
            for ( int i = 1; i <= C_COUNT_PET; i++ )
            {
                if ( isPetSkinBuy((E_PET)i, 1) == false )
                {
                    bStory = false;
                    break;
                }
            }

            if ( bStory == true )
            {
                StoryManager::getInstance()->showStory((int)E_COMIC::EVENT_SUMMER);
            }
        }
        else if ( nSkinIdx == 2 )
        {
            StoryManager::getInstance()->showStory((int)E_COMIC::EVENT_HALLOWEEN);
        }
     
        if ( _callbackSkinBuy != nullptr )
        {
            _callbackSkinBuy(true, nResult);
            _callbackSkinBuy = nullptr;
        }
    }
    else
    {
        if ( _callbackSkinBuy != nullptr )
        {
            _callbackSkinBuy(false, nResult);
            _callbackSkinBuy = nullptr;
        }
    }
}

void PetNewManager::requestEquipInfo(bool bPopupLoading, const std::function<void(bool)>& callback)
{
    if ( bPopupLoading == true )
    {
        PopupLoading::show();
    }
    
    //
    _callbackEquipInfo = callback;
    
    //
    std::string url = "/pet/equip/v1/info/";
    url.append(MafUtils::toString(AccountManager::getInstance()->getUserIdx()));
    
    auto req = new MafHttpNew(network::HttpRequest::Type::GET, url.c_str());
    MafResponseNew cb = [=](void* http,network::HttpResponse* response,const char *data){
        if ( bPopupLoading == true )
        {
            PopupLoading::hide();
        }
        
        std::string dd = data;
        responseEquipInfo(response,dd);
    };
    req->send(cb);
}

void PetNewManager::responseEquipInfo(cocos2d::network::HttpResponse* response,std::string &data)
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
        if ( _callbackEquipInfo != nullptr )
        {
            _callbackEquipInfo(false);
            _callbackEquipInfo = nullptr;
        }
        return;
    }
    
    for(int i = 0; i < jsonParser["_petdata"].Size(); ++i)
    {
        const rapidjson::Value& jsonValue = jsonParser["_petdata"][i];

        auto idx = jsonValue["_equipidx"].GetInt();
        auto level = jsonValue["_level"].GetInt();
        auto count = jsonValue["_count"].GetInt();

        setPetEquipLevel(idx, level);
        setPetEquipAmount(idx, count);
    }
    
    if ( _callbackEquipInfo != nullptr )
    {
        _callbackEquipInfo(true);
        _callbackEquipInfo = nullptr;
    }
}

void PetNewManager::requestEquipLevelUP(int idx, int level, bool bPopupLoading, const std::function<void(bool)>& callback)
{
    if ( bPopupLoading == true )
    {
        PopupLoading::show();
    }
    
    //
    _callbackEquipLevelUP = callback;
    
    //
    std::string url = "/pet/equip/v1/levelup";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_equipidx", idx);
    req->setData("_level", level);
    
    MafResponseNew cb = [=](void* http,network::HttpResponse* response,const char *data){
        if ( bPopupLoading == true )
        {
            PopupLoading::hide();
        }
        
        std::string dd = data;
        responseEquipLevelUP(idx, response, dd);
    };
    req->send(cb);
}

void PetNewManager::responseEquipLevelUP(int idx, cocos2d::network::HttpResponse* response,std::string &data)
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
    
    if ( bError == false && jsonParser.HasMember("_result") == false )
    {
        bError = true;
    }
    
    if ( bError == true )
    {
        if ( _callbackEquipLevelUP != nullptr )
        {
            _callbackEquipLevelUP(false);
            _callbackEquipLevelUP = nullptr;
        }
        return;
    }
    
    bool bResult = jsonParser["_result"].GetBool();
    if ( bResult == true )
    {
        auto level = jsonParser["_level"].GetInt();
        auto count = jsonParser["_total_equip"].GetInt();

        setPetEquipLevel(idx, level);
        setPetEquipAmount(idx, count);
     
        //
        auto callback = _callbackEquipLevelUP;
        _callbackEquipLevelUP = nullptr;
        
        if ( callback != nullptr )
        {
            callback(true);
        }
        
        //
        AchievementManager::getInstance()->setAchieveNormalMission(E_NORMAL_ACHIEVE_MISSION::ACHIEVE_PET_EQUIP_COLLECT_LEVEL, getPetEquipCollectLevelAverage());
        
        //
        RefreshReddotManager::getInstance()->refreshUI(E_REFRESH_REDDOT::PET_EQUIP_CHICK);
        RefreshReddotManager::getInstance()->refreshUI(E_REFRESH_REDDOT::PET_EQUIP_HAMSTER);
        RefreshReddotManager::getInstance()->refreshUI(E_REFRESH_REDDOT::PET_EQUIP_DRAGON);
        RefreshReddotManager::getInstance()->refreshUI(E_REFRESH_REDDOT::PET_EQUIP_PRINCESS);
        RefreshReddotManager::getInstance()->refreshUI(E_REFRESH_REDDOT::PET_EQUIP_PURIRING);
        RefreshReddotManager::getInstance()->refreshUI(E_REFRESH_REDDOT::PET_EQUIP_HONG);
        RefreshReddotManager::getInstance()->refreshUI(E_REFRESH_REDDOT::PET_EQUIP_MOLE);
        RefreshReddotManager::getInstance()->refreshUI(E_REFRESH_REDDOT::PET_EQUIP_HANDSOME);
        RefreshReddotManager::getInstance()->refreshUI(E_REFRESH_REDDOT::PET_EQUIP_ALL);
        
        //
        RefreshManager::getInstance()->refreshUI(E_REFRESH::PET_EQUIP);
    }
    else
    {
        if ( _callbackEquipLevelUP != nullptr )
        {
            _callbackEquipLevelUP(false);
            _callbackEquipLevelUP = nullptr;
        }
    }
}
