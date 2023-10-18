//
//  UICashBox.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/12/03.
//

#ifndef UICashBox_h
#define UICashBox_h

#include "GameUIPopup/Base/PopupBase.h"

class InfoProduct;
class UICashBox : public cocos2d::ui::Layout
{
public:
    static UICashBox* create(int idx, std::string path);
    
    UICashBox(void);
    virtual ~UICashBox(void) {};
    virtual bool init(int idx, std::string path);
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
    std::string getInfoTime(InfoProduct* obj);
    std::string getInfoCount(InfoProduct* obj);
    std::string getInfoPrice(InfoProduct* obj);
    
    // callback
    
    // click
    

private:
    bool _bInitUI;
    
    //
    cocos2d::Size _size;
    
    int _idx;
    std::string _path;
    
    // widget
    cocos2d::ui::Layout* _uiContainerInfo;
    
};
#endif /* UICashBox_h */
