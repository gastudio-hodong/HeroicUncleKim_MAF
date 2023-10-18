//
//  PopupBonsikDexExplain.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 26/06/2019.
//

#ifndef PopupBonsikDexExplain_hpp
#define PopupBonsikDexExplain_hpp

#include "GameUIPopup/Base/PopupBase.h"

#include "GameUI/UICell/ModelCell.h"

class PopupBonsikDexExplain : public PopupBase
{
public:
    enum E_EXPLAIN_CATEGORY
    {
        TEXT,
        IMAGE
    };
    
public:
    static PopupBonsikDexExplain* create();
    
    PopupBonsikDexExplain(void);
    virtual ~PopupBonsikDexExplain(void);
    virtual bool init(void);
    
    
protected:
    void initUI();
    void onClickClose(cocos2d::Ref* sender);
    
};
#endif /* PopupBonsikDexExplain_hpp */
