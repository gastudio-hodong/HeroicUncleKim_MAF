//
//  PopupAdventureRelicMain.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/05/04.
//

#include "PopupAdventureRelicDefault.h"

#include "GameUI/UI/UICurrency.h"
#include "GameUI/UI/UIReddot.h"

#include "GameUIPopup/Common/PopupButtonForceTutorial.h"
#include "GameUIPopup/Other/Adventure/Relic/Layer/UILayerAdventureRelicList.h"
#include "GameUIPopup/Other/Adventure/Relic/Layer/UILayerAdventureRelicCrafting.h"
#include "GameUIPopup/Other/Adventure/Relic/Layer/UILayerAdventurePurchase.h"
#include "GameUIPopup/Other/Adventure/PopupAdventureTutorial.h"

#include "ManagerGame/AdventureManager.h"
#include "ManagerGame/AdventureRelicManager.h"

USING_NS_CC;

PopupAdventureRelicDefault* PopupAdventureRelicDefault::create(PopupAdventureRelicDefault::E_TYPE eType)
{
    PopupAdventureRelicDefault* pRet = new(std::nothrow) PopupAdventureRelicDefault();
    if ( pRet && pRet->init(eType) )
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

PopupAdventureRelicDefault::PopupAdventureRelicDefault(void):
_isCloseCondition(false)
,_eType(PopupAdventureRelicDefault::E_TYPE::LIST)
,_lyTopUI(nullptr)
,_lyContentArea(nullptr)
,_btnTutorial(nullptr)
,_lyContainerNow(nullptr)
,_lyContainerList(nullptr)
,_lyContainerCrafting(nullptr)
,_lyContainerShop(nullptr)
{
}

PopupAdventureRelicDefault::~PopupAdventureRelicDefault(void)
{
}

#pragma mark -init
bool PopupAdventureRelicDefault::init(PopupAdventureRelicDefault::E_TYPE eType)
{
    if ( !PopupBase::init())
    {
        return false;
    }
    _eType = eType;
    initVar();
    initUI();
    
    RefreshManager::getInstance()->addUI(E_REFRESH::ADVENTURE, this);
    if(AdventureManager::getInstance()->getTutorialStep() < 7)
        RefreshManager::getInstance()->addUI(E_REFRESH::ADVENTURE_TUTORIAL_COMPLETE, this);
    return true;
}

#pragma mark - override
void PopupAdventureRelicDefault::refresh(E_REFRESH eType, cocos2d::ValueMap listData)
{
    if(eType == E_REFRESH::ADVENTURE_TUTORIAL_COMPLETE)
        this->setLocalZOrder(ZORDER_SCENE_POPUP);
    else if(eType == E_REFRESH::ADVENTURE)
    {
        uiReload();
        showButtonForceTutorial();
    }
}
void PopupAdventureRelicDefault::show()
{
    PopupBase::show();
    
    if(AdventureManager::getInstance()->getTutorialStep() < 7)
        this->setLocalZOrder(ZORDER_SCENE_FORCE_TUTORIAL);
}

#pragma mark - Tutorial
void PopupAdventureRelicDefault::showButtonForceTutorial()
{
    if(_btnTutorial == nullptr || AdventureManager::getInstance()->getTutorialStep() != 4)
        return;
    
    auto popup = PopupButtonForceTutorial::create();
    popup->setButton(_btnTutorial);
    if(AdventureManager::getInstance()->getTutorialStep() == 4)
        popup->setCallbackListener(CC_CALLBACK_1(PopupAdventureRelicDefault::onClickTab, this));
    popup->show();
    
    _btnTutorial = nullptr;
}

void PopupAdventureRelicDefault::showTutorial()
{
    if(AdventureManager::getInstance()->getTutorialStep() != 4)
        return;
    
    auto popup = PopupAdventureTutorial::create(AdventureManager::getInstance()->getTutorialStep());
    popup->show();
}


void PopupAdventureRelicDefault::initVar()
{
    _spriteContainer->setContentSize(Size(735, 1268));
    _spriteContainer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _spriteContainer->setPosition(getContentSize().width / 2, 0);
}

void PopupAdventureRelicDefault::initUI()
{
    // title
    
    auto lbTitle = Label::createWithTTF(GAME_TEXT("t_ui_advt_info_title_7"), GAME_FONT, 28);
    lbTitle->setColor(COLOR_TITLE);
    lbTitle->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5) );
    _spriteContainer->addChild(lbTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupAdventureRelicDefault::onClickClose, this));
    itemClose->setPosition( Vec2(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5) );
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
    
    
    _lyTopUI = LayerColor::create();
    _lyTopUI->setContentSize( Size(_spriteContainer->getContentSize().width - 10, 100));
    _lyTopUI->setIgnoreAnchorPointForPosition(false);
    _lyTopUI->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lyTopUI->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75));
    _spriteContainer->addChild(_lyTopUI);
    
    _lyContentArea = LayerColor::create();
    _lyContentArea->setContentSize( Size(_spriteContainer->getContentSize().width - 10, _spriteContainer->getContentSize().height - 75 - _lyTopUI->getContentSize().height));
    _lyContentArea->setIgnoreAnchorPointForPosition(false);
    _lyContentArea->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lyContentArea->setPosition( _lyTopUI->getPosition() + Vec2::DOWN * _lyTopUI->getContentSize().height);
    _spriteContainer->addChild(_lyContentArea);
    
    uiTop();
    uiMiddle();
}

#pragma mark -init
void PopupAdventureRelicDefault::uiTop()
{
    _lyTopUI->removeAllChildren();
    
    std::vector<PopupAdventureRelicDefault::E_TYPE> listTab = {
        PopupAdventureRelicDefault::E_TYPE::LIST,
        PopupAdventureRelicDefault::E_TYPE::CRAFTING,
        PopupAdventureRelicDefault::E_TYPE::SHOP
    };
    
    int size = (int)listTab.size();
    
    float width = _lyTopUI->getContentSize().width / size - 10;
    float posX = _lyTopUI->getContentSize().width/2 - width * (size/2) - 10 * (size/2);
    
    for(int i = 0; i < listTab.size(); ++i)
    {
        auto obj = listTab[i];
        
        std::string strPath = "Assets/ui_common/btn_tap_2_off.png";
        std::string strIconPath = "Assets/ui/adventure/adventure_relic_category_%d_2.png";
        if(obj == _eType)
        {
            strPath = "Assets/ui_common/btn_tap_2_on.png";
            strIconPath = "Assets/ui/adventure/adventure_relic_category_%d_1.png";
        }
        std::string strText = "";
        switch (obj) {
            case PopupAdventureRelicDefault::E_TYPE::LIST: strText = GAME_TEXT("t_ui_advt_relic_list"); break;
            case PopupAdventureRelicDefault::E_TYPE::CRAFTING: strText = GAME_TEXT("t_ui_advt_info_title_8"); break;
            case PopupAdventureRelicDefault::E_TYPE::SHOP: strText = GAME_TEXT("t_ui_shop_gem"); break;
            default:  break;
        }
        
        auto btnTab = ui::Button::create(strPath);
        btnTab->setScale9Enabled(true);
        btnTab->setContentSize(Size(width, 90));
        btnTab->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        btnTab->setPosition(Vec2(posX, 0));
        btnTab->addClickEventListener(CC_CALLBACK_1(PopupAdventureRelicDefault::onClickTab, this));
        btnTab->setZoomScale(0.05f);
        btnTab->setTag((int)obj);
        _lyTopUI->addChild(btnTab);
        {
            auto sprIcon = Sprite::create(MafUtils::format(strIconPath.c_str(), (int)obj));
            sprIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            sprIcon->setPosition(Vec2(btnTab->getContentSize()/2));
            btnTab->addChild(sprIcon);
            
            auto lbText = Label::createWithTTF(strText, GAME_FONT, 25);
            lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
            lbText->setPosition(btnTab->getContentSize().width/2, btnTab->getContentSize().height/2 - 5);
            lbText->setDimensions(btnTab->getContentSize().width - 10, lbText->getContentSize().height);
            lbText->setAlignment(TextHAlignment::CENTER, TextVAlignment::TOP);
            lbText->setOverflow(Label::Overflow::SHRINK);
            btnTab->addChild(lbText);
            
            if(obj == PopupAdventureRelicDefault::E_TYPE::LIST)
            {
                auto uiReddot = UIReddot::create(E_REDDOT_TYPE::NOMAL, Vec2::ANCHOR_MIDDLE);
                uiReddot->setPosition(10, btnTab->getContentSize().height - 10);
                uiReddot->setVisible(false);
                btnTab->addChild(uiReddot);
                
                auto action1 = CallFuncN::create([=](Ref* sender){
                    auto uiReddot = (UIReddot*)sender;
                    
                    bool isActivate = false;
                    
                    if(AdventureRelicManager::getInstance()->isRelicReddotCondition() ||
                       AdventureRelicManager::getInstance()->isRelicCollectReddotCondition())
                        isActivate = true;
                    
                    uiReddot->setVisible(isActivate);
                });
                auto action2 = DelayTime::create(2);
                auto seq = Sequence::create(action1, action2, NULL);
                uiReddot->runAction(RepeatForever::create(seq));
                
                if(AdventureManager::getInstance()->getTutorialStep() == 4)
                    _btnTutorial = btnTab;
            }
            if(obj != _eType)
            {
                lbText->setColor(Color3B(90,90,90));
            }
        }
        
        if(size % 2 == 0)
            btnTab->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        
        posX += width + 10;
    }
}

void PopupAdventureRelicDefault::uiMiddle()
{
    _lyContentArea->removeAllChildren();
    
    uiLoadLayer();
}

void PopupAdventureRelicDefault::uiReload()
{
    uiTop();
    uiLoadLayer();
}

void PopupAdventureRelicDefault::uiLoadLayer()
{
    if(_lyContainerNow != nullptr)
        _lyContainerNow->setVisible(false);
    
    switch(_eType)
    {
        case PopupAdventureRelicDefault::E_TYPE::LIST:
        {
            if(_lyContainerList == nullptr)
            {
                auto layer = UILayerAdventureRelicList::create(_lyContentArea->getContentSize());
                layer->setCallbackHide(CC_CALLBACK_0(PopupAdventureRelicDefault::callbackHide, this));
                _lyContentArea->addChild(layer);
                //layer->showButtonForceTutorial();
                _lyContainerList = layer;
            }
            _lyContainerNow =  _lyContainerList;

            ((UILayerAdventureRelicList*) _lyContainerNow)->refreshUI();
        }break;
        case PopupAdventureRelicDefault::E_TYPE::CRAFTING:
        {
            bool isCreate = false;
            if(_lyContainerCrafting == nullptr)
            {
                auto layer = UILayerAdventureRelicCrafting::create(_lyContentArea->getContentSize());
                layer->setCallbackHide(CC_CALLBACK_0(PopupAdventureRelicDefault::callbackHide, this));
                layer->setCallbackMoveShop(CC_CALLBACK_1(PopupAdventureRelicDefault::callbackSetTab, this));
                _lyContentArea->addChild(layer);
                layer->showButtonForceTutorial();
                _lyContainerCrafting = layer;
                isCreate = true;
            }
            _lyContainerNow = _lyContainerCrafting;
            
            ((UILayerAdventureRelicCrafting*) _lyContainerNow)->refreshUI();
                
        }break;
        case PopupAdventureRelicDefault::E_TYPE::SHOP:
        {
            if(_lyContainerShop == nullptr)
            {
                auto layer = UILayerAdventurePurchase::create(_lyContentArea->getContentSize());
                _lyContentArea->addChild(layer);

                _lyContainerShop = layer;
            }
            _lyContainerNow = _lyContainerShop;

            ((UILayerAdventurePurchase*) _lyContainerNow)->refreshUI();
        }break;
    }

    _lyContainerNow->setVisible(true);
}

#pragma mark -utils
void PopupAdventureRelicDefault::onClickClose(Ref* sender)
{
    if(AdventureManager::getInstance()->getTutorialStep() == 5)
        RefreshManager::getInstance()->refreshUI(E_REFRESH::ADVENTURE);
    hide();
}

void PopupAdventureRelicDefault::onClickTab(Ref* sender)
{
    auto node = (Node*)sender;
    _eType = (PopupAdventureRelicDefault::E_TYPE)node->getTag();
    
    uiReload();
}

void PopupAdventureRelicDefault::setCloseCondition(bool value)
{
    _isCloseCondition = value;
    
}

void PopupAdventureRelicDefault::callbackHide()
{
    hide();
}

void PopupAdventureRelicDefault::callbackSetTab(PopupAdventureRelicDefault::E_TYPE value)
{
    _eType = value;
    
    uiReload();
}
