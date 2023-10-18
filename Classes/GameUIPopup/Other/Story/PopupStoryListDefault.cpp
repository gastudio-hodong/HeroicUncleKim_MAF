//
//  PopupStoryList.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#include "PopupStoryListDefault.h"

#include "GameUIPopup/Other/Story/PopupStory.h"
#include "GameUIPopup/Other/Story/PopupStoryDownloadList.h"
#include "GameUIPopup/Other/Story/Layer/LayerStoryList.h"
#include "GameUIPopup/Other/Story/Layer/LayerDialogList.h"

USING_NS_CC;

PopupStoryListDefault* PopupStoryListDefault::create()
{
    PopupStoryListDefault* pRet = new(std::nothrow) PopupStoryListDefault();
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

PopupStoryListDefault::PopupStoryListDefault(void) :
_eType(PopupStoryListDefault::E_TYPE::STORY),
_lyTop(nullptr),
_lyContentArea(nullptr),
_lyContentStoryList(nullptr),
_lyContentDialogList(nullptr),
_lyContainerNow(nullptr)
{
    
}

PopupStoryListDefault::~PopupStoryListDefault(void)
{
    
}

bool PopupStoryListDefault::init()
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

#pragma mark - init
void PopupStoryListDefault::initVar()
{
    
}

void PopupStoryListDefault::initUi()
{
    Size size = Size(729,1250);

    //
    _spriteContainer->setContentSize( size );
    _spriteContainer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _spriteContainer->setPosition(getContentSize().width / 2, 10);
    
    _lyTop = LayerColor::create();
    _lyTop->setContentSize(Size(_spriteContainer->getContentSize().width - 10, 115));
    _lyTop->setIgnoreAnchorPointForPosition(false);
    _lyTop->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _lyTop->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75);
    _spriteContainer->addChild(_lyTop);
    
    _lyContentArea = LayerColor::create();
    _lyContentArea->setContentSize(_spriteContainer->getContentSize() - Size(10, 200));
    _lyContentArea->setIgnoreAnchorPointForPosition(false);
    _lyContentArea->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _lyContentArea->setPosition(getContentSize().width / 2, 10);
    _spriteContainer->addChild(_lyContentArea);
    
    // title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_menu_store"), GAME_FONT, 28);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5) );
    _spriteContainer->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupStoryListDefault::onClickClose, this));
    itemClose->setPosition( Vec2(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5) );
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
    
    //
    uiTop();
    uiLoadLayer();
}


#pragma mark - ui
void PopupStoryListDefault::uiTop()
{
    _lyTop->removeAllChildren();
    
    std::vector<PopupStoryListDefault::E_TYPE> listTab = {
        PopupStoryListDefault::E_TYPE::STORY,
        PopupStoryListDefault::E_TYPE::DIALOG
    };
    
    int size = (int)listTab.size();
    
    float width = _lyTop->getContentSize().width / size - 10;
    float posX = _lyTop->getContentSize().width/2 - width * (size/2) - 10 * (size/2) + 5;
    
    for(int i = 0; i < listTab.size(); ++i)
    {
        auto obj = listTab[i];
        
        std::string strPath = "Assets/ui_common/btn_tap_2_off.png";
        if(obj == _eType)
        {
            strPath = "Assets/ui_common/btn_tap_2_on.png";
        }
        std::string strText = "";
        switch (obj)
        {
            case PopupStoryListDefault::E_TYPE::STORY: strText = GAME_TEXT("t_ui_story_type_1"); break;
            case PopupStoryListDefault::E_TYPE::DIALOG: strText = GAME_TEXT("t_ui_story_type_2"); break;
            default:  break;
        }
        
        auto btnTab = ui::Button::create(strPath);
        btnTab->setScale9Enabled(true);
        btnTab->setContentSize(Size(width, 90));
        btnTab->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        btnTab->setPosition(Vec2(posX, 0));
        btnTab->addClickEventListener(CC_CALLBACK_1(PopupStoryListDefault::onClickTab, this));
        btnTab->setZoomScale(0.05f);
        btnTab->setTag((int)obj);
        _lyTop->addChild(btnTab);
        {
            auto lbText = Label::createWithTTF(strText, GAME_FONT, 25);
            lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            lbText->setPosition(btnTab->getContentSize()/2);
            lbText->setDimensions(btnTab->getContentSize().width - 10, lbText->getContentSize().height);
            lbText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
            lbText->setOverflow(Label::Overflow::SHRINK);
            btnTab->addChild(lbText);
            
            if(obj != _eType)
            {
                lbText->setColor(Color3B(90,90,90));
            }
        }
        
        if(size % 2 == 0)
            btnTab->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        
        posX += width + 10;
    }
}
void PopupStoryListDefault::uiLoadLayer()
{
    if(_lyContainerNow != nullptr)
        _lyContainerNow->setVisible(false);
    
    switch(_eType)
    {
        case PopupStoryListDefault::E_TYPE::STORY:
        {
            if(_lyContentStoryList == nullptr)
            {
                auto layer = LayerStoryList::create(_lyContentArea->getContentSize());
                layer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                layer->setPosition(_lyContentArea->getContentSize()/2);
                _lyContentArea->addChild(layer);
                _lyContentStoryList = layer;
            }
            _lyContainerNow = _lyContentStoryList;

        }break;
        case PopupStoryListDefault::E_TYPE::DIALOG:
        {
            if(_lyContentDialogList == nullptr)
            {
                auto layer = LayerDialogList::create(_lyContentArea->getContentSize());
                layer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                layer->setPosition(_lyContentArea->getContentSize()/2);
                _lyContentArea->addChild(layer);
                _lyContentDialogList = layer;
            }
            _lyContainerNow = _lyContentDialogList;
                
        }break;
    }

    _lyContainerNow->setVisible(true);
}

void PopupStoryListDefault::uiReload()
{
    uiTop();
    uiLoadLayer();
}

#pragma mark - click
void PopupStoryListDefault::onClickClose(cocos2d::Ref* sender)
{
    hide();
}
void PopupStoryListDefault::onClickTab(cocos2d::Ref* sender)
{
    auto node = (Node*)sender;
    _eType = (PopupStoryListDefault::E_TYPE)node->getTag();
    
    uiReload();
}
