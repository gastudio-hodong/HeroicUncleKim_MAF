//
//  PopupBafometz.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#include "PopupBafometz.h"

#include "GameUI/UI/UIReddot.h"
#include "GameUI/UI/UICurrency.h"

#include "GameUIPopup/Other/CurrencyDetail/PopupCurrencyDetail.h"
#include "GameUIPopup/Other/BattlePrison/PopupAuthority.h"

#include "ManagerGame/UserInfoManager.h"
#include "ManagerGame/StoryManager.h"
#include "ManagerGame/AccountManager.h"
#include "ManagerGame/CostumeManager.h"
#include "ManagerGame/PrisonManager.h"

USING_NS_CC;

PopupBafometz* PopupBafometz::create()
{
    PopupBafometz* pRet = new(std::nothrow) PopupBafometz();
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

PopupBafometz::PopupBafometz(void) :
_nBafometzCount(0),
_nSkip(0),
_isAuto(false),
_fUpgradeTime(0),
_fUpgradeTimeMax(0.2),

_onCallbackSkip(nullptr),
_onCallbackAuto(nullptr),
_onCallbackReward(nullptr),


_uiContentsUIInfo(nullptr),
_uiContentsUIContent(nullptr),

_uiInfoBafometzIcon(nullptr),
_uiContentCount(nullptr),
_uiContentArrowL(nullptr),
_uiContentArrowR(nullptr),
_uiContentProgress(nullptr),
_uiContentGet(nullptr)
{
    
}

PopupBafometz::~PopupBafometz(void)
{
    
}

bool PopupBafometz::init()
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    // init
    initVar();
    initUi();
    
    return true;
}

void PopupBafometz::onEnter()
{
    PopupBase::onEnter();
    
    // story
    StoryManager::getInstance()->showStory((int)E_COMIC::BAFOMETZ);
    
    //
    scheduleUpdate();
}

void PopupBafometz::update(float dt)
{
    if ( _uiContentArrowL != nullptr && _uiContentArrowL->isHighlighted() == true )
    {
        _fUpgradeTime += dt;
        if (_fUpgradeTimeMax <= _fUpgradeTime)
        {
            _fUpgradeTime = 0;
            _fUpgradeTimeMax -= 0.02f;
            
            onClickCountUpDown(_uiContentArrowL);
        }
    }
    else if ( _uiContentArrowR != nullptr && _uiContentArrowR->isHighlighted() == true )
    {
        _fUpgradeTime += dt;
        if (_fUpgradeTimeMax <= _fUpgradeTime)
        {
            _fUpgradeTime = 0;
            _fUpgradeTimeMax -= 0.02f;
            
            onClickCountUpDown(_uiContentArrowR);
        }
    }
    else
    {
        _fUpgradeTime = 0;
        _fUpgradeTimeMax = 0.2f;
    }
}

#pragma mark - init
void PopupBafometz::initVar()
{
    _nBafometzCount = PrisonManager::getInstance()->getBafometzCount();
}

void PopupBafometz::initUi()
{
    Size size = Size(732, 1017);
    
    //
    _spriteContainer->setContentSize( size );
    _spriteContainer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _spriteContainer->setPositionY(0);
    
    // label title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_pet_ui_bafometz"), GAME_FONT, 32);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5) );
    _spriteContainer->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupBafometz::onClickClose, this));
    itemClose->setPosition( Vec2(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5) );
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
    
    //
    {
        double width = 710;
        double heightUIInfo = 414;
        double heightUIContent = size.height - 75 - heightUIInfo - 25 - 20;
        
        _uiContentsUIInfo = ui::Layout::create();
        _uiContentsUIInfo->setContentSize(Size(width, heightUIInfo));
        _uiContentsUIInfo->setIgnoreAnchorPointForPosition(false);
        _uiContentsUIInfo->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        _uiContentsUIInfo->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75));
        _spriteContainer->addChild(_uiContentsUIInfo);
        
        _uiContentsUIContent = ui::Layout::create();
        _uiContentsUIContent->setContentSize(Size(width, heightUIContent));
        _uiContentsUIContent->setIgnoreAnchorPointForPosition(false);
        _uiContentsUIContent->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        _uiContentsUIContent->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, 25));
        _spriteContainer->addChild(_uiContentsUIContent);
    }
    
    // ui
    uiInfo();
    uiContent();
}

#pragma mark - ui
void PopupBafometz::uiInfo()
{
    auto layout = _uiContentsUIInfo;
    auto size = _uiContentsUIInfo->getContentSize();
    
    //
    auto uiImgInfoBG = ui::ImageView::create("Assets/ui/prison/prison_bg_baformetz_1.png");
    uiImgInfoBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiImgInfoBG->setPosition(Vec2(size.width / 2, size.height));
    uiImgInfoBG->setScale(3);
    ((ui::Scale9Sprite*)uiImgInfoBG->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
    layout->addChild(uiImgInfoBG);
    
    //
    _uiInfoBafometzIcon = ui::ImageView::create("Assets/ui/prison/icon_baformetz_1.png");
    _uiInfoBafometzIcon->setPosition(Vec2(40, size.height - 40));
    _uiInfoBafometzIcon->setVisible(false);
    _uiInfoBafometzIcon->setTouchEnabled(true);
    _uiInfoBafometzIcon->addClickEventListener([=](Ref* sender){
        
        auto popup = PopupAuthority::create(PrisonManager::getInstance()->getPrisonType());
        popup->show();
    });
    layout->addChild(_uiInfoBafometzIcon);
    {
        auto action1 = FadeOut::create(1.0);
        auto action2 = FadeIn::create(1.0);
        auto seq = Sequence::create(action1, action2, NULL);
        
        auto uiInfoBafometzIconLight = ui::ImageView::create("Assets/ui/prison/icon_baformetz_2.png");
        uiInfoBafometzIconLight->setPositionPercent(Vec2::ANCHOR_MIDDLE);
        uiInfoBafometzIconLight->setPositionType(ui::Widget::PositionType::PERCENT);
        uiInfoBafometzIconLight->runAction(RepeatForever::create(seq));
        _uiInfoBafometzIcon->addChild(uiInfoBafometzIconLight);
    }
    
    //
    std::vector<E_ITEMS> list;
    list.push_back(E_ITEMS::TICKET);
    
    auto uiCurrency = UICurrency::create(list, Size(170, 50));
    uiCurrency->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    uiCurrency->setPosition(Vec2(size.width, size.height - 246));
    layout->addChild(uiCurrency);
    
    //
    auto uiImgInfoBoard = ui::ImageView::create("Assets/ui_common/box_item.png");
    uiImgInfoBoard->setScale9Enabled(true);
    uiImgInfoBoard->setContentSize(Size(size.width, 168));
    uiImgInfoBoard->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    uiImgInfoBoard->setPosition(Vec2(size.width / 2, 0));
    layout->addChild(uiImgInfoBoard);
    {
        auto uiTextInfo = ui::CText::create(GAME_TEXT("t_ui_pet_ui_bafometz_msg_3"), GAME_FONT, 36);
        uiTextInfo->setPosition(Vec2(uiImgInfoBoard->getContentSize().width / 2, uiImgInfoBoard->getContentSize().height / 2));
        uiTextInfo->setTextAreaSize(Size(650, 118));
        uiTextInfo->setTextOverflow(Label::Overflow::SHRINK);
        uiTextInfo->setTextHorizontalAlignment(TextHAlignment::LEFT);
        uiTextInfo->setTextVerticalAlignment(TextVAlignment::CENTER);
        layout->addChild(uiTextInfo);
    }
    
    // draw
    drawInfo();
}

void PopupBafometz::uiContent()
{
    auto layout = _uiContentsUIContent;
    auto size = _uiContentsUIContent->getContentSize();
    
    //
    auto uiTextInfoReward = ui::CText::create(GAME_TEXT("t_ui_pet_ui_bafometz_msg_4"), GAME_FONT, 26);
    uiTextInfoReward->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiTextInfoReward->setPosition(Vec2(size.width / 2, size.height));
    uiTextInfoReward->setTextHorizontalAlignment(TextHAlignment::CENTER);
    uiTextInfoReward->setTextColor(Color4B(62, 128, 118, 255));
    layout->addChild(uiTextInfoReward);
    
    //
    {
        auto uiImgIconL = ui::ImageView::create("Assets/ui/prison/icon_baformetz_1.png");
        uiImgIconL->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        uiImgIconL->setPosition(Vec2(size.width / 2 - 40, 350));
        layout->addChild(uiImgIconL);
        
        auto uiImgIconM = ui::ImageView::create("Assets/ui_common/icon_equal.png");
        uiImgIconM->setPosition(Vec2(size.width / 2, 350));
        layout->addChild(uiImgIconM);
        
        auto uiImgIconR = ui::ImageView::create("Assets/ui/prison/icon_prison_1.png");
        uiImgIconR->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiImgIconR->setPosition(Vec2(size.width / 2 + 40, 350));
        layout->addChild(uiImgIconR);
    }
    
    //
    auto uiImgContentBoard = ui::ImageView::create("Assets/ui_common/box.png");
    uiImgContentBoard->setScale9Enabled(true);
    uiImgContentBoard->setContentSize(Size(size.width, 222));
    uiImgContentBoard->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    uiImgContentBoard->setPosition(Vec2(size.width / 2, 70));
    layout->addChild(uiImgContentBoard);
    {
        _uiContentArrowL = ui::Button::create("Assets/ui_common/btn_quantity_down.png");
        _uiContentArrowL->setPosition(Vec2(uiImgContentBoard->getContentSize().width / 2 - 100, 175));
        _uiContentArrowL->addClickEventListener(CC_CALLBACK_1(PopupBafometz::onClickCountUpDown, this));
        _uiContentArrowL->setTag(1);
        uiImgContentBoard->addChild(_uiContentArrowL);
        
        _uiContentArrowR = ui::Button::create("Assets/ui_common/btn_quantity_up.png");
        _uiContentArrowR->setPosition(Vec2(uiImgContentBoard->getContentSize().width / 2 + 100, 175));
        _uiContentArrowR->addClickEventListener(CC_CALLBACK_1(PopupBafometz::onClickCountUpDown, this));
        _uiContentArrowR->setTag(2);
        uiImgContentBoard->addChild(_uiContentArrowR);
        
        _uiContentCount = ui::CText::create("0", GAME_FONT, 32);
        _uiContentCount->setPosition(Vec2(uiImgContentBoard->getContentSize().width / 2, 175));
        _uiContentCount->setTextColor(Color4B(COLOR_TITLE));
        uiImgContentBoard->addChild(_uiContentCount);
        {
            auto uiImgCountBG = ui::ImageView::create("Assets/ui/prison/bafometz/baphomet_numbox.png");
            uiImgCountBG->setPositionType(ui::Widget::PositionType::PERCENT);
            uiImgCountBG->setPositionPercent(Vec2::ANCHOR_MIDDLE);
            _uiContentCount->addChild(uiImgCountBG, -1);
        }
        
        auto uiBtnGive = ui::Button::create("Assets/ui_common/btn_baphomet.png");
        uiBtnGive->setPosition(Vec2(uiImgContentBoard->getContentSize().width / 2, 110));
        uiBtnGive->addClickEventListener(CC_CALLBACK_1(PopupBafometz::onClickSkip, this));
        uiImgContentBoard->addChild(uiBtnGive);
        {
            auto uiText = ui::CText::create(GAME_TEXT("t_ui_pet_ui_bafometz_msg_2"), GAME_FONT, 28);
            uiText->setPositionType(ui::Widget::PositionType::PERCENT);
            uiText->setPositionPercent(Vec2::ANCHOR_MIDDLE);
            uiText->setTextColor(Color4B(COLOR_TITLE));
            uiBtnGive->addChild(uiText);
        }

        auto uiImgContentBarBG = ui::ImageView::create("Assets/ui/prison/bafometz/baphomet_gift_bar_1.png");
        uiImgContentBarBG->setPosition(Vec2(uiImgContentBoard->getContentSize().width / 2, 40));
        uiImgContentBoard->addChild(uiImgContentBarBG);
        {
            _uiContentProgress = ui::LoadingBar::create("Assets/ui/prison/bafometz/baphomet_gift_bar_2.png");
            _uiContentProgress->setPositionPercent(Vec2::ANCHOR_MIDDLE);
            _uiContentProgress->setPositionType(ui::Widget::PositionType::PERCENT);
            _uiContentProgress->setPercent(50);
            uiImgContentBarBG->addChild(_uiContentProgress);
            
            _uiContentProgressCount = ui::CText::create("0/0", GAME_FONT, 24);
            _uiContentProgressCount->setPositionPercent(Vec2::ANCHOR_MIDDLE);
            _uiContentProgressCount->setPositionType(ui::Widget::PositionType::PERCENT);
            uiImgContentBarBG->addChild(_uiContentProgressCount);
            
            _uiContentReward = ui::Button::create("Assets/ui/prison/icon_baformetz_1.png");
            _uiContentReward->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
            _uiContentReward->setPositionPercent(Vec2::ANCHOR_MIDDLE_RIGHT);
            _uiContentReward->setPositionType(ui::Widget::PositionType::PERCENT);
            _uiContentReward->addClickEventListener(CC_CALLBACK_1(PopupBafometz::onClickGetReward, this));
            uiImgContentBarBG->addChild(_uiContentReward);
        }
    }
    
    //
    _uiContentGet = ui::CText::create(GAME_TEXT("t_ui_pet_ui_bafometz_msg_5"), GAME_FONT, 24);
    _uiContentGet->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _uiContentGet->setPosition(Vec2(size.width / 2, 10));
    _uiContentGet->setTextColor(Color4B(127, 127, 127, 255));
    _uiContentGet->setTextAreaSize(Size(size.width - 100, _uiContentGet->getContentSize().height));
    _uiContentGet->setTextOverflow(Label::Overflow::SHRINK);
    _uiContentGet->setTextHorizontalAlignment(TextHAlignment::CENTER);
    _uiContentGet->setTextVerticalAlignment(TextVAlignment::CENTER);
    layout->addChild(_uiContentGet);
    
    
    // draw
    drawContent();
}

#pragma mark - ui draw
void PopupBafometz::drawInfo()
{
    auto layout = _uiContentsUIInfo;
    
    if ( UserInfoManager::getInstance()->getPetBafometzHave() == true )
    {
        _uiInfoBafometzIcon->setVisible(true);
    }
    else
    {
        _uiInfoBafometzIcon->setVisible(false);
    }
}

void PopupBafometz::drawContent()
{
    auto layout = _uiContentsUIContent;
    
    //
    int countMax = UserInfoManager::getInstance()->getTicketMax();
    int countNow = _nBafometzCount;
    int nPercent = (double)countNow / countMax * 100;
    if ( nPercent > 100 )
        nPercent = 100;
    
    _uiContentProgress->setPercent(nPercent);
    _uiContentProgressCount->setString(MafUtils::format("%d / %d", countNow, countMax));
    
    
    //
    if ( UserInfoManager::getInstance()->getPetBafometzHave() == true )
    {
        _uiContentGet->setVisible(false);
        if ( countNow < countMax )
        {
            _uiContentReward->loadTextureNormal("Assets/ui/prison/prison_ticket_off.png");
        }
        else
        {
            _uiContentReward->loadTextureNormal("Assets/ui/prison/prison_ticket.png");
        }
        
    }
    else
    {
        _uiContentGet->setVisible(true);
        _uiContentReward->loadTextureNormal("Assets/ui/prison/icon_baformetz_1.png");
    }
}
    
  

#pragma mark - set, get, other
void PopupBafometz::setCallbackSkip(const std::function<void(int)>& callback)
{
    _onCallbackSkip = callback;
}

void PopupBafometz::setCallbackAuto(const std::function<void(bool)>& callback, bool isAuto)
{
    _onCallbackAuto = callback;
    _isAuto = isAuto;
}

void PopupBafometz::setCallbackReward(const std::function<void(void)>& callback)
{
    _onCallbackReward = callback;
}

#pragma mark - click
void PopupBafometz::onClickClose(Ref* sender)
{
    if ( _onCallbackAuto != nullptr )
    {
        _onCallbackAuto(_isAuto);
    }
    
    hide();
}

void PopupBafometz::onClickCountUpDown(cocos2d::Ref* sender)
{
    auto item = (MenuItem *)sender;
    
    int nHeart = PrisonManager::getInstance()->getHeart();
    if ( item->getTag() == 1 )
    {
        _nSkip -= 1;
        if ( _nSkip < 0 )
        {
            _nSkip = nHeart;
        }
    }
    else if ( item->getTag() == 2 )
    {
        _nSkip += 1;
        if ( _nSkip > nHeart )
        {
            _nSkip = 0;
        }
    }
    
    _uiContentCount->setString(MafUtils::toString(_nSkip));
}
void PopupBafometz::onClickSkip(cocos2d::Ref* sender)
{
    if ( _nSkip <= 0 )
    {
        return;
    }
    
    //
    if ( PrisonManager::getInstance()->getResetTime() <= 0 )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_prison_error_reset"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), false, [=](Ref* sender){

            PrisonManager::getInstance()->requestInfo(true, (int)PrisonManager::getInstance()->getPrisonType(), nullptr);
            
        });
        popup->setHideBackKey(false);
        popup->show();
        
        hide();
        return;
    }
    
    //
    if ( _onCallbackSkip != nullptr )
    {
        _onCallbackSkip(_nSkip);
    }
    
    hide();
}

void PopupBafometz::onClickGetReward(cocos2d::Ref *sender)
{
    if ( UserInfoManager::getInstance()->getPetBafometzHave() == false )
    {
        auto popup = PopupAuthority::create(PrisonManager::getInstance()->getPrisonType());
        popup->show();
        return;
    }
    
    int countMax = UserInfoManager::getInstance()->getTicketMax();
    int countNow = _nBafometzCount;
    if ( countNow < countMax )
    {
        return;
    }
    
    //
    requestReward();
}

#pragma mark - network
void PopupBafometz::requestReward()
{
    bool isHaveCostume = CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_33);
    
    PopupLoading::show();
    
    std::string url = "/bafometz/reward/%d/%d";
    url = MafUtils::format(url.c_str(), AccountManager::getInstance()->getUserIdx(), isHaveCostume);
    
    auto req = new MafHttpNew(network::HttpRequest::Type::GET, url.c_str());
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseReward(response,dd);
    };
    req->send(cb);
}

void PopupBafometz::responseReward(network::HttpResponse* response,std::string &data)
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
    
    if ( bError == false && (jsonParser.HasMember("_returnCount") == false || jsonParser.HasMember("_ticket") == false) )
    {
        bError = true;
    }
    
    if ( bError == true )
    {
        return;
    }
    
    if(data.length()>0)
    {
        
        auto ticket = jsonParser[("_ticket")].GetInt();
        auto returnCount = jsonParser[("_returnCount")].GetInt();
        
        if(ticket)
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXTFORMAT("t_ui_prison_msg_31", ticket));
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
            
            //
            _nBafometzCount = returnCount;
            
            
            // draw
            drawInfo();
            drawContent();
            
            //
            if ( _onCallbackReward != nullptr )
            {
                _onCallbackReward();
            }
            
            return;
        }
        
        
    }
}

