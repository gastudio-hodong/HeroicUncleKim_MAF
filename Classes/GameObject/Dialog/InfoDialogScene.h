//
//  InfoDialogScene.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/07/19.
//

#ifndef InfoDialogScene_hpp
#define InfoDialogScene_hpp

#include "Common/ConfigDefault.h"

class InfoDialogScene : public cocos2d::Ref
{
public:
    CREATE_FUNC(InfoDialogScene);
    InfoDialogScene();
    virtual ~InfoDialogScene();
    virtual bool init();
    
public:
    //Scene
    const int getIdx() const;
    void setIdx(const int value);
    
    const std::string getSceneName() const;
    void setSceneName(const std::string value);
    
    const int getSceneIdx() const;
    void setSceneIdx(const int value);
    
    const bool isEndMotionComplete() const;
    void setIsEndMotionComplete(const bool value);
    
    const bool isEndMotionStart() const;
    void setIsEndMotionStart(const bool value);
    
    //Speaker
    void setIsSpeakerEndMotionComplete(const bool value);
    
    const int getSpeakerID() const;
    void setSpeakerID(const int value);
    
    const std::string getSpeakerName() const;
    void setSpeakerNameKey(const std::string value);
    
    const std::string getSpeakerPosition() const;
    void setSpeakerPosition(const std::string value);
    
    const std::string getSpeakerStartMotionType() const;
    void setSpeakerStartMotionType(const std::string value);
    
    const std::string getSpeakerEndMotionType() const;
    void setSpeakerEndMotionType(const std::string value);
    
    const std::vector<std::string> getListTextKey()const;
    const std::vector<std::string> getListText()const;
    /// text key 입력
    void addTextKey(const std::string key);
    
    //Listener
    void setIsListenerEndMotionComplete(const bool value);
    
    const int getListenerID() const;
    void setListenerID(const int value);
    
    const std::string getListenerPosition() const;
    void setListenerPosition(const std::string value);
    
    const std::string getListenerStartMotionType() const;
    void setListenerStartMotionType(const std::string value);
    
    const std::string getListenerEndMotionType() const;
    void setListenerEndMotionType(const std::string value);
    
    //Event
    const std::string getEventEndType() const;
    void setEventEndType(const std::string value);
    
    const std::string getEventEndInfo() const;
    void setEventEndInfo(const std::string value);
protected:
    //Scene
    bool _isEndMotionStart;
    int _nIdx;
    int _nSceneIdx;
    std::string _strSceneName;
    
    //Speaker
    bool _isSpeakerEndMotionComplete;
    int _nSpeakerID;
    std::string _strSpeakerPosition;
    std::string _strSpeakerNameKey;
    std::string _strSpeakerStartMotionType;
    std::string _strSpeakerEndMotionType;
    std::vector<std::string> _listTextKey;
    
    //Listener
    bool _isListenerEndMotionComplete;
    int _nListenerID;
    std::string _strListenerPosition;
    std::string _strListenerStartMotionType;
    std::string _strListenerEndMotionType;
    
    //Event
    std::string _strEventEndType;
    std::string _strEventEndInfo;
};

#endif /* InfoDialogScene_hpp */
