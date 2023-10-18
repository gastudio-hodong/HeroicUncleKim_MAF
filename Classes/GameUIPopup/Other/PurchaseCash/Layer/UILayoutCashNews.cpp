//
//  UILayoutCashNews.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/12/03.
//

#include "UILayoutCashNews.h"

#include "Common/ConfigGameNormal.h"

#include "GameObject/InfoSetting.h"

#include "ManagerGame/CashManager.h"

USING_NS_CC;

UILayoutCashNews* UILayoutCashNews::create()
{
    UILayoutCashNews* pRet = new(std::nothrow) UILayoutCashNews();
    if ( pRet && pRet->init() )
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

UILayoutCashNews::UILayoutCashNews():
_bInitUI(false),

//
_time(0),

//
_uiContainerInfo(nullptr),
_uiList(nullptr)
{
    
}

bool UILayoutCashNews::init()
{
    if ( !ui::Layout::init() )
    {
        return false;
    }
    
    //
    initVar();
    initUI();
    
    return true;
}


void UILayoutCashNews::onEnter()
{
    ui::Layout::onEnter();
    
    if ( _bInitUI == false )
    {
        //
        uiContainer();
        uiInfo();
        

        //
        CashManager::getInstance()->requestNewsInfo(CC_CALLBACK_1(UILayoutCashNews::callbackInfo, this));
        
        _bInitUI = true;
    }
    
    // draw
    drawInfo();
    
    //
    scheduleUpdate();
}

void UILayoutCashNews::update(float dt)
{
    if ( _bInitUI == false )
    {
        return;
    }
    
    if ( _bTouch == true )
    {
        return;
    }
    
    _time += dt;
    if ( _time > 3 )
    {
        _time -= 3;
        
        ssize_t idx = _uiList->getCurrentPageIndex() + 1;
        if ( idx >= _uiList->getItems().size() )
        {
            idx = 0;
        }
        
        _uiList->scrollToPage(idx);
    }
    
}

#pragma mark - init
void UILayoutCashNews::initVar()
{
    _listNews = CashManager::getInstance()->getNewsAll();
}

void UILayoutCashNews::initUI()
{
    
}

#pragma mark - ui
void UILayoutCashNews::uiContainer()
{
    Size size = getContentSize();
    
    //
    auto layout = ui::Layout::create();
    layout->setContentSize(Size(size.width, size.height));
    layout->setIgnoreAnchorPointForPosition(false);
    layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    layout->setPosition(Vec2(getContentSize().width / 2, getContentSize().height));
    addChild(layout);

    _uiContainerInfo = ui::Layout::create();
    _uiContainerInfo->setContentSize(Size(size.width, size.height));
    _uiContainerInfo->setIgnoreAnchorPointForPosition(false);
    _uiContainerInfo->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _uiContainerInfo->setPosition(Vec2(size.width / 2, size.height));
    _uiContainerInfo->setClippingEnabled(true);
    layout->addChild(_uiContainerInfo);
}

void UILayoutCashNews::uiInfo()
{
    auto layout = _uiContainerInfo;
    auto size = layout->getContentSize();

    _uiList = ui::PageView::create();
    _uiList->setContentSize(Size(size.width, size.height));
    _uiList->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _uiList->setPosition(Vec2(size.width / 2, size.height));
    _uiList->setDirection(ui::PageView::Direction::HORIZONTAL);
    _uiList->addTouchEventListener([=](Ref* sender, Widget::TouchEventType type){
        if (type == Widget::TouchEventType::BEGAN)
        {
            _time = 0;
            _bTouch = true;
        }
        else if(type == Widget::TouchEventType::MOVED)
        {
            
        }
        else if(type == Widget::TouchEventType::ENDED)
        {
            _bTouch = false;
        }
        else
        {
            _bTouch = false;
        }
    });
    _uiList->addEventListener((ui::PageView::ccPageViewCallback)[=](Ref *pSender, ui::PageView::EventType type){
        _time = 0;
    });
    layout->addChild(_uiList);
    
    _uiList->setIndicatorEnabled(true);
    _uiList->setIndicatorSpaceBetweenIndexNodes(10);
    _uiList->setIndicatorIndexNodesColor(Color3B(44, 44, 44));
    _uiList->setIndicatorIndexNodesOpacity(255);
    _uiList->setIndicatorSelectedIndexColor(Color3B(84, 252, 255));
    _uiList->setIndicatorSelectedIndexOpacity(255);
    
    auto posIndicator = _uiList->getIndicatorPosition();
    _uiList->setIndicatorPosition(Vec2(posIndicator.x, posIndicator.y - 80));
}
#pragma mark - draw
void UILayoutCashNews::drawInfo()
{
    auto layout = _uiContainerInfo;
    auto size = layout->getContentSize();
    
    auto sizeNews = layout->getContentSize();
    sizeNews.height = 930;
 
    
    int idx = (int)_uiList->getCurrentPageIndex();
    
    //
    _uiList->removeAllPages();
    for ( int i = 0; i < _listNews.size(); i++ )
    {
        auto path = _listNews.at(i);
        
        auto uiLayout = ui::Layout::create();
        uiLayout->setContentSize(size);
        
        auto uiLayoutInner = ui::Layout::create();
        uiLayoutInner->setContentSize(sizeNews);
        uiLayoutInner->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        uiLayoutInner->setPosition(Vec2(uiLayout->getContentSize().width / 2.0f, uiLayout->getContentSize().height));
        uiLayoutInner->setClippingEnabled(true);
        uiLayout->addChild(uiLayoutInner);
        
        auto uiNews = ui::ImageView::create(path);
        uiNews->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        uiNews->setPosition(Vec2(uiLayoutInner->getContentSize().width / 2.0f, uiLayoutInner->getContentSize().height));
        uiNews->setScale(uiLayoutInner->getContentSize().width / uiNews->getContentSize().width);
        uiLayoutInner->addChild(uiNews);
        
        _uiList->addPage(uiLayout);
    }
    
    //
    _uiList->setCurrentPageIndex(idx);
}

#pragma mark - set, get

#pragma mark - callback
void UILayoutCashNews::callbackInfo(bool bResult)
{
    auto listNewDownload = CashManager::getInstance()->getNewsDownloadAll();
    if ( listNewDownload.size() > 0 )
    {
        CashManager::getInstance()->downloadNews(
                                                 [=](int total){
                                                     PopupLoadingLock::show();
                                                 },
                                                 [=](double percent){

                                                 }, [=](int success, int error){

                                                 },
                                                 [=](){

                                                     PopupLoadingLock::hide();

                                                     _listNews.clear();
                                                     
                                                     //
                                                     auto listNews = CashManager::getInstance()->getNewsAll();
                                                     for(const auto &path : listNews)
                                                     {
                                                         if ( FileUtils::getInstance()->isFileExist(path) == true )
                                                         {
                                                             _listNews.push_back(path);
                                                         }
                                                     }
                                                     
                                                     //
                                                     drawInfo();
                                                 });
    }
    else
    {
        _listNews.clear();
        
        //
        auto listNews = CashManager::getInstance()->getNewsAll();
        for(const auto &path : listNews)
        {
            if ( FileUtils::getInstance()->isFileExist(path) == true )
            {
                _listNews.push_back(path);
            }
        }
        
        //
        drawInfo();
    }
}

#pragma mark - click
