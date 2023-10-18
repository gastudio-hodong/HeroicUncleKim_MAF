//
//  ChatManager.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 19/09/2019.
//

#include "ChatManager.h"

#include "Common/ConfigKey.h"

#include "ManagerGame/AccountManager.h"

USING_NS_CC;

ChatManager::ChatManager(void) :

_bBan(false),
_timeMessageSend(0),

_callbackInfo(nullptr),
_callbackMessage(nullptr)
{
    
}

ChatManager::~ChatManager(void)
{
    
}

bool ChatManager::init()
{
    
    return true;
}

#pragma mark - data
void ChatManager::saveData()
{
    std::string

    // block
    str = UtilsString::getInstance()->splitJoin(_listBlock, ",");
    UserDefault::getInstance()->setStringForKey(KEY_CHAT_BLOCK, MafAes256::Encrypt(str));
}

void ChatManager::loadData()
{
    std::string str = "";
    
    // block
    str = UserDefault::getInstance()->getStringForKey(KEY_CHAT_BLOCK);
    _listBlock = UtilsString::getInstance()->split(MafAes256::Decrypt(str), ',');
}

#pragma mark - set, get : list
Vector<InfoChat*> ChatManager::getChatList()
{
    cocos2d::Vector<InfoChat*> listResult;
    
    for ( int i = 0; i < _listChatInfo.size(); i++ )
    {
        auto objChat = _listChatInfo.at(i);
        
        int nIdx = getBlockListIndex(objChat->getUserId());
        if ( nIdx == -1 )
        {
            listResult.pushBack(objChat);
        }
    }
    
    return listResult;
}

std::vector<std::string> ChatManager::getBlockList()
{
    std::vector<std::string> listResult;
    
    for ( int i = 0; i < _listBlock.size(); i++ )
    {
        std::string strBlock = _listBlock.at(i);
        if (strBlock.length() <= 0 )
        {
            continue;
        }
        
        std::vector<std::string> listBlockInfo = UtilsString::getInstance()->split(strBlock, '#');
        if ( listBlockInfo.size() < 2 )
        {
            continue;;
        }
        
        listResult.push_back(strBlock);
    }
    
    return listResult;
}

void ChatManager::setChatList(const rapidjson::Value& jsonChatList)
{
    //
    _listChatInfo.clear();
    
    //
    for ( int i = 0; i < jsonChatList.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonChatList[i];
 
        int nIdx = jsonValue["idx"].GetInt();
        
        int nUserIdx = jsonValue["useridx"].GetInt();
        std::string strUserId = jsonValue["userid"].GetString();
        std::string strUserNick = jsonValue["nick"].GetString();
        std::string strUserPlatform = jsonValue["platform"].GetString();
        std::string strMessage = jsonValue["message"].GetString();
        
        int nRank = jsonValue["rank"].GetInt();
        
        int nSkin = jsonValue["skin"].GetInt();
        std::string strCostume = jsonValue["costume"].GetString();
        
        auto infoChat = new InfoChat();
        infoChat->setIdx(nIdx);
        infoChat->setUserIdx(nUserIdx);
        infoChat->setUserId(strUserId);
        infoChat->setUserNick(strUserNick);
        infoChat->setUserPlatform(strUserPlatform);
        infoChat->setMessage(strMessage);
        infoChat->setRank(nRank);
        infoChat->setSkin(nSkin);
        infoChat->setCostume(strCostume);

        _listChatInfo.pushBack(infoChat);
    }
}

#pragma mark - set, get : block
void ChatManager::addBlockList(std::string userId, std::string userNick)
{
    if ( userId.empty() == true )
    {
        return;
    }
    
    int nIdx = getBlockListIndex(userId);
    if ( nIdx == -1 )
    {
        if ( userNick.empty() == true )
        {
            userNick = "N/A";
        }
        
        std::string strBlock = UtilsString::getInstance()->format("%s#%s", userId.c_str(), userNick.c_str());
        _listBlock.push_back(strBlock);
    }
    
    //
    saveData();
}

void ChatManager::delBlockList(std::string userId)
{
    if ( userId.empty() == true )
    {
        return;
    }
    
    int nIdx = getBlockListIndex(userId);
    if ( nIdx >= 0 )
    {
        _listBlock.erase(_listBlock.begin() + nIdx);
    }
    
    //
    saveData();
}

int ChatManager::getBlockListIndex(std::string userId)
{
    int nIdx = -1;
    
    if ( userId.empty() == true )
    {
        return nIdx;
    }
    
    for ( int i = 0; i < _listBlock.size(); i++ )
    {
        std::string strBlock = _listBlock.at(i);
        if (strBlock.length() <= 0 )
        {
            continue;
        }
        
        std::vector<std::string> listBlockInfo = UtilsString::getInstance()->split(strBlock, '#');
        
        std::string userIdTemp = listBlockInfo.at(0);
        if ( userIdTemp.compare(userId) == 0 )
        {
            nIdx = i;
            break;
        }
    }
    
    return nIdx;
}

int ChatManager::getBlockListCount()
{
    return (int)_listBlock.size();
}

int ChatManager::getBlockListMax()
{
    return 50;
}

#pragma mark - set, get
time_t ChatManager::getTimeMessageSend()
{
    return _timeMessageSend + getTimeMessageCoolDown();
}

int ChatManager::getTimeMessageCoolDown()
{
    return 5;
}

bool ChatManager::isBan()
{
    return _bBan;
}


#pragma mark - callback


#pragma mark - network
void ChatManager::requestInfo(E_CHAT_TYPE eType, const std::function<void(bool, int)>& callback)
{
    //
    _callbackInfo = callback;
    
    //
    std::string url = "/chat/list/%d/%d";
    url = MafUtils::format(url.c_str(), AccountManager::getInstance()->getUserIdx(), (int)eType);
    
    auto req = new MafHttpNew(network::HttpRequest::Type::GET, url.c_str());
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseInfo(response,dd);
    };
    req->send(cb);
}

void ChatManager::responseInfo(network::HttpResponse* response,std::string &data)
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
        if ( _callbackInfo != nullptr )
        {
            _callbackInfo(false, -1);
            _callbackInfo = nullptr;
        }
        return;
    }

    _bBan = false;
    
    /*
     0 : 유저 정보 없음
     1 : 성공
     2 : 채팅 벤 유저
     */
    auto nResult = jsonParser["_result"].GetInt();
    if ( nResult == 1 )
    {
        setChatList(jsonParser["_chat_list"]);
        
        if ( _callbackInfo != nullptr )
        {
            _callbackInfo(true, nResult);
            _callbackInfo = nullptr;
        }
    }
    else
    {
        if ( nResult == 2 )
        {
            _bBan = true;
        }
        
        if ( _callbackInfo != nullptr )
        {
            _callbackInfo(false, nResult);
            _callbackInfo = nullptr;
        }
    }
}

void ChatManager::requestMessage(E_CHAT_TYPE eType, std::string strMessage, const std::function<void(bool, int)>& callback)
{
    //
    _callbackMessage = callback;
    
    //
    std::string url = "/chat/update";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_type", (int)eType);
    req->setData("_message", strMessage);
    
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseMessage(response,dd);
    };
    req->send(cb);
}

void ChatManager::responseMessage(network::HttpResponse* response,std::string &data)
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
        if ( _callbackMessage != nullptr )
        {
            _callbackMessage(false, -1);
            _callbackMessage = nullptr;
        }
        return;
    }

    _bBan = false;
    
    /*
     0 : 유저 정보 없음
     1 : 성공
     2 : 채팅 벤 유저
     */
    auto nResult = jsonParser["_result"].GetInt();
    if ( nResult == 1 )
    {
        _timeMessageSend = UtilsDate::getInstance()->getTime();
        
        setChatList(jsonParser["_chat_list"]);
        
        if ( _callbackMessage != nullptr )
        {
            _callbackMessage(true, nResult);
            _callbackMessage = nullptr;
        }
    }
    else
    {
        if ( nResult == 2 )
        {
            _bBan = true;
        }
        
        if ( _callbackMessage != nullptr )
        {
            _callbackMessage(false, nResult);
            _callbackMessage = nullptr;
        }
    }
}
