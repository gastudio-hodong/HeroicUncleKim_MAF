//
//  PopupDialog.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/07/19.
//

#ifndef PopupDialog_hpp
#define PopupDialog_hpp

#include "GameUIPopup/Base/PopupBase.h"

#include "GameObject/Dialog/InfoDialogScene.h"

#include "GameUI/UI/UIDialogueText.h"

class PopupDialog : public PopupBase
{
public:
    static PopupDialog* create(std::string strSceneName);
    
    PopupDialog();
    virtual ~PopupDialog(void);
    virtual bool init(std::string strSceneName);
    virtual void show() override;
    
protected:
    void initVar();
    void initUI();
    
    void uiText();
    
    //scene
    void startScene();
    void nextScene();
    
    void setCurrentName();
    void setCurrentText();
    void setCurrentActor();
    void setActorMotion(const std::string strType);
    void actionActorMotion(Node* node, const std::string strAction, const std::string strType);
    
    //click
    void onClickSceneSkip(Ref* sender);
    void onClickSkip(Ref* sender);
    void onClickScreen(Ref* sender);
    
    //
    void onEventNextStep();
    //utils
    InfoDialogScene* getDialogScene(const int nIdx);
private:
    bool _isEndDialog;
    std::string _strSceneName;
    cocos2d::Vector<InfoDialogScene*> _listInfoDialogScene;
    InfoDialogScene* _currentScene;
    
    // widget
    cocos2d::Layer* _lyImage;
    cocos2d::Layer* _lyText;
    UIDialogueText* _uiDialogText;
    cocos2d::Sprite* _sprNameBox;
};

#endif /* PopupDialog_hpp */
