//
//  PopupToast.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 6. 4..
//

#ifndef PopupToast_h
#define PopupToast_h

#include "Common/ConfigDefault.h"

class PopupToast : public cocos2d::Layer
{
public:
    static PopupToast* create(std::string strMessage, double nPosX, double nPosY);
    
    PopupToast(void);
    virtual ~PopupToast(void);
    virtual bool init(std::string strMessage, double nPosX, double nPosY);
    
public:
    static void show(std::string strMessage, double nPosX = 0, double nPosY = 0);
    static void showImmediately(std::string strMessage, double nPosX = 0, double nPosY = 0);
    
protected:
    // ui
    void uiToast();
    
private:
    //
    struct elementToast
    {
        std::string strMessage;
        double nPosX;
        double nPosY;
    };
    
    //
    std::string _strMessage;
    double _nPosX;
    double _nPosY;
    static std::vector<elementToast> _listToast;
    static std::vector<elementToast> _listToastImmediately;
    
};

#endif /* PopupToast_h */
