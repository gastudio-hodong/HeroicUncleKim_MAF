//
//  PopupBeelzebub.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#include "PopupBeelzebub.h"

#include "ManagerGame/UserInfoManager.h"
#include "GameObject/Prison/InfoTradingMarket.h"

#include "GameUI/UI/UICurrency.h"

#include "GameUIPopup/Other/Story/PopupStory.h"
#include "GameUIPopup/Other/Story/PopupStoryDownload.h"
#include "GameUIPopup/Other/TradingMarket/LayerTradingMarket.h"

#include "ManagerGame/StoryManager.h"
#include "ManagerGame/SaveManager.h"
#include "ManagerGame/AccountManager.h"
#include "ManagerGame/ItemsManager.h"
#include "ManagerGame/CostumeManager.h"
#include "ManagerGame/TradingMarketManager.h"

USING_NS_CC;

PopupBeelzebub* PopupBeelzebub::create()
{
    PopupBeelzebub* pRet = new(std::nothrow) PopupBeelzebub();
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

PopupBeelzebub::PopupBeelzebub(void) :
_bInitUI(false),

//
_nBlessing(0),
_hellPointNow(0),
_hellPointMax(0),


// widget
_spriteContainerBottom(nullptr),

_uiContainerList(nullptr),
_uiContainerAuthority(nullptr)
{
    
}

PopupBeelzebub::~PopupBeelzebub(void)
{
    
}

bool PopupBeelzebub::init()
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    //
    setName("PopupBeelzebub");
    
    // init
    initVar();
    initUi();
    
    return true;
}

void PopupBeelzebub::onEnter()
{
    PopupBase::onEnter();
    
    if ( _bInitUI == false )
    {
        _bInitUI = true;
        
        //
        uiContainer();
        uiAuthority();
        
        //
        if ( UserInfoManager::getInstance()->getPetBafometzHave() == true )
        {
            StoryManager::getInstance()->showStory((int)E_COMIC::BEELZEBUB);
        }
    }
    
    //
    drawAuthority();
}

#pragma mark - init
void PopupBeelzebub::initVar()
{
}

void PopupBeelzebub::initUi()
{
    Size size = Size(730, 700);

    //
    _spriteContainer->setContentSize( size );
    
    _spriteContainerBottom = ui::ImageView::create("Assets/ui_common/popup_mini_scale2_1.png");
    _spriteContainerBottom->setScale9Enabled(true);
    _spriteContainerBottom->setContentSize(Size(size.width, 200));
    _spriteContainerBottom->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _spriteContainerBottom->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, - 10));
    _spriteContainer->addChild(_spriteContainerBottom);
    
    // title
    auto spriteTitle = Sprite::create("Assets/ui/prison/beelzebub/beelzebub_title.png");
    spriteTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    spriteTitle->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height);
    _spriteContainer->addChild(spriteTitle);
    
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_prison_msg_33"), GAME_FONT, 28);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5) );
    _spriteContainer->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupBeelzebub::onClickClose, this));
    itemClose->setPosition( Vec2(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5) );
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
   
    //
    std::vector<E_ITEMS> list;
    list.push_back(E_ITEMS::HELL);
    
    auto uiCurrency = UICurrency::create(list, Size(_spriteContainer->getContentSize().width, 50));
    uiCurrency->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiCurrency->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75));
    _spriteContainer->addChild(uiCurrency);
}
 

#pragma mark - ui
void PopupBeelzebub::uiContainer()
{
    // default
    {
        Size size = _spriteContainer->getContentSize();
        size.width = size.width - 38;
        size.height = size.height - 75 - 50 - 25;
        
        auto layout = ui::Layout::create();
        layout->setContentSize(Size(size.width, size.height));
        layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        layout->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, 25));
        _spriteContainer->addChild(layout);
        
        //
        auto uiContainer = LayerTradingMarket::create(0);
        uiContainer->setContentSize(Size(size.width, size.height));
        uiContainer->setPosition(Vec2(size.width / 2, size.height / 2));
        uiContainer->setCallbackInfo(CC_CALLBACK_1(PopupBeelzebub::callbackInfo, this));
        layout->addChild(uiContainer);
        
        _uiContainerList = uiContainer;
    }
    
    // bottom
    {
        Size size = _spriteContainerBottom->getContentSize();
        size.width = size.width - 50;
        size.height = size.height - 50;
        
        auto layout = ui::Layout::create();
        layout->setContentSize(Size(size.width, size.height));
        layout->setIgnoreAnchorPointForPosition(false);
        layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        layout->setPosition(Vec2(_spriteContainerBottom->getContentSize().width / 2, 25));
        _spriteContainerBottom->addChild(layout);
        
        _uiContainerAuthority = ui::Layout::create();
        _uiContainerAuthority->setContentSize( Size(size.width, size.height) );
        _uiContainerAuthority->setAnchorPoint( Vec2::ANCHOR_MIDDLE );
        _uiContainerAuthority->setPositionType(ui::Widget::PositionType::PERCENT);
        _uiContainerAuthority->setPositionPercent(Vec2::ANCHOR_MIDDLE);
        layout->addChild(_uiContainerAuthority);
    }
}
 
void PopupBeelzebub::uiAuthority()
{
    auto layout = _uiContainerAuthority;
    auto size = layout->getContentSize();
    
    //
    auto uiLayoutExplain = ui::Layout::create();
    uiLayoutExplain->setContentSize(size);
    uiLayoutExplain->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    uiLayoutExplain->setPositionType(ui::Widget::PositionType::PERCENT);
    uiLayoutExplain->setPositionPercent(Vec2::ANCHOR_MIDDLE);
    uiLayoutExplain->setName("UI_LAYOUT_EXPLAIN");
    uiLayoutExplain->setVisible(false);
    layout->addChild(uiLayoutExplain);
    {
        auto uiText = ui::CText::create(GAME_TEXT("t_ui_menu_hellprison_guide"), GAME_FONT, 24);
        uiText->setPositionType(ui::Widget::PositionType::PERCENT);
        uiText->setPositionPercent(Vec2::ANCHOR_MIDDLE);
        uiText->setTextAreaSize(Size(uiLayoutExplain->getContentSize().width - 10, uiLayoutExplain->getContentSize().height - 10));
        uiText->setTextOverflow(Label::Overflow::SHRINK);
        uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
        uiLayoutExplain->addChild(uiText);
    }
    
    //
    auto uiLayoutPoint = ui::Layout::create();
    uiLayoutPoint->setContentSize(size);
    uiLayoutPoint->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    uiLayoutPoint->setPositionType(ui::Widget::PositionType::PERCENT);
    uiLayoutPoint->setPositionPercent(Vec2::ANCHOR_MIDDLE);
    uiLayoutPoint->setName("UI_LAYOUT_POINT");
    uiLayoutPoint->setVisible(false);
    layout->addChild(uiLayoutPoint);
    {
        //배경 앞 회색 네모
        auto uiPointBG = ui::ImageView::create("Assets/ui/prison/beelzebub/box_beelzebub_list1.png");
        uiPointBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        uiPointBG->setPosition(Vec2(uiLayoutPoint->getContentSize().width / 2, 0));
        uiLayoutPoint->addChild(uiPointBG);
        {
            //
            auto uiIconBG = ui::ImageView::create("Assets/ui_common/box_item.png");
            uiIconBG->setScale9Enabled(true);
            uiIconBG->setContentSize(Size(82, 82));
            uiIconBG->setAnchorPoint( Vec2::ANCHOR_MIDDLE_LEFT );
            uiIconBG->setPosition(Vec2(10, uiPointBG->getContentSize().height / 2));
            uiPointBG->addChild(uiIconBG);
            {
                auto uiIcon = ui::ImageView::create("Assets/icon/evilcrystal_2.png");
                uiIcon->setPosition(Vec2(uiIconBG->getContentSize().width / 2, uiIconBG->getContentSize().height / 2));
                uiIcon->setScale(uiIconBG->getContentSize().width / uiIcon->getContentSize().width);
                ((ui::Scale9Sprite*)uiIcon->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
                uiIconBG->addChild(uiIcon);
            }
            
            //
            auto uiBtnSend = ui::Button::create("Assets/ui_common/btn_1_1_on.png");
            uiBtnSend->setScale9Enabled(true);
            uiBtnSend->setContentSize(Size(142, 86));
            uiBtnSend->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
            uiBtnSend->setPosition(Vec2(uiPointBG->getContentSize().width - 10, uiPointBG->getContentSize().height / 2));
            uiBtnSend->addClickEventListener(CC_CALLBACK_1(PopupBeelzebub::onClickPointSend, this));
            uiPointBG->addChild(uiBtnSend);
            {
                auto labelText = Label::createWithTTF(GAME_TEXT("t_ui_pet_ui_bafometz_msg_2"), GAME_FONT, 24);
                labelText->setPosition(uiBtnSend->getContentSize().width / 2, uiBtnSend->getContentSize().height / 2);
                uiBtnSend->addChild(labelText);
            }
            
            //
            auto uiCountBG = ui::ImageView::create("Assets/ui_common/base_box_1_1.png");
            uiCountBG->setScale9Enabled(true);
            uiCountBG->setContentSize(Size(82, 50));
            uiCountBG->setPosition(Vec2(uiPointBG->getContentSize().width - 270, uiPointBG->getContentSize().height - 50));
            uiPointBG->addChild(uiCountBG);
            {
                auto uiBtnArrowL = ui::Button::create("Assets/ui_common/btn_quantity_down.png");
                uiBtnArrowL->setPosition(Vec2(uiCountBG->getContentSize().width / 2 - 70, uiCountBG->getContentSize().height / 2));
                uiBtnArrowL->setTag(1);
                uiBtnArrowL->addClickEventListener(CC_CALLBACK_1(PopupBeelzebub::onClickPointArrow, this));
                uiCountBG->addChild(uiBtnArrowL);
                
                auto uiBtnArrowR = ui::Button::create("Assets/ui_common/btn_quantity_up.png");
                uiBtnArrowR->setPosition(Vec2(uiCountBG->getContentSize().width / 2 + 70, uiCountBG->getContentSize().height / 2));
                uiBtnArrowR->setTag(2);
                uiBtnArrowR->addClickEventListener(CC_CALLBACK_1(PopupBeelzebub::onClickPointArrow, this));
                uiCountBG->addChild(uiBtnArrowR);
                
                auto uiPointCount = ui::CText::create(GAME_TEXTFORMAT("t_ui_pet_ui_bafometz_msg_1", 0), GAME_FONT, 24);
                uiPointCount->setPosition(Vec2(uiCountBG->getContentSize().width / 2, uiCountBG->getContentSize().height / 2));
                uiPointCount->setName("UI_POINT_COUNT");
                uiCountBG->addChild(uiPointCount);
            }
            
            auto uiBarBG = ui::ImageView::create("Assets/ui/prison/beelzebub/beelzebub_progressbar_2.png");
            uiBarBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
            uiBarBG->setPosition(Vec2(uiPointBG->getContentSize().width - 170, 25));
            uiPointBG->addChild(uiBarBG);
            {
                auto uiPointBar = ui::LoadingBar::create("Assets/ui/prison/beelzebub/beelzebub_progressbar_1.png");
                uiPointBar->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
                uiPointBar->setPosition(Vec2(0, 0));
                uiPointBar->setPercent(0);
                uiPointBar->setName("UI_POINT_BAR");
                uiBarBG->addChild(uiPointBar);
                
                auto uiPointTotalCount = ui::CText::create("0/0", GAME_FONT, 18);
                uiPointTotalCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
                uiPointTotalCount->setPosition(Vec2(uiBarBG->getContentSize().width - 10, uiBarBG->getContentSize().height / 2));
                uiPointTotalCount->setName("UI_POINT_TOTAL_COUNT");
                uiBarBG->addChild(uiPointTotalCount);
            }
        }
        
        auto uiTitle = ui::CText::create(GAME_TEXT("t_ui_prison_msg_38"), GAME_FONT, 28);
        uiTitle->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        uiTitle->setPosition(Vec2(uiPointBG->getPositionX() - uiPointBG->getContentSize().width / 2, uiPointBG->getContentSize().height));
        uiTitle->setTextColor(Color4B(COLOR_COIN));
        uiLayoutPoint->addChild(uiTitle);
    }
}

#pragma mark - ui draw
void PopupBeelzebub::drawAuthority()
{
    auto layout = _uiContainerAuthority;
    
    //
    auto uiLayoutExplain = layout->getChildByName<ui::Layout*>("UI_LAYOUT_EXPLAIN");
    auto uiLayoutPoint = layout->getChildByName<ui::Layout*>("UI_LAYOUT_POINT");

    if ( CostumeManager::getInstance()->getExist(E_COSTUME::JEWEL, E_COSTUME_IDX::IDX_18) || _hellPointNow >= _hellPointMax )
    {
        uiLayoutExplain->setVisible(true);
        uiLayoutPoint->setVisible(false);
    }
    else
    {
        uiLayoutExplain->setVisible(false);
        uiLayoutPoint->setVisible(true);
        
        //
        if ( auto uiPointCount = utils::findChild<ui::CText*>(uiLayoutPoint, "UI_POINT_COUNT"); uiPointCount != nullptr )
        {
            std::string strCount = GAME_TEXTFORMAT("t_ui_pet_ui_bafometz_msg_1", _nBlessing.valueInt());
            uiPointCount->setString(strCount);
        }
        
        double percent = (double)_hellPointNow / _hellPointMax * 100;
        if ( percent > 100 )
            percent = 100;
        
        if ( auto uiPointBar = utils::findChild<ui::LoadingBar*>(uiLayoutPoint, "UI_POINT_BAR"); uiPointBar != nullptr )
        {
            uiPointBar->setPercent(percent);
        }
        
        if ( auto uiPointTotalCount = utils::findChild<ui::CText*>(uiLayoutPoint, "UI_POINT_TOTAL_COUNT"); uiPointTotalCount != nullptr )
        {
            uiPointTotalCount->setString(MafUtils::format("%d/%d", _hellPointNow.valueInt(), _hellPointMax.valueInt()));
        }
    }
}

#pragma mark - set, get

#pragma mark - callback
void PopupBeelzebub::callbackInfo(bool bResult)
{
    if ( bResult == false )
    {
        std::string strMessage = GAME_TEXT("t_ui_error_30");
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strMessage);
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        hide();
        return;
    }
    
    //
    _nBlessing = 0;
    _hellPointNow = TradingMarketManager::getInstance()->getHellPointNow();
    _hellPointMax = TradingMarketManager::getInstance()->getHellPointMax();
    
    //
    if ( _hellPointMax != 0 && _hellPointNow >= _hellPointMax )
    {
        if ( CostumeManager::getInstance()->isExist(E_COSTUME::JEWEL, E_COSTUME_IDX::IDX_18) == false )
        {
            CostumeManager::getInstance()->setExist(E_COSTUME::JEWEL, E_COSTUME_IDX::IDX_18, 1);
            SaveManager::saveCostume();
            
            bool bFile = StoryManager::getInstance()->isStoryFile((int)E_COMIC::BEELZEBUB);
            if ( bFile == false )
            {
                auto popup = PopupStoryDownload::create((int)E_COMIC::BEELZEBUB);
                popup->setHideBackKey(false);
                popup->show();
            }
            else
            {
                // open
                auto popup = PopupStory::create((int)E_COMIC::BEELZEBUB);
                popup->setCallbackFinish(nullptr);
                popup->setHideAll(false);
                popup->show();
            }
        }
    }
    
    //
    drawAuthority();
}

#pragma mark - schedule

#pragma mark - click
void PopupBeelzebub::onClickClose(cocos2d::Ref* sender)
{
    hide();
}

void PopupBeelzebub::onClickPointSend(cocos2d::Ref *sender)
{
    if( UserInfoManager::getInstance()->getPetBafometzHave() == false)
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_prison_msg_39"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    if ( _nBlessing <= 0 )
    {
        return;
    }
    
    requestBlessing();
}

void PopupBeelzebub::onClickPointArrow(cocos2d::Ref *sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiBtn = dynamic_cast<ui::Button*>(sender);
    
    if ( UserInfoManager::getInstance()->getPetBafometzHave() == false )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_prison_msg_39"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    
    if ( uiBtn->getTag() == 1 )
    {
        _nBlessing -= 1;
        if ( _nBlessing < 0 )
        {
            _nBlessing = atoi(ItemsManager::getInstance()->getItems(E_ITEMS::HELL).c_str());
            if( _nBlessing > _hellPointMax - _hellPointNow )
            {
                _nBlessing = _hellPointMax - _hellPointNow;
            }
        }
    }
    else if ( uiBtn->getTag() == 2 )
    {
        _nBlessing += 1;
        if ( _nBlessing > atoi(ItemsManager::getInstance()->getItems(E_ITEMS::HELL).c_str()) || _nBlessing > _hellPointMax - _hellPointNow)
        {
            _nBlessing = 0;
        }
    }

    //
    drawAuthority();
}
 
#pragma mark - click
void PopupBeelzebub::requestBlessing()
{
    // network
    PopupLoading::show();
    
    std::string url = "/exchange/point";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_hell_count", _nBlessing.valueInt());
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseBlessing(response, dd);
    };
    req->send(cb);
}

void PopupBeelzebub::responseBlessing(network::HttpResponse* response,std::string &data)
{
    PopupLoading::hide();

    bool bError = false;
    if ( response == nullptr || response->isSucceed() == false || response->getResponseCode() != 200 )
    {
        bError = true;
    }

    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)data.c_str());
    if(jsonParser.HasParseError())
    {
        bError = true;
    }

    if ( bError == true )
    {
        return;
    }
    
    bool bResult = jsonParser["_result"].GetBool();
    if ( bResult == true )
    {
        _hellPointNow += _nBlessing;
        _nBlessing = 0;

        if ( _hellPointNow >= _hellPointMax )
        {
            CostumeManager::getInstance()->setExist(E_COSTUME::JEWEL, E_COSTUME_IDX::IDX_18, 1);
            SaveManager::saveCostume();
            
            
            bool bFile = StoryManager::getInstance()->isStoryFile((int)E_COMIC::BEELZEBUB);
            if ( bFile == false )
            {
                auto popup = PopupStoryDownload::create((int)E_COMIC::BEELZEBUB);
                popup->setHideBackKey(false);
                popup->show();
            }
            else
            {
                // open
                auto popup = PopupStory::create((int)E_COMIC::BEELZEBUB);
                popup->setCallbackFinish(nullptr);
                popup->setHideAll(false);
                popup->show();
            }
        }
    }
    
    //
    TradingMarketManager::getInstance()->requestInfo(true, 0, CC_CALLBACK_1(PopupBeelzebub::callbackInfo, this));
}
