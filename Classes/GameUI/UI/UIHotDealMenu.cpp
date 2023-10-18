//
//  UIHotDealMenu.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#include "UIHotDealMenu.h"

#include "GameUI/UI/UIBannerPurchase.h"
#include "GameUI/UI/UIFirstPurchase.h"
#include "GameUI/UI/UIReturnPurchase.h"
#include "GameUI/UI/UIGrowthPurchase.h"

#include "GameUIPopup/Other/TimingPackage/UITimingPackageBtn.h"

#include "ManagerEvent/EventManager.h"
#include "ManagerGame/CashManager.h"

USING_NS_CC;

UIHotDealMenu* UIHotDealMenu::create(E_PLACE eType)
{
    UIHotDealMenu* pRet = new(std::nothrow) UIHotDealMenu();
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

UIHotDealMenu::UIHotDealMenu(void) :
_eType(E_PLACE::EMPTY),
_bExpand(false),
_zorder(0)
{
}

UIHotDealMenu::~UIHotDealMenu(void)
{
}

bool UIHotDealMenu::init(E_PLACE eType)
{
    if ( !BaseTimingPackageManagement::init() )
    {
        return false;
    }
    
    //
    setIgnoreAnchorPointForPosition(false);
    setAnchorPoint( Vec2(0.5f, 0.5f) );
    setContentSize(Size(64, 64));
    
    //
    _eType = eType;
    
    // init
    initVar();
    initUi();
    
    initTimingPackage();
    
    return true;
}
 
#pragma mark - init
void UIHotDealMenu::initVar()
{
}

void UIHotDealMenu::initUi()
{
    Size size = getContentSize();
    
    _uiContainer = ui::Layout::create();
    _uiContainer->setBackGroundImage("Assets/ui/main/main_side_menu_listbox.png");
    _uiContainer->setBackGroundImageScale9Enabled(true);
    _uiContainer->setBackGroundImageCapInsets(Rect(8, 8, 2, 44));
    _uiContainer->setAnchorPoint( Vec2::ANCHOR_MIDDLE_RIGHT );
    _uiContainer->setPosition(Vec2(size.width - 64, size.height / 2));
    addChild(_uiContainer);
    
    auto uiBtnExpand = ui::Layout::create();
    uiBtnExpand->setContentSize(Size(64, 64));
    uiBtnExpand->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    uiBtnExpand->setPosition(Vec2(size.width, size.height / 2));
    uiBtnExpand->addClickEventListener(CC_CALLBACK_1(UIHotDealMenu::onClickExpand, this));
    uiBtnExpand->setTouchEnabled(true);
    addChild(uiBtnExpand);
    {
        auto uiImgIcon = ui::ImageView::create("Assets/ui/main/icon_hot_00.png");
        uiImgIcon->setAnchorPoint(Vec2::ZERO);
        uiImgIcon->setPosition(Vec2::ZERO);
        uiBtnExpand->addChild(uiImgIcon);
        
        auto aniframeRun = Animation::create();
        aniframeRun->setDelayPerUnit(0.1f);
        for ( int i = 0; i <= 7; i++ )
        {
            aniframeRun->addSpriteFrameWithFile(MafUtils::format("Assets/ui/main/icon_hot_%02d.png", i));
        }
        aniframeRun->addSpriteFrameWithFile("Assets/ui/main/icon_hot_00.png");
        
        auto action1 = Animate::create(aniframeRun);
        auto action2 = DelayTime::create(0.5);
        auto seq = Sequence::create(action1, action2, NULL);
        uiImgIcon->getVirtualRenderer()->runAction(RepeatForever::create(seq));
        
        auto uiTextName = ui::CText::create(GAME_TEXT("t_ui_deals"), GAME_FONT, 16);
        uiTextName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        uiTextName->setPosition(Vec2(uiBtnExpand->getContentSize().width / 2, 20));
        uiTextName->enableOutline(Color4B::BLACK, 2);
        uiTextName->setTextAreaSize(Size(uiBtnExpand->getContentSize().width, uiTextName->getContentSize().height));
        uiTextName->setTextOverflow(Label::Overflow::SHRINK);
        uiTextName->setTextHorizontalAlignment(TextHAlignment::CENTER);
        uiBtnExpand->addChild(uiTextName);
        
        auto uiReddot = UIReddot::create(E_REDDOT_TYPE::NOMAL);
        uiReddot->setPosition(uiImgIcon->getContentSize().width - 30, uiImgIcon->getContentSize().height - 30);
        uiImgIcon->addChild(uiReddot);
        uiReddot->setVisible(false);
        auto func = CallFuncN::create([=](Ref* sender){
                
            auto uiReddot = (UIReddot*)sender;
                
            if(_listUITimingbtn.size() > 0)
            {
                uiReddot->setVisible(true);
            }
            else
            {
                uiReddot->setVisible(false);
            }
        });
        uiReddot->runAction(RepeatForever::create(Sequence::create(func, DelayTime::create(1), NULL)));
    }
    
    //
    setMenu();
    drawContainer();
}

#pragma mark - ui
void UIHotDealMenu::drawContainer()
{
    _uiContainer->stopAllActions();
    _uiContainer->removeAllChildren();
    if ( _bExpand == false )
    {
        if ( _uiContainer->isVisible() == false )
        {
            return;
        }
        
        setLocalZOrder(_zorder);
        
        //
        auto action1 = ResizeTo::create(0.1, Size(0, 60));
        auto action2 = Hide::create();
        auto seq = Sequence::create(action1, action2, NULL);
        _uiContainer->runAction(seq);
        
        //
        auto uiContainerTouchHide = getChildByName<ui::Layout*>("UI_LAYOUT_HIDE");
        if ( uiContainerTouchHide != nullptr )
        {
            uiContainerTouchHide->removeFromParent();
        }
    }
    else
    {
        _zorder = getLocalZOrder();
        setLocalZOrder(9999);
        
        //
        Size size = Size(66, 66);
        size.width += _widthExpand;
        size.width += 20 * (_listMenu.size() - 1);
        
        if ( _uiContainer->isVisible() == true )
        {
            _uiContainer->setContentSize(Size(size.width, size.height));
            drawExpand();
        }
        else
        {
            auto actionShow = Show::create();
            auto action1 = ResizeTo::create(0.1, Size(size.width * 1.1, size.height));
            auto action2 = ResizeTo::create(0.1, Size(size.width * 1.1, size.height * 1.1));
            auto action3 = ResizeTo::create(0.1, Size(size.width, size.height));
            auto action4 = ResizeTo::create(0.1, Size(size.width, size.height * 1.1));
            auto action5 = ResizeTo::create(0.1, Size(size.width, size.height));
            auto actionDraw = CallFunc::create(CC_CALLBACK_0(UIHotDealMenu::drawExpand, this));
            auto seq = Sequence::create(actionShow, action1, action2, action3, actionDraw, action4, action5, NULL);
            _uiContainer->runAction(seq);
            
            //
            Size sizeVisible = UtilsRect::getVisibleSize();
            Vec2 pos = this->convertToNodeSpace(Vec2::ZERO);
           
            auto uiContainerTouchHide = ui::Layout::create();
            uiContainerTouchHide->setContentSize(sizeVisible);
            uiContainerTouchHide->setAnchorPoint(Vec2::ZERO);
            uiContainerTouchHide->setPosition(pos);
            uiContainerTouchHide->setName("UI_LAYOUT_HIDE");
            uiContainerTouchHide->setTouchEnabled(true);
            uiContainerTouchHide->addTouchEventListener([=](cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type){
                auto uiContainerTouchHide = (ui::Layout*)sender;
                uiContainerTouchHide->removeFromParent();
                
                if ( type == ui::Widget::TouchEventType::BEGAN )
                {
                    onExpand();
                }
            });
            addChild(uiContainerTouchHide, -1);
        }
    }
}

void UIHotDealMenu::drawExpand()
{
    double padding = 20;
    double posX = _uiContainer->getContentSize().width - 33;
    double posY = 15;

    for ( int i = 0; i < _listMenu.size(); i++ )
    {
        auto uiItem = _listMenu.at(i);
        if(uiItem->getName().compare("TIMING") == 0)
        {
            auto btnTimingPackage = dynamic_cast<UITimingPackageBtn*>(uiItem);
            if ( btnTimingPackage != nullptr )
            {
                if(btnTimingPackage->getParent() != nullptr)
                    btnTimingPackage->removeFromParent();
                
                if(btnTimingPackage->isPurchase())
                    btnTimingPackage->scheduleUpdate();
                
                if(_bExpand)
                {
                    btnTimingPackage->setVisibleTimingPackageBtn(true);
                }
            }
        }
        
        uiItem->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        uiItem->setPosition(Vec2(posX, posY));
        _uiContainer->addChild(uiItem);
        
        auto uiTemp = ui::Layout::create();
        uiTemp->setContentSize(uiItem->getContentSize());
        uiTemp->setAnchorPoint(Vec2::ZERO);
        uiTemp->setPosition(Vec2::ZERO);
        uiItem->addChild(uiTemp, -1);

        posX -= uiItem->getContentSize().width;
        posX -= padding;
    }
}

#pragma mark - set, get
void UIHotDealMenu::setMenu()
{
    _listMenu.clear();
    
    // 배너 상품
    if ( EventManager::getInstance()->isActivatedEvent(21) == true || EventManager::getInstance()->isActivatedEvent(22) == true )
    {
        auto uiItem = UIBannerPurchase::create();
        uiItem->setCallbackReload(CC_CALLBACK_0(UIHotDealMenu::callbackReload, this));
        _listMenu.pushBack(uiItem);
    }
    
    // 성장 패키지 상품
    {
        auto uiItem = UIGrowthPurchase::create();
        uiItem->setCallbackReload(CC_CALLBACK_0(UIHotDealMenu::callbackReload, this));
        _listMenu.pushBack(uiItem);
    }
    
    // 첫결제 상품
    {
        int nResult = CashManager::getInstance()->getFirstBonusResult();
        if ( nResult == 0 || nResult == 1 )
        {
            auto uiItem = UIFirstPurchase::create();
            uiItem->setCallbackReload(CC_CALLBACK_0(UIHotDealMenu::callbackReload, this));
            _listMenu.pushBack(uiItem);
        }
    }
    
    // 복귀 상품
    if ( EventManager::getInstance()->isActivatedEvent(14) == true )
    {
        auto uiItem = UIReturnPurchase::create();
        uiItem->setCallbackReload(CC_CALLBACK_0(UIHotDealMenu::callbackReload, this));
        _listMenu.pushBack(uiItem);
    }
    
    // 타이밍 패키지
    {
        for(auto obj : _listUITimingbtn)
        {
            obj->removeFromParent();
            if(obj->isPurchase())
                obj->scheduleUpdate();
            
            obj->setVisibleTimingPackageBtn(false);
            obj->setName("TIMING");
            addChild(obj);
            
            _listMenu.pushBack(obj);
        }
    }
    
    _widthExpand = 0;
    for (auto& node : _listMenu )
    {
        _widthExpand += node->getContentSize().width;
    }
}
#pragma mark - schedule

#pragma mark - callback
void UIHotDealMenu::callbackReload()
{
    if ( _bExpand == true )
    {
        setMenu();
        drawContainer();
    }
}

#pragma mark - click
void UIHotDealMenu::onClickExpand(cocos2d::Ref* sender)
{
    onExpand();
}

#pragma mark - event
void UIHotDealMenu::onExpand()
{
    _bExpand = !_bExpand;
    setMenu();
    drawContainer();
}
