//
//  CostumeSkinManager.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 19/09/2019.
//

#include "CostumeSkinManager.h"

#include "Common/ConfigKey.h"

#include "ManagerGame/AccountManager.h"
#include "ManagerGame/ItemsManager.h"

USING_NS_CC;

CostumeSkinManager::CostumeSkinManager(void) :
_skinEquip(0),
_skinEquipDummy(0),
 
// callback
_callbackSkinIsBuy(nullptr),
_callbackSkinBuy(nullptr)
{
    
}

CostumeSkinManager::~CostumeSkinManager(void)
{
    
}

bool CostumeSkinManager::init()
{
    // load
    setLoadSkinList();
    setLoadSkinShop();
    
    return true;
}

#pragma mark - data
void CostumeSkinManager::saveData()
{
    std::stringstream str;
    
    //
    str.str("");
    for ( int i = 0; i < _listSkinExist.size(); i++ )
    {
        str << _listSkinExist[i] << ",";
    }
    UserDefault::getInstance()->setStringForKey(KEY_COSTUME_SKIN_BUY, MafAes256::Encrypt(str.str()));
    
    //
    str.str("");
    str << MafUtils::toString(_skinEquip.value());
    UserDefault::getInstance()->setStringForKey(KEY_COSTUME_SKIN, MafAes256::Encrypt(str.str()));
}

void CostumeSkinManager::loadData()
{
    std::string str = "";
    
    //
    str = UserDefault::getInstance()->getStringForKey(KEY_COSTUME_SKIN_BUY);
    if ( !str.empty() )
    {
        str = MafAes256::Decrypt(str);
        
        auto list = UtilsString::getInstance()->split(str, ',');
        for ( auto skin : list )
        {
            if ( MafUtils::isNumeric(skin) == false )
            {
                continue;
            }
            setCostumeSkinBuy(std::atoi(skin.c_str()));
        }
    }
    
    //
    str.clear();
    str = UserDefault::getInstance()->getStringForKey(KEY_COSTUME_SKIN);
    if ( !str.empty() )
    {
        setCostumeSkinEquip(std::atoi(MafAes256::Decrypt(str).c_str()));
    }
    else
    {
        setCostumeSkinEquip(0);
    }
}

#pragma mark - load
void CostumeSkinManager::setLoadSkinList()
{
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::COSTUME_SKIN_LIST);
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)strText.c_str());
    if( jsonParser.HasParseError() )
    {
        return;
    }
    
    _listCostumeSkin.clear();
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];
        if ( MafNative::IsLessThanCurrVersion(jsonValue["version"].GetString()) == false )
        {
            continue;
        }
        
        int idx = jsonValue["idx"].GetInt();
        int sort = jsonValue["sort"].GetInt();
        
        std::string nameKey = jsonValue["name_key"].GetString();
        
        std::string buffType = jsonValue["buff_type"].GetString();
        double buffStrength = jsonValue["buff_strength"].GetDouble();
        
        
        // obj
        auto obj = InfoCostumeSkin::create();
        obj->setIdx(idx);
        obj->setSort(sort);
        
        obj->setNameKey(nameKey);
        
        obj->setBuffType(buffType);
        obj->setBuffStrength(buffStrength);
        
        _listCostumeSkin.pushBack(obj);
    }
}
 
void CostumeSkinManager::setLoadSkinShop()
{
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::COSTUME_SKIN_SHOP);
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)strText.c_str());
    if( jsonParser.HasParseError() )
    {
        return;
    }
    
    _listCostumeSkinShop.clear();
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];
        if ( MafNative::IsLessThanCurrVersion(jsonValue["version"].GetString()) == false )
        {
            continue;
        }
        
        int idx = jsonValue["idx"].GetInt();
        int sort = jsonValue["sort"].GetInt();
        
        std::string commentKey = jsonValue["skin_comment"].GetString();
        int skinIdx = jsonValue["skin_list_idx"].GetInt();
        int label = jsonValue["label"].GetInt();
        int rare = jsonValue["rare"].GetInt();
        
        
        std::string buyType = jsonValue["buy_event"].GetString();
        int buyCashIdx = jsonValue["buy_event_params"].GetInt();
        int buyItemIdx = jsonValue["buy_item_idx"].GetInt();
        int buyItemParams = jsonValue["buy_item_type_params"].GetInt();
        int buyItemCount = jsonValue["buy_item_count"].GetInt();
        
        int64_t buySaleTimeStart = jsonValue["date_start"].GetInt64();
        int64_t buySaleTimeEnd = jsonValue["date_end"].GetInt64();
        
        // obj
        auto obj = InfoCostumeSkinShop::create();
        obj->setIdx(idx);
        obj->setSort(sort);
        
        obj->setCommentKey(commentKey);
        obj->setSkinIdx(skinIdx);
        obj->setLabel(label);
        obj->setRare(rare);

        obj->setBuyType(buyType);
        obj->setBuyCashIdx(buyCashIdx);
        obj->setBuyItemIdx(buyItemIdx);
        obj->setBuyItemParams(buyItemParams);
        obj->setBuyItemCount(buyItemCount);
        obj->setBuySaleTimeStart(buySaleTimeStart);
        obj->setBuySaleTimeEnd(buySaleTimeEnd);
        
        _listCostumeSkinShop.pushBack(obj);
    }
    
    // sort
    std::sort(_listCostumeSkinShop.begin(), _listCostumeSkinShop.end(), [](InfoCostumeSkinShop *a, InfoCostumeSkinShop *b) -> bool{
        return a->getSort() < b->getSort();
    });
}

#pragma mark - set, get : list
Vector<InfoCostumeSkin*> CostumeSkinManager::getListCostumeSkin()
{
    return _listCostumeSkin;
}

Vector<InfoCostumeSkinShop*> CostumeSkinManager::getListCostumeSkinShop()
{
    return _listCostumeSkinShop;
}

InfoCostumeSkin* CostumeSkinManager::getInfoSkin(int nIdx)
{
    InfoCostumeSkin *pResult = nullptr;
    
    for ( int i = 0; i < _listCostumeSkin.size(); i++ )
    {
        auto obj = _listCostumeSkin.at(i);
        
        if ( obj->getIdx() == nIdx )
        {
            pResult = obj;
            break;
        }
    }
    
    return pResult;
}

InfoCostumeSkinShop* CostumeSkinManager::getInfoSkinShop(int nIdx)
{
    InfoCostumeSkinShop *pResult = nullptr;
    
    for ( int i = 0; i < _listCostumeSkinShop.size(); i++ )
    {
        auto obj = _listCostumeSkinShop.at(i);
        
        if ( obj->getIdx() == nIdx )
        {
            pResult = obj;
            break;
        }
    }
    
    return pResult;
}

#pragma mark - set, get : other
int CostumeSkinManager::getCostumeSkinEquip()
{
    return _skinEquip.valueInt();
}

void CostumeSkinManager::setCostumeSkinEquip(int nSkin)
{
    _skinEquip = nSkin;
}

int CostumeSkinManager::getCostumeSkinEquipDummy()
{
    return _skinEquipDummy.valueInt();
}

void CostumeSkinManager::setCostumeSkinEquipDummy(int nSkin)
{
    _skinEquipDummy = nSkin;
}

bool CostumeSkinManager::isCostumeSkinBuy(int nSkin)
{
    bool bResult = false;
    
    if ( std::find(_listSkinExist.begin(), _listSkinExist.end(), nSkin) != _listSkinExist.end() )
    {
        bResult = true;
    }
    
    if ( nSkin == 0 )
    {
        bResult = true;
    }
    
    return bResult;
}

void CostumeSkinManager::setCostumeSkinBuy(int nSkin)
{
    if ( nSkin == 0 )
    {
        return;
    }
    
    if ( std::find(_listSkinExist.begin(), _listSkinExist.end(), nSkin) != _listSkinExist.end() )
    {
        return;
    }
    
    _listSkinExist.push_back(nSkin);
}

void CostumeSkinManager::addCostumeSkinBuy(int nSkin)
{
    if ( nSkin == 0 )
    {
        return;
    }
    
    if ( std::find(_listSkinExist.begin(), _listSkinExist.end(), nSkin) != _listSkinExist.end() )
    {
        return;
    }
    
    _listSkinExist.push_back(nSkin);
    
    //
    saveData();
    
    //
    RefreshManager::getInstance()->refreshUI(E_REFRESH::COSTUME_BUY);
}

#pragma mark - effect
double CostumeSkinManager::getEffectOwned()
{
    double effect = 1;
    for ( auto& objSkin : getListCostumeSkin() )
    {
        if ( isCostumeSkinBuy(objSkin->getIdx()) == true )
        {
            effect += effect * 1.5f;
        }
    }
    
    effect -= 1;
    return effect;
}

double CostumeSkinManager::getEffectBuff()
{
    double effect = 1;
    for ( auto& objSkin : getListCostumeSkin() )
    {
        if ( objSkin->getBuffType().compare("ALL_DAMAGE") == 0 && isCostumeSkinBuy(objSkin->getIdx()) == true )
        {
            effect += effect * objSkin->getBuffStrength();
        }
    }
    
    effect -= 1;
    return effect;
}

#pragma mark - event
int CostumeSkinManager::onSkinEquip(int skin)
{
    /*
     0 : 성공
     1 : 스킨 없음
     */
    
    if ( skin != 0 )
    {
        bool bBuy =  isCostumeSkinBuy(skin);
        if ( bBuy == false )
        {
            return 1;
        }
    }
    
    setCostumeSkinEquip(skin);
    saveData();
    
    //
    RefreshManager::getInstance()->refreshUI(E_REFRESH::COSTUME);
    
    return 0;
}

#pragma mark - network
void CostumeSkinManager::requestSkinIsBuy(const std::function<void(bool)>& callback)
{
    //
    _callbackSkinIsBuy = callback;
    
    //
    std::string url = "/v1/premiumSkin/info/list/%d";
    url = MafUtils::format(url.c_str(), AccountManager::getInstance()->getUserIdx());
    
    auto req = new MafHttpNew(network::HttpRequest::Type::GET, url.c_str());
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseSkinIsBuy(response,dd);
    };
    req->send(cb);
}

void CostumeSkinManager::responseSkinIsBuy(network::HttpResponse* response,std::string &data)
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
        // callback
        if ( _callbackSkinIsBuy != nullptr )
        {
            _callbackSkinIsBuy(false);
            _callbackSkinIsBuy = nullptr;
        }
        return;
    }
    
    // 정보를 다시 가져와서 모두 삭제
    _listSkinExist.clear();
    
    //
    if ( jsonParser.HasMember("_items") == true )
    {
        std::string strItems = jsonParser["_items"].GetString();
        ItemsManager::getInstance()->setItemsServer(strItems);
        
        //
        int nCostumeSkin = getCostumeSkinEquip();
        if ( isCostumeSkinBuy(nCostumeSkin) == false )
        {
            setCostumeSkinEquip(0);
            RefreshManager::getInstance()->refreshUI(E_REFRESH::COSTUME);
        }
    }
    
    //
    saveData();
    
    //
    if ( _callbackSkinIsBuy != nullptr )
    {
        _callbackSkinIsBuy(true);
        _callbackSkinIsBuy = nullptr;
    }
}

void CostumeSkinManager::requestSkinBuy(int nShopIdx, const std::function<void(bool, int, int)>& callback)
{
    //
    _callbackSkinBuy = callback;
    
    //
    auto objSkinShop = getInfoSkinShop(nShopIdx);
    if ( objSkinShop == nullptr )
    {
        if ( _callbackSkinBuy != nullptr )
        {
            _callbackSkinBuy(false, -1, 0);
            _callbackSkinBuy = nullptr;
        }
        
        return;
    }
    
    //
    std::string url = "/v2/premiumSkin/buy";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_shopidx", nShopIdx);
    
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseSkinBuy(response,dd);
    };
    req->send(cb);
}

void CostumeSkinManager::responseSkinBuy(network::HttpResponse* response,std::string &data)
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
        // callback
        if ( _callbackSkinBuy != nullptr )
        {
            _callbackSkinBuy(false, -1, 0);
            _callbackSkinBuy = nullptr;
        }
        return;
    }
     
    //# 5 : 없는 상품, 4 : 이미 구매한 스킨, 0 : 재화 부족, 1 : 성공
    int nResult = jsonParser["_result"].GetInt();
    if ( nResult == 1 || nResult == 4 )
    {
        std::string strReward         = jsonParser["_reward"].GetString();
        std::string strItems         = jsonParser["_items"].GetString();
        ItemsManager::getInstance()->addItems(strReward, false);
        ItemsManager::getInstance()->setItemsServer(strItems);
        
        int nSkinIdx = 0;
        
        rapidjson::Document jsonParserReward;
        jsonParserReward.Parse<0>((char*)strReward.c_str());
        if ( jsonParserReward.HasParseError() == false )
        {
            for ( int i = 0; i < jsonParserReward.Size(); i++ )
            {
                const rapidjson::Value& jsonValue = jsonParserReward[i];
                
                int nItemIdx = jsonValue["idx"].GetInt();
                int nItemParams = 0;
                if ( jsonValue.HasMember("type_params") == true )
                {
                    nItemParams = jsonValue["type_params"].GetInt();
                }
                
                if ( nItemIdx == 11000 && nItemParams != 0  )
                {
                    nSkinIdx = nItemParams;
                    setCostumeSkinEquip(nSkinIdx);
                }
            }
        }
        
        //
        RefreshManager::getInstance()->refreshUI(E_REFRESH::COSTUME);
        
        // callback
        if ( nSkinIdx != 0 )
        {
            if ( _callbackSkinBuy != nullptr )
            {
                _callbackSkinBuy(true, nResult, nSkinIdx);
                _callbackSkinBuy = nullptr;
            }
        }
        else
        {
            if ( _callbackSkinBuy != nullptr )
            {
                _callbackSkinBuy(false, nResult, 0);
                _callbackSkinBuy = nullptr;
            }
        }
    }
    else
    {
        // callback
        if ( _callbackSkinBuy != nullptr )
        {
            _callbackSkinBuy(false, nResult, 0);
            _callbackSkinBuy = nullptr;
        }
    }
    
}
