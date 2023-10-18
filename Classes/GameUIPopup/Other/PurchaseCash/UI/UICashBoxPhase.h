//
//  UICashBoxPhase.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/12/03.
//

#ifndef UICashBoxPhase_h
#define UICashBoxPhase_h

#include "GameUIPopup/Base/PopupBase.h"

class InfoProduct;
class UICashBoxPhase : public cocos2d::ui::Layout
{
public:
    static UICashBoxPhase* create(int idx, int idxPrev, std::string pathEnable, std::string pathDisable);
    
    UICashBoxPhase(void);
    virtual ~UICashBoxPhase(void) {};
    virtual bool init(int idx, int idxPrev, std::string pathEnable, std::string pathDisable);
    virtual void onEnter() override;
    
public:
    
    
protected:
    // init
    void initVar();
    void initUI();
    
    // ui
    void uiContainer();
    void uiInfo();
    
    // ui draw
    
    
    // set, get
    std::string getInfoPrice(InfoProduct* obj);
    
    // callback
    
    // click
    

private:
    bool _bInitUI;
    
    //
    cocos2d::Size _size;
    
    int _idx;
    int _idxPrev;
    std::string _pathEnable;
    std::string _pathDisable;
    
    // widget
    cocos2d::ui::Layout* _uiContainerInfo;
    
};
#endif /* UICashBoxPhase_h */
