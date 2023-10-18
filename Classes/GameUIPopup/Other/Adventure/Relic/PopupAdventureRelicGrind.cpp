//
//  PopupAdventureRelicGrind.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/05/13.
//

#include "PopupAdventureRelicGrind.h"

#include "Common/ConfigGameNormal.h"

#include "GameUIPopup/Other/Ads/PopupAdsMafGames.h"
#include "GameUIPopup/Other/CurrencyDetail/PopupCurrencyDetail.h"

#include "ManagerGame/AdventureManager.h"
#include "ManagerGame/AdventureRelicManager.h"

USING_NS_CC;

PopupAdventureRelicGrind* PopupAdventureRelicGrind::create(const int nIdx)
{
    PopupAdventureRelicGrind* pRet = new(std::nothrow) PopupAdventureRelicGrind();
    if ( pRet && pRet->init(nIdx) )
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

PopupAdventureRelicGrind::PopupAdventureRelicGrind(void) :
_nIdx(0)
,_nCount(0)
,_callbackRefresh(nullptr)
,_infoCurrentRelic(nullptr)
,_layerContainerContents(nullptr)
,_layerContainerButton(nullptr)
,_lbGrind(nullptr)
{
    
}

PopupAdventureRelicGrind::~PopupAdventureRelicGrind(void)
{
    
}

bool PopupAdventureRelicGrind::init(const int nIdx)
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    _nIdx = nIdx;
    _infoCurrentRelic = AdventureRelicManager::getInstance()->getInfoRelic(_nIdx);
    if(_infoCurrentRelic->getCount() > 0)
        _nCount = 1;
    // init
    initVar();
    initUi();
    if(AdventureManager::getInstance()->getTutorialStep() < 7)
        RefreshManager::getInstance()->addUI(E_REFRESH::ADVENTURE_TUTORIAL_COMPLETE, this);
    
    return true;
}
#pragma mark - Override
void PopupAdventureRelicGrind::refresh(E_REFRESH eType, cocos2d::ValueMap listData)
{
    if(eType == E_REFRESH::ADVENTURE_TUTORIAL_COMPLETE)
        this->setLocalZOrder(ZORDER_SCENE_POPUP);
}
void PopupAdventureRelicGrind::show()
{
    PopupBase::show();
    
    if(AdventureManager::getInstance()->getTutorialStep() < 7)
        this->setLocalZOrder(ZORDER_SCENE_FORCE_TUTORIAL);
}
#pragma mark - init
void PopupAdventureRelicGrind::initVar()
{

}

void PopupAdventureRelicGrind::initUi()
{
    Size size = Size(650, 500);
    
    //
    _spriteContainer->setContentSize( size );
    
    //
    _layerContainerButton = LayerColor::create();
    _layerContainerButton->setContentSize( Size(size.width - 50, 130) );
    _layerContainerButton->setIgnoreAnchorPointForPosition(false);
    _layerContainerButton->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _layerContainerButton->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, 15) );
    _spriteContainer->addChild(_layerContainerButton);
    
    _layerContainerContents = LayerColor::create();
    _layerContainerContents->setContentSize( Size(size.width - 50, size.height - 75 - 25 - 25 - _layerContainerButton->getContentSize().height - 25) );
    _layerContainerContents->setIgnoreAnchorPointForPosition(false);
    _layerContainerContents->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerContents->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75 - 25) );
    _spriteContainer->addChild(_layerContainerContents);
    
    // title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_advt_relic_grind_title"), GAME_FONT, 28);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5) );
    _spriteContainer->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupAdventureRelicGrind::onClickClose, this));
    itemClose->setPosition( Vec2(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5) );
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
    
    uiContents();
    uiButton();
}

#pragma mark - get set
void PopupAdventureRelicGrind::setCallbackRefresh(const std::function<void(void)>& _callback)
{
    _callbackRefresh = _callback;
}

#pragma mark - ui
void PopupAdventureRelicGrind::uiContents()
{
    _layerContainerContents->removeAllChildren();
    
    auto lbContent = Label::createWithTTF(GAME_TEXTFORMAT("t_ui_advt_relic_grind_msg", GAME_TEXT(MafUtils::format("t_ui_advt_relic_name_%d", _nIdx)).c_str()), GAME_FONT, 25);
    lbContent->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    lbContent->setPosition(_layerContainerContents->getContentSize().width/2, _layerContainerContents->getContentSize().height - 20);
    lbContent->setDimensions(_layerContainerContents->getContentSize().width, _layerContainerContents->getContentSize().height/2);
    lbContent->setOverflow(cocos2d::Label::Overflow::SHRINK);
    lbContent->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::TOP);
    _layerContainerContents->addChild(lbContent);
    
    auto sprIconCore = Sprite::create("Assets/icon/icon_core_06.png");
    sprIconCore->getTexture()->setTexParameters(texParams);
    sprIconCore->setScale(2);
    sprIconCore->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    sprIconCore->setPosition(_layerContainerContents->getContentSize().width/2 - 10, _layerContainerContents->getContentSize().height/2);
    _layerContainerContents->addChild(sprIconCore);
    
    auto objItem = _infoCurrentRelic->getListGrindReward().at(0);
    
    auto lbCount = Label::createWithTTF(MafUtils::format(": %02d", std::atoi(objItem->getCount().c_str()) * _nCount), GAME_FONT, 35);
    lbCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    lbCount->setPosition(_layerContainerContents->getContentSize().width/2 -10, _layerContainerContents->getContentSize().height/2 - 55);
    lbCount->setDimensions(_layerContainerContents->getContentSize().width/2, lbCount->getContentSize().height);
    lbCount->setOverflow(cocos2d::Label::Overflow::SHRINK);
    lbCount->setAlignment(cocos2d::TextHAlignment::LEFT, cocos2d::TextVAlignment::CENTER);
    _layerContainerContents->addChild(lbCount);
}

void PopupAdventureRelicGrind::uiButton()
{
    _layerContainerButton->removeAllChildren();
    
    
    auto btnRankUp = ui::Button::create("Assets/ui_common/btn_ok.png");
    btnRankUp->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    btnRankUp->setPosition(Vec2(_layerContainerButton->getContentSize()/2));
    btnRankUp->setZoomScale(0.05f);
    btnRankUp->addClickEventListener(CC_CALLBACK_1(PopupAdventureRelicGrind::onClickGrind, this));
    btnRankUp->setTitleText(GAME_TEXTFORMAT("t_ui_advt_relic_grind_1", _nCount));
    btnRankUp->setTitleAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
    btnRankUp->getTitleLabel()->setDimensions(btnRankUp->getContentSize().width, btnRankUp->getContentSize().height);
    btnRankUp->setTitleFontSize(30);
    _layerContainerButton->addChild(btnRankUp);
    
    _lbGrind = btnRankUp->getTitleLabel();
    
    auto btnPlus = ui::Button::create("Assets/ui_common/btn_updown_1_1.png");
    btnPlus->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    btnPlus->setPosition(btnRankUp->getPosition() + Vec2::RIGHT * (btnRankUp->getContentSize().width/2 + 15));
    btnPlus->setZoomScale(0.05f);
    btnPlus->setName("PLUS");
    btnPlus->addClickEventListener(CC_CALLBACK_1(PopupAdventureRelicGrind::onClickFluctuation, this));
    _layerContainerButton->addChild(btnPlus);
    
    auto btnMinus = ui::Button::create("Assets/ui_common/btn_updown_1_2.png");
    btnMinus->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    btnMinus->setPosition(btnRankUp->getPosition() + Vec2::LEFT * (btnRankUp->getContentSize().width/2 + 15));
    btnMinus->setZoomScale(0.05f);
    btnMinus->setName("MINUS");
    btnMinus->addClickEventListener(CC_CALLBACK_1(PopupAdventureRelicGrind::onClickFluctuation, this));
    _layerContainerButton->addChild(btnMinus);
}

#pragma mark - utils
void PopupAdventureRelicGrind::onClickClose(cocos2d::Ref* sender)
{
    hide();
}

void PopupAdventureRelicGrind::onClickGrind(cocos2d::Ref* sender)
{
    auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_advt_relic_grind_msg_2"));
    popup->addButton(GAME_TEXT("t_ui_advt_relic_list_bt_grind"), true, [&](Ref*){
        PopupLoading::show();
        
        AdventureRelicManager::getInstance()->requestRelicGrind(CC_CALLBACK_2(PopupAdventureRelicGrind::callbackGrind, this), _infoCurrentRelic->getIdx(), _nCount);
    });
    popup->addButton(GAME_TEXT("t_ui_close"), true, nullptr);
    popup->show();
    if(AdventureManager::getInstance()->getTutorialStep() < 7)
        popup->setLocalZOrder(ZORDER_SCENE_FORCE_TUTORIAL);
}

void PopupAdventureRelicGrind::onClickFluctuation(Ref* sender)
{
    auto node = (Node*)sender;
    
    if(node->getName().compare("PLUS") == 0)
    {
        _nCount++;
        
        if(_nCount > _infoCurrentRelic->getCount())
            _nCount = 1;
    }
    else if(node->getName().compare("MINUS") == 0)
    {
        _nCount--;
        
        if(_nCount < 1)
            _nCount = _infoCurrentRelic->getCount();
    }
    
    std::string strNowInfoCount = "%d{#EF1F1F:(-%d)}";
    std::string strNextInfoCount = "%d{#018123:(+%d)}";
    
    _lbGrind->setString(GAME_TEXTFORMAT("t_ui_advt_relic_grind_1", _nCount));
    uiContents();
}

void PopupAdventureRelicGrind::callbackGrind(bool bResult, int nResult)
{
    PopupLoading::hide();
    
    if(nResult != 1)
    {
        std::string strMessageKey = "t_ui_error_30";
        
        if(nResult == 7)
            strMessageKey = "t_ui_advt_error_2";
        else if(nResult == 8)
            strMessageKey = "t_ui_advt_error_3";
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT(strMessageKey));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        if(AdventureManager::getInstance()->getTutorialStep() < 7)
            popup->setLocalZOrder(ZORDER_SCENE_FORCE_TUTORIAL);
        
        if(nResult == 7 || nResult == 8)
        {
            ValueMap listData;
            listData["inspection_code"] = Value(nResult);
            RefreshManager::getInstance()->refreshUI(E_REFRESH::ADVENTURE, listData);
        }
        
        hide();
        return;
    }
    
    RefreshManager::getInstance()->refreshUI(E_REFRESH::CURRENCY);
    
    if(AdventureManager::getInstance()->getTutorialStep() > 5)
        RefreshManager::getInstance()->refreshUI(E_REFRESH::ADVENTURE);
    else
    {
        if(_callbackRefresh != nullptr)
            _callbackRefresh();
    }
    
    hide();
}
