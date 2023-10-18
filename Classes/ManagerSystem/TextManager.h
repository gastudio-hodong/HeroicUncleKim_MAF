//
//  TextManager.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 19/09/2019.
//

#ifndef TextManager_h
#define TextManager_h

#include "Libs/maf/MafSingleton.h"

#define GAME_TEXT(key) TextManager::getInstance()->text(key)
#define GAME_TEXTFORMAT(key, args...) TextManager::getInstance()->textFormat(key, ##args)

#define GAME_DIALOG_TEXT(key) TextManager::getInstance()->dialogText(key)
#define GAME_DIALOG_TEXTFORMAT(key, args...) TextManager::getInstance()->dialogTextFormat(key, ##args)

#define GAME_TALK() TextManager::getInstance()->talk()
#define GAME_TALKTIP() TextManager::getInstance()->talkTip()

class TextManager : public MafSingleton<TextManager>
{
public:
    TextManager(void);
    virtual ~TextManager(void);
    virtual bool init();
    
public:
    // load
    
    // set, get
    std::string getLang();
    std::string getLangFileName(std::string strPath);
    void setLang(std::string strLang);
    
    // text
    void textInit();
    std::string text(std::string strKey);
    std::string textFormat(std::string strKey, ...);
    
    // dialog
    void dialogTextInit();
    std::string dialogText(std::string strKey);
    std::string dialogTextFormat(std::string strKey, ...);
    
    // talk
    void talkInit();
    std::string talk();
    std::string talkTip();
    
    //
    bool isBadWord(std::string strText);
    
protected:
    
    

private:
    //
    std::string _strLanguage;
    
    std::string _strTalkTipText;
    std::string _strBadWordText;
    
    std::unordered_map<std::string, std::string> _listText;
    std::unordered_map<std::string, std::string> _listDialogText;
    std::vector<std::string> _listTalk;
    
};

#endif /* TextManager_h */
