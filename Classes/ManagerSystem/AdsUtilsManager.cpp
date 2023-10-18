//
//  AdsInterstitialErrorManager.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/03/31.
//

#include "AdsUtilsManager.h"

#include "Common/ConfigKey.h"

#include "GameScene/DevilDomScene.h"
#include "GameScene/HelloWorldScene.h"

#include "ManagerSystem/ParameterManager.h"

USING_NS_CC;

AdsUtilsManager::AdsUtilsManager(void):
_nTimer(0),
_uiAdsCooldown(nullptr)
{
    _listAdsInterstitialError.clear();
}

AdsUtilsManager::~AdsUtilsManager(void)
{
    if(_uiAdsCooldown != nullptr)
        _uiAdsCooldown->release();
    
    _uiAdsCooldown = nullptr;
}

bool AdsUtilsManager::init()
{
    loadListAdsInterstitialError();
    
    //
    auto uiAdsCooldown = UIAdsCooldown::create();
    uiAdsCooldown->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    uiAdsCooldown->retain();
    uiAdsCooldown->hide();
    
    _uiAdsCooldown = uiAdsCooldown;
    
    return true;
}

#pragma mark - Ads Cooldown
int AdsUtilsManager::addAdsCooldown()
{
    _nTimer -= 1;
    if(_nTimer <= 0)
        _nTimer = 0;
    
    return _nTimer;
}

int AdsUtilsManager::getAdsCooldown()
{
    return _nTimer;
}

int AdsUtilsManager::getAdsCooldownMax()
{
    return ParameterManager::getInstance()->getParameterInteger("ads_play_cooldown");
}

void AdsUtilsManager::setAdsCooldown(int nValue, bool isAuto)
{
    if(nValue == -1)
    {
        _nTimer = getAdsCooldownMax();
    }
    else
    {
        _nTimer = nValue;
    }
    
    if(isAuto)
    {
        auto layer = UtilsScene::getInstance()->getRunningLayer();
        auto originScene = UtilsScene::getInstance()->getRunningScene();
        
        if ( dynamic_cast<HelloWorldScene *>(layer) != nullptr )
        {
            auto scene = dynamic_cast<HelloWorldScene *>(layer);
            scene->runAction(CallFunc::create([=](){
                
                if ( _uiAdsCooldown->getParent() == nullptr )
                {
                    _uiAdsCooldown->setPosition(scene->getContentSize().width/2, scene->getContentSize().height - (scene->_sizeMapPattern.height - 80));
                    originScene->addChild(_uiAdsCooldown, ZORDER_SCENE_POPUP, TAG_SCENE_POPUP);
                }
                
                //
                ((UIAdsCooldown*)_uiAdsCooldown)->show();
                
            }));
        }
        else if ( dynamic_cast<DevilDomScene *>(layer) != nullptr )
        {
            auto scene = dynamic_cast<DevilDomScene *>(layer);
            scene->runAction(CallFunc::create([=](){
                if ( _uiAdsCooldown->getParent() == nullptr )
                {
                    _uiAdsCooldown->setPosition(scene->getContentSize().width/2, scene->getContentSize().height - (scene->_sizeMapPattern.height - 100));
                    originScene->addChild(_uiAdsCooldown, ZORDER_SCENE_POPUP, TAG_SCENE_POPUP);
                }
                
                //
                ((UIAdsCooldown*)_uiAdsCooldown)->show();
            }));
        }
    }
}

void AdsUtilsManager::adsCooldownHide()
{
    if ( ((UIAdsCooldown*)_uiAdsCooldown)->isActiveUI() )
    {
        ((UIAdsCooldown*)_uiAdsCooldown)->hide();
    }
}

#pragma mark - AdsInterstitial Error Check
bool AdsUtilsManager::isErrorAdsInterstitial(E_ADS_INTERSTITIAL_TYPE eType)
{
    bool result = false;
    
    checkAdsInterstitialErrorTime();
    
    for(auto iter : _listAdsInterstitialError)
    {
        if(iter.first == (int)eType)
        {
            result = true;
            break;
        }

    }
    
    return result;
}

int AdsUtilsManager::getAdsInterstitialErrorType(E_ADS_INTERSTITIAL_TYPE eType)
{
    int result = 0;

    checkAdsInterstitialErrorTime();
    
    for(auto iter : _listAdsInterstitialError)
    {
        if(iter.first == (int)eType)
        {
            result = iter.first;
            break;
        }
    }
    
    return result;
}

void AdsUtilsManager::addAdsInterstitialErrorType(int nType, int64_t nTime)
{
    auto iter = _listAdsInterstitialError.find(nType);
    if(iter == _listAdsInterstitialError.end())
    {
        _listAdsInterstitialError.insert(std::make_pair(nType, nTime));
    }
    saveListAdsInterstitialError();
}

void AdsUtilsManager::removeAdsInterstitialErrorType(int nType)
{
    auto iter = _listAdsInterstitialError.find(nType);
    
    if( iter != _listAdsInterstitialError.end())
    {
        _listAdsInterstitialError.erase(iter);
    }
    
    saveListAdsInterstitialError();
}

std::map<int, int64_t> AdsUtilsManager::getListAdsInterstitialError()
{
    return _listAdsInterstitialError;
}

void AdsUtilsManager::saveListAdsInterstitialError()
{
    std::string strData = UserDefault::getInstance()->getStringForKey(KEY_ADS_INTERSTITIAL_ERROR, "");
    strData = MafAes256::Decrypt(strData);
    if(strData.compare("null") == 0)
    {
        strData = "";
    }
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*) strData.c_str());
    
    checkAdsInterstitialErrorTime();
    
    for(auto iter : _listAdsInterstitialError)
    {
        jsonParser.SetObject();
        rapidjson::Value jsonKey("Error", jsonParser.GetAllocator());
        
        rapidjson::Value errorData;
        errorData.SetArray();
        
        rapidjson::Value errorObj;
        errorObj.SetObject();

        rapidjson::Value vReset_type;
        vReset_type.SetInt(iter.first);
        errorObj.AddMember("type", vReset_type, jsonParser.GetAllocator());

        rapidjson::Value vType;
        vType.SetInt64(iter.second);
        errorObj.AddMember("time", vType, jsonParser.GetAllocator());
        
        errorData.PushBack(errorObj, jsonParser.GetAllocator());
        
        jsonParser.AddMember(jsonKey, errorData, jsonParser.GetAllocator());
    }
    
    rapidjson::StringBuffer jsonBuffer;
    rapidjson::Writer<rapidjson::StringBuffer> jsonWriter(jsonBuffer);
    jsonParser.Accept(jsonWriter);
    
    strData = jsonBuffer.GetString();
    

    strData = MafAes256::Encrypt(strData);
    UserDefault::getInstance()->setStringForKey(KEY_ADS_INTERSTITIAL_ERROR, strData);
}
void AdsUtilsManager::loadListAdsInterstitialError()
{
    std::string strData = UserDefault::getInstance()->getStringForKey(KEY_ADS_INTERSTITIAL_ERROR, "");
    strData = MafAes256::Decrypt(strData);
    if(strData.compare("null") == 0)
    {
        strData = "";
    }
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*) strData.c_str());
    std::string strKey = "Error";
        
    if( jsonParser.HasParseError() == false )
    {
        if ( jsonParser.HasMember(strKey.c_str()) == true )
        {
            auto ErrorList = jsonParser[strKey.c_str()].GetArray();
            for(int i = 0; i < ErrorList.Size(); ++i)
            {
                auto errorInfo = ErrorList[i].GetObject();
                int nType = errorInfo["type"].GetInt();
                auto nTime = errorInfo["time"].GetInt64();
                
                addAdsInterstitialErrorType(nType, nTime);
            }
        }
    }
    checkAdsInterstitialErrorTime();
    
    saveListAdsInterstitialError();
}

std::string AdsUtilsManager::getAdsInterstitialErrorData()
{
    std::string strResult = "";
    std::string strData = UserDefault::getInstance()->getStringForKey(KEY_ADS_INTERSTITIAL_ERROR, "");
    strData = MafAes256::Decrypt(strData);
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)strData.c_str());
    if( jsonParser.HasParseError() == false )
    {
        
        rapidjson::StringBuffer jsonBuffer;
        rapidjson::Writer<rapidjson::StringBuffer> jsonWriter(jsonBuffer);
        jsonParser.Accept(jsonWriter);
        
        strResult = jsonBuffer.GetString();
    }
    else
    {
        strResult = "";
    }
    
    
    UserDefault::getInstance()->setStringForKey(KEY_ADS_INTERSTITIAL_ERROR, strResult);
    
    return strResult;
}

void AdsUtilsManager::checkAdsInterstitialErrorTime()
{
    std::map<int, int64_t> temp;
    
    for(auto iter : _listAdsInterstitialError)
    {
        time_t errorTime = iter.second;
        time_t nowTime = UtilsDate::getInstance()->getTime();
        struct tm *info = localtime(&errorTime);
        int errorDay = info->tm_mday;
        
        info = localtime(&nowTime);
        int nowDay = info->tm_mday;
        
        if(errorDay == nowDay)
        {
            temp.insert(iter);
        }
    }
    
    _listAdsInterstitialError.clear();
    _listAdsInterstitialError = temp;
}
