//
//  PopupHelp.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/05/17.
//

#ifndef PopupHelp_hpp
#define PopupHelp_hpp

#include "GameUIPopup/Base/PopupBase.h"

class PopupHelp : public PopupBase
{
public:
    static PopupHelp* create(const int nMaxPage, const std::string strTitle, const std::string strContent);
    
    PopupHelp(void);
    virtual ~PopupHelp(void);
    
    virtual bool init(const int nMaxPage, const std::string strTitle, const std::string strContent);
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiContents();
    void uiRedraw();
    
    // click
    void onClick(cocos2d::Ref* sender);
    
    void onClickPage(int nPage);
private:
    int _nMaxPage;
    int _nCurrentPage;
    std::string _strTitle;
    std::string _strContent;
    
    cocos2d::Layer* _layerContainerContents;
    cocos2d::Layer* _lyTextContainer;
    
    cocos2d::extension::ScrollView* _table;
    
    cocos2d::Label* _lbRuleContent;
    cocos2d::Label* _lbRuleTitle;
    
    cocos2d::Vector<MafNode::MafMenuItemSprite*> _vecItemPage;
};

#endif /* PopupHelp_hpp */
