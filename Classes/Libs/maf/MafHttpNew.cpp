//
//  MafHttpManager.cpp
//  FantasyClicker
//
//  Created by SanHeo on 2015. 6. 21..
//
//

#include "MafHttpNew.h"

#include "Common/Support/DebugLog.h"

#include "ManagerSystem/TextManager.h"
#include "ManagerGame/AccountManager.h"

#include "Libs/maf/MafAes256.h"
#include "Libs/maf/MafConfig.h"
#include "Libs/maf/MafUtils.h"
#include "Libs/maf/MafNative.h"

USING_NS_CC;

MafHttpModelNew::MafHttpModelNew(){
    _key = "";
    _value = "";
    _nValue = 0;
    _nValueDouble = 0;
    _type = TYPE_CHAR;
    _listValueInt.clear();
    _listValueDouble.clear();
    _listValueStr.clear();
    _ObjValue.clear();
    _listObjValue.clear();
}

void MafHttpModelNew::setData(const char *key,const char *value){
    _type = TYPE_CHAR;
    _key = key;
    _value = value;
}

void MafHttpModelNew::setData(const char *key,int value){
    _type = TYPE_INT;
    _key = key;
    _nValue = value;
}

void MafHttpModelNew::setDataDouble(const char *key,double value){
    _type = TYPE_DOUBLE;
    _key = key;
    _nValueDouble = value;
}
void MafHttpModelNew::setData(const char *key,std::vector<int> value)
{
    _type = TYPE_ARRAY_INT;
    _key = key;
    _listValueInt.clear();
    _listValueInt = value;
}

void MafHttpModelNew::setData(const char *key,std::vector<double> value)
{
    _type = TYPE_ARRAY_DOUBLE;
    _key = key;
    _listValueDouble.clear();
    _listValueDouble = value;
}

void MafHttpModelNew::setData(const char *key,std::vector<std::string> value)
{
    _type = TYPE_ARRAY_STR;
    _key = key;
    _listValueStr.clear();
    _listValueStr = value;
}

void MafHttpModelNew::setData( const char *key, std::vector<MafHttpModelNew*> value, bool isArray)
{
    _key = key;
    
    if(isArray) _type = Type_OBJECT_FOR_ARRAY;
    else _type = Type_OBJECT;
    
    _ObjValue.clear();
    _ObjValue = value;
}

void MafHttpModelNew::setDataObjArray(const char *key, std::vector<MafHttpModelNew*> value)
{
    _key = key;
    
    _type = Type_ARRAY_OBJECT;
    
    _listObjValue.clear();
    _listObjValue = value;
    
}

void MafHttpModelNew::converJsonPost(rapidjson::Writer<rapidjson::StringBuffer> &wr){
    
    if(_type != Type_OBJECT_FOR_ARRAY)
        wr.String(_key.c_str());
    
    if(_type==TYPE_CHAR){
        wr.String(_value.c_str());
    }
    else if(_type==TYPE_INT){
        wr.Int(_nValue);
    }
    else if(_type==TYPE_DOUBLE){
        wr.Double(_nValueDouble);
    }
    else if(_type==TYPE_ARRAY_INT){
        wr.StartArray();
        for ( int temp : _listValueInt) {
            wr.Int(temp);
        }
        wr.EndArray();
    }
    else if(_type==TYPE_ARRAY_DOUBLE){
        wr.StartArray();
        for ( int temp : _listValueDouble) {
            wr.Double(temp);
        }
        wr.EndArray();
    }
    else if(_type==TYPE_ARRAY_STR){
        wr.StartArray();
        for ( std::string temp : _listValueStr) {
            wr.String(temp.c_str());
        }
        wr.EndArray();
    }
    else if(_type==Type_OBJECT || _type == Type_OBJECT_FOR_ARRAY)
    {
        wr.StartObject();
        for ( auto temp : _ObjValue)
        {
            temp->converJsonPost(wr);
            delete temp;
        }
        wr.EndObject();
        _ObjValue.clear();
    }
    else if(_type == Type_ARRAY_OBJECT)
    {
        wr.StartArray();
        
        for(int i = 0; i<_listObjValue.size(); ++i)
        {
            _listObjValue[i]->_key = MafUtils::toString(i);
            _listObjValue[i]->converJsonPost(wr);
            delete _listObjValue[i];
        }
        
        wr.EndArray();
        _listObjValue.clear();
    }
}
void MafHttpModelNew::converGet(std::string &url){
    
    url.append(_key).append("=");
    if(_type==TYPE_CHAR){
        url.append(_value.c_str());
    }
    else if(_type==TYPE_INT){
        url.append(MafUtils::toString(_nValue));
    }
    
}

#pragma mark-
// static
std::string MafHttpNew::_strRootUrl = DUNGEON_SERVER_URL_LIVE;
cocos2d::Vector<network::HttpRequest*> MafHttpNew::_vecRequest;


MafHttpNew::MafHttpNew(network::HttpRequest::Type requestType, std::string url) :
_request(nullptr),
_cbFunction(nullptr),

_strUrl(""),
_strParameter("")
{
    if ( url.find("http") == -1 )
    {
        url = std::string(MafHttpNew::_strRootUrl).append(url);
    }
    
    // init
    _request = new network::HttpRequest();
    _request->setRequestType(requestType);
    _request->setResponseCallback(CC_CALLBACK_2(MafHttpNew::response, this));
    _vecRequest.pushBack(_request);
    
    // header
    std::vector<std::string> headers;
    headers.push_back("Content-Type: application/x-www-form-urlencoded");
    _request->setHeaders(headers);
    
    // data
    _strUrl = url;
    
    _strParameter = "";
    _strParameter.append(MafUtils::format("_platform=%s", CC_TARGET_PLATFORM == CC_PLATFORM_IOS ? "ios" : "aos"));
    _strParameter.append("&");
    _strParameter.append(MafUtils::format("_lang=%s", TextManager::getInstance()->getLang().c_str()));
    _strParameter.append("&");
    _strParameter.append(MafUtils::format("_version=%s", MafNative::GetGameVersion().c_str()));
    _strParameter.append("&");
    _strParameter.append(MafUtils::format("useridx=%d", AccountManager::getInstance()->getUserIdx()));
}

MafHttpNew::~MafHttpNew()
{
    _cbFunction = nullptr;
}

#pragma mark - data
void MafHttpNew::setData(const char *key, const char* value)
{
    MafHttpModelNew *model = new MafHttpModelNew();
    model->setData(key, value);
    _vecData.push_back(model);
}

void MafHttpNew::setData(const char *key, std::string value)
{
    MafHttpModelNew *model = new MafHttpModelNew();
    model->setData(key, value.c_str());
    _vecData.push_back(model);
}
void MafHttpNew::setData(const char *key, int value)
{
    MafHttpModelNew *model = new MafHttpModelNew();
    model->setData(key, value);
    _vecData.push_back(model);
}
void MafHttpNew::setDataDouble(const char *key, double value)
{
    MafHttpModelNew *model = new MafHttpModelNew();
    model->setDataDouble(key, value);
    _vecData.push_back(model);
}

void MafHttpNew::setData(const char *key, std::vector<int> value)
{
    MafHttpModelNew *model = new MafHttpModelNew();
    model->setData(key, value);
    _vecData.push_back(model);
}
void MafHttpNew::setData(const char *key, std::vector<double> value)
{
    MafHttpModelNew *model = new MafHttpModelNew();
    model->setData(key, value);
    _vecData.push_back(model);
}
void MafHttpNew::setData(const char *key, std::vector<std::string> value)
{
    MafHttpModelNew *model = new MafHttpModelNew();
    model->setData(key, value);
    _vecData.push_back(model);
}

void MafHttpNew::setData(const char *key, std::vector<MafHttpModelNew*> value, bool isArray)
{
    MafHttpModelNew *model = new MafHttpModelNew();
    model->setData(key, value, isArray);
    _vecData.push_back(model);
}

void MafHttpNew::setDataObjecArray(const char *key, std::vector<MafHttpModelNew*> value)
{
    MafHttpModelNew *model = new MafHttpModelNew();
    model->setDataObjArray(key, value);
    _vecData.push_back(model);
}

#pragma mark - request
void MafHttpNew::send(MafResponseNew cb)
{
    _cbFunction = cb;
    
    //
    std::string strUrl = _strUrl;
    std::string strParameter = _strParameter;
    
    if ( _request->getRequestType() == network::HttpRequest::Type::GET )
    {/*get*/
        if ( !_vecData.empty() )
        {
            strParameter.append("&");
            for(auto data : _vecData)
            {
                data->converGet(strParameter);
                if(_vecData.back() != data)
                    strParameter.append("&");
                
                delete data;
            }
        }
    }
    else
    {/*post*/
        rapidjson::GenericStringBuffer<rapidjson::UTF8<> > bf;
        rapidjson::Writer<rapidjson::StringBuffer> wr(bf);
        
        wr.StartObject();
        for(auto data : _vecData){
            data->converJsonPost(wr);
            delete data;
        }
        wr.EndObject();
        _vecData.clear();
        
        std::string sendData= "_data=";//"_data=";
        sendData.append(MafAes256::URLEncode(bf.GetString()));
        LOG("HTTP response send : _data=%s", bf.GetString());
        if(sendData.length()>0){
            _request->setRequestData(sendData.c_str(), sendData.length());
        }
    }
    strUrl.append("?");
    strUrl.append(strParameter);
    
    
    //
    _request->setUrl(strUrl);
    
    //
    network::HttpClient::getInstance()->setTimeoutForConnect(30);
    network::HttpClient::getInstance()->setTimeoutForRead(30);
    network::HttpClient::getInstance()->send(_request);
    _request->release();
    _request = nullptr;
}

void MafHttpNew::response(network::HttpClient* client, network::HttpResponse* response)
{
    //
    std::vector<char> *buffer = response->getResponseData();
    buffer->push_back('\0');
    std::string strResult = reinterpret_cast<char*>(buffer->data());
    #if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
    {
        DebugLog::log("HTTP response url: %s", response->getHttpRequest()->getUrl());
        DebugLog::log("HTTP response code: %ld", response->getResponseCode());
        DebugLog::log("HTTP response data: %s", strResult.c_str());
    }
    #endif

    auto iter = _vecRequest.find(response->getHttpRequest());
    if ( iter != _vecRequest.end() )
    {
        _vecRequest.erase(iter);
        
        if ( _cbFunction != nullptr )
        {
            _cbFunction((void*)this, response, strResult.c_str());
        }
    }
    
    // release 해줘야 함.
    release();
}

