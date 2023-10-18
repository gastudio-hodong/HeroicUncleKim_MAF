//
//  DialogManager.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/07/19.
//

#ifndef DialogManager_hpp
#define DialogManager_hpp

#include "Libs/maf/MafSingleton.h"

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

#include "GameObject/Dialog/InfoDialogScene.h"
#include "GameObject/Dialog/InfoDialogActor.h"
#include "GameObject/Dialog/InfoDialog.h"

class DialogManager : public MafSingleton<DialogManager>
{
private:
    DialogManager(void);
    virtual ~DialogManager(void);
    friend class MafSingleton;
    
public:
    virtual bool init();
    
    const cocos2d::Vector<InfoDialogScene*> getListDialogScene(const std::string sceneName) const;
    InfoDialogActor* const getInfoActor(const int nId) const;
    InfoDialog* const getInfoDialog(const std::string sceneName) const;
    InfoDialog* const getInfoDialogByIdx(const int value) const;
    const cocos2d::Vector<InfoDialog*> getListDialogByType(const std::string type) const;
    
    //load table
    void setLoadAll();
    void setLoadDialogScript();
    void setLoadDialogList();
    void setLoadDialogActor();
    
    // cloud
    void setCloud(std::string strData);
    std::string getCloud();
    
    //utils
    const int isViewDialog(const std::string sceneName);
    void showDialog(const std::string sceneName);
    const bool isDialogOpenException(const int nIdx);
    
    cocos2d::FiniteTimeAction* getAction(const std::string strAction, float durection = 0.5f, cocos2d::Vec2 startPos = cocos2d::Vec2::ZERO, cocos2d::Vec2 endPos = cocos2d::Vec2::ZERO);
private:
    cocos2d::Vector<InfoDialogScene*> _listInfoDialogScene;
    cocos2d::Vector<InfoDialogActor*> _listInfoDialogActor;
    cocos2d::Vector<InfoDialog*> _listInfoDialog;
};
#endif /* DialogManager_hpp */
