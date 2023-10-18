//
//  DefenseManaAMPManager.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/08/21.
//

#include "DefenseManaAMPManager.h"

#include "Common/ConfigKey.h"

#include "GameObject/InfoSetting.h"
#include "GameObject/Defense/InfoDefenseManaAmpStone.h"
#include "GameObject/Defense/InfoDefenseManaAMP.h"

#include "ManagerGame/DefenseManager.h"
#include "ManagerGame/AccountManager.h"
#include "ManagerGame/ItemsManager.h"
#include "ManagerGame/SaveManager.h"

#include "GameUIPopup/Common/PopupLoading.h"
#include "GameUIPopup/Common/PopupDefault.h"

USING_NS_CC;

DefenseManaAMPManager::DefenseManaAMPManager(void):
_callbackInfo(nullptr)
{
    _listInfoManaAmp.clear();
    _listInfoManaAmpStome.clear();
    _listAMPOpenFloor.clear();
    
    _onServerInfo.Clear();
    _onServerUpdateAMP.Clear();
}

DefenseManaAMPManager::~DefenseManaAMPManager(void)
{
    _listInfoManaAmp.clear();
    _listInfoManaAmpStome.clear();
    _listAMPOpenFloor.clear();
    
    _onServerInfo.Clear();
    _onServerUpdateAMP.Clear();
}

bool DefenseManaAMPManager::init()
{
    loadData();

    return true;
}

#pragma mark -Load Table Data
void DefenseManaAMPManager::loadData()
{
    setLoadManaAMPStoneData();
    setLoadManaAMPStoneCost();
    setLoadManaAMPOpenFloor();
}
void DefenseManaAMPManager::setLoadManaAMPStoneData()
{
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::DEFENSE_MANA_AMP_STONE);

    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)strText.c_str());
    if( jsonParser.HasParseError() == true )
    {
        return;
    }

    _listInfoManaAmpStome.clear();
    std::string strCurType = "";
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];

        if ( MafNative::IsLessThanCurrVersion(jsonValue["version"].GetString()) == false )
        {
            continue;
        }

        const int idx = jsonValue["idx"].GetInt();
        const int bonus_success_rate = jsonValue["bonus_success_rate"].GetInt();
        const int success_increase_rate = jsonValue["success_increase_rate"].GetInt();
        const int success_decrease_rate = jsonValue["success_decrease_rate"].GetInt();
        const int fail_replace_idx = jsonValue["fail_replace_idx"].GetInt();
        const float output_quantity = jsonValue["output_quantity"].GetFloat();


        InfoDefenseManaAmpStone* obj = InfoDefenseManaAmpStone::create();
        obj->setIndex(idx);
        obj->setBonusSuccessRate(bonus_success_rate);
        obj->setSuccessIncreaseRate(success_increase_rate);
        obj->setSuccessDecreaseRate(success_decrease_rate);
        obj->setFailReplaceIndex(fail_replace_idx);
        obj->setOutputQuantity(output_quantity);

        _listInfoManaAmpStome.pushBack(obj);
    }
}

void DefenseManaAMPManager::setLoadManaAMPStoneCost()
{
    auto objSetting = DefenseManager::getInstance()->getInfoSetting("defense_manaamp_cost");
    
    if(objSetting == nullptr)
        return;
    
    std::string strText = objSetting->getValueToString();
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)strText.c_str());
    if( jsonParser.HasParseError() == true )
    {
        return;
    }
    for ( int i = 0; i < jsonParser["cost"].Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser["cost"][i];
        
        if(auto ampData = getManaAmpData(i+1))
        {
            ampData->setEquipCOst(jsonValue.GetInt());
        }
    }
}

void DefenseManaAMPManager::setLoadManaAMPOpenFloor()
{
    auto objSetting = DefenseManager::getInstance()->getInfoSetting("defense_manaamp_setting");
    
    if(objSetting == nullptr)
        return;
    
    std::string strText = objSetting->getValueToString();
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)strText.c_str());
    if( jsonParser.HasParseError() == true )
    {
        return;
    }
    
    _listAMPOpenFloor.clear();
    for ( int i = 0; i < jsonParser["floor"].Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser["floor"][i];
        
        _listAMPOpenFloor.push_back(jsonValue.GetInt());
        if(auto ampData = getManaAmpData(i+1))
        {
            ampData->setOpenFloor(jsonValue.GetInt());
        }
    }
}

#pragma mark -Getter

Vector<InfoDefenseManaAMP*> DefenseManaAMPManager::getManaAmpList() const
{
    return _listInfoManaAmp;
}
Vector<InfoDefenseManaAmpStone*> DefenseManaAMPManager::getManaAmpStoneList() const
{
    return _listInfoManaAmpStome;
}
int DefenseManaAMPManager::getMaxManaAMPCount()
{
    return _listInfoManaAmp.size();
}
std::vector<int> DefenseManaAMPManager::getAMPOpenFloorList() const
{
    return _listAMPOpenFloor;
}
InfoDefenseManaAMP* DefenseManaAMPManager::getManaAmpData(int idx) const
{
    InfoDefenseManaAMP* result = nullptr;
    
    for(auto data : _listInfoManaAmp)
    {
        if(data->getIndex() == idx)
        {
            result = data;
            break;
        }
    }
    
    return result;
}
InfoDefenseManaAmpStone* DefenseManaAMPManager::getManaAmpStoneData(int idx) const
{
    InfoDefenseManaAmpStone* result = nullptr;
    
    for(auto data : _listInfoManaAmpStome)
    {
        if(data->getIndex() == idx)
        {
            result = data;
            break;
        }
    }
    
    return result;
}
bool DefenseManaAMPManager::isReddotCondition() const
{
    for(auto data : _listInfoManaAmp)
    {
        if(data->isReddotCondition())
            return true;
    }
    return false;
}
#pragma mark -Utils
void DefenseManaAMPManager::updateManaAMP(const rapidjson::Document& jsonParser)
{
    _listInfoManaAmp.clear();
    for ( int i = 0; i < jsonParser["_mana_amp_list"].Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser["_mana_amp_list"][i];
        
        int idx = jsonValue["index"].GetInt();
        int successRate = jsonValue["success_rate"].GetInt();
        
        std::vector<int> stoneList;
        for(int j = 0; j < jsonValue["amp_list"].Size(); ++j)
        {
            stoneList.push_back(jsonValue["amp_list"][j].GetInt());
        }
        
        auto data = InfoDefenseManaAMP::create();
        data->setIndex(idx);
        data->setCurrentSuccessRate(successRate);
        data->setStoneList(stoneList);
        _listInfoManaAmp.pushBack(data);
    }
    
    setLoadManaAMPStoneCost();
    setLoadManaAMPOpenFloor();
}

std::string DefenseManaAMPManager::calculatorAMPPower(std::string value, bool isOrginal)
{
    bool isOpen = false;
    if(auto setting = DefenseManager::getInstance()->getInfoSetting("defense_manaamp_switch"))
        isOpen = setting->getValueToBool();
    
    if(!DefenseManager::getInstance()->isFirstGift())
        return value;
    
    if(isOpen)
    {
        for(auto info : _listInfoManaAmp)
        {
            if(!info->isComplete())
                continue;
            
            auto tt = MafUtils::bigMulNum(value, MafUtils::doubleToString(info->getManaAMPPower()));
            
            if(tt.length() > 0)
                value = MafUtils::bigAddNum(value, tt);
        }
    }

    
    return value;
}

#pragma mark - network : info
void DefenseManaAMPManager::requestInfo(bool isShowLoading, const std::function<void(bool, int)>& callback, bool isIntro)
{
    if ( isShowLoading == true )
    {
        PopupLoading::show();
    }

    _callbackInfo = callback;
    //
    std::string url = "/defense/manaamp/info/%d";
    url = MafUtils::format(url.c_str(), AccountManager::getInstance()->getUserIdx());

    auto req = new MafHttpNew(network::HttpRequest::Type::GET, url.c_str());
    MafResponseNew cb = [=](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseInfo(isShowLoading, isIntro, response, dd);
    };
    req->send(cb);
}

void DefenseManaAMPManager::responseInfo(bool isShowLoading, bool isIntro, cocos2d::network::HttpResponse* response,std::string &data)
{
    if ( isShowLoading == true )
    {
        PopupLoading::hide();
    }

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
        _onServerInfo.Invoke(false, -1);
        if ( _callbackInfo != nullptr )
        {
            _callbackInfo(false, -1);
            _callbackInfo = nullptr;
        }
        return;
    }

    int result = jsonParser["_result"].GetInt();
    if ( result != 1 )
    {
        //
        _onServerInfo.Invoke(false, result);
        if ( _callbackInfo != nullptr )
        {
            _callbackInfo(false, result);
            _callbackInfo = nullptr;
        }
        
        if(result == 9 && !isIntro)
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_52"));
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
            
            if(auto setting = DefenseManager::getInstance()->getInfoSetting("defense_manaamp_switch"))
            {
                setting->setValue("false");
            }
        }
        return;
    }
    if(auto setting = DefenseManager::getInstance()->getInfoSetting("defense_manaamp_switch"))
    {
        setting->setValue("true");
    }

    updateManaAMP(jsonParser);
    //
    _onServerInfo.Invoke(true, result);
    if ( _callbackInfo != nullptr )
    {
        _callbackInfo(true, result);
        _callbackInfo = nullptr;
    }
}

void DefenseManaAMPManager::requestChoiceStone(int idx, E_DF_AMP_STONE_Type type, bool isShowLoading)
{
    if ( isShowLoading == true )
    {
        PopupLoading::show();
    }
    //
    std::string url = "/defense/manaamp/choice";

    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_index", idx);
    req->setData("_type", (int)type);
    MafResponseNew cb = [=](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseChoiceStone(idx, type, isShowLoading, response,dd);
    };
    req->send(cb);
}

void DefenseManaAMPManager::responseChoiceStone(int idx, E_DF_AMP_STONE_Type type, bool isShowLoading, cocos2d::network::HttpResponse* response,std::string &data)
{
    if ( isShowLoading == true )
    {
        PopupLoading::hide();
    }

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
        _onServerEquipStone.Invoke(false, -1, false, type);
        return;
    }

    int result = jsonParser["_result"].GetInt();
    if ( result != 1 )
    {
        //
        _onServerEquipStone.Invoke(false, result, false, type);
        
        if(result == 9)
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_52"));
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
            
            if(auto setting = DefenseManager::getInstance()->getInfoSetting("defense_manaamp_switch"))
            {
                setting->setValue("false");
            }
        }
        return;
    }
    if(auto setting = DefenseManager::getInstance()->getInfoSetting("defense_manaamp_switch"))
    {
        setting->setValue("true");
    }
    
    if(auto ampData = getManaAmpData(idx))
    {
        DefenseManager::getInstance()->setDefenseCoin(ampData->getEquipCost() * -1);
    }
    
    auto selectInfo = jsonParser["_select_info"].GetObject();
    int isSuccess = selectInfo["_is_success"].GetInt();
    updateManaAMP(jsonParser);
    

    _onServerEquipStone.Invoke(true, result, isSuccess, type);
}

void DefenseManaAMPManager::requestResetAMP(int idx, bool isShowLoading)
{
    if ( isShowLoading == true )
    {
        PopupLoading::show();
    }
    //
    std::string url = "/defense/manaamp/retry";

    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_index", idx);
    MafResponseNew cb = [=](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseResetAMP(isShowLoading, response,dd);
    };
    req->send(cb);
}

void DefenseManaAMPManager::responseResetAMP(bool isShowLoading, cocos2d::network::HttpResponse* response,std::string &data)
{
    if ( isShowLoading == true )
    {
        PopupLoading::hide();
    }

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
        _onServerUpdateAMP.Invoke(false, -1);
        return;
    }

    int result = jsonParser["_result"].GetInt();
    if ( result != 1 )
    {
        //
        _onServerUpdateAMP.Invoke(false, result);
        
        if(result == 9)
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_52"));
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
            
            if(auto setting = DefenseManager::getInstance()->getInfoSetting("defense_manaamp_switch"))
            {
                setting->setValue("false");
            }
        }
        return;
    }
    if(auto setting = DefenseManager::getInstance()->getInfoSetting("defense_manaamp_switch"))
    {
        setting->setValue("true");
    }
    
    std::string strItems = jsonParser["_items"].GetString();
    ItemsManager::getInstance()->setItemsServer(strItems);
    
    updateManaAMP(jsonParser);
    //
    _onServerUpdateAMP.Invoke(true, result);
}
