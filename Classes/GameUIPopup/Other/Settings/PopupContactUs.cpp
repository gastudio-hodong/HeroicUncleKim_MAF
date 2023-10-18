//
//  lkjkl.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 17/02/2020.
//

#include "PopupContactUs.h"

#include "ManagerGame/AccountManager.h"

USING_NS_CC;
 
PopupContactUs* PopupContactUs::create()
{
    PopupContactUs* pRet = new(std::nothrow) PopupContactUs();
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

PopupContactUs::PopupContactUs(void) :


_layerContainerFAQ(nullptr),
_layerContainerContactUs(nullptr)
{
    
}

PopupContactUs::~PopupContactUs(void)
{
    
}

bool PopupContactUs::init()
{
    if( !PopupBase::init() )
    {
        return false;
    }
    
    //
    initVar();
    initUi();
    
    //
    _eventDispatcher->removeEventListenersForTarget(this);
    auto listener = cocos2d::EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [=](cocos2d::Touch *touch, cocos2d::Event *event)->bool {
        cocos2d::Rect rect = cocos2d::Rect(0, 0, this->getContentSize().width, this->getContentSize().height);
        if (rect.containsPoint(this->convertToNodeSpace(touch->getLocation())))
        {
            return true;
        }
        
        return false;
    };
    listener->onTouchEnded = [=](cocos2d::Touch *touch, cocos2d::Event *event)->void {
        cocos2d::Rect rect = _spriteContainer->getBoundingBox();
        if (rect.containsPoint(this->convertToNodeSpace(touch->getLocation())) == false)
        {
            hide();
        }
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}
#pragma mark - init
void PopupContactUs::initVar()
{
    std::string strLang = TextManager::getInstance()->getLang();
    std::string strText = ParameterManager::getInstance()->getParameterString("contents_us");
    
    rapidjson::Document jsonParser;
    jsonParser.Parse<0>((char*)strText.c_str());
    if ( strText.length() != 0 && jsonParser.HasParseError() == false )
    {
        std::string strKey = "other";
        if ( jsonParser.HasMember(strLang.c_str()) == true )
        {
            strKey = strLang;
        }
        
        const rapidjson::Value& jsonArray = jsonParser[strKey.c_str()];
        
        for ( int i = 0; i < jsonArray.Size(); i++ )
        {
            const rapidjson::Value& jsonValue = jsonArray[i];
            
            element obj;
            obj.strTextKey = jsonValue["text"].GetString();;
            obj.strLink = jsonValue["link"].GetString();
            
            _list.push_back(obj);
        }
    }
}

void PopupContactUs::initUi()
{
    Size size = Size(550, 600);
    if ( _list.size() == 1  )
    {
        size.height += 82;
    }
    else if ( _list.size() > 1  )
    {
        size.height += _list.size() * 50 + (_list.size() - 1) * 10;
    }
    
    // size
    _spriteContainer->setContentSize(size);
    
    // label title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_customer_title"), GAME_FONT, 32);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5);
    _spriteContainer->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupContactUs::onClickClose, this));
    itemClose->setPosition(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5);
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
   
    _layerContainerContactUs = LayerColor::create();
    _layerContainerContactUs->setContentSize(Size(_spriteContainer->getContentSize().width - 50, 250));
    _layerContainerContactUs->setIgnoreAnchorPointForPosition(false);
    _layerContainerContactUs->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _layerContainerContactUs->setPosition(_spriteContainer->getContentSize().width / 2, 25);
    _spriteContainer->addChild(_layerContainerContactUs);
    
    _layerContainerFAQ = LayerColor::create();
    _layerContainerFAQ->setContentSize(Size(_spriteContainer->getContentSize().width - 50, _spriteContainer->getContentSize().height - 75 - 25 - 75 - _layerContainerContactUs->getContentSize().height - 25));
    _layerContainerFAQ->setIgnoreAnchorPointForPosition(false);
    _layerContainerFAQ->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _layerContainerFAQ->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75 - 25);
    _spriteContainer->addChild(_layerContainerFAQ);
    
    //
    uiDefaultFAQ();
    uiDefaultContactUs();
}

#pragma mark - ui
void PopupContactUs::uiDefaultFAQ()
{
    _layerContainerFAQ->removeAllChildren();
    auto layerBG = _layerContainerFAQ;
    
    auto nodeText = ui::CText::create(GAME_TEXT("t_ui_customer_text_1"), GAME_FONT, 28);
    nodeText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    nodeText->setPosition(Vec2(layerBG->getContentSize().width / 2, layerBG->getContentSize().height));
    nodeText->setTextHorizontalAlignment(TextHAlignment::CENTER);
    layerBG->addChild(nodeText);
    
    bool bAlignmentCenter = true;
    double nHeightCell = 82;
    if ( _list.size() > 1 )
    {
        nHeightCell = 50;
        bAlignmentCenter = false;
    }
    
    double nPosX = layerBG->getContentSize().width / 2;
    double nPosY = nodeText->getPositionY() - nodeText->getContentSize().height - 25;
    for ( int i = 0; i < _list.size(); i++ )
    {
        auto objElement = _list.at(i);
        
        ui::Button* nodeButton = nullptr;
        if ( bAlignmentCenter == true )
        {
            nodeButton = ui::Button::create("Assets/ui_common/btn_1_1_on.png");
            nodeButton->setContentSize(Size(426, nHeightCell));
            nodeButton->setScale9Enabled(true);
            nodeButton->setCapInsets(Rect(33,33,2,2));
            nodeButton->setPosition(Vec2(nPosX, nPosY - nodeButton->getContentSize().height / 2));
            nodeButton->addClickEventListener(CC_CALLBACK_1(PopupContactUs::onClickFAQ, this));
            nodeButton->setTag(i);
            layerBG->addChild(nodeButton);
            {
                auto nodeContents = UtilsWidget::getInstance()->createIconText("Assets/ui_common/btn_shortcuts_4.png", GAME_TEXT(objElement.strTextKey), nodeButton->getContentSize().width - 40);
                nodeContents->setPosition(Vec2(nodeButton->getContentSize().width / 2, nodeButton->getContentSize().height / 2));
                nodeButton->addChild(nodeContents);
                
                auto nodeText = (ui::CText*)nodeContents->getChildByName("WIDGET_TEXT");
                nodeText->setTextColor(Color4B::YELLOW);
            }
        }
        else
        {
            nodeButton = ui::Button::create("Assets/ui_common/box.png");
            nodeButton->setContentSize(Size(426, nHeightCell));
            nodeButton->setScale9Enabled(true);
            nodeButton->setCapInsets(Rect(10,10,10,10));
            nodeButton->setPosition(Vec2(nPosX, nPosY - nodeButton->getContentSize().height / 2));
            nodeButton->addClickEventListener(CC_CALLBACK_1(PopupContactUs::onClickFAQ, this));
            nodeButton->setTag(i);
            layerBG->addChild(nodeButton);
            {
                auto nodeContents = UtilsWidget::getInstance()->createIconText("Assets/ui_common/btn_shortcuts_4.png", GAME_TEXT(objElement.strTextKey), nodeButton->getContentSize().width - 40);
                nodeContents->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
                nodeContents->setPosition(Vec2(20, nodeButton->getContentSize().height / 2));
                nodeButton->addChild(nodeContents);
                
                auto nodeText = (ui::CText*)nodeContents->getChildByName("WIDGET_TEXT");
                nodeText->setFontSize(24);
            }
        }
        
        
        nPosY -= nodeButton->getContentSize().height;
        nPosY -= 10;
    }
}

void PopupContactUs::uiDefaultContactUs()
{
    _layerContainerContactUs->removeAllChildren();
    auto layerBG = _layerContainerContactUs;
    
    auto nodeText = ui::CText::create(GAME_TEXT("t_ui_customer_text_2"), GAME_FONT, 28);
    nodeText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    nodeText->setPosition(Vec2(layerBG->getContentSize().width / 2, layerBG->getContentSize().height));
    nodeText->setTextHorizontalAlignment(TextHAlignment::CENTER);
    nodeText->setTextOverflow(Label::Overflow::SHRINK);
    nodeText->setTextAreaSize(Size(layerBG->getContentSize().width, nodeText->getContentSize().height));
    layerBG->addChild(nodeText);
    
    auto nodeButton = ui::Button::create("Assets/ui_common/btn_1_2_on.png");
    nodeButton->setContentSize(Size(426, 82));
    nodeButton->setScale9Enabled(true);
    nodeButton->setCapInsets(Rect(33,33,2,2));
    nodeButton->setPosition(Vec2(layerBG->getContentSize().width / 2, nodeText->getPositionY() - nodeText->getContentSize().height - 25 - nodeButton->getContentSize().height / 2));
    nodeButton->addClickEventListener(CC_CALLBACK_1(PopupContactUs::onClickContactUs, this));
    layerBG->addChild(nodeButton);
    {
        auto uiText = ui::CText::create(GAME_TEXT("t_ui_customer_title"), GAME_FONT, 30);
        uiText->setPosition(Vec2(nodeButton->getContentSize().width / 2, nodeButton->getContentSize().height / 2));
        uiText->setTextColor(Color4B::YELLOW);
        nodeButton->addChild(uiText);
    }
}

#pragma mark - click
void PopupContactUs::onClickClose(Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_SELECT);
    auto item = (MafNode::MafMenuItemSprite*)sender;
    
    hide();
}

void PopupContactUs::onClickFAQ(Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_SELECT);
    auto item = (MafNode::MafMenuItemSprite*)sender;
    auto objElement = _list.at(item->getTag());
    
    MafNative::OpenUrl(objElement.strLink.c_str());
}

void PopupContactUs::onClickContactUs(Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_SELECT);
    auto item = (MafNode::MafMenuItemSprite*)sender;
    
    std::string strGameId = "";
    std::string strVersion = "";
    std::string strAuth = "";

    if ( AccountManager::getInstance()->getUserID().length() != 0 )
        strGameId = AccountManager::getInstance()->getUserID();
    else
        strGameId = GAME_TEXT("t_ui_error_1");

    strVersion = MafNative::GetGameVersion();
    strAuth = MafAes256::Encrypt(strGameId);
    
    std::string strSubject = GAME_TEXT("t_ui_menu_msg_13");
    std::string strBody = GAME_TEXTFORMAT("t_ui_menu_msg_14", strGameId.c_str(), strVersion.c_str(), MafNative::getDeviceModel().c_str(), MafNative::getDeviceSystemVersion().c_str(), strAuth.c_str());

    MafNative::SendEmail(EMAIL_HELP, strSubject.c_str(), strBody.c_str());
}

#pragma mark - callback
void PopupContactUs::callbackAlarm()
{
    
}

#pragma mark - game
