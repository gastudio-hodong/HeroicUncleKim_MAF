//
//  InfoStory.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 15/07/2019.
//

#ifndef InfoStory_h
#define InfoStory_h

#include "Common/ConfigDefault.h"

class InfoStory : public cocos2d::Ref
{
public:
    CREATE_FUNC(InfoStory);
    
    InfoStory();
    virtual ~InfoStory();
    virtual bool init();
    
public:
    int getIdx();
    void setIdx(int nIdx);
    
    //
    std::string getConditionType();
    void setConditionType(std::string strType);
    
    int getConditionParams();
    void setConditionParams(int nParams);
    
    //
    std::string getFileLanguage();
    void setFileLanguage(std::string strLang);
    
    std::string getFileNameIcon();
    std::vector<std::string> getFileNameImage();
    std::vector<std::string> getFileNameText();
    void setFileName(std::string strName);
    
    int getFileCount();
    void setFileCount(int nCount);
 
    //
    std::string getTextName();
    std::string getTextCondition();
    
    
private:
    int _nIdx;
    
    std::string _strConditionType;
    int _nConditionParams;
    
    std::string _strFileLanguage;
    std::string _strFileName;
    int _nFileCount;
    
};

#endif /* InfoStory_h */
