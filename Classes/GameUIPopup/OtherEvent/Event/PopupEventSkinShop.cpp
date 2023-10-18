//
//  PopupEventSkinShop.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/10/13.
//

#include "PopupEventSkinShop.h"

#include "GameUIPopup/Other/CostumeSkin/PopupCostumeSkin.h"

#include "ManagerEvent/EventManager.h"
#include "ManagerGame/CostumeSkinManager.h"

USING_NS_CC;

PopupEventSkinShop* PopupEventSkinShop::create()
{
    PopupEventSkinShop* pRet = new(std::nothrow) PopupEventSkinShop();
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

PopupEventSkinShop::PopupEventSkinShop(void)
{
}

PopupEventSkinShop::~PopupEventSkinShop(void)
{
}

bool PopupEventSkinShop::init()
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    initVar();
    initUI();
    

    return true;
}
void PopupEventSkinShop::initVar()
{
    double nWidth = 734;
    double nHeight = getContentSize().height;
    if ( nHeight > 700 )
        nHeight = 700;
    
    _spriteContainer->setContentSize(Size(nWidth, nHeight));
}

void PopupEventSkinShop::initUI()
{
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"),nullptr, CC_CALLBACK_1(PopupEventSkinShop::onClickButton, this));
    itemClose->setPosition( Vec2(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5) );
    itemClose->setTouchRect( itemClose->getContentSize());
    itemClose->setName("CLOSE");
    menu->addChild(itemClose);
    
    auto lbTitle = Label::createWithTTF(GAME_TEXT("t_ui_raid_event_reward_title"), GAME_FONT, 30);
    lbTitle->setColor(COLOR_TITLE);
    lbTitle->setPosition(Vec2(_spriteContainer->getContentSize().width/2, _spriteContainer->getContentSize().height - 40));
    _spriteContainer->addChild(lbTitle);
    
    auto sprShop = ui::Scale9Sprite::create("Assets/ui_common/btn_2_1_on.png", Rect::ZERO);
    sprShop->setContentSize(Size(300, 100));
    
    auto itemShop = MafNode::MafMenuItemSprite::create(sprShop, nullptr, CC_CALLBACK_1(PopupEventSkinShop::onClickButton, this));
    itemShop->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    itemShop->setPosition( Vec2(_spriteContainer->getContentSize().width/2, 30));
    itemShop->setTouchRect( sprShop->getContentSize());
    itemShop->setName("SKIN_SHOP");
    menu->addChild(itemShop);
    
    auto lbShop = Label::createWithTTF(GAME_TEXT("t_ui_raid_event_reward_goto"), GAME_FONT, 30);
    lbShop->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    lbShop->setHorizontalAlignment(TextHAlignment::CENTER);
    lbShop->setVerticalAlignment(TextVAlignment::CENTER);
    lbShop->setDimensions(itemShop->getContentSize().width-10, itemShop->getContentSize().height-5);
    lbShop->setOverflow(Label::Overflow::SHRINK);
    lbShop->setPosition(itemShop->getContentSize()/2);
    itemShop->addChild(lbShop);
    
    initUIContents();
}

void PopupEventSkinShop::initUIContents()
{
//    auto sprSkinBg = ui::Scale9Sprite::create("Assets/ui_common/base_text_1_1.png");
//    sprSkinBg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//    sprSkinBg->setContentSize(Size(700, 206));
//    sprSkinBg->setPosition(_spriteContainer->getContentSize().width/2, _spriteContainer->getContentSize().height - _spriteContainer->getContentSize().height / 3.5f);
//    _spriteContainer->addChild(sprSkinBg);
    
    Vector<InfoCostumeSkin*> listSkin;
    
    std::string strEventOtherText = EventManager::getInstance()->getEventOhterDataByEventIdx(26);
    if ( strEventOtherText.empty() == false )
    {
        rapidjson::Document jsonParser;
        jsonParser.ParseInsitu<0>((char*)strEventOtherText.c_str());
        if( jsonParser.HasParseError() == false )
        {
            for ( int i = 0; i < jsonParser["info_skin"].Size(); i++ )
            {
                auto objSkin = CostumeSkinManager::getInstance()->getInfoSkin(jsonParser["info_skin"][i].GetInt());
                if ( objSkin != nullptr )
                {
                    listSkin.pushBack(objSkin);
                }
            }
        }
    }
    
    for ( int i = 0; i < listSkin.size(); i++ )
    {
        auto objSkin = listSkin.at(i);
        
        auto uiBtnBG = ui::Button::create("Assets/ui/costume/skin_box1_1.png");
        uiBtnBG->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        uiBtnBG->setZoomScale(0);
        _spriteContainer->addChild(uiBtnBG);
        
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

        double widthTotal = (listSkin.size() * uiBtnBG->getContentSize().width) + ((listSkin.size() - 1) * 10);
        float startX = _spriteContainer->getContentSize().width / 2 - widthTotal / 2;
        Vec2 pos = Vec2(startX + i * uiBtnBG->getContentSize().width + i * 10, _spriteContainer->getContentSize().height - 95 );

        uiBtnBG->setPosition(pos);
    }
    
    auto lbContent = Label::createWithTTF(GAME_TEXT("t_ui_event_boardgame_main_reward_desc"), GAME_FONT, 25);
    lbContent->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    lbContent->setPosition(_spriteContainer->getContentSize().width/2, 130);
    lbContent->setDimensions(_spriteContainer->getContentSize().width - 100, 240);
    lbContent->setOverflow(Label::Overflow::SHRINK);
    lbContent->setHorizontalAlignment(TextHAlignment::CENTER);
    lbContent->setVerticalAlignment(TextVAlignment::CENTER);
    _spriteContainer->addChild(lbContent);
}

void PopupEventSkinShop::onClickButton(Ref* sender)
{
    
    Node* item = (Node*)sender;
    if(item->getName().compare("CLOSE") == 0)
    {
        hide();
    }
    else if(item->getName().compare("SKIN_SHOP") == 0)
    {
        auto popup = PopupCostumeSkin::create();
        popup->setTabType(PopupCostumeSkin::E_TYPE::SHOP);
        popup->show();
        
        hide();
    }
    
}

