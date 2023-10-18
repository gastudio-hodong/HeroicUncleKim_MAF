//test
//  CashManager.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#include "CashManager.h"

#include "ManagerGame/UserInfoManager.h"

#include "ManagerGame/InfiniteArtifactManager.h"
#include "ManagerGame/ItemsManager.h"
#include "ManagerGame/DataManager.h"
#include "ManagerGame/SaveManager.h"
#include "ManagerGame/AccountManager.h"

USING_NS_CC;

CashManager::CashManager(void) :
_isNewIconNormal(false),
_isNewIconDevil(false),

_objProduct(nullptr),

_strPurchaseSignature(""),
_strPurchaseData(""),

_nBannerEventIdx(0),
_nBannerEventGrade(0),
_nBannerEventTime(0),

_nFirstBonusResult(-1),
_nFirstBonusVersion(-1),

//
_adsFreeEndtime(0),

//
_downloadTotal(0),
_downloadSuccess(0),
_downloadError(0),
_downloadStoragePathFull(""),
_downloadStoragePath(""),

//
_callbackPurchase(nullptr),
_callbackInfo(nullptr),
_callbackPurchaseRemainingAll(nullptr),
_callbackPurchaseRemaining(nullptr),
_callbackDailyInfo(nullptr),
_callbackBannerInfo(nullptr),
_callbackGrowthInfo(nullptr),
_callbackFirstBonusInfo(nullptr),
_callbackBannerBonusReceive(nullptr),
_callbackGrowthReceive(nullptr),
_callbackDailyReceive(nullptr),
_callbackFirstBonusReceive(nullptr),
_callbackNewsInfo(nullptr),

//
_callbackDownloadStart(nullptr),
_callbackDownloadProgress(nullptr),
_callbackDownloadCount(nullptr),
_callbackDownloadEnd(nullptr)
{
    
}

CashManager::~CashManager(void)
{
    _delegateInfo.Clear();
    _delegateDailyInfo.Clear();
    _delegatePurchaseRemainingAll.Clear();
}

bool CashManager::init()
{
    _downloader.reset(new network::Downloader());
    _downloader->onTaskProgress = std::bind(&CashManager::downloadTaskProgress, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
    _downloader->onFileTaskSuccess = std::bind(&CashManager::downloadFileSuccess, this, std::placeholders::_1);
    _downloader->onTaskError = std::bind(&CashManager::downloadTaskError, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
    
    // load
    setLoad();
    setLoadGrowth();
    
    // 이전 결제 내역 복구
    MafGooglePlay::PurchaseRestore();
    
    return true;
}

#pragma mark - set, get, function
void CashManager::setLoad()
{
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::PURCHASE);
    
    rapidjson::Document jsonParser;
    jsonParser.Parse<0>(strText.c_str());
    if( jsonParser.HasParseError() )
    {
        return;
    }
    
    _listProduct.clear();
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonProduct = jsonParser[i];
        
        int nIdx = jsonProduct["idx"].GetInt();
        
        int nOrder = 0;
        
        if(jsonProduct.HasMember("order"))
            nOrder = jsonProduct["order"].GetInt();
        std::string strNameKey =  jsonProduct["name_key"].GetString();
        std::string strIconName = jsonProduct["icon_name"].GetString();
        
        int nShopGroup = jsonProduct["shop_group"].GetInt();
        std::string strShopType = jsonProduct["shop_type"].GetString();
        
        int nSaleType = jsonProduct["sale_type"].GetInt();
        int64_t nSaleTimeStart = jsonProduct["sale_start_time"].GetInt64();
        int64_t nSaleTimeEnd = jsonProduct["sale_end_time"].GetInt64();
        int nSaleLimit = jsonProduct["sale_limit"].GetInt();
        std::string strSaleRatio = jsonProduct["sale_ratio"].GetString();
        strSaleRatio = UtilsString::getInstance()->replaceAll(strSaleRatio.c_str(), "\\n", "\n");
        
        
            
        //
        std::string strItems = jsonProduct["items"].GetString();
        std::string strItemsOther = jsonProduct["items_other"].GetString();
        
        //
        std::string strProductIdReal = "";
        std::string strProductIdWatch = jsonProduct[("product_id_watch")].GetString();
        if ( CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
        {
            strProductIdReal = jsonProduct[("product_id_ios")].GetString();
        }
        else
        {
            strProductIdReal = jsonProduct[("product_id_aos")].GetString();
        }
        
        std::string strVersion = jsonProduct["version"].GetString();
        
        // obj
        auto obj = InfoProduct::create();
        obj->setIdx(nIdx);
        obj->setOrder(nOrder);
        obj->setName(strNameKey);
        obj->setIconName(strIconName);
        obj->setShopGroup(nShopGroup);
        obj->setShopType(strShopType);
        
        obj->setSaleType(nSaleType);
        obj->setSaleTimeStart(nSaleTimeStart);
        obj->setSaleTimeEnd(nSaleTimeEnd);
        obj->setSaleLimit(nSaleLimit);
        obj->setSaleRatio(strSaleRatio);

        obj->setVersion(strVersion);
        
        obj->setItems(strItems);
        obj->setItemsOther(strItemsOther);
        
        obj->setProductIdReal(strProductIdReal);
        obj->setProductIdWatch(strProductIdWatch);
        
        _listProduct.pushBack(obj);
    }
    
    //
    setProductPriceLoad();
}

void CashManager::setLoadGrowth()
{
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::GROWTH_PACK_REWARD);
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)strText.c_str());
    if( jsonParser.HasParseError() )
    {
        return;
    }
    
    _listGrowth.clear();
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];
        
        std::string type = jsonValue["type"].GetString();
        
        int level = jsonValue["lv"].GetInt();
        int idx = jsonValue["idx"].GetInt();
        int idxCash = jsonValue["cash_idx"].GetInt();

        int condition = jsonValue["condition"].GetInt();
        
        std::string itemsFree = jsonValue["free_reward"].GetString();
        std::string itemsCash = jsonValue["cash_reward"].GetString();
        
        auto objProductBonus = InfoProductGrowth::create();
        objProductBonus->setType(type);
        objProductBonus->setLevel(level);
        objProductBonus->setIdx(idx);
        objProductBonus->setIdxCash(idxCash);
        objProductBonus->setCondition(condition);
        objProductBonus->setItemsFree(itemsFree);
        objProductBonus->setItemsCash(itemsCash);
        
        _listGrowth.pushBack(objProductBonus);
    }
}

void CashManager::setProductPriceLoad()
{
    std::vector<std::string> listProductId;
    
    for ( int i = 0; i < _listProduct.size(); i++ )
    {
        auto obj = _listProduct.at(i);
        if ( obj->getPrice().length() == 0 )
        {
            std::string strProductIdReal = obj->getProductIdReal();
            if ( strProductIdReal.length() == 0 )
            {
                continue;
            }
            
            // product id add : price가져오기 위해.
            auto iter = std::find(listProductId.begin(), listProductId.end(), strProductIdReal);
            if ( iter == listProductId.end() )
            {
                listProductId.push_back(strProductIdReal);
            }
        }
    }
    
    if ( listProductId.size() != 0 )
    {
        MafGooglePlay::PurchaseLoad(listProductId);
    }
}

void CashManager::setProductPrice(std::string strProductId, std::string strPrice, std::string strPriceCurrencyCode)
{
    for ( auto &obj : _listProduct )
    {
        if ( obj->getReferenceCount() == 0 )
        {
            continue;
        }
            
        if ( obj->getProductIdReal().compare(strProductId) == 0 )
        {
            obj->setPrice(strPrice);
            obj->setPriceCurrencyCode(strPriceCurrencyCode);
        }
    }
}

#pragma mark -
Vector<InfoProduct*> CashManager::getProductGroup(int group)
{
    Vector<InfoProduct*> listResult;
    
    int64_t nTimeNow = UtilsDate::getInstance()->getTime();
    for ( auto &obj : _listProduct )
    {
        if ( obj->getShopGroup() != group || MafNative::IsLessThanCurrVersion(obj->getVersion()) == false )
        {
            continue;
        }
        
        if ( obj->getSaleType() >= 1 )
        {
            if ( obj->getSaleTimeStart() != 0 && obj->getSaleTimeStart() > nTimeNow )
            {
                continue;
            }
            
            if ( obj->getSaleTimeEnd() != 0 && obj->getSaleTimeEnd() < nTimeNow )
            {
                continue;
            }
        }
        
        
        listResult.pushBack(obj);
        
    }
    
    return listResult;
}

Vector<InfoProduct*> CashManager::getProductType(E_PRODUCT eType)
{
    Vector<InfoProduct*> listResult;
    
    int64_t nTimeNow = UtilsDate::getInstance()->getTime();
    for ( auto &obj : _listProduct )
    {
        if ( obj->getShopType() != eType || MafNative::IsLessThanCurrVersion(obj->getVersion()) == false )
        {
            continue;
        }
        
        if ( obj->getSaleType() >= 1 )
        {
            if ( obj->getSaleTimeStart() != 0 && obj->getSaleTimeStart() > nTimeNow )
            {
                continue;
            }
            
            if ( obj->getSaleTimeEnd() != 0 && obj->getSaleTimeEnd() < nTimeNow )
            {
                continue;
            }
        }
        
        
        listResult.pushBack(obj);
    }
    
    return listResult;
}

InfoProduct* CashManager::getProduct(int nIdx)
{
    InfoProduct *pResult = nullptr;
    
    for ( int i = 0; i < _listProduct.size(); i++ )
    {
        auto obj = _listProduct.at(i);
        
        if ( obj->getIdx() == nIdx )
        {
            pResult = obj;
            break;
        }
    }
    
    return pResult;
}

Vector<InfoItems*> CashManager::getItemsConvert(std::string strItems)
{
    Vector<InfoItems*> listItems;
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)strItems.c_str());
    if( jsonParser.HasParseError() )
    {
        return listItems;
    }
    
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonItems = jsonParser[i];
        
        auto objItem = ItemsManager::getInstance()->getInfoItems(jsonItems["idx"].GetInt());
        if ( objItem == nullptr )
        {
            continue;
        }
        
        if ( jsonItems["count"].IsString() == true )      objItem->setCount(jsonItems["count"].GetString());
        else                                              objItem->setCount(jsonItems["count"].GetInt());
        
        if ( jsonItems.HasMember("type_params") == true )
        {
            objItem->setTypeParams(jsonItems["type_params"].GetInt());
        }
        
       
        
        listItems.pushBack(objItem);
    }
    
    return listItems;
}

#pragma mark - history
int CashManager::getHistoryTotal()
{
    return UserDefault::getInstance()->getIntegerForKey(KEY_PURCHASE_COUNT, 0);
}

void CashManager::setHistoryTotal(int nCount)
{
    UserDefault::getInstance()->setIntegerForKey(KEY_PURCHASE_COUNT, nCount);
}

void CashManager::addHistoryTotal()
{
    int countTotal = UserDefault::getInstance()->getIntegerForKey(KEY_PURCHASE_COUNT, 0);
    UserDefault::getInstance()->setIntegerForKey(KEY_PURCHASE_COUNT, countTotal + 1);
}

std::string CashManager::getHistoryProductCloud()
{
    std::string strResult = "";
    std::string strData = UserDefault::getInstance()->getStringForKey(KEY_PURCHASE_HISTORY, "");
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)strData.c_str());
    if( jsonParser.HasParseError() == false )
    {
        rapidjson::Value& jsonValue = jsonParser;
        for (auto itr = jsonValue.MemberBegin(); itr != jsonValue.MemberEnd(); )
        {
            std::string strKey = itr->name.GetString();
            
            auto obj = getProduct(atoi(strKey.c_str()));
            if ( obj == nullptr )
            {
                jsonValue.RemoveMember(itr);
            }
            else
            {
                ++itr;
            }
        }
        
        rapidjson::StringBuffer jsonBuffer;
        rapidjson::Writer<rapidjson::StringBuffer> jsonWriter(jsonBuffer);
        jsonParser.Accept(jsonWriter);
        
        strResult = jsonBuffer.GetString();
    }
    else
    {
        strResult = "";
    }
    
    
    UserDefault::getInstance()->setStringForKey(KEY_PURCHASE_HISTORY, strResult);
    
    return strResult;
}

void CashManager::setHistoryProductCloud(std::string strData)
{
    UserDefault::getInstance()->setStringForKey(KEY_PURCHASE_HISTORY, strData);
}

int CashManager::getHistoryProduct(int nIdx)
{
    int nResult = 0;
    std::string strIdx = MafUtils::toString(nIdx);
    std::string strData = UserDefault::getInstance()->getStringForKey(KEY_PURCHASE_HISTORY, "");
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)strData.c_str());
    if( jsonParser.HasParseError() == false )
    {
        if ( jsonParser.HasMember(strIdx.c_str()) == true )
        {
            nResult = jsonParser[strIdx.c_str()].GetInt();
        }
    }

    return nResult;
}

void CashManager::addHistoryProduct(int nIdx)
{
    std::string strIdx = MafUtils::toString(nIdx);
    std::string strData = UserDefault::getInstance()->getStringForKey(KEY_PURCHASE_HISTORY, "");
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)strData.c_str());
    if( jsonParser.HasParseError() == false )
    {
        if ( jsonParser.HasMember(strIdx.c_str()) == true )
        {
            jsonParser[strIdx.c_str()].SetInt(jsonParser[strIdx.c_str()].GetInt() + 1);
        }
        else
        {
            rapidjson::Value jsonKey(strIdx.c_str(), jsonParser.GetAllocator());
            rapidjson::Value jsonValue;
            jsonValue.SetInt(1);
            
            jsonParser.AddMember(jsonKey, jsonValue, jsonParser.GetAllocator());
        }
    }
    else
    {
        jsonParser.SetObject();
        
        rapidjson::Value jsonKey(strIdx.c_str(), jsonParser.GetAllocator());
        rapidjson::Value jsonValue;
        jsonValue.SetInt(1);
        
        jsonParser.AddMember(jsonKey, jsonValue, jsonParser.GetAllocator());
    }
    
    rapidjson::StringBuffer jsonBuffer;
    rapidjson::Writer<rapidjson::StringBuffer> jsonWriter(jsonBuffer);
    jsonParser.Accept(jsonWriter);
    
    strData = jsonBuffer.GetString();
    UserDefault::getInstance()->setStringForKey(KEY_PURCHASE_HISTORY, strData);
}

std::string CashManager::getHistoryAdsCloud()
{
    std::string strResult = "";
    std::string strData = UserDefault::getInstance()->getStringForKey(KEY_PURCHASE_ADS, "");
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)strData.c_str());
    if( jsonParser.HasParseError() == false )
    {
        rapidjson::Value& jsonValue = jsonParser;
        for (auto itr = jsonValue.MemberBegin(); itr != jsonValue.MemberEnd(); )
        {
            std::string strKey = itr->name.GetString();
            
            auto obj = getProduct(atoi(strKey.c_str()));
            if ( obj == nullptr )
            {
                jsonValue.RemoveMember(itr);
            }
            else
            {
                ++itr;
            }
        }
        
        rapidjson::StringBuffer jsonBuffer;
        rapidjson::Writer<rapidjson::StringBuffer> jsonWriter(jsonBuffer);
        jsonParser.Accept(jsonWriter);
        
        strResult = jsonBuffer.GetString();
    }
    else
    {
        strResult = "";
    }
    
    UserDefault::getInstance()->setStringForKey(KEY_PURCHASE_ADS, strResult);
    
    return strResult;
}

void CashManager::setHistoryAdsCloud(std::string strData)
{
    UserDefault::getInstance()->setStringForKey(KEY_PURCHASE_ADS, strData);
}

int CashManager::getHistoryAds(int nIdx)
{
    int nResult = 0;
    std::string strIdx = MafUtils::toString(nIdx);
    std::string strData = UserDefault::getInstance()->getStringForKey(KEY_PURCHASE_ADS, "");
    if ( strData.compare("null") == 0 )
    {
        strData = "";
    }
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)strData.c_str());
    if( jsonParser.HasParseError() == false )
    {
        if ( jsonParser.HasMember(strIdx.c_str()) == true )
        {
            nResult = jsonParser[strIdx.c_str()].GetInt();
        }
    }
    
    return nResult;
}

void CashManager::addHistoryAds(int nIdx)
{
    std::string strIdx = MafUtils::toString(nIdx);
    std::string strData = UserDefault::getInstance()->getStringForKey(KEY_PURCHASE_ADS, "");
    if ( strData.compare("null") == 0 )
    {
        strData = "";
    }
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)strData.c_str());
    if( jsonParser.HasParseError() == false )
    {
        if ( jsonParser.HasMember(strIdx.c_str()) == true )
        {
            jsonParser[strIdx.c_str()].SetInt(jsonParser[strIdx.c_str()].GetInt() + 1);
        }
        else
        {
            rapidjson::Value jsonKey(strIdx.c_str(), jsonParser.GetAllocator());
            rapidjson::Value jsonValue;
            jsonValue.SetInt(1);
            
            jsonParser.AddMember(jsonKey, jsonValue, jsonParser.GetAllocator());
        }
    }
    else
    {
        jsonParser.SetObject();
        
        rapidjson::Value jsonKey(strIdx.c_str(), jsonParser.GetAllocator());
        rapidjson::Value jsonValue;
        jsonValue.SetInt(1);
        
        jsonParser.AddMember(jsonKey, jsonValue, jsonParser.GetAllocator());
    }
    
    rapidjson::StringBuffer jsonBuffer;
    rapidjson::Writer<rapidjson::StringBuffer> jsonWriter(jsonBuffer);
    jsonParser.Accept(jsonWriter);
    
    strData = jsonBuffer.GetString();
    UserDefault::getInstance()->setStringForKey(KEY_PURCHASE_ADS, strData);
}

#pragma mark - info : daily(연금)
int CashManager::getDailyDate(int nIdx)
{
    int nResult = 0;
    
    auto iter = _listDailyDate.find(nIdx);
    if ( iter != _listDailyDate.end() )
    {
        nResult = iter->second;
    }
    
    return nResult;
}

void CashManager::setDailyDate(int nIdx, int nDate)
{
    if ( nDate > 0 )
    {
        _listDailyDate[nIdx] = nDate;
    }
}

bool CashManager::isDailyReceive(int nIdx)
{
    bool isResult = false;
    
    auto iter = _listDailyReceive.find(nIdx);
    if ( iter != _listDailyReceive.end() )
    {
        isResult = iter->second;
    }
    
    return isResult;
}

bool CashManager::isDailyReceiveAll()
{
    bool isResult = false;
    for( const auto& obj : _listDailyReceive )
    {
        if ( obj.second == true )
        {
            isResult = true;
            break;
        }
    }
    
    return isResult;
}

void CashManager::setDailyReceive(int nIdx, bool isReceive)
{
    _listDailyReceive[nIdx] = isReceive;
}

#pragma mark - info : adstime(광고 스킵)
void CashManager::setAdsfreeTime(int nTime)
{
    _adsFreeEndtime = nTime;
}

int CashManager::getAdsfreeTime()
{
    time_t endtime = (time_t)_adsFreeEndtime;
    time_t currenttime = UtilsDate::getInstance()->getTime();
    
    return difftime(endtime, currenttime);
}

bool CashManager::isAdsfreeTime()
{
    return getAdsfreeTime() > 0;
}

#pragma mark - info : banner
Vector<InfoProductBanner*> CashManager::getBannerAll()
{
    return _listBanner;
}

int CashManager::getBannerIdx()
{
    return _nBannerEventIdx;
}

int CashManager::getBannerTimeRemaining()
{
    int nTimeDiff = (int)(_nBannerEventTime - UtilsDate::getInstance()->getTime());
    return nTimeDiff;
}

int CashManager::getBannerGrade()
{
    return _nBannerEventGrade;
}

void CashManager::setBannerGrade(int nGrade)
{
    _nBannerEventGrade = nGrade;
}

#pragma mark - info : growth
cocos2d::Vector<InfoProductGrowth*> CashManager::getGrowthAll()
{
    return _listGrowth;
}
cocos2d::Vector<InfoProductGrowth*> CashManager::getGrowthGroup(std::string type)
{
    Vector<InfoProductGrowth*> listResult;
    
    for ( auto &obj : _listGrowth )
    {
        if ( obj->getType().compare(type) == 0 )
        {
            listResult.pushBack(obj);
        }
    }
    
    return listResult;
}

cocos2d::Vector<InfoProductGrowth*> CashManager::getGrowthGroup(std::string type, int level)
{
    Vector<InfoProductGrowth*> listResult;
    
    for ( auto &obj : _listGrowth )
    {
        if ( obj->getType().compare(type) == 0 && obj->getLevel() == level )
        {
            listResult.pushBack(obj);
        }
    }
    
    return listResult;
}

InfoProductGrowth* CashManager::getGrowth(int idx, int idxCash)
{
    InfoProductGrowth* objResult = nullptr;

    for ( auto &obj : _listGrowth )
    {
        if ( obj->getIdx() == idx && obj->getIdxCash() == idxCash )
        {
            objResult = obj;
            break;
        }
    }

    return objResult;
}

int CashManager::getGrowthNowLevel(std::string type)
{
    int level = 1;
    int floor = 0;

    if ( type.compare("FLOOR") == 0 )
    {
        floor = UserInfoManager::getInstance()->getHighFloor(E_PLACE::DG_NORMAL);
    }
    else if ( type.compare("PRISON_FLOOR") == 0 )
    {
        floor = SaveManager::getHistoryDataInt(E_HISTORY_DATA::K_PRISON_METER);
    }
    
    //
    auto list = getGrowthGroup(type);
    for ( auto& obj : list )
    {
        if ( obj->getCondition() <= floor )
        {
            level = obj->getLevel();
        }
        else
        {
            break;
        }
    }
    
    return level;
}

int CashManager::getGrowthMaxLevel(std::string type)
{
    int level = 1;
    
    auto list = getGrowthGroup(type);
    for ( auto& obj : list )
    {
        if ( level < obj->getLevel() )
        {
            level = obj->getLevel();
        }
    }
    
    return level;
}

int CashManager::getGrowthToIdxCash(std::string type, int level)
{
    int idxCash = 0;
    
    auto list = getGrowthGroup(type, level);
    if ( list.size() > 0 )
    {
        auto obj = list.at(0);
        idxCash = obj->getIdxCash();
    }
    
    return idxCash;
}

bool CashManager::isGrowthReward(std::string type)
{
    bool bResult = false;
    int floor = 0;

    if ( type.compare("FLOOR") == 0 )
    {
        floor = UserInfoManager::getInstance()->getHighFloor(E_PLACE::DG_NORMAL);
    }
    else if ( type.compare("PRISON_FLOOR") == 0 )
    {
        floor = SaveManager::getHistoryDataInt(E_HISTORY_DATA::K_PRISON_METER);
    }
    
    auto list = getGrowthGroup(type);
    for ( auto& obj : list )
    {
        if ( obj->getCondition() <= floor && obj->isRewardFree() == false )
        {
            bResult = true;
        }
        else
        {
            break;
        }
    }
    
    return bResult;
}

bool CashManager::isGrowthFinish(std::string type)
{
    bool bResult = true;
    
    auto list = getGrowthGroup(type);
    for ( auto& obj : list )
    {
        if ( obj->isRewardFree() == false || obj->isRewardCash() == false )
        {
            bResult = false;
        }
    }
    
    return bResult;
}

#pragma mark - info : growth
int CashManager::getFirstBonusResult()
{
    return _nFirstBonusResult;
}

#pragma mark - info : news
std::vector<std::string> CashManager::getNewsAll()
{
    return _listNews;
}
std::vector<std::string> CashManager::getNewsDownloadAll()
{
    return _listNewsDownload;
}

#pragma mark - 남은 결제 횟수
int CashManager::getPurchaseRemaining(int nItemIdx)
{
    int nResult = 0;
    
    auto iter = _listPurchaseRemaining.find(nItemIdx);
    if ( iter != _listPurchaseRemaining.end() )
    {
        nResult = iter->second;
    }
    
    return nResult;
}

void CashManager::setPurchaseRemaining(int nItemIdx, int nCount)
{
    _listPurchaseRemaining[nItemIdx] = nCount;
}

#pragma mark - reddot
bool CashManager::isNewIcon(E_PLACE type)
{
    bool isResult = false;
    if ( type == E_PLACE::DG_NORMAL )
    {
        isResult = _isNewIconNormal;
    }
    else if ( type == E_PLACE::DG_DEVIL_DOM )
    {
        isResult = _isNewIconDevil;
    }
    
    return isResult;
}

void CashManager::setNewIcon(bool bNew, E_PLACE type)
{
    if ( type == E_PLACE::EMPTY )
    {
        _isNewIconNormal = bNew;;
        _isNewIconDevil = bNew;
    }
    else if ( type == E_PLACE::DG_NORMAL )
    {
        _isNewIconNormal = bNew;
    }
    else if ( type == E_PLACE::DG_DEVIL_DOM )
    {
        _isNewIconDevil = bNew;
    }
}


#pragma mark - delegate
void CashManager::subDelegateInfo(MafFunction<void(bool)>* func)
{
    _delegateInfo += func;
}
void CashManager::invokeDelegateInfo(bool bResult)
{
    _delegateInfo(bResult);
}
void CashManager::unSubDelegateInfo(void* obj)
{
    _delegateInfo.Clear(obj);
}

void CashManager::subDelegateDailyInfo(MafFunction<void(bool)>* func)
{
    _delegateDailyInfo += func;
}
void CashManager::invokeDelegateDailyInfo(bool bResult)
{
    _delegateDailyInfo(bResult);
}
void CashManager::unSubDelegateDailyInfo(void* obj)
{
    _delegateDailyInfo.Clear(obj);
}

void CashManager::subDelegatePurchaseRemainingAll(MafFunction<void(bool)>* func)
{
    _delegatePurchaseRemainingAll += func;
}
void CashManager::invokeDelegatePurchaseRemainingAll(bool bResult)
{
    _delegatePurchaseRemainingAll(bResult);
}
void CashManager::unSubDelegatePurchaseRemainingAll(void* obj)
{
    _delegatePurchaseRemainingAll.Clear(obj);
}


#pragma mark - download
void CashManager::downloadNews(const std::function<void(int)>& callbackStart, const std::function<void(double)>& callbackProgress, const std::function<void(int, int)>& callbackCount, const std::function<void(void)>& callbackEnd)
{
    _listDownload.clear();
    for(const auto &path : _listNewsDownload)
    {
        _listDownload.push_back(path);
    }
    
    if ( _listDownload.size() <= 0 )
    {
        return;
    }
    
    //
    _callbackDownloadStart = callbackStart;
    _callbackDownloadProgress = callbackProgress;
    _callbackDownloadCount = callbackCount;
    _callbackDownloadEnd = callbackEnd;
    
    //
    _downloadTotal = (int)_listDownload.size();
    _downloadSuccess = 0;
    _downloadError = 0;
    if ( _callbackDownloadStart != nullptr )
    {
        _callbackDownloadStart((int)_listDownload.size());
    }
    downloadStart();
}

void CashManager::downloadStart()
{
    if ( _listDownload.size() == 0 )
    {
        downloadEnd();
        return;
    }
    
    // 다운 경로
    std::string strUrl = _listDownload.at(0);
    _listDownload.erase(_listDownload.begin());
    
    // 파일 경로
    std::string strFilePath = strUrl.substr(strUrl.find("Assets/"));
    
    // 저장 경로
    _downloadStoragePathFull = FileUtils::getInstance()->getWritablePath() + strFilePath;
    _downloadStoragePath = _downloadStoragePathFull.substr(0, _downloadStoragePathFull.find_last_of("/") + 1);

    // 파일 생성
    if ( FileUtils::getInstance()->isDirectoryExist(_downloadStoragePath) == false )
    {
        FileUtils::getInstance()->createDirectory(_downloadStoragePath);
    }
    
    // 이전에 파일이 있으면 삭제.
    FileUtils::getInstance()->removeDirectory(_downloadStoragePathFull);
    FileUtils::getInstance()->removeDirectory(_downloadStoragePathFull + ".tmp");
    FileUtils::getInstance()->removeFile(_downloadStoragePathFull);
    FileUtils::getInstance()->removeFile(_downloadStoragePathFull + ".tmp");
    
    //
    _downloader->createDownloadFileTask(strUrl, _downloadStoragePathFull);
}

void CashManager::downloadEnd()
{
    _callbackDownloadStart = nullptr;
    _callbackDownloadProgress = nullptr;
    _callbackDownloadCount = nullptr;
    if ( _callbackDownloadEnd != nullptr )
    {
        _callbackDownloadEnd();
        _callbackDownloadEnd = nullptr;
    }
}

void CashManager::downloadTaskProgress(const cocos2d::network::DownloadTask& task, int64_t bytesReceived, int64_t totalBytesReceived, int64_t totalBytesExpected)
{
    double nPercent = double(totalBytesReceived * 100) / totalBytesExpected;
    if ( _callbackDownloadProgress != nullptr )
    {
        _callbackDownloadProgress(nPercent);
    }
}

void CashManager::downloadFileSuccess(const cocos2d::network::DownloadTask& task)
{
    _downloadSuccess++;
    if ( _callbackDownloadCount != nullptr )
    {
        _callbackDownloadCount(_downloadSuccess, _downloadError);
    }
    
    downloadStart();
}

void CashManager::downloadTaskError(const cocos2d::network::DownloadTask& task, int errorCode, int errorCodeInternal, const std::string& errorStr)
{
    _downloadError++;
    if ( _callbackDownloadCount != nullptr )
    {
        _callbackDownloadCount(_downloadSuccess, _downloadError);
    }
    
    downloadStart();
}

#pragma mark - purchase(결제)
void CashManager::purchaseInitiate(int nIdx, const std::function<void(MafGooglePlay::RESULT, int, int, std::string)>& pCallback)
{
    if ( _objProduct != nullptr )
    {
        if ( pCallback != nullptr )
        {
            pCallback(MafGooglePlay::RESULT::CANCEL, -1, nIdx, "");
        }
        return;
    }
    
    _callbackPurchase = pCallback;
    
    _strPurchaseData = "";
    _strPurchaseSignature = "";
    
    _objProduct = getProduct(nIdx);
    if ( _objProduct == nullptr || _objProduct->getPrice().length() == 0 )
    {// NULL, 사용하지 않을때, 가격을 못가져올때  리턴
        if ( _callbackPurchase != nullptr )
        {
            _callbackPurchase(MafGooglePlay::RESULT::CANCEL, -1, nIdx, "");
        }
        
        _objProduct = nullptr;
        _callbackPurchase = nullptr;
        return;
    }
    
    if ( AccountManager::getInstance()->getUserID().length() == 0 )
    {// 로그인이 없을때 리턴
        if ( _callbackPurchase != nullptr )
        {
            _callbackPurchase(MafGooglePlay::RESULT::CANCEL, -1, _objProduct->getIdx(), "");
        }
        
        _objProduct = nullptr;
        _callbackPurchase = nullptr;
        return;
    }
    
    //
    if ( _objProduct->getSaleLimit() > 0 )
    {
        //
        purchaseConsumeAll();
        
        //
        requestPurchaseRemaining(_objProduct->getIdx(), [=](bool bResult, int nResult){
            
            if ( bResult == true )
            {
                MafGooglePlay::Purchase(_objProduct->getProductIdReal().c_str(), _objProduct->getProductIdWatch().c_str(), _objProduct->getPriceOriginal().c_str(), _objProduct->getPriceCurrencyCode().c_str(), CC_CALLBACK_3(CashManager::purchaseResult, this));
            }
            else
            {
                if ( _callbackPurchase != nullptr )
                {
                    _callbackPurchase(MafGooglePlay::RESULT::CANCEL, nResult, _objProduct->getIdx(), "");
                }
                
                _objProduct = nullptr;
                _callbackPurchase = nullptr;
            }
        });
    }
    else
    {
        MafGooglePlay::Purchase(_objProduct->getProductIdReal().c_str(), _objProduct->getProductIdWatch().c_str(), _objProduct->getPriceOriginal().c_str(), _objProduct->getPriceCurrencyCode().c_str(), CC_CALLBACK_3(CashManager::purchaseResult, this));
    }
}

void CashManager::purchaseResult(MafGooglePlay::RESULT eResult, std::string strData, std::string strSignature)
{
    if ( eResult == MafGooglePlay::RESULT::SUCCESS )
    {
        _strPurchaseData = strData;
        _strPurchaseSignature = strSignature;
        
        // network 통신
        requestPurchase();
    }
    else
    {
        if ( _callbackPurchase != nullptr && _objProduct != nullptr )
        {
            _callbackPurchase(MafGooglePlay::RESULT::CANCEL, -1, _objProduct->getIdx(), "");
        }
        else if ( _callbackPurchase != nullptr )
        {
            _callbackPurchase(MafGooglePlay::RESULT::CANCEL, -1, -1, "");
        }
        
        _objProduct = nullptr;
        _callbackPurchase = nullptr;
    }
}

void CashManager::purchaseHas(int nIdx, const std::function<void(int)>& callback)
{
    auto objProduct = getProduct(nIdx);
    if ( objProduct != nullptr )
    {
        MafGooglePlay::PurchaseHas(objProduct->getProductIdReal().c_str(), objProduct->getProductIdWatch().c_str(), callback);
    }
    else
    {
        callback(0);
    }
}

void CashManager::purchaseConsumeAll()
{
    MafGooglePlay::PurchaseConsumeAll();
}

#pragma mark - network : info
void CashManager::requestInfo(const std::function<void(bool)>& pCallback)
{
    //
    _callbackInfo = pCallback;
    
    //
    std::string url = "/shop/user/info/%s/%d";
    url = MafUtils::format(url.c_str(), AccountManager::getInstance()->getUserID().c_str(), AccountManager::getInstance()->getUserIdx());
    
    auto req = new MafHttpNew(network::HttpRequest::Type::GET, url.c_str());
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseInfo(response,dd);
    };
    req->send(cb);
}

void CashManager::responseInfo(network::HttpResponse* response,std::string &data)
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
        invokeDelegateInfo(false);
        if ( _callbackInfo != nullptr )
        {
            _callbackInfo(false);
        }
        return;
    }
    
    if ( jsonParser.HasMember("_coin") == true )
    {
        int nCount = jsonParser["_coin"].GetInt();
        ItemsManager::getInstance()->setItemsServer(E_ITEMS::COSTUME_COIN, 0, nCount);
    }
    
    if ( jsonParser.HasMember("_material") == true )
    {
        int nCount = jsonParser["_material"].GetInt();
        ItemsManager::getInstance()->setItemsServer(E_ITEMS::MATERIAL, 0, nCount);
    }
    
    // callback
    invokeDelegateInfo(true);
    if ( _callbackInfo != nullptr )
    {
        _callbackInfo(true);
    }
}

void CashManager::requestPurchaseRemainingAll(const std::function<void(bool)>& pCallback)
{
    //
    _callbackPurchaseRemainingAll = pCallback;
    
    //
    std::string url = "/v2/shop/data/%d";
    url = MafUtils::format(url.c_str(), AccountManager::getInstance()->getUserIdx());
    
    auto req = new MafHttpNew(network::HttpRequest::Type::GET, url.c_str());
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responsePurchaseRemainingAll(response,dd);
    };
    req->send(cb);
}

void CashManager::responsePurchaseRemainingAll(network::HttpResponse* response,std::string &data)
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
    
    if ( bError == false && jsonParser.HasMember("_userdata") == false )
    {
        bError = true;
    }
    
    if ( bError == true )
    {
        // callback
        invokeDelegatePurchaseRemainingAll(false);
        if ( _callbackPurchaseRemainingAll != nullptr )
        {
            _callbackPurchaseRemainingAll(false);
            _callbackPurchaseRemainingAll = nullptr;
        }
        return;
    }

    _listPurchaseRemaining.clear();
    if ( jsonParser["_userdata"].IsArray() == true )
    {
        for ( int i = 0; i < jsonParser["_userdata"].Size(); i++ )
        {
            const rapidjson::Value& jsonValue = jsonParser["_userdata"][i];

            int nIdx = jsonValue["itemidx"].GetInt();
            int nCount = jsonValue["count"].GetInt();

            setPurchaseRemaining(nIdx, nCount);
        }
    }
    
    // callback
    invokeDelegatePurchaseRemainingAll(true);
    if ( _callbackPurchaseRemainingAll != nullptr )
    {
        _callbackPurchaseRemainingAll(true);
        _callbackPurchaseRemainingAll = nullptr;
    }
}

void CashManager::requestPurchaseRemaining(int nItemIdx, const std::function<void(bool, int)>& pCallback)
{
    //
    _callbackPurchaseRemaining = pCallback;
    
    //
    std::string url = "/v2/shop/user/isPurchase";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_itemidx", nItemIdx);
    req->setData("_version", 201);
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responsePurchaseRemaining(response,dd);
    };
    req->send(cb);
}

void CashManager::responsePurchaseRemaining(network::HttpResponse* response,std::string &data)
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
        if ( _callbackPurchaseRemaining != nullptr )
        {
            _callbackPurchaseRemaining(false, -1);
        }
        return;
    }

    if ( jsonParser.HasMember("_items") == true && jsonParser["_items"].IsArray() == true )
    {
        for ( int i = 0; i < jsonParser["_items"].Size(); i++ )
        {
            const rapidjson::Value& jsonValue = jsonParser["_items"][i];
            
            int nIdx = jsonValue["idx"].GetInt();
            int nCount = jsonValue["count"].GetInt();
            
            setPurchaseRemaining(nIdx, nCount);
        }
    }
    
    int nResult = jsonParser["_result"].GetInt();
    if ( nResult == 1 )
    {
        if ( _callbackPurchaseRemaining != nullptr )
        {
            _callbackPurchaseRemaining(true, nResult);
        }
    }
    else
    {
        // callback
        if ( _callbackPurchaseRemaining != nullptr )
        {
            _callbackPurchaseRemaining(false, nResult);
        }
    }
}

#pragma mark - network : banner
void CashManager::requestBannerInfo(const std::function<void(bool)>& pCallback)
{
    //
    _callbackBannerInfo = pCallback;
    
    //
    std::string url = "/v1/shop/banner";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_grade", _nBannerEventGrade); //0 : 일반, 1 : 전문가
    
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseBannerInfo(response,dd);
    };
    req->send(cb);
}

void CashManager::responseBannerInfo(network::HttpResponse* response,std::string &data)
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
        if ( _callbackBannerInfo != nullptr )
        {
            _callbackBannerInfo(false);
        }
        return;
    }
    
    int nResult = jsonParser["_result"].GetInt();
    if ( nResult == 1 )
    {
        _nBannerEventIdx = jsonParser["_banner_idx"].GetInt();
        _nBannerEventGrade = jsonParser["_grade"].GetInt();
        _nBannerEventTime = jsonParser["_end_time"].GetInt64();
        
        
        //
        int nBannerEventIdxOld = UserDefault::getInstance()->getIntegerForKey(KEY_PURCHASE_BANNER, 0);
        if ( nBannerEventIdxOld != _nBannerEventIdx )
        {
            UserDefault::getInstance()->setIntegerForKey(KEY_PURCHASE_BANNER, _nBannerEventIdx);
            
            RefreshReddotManager::getInstance()->setValue(E_REFRESH_REDDOT::PURCHASE_BANNER, true);
            RefreshReddotManager::getInstance()->refreshUI(E_REFRESH_REDDOT::PURCHASE_BANNER);
        }
        

        //
        _listBanner.clear();
        for ( int i = 0; i < jsonParser["_data"].Size(); i++ )
        {
            const rapidjson::Value& jsonValue = jsonParser["_data"][i];

            int nItemIdx = jsonValue["_itemidx"].GetInt();
            bool bReceive = jsonValue["_is_receive"].GetBool();
            std::string strBGPath = jsonValue["_bg"].GetString();
            std::string strBGLang = jsonValue["_bg_lang"].GetString();

            auto obj = InfoProductBanner::create();
            obj->setItemIdx(nItemIdx);
            obj->setReceive(bReceive);
            obj->setBGPath(strBGPath);
            obj->setBGLang(strBGLang);

            _listBanner.pushBack(obj);
        }
        
        // callback
        if ( _callbackBannerInfo != nullptr )
        {
            _callbackBannerInfo(true);
        }
    }
    else
    {
        // callback
        if ( _callbackBannerInfo != nullptr )
        {
            _callbackBannerInfo(false);
        }
    }
}

void CashManager::requestBannerBonusReceive(const std::function<void(bool)>& pCallback)
{
    //
    _callbackBannerBonusReceive = pCallback;
    
    //
    std::string url = "/shop/banner/bonus";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_banneridx", _nBannerEventIdx);
    req->setData("_grade", _nBannerEventGrade);
    
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseBannerBonusReceive(response,dd);
    };
    req->send(cb);
}

void CashManager::responseBannerBonusReceive(network::HttpResponse* response,std::string &data)
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
    
    if ( bError == false && jsonParser.HasMember("_result") == false  )
    {
        bError = true;
    }
    
    if ( bError == true )
    {
        // callback
        if ( _callbackBannerBonusReceive != nullptr )
        {
            _callbackBannerBonusReceive(false);
        }
        return;
    }
    
    int nResult = jsonParser["_result"].GetInt();
    if ( nResult == 1 )
    {
        std::string strReward         = jsonParser["_reward"].GetString();
        std::string strItems         = jsonParser["_items"].GetString();
        ItemsManager::getInstance()->addItems(strReward);
        ItemsManager::getInstance()->setItemsServer(strItems);
        
        // callback
        if ( _callbackBannerBonusReceive != nullptr )
        {
            _callbackBannerBonusReceive(true);
        }
    }
    else
    {
        // callback
        if ( _callbackBannerBonusReceive != nullptr )
        {
            _callbackBannerBonusReceive(false);
        }
    }
}

#pragma mark - network : daily
void CashManager::requestDailyInfo(const std::function<void(bool)>& pCallback)
{
    //
    _callbackDailyInfo = pCallback;
    
    //
    _listDailyDate.clear();
    _listDailyReceive.clear();
    
    std::string url = "/v1/shop/user/pension/%s/%d";
    url = MafUtils::format(url.c_str(), AccountManager::getInstance()->getUserID().c_str(), AccountManager::getInstance()->getUserIdx());
    
    auto req = new MafHttpNew(network::HttpRequest::Type::GET, url.c_str());
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseDailyInfo(response,dd);
    };
    req->send(cb);
}

void CashManager::responseDailyInfo(network::HttpResponse* response,std::string &data)
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
    
    if ( bError == false && jsonParser.IsArray() == false  )
    {
        bError = true;
    }
    
    if ( bError == true )
    {
        // callback
        invokeDelegateDailyInfo(false);
        if ( _callbackDailyInfo != nullptr )
        {
            _callbackDailyInfo(false);
            _callbackDailyInfo = nullptr;
        }
        return;
    }
    
    //
    if ( jsonParser.IsArray() == true )
    {
        for ( int i = 0; i < jsonParser.Size(); i++ )
        {
            const rapidjson::Value& jsonProduct = jsonParser[i];
        
            int nIdx = jsonProduct["_itemidx"].GetInt();
            int nTime = 0;
            int nDate = 0;
            bool isReceive = false;
            
            if(nIdx == E_CASH::CASH_DAILY_ADS_NEW)
            {
                nTime = jsonProduct["_end_time"].GetInt();
                if ( nTime > 0 )
                {// 광고 스킵 적용
                    CashManager::getInstance()->setAdsfreeTime(nTime);
                }
            }
            else
            {
                nDate = jsonProduct["_restday"].GetInt();
                isReceive = jsonProduct["_available"].GetBool();
            }
            
            if ( nDate > 0 )
            {
                setDailyDate(nIdx, nDate);
                setDailyReceive(nIdx, isReceive);
            }
        }
        
        // callback
        invokeDelegateDailyInfo(true);
        if ( _callbackDailyInfo != nullptr )
        {
            _callbackDailyInfo(true);
            _callbackDailyInfo = nullptr;
        }
    }
}

void CashManager::requestDailyReceive(int nIdx, const std::function<void(bool, int, std::string)>& pCallback)
{
    _callbackDailyReceive = pCallback;
    
    _objProduct = getProduct(nIdx);
    if ( _objProduct == nullptr )
    {// 널이거나 사용하지 않을때 리턴
        if ( _callbackDailyReceive != nullptr )
        {
            _callbackDailyReceive(false, nIdx, "");
        }
        
        _objProduct = nullptr;
        _callbackDailyReceive = nullptr;
        return;
    }
    
    //
    std::string url = "/v1/shop/user/pension/receive";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_userid", AccountManager::getInstance()->getUserID().c_str());
    req->setData("_itemidx", _objProduct->getIdx());
    
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseDailyReceive(response,dd);
    };
    req->send(cb);
}

void CashManager::responseDailyReceive(network::HttpResponse* response,std::string &data)
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
    
    if ( bError == false && jsonParser.HasMember("_result") == false  )
    {
        bError = true;
    }
    
    if ( bError == false && jsonParser.HasMember("_items") == false )
    {
        bError = true;
    }
    
    if ( bError == true )
    {
        // callback
        if ( _callbackDailyReceive != nullptr )
        {
            _callbackDailyReceive(false, _objProduct->getIdx(), "");
        }
        _objProduct = nullptr;
        _callbackDailyReceive = nullptr;
        return;
    }
    
    bool isResult = jsonParser["_result"].GetBool();
    if ( isResult == true )
    {
        std::string strReward         = jsonParser["_reward"].GetString();
        std::string strItems         = jsonParser["_items"].GetString();
        ItemsManager::getInstance()->addItems(strReward);
        ItemsManager::getInstance()->setItemsServer(strItems);
        
        // callback
        if ( _callbackDailyReceive != nullptr )
        {
            _callbackDailyReceive(true, _objProduct->getIdx(), strItems);
        }
    }
    else
    {
        // callback
        if ( _callbackDailyReceive != nullptr )
        {
            _callbackDailyReceive(false, _objProduct->getIdx(), "");
        }
    }
    
    _objProduct = nullptr;
    _callbackDailyReceive = nullptr;
}

#pragma mark - network : growth
void CashManager::requestGrowthInfo(int nItemIdx, const std::function<void(bool, bool)>& pCallback)
{
    //
    _callbackGrowthInfo = pCallback;
    
    //
    std::string url = "/v1/shop/user/growth/pass/%d/%d";
    url = MafUtils::format(url.c_str(), AccountManager::getInstance()->getUserIdx(), nItemIdx);
    
    auto req = new MafHttpNew(network::HttpRequest::Type::GET, url.c_str());
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseGrowthInfo(response,dd);
    };
    req->send(cb);
}

void CashManager::responseGrowthInfo(network::HttpResponse* response,std::string &data)
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
        if ( _callbackGrowthInfo != nullptr )
        {
            _callbackGrowthInfo(false, false);
        }
        return;
    }
    
    //
    int nResult = jsonParser["_result"].GetInt();
    if ( nResult == 1 )
    {
        bool bBuy = false;
        
        //
        for ( int i = 0; i < jsonParser["_items"].Size(); i++ )
        {
            const rapidjson::Value& jsonValue = jsonParser["_items"][i];
            
            int nIdxCash = jsonValue["itemidx"].GetInt();
            for ( int j = 0; j < jsonValue["normal_reward"].Size(); j++ )
            {
                int nIdx = j + 1;
                auto obj = getGrowth(nIdx, nIdxCash);
                if ( obj != nullptr )
                {
                    obj->setRewardFree(jsonValue["normal_reward"][j].GetInt());
                }
            }
            
            for ( int j = 0; j < jsonValue["normal_reward"].Size(); j++ )
            {
                int nIdx = j + 1;
                auto obj = getGrowth(nIdx, nIdxCash);
                if ( obj != nullptr )
                {
                    obj->setRewardCash(jsonValue["pass_reward"][j].GetInt());
                }
            }
            
            bBuy = jsonValue["buy_pass"].GetBool();
            
            if ( bBuy == true && getHistoryProduct(nIdxCash) <= 0 )
            {
                addHistoryProduct(nIdxCash);
            }
        }
        
        // callback
        if ( _callbackGrowthInfo != nullptr )
        {
            _callbackGrowthInfo(true, bBuy);
        }
    }
    else
    {
        // callback
        if ( _callbackGrowthInfo != nullptr )
        {
            _callbackGrowthInfo(false, false);
        }
    }
}

void CashManager::requestGrowthReceive(int nItemIdx, int nIdx, const std::function<void(bool)>& pCallback)
{
    //
    _callbackGrowthReceive = pCallback;
    
    //
    std::string url = "/v2/shop/user/growth/pass/receive";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_itemidx", nItemIdx);
    req->setData("_recvidx", nIdx);
    
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseGrowthReceive(response,dd);
    };
    req->send(cb);
}

void CashManager::responseGrowthReceive(network::HttpResponse* response,std::string &data)
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
    
    if ( bError == false && jsonParser.HasMember("_result") == false  )
    {
        bError = true;
    }
    
    if ( bError == true )
    {
        // callback
        if ( _callbackGrowthReceive != nullptr )
        {
            _callbackGrowthReceive(false);
        }
        return;
    }
    
    int nResult = jsonParser["_result"].GetInt();
    if ( nResult == 1 )
    {
        std::string strReward = jsonParser["_reward"].GetString();
        std::string strItems = jsonParser["_items"].GetString();
        ItemsManager::getInstance()->addItems(strReward);
        ItemsManager::getInstance()->setItemsServer(strItems);
        
        // callback
        if ( _callbackGrowthReceive != nullptr )
        {
            _callbackGrowthReceive(true);
        }
    }
    else
    {
        // callback
        if ( _callbackGrowthReceive != nullptr )
        {
            _callbackGrowthReceive(false);
        }
    }
}

#pragma mark - network : first
void CashManager::requestFirstBonusInfo(const std::function<void(bool)>& pCallback)
{
    //
    _callbackFirstBonusInfo = pCallback;
    
    //
    std::string url = "/shop/user/first/bonus/info/%d";
    url = MafUtils::format(url.c_str(), AccountManager::getInstance()->getUserIdx());
    
    auto req = new MafHttpNew(network::HttpRequest::Type::GET, url.c_str());
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseFirstBonusInfo(response,dd);
    };
    req->send(cb);
}

void CashManager::responseFirstBonusInfo(network::HttpResponse* response,std::string &data)
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
        if ( _callbackFirstBonusInfo != nullptr )
        {
            _callbackFirstBonusInfo(false);
        }
        return;
    }
    
    /*
    0 # 구매한적이 없는데 보상 받을 수 있는 상태
    1 # 구매한적이 없음.
    2 # 이벤트 시간 아님
    3 # 구매한적은 있는데 보상을 못받는 상태
    4 # 보상 정보 없음
    5 # 이미 받음
     */
    _nFirstBonusResult = jsonParser["_result"].GetInt();
    _nFirstBonusVersion = -1;
    if ( jsonParser.HasMember("_version") == true )
    {
        _nFirstBonusVersion = jsonParser["_version"].GetInt();
    }
    
    // callback
    if ( _callbackFirstBonusInfo != nullptr )
    {
        _callbackFirstBonusInfo(true);
    }
}

void CashManager::requestFirstBonusReceive(const std::function<void(bool, std::string)>& pCallback)
{
    //
    _callbackFirstBonusReceive = pCallback;
    
    //
    std::string url = "/v1/shop/user/first/bonus/reward";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_version", _nFirstBonusVersion);
    
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseFirstBonusReceive(response,dd);
    };
    req->send(cb);
}

void CashManager::responseFirstBonusReceive(network::HttpResponse* response,std::string &data)
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
    
    if ( bError == false && jsonParser.HasMember("_result") == false  )
    {
        bError = true;
    }
    
    if ( bError == true )
    {
        // callback
        if ( _callbackFirstBonusReceive != nullptr )
        {
            _callbackFirstBonusReceive(false, "");
        }
        return;
    }
    
    int nResult = jsonParser["_result"].GetInt();
    if ( nResult == 1 )
    {
        _nFirstBonusResult = 5;
        
        std::string strReward = jsonParser["_reward"].GetString();
        std::string strItems = jsonParser["_items"].GetString();
        ItemsManager::getInstance()->addItems(strReward);
        ItemsManager::getInstance()->setItemsServer(strItems);
        
        // callback
        if ( _callbackFirstBonusReceive != nullptr )
        {
            _callbackFirstBonusReceive(true, strItems);
        }
    }
    else
    {
        // callback
        if ( _callbackFirstBonusReceive != nullptr )
        {
            _callbackFirstBonusReceive(false, "");
        }
    }
}

#pragma mark - network : news
void CashManager::requestNewsInfo(const std::function<void(bool)>& pCallback)
{
    //
    _callbackNewsInfo = pCallback;
    
    //
    std::string url = "/getGameShopNoticeList/%s";
    url = MafUtils::format(url.c_str(), TextManager::getInstance()->getLang().c_str());
    
    auto req = new MafHttpNew(network::HttpRequest::Type::GET, url.c_str());
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseNewsInfo(response,dd);
    };
    req->send(cb);
}

void CashManager::responseNewsInfo(network::HttpResponse* response,std::string &data)
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
        if ( _callbackNewsInfo != nullptr )
        {
            _callbackNewsInfo(false);
            _callbackNewsInfo = nullptr;
        }
        return;
    }
    
    std::vector<std::string> listNews;
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];
     
        
        listNews.push_back(jsonValue["img_url"].GetString());
    }
    std::reverse( std::begin(listNews), std::end(listNews) );
  
    _listNews.clear();
    _listNewsDownload.clear();
    for(const auto &path : listNews)
    {
        std::string filePath = path.substr(path.find("Assets/"));
        
        _listNews.push_back(filePath);
        if ( FileUtils::getInstance()->isFileExist(filePath) == false )
        {
            _listNewsDownload.push_back(path);
        }
    }
    
    // callback
    if ( _callbackNewsInfo != nullptr )
    {
        _callbackNewsInfo(true);
        _callbackNewsInfo = nullptr;
    }
}


#pragma mark - network : purchase
void CashManager::requestPurchase()
{
    if ( _objProduct == nullptr )
    {
        return;
    }
    
    std::string url = "/v4/shop/item/purchase";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_userid", AccountManager::getInstance()->getUserID().c_str());
    req->setData("_itemidx", _objProduct->getIdx());
    req->setData("_tableversion", TableManager::getInstance()->getVersion());
    req->setData("signed_data", _strPurchaseData.c_str());
    req->setData("signature", _strPurchaseSignature.c_str());
    req->setData("price_code", _objProduct->getPriceCurrencyCode().c_str());
    req->setData("price", _objProduct->getPriceOriginal().c_str());
    
    if ( _objProduct->getShopType() == E_PRODUCT::EVENT_BANNER_NEW )
    {
        req->setData("_event_idx", _nBannerEventIdx);
    }
    else
    {
        req->setData("_event_idx", 0);
    }
    
    if ( _objProduct->getIdx() == E_CASH::CASH_ARTIFACT_300_1 || _objProduct->getIdx() == E_CASH::CASH_ARTIFACT_300_2 || _objProduct->getIdx() == E_CASH::CASH_ARTIFACT_300_3 || _objProduct->getIdx() == E_CASH::CASH_ARTIFACT_300_4 ||
         _objProduct->getIdx() == E_CASH::CASH_ARTIFACT_3000_1 || _objProduct->getIdx() == E_CASH::CASH_ARTIFACT_3000_2 || _objProduct->getIdx() == E_CASH::CASH_ARTIFACT_3000_3 || _objProduct->getIdx() == E_CASH::CASH_ARTIFACT_3000_4 )
    {
        E_ARTIFACT eArtifact = (E_ARTIFACT)ItemsManager::getInstance()->getParameterArtifact();
        auto objArtifact = DataManager::getInstance()->GetArtifactInfo(eArtifact);
        if ( objArtifact != nullptr )
        {
            req->setData("_artifact", objArtifact->getGeneration());
        }
        else
        {
            req->setData("_artifact", 0);
        }
    }
    else if(_objProduct->getIdx() == E_CASH::CASH_ARTIFACT_INFINITE_3000_1 || _objProduct->getIdx() == E_CASH::CASH_ARTIFACT_INFINITE_3000_2 || _objProduct->getIdx() == E_CASH::CASH_ARTIFACT_INFINITE_3000_3 || _objProduct->getIdx() == E_CASH::CASH_ARTIFACT_INFINITE_3000_4 )
    {
        if(InfiniteArtifactManager::getInstance()->isActive())
            req->setData("_artifact", 1000 + InfiniteArtifactManager::getInstance()->getCurGrade());
        else
            req->setData("_artifact", 0);
    }
    else
    {
        req->setData("_artifact", 0);
    }
    
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responsePurchase(response,dd);
    };
    req->send(cb);
}

void CashManager::responsePurchase(network::HttpResponse* response,std::string &data)
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
    
    if ( bError == false && jsonParser.HasMember("_result") == false  )
    {
        bError = true;
    }
    
    int nResult = 0;
    if ( bError == false )
    {
        nResult = jsonParser["_result"].GetInt();
    }
    
    if ( nResult == 1 )
    {
        std::string strReward = jsonParser["_reward"].GetString();
        std::string strItems = jsonParser["_items"].GetString();
        ItemsManager::getInstance()->addItems(strReward);
        ItemsManager::getInstance()->setItemsServer(strItems);
        
        int nCoin = jsonParser["_coin"].GetInt();
        int nMaterial = jsonParser["_material"].GetInt();
        int nHonor = jsonParser["_honor"].GetInt();
        ItemsManager::getInstance()->setItemsServer(E_ITEMS::COSTUME_COIN, 0, nCoin);
        ItemsManager::getInstance()->setItemsServer(E_ITEMS::MATERIAL, 0, nMaterial);
        ItemsManager::getInstance()->setItemsServer(E_ITEMS::HONOR, 0, nHonor);

        // 기록
        addHistoryTotal();
        if ( _objProduct->getShopType() != E_PRODUCT::EVENT_BANNER_NEW && _objProduct->getShopType() != E_PRODUCT::EVENT_RETURN )
        {
            addHistoryProduct(_objProduct->getIdx());
        }
        
        //
        MafGooglePlay::PurchaseConsume(_objProduct->getProductIdReal().c_str(), _objProduct->getProductIdWatch().c_str());

        // callback
        if ( _callbackPurchase != nullptr )
        {
            _callbackPurchase(MafGooglePlay::RESULT::SUCCESS, 1, _objProduct->getIdx(), strItems);
        }
        
        #if !defined(COCOS2D_DEBUG) || (COCOS2D_DEBUG == 0)
        {
            MafAppsFlyer::TrackEventPurchase(_objProduct->getProductIdWatch().c_str(), _objProduct->getPriceOriginal().c_str(), _objProduct->getPriceCurrencyCode().c_str());
        }
        #endif
        
        //
        _objProduct = nullptr;
        _callbackPurchase = nullptr;
        
        // 첫 결제 기능 api 호출
        if ( getFirstBonusResult() == 1 )
        {
            requestFirstBonusInfo(nullptr);
        }
    }
    else if ( nResult == 5 || nResult == 4 )
    {
        /*
         5 : 이미 지급한 상품임
         4 : 배너상품 이미 지급 완료
         */
        //
        MafGooglePlay::PurchaseConsume(_objProduct->getProductIdReal().c_str(), _objProduct->getProductIdWatch().c_str());
        
        // callback
        if ( _callbackPurchase != nullptr )
        {
            _callbackPurchase(MafGooglePlay::RESULT::FAIL, -1, _objProduct->getIdx(), "");
        }
        
        _objProduct = nullptr;
        _callbackPurchase = nullptr;
    }
    else
    {
        PopupLoading::hide();
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_43"));
        popup->addButton(GAME_TEXT("t_ui_cancel"), true, [=](Ref* sender){
            
            //
            PopupLoading::show();
            
            // callback
            if ( _callbackPurchase != nullptr )
            {
                _callbackPurchase(MafGooglePlay::RESULT::FAIL, -1, _objProduct->getIdx(), "");
            }
            
            _objProduct = nullptr;
            _callbackPurchase = nullptr;
        });
        popup->addButton(GAME_TEXT("t_ui_again"), false, [=](Ref* sender){
            
            //
            PopupLoading::show();
            
            //
            requestPurchase();
        });
        popup->show();
    }
}

