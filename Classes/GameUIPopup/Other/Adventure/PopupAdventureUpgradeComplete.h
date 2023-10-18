//
//  PopupAdventureUpgradeComplete.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/04/25.
//

#ifndef PopupAdventureUpgradeComplete_hpp
#define PopupAdventureUpgradeComplete_hpp

#include "GameUIPopup/Base/PopupBase.h"
#include "Common/Observer/IRefresh.h"

#include "GameObject/Adventure/InfoAdventureFurniture.h"
#include "GameObject/Adventure/InfoAdventureMember.h"
#include "GameObject/Adventure/InfoAdventureRelic.h"

class PopupAdventureUpgradeComplete : public PopupBase, public IRefresh
{
    enum class E_TYPE
    {
        NONE = 0,
        OFFICE,
        MEMBER,
        RELIC,
    };
public:
    
    static PopupAdventureUpgradeComplete* create(InfoAdventureFurniture* infoFurniture);
    static PopupAdventureUpgradeComplete* create(InfoAdventureMember* infoMember);
    static PopupAdventureUpgradeComplete* create(InfoAdventureRelic* infoRelic);
    
    PopupAdventureUpgradeComplete();
    virtual ~PopupAdventureUpgradeComplete(void);
    virtual bool init(InfoAdventureFurniture* infoFurniture);
    virtual bool init(InfoAdventureMember* infoMember);
    virtual bool init(InfoAdventureRelic* infoMember);
    virtual void show() override;
    virtual void refresh(E_REFRESH eType, cocos2d::ValueMap listData)  override;
    
    void setCallbackHide(const std::function<void(void)>& callback);
    
    void setIsTutorial(const bool isTutorial);
protected:
    
    // init
    void initUi();
    
    // ui
    void uiContentsFurniture();
    void uiContentsMember();
    void uiContentsRelic();
    //click
    void onClickClose(Ref* sender);
    
private:
    bool _isTutorial;
    E_TYPE _eType;
    InfoAdventureFurniture* _infoFurniture;
    InfoAdventureMember* _infoMember;
    InfoAdventureRelic* _infoRelic;
    
    // widget
    cocos2d::Layer* _layerContainerContents;
    
    std::function<void(void)> _callbackHide;
};

#endif /* PopupAdventureUpgradeComplete_hpp */
