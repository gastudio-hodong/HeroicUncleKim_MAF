//
//  ChatManager.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 19/09/2019.
//

#ifndef ChatManager_h
#define ChatManager_h

#include "Libs/maf/MafSingleton.h"

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

#include "GameObject/InfoChat.h"

class ChatManager : public MafSingleton<ChatManager>
{
public:
    ChatManager(void);
    virtual ~ChatManager(void);
    virtual bool init();
    
public:
    void saveData();
    void loadData();
    
    // set, get : list
    cocos2d::Vector<InfoChat*> getChatList();
    std::vector<std::string> getBlockList();
    
    void setChatList(const rapidjson::Value& jsonChatList);
    
    // set, get : block
    void addBlockList(std::string userId, std::string userNick);
    void delBlockList(std::string userId);
    int getBlockListIndex(std::string userId);
    int getBlockListCount();
    int getBlockListMax();
    
    // set, get
    time_t getTimeMessageSend();
    int getTimeMessageCoolDown();
    
    bool isBan();
        
    // callback
    
    // network
    void requestInfo(E_CHAT_TYPE eType, const std::function<void(bool, int)>& callback);
    void requestMessage(E_CHAT_TYPE eType, std::string strMessage, const std::function<void(bool, int)>& callback);
    
    
protected:
    // network
    void responseInfo(cocos2d::network::HttpResponse* response,std::string &data);
    void responseMessage(cocos2d::network::HttpResponse* response,std::string &data);
    
    
private:
    cocos2d::Vector<InfoChat*> _listChatInfo;
    std::vector<std::string> _listBlock;
    
    bool _bBan;
    
    time_t _timeMessageSend;
    
    //
    std::function<void(bool, int)> _callbackInfo;
    std::function<void(bool, int)> _callbackMessage;
    
};

#endif /* ChatManager_h */
