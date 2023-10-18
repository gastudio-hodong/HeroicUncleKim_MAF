//
//  PopupSupporters.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 25/06/2019.
//

#ifndef PopupSupporters_hpp
#define PopupSupporters_hpp

#include "GameUIPopup/Base/PopupBase.h"

#include "GameUI/UICell/ModelCell.h"
#include "GameUI/UICell/ModelCellDevil.h"

class PopupSupporters : public PopupBase
{
public:
    static PopupSupporters* create();
    PopupSupporters(void);
    virtual ~PopupSupporters(void);
    virtual bool init();
    
    void onClickClose(Ref *sender);
    
    
protected:
    
    void initUi();
    
    

    
};
#endif /* PopupSupporters_hpp */
