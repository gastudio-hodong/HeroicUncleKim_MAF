//
//  UILayerCostumeSkinShop.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#include "UILayerCostumeSkinShop.h"

#include "GameUIPopup/Other/CostumeSkin/PopupCostumeSkinBuy.h"

#include "ManagerGame/CostumeSkinManager.h"

USING_NS_CC;

UILayerCostumeSkinShop* UILayerCostumeSkinShop::create()
{
    UILayerCostumeSkinShop* pRet = new(std::nothrow) UILayerCostumeSkinShop();
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

UILayerCostumeSkinShop::UILayerCostumeSkinShop(void)
{
    
}

UILayerCostumeSkinShop::~UILayerCostumeSkinShop(void)
{
    
}

bool UILayerCostumeSkinShop::init()
{
    if ( !UILayerCostumeSkinParent::init() )
    {
        return false;
    }
    
    
    
    return true;
}

void UILayerCostumeSkinShop::onEnter()
{
    UILayerCostumeSkinParent::onEnter();
    
    // init
    initVar();
    initUi();
}

void UILayerCostumeSkinShop::refreshList()
{
    //
    setListLoad();
    
    // draw
    drawList();
}

#pragma mark - override DelegateListView
void UILayerCostumeSkinShop::listviewScrollView(ui::CListView* listview, ui::ScrollView::EventType type)
{
    switch (type) {
        case ui::ScrollView::EventType::SCROLL_TO_BOTTOM:
            break;
        case ui::ScrollView::EventType::SCROLL_TO_TOP:
            break;
            
        default:
            break;
    }
}

Size UILayerCostumeSkinShop::listviewCellSize(ui::CListView* listview, ssize_t idx)
{
    double width = listview->getContentSize().width;
    double height = 249 + 10;

    return Size(width, height);
}

void UILayerCostumeSkinShop::listviewCell(ui::CListView *listview, ui::Widget* childview, ssize_t idx)
{
    // size
    Size size = childview->getContentSize();
    size.width = 201 * 3 + 15 * 2;
    size.height = 249;

    // data
    ssize_t nIdxStart = idx * 3;
    ssize_t nIdxEnd = nIdxStart + 3;
    if ( nIdxEnd > _listSkinShop.size() )
    {
        nIdxEnd = _listSkinShop.size();
    }
    
    std::string pathBG = "Assets/ui/costume/skin_box1_1.png";
    
    // ui
    childview->removeAllChildren();
    
    auto uiLayout = ui::Layout::create();
    uiLayout->setContentSize(Size(size.width, size.height));
    uiLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    uiLayout->setPosition(Vec2(childview->getContentSize().width / 2, childview->getContentSize().height / 2));
    childview->addChild(uiLayout);
    
    for ( int i = 0; nIdxStart + i < nIdxEnd; i++ )
    {
        int nIdxReal = (int)nIdxStart + i;
        
        auto objSkinShop = _listSkinShop.at(nIdxReal);
        if ( objSkinShop == nullptr )
        {
            continue;
        }
        
        auto objSkin = CostumeSkinManager::getInstance()->getInfoSkin(objSkinShop->getSkinIdx());
        if ( objSkin == nullptr )
        {
            continue;
        }
        
        double posX = 201 * i + 15 * i;
        double posY = uiLayout->getContentSize().height;
        
        auto uiBtnBG = ui::Button::create(pathBG);
        uiBtnBG->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        uiBtnBG->setPosition(Vec2(posX, posY));
        uiBtnBG->addClickEventListener(CC_CALLBACK_1(UILayerCostumeSkinShop::onClickSkin, this));
        uiBtnBG->setTag(nIdxReal);
        uiLayout->addChild(uiBtnBG);
        
        // icon
        auto uiIcon = ui::ImageView::create(MafUtils::format("Assets/character_hero/skin/skin_%02d_walk_1.png", objSkin->getIdx()));
        uiIcon->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        uiIcon->setPosition(Vec2(100.5 + 165 + 15, 33));
        uiBtnBG->addChild(uiIcon);
        
        // name
        auto uiName = ui::CText::create(objSkin->getName(), GAME_FONT, 20);
        uiName->setPosition(Vec2(uiBtnBG->getContentSize().width / 2, 24));
        uiName->setTextColor(Color4B(225, 230, 235, 255));
        uiName->setTextOverflow(Label::Overflow::SHRINK);
        uiName->setTextAreaSize(Size(uiBtnBG->getContentSize().width - 10, uiName->getContentSize().height));
        uiName->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiName->setTextHorizontalAlignment(TextHAlignment::CENTER);
        uiBtnBG->addChild(uiName);
        
        // label
        if ( objSkinShop->getLabel() != 0 )
        {
            /*
             0 : 없음
             1 : 패키지
             2 : 할인
             3 : 이벤트
             4 : 기간 한정판매
             */
            std::string pathLabel = "Assets/ui/costume/costume_label_event.png";
            switch (objSkinShop->getLabel()) {
                case 1: pathLabel = "Assets/ui/costume/costume_label_package.png"; break;
                case 2: pathLabel = "Assets/ui/costume/costume_label_sale.png"; break;
                case 3: pathLabel = "Assets/ui/costume/costume_label_event.png"; break;
                case 4: pathLabel = "Assets/ui/costume/costume_label_limited.png"; break;
                    
                default:
                    break;
            }
            
            auto uiLabel = ui::ImageView::create(pathLabel);
            uiLabel->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
            uiLabel->setPosition(Vec2(uiBtnBG->getContentSize().width, 48));
            uiLabel->setScale(2);
            ((ui::Scale9Sprite*)uiLabel->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
            uiBtnBG->addChild(uiLabel);
        }
        
    }
}

int64_t UILayerCostumeSkinShop::listviewNumberOfCells(ui::CListView *listview)
{
    int64_t count = _listSkinShop.size() / 3;
        if ( _listSkinShop.size() % 3 != 0 )
            count++;
   
    return count;
}

#pragma mark - init
void UILayerCostumeSkinShop::initVar()
{
    // load
    setListLoad();
}

void UILayerCostumeSkinShop::initUi()
{
    uiContainer();
    uiList();
}


#pragma mark - ui
void UILayerCostumeSkinShop::uiContainer()
{
    Size size = getContentSize();
    
    //
    _uiContainerList = ui::Layout::create();
    _uiContainerList->setContentSize( Size(size.width, size.height) );
    _uiContainerList->setIgnoreAnchorPointForPosition(false);
    _uiContainerList->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _uiContainerList->setPosition( Vec2(size.width / 2, size.height) );
    addChild(_uiContainerList);
}

void UILayerCostumeSkinShop::uiList()
{
    _uiContainerList->removeAllChildren();
    
    auto layout = _uiContainerList;
    auto size = layout->getContentSize();
    
    //
    _uiList = ui::CListView::create(this, Size(size.width, size.height), ui::ScrollView::Direction::VERTICAL);
    _uiList->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _uiList->setPosition(Vec2(size.width / 2, 0));
    _uiList->setBounceEnabled(true);
    _uiList->setInertiaScrollEnabled(false);
    _uiList->setScrollBarPositionFromCorner(Vec2(7, 7));
    _uiList->reloadData();
    layout->addChild(_uiList);
}

#pragma mark - ui draw
void UILayerCostumeSkinShop::drawList()
{
    Vec2 pos = _uiList->getContainerOffset();
    _uiList->reloadData();
    _uiList->setContainerOffset(pos);
}

#pragma mark - set, get, add
void UILayerCostumeSkinShop::setListLoad()
{
    //
    _listSkinShop.clear();
    
    //
    auto list = CostumeSkinManager::getInstance()->getListCostumeSkinShop();
    for ( auto& objSkinShop : list )
    {
        
        bool bBuySaleTime = objSkinShop->isBuySaleTime();
        if ( bBuySaleTime == false )
        {
            continue;
        }
        
        bool bBuy = CostumeSkinManager::getInstance()->isCostumeSkinBuy(objSkinShop->getSkinIdx());
        if ( bBuy == true )
        {
            continue;
        }
        
        _listSkinShop.pushBack(objSkinShop);
    }
}

#pragma mark - click
void UILayerCostumeSkinShop::onClickSkin(cocos2d::Ref* sender)
{
    auto uiButton = (ui::Button*)sender;
    
    auto idx = uiButton->getTag();
    auto objSkinShop = _listSkinShop.at(idx);
    
    bool bBuySaleTime = objSkinShop->isBuySaleTime();
    if ( bBuySaleTime == false )
    {
        return;
    }
    
    bool bBuy = CostumeSkinManager::getInstance()->isCostumeSkinBuy(objSkinShop->getSkinIdx());
    if ( bBuy == true )
    {
        return;
    }
    
    //
    auto popup = PopupCostumeSkinBuy::create();
    popup->setSkinShopIdx(objSkinShop->getIdx());
    popup->show();
}

#pragma mark - callback
