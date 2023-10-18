//
//  PopupCostumeReview.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#ifndef PopupCostumeReview_h
#define PopupCostumeReview_h

#include "GameUIPopup/Base/PopupBase.h"

#include "GameUI/UI/UIComboBox.h"

class InfoReviewCostume;
class PopupCostumeReview : public PopupBase, public cocos2d::extension::TableViewDataSource, public cocos2d::extension::TableViewDelegate
{
public:
    enum eZorder {
        zorder_layer_top = 10,
        zorder_layer_costume = 11,
        zorder_layer_list = 12,
        zorder_layer_my = 13,
    };

public:
    static PopupCostumeReview* create(E_COSTUME eType, E_COSTUME_IDX eIdx);
    
    PopupCostumeReview(void);
    virtual ~PopupCostumeReview(void);
    virtual bool init(E_COSTUME eType, E_COSTUME_IDX eIdx);
    virtual void onEnter();
    
    // table TableViewDelegate
    virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view);
    virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell);
    
    // table TableViewDataSource
    virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx);
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table);
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiReviewList();
    void uiReviewMy();
    
    // set, get, sort, add
    bool sortList(cocos2d::Ref *a, cocos2d::Ref *b);
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickReviewRegister(cocos2d::Ref* sender);
    void onClickReviewUnRegister(cocos2d::Ref* sender);
    void onClickReviewRecommend(cocos2d::Ref* sender);
    void onClickReviewWarring(cocos2d::Ref* sender);
    
    // network
    void requestList();
    void responseList(cocos2d::network::HttpResponse* response,std::string &data);
    void requestBest();
    void responseBest(cocos2d::network::HttpResponse* response,std::string &data);
    
    // network - 나의 리뷰 정보
    void requestMyReviewInfo();
    void responseMyReviewInfo(cocos2d::network::HttpResponse* response,std::string &data);
    void requestMyRecommendList();
    void responseMyRecommendList(cocos2d::network::HttpResponse* response,std::string &data);
    
    // network - 리뷰 추천
    void requestRecommend();
    void responseRecommend(cocos2d::network::HttpResponse* response,std::string &data);
    void requestUnRecommend();
    void responseUnRecommend(cocos2d::network::HttpResponse* response,std::string &data);
    
    // network - 리뷰 등록
    void requestRegister(std::string strRating, std::string strContests);
    void responseRegister(cocos2d::network::HttpResponse* response,std::string &data);
    void requestUnRegister();
    void responseUnRegister(cocos2d::network::HttpResponse* response,std::string &data);
    
    
private:
    E_COSTUME _eType;
    E_COSTUME_IDX _eIdx;
    
    InfoReviewCostume* _objReviewMy;
    cocos2d::Vector<InfoReviewCostume*> _listReview;
    cocos2d::ValueVector _listMyRecommend;
    
    bool _isRequestReviewList;
    bool _isRequestReviewListWaiting;
    int _nListReviewIdx;        // 추천하는 리뷰 idx
    
    std::string _strLanguage;
    
    // widget
    cocos2d::Layer* _layerReveiwList;
    cocos2d::Layer* _layerReveiwMy;
    cocos2d::Label* _labelListNothing;
    UIComboBox* _cbRating;
    cocos2d::ui::EditBox* _edit;
    cocos2d::extension::TableView *_table;
};

#endif /* PopupCostumeReview_h */
