//
//  Popup7thInformation.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/12/03.
//

#include "Popup7thInformation.h"

#include "ManagerEvent/EventManager.h"
#include "ManagerGame/CostumeSkinManager.h"

USING_NS_CC;

Popup7thInformation* Popup7thInformation::create()
{
    Popup7thInformation* pRet = new(std::nothrow) Popup7thInformation();
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

Popup7thInformation::Popup7thInformation():

_uiContainerContents(nullptr)
{
    
}

Popup7thInformation::~Popup7thInformation()
{
    
}

bool Popup7thInformation::init()
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    //
    initVar();
    initUI();
    
    return true;
}

void Popup7thInformation::initVar()
{
    
}

void Popup7thInformation::initUI()
{
    // size
    Size size = Size(650, 1030);
    
    _spriteContainer->setContentSize(size);
    
    // label title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_coevt_ready_title"), GAME_FONT, 32);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5);
    _spriteContainer->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(Popup7thInformation::onClickClose, this));
    itemClose->setPosition(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5);
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
    
    //
    _uiContainerContents = ui::Layout::create();
    _uiContainerContents->setContentSize(Size(size.width - 25, size.height - 75 - 25));
    _uiContainerContents->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _uiContainerContents->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75) );
    _uiContainerContents->setLayoutType(ui::Layout::Type::VERTICAL);
    _spriteContainer->addChild(_uiContainerContents);
    
    
    //
    uiContent();
}
 
 
#pragma mark - ui
void Popup7thInformation::uiContent()
{
    _uiContainerContents->removeAllChildren();
 
    auto uiTextTop = ui::CText::create(GAME_TEXT("t_ui_evt_welcome"), GAME_FONT, 32);
    _uiContainerContents->addChild(uiTextTop);
    {
        auto params = ui::LinearLayoutParameter::create();
        params->setGravity(ui::LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);
        params->setMargin(ui::Margin(0.0f, 25.0f, 0.0f, 0.0f));
        uiTextTop->setLayoutParameter(params);
    }
    
    // skin
    auto objSkin = CostumeSkinManager::getInstance()->getInfoSkin(88);
    if ( objSkin != nullptr )
    {
        auto uiImageSkin = ui::ImageView::create(MafUtils::format("Assets/character_hero/skin/skin_%d_walk_1.png", objSkin->getIdx()));
        _uiContainerContents->addChild(uiImageSkin);
        {
            auto params = ui::LinearLayoutParameter::create();
            params->setGravity(ui::LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);
            params->setMargin(ui::Margin(0.0f, 25.0f, 0.0f, 0.0f));
            uiImageSkin->setLayoutParameter(params);
        }
        
        // skin info
        std::string strSkinInfo = "";
        strSkinInfo += objSkin->getName();
        if ( objSkin->getBuffType().compare("ALL_DAMAGE") == 0 && objSkin->getBuffStrength() > 0 )
        {
            strSkinInfo += "\n(";
            strSkinInfo += GAME_TEXTFORMAT("t_costume_skin_option", (int)(objSkin->getBuffStrength() * 100));
            strSkinInfo += ")";
        }
        
        auto uiTextSkinInfo = ui::CText::create(strSkinInfo, GAME_FONT, 24);
        uiTextSkinInfo->setTextAreaSize(Size(_uiContainerContents->getContentSize().width, 80));
        uiTextSkinInfo->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiTextSkinInfo->setTextHorizontalAlignment(TextHAlignment::CENTER);
        ((Label*)uiTextSkinInfo->getVirtualRenderer())->setOverflow(Label::Overflow::SHRINK);
        _uiContainerContents->addChild(uiTextSkinInfo);
        {
            auto params = ui::LinearLayoutParameter::create();
            params->setGravity(ui::LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);
            params->setMargin(ui::Margin(0.0f, -50.0f, 0.0f, 0.0f));
            uiTextSkinInfo->setLayoutParameter(params);
        }
    }
    
    auto uiTextInfo = ui::CText::create(GAME_TEXT("t_ui_coevt_ready_text"), GAME_FONT, 24);
    uiTextInfo->setTextAreaSize(Size(_uiContainerContents->getContentSize().width, 200));
    uiTextInfo->setTextVerticalAlignment(TextVAlignment::CENTER);
    uiTextInfo->setTextHorizontalAlignment(TextHAlignment::CENTER);
    ((Label*)uiTextInfo->getVirtualRenderer())->setOverflow(Label::Overflow::SHRINK);
    _uiContainerContents->addChild(uiTextInfo);
    {
        auto params = ui::LinearLayoutParameter::create();
        params->setGravity(ui::LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);
        params->setMargin(ui::Margin(0.0f, 25.0f, 0.0f, 0.0f));
        uiTextInfo->setLayoutParameter(params);
    }
    
    
    auto uiTextTime = ui::CText::create(GAME_TEXTFORMAT("t_ui_raid_event_time_remain", 0,0,0), GAME_FONT, 24);
    uiTextTime->setTextColor(Color4B(255, 255, 0, 255));
    _uiContainerContents->addChild(uiTextTime);
    {
        auto params = ui::LinearLayoutParameter::create();
        params->setGravity(ui::LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);
        params->setMargin(ui::Margin(0.0f, 50.0f, 0.0f, 0.0f));
        uiTextTime->setLayoutParameter(params);
        
        auto action1 = CallFuncN::create([=](Node* sender){
            
            auto objEvent = EventManager::getInstance()->getActivatedEvent(30);
            if ( objEvent == nullptr )
            {
                return;
            }
            
            auto timeDiff = objEvent->getEventEnd() - UtilsDate::getInstance()->getTime();
            if ( timeDiff <= 0 )
                timeDiff = 0;
            
            int32_t h = (int32_t)(timeDiff / 3600);
            int32_t m = timeDiff % 3600 / 60;
            int32_t s = timeDiff % 3600 % 60;
            
            auto uiTextTime = (ui::CText*)sender;
            uiTextTime->setString(GAME_TEXTFORMAT("t_ui_raid_event_time_remain", h, m, s));
        });
        auto action2 = DelayTime::create(1.0f);
        auto seq = Sequence::create(action1, action2, NULL);
        uiTextTime->runAction(RepeatForever::create(seq));
    }
    
    
    auto uiTextCondition = ui::CText::create(GAME_TEXTFORMAT("t_ui_evt_condition", 300, 1), GAME_FONT, 24);
    uiTextCondition->setTextAreaSize(Size(_uiContainerContents->getContentSize().width, 80));
    uiTextCondition->setTextVerticalAlignment(TextVAlignment::CENTER);
    uiTextCondition->setTextHorizontalAlignment(TextHAlignment::CENTER);
    ((Label*)uiTextCondition->getVirtualRenderer())->setOverflow(Label::Overflow::SHRINK);
    _uiContainerContents->addChild(uiTextCondition);
    {
        auto params = ui::LinearLayoutParameter::create();
        params->setGravity(ui::LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);
        params->setMargin(ui::Margin(0.0f, 50.0f, 0.0f, 0.0f));
        uiTextCondition->setLayoutParameter(params);
    }
    
    auto uiBtnOk = ui::Button::create("Assets/ui_common/btn_1_1_on.png");
    uiBtnOk->setScale9Enabled(true);
    uiBtnOk->setCapInsets(Rect(33,33,2,2));
    uiBtnOk->setContentSize(Size(150, 80));
    uiBtnOk->setTitleText(GAME_TEXT("t_ui_confirm"));
    uiBtnOk->setTitleFontSize(28);
    uiBtnOk->addClickEventListener(CC_CALLBACK_1(Popup7thInformation::onClickClose, this));
    _uiContainerContents->addChild(uiBtnOk);
    {
        auto params = ui::LinearLayoutParameter::create();
        params->setGravity(ui::LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);
        params->setMargin(ui::Margin(0.0f, 50.0f, 0.0f, 0.0f));
        uiBtnOk->setLayoutParameter(params);
    }
     
}

#pragma mark - set, get

#pragma mark - callback
 
#pragma mark - click
void Popup7thInformation::onClickClose(Ref* sender)
{
    hide();
}
 
