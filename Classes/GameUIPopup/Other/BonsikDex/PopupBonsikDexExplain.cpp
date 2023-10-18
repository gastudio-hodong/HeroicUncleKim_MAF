//
//  PopupBonsikDexExplain.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 26/06/2019.
//

#include "PopupBonsikDexExplain.h"

USING_NS_CC;

PopupBonsikDexExplain* PopupBonsikDexExplain::create()
{
    PopupBonsikDexExplain* pRet = new(std::nothrow) PopupBonsikDexExplain();
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
PopupBonsikDexExplain::PopupBonsikDexExplain()
{
    
}
PopupBonsikDexExplain::~PopupBonsikDexExplain()
{
    
}
bool PopupBonsikDexExplain::init(void)
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    initUI();
    
    return true;
}
#pragma mark - init

void PopupBonsikDexExplain::initUI()
{
    _spriteContainer->setContentSize(Size(getContentSize().width, 1227));
    
    MafNode::MafMenu *menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"),
                                               Sprite::create("Assets/ui_common/btn_x.png"),
                                               CC_CALLBACK_1(PopupBonsikDexExplain::onClickClose, this));
    itemClose->setPosition(Vec2(_spriteContainer->getContentSize().width-itemClose->getContentSize().width,
                                _spriteContainer->getContentSize().height-itemClose->getContentSize().height*0.8));
    itemClose->setAnchorPoint(Vec2(0.5,0.5));
    itemClose->setTouchRect(itemClose->getContentSize()*2);
    menu->addChild(itemClose);
    
    auto lbTitle = Label::createWithTTF(GAME_TEXT("t_ui_bonsikdex_explain_title"), GAME_FONT, 36);
    lbTitle->setAnchorPoint(Vec2(0.5,0.5));
    lbTitle->setColor(COLOR_TITLE);
    lbTitle->setPosition( Vec2(_spriteContainer->getContentSize().width/2,_spriteContainer->getContentSize().height - 37.5) );
    _spriteContainer->addChild(lbTitle);
    
    
    
    extension::ScrollView* scroll = extension::ScrollView::create(Size(_spriteContainer->getContentSize().width*0.9, _spriteContainer->getContentSize().height*0.86));
    Layer* container = (Layer*)scroll->getContainer();
    container->setContentSize(Size(getContentSize().width,1500));
    scroll->setPosition(40, _spriteContainer->getContentSize().height*0.05);
    
    
    scroll->setAnchorPoint(Vec2(0.5,0));
    scroll->setDirection(extension::ScrollView::Direction::VERTICAL);
    scroll->setClippingToBounds(true);
    scroll->setBounceable(false);
    scroll->setIgnoreAnchorPointForPosition(false);
    scroll->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    scroll->setPositionX(getContentSize().width/2);
    _spriteContainer->addChild(scroll);
    
    
    struct element
    {
        int category;
        std::string path;
    };
    
    std::vector<element> listElement;
    
    listElement.push_back( {TEXT,  "t_ui_bonsikdex_explain_1"            } );
    listElement.push_back( {IMAGE, "Assets/ui/bonsikdex/explain_1_%s.png"} );
    listElement.push_back( {TEXT,  "t_ui_bonsikdex_explain_2"            } );
    listElement.push_back( {IMAGE, "Assets/ui/bonsikdex/explain_2_%s.png"} );
    listElement.push_back( {TEXT,  "t_ui_bonsikdex_explain_3"            } );
    listElement.push_back( {TEXT,  "t_ui_bonsikdex_explain_4"            } );
    listElement.push_back( {IMAGE, "Assets/ui/bonsikdex/explain_3_%s.png"} );
    listElement.push_back( {TEXT,  "t_ui_bonsikdex_explain_5"            } );
    
    int size = (int)listElement.size();
    int posY = container->getContentSize().height;
    
    scroll->setContentOffset(Vec2(0, -(posY-100)));
    
    for(int i = 0; i < size; i++)
    {
        
        element ele = listElement.at(i);
        
        std::string path = ele.path;
        std::string text;
        int height = 0;
        
        if(ele.category == TEXT)
        {
            text = GAME_TEXT(path).c_str();
            auto list = Label::createWithTTF(text, GAME_FONT, 28);
            list->setAnchorPoint(Vec2(0,1));
            list->setPosition(0,posY);
            container->addChild(list);
            height = list->getStringNumLines();
            list->setDimensions(scroll->getContentSize().width-100, height * 50);
            list->setOverflow(Label::Overflow::RESIZE_HEIGHT);
            height = list->getStringNumLines();
            posY -= height*50;
        }
        else if(ele.category == IMAGE)
        {
            path = TextManager::getInstance()->getLangFileName(path);
            auto list = Sprite::create(path);
            list->setAnchorPoint(Vec2(0.5,1));
            list->setScale(1.2);
            list->setPosition(scroll->getContentSize().width/2-40,posY);
            container->addChild(list);
            posY -= list->getBoundingBox().size.height;
        }
        
    }
    
    container->setAnchorPoint(Vec2(0,0));
    
    
    
}
void PopupBonsikDexExplain::onClickClose(Ref* sender)
{
    hide();
}
