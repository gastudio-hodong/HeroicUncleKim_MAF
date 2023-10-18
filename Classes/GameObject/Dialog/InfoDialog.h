//
//  InfoDialog.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/08/05.
//

#ifndef InfoDialog_hpp
#define InfoDialog_hpp

#include "Common/ConfigDefault.h"

class InfoDialogScene;
class InfoDialog : public cocos2d::Ref
{
public:
    CREATE_FUNC(InfoDialog);
    InfoDialog();
    virtual ~InfoDialog();
    virtual bool init();
    
public:
    //Scene
    const bool isView() const;
    void setIsView (const bool value);
    
    const bool isRepeat() const;
    void setIsRepeat (const bool value);
    
    const int getIdx() const;
    void setIdx(const int value);
    
    const std::string getSceneName() const;
    void setSceneName(const std::string value);
    
    const std::string getType() const;
    void setType(const std::string value);
    
    const cocos2d::Vector<InfoDialogScene*> getListScene() const;
    void setListScene(const cocos2d::Vector<InfoDialogScene*> list);
    
    //condition
    const std::string getConditionType()const;
    void setConditionType(const std::string strType);
    const int getConditionParams()const;
    void setConditionParams(const int nParams);
    
    const std::string getFileNameIcon() const;
    void setFileNameIcon(const std::string value);
    const std::string getTextName() const;
    void setTextNameKey(const std::string value);
    const std::string getTextLock() const;
    void setTextLockKey(const std::string value);
protected:
    //Scene
    bool _isRepeat;
    int _nIdx;
    std::string _strSceneName;
    std::string _strType;
    std::string _strIconPath;
    std::string _strTextNameKey;
    std::string _strTextLockKey;
    
    int _nConditionParams;
    std::string _strConditionType;
    cocos2d::Vector<InfoDialogScene*> _listScene;
};

#endif /* InfoDialog_hpp */
