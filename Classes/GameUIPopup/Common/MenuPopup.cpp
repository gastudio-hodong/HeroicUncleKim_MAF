//
//  ModelComic.cpp
//  FantasyClicker
//
//  Created by SanHeo on 2017. 8. 8..
//
//

#include "MenuPopup.h"

#include "GameUIPopup/Other/Settings/PopupSettings.h"
#include "GameUIPopup/Other/Settings/PopupLanguage.h"
#include "GameUIPopup/Other/Settings/Transfer/PopupAccountTransfer.h"
#include "GameUIPopup/Other/BonsikDex/PopupBonsikDexDefault.h"
#include "GameUIPopup/Other/Supporters/PopupSupporters.h"
#include "GameUIPopup/Other/Attend/PopupAttend.h"
#include "GameUIPopup/Other/Stat/PopupStat.h"
#include "GameUIPopup/Other/Settings/Mail/MailPopup.h"
#include "GameUIPopup/Other/Settings/SaveLoad/PopupCloudSaveLoad.h"
#include "GameUIPopup/Other/Story/PopupStoryListDefault.h"
#include "GameUIPopup/Other/Friend/PopupFriend.h"
#include "GameUIPopup/Other/Settings/SaveLoad/PopupCloudLoad.h"
#include "GameUIPopup/Other/Settings/PopupContactUs.h"
#include "GameUIPopup/Other/User/PopupWithdrawal.h"
#include "GameUIPopup/Other/User/PopupUserLoginGuestAgain.h"

#include "ManagerGame/UserInfoManager.h"
#include "ManagerGame/UserInfoConfigManager.h"
#include "ManagerGame/SaveManager.h"
#include "ManagerGame/DataManager.h"
#include "ManagerGame/AccountManager.h"
#include "ManagerGame/LootsManager.h"

USING_NS_CC;

MenuPopup::MenuPopup() :
_uiReddotAttent(nullptr),
_uiReddotGift(nullptr),
_uiReddotBonsikdex(nullptr)
{
    
}

MenuPopup::~MenuPopup()
{
    
}

bool MenuPopup::init()
{
    if( !PopupBase::init() )
    {
        return false;
    }
    
    // init ui
    initVar();
    initUi();
    
    RefreshManager::getInstance()->addUI(E_REFRESH::MAIL, this);
    
    return true;
}

void MenuPopup::refresh(E_REFRESH eType, cocos2d::ValueMap listData)
{
    if ( eType == E_REFRESH::MAIL )
    {
        if ( _uiReddotGift != nullptr )
        {
            _uiReddotGift->setVisible(UserInfoManager::getInstance()->getGiftHave());
        }
    }
}

#pragma mark - init
void MenuPopup::initVar()
{
    //
    if ( TextManager::getInstance()->getLang().compare("ko") == 0 )
    {
        _listElementTop.push_back( {tag_menu_facebook, "Assets/ui/menu/btn_facebook.png",         "t_ui_menu_facebook"} );
        _listElementTop.push_back( {tag_menu_naver,    "Assets/ui/menu/btn_cafe.png",             "t_ui_menu_naver"} );

    }
    else if ( TextManager::getInstance()->getLang().compare("jp") == 0 )
    {
        _listElementTop.push_back( {tag_menu_lobi,     "Assets/ui/menu/btn_lobi.png",             "t_ui_menu_lobi"} );
        _listElementTop.push_back( {tag_menu_twitter,  "Assets/ui/menu/btn_twitter.png",          "t_ui_menu_twitter"} );
        _listElementTop.push_back( {tag_menu_jpwiki,   "Assets/ui/menu/btn_wiki.png",             "t_ui_menu_jpwiki"} );
        _listElementTop.push_back( {tag_menu_faq,      "Assets/ui/menu/btn_faq.png",              "t_ui_menu_faq"} );
    }
    else
    {
        _listElementTop.push_back( {tag_menu_facebook, "Assets/ui/menu/btn_facebook.png",         "t_ui_menu_facebook"} );
        _listElementTop.push_back( {tag_menu_discord,  "Assets/ui/menu/btn_discord.png",          "t_ui_menu_discord"} );
    }
    
    //
    _listElementMini.push_back( {tag_menu_mail,"", "t_ui_menu_contactus"} );
    _listElementMini.push_back( {tag_menu_rule_terms,"", "t_ui_terms_link"} );
    _listElementMini.push_back( {tag_menu_rule_privacy,"", "t_ui_privacy_link"} );
    
    //
    _listElementMini.push_back( {tag_menu_rate,"", "t_ui_menu_recommend"} );
    _listElementMini.push_back({tag_menu_withdrawal,"", "t_ui_withdrawal_title"});
    if ( AccountManager::getInstance()->isGuest() == true )
    {
        _listElementMini.push_back({tag_menu_login, "", "t_ui_login_guest_login_account_popup_title"});
    }
    
    //
    if ( UserInfoManager::getInstance()->getCurrentPlace() == E_PLACE::DG_DEVIL_DOM )
    {
        initVarDevil();
    }
    else
    {
        initVarNormal();
    }
}

void MenuPopup::initVarNormal()
{
    _listElement.push_back( {tag_menu_save,     "Assets/ui/menu/btn_save.png",             "t_ui_menu_data_save"} );
    _listElement.push_back( {tag_menu_story,    "Assets/ui/menu/btn_story.png",            "t_ui_menu_store"} );
    _listElement.push_back( {tag_menu_achive,   "Assets/ui/menu/btn_achieve_google.png",   "t_ui_menu_achievements"} );
    _listElement.push_back( {tag_menu_rank,     "Assets/ui/menu/btn_rank_google.png",      "t_ui_rank_title"} );
    _listElement.push_back( {tag_menu_language, "Assets/ui/menu/btn_language.png",         MafUtils::format("t_ui_language_%s", TextManager::getInstance()->getLang().c_str())} );
    _listElement.push_back( {tag_menu_transfer, "Assets/ui/menu/btn_data.png",             "t_ui_menu_transfer"} );
    _listElement.push_back( {tag_menu_share,    "Assets/ui/menu/btn_share.png",            ""});
    _listElement.push_back( {tag_menu_mini,     "",                                        ""} );
    _listElement.push_back( {tag_menu_load,     "Assets/ui/menu/btn_load.png",             "t_ui_menu_data_load"} );
    _listElement.push_back( {tag_menu_setting,  "Assets/ui/menu/btn_setting.png",          "t_ui_menu_setting"} );
}

void MenuPopup::initVarDevil()
{
    _listElement.push_back( {tag_menu_save,     "Assets/ui/menu/btn_save.png",             "t_ui_menu_data_save"} );
    _listElement.push_back( {tag_menu_gift,     "Assets/ui/menu/btn_present.png",          "t_ui_menu_present"} );
    _listElement.push_back( {tag_menu_attend,   "Assets/ui/menu/btn_attendance.png",       "t_ui_menu_attend"} );
    _listElement.push_back( {tag_menu_stat,     "Assets/ui/menu/btn_stats.png",            "t_ui_menu_stat"} );
    _listElement.push_back( {tag_menu_story,    "Assets/ui/menu/btn_story.png",            "t_ui_menu_store"} );
    _listElement.push_back( {tag_menu_achive,   "Assets/ui/menu/btn_achieve_google.png",   "t_ui_menu_achievements"} );
    _listElement.push_back( {tag_menu_rank,     "Assets/ui/menu/btn_rank_google.png",      "t_ui_rank_title"} );
    _listElement.push_back( {tag_menu_empty,    "",                                        ""} );
    _listElement.push_back( {tag_menu_setting,  "Assets/ui/menu/btn_setting.png",          "t_ui_menu_setting"} );
    _listElement.push_back( {tag_menu_bonsikdex,"Assets/ui/menu/btn_book.png",             "t_ui_menu_bonsikdex"} );
    _listElement.push_back( {tag_menu_transfer, "Assets/ui/menu/btn_data.png",             "t_ui_menu_transfer"} );
    _listElement.push_back( {tag_menu_language, "Assets/ui/menu/btn_language.png",         MafUtils::format("t_ui_language_%s", TextManager::getInstance()->getLang().c_str())} );
    _listElement.push_back( {tag_menu_load,     "Assets/ui/menu/btn_load.png",             "t_ui_menu_data_load"} );
    _listElement.push_back( {tag_menu_battery,     "Assets/ui/menu/btn_screensaver.png",   "t_ui_battery"} );
    _listElement.push_back( {tag_menu_friend,   "Assets/ui/menu/btn_friend.png",           "t_ui_menu_friend"} );
    _listElement.push_back( {tag_menu_mini,     "",                                        ""} );
    _listElement.push_back( {tag_menu_share,    "Assets/ui/menu/btn_share.png",            ""});
}

void MenuPopup::initUi()
{
    int size         = (int)_listElement.size();
    
    const int paddingX = 10;
    const int paddingY = 10;
    
    int drawCol = 4;
    int drawRow = size / drawCol;
    if ( size % drawCol != 0 )
    {
        drawRow += 1;
    }
    
    double middleWidth = 175 * drawCol + paddingX * (drawCol - 1);
    double middleHeight = 155 * drawRow + paddingY * (drawRow - 1);
    
    
    Size sizePopup = Size(getContentSize().width, 75 + middleHeight + 350);
    if ( sizePopup.height < 1200 )
    {
        sizePopup.height = 1200;
    }
    
    // size
    _spriteContainer->setContentSize( sizePopup );
    _spriteContainer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _spriteContainer->setPosition(getContentSize().width / 2, 0);
    
    _layerContainerMenuTop = LayerColor::create();
    _layerContainerMenuTop->setContentSize( Size(middleWidth, 67 + 10) );
    _layerContainerMenuTop->setIgnoreAnchorPointForPosition(false);
    _layerContainerMenuTop->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerMenuTop->setPosition( _spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75);
    _spriteContainer->addChild(_layerContainerMenuTop);
    
    _layerContainerMenu = LayerColor::create();
    _layerContainerMenu->setContentSize( Size(middleWidth, middleHeight) );
    _layerContainerMenu->setIgnoreAnchorPointForPosition(false);
    _layerContainerMenu->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerMenu->setPosition( _spriteContainer->getContentSize().width / 2, _layerContainerMenuTop->getPositionY() - _layerContainerMenuTop->getContentSize().height);
    _spriteContainer->addChild(_layerContainerMenu);
    
    _layerContainerGuest = LayerColor::create();
    _layerContainerGuest->setContentSize( Size(694, 125) );
    _layerContainerGuest->setIgnoreAnchorPointForPosition(false);
    _layerContainerGuest->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerGuest->setPosition( _spriteContainer->getContentSize().width / 2, 410);
    _spriteContainer->addChild(_layerContainerGuest);
    
    _layerContainerGames = LayerColor::create();
    _layerContainerGames->setContentSize( Size(694, 170) );
    _layerContainerGames->setIgnoreAnchorPointForPosition(false);
    _layerContainerGames->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerGames->setPosition( _spriteContainer->getContentSize().width / 2, 273);
    _spriteContainer->addChild(_layerContainerGames);
    
    
    // label title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_menu_setting"), GAME_FONT, 28);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5) );
    _spriteContainer->addChild(labelTitle);
    
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), Sprite::create("Assets/ui_common/btn_x.png"), CC_CALLBACK_1(MenuPopup::onClickClose, this));
    itemClose->setPosition(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5);
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
    
    
    //
    uiMenuListTop();
    uiMenuList();
    uiGuest();
    uiGames();
    
    
    // version, id
    {
        auto itemSupporters = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/menu/btn_supporters.png"), nullptr,CC_CALLBACK_1(MenuPopup::onClickOption, this));
        itemSupporters->setPosition(_spriteContainer->getContentSize().width - 20 - itemSupporters->getContentSize().width / 2, 50);
        itemSupporters->setTag(eTag::tag_menu_supporters);
        menu->addChild(itemSupporters);
        
        // version
        std::string strId = "ID = ";
        if ( AccountManager::getInstance()->getUserID().length() != 0 )
            strId.append(AccountManager::getInstance()->getUserID());
        else
            strId.append(GAME_TEXT("t_ui_error_1"));
        auto labelID = Label::createWithSystemFont(strId, "", 25);
        labelID->setAnchorPoint(Vec2(0,0));
        labelID->setColor(COLOR_TITLE);
        labelID->setPosition(Vec2(_spriteContainer->getContentSize().width*0.05, _spriteContainer->getContentSize().height*0.02));
        labelID->setDimensions(_spriteContainer->getContentSize().width*0.55, labelID->getContentSize().height);
        labelID->setAlignment(cocos2d::TextHAlignment::LEFT, cocos2d::TextVAlignment::BOTTOM);
        labelID->setOverflow(cocos2d::Label::Overflow::SHRINK);
        _spriteContainer->addChild(labelID);
        
        // label version
        std::string strVer = "VER = ";
        strVer.append(MafNative::GetGameVersion());
        auto labelVersion = Label::createWithSystemFont(strVer, "", 25);
        labelVersion->setAnchorPoint(Vec2(0,0));
        labelVersion->setColor(COLOR_TITLE);
        labelVersion->setPosition(Vec2(_spriteContainer->getContentSize().width*0.05, _spriteContainer->getContentSize().height*0.045));
        _spriteContainer->addChild(labelVersion);
        
        //
        auto copyBtn = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/menu/btn_copy.png"), nullptr,  [&](Ref* sender){
               MafNative::copyString(AccountManager::getInstance()->getUserID().c_str());
               PopupToast::show("copy complete", 0, UtilsRect::getVisibleSize().height * 0.24);
        });
        copyBtn->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        copyBtn->setPosition(labelID->getPosition() + Vec2::RIGHT * labelID->getContentSize().width + Vec2::DOWN * 10);
        menu->addChild(copyBtn);
    }
}

#pragma mark - ui
void MenuPopup::uiMenuListTop()
{
    //
    _layerContainerMenuTop->removeAllChildren();
    
    //
    const int padding = 10;
    
    auto layerMenu = Layer::create();
    layerMenu->setContentSize(_layerContainerMenuTop->getContentSize());
    layerMenu->setIgnoreAnchorPointForPosition(false);
    layerMenu->setAnchorPoint( Vec2::ANCHOR_MIDDLE );
    layerMenu->setPosition(_layerContainerMenuTop->getContentSize().width / 2, _layerContainerMenuTop->getContentSize().height / 2);
    layerMenu->setScale(0.95f);
    _layerContainerMenuTop->addChild(layerMenu);
    
    for ( int i = 0; i < _listElementTop.size(); i++ )
    {
        double posX = i * 175 + i * padding;
        double posY = layerMenu->getContentSize().height / 2;
        
        element ele = _listElementTop.at(i);

        auto itemSave = ui::Button::create(ele.strImage );
        itemSave->addClickEventListener(CC_CALLBACK_1(MenuPopup::onClickOption, this));
        itemSave->setZoomScale(0.05f);
        itemSave->setPosition( Vec2(posX + itemSave->getContentSize().width / 2, posY) );
        itemSave->setTag(ele.tag);
        layerMenu->addChild(itemSave);
        
        auto label = Label::createWithTTF(GAME_TEXT(ele.strNameKey), GAME_FONT, 22);
        label->setDimensions(itemSave->getBoundingBox().size.width, label->getContentSize().height);
        label->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        label->setOverflow(Label::Overflow::SHRINK);
        label->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
        label->setPosition( Vec2(itemSave->getContentSize().width * 2 / 3 , 15) );
        itemSave->addChild(label, 100, 100);
    }
}

void MenuPopup::uiMenuList()
{
    //
    _layerContainerMenu->removeAllChildren();
    
    //
    const int paddingX = 10;
    const int paddingY = 10;
    const int maxCol = 4;
    
    //
    auto layerMenu = Layer::create();
    layerMenu->setContentSize(_layerContainerMenu->getContentSize());
    layerMenu->setIgnoreAnchorPointForPosition(false);
    layerMenu->setAnchorPoint( Vec2::ANCHOR_MIDDLE );
    layerMenu->setPosition(_layerContainerMenu->getContentSize().width / 2, _layerContainerMenu->getContentSize().height / 2);
    layerMenu->setScale(0.95f);
    _layerContainerMenu->addChild(layerMenu);
    
    //

    for (int i = 0; i < _listElement.size(); i++ )
    {
        int x = i % maxCol;
        int y = i / maxCol;
        double posX = x * 175 + x * paddingX;
        double posY = layerMenu->getContentSize().height - y * 155 - y * paddingY;
        
        element ele = _listElement.at(i);
        if ( ele.tag == MenuPopup::tag_menu_empty )
        {
            continue;
        }
        else if ( ele.tag == MenuPopup::tag_menu_mini )
        {
            for ( int j = 0; j < _listElementMini.size(); j++ )
            {
                element ele = _listElementMini.at(j);
    
                auto smallBtn = ui::Button::create("Assets/ui/menu/btn_s.png");
                smallBtn->addClickEventListener(CC_CALLBACK_1(MenuPopup::onClickOption, this));
                smallBtn->setZoomScale(0.05f);
                smallBtn->setAnchorPoint( Vec2::ANCHOR_TOP_LEFT );
                smallBtn->setPosition(Vec2(posX, posY - 45 * j - 10 * j));
                smallBtn->setTag(ele.tag);
                layerMenu->addChild(smallBtn);
    
                auto label = Label::createWithTTF(GAME_TEXT(ele.strNameKey), GAME_FONT, 24);
                label->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
                label->setPosition( Vec2(smallBtn->getContentSize().width / 2, 5) );
                label->setDimensions(smallBtn->getBoundingBox().size.width - 5, label->getContentSize().height);
                label->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
                label->setOverflow(Label::Overflow::SHRINK);
                smallBtn->addChild(label, 100, 100);
                
                if ( ele.tag == eTag::tag_menu_login )
                {
                    auto uiReddot = UIReddot::create(E_REDDOT_TYPE::NOMAL, Vec2::ANCHOR_MIDDLE);
                    uiReddot->setPosition(Vec2(10, smallBtn->getContentSize().height - 5));
                    smallBtn->addChild(uiReddot, 100, 100);
                }
            }
        }
        else
        {
            auto itemSave = ui::Button::create(ele.strImage);
            itemSave->addClickEventListener(CC_CALLBACK_1(MenuPopup::onClickOption, this));
            itemSave->setAnchorPoint( Vec2::ANCHOR_TOP_LEFT );
            itemSave->setPosition(Vec2(posX, posY));
            itemSave->setZoomScale(0.05f);
            itemSave->setTag(ele.tag);
            layerMenu->addChild(itemSave);
            
            auto label = Label::createWithTTF(GAME_TEXT(ele.strNameKey), GAME_FONT, 24);
            label->setDimensions(itemSave->getBoundingBox().size.width - 5, label->getContentSize().height);
            label->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
            label->setOverflow(Label::Overflow::SHRINK);
            label->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
            label->setPosition( Vec2(itemSave->getContentSize().width / 2, 10) );
            itemSave->addChild(label, 100, 100);
            if ( ele.tag == tag_menu_share )
            {
                label->setString("share");
            }
            
            //
            if ( ele.tag == tag_menu_attend )
            {
                _uiReddotAttent = UIReddot::create(E_REDDOT_TYPE::NOMAL, Vec2::ANCHOR_MIDDLE);
                _uiReddotAttent->setPosition( Vec2(20, itemSave->getContentSize().height-15) );
                itemSave->addChild(_uiReddotAttent, 100, 100);
                
                _uiReddotAttent->setVisible(false);
                if(UserInfoManager::getInstance()->getAttendHave() == true)
                {
                    _uiReddotAttent->setVisible(true);
                }
            }
            else if ( ele.tag == tag_menu_gift )
            {
                _uiReddotGift = UIReddot::create(E_REDDOT_TYPE::NOMAL, Vec2::ANCHOR_MIDDLE);
                _uiReddotGift->setPosition( Vec2(20, itemSave->getContentSize().height-15) );
                itemSave->addChild(_uiReddotGift,100, 100);
                
                
                _uiReddotGift->setVisible(false);
                if(UserInfoManager::getInstance()->getGiftHave() == true)
                {
                    _uiReddotGift->setVisible(true);
                }
            }
            else if ( ele.tag == MenuPopup::tag_menu_save )
            {
                int diff = UserInfoConfigManager::getInstance()->getSaveTimeDiff();
                if ( diff > 0 )
                {
                    if ( diff >= 30 )       diff = 30;
                    else if ( diff >= 15 )  diff = 15;
                    else if ( diff >= 7 )   diff = 7;
                    else if ( diff >= 3 )   diff = 3;
                    else                    diff = 1;
                    
                    auto spriteBG = Sprite::create("Assets/ui/menu/btn_save_day.png");
                    spriteBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
                    spriteBG->setPosition(itemSave->getContentSize().width / 2, itemSave->getContentSize().height - 82);
                    itemSave->addChild(spriteBG);
                    
                    auto labelDay = MafNode::createBmFont(MafUtils::format("D+%d", diff), GAME_BM_FONT_AL, 20);
                    labelDay->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
                    labelDay->setPosition(spriteBG->getContentSize().width - 10, spriteBG->getContentSize().height / 2);
                    labelDay->setColor(Color3B::BLACK);
                    spriteBG->addChild(labelDay);
                }
            }
            else if ( ele.tag == MenuPopup::tag_menu_bonsikdex )
            {
                _uiReddotBonsikdex = UIReddot::create(E_REDDOT_TYPE::NOMAL, Vec2::ANCHOR_MIDDLE);
                _uiReddotBonsikdex->setPosition( Vec2(20, itemSave->getContentSize().height-15) );
                _uiReddotBonsikdex->setVisible(false);
                itemSave->addChild(_uiReddotBonsikdex,100, 100);
                
                RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::DEX_MONSTER_LEVELUP, _uiReddotBonsikdex);
            }
        }
    }
}

void MenuPopup::uiGuest()
{
    auto layout = _layerContainerGuest;
    auto size = layout->getContentSize();
    
    //
    layout->removeAllChildren();
    layout->setVisible(true);
    
    
    
    bool bGuest = AccountManager::getInstance()->isGuest();
    if ( bGuest == false )
    {
        layout->setVisible(false);
    }
    
    auto uiBG = ui::ImageView::create("Assets/ui_common/base_box_1_14.png");
    uiBG->setScale9Enabled(true);
    uiBG->setContentSize(size);
    uiBG->setPositionType(ui::Widget::PositionType::PERCENT);
    uiBG->setPositionPercent(Vec2::ANCHOR_MIDDLE);
    layout->addChild(uiBG);
    
    auto uiText = ui::CText::create(GAME_TEXT("t_ui_login_guest_login_setting_msg"), GAME_FONT, 24);
    uiText->setPositionType(ui::Widget::PositionType::PERCENT);
    uiText->setPositionPercent(Vec2::ANCHOR_MIDDLE);
    uiText->setTextAreaSize(Size(size.width - 10, size.height - 10));
    uiText->setTextOverflow(Label::Overflow::SHRINK);
    uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
    uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
    uiText->setTextColor(Color4B(255, 0, 0, 255));
    layout->addChild(uiText);
}

void MenuPopup::uiGames()
{
    //
    _layerContainerGames->removeAllChildren();
    
    //
    auto spriteMoreBG = ui::Scale9Sprite::create("Assets/ui_common/base_box_2.png", Rect::ZERO, Rect(70,110,10,10));
    spriteMoreBG->setContentSize( _layerContainerGames->getContentSize() );
    spriteMoreBG->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    spriteMoreBG->setPosition(_layerContainerGames->getContentSize().width / 2, 0);
    _layerContainerGames->addChild(spriteMoreBG);
    {
        auto spriteTitleBG = Sprite::create("Assets/ui/menu/line_more.png");
        spriteTitleBG->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        spriteTitleBG->setPosition(3, spriteMoreBG->getContentSize().height - 3);
        spriteMoreBG->addChild(spriteTitleBG);
        
        auto labelTitle = Label::createWithSystemFont("More Games", GAME_FONT, 25);
        labelTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        labelTitle->setPosition(10, spriteTitleBG->getContentSize().height * 0.5f);
        labelTitle->enableOutline(Color4B::BLACK, 2);
        spriteTitleBG->addChild(labelTitle);
    }
    {
        auto layerScroll = extension::ScrollView::create(Size(spriteMoreBG->getContentSize().width - 6, spriteMoreBG->getContentSize().height - 40 - 6) );
        layerScroll->setDirection(extension::ScrollView::Direction::HORIZONTAL);
        layerScroll->setIgnoreAnchorPointForPosition(false);
        layerScroll->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        layerScroll->setPosition(3, 3);
        spriteMoreBG->addChild(layerScroll);

        auto layerContainer = Layer::create();
        layerContainer->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        layerContainer->setPosition(Vec2(3, 3));
        layerScroll->setContainer(layerContainer);

        auto menu = MafNode::MafMenu::create();
        menu->setAnchorPoint(Vec2::ZERO);
        menu->setPosition(Vec2::ZERO);
        layerContainer->addChild(menu);
        

        _vecMoreGameLink.clear();
        std::function<void (int)> callback = [this](int index) {
            MafNative::OpenUrl(_vecMoreGameLink.at(index).c_str());
        };
        
        std::string strText = TableManager::getInstance()->getTableData(E_TABLE::GAME_LIST);

        rapidjson::Document jsonParser;
        jsonParser.Parse<0>((char*)strText.c_str());
        if ( strText.length() != 0 && jsonParser.HasParseError() == false  )
        {
            for ( int i = 0; i < jsonParser.Size(); i++ )
            {
                const rapidjson::Value& jsonValue = jsonParser[i];
                
                bool bInclude = false;
                std::string strCountry = jsonValue["country"].GetString();
                std::string strCountryExcept = jsonValue["country_except"].GetString();
                std::string strPlatform = "";
                if ( jsonValue.HasMember("platform") == true )
                {
                    strPlatform = jsonValue["platform"].GetString();
                }
                
                if ( strCountry.length() == 0 )
                {
                    continue;
                }
                
                if ( strCountry.compare("all") == 0 )
                {
                    bInclude = true;
                }
                else
                {
                    std::string strLang = TextManager::getInstance()->getLang();
                    std::vector<std::string> listLang = MafUtils::split(strCountry, ',');
                    if ( std::find(listLang.begin(), listLang.end(), strLang) != listLang.end() )
                    {
                        bInclude = true;
                    }
                }
                if(bInclude == true)
                {
                    if(strPlatform.compare("all") != 0)
                    {
                    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
                        if(strPlatform.compare("ios") != 0)
                        {
                            bInclude = false;
                        }
                    #elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                        if(strPlatform.compare("aos") != 0)
                        {
                            bInclude = false;
                        }
                    #endif
                    }
                }
                
                if ( bInclude == true && strCountryExcept.length() != 0 )
                {
                    std::string strLang = TextManager::getInstance()->getLang();
                    std::vector<std::string> listLang = MafUtils::split(strCountryExcept, ',');
                    if ( std::find(listLang.begin(), listLang.end(), strLang) != listLang.end() )
                    {
                        bInclude = false;
                    }
                }
                
                if( bInclude == false )
                    continue;
                
                auto spriteIcon = Sprite::create(MafUtils::format("Assets/ui/menu/moregames/%s", jsonValue["file_name"].GetString()));
                if ( spriteIcon == nullptr )
                {
                    continue;
                }
                spriteIcon->setScale(120.f / spriteIcon->getContentSize().width);

                auto itemIcon = MafNode::MafMenuItemSprite::create(spriteIcon, nullptr, nullptr);
                itemIcon->setCallback(std::bind(callback, _vecMoreGameLink.size()));
                itemIcon->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
                itemIcon->setPosition(_vecMoreGameLink.size() * (120 + 10), 0);
                menu->addChild(itemIcon);
                
                //
                _vecMoreGameLink.push_back(jsonValue["link"].GetString());
            }
        }
        
        
        
      
        //
        layerContainer->setContentSize(Size(_vecMoreGameLink.size() * (120 + 10), layerScroll->getViewSize().height));
    }
}

#pragma mark -
void MenuPopup::showMailPopup(void)
{
    auto popup = MailPopup::create();
    popup->show();
}

void MenuPopup::showStatPopup(void)
{
    if ( UserInfoManager::getInstance()->getCurrentPlace() == E_PLACE::DG_NORMAL )
    {
        PopupStat *popup = PopupStat::create(DG_NORMAL);
        popup->show();
    }
    else if ( UserInfoManager::getInstance()->getCurrentPlace() == E_PLACE::DG_DEVIL_DOM )
    {
        PopupStat *popup = PopupStat::create(DG_DEVIL_DOM);
        popup->show();
    }
}

void MenuPopup::showAttendPopup(void)
{
    auto popup = PopupAttend::create();
    popup->show();
}
void MenuPopup::showSupportersPopup(void)
{
    auto popup = PopupSupporters::create();
    popup->show();
}
#pragma mark - click
void MenuPopup::onClickClose(Ref* sender)
{
    hide();
}

void MenuPopup::onClickOption(Ref* sender)
{
    auto menuItem = (MenuItem*)sender;
    
    // sound
    SoundManager::SoundEffectStart(SOUND_SELECT);
    
    // event
    switch (menuItem->getTag()) {
        case tag_menu_save:
        {
            auto popup = PopupCloudSaveLoad::create(true);
            popup->show();
            break;
        }
        case tag_menu_facebook:
        {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            std::string url = "https://www.facebook.com/";
            if ( TextManager::getInstance()->getLang().compare("ko") == 0 )
            {
                url.append(FACEBOOK_PAGE_ID);
            }
            else
            {
                url.append(FACEBOOK_PAGE_ID_BILLYBOB);
            }
            
            MafNative::OpenUrl(url.c_str());
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            std::string url = "";
            if ( TextManager::getInstance()->getLang().compare("ko") == 0 )
            {
                url.append(FACEBOOK_PAGE_ID);
            }
            else
            {
                url.append(FACEBOOK_PAGE_ID_BILLYBOB);
            }
            MafShareSNS::OpenFacebookPage(url.c_str());
#endif
            break;
        }
        case tag_menu_naver:
        {
            MafNative::OpenUrl("https://cafe.naver.com/gimbongsik");
            break;
        }
        case tag_menu_twitter:
        {
            MafNative::OpenUrl("https://twitter.com/princesss_ema");
            break;
        }
        case tag_menu_jpwiki:
        {
            MafNative::OpenUrl("https://gamerch.com/yasuhiro/entry/67398");
            break;
        }
        case tag_menu_faq:
        {
            MafNative::OpenUrl("https://yasuhiroknight.com/newsandfaq/");
            break;
        }
        case tag_menu_discord:
        {
            MafNative::OpenUrl("https://discord.gg/NPhhTdc");
            break;
        }
        case tag_menu_mail:
        {
            auto popup = PopupContactUs::create();
            popup->show();
            break;
        }
        case tag_menu_gift:
        {
            //
            if ( AccountManager::getInstance()->getUserIdx() == 0 )
            {
                auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_1"));
                popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
                popup->show();
                return;
            }
            
            //
            showMailPopup();
            break;
        }
        case tag_menu_attend:
        {
            //
            if ( AccountManager::getInstance()->getUserIdx() == 0 )
            {
                auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_1"));
                popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
                popup->show();
                return;
            }
            
            if ( _uiReddotAttent != nullptr )
            {
                _uiReddotAttent->setVisible(false);
            }
            showAttendPopup();
            break;
        }
        case tag_menu_stat:
        {
            showStatPopup();
            break;
        }
        case tag_menu_rank:
        {
            if ( AccountManager::getInstance()->isGuest() == true )
            {
                auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_transfer_guest_login_error"));
                popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
                popup->show();
                return;
            }
            
            if ( MafGooglePlay::IsSignedIn() == false )
            {
                auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_1"));
                popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
                popup->show();
                return;
            }
            
            if ( UserInfoManager::getInstance()->getCurrentPlace() == E_PLACE::DG_NORMAL )
            {
                int level = 0;
                E_ARTIFACT eArti = E_ARTIFACT::ARTI_1_EVOL1;
                for(int i= E_ARTIFACT::ARTI_1_EVOL1; i< E_ARTIFACT_999_NONE; i++)
                {
                    eArti = (E_ARTIFACT)i;
                    if(DataManager::GetArtifactType(eArti) == E_ARTIFACT_TYPE::ARTI_TYPE_ATK && UserInfoManager::getInstance()->getArtifactLevel(E_PLACE::DG_NORMAL, eArti)>0)
                        level += UserInfoManager::getInstance()->getArtifactLevel(E_PLACE::DG_NORMAL, eArti);
                }
                MafGooglePlay::UpdateLeaderBoardScore(LEADERBOARD_ARTIFACT1, level);
                
                level = 0;
                eArti = E_ARTIFACT::ARTI_1_EVOL1;
                for(int i= E_ARTIFACT::ARTI_1_EVOL1; i< E_ARTIFACT_999_NONE; i++)
                {
                    eArti = (E_ARTIFACT)i;
                    if(DataManager::GetArtifactType(eArti) == E_ARTIFACT_TYPE::ARTI_TYPE_KILL && UserInfoManager::getInstance()->getArtifactLevel(E_PLACE::DG_NORMAL, eArti)>0)
                        level += UserInfoManager::getInstance()->getArtifactLevel(E_PLACE::DG_NORMAL, eArti);
                }
                MafGooglePlay::UpdateLeaderBoardScore(LEADERBOARD_ARTIFACT4, level);
                
                int revival = UserInfoManager::getInstance()->getRevival(UserInfoManager::getInstance()->getCurrentPlace());
                revival-=1;
                MafGooglePlay::UpdateLeaderBoardScore(LEADERBOARD_REVIVE, revival);
                
                std::string data;
                data = UserDefault::getInstance()->getStringForKey(DATA_TOTAL_FLOOR);
                if(!data.empty()){
                    data = MafAes256::Decrypt(data);
                    int total = atoi(data.c_str());
                    MafGooglePlay::UpdateLeaderBoardScore(LEADERBOARD_TOTAL, total);
                }
            }
            
            MafGooglePlay::ShowLeaderBoardAll();
            break;
        }
        case tag_menu_achive:
        {
            if ( AccountManager::getInstance()->isGuest() == true )
            {
                auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_transfer_guest_login_error"));
                popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
                popup->show();
                return;
            }
            
            if ( MafGooglePlay::IsSignedIn() == false )
            {
                auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_1"));
                popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
                popup->show();
                return;
            }
            
            MafGooglePlay::ShowAchievements();
            break;
        }
        case tag_menu_story:
        {
            auto popup = PopupStoryListDefault::create();
            popup->show();
            break;
        }
        case tag_menu_rate:
            MafNative::OpenUrl(URL_THISGAME);
            break;
        case tag_menu_rule_terms:
        {
            std::string strLang = TextManager::getInstance()->getLang();
            std::string strText = ParameterManager::getInstance()->getParameterString("terms_link");
            
            rapidjson::Document jsonParser;
            jsonParser.Parse<0>((char*)strText.c_str());
            if ( strText.length() != 0 && jsonParser.HasParseError() == false )
            {
                std::string strKey = "en";
                if ( jsonParser.HasMember(strLang.c_str()) == true )
                {
                    strKey = strLang;
                }
                
                MafNative::OpenUrl(jsonParser[strKey.c_str()].GetString());
            }
        }
            break;
        case tag_menu_rule_privacy:
        {
            std::string strLang = TextManager::getInstance()->getLang();
            std::string strText = ParameterManager::getInstance()->getParameterString("privacy_link");
            
            rapidjson::Document jsonParser;
            jsonParser.Parse<0>((char*)strText.c_str());
            if ( strText.length() != 0 && jsonParser.HasParseError() == false )
            {
                std::string strKey = "en";
                if ( jsonParser.HasMember(strLang.c_str()) == true )
                {
                    strKey = strLang;
                }
                
                MafNative::OpenUrl(jsonParser[strKey.c_str()].GetString());
            }
        }
            break;
        case tag_menu_setting:
        {
            auto popup = PopupSettings::create();
            popup->show();
            break;
        }
        case tag_menu_bonsikdex:
        {
            MafAnalyticsManager::LogEvent(kAnalPediaEnter, kRepeatTrue);
            
            auto popup = PopupBonsikDexDefault::create();
            popup->show();
            break;
        }
        case tag_menu_push:
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_menu_msg_17"));
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
            break;
        }
        case tag_menu_load:
        {
            //
            if ( AccountManager::getInstance()->getUserIdx() == 0 )
            {
                auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_1"));
                popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
                popup->show();
                return;
            }
            
            //
            auto popup = PopupCloudSaveLoad::create(false);
            popup->show();
            break;
        }
        case tag_menu_language:
        {
            auto popup = PopupLanguage::create();
            popup->show();
            break;
        }
        case tag_menu_login:
        {
            auto popup = PopupUserLoginGuestAgain::create();
            popup->show();
            break;
        }
        case tag_menu_supporters:
        {
            showSupportersPopup();
            break;
        }
        case tag_menu_transfer:
        {
            if ( AccountManager::getInstance()->isGuest() == true )
            {
                auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_transfer_guest_login_error"));
                popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
                popup->show();
                return;
            }
            
            //
            if ( AccountManager::getInstance()->getUserIdx() == 0 )
            {
                auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_1"));
                popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
                popup->show();
                return;
            }
            
            //
            if ( this->getParent() != nullptr )
            {   
                auto popup = PopupAccountTransfer::create();
                popup->show();
            }
            break;
        }
        case tag_menu_battery:
        {
            hide();
            
            PopupScreenLock::show();
            break;
        }
        case tag_menu_friend:
        {
            if ( AccountManager::getInstance()->getUserIdx() == 0 )
            {
                auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_1"));
                popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
                popup->show();
                return;
            }
            else
            {
                auto popup = PopupFriend::create();
                popup->show();
            }
            break;
        }
        case tag_menu_lobi:
        {
            MafNative::OpenUrl("https://web.lobi.co/game/chunenkishi_yasuhiro/group/9c2bc16898b2885a3e39e808fcf8aa6150c90cbd");
            break;
        }
        case tag_menu_share:
        {
            MafNative::shareString("https://moneyhero.page.link/47Ac");
            break;
        }
        case tag_menu_withdrawal:
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_withdrawal_title"), GAME_TEXT("t_ui_withdrawal_info_1"));
            popup->addButton(GAME_TEXT("t_ui_withdrawal_bt_1"), false, [=](Ref*){
                auto popup = PopupWithdrawal::create();
                popup->show();
            });
            popup->addButton(GAME_TEXT("t_ui_cancel"), true, nullptr);
            popup->show();
            break;
        }
        default:
            break;
    }
    
}

#pragma mark - callback

#pragma mark - network
 
 

