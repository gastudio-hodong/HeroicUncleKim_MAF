//
//  PopupPetDevil.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 12/06/2019.
//

#ifndef PopupPetDevil_hpp
#define PopupPetDevil_hpp

#include "GameUIPopup/Base/PopupBase.h"

#include "GameUnit/Pet.h"

#include "GameUI/UICell/ModelCell.h"
#include "GameUI/UICell/ModelCellDevil.h"

class PopupPetDevil : public PopupBase
{
public:
    static PopupPetDevil* create(ModelCellDevil::CellDevilPet &celldevil);
    PopupPetDevil(void);
    virtual ~PopupPetDevil(void);
    virtual bool init(ModelCellDevil::CellDevilPet &celldevil);

public:
    ModelCellDevil::CellDevilPet *_selectPetDevil;
    
protected:
    
    void initUi(ModelCellDevil::CellDevilPet &celldevil);
    void initBottomUi(std::string strSkill);
    
    //
    void showPopupPetGrade(cocos2d::Ref* pSender);
    void showPromoExplainPopup(cocos2d::Ref* pSender);
    
    // set, get
    
    //
    void onClickClose(Ref *sender);
    void switchMenu(Ref* sender);
    
    // callback
    
    // network
    void requestCheckCostumeCoin();
    void ResultCheckCostumeCoin(cocos2d::network::HttpResponse* response,std::string &data);
    
private:
    E_PET _ePet;
    
    cocos2d::Vector<MafNode::MafMenuItemSprite*> _vecButton;
    cocos2d::Vector<cocos2d::Layer*> _vecLayer;
    
    cocos2d::Layer* _layerContainerParent;
    cocos2d::Layer* _containerRein;
        
    Pet* _objPet;
    cocos2d::Sprite* _sprBack;
    cocos2d::Label* _lbGuide2;
    cocos2d::Label* _lbGuideExplain2;
    
    std::string strPromoExplain;
    int _nSkinBuy;
    
};
#endif
