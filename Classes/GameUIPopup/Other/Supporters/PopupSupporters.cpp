//
//  PopupSupporters.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 25/06/2019.
//

#include "PopupSupporters.h"

USING_NS_CC;

PopupSupporters* PopupSupporters::create()
{
    PopupSupporters* pRet = new(std::nothrow) PopupSupporters();
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
PopupSupporters::PopupSupporters(void)
{
    
}

PopupSupporters::~PopupSupporters(void)
{
    
}

bool PopupSupporters::init()
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    
    // init
    initUi();
    
    
    return true;
}
void PopupSupporters::initUi()
{
    _spriteContainer->setContentSize( Size(729,848) );
    
    MafNode::MafMenu *menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"),
                                               Sprite::create("Assets/ui_common/btn_x.png"),
                                               CC_CALLBACK_1(PopupSupporters::onClickClose, this));
    itemClose->setPosition(Vec2(_spriteContainer->getContentSize().width-itemClose->getContentSize().width,
                                _spriteContainer->getContentSize().height-itemClose->getContentSize().height*0.8));
    itemClose->setAnchorPoint(Vec2(0.5,0.5));
    itemClose->setTouchRect(itemClose->getContentSize()*2);
    menu->addChild(itemClose);
    
    auto lbTitle = Label::createWithTTF("Supporters", GAME_FONT, 36);
    lbTitle->setAnchorPoint(Vec2(0.5,0.5));
    lbTitle->setColor(COLOR_TITLE);
    lbTitle->setPosition( Vec2(_spriteContainer->getContentSize().width/2,_spriteContainer->getContentSize().height - 37.5) );
    _spriteContainer->addChild(lbTitle);
    
    
    std::string contact = GAME_TEXT("t_ui_supporters_contact").c_str();
    auto lbContact = Label::createWithTTF(contact, GAME_FONT, 25);
    lbContact->setAnchorPoint(Vec2(0.5,0.5));
    lbContact->setPosition( Vec2(_spriteContainer->getContentSize().width/2,_spriteContainer->getContentSize().height/9) );
    lbContact->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    _spriteContainer->addChild(lbContact);
    
    auto lbContact2 = Label::createWithTTF("support@midagedknight.zendesk.com", GAME_FONT, 25);
    lbContact2->setAnchorPoint(Vec2(0.5,0));
    lbContact2->setColor(COLOR_TITLE);
    lbContact2->setPosition( Vec2(_spriteContainer->getContentSize().width/2,lbContact->getPositionY()-lbContact->getHeight()/2 - 70) );
    _spriteContainer->addChild(lbContact2);
    
    extension::ScrollView* scroll = extension::ScrollView::create(Size(_spriteContainer->getContentSize().width*0.8, _spriteContainer->getContentSize().height*0.63));
    Layer* container = (Layer*)scroll->getContainer();
    scroll->setPosition(_spriteContainer->getContentSize().width*0.1, _spriteContainer->getContentSize().height*0.22);
    //    scrollView->setContentSize(Size(sprBg->getContentSize().width, (((nCount-1)/3)+1)*210));
    
    scroll->setAnchorPoint(Vec2(0.5,0));
    scroll->setDirection(extension::ScrollView::Direction::VERTICAL);
    scroll->setClippingToBounds(true);
    scroll->setBounceable(false);
    _spriteContainer->addChild(scroll);
    
    
    
    std::string str;
    std::vector<std::string> vec;
    std::string community = GAME_TEXT("t_ui_supporters_community").c_str();
    std::string translation = GAME_TEXT("t_ui_supporters_translation").c_str();
    
    str = community + "\n\n";
    std::string text = GAME_TEXT("t_ui_supporters_community_text").c_str();
    vec = MafUtils::split(text,',');
    
    for(int i=0;i<vec.size();i++)
    {
        std::string commu = "   - " + vec.at(i)+"\n";
        //std:std::strtok(idx, ",")
        str += commu;
    }
    
    str += "\n\n";
    
    str += translation;
    str += "\n\n";
    
    text = GAME_TEXT("t_ui_supporters_translation_text").c_str();
    vec = MafUtils::split(text,',');
    
    for(int i=0;i<vec.size();i++)
    {
    
        std::string commu = "   - " + vec.at(i)+"\n";
        str += commu;
    }
    
    
    
    
    
    auto lbText = Label::createWithTTF(str, GAME_FONT, 30);
    lbText->setAnchorPoint(Vec2(0,0));
    lbText->setPosition(0,0);
    container->addChild(lbText);
    container->setAnchorPoint(Vec2(0,0));
    scroll->setContentOffset(Vec2(0, -(lbText->getContentSize().height/2)-70));
    scroll->setContentSize(Size(_spriteContainer->getContentSize().width*0.8,lbText->getContentSize().height));

}

void PopupSupporters::onClickClose(Ref *sender)
{
    hide();
}
