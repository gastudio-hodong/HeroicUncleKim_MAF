//
//  PopupCloudSave.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 28/06/2019.
//

#ifndef PopupCloudSave_hpp
#define PopupCloudSave_hpp

#include "GameUIPopup/Base/PopupBase.h"

class PopupCloudSaveLoad : public PopupBase
{
public:
    static PopupCloudSaveLoad* create(bool mode);
    PopupCloudSaveLoad(void);
    virtual ~PopupCloudSaveLoad(void);
    virtual bool init(bool mode);
    
    void onClickClose(Ref *sender);
    
    
protected:
    // init
    void initUi(bool mode);
    
    // network
    void requestCloudSaveData(void);
    void responseCloudSaveData(cocos2d::network::HttpResponse* response,std::string &data);
    void requestCloudLoadData(void);
    void responseCloudLoadData(cocos2d::network::HttpResponse* response,std::string &data);
    
    // callback
    void callbackSave(bool bResult);
    void callbackLoad(bool bResult, bool bData);
    
private:
    
};

#endif /* PopupCloudSave_hpp */
