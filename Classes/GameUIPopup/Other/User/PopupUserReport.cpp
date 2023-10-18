//
//  PopupUserReport.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/09/17.
//

#include "PopupUserReport.h"

#include "Common/ConfigGameNormal.h"

#include "GameUI/UI/UIComboBox.h"

#include "ManagerGame/AccountManager.h"

USING_NS_CC;

PopupUserReport* PopupUserReport::create(E_REPORT_TYPE eType, int nUseridx, std::string strNickname, std::string strPlatform, std::string strMessage)
{
    PopupUserReport* pRet = new(std::nothrow) PopupUserReport();
    if ( pRet && pRet->init(eType, nUseridx, strNickname, strPlatform, strMessage) )
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

PopupUserReport::PopupUserReport():
_eReportType(E_REPORT_TYPE::NONE),

_nUserIdx(0),
_strUserNickname(""),
_strUserPlatform(""),

_strMessage(""),


_layerContainerContents(nullptr),
_layerContainerButton(nullptr),
_labelReportCount(nullptr),
_editBox(nullptr)
{
    
}

PopupUserReport::~PopupUserReport(void)
{
    
}

bool PopupUserReport::init(E_REPORT_TYPE eType, int nUseridx, std::string strNickname, std::string strPlatform, std::string strMessage)
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    _eReportType = eType;
    
    _nUserIdx = nUseridx;
    _strUserNickname = strNickname;
    _strUserPlatform = strPlatform;
    
    _strMessage = strMessage;
    
    //
    initVar();
    initUI();
    
    return true;
}

#pragma mark - init
void PopupUserReport::initVar()
{
    
    
    
}

void PopupUserReport::initUI()
{
    Size size = Size(650, 756);
    
    //
    _spriteContainer->setContentSize( size );
    
    //
    _layerContainerButton = Layer::create();
    _layerContainerButton->setContentSize( Size(size.width - 50, 116) );
    _layerContainerButton->setIgnoreAnchorPointForPosition(false);
    _layerContainerButton->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _layerContainerButton->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, 25) );
    _spriteContainer->addChild(_layerContainerButton);
    
    _layerContainerContents = Layer::create();
    _layerContainerContents->setContentSize( Size(size.width - 50, size.height - 75 - 25 - _layerContainerButton->getContentSize().height - 25) );
    _layerContainerContents->setIgnoreAnchorPointForPosition(false);
    _layerContainerContents->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerContents->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75 - 25) );
    _spriteContainer->addChild(_layerContainerContents);
    
    // title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_report"), GAME_FONT, 28);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5) );
    _spriteContainer->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupUserReport::onClickClose, this));
    itemClose->setPosition(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5);
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
    
    //
    uiContents();
    uiButton();
}

#pragma mark - ui
void PopupUserReport::uiContents()
{
    _layerContainerContents->removeAllChildren();
 
    // 신고 대상
    {
        auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_report_target"), GAME_FONT, 24);
        labelTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        labelTitle->setPosition(110, _layerContainerContents->getContentSize().height - 45);
        labelTitle->setDimensions(110, labelTitle->getContentSize().height);
        labelTitle->setAlignment(TextHAlignment::RIGHT, TextVAlignment::CENTER);
        labelTitle->setOverflow(Label::Overflow::SHRINK);
        _layerContainerContents->addChild(labelTitle);
        
        auto spriteContentsBG = ui::Scale9Sprite::create("Assets/ui_common/box.png", Rect::ZERO, Rect(10,10,10,10));
        spriteContentsBG->setContentSize(Size(446, 52));
        spriteContentsBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        spriteContentsBG->setPosition(125, _layerContainerContents->getContentSize().height - 45);
        _layerContainerContents->addChild(spriteContentsBG);
        {
            auto labelContents = Label::createWithTTF(_strUserNickname, GAME_FONT, 24);
            labelContents->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            labelContents->setPosition(25, spriteContentsBG->getContentSize().height / 2);
            spriteContentsBG->addChild(labelContents);
        }
    }
    
    // 신고 내용
    {
        auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_report_contents"), GAME_FONT, 24);
        labelTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        labelTitle->setPosition(110, _layerContainerContents->getContentSize().height - 205);
        labelTitle->setDimensions(110, labelTitle->getContentSize().height);
        labelTitle->setAlignment(TextHAlignment::RIGHT, TextVAlignment::CENTER);
        labelTitle->setOverflow(Label::Overflow::SHRINK);
        _layerContainerContents->addChild(labelTitle);
        
        _editBox = ui::EditBox::create(Size(446,208), ui::Scale9Sprite::create("Assets/ui_common/box.png", Rect::ZERO, Rect(10,10,10,10)));
        _editBox->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        _editBox->setPosition(Vec2(125, _layerContainerContents->getContentSize().height - 205 + 26));
        _editBox->setFont(GAME_FONT, 34);
        _editBox->setMaxLength(40);
        _editBox->setInputMode(ui::EditBox::InputMode::ANY);
        _editBox->setInputFlag(ui::EditBox::InputFlag::SENSITIVE);
        _editBox->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
        _editBox->setPlaceHolder(GAME_TEXT("t_ui_report_contents_text").c_str());
        _editBox->setPlaceholderFont(GAME_FONT, 24);
        _editBox->setPlaceholderFontColor(Color3B(128, 128, 128));
        _editBox->setText(_strMessage.c_str());
        _layerContainerContents->addChild(_editBox);
        
        if ( _eReportType == E_REPORT_TYPE::CHAT )
        {
            _editBox->setEnabled(false);
        }
    }
    
    // 신고 사유
    {
        auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_report_type"), GAME_FONT, 24);
        labelTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        labelTitle->setPosition(110, _layerContainerContents->getContentSize().height - 125);
        labelTitle->setDimensions(110, labelTitle->getContentSize().height);
        labelTitle->setAlignment(TextHAlignment::RIGHT, TextVAlignment::CENTER);
        labelTitle->setOverflow(Label::Overflow::SHRINK);
        _layerContainerContents->addChild(labelTitle);
        
        ValueVector listRating;
        listRating.push_back(Value(GAME_TEXT("t_ui_report_type_1")));
        listRating.push_back(Value(GAME_TEXT("t_ui_report_type_2")));
        listRating.push_back(Value(GAME_TEXT("t_ui_report_type_4")));
        listRating.push_back(Value(GAME_TEXT("t_ui_report_type_3")));
        
        auto comboType = UIComboBox::create(listRating, UIComboBox::eType::type_down, "Assets/ui_common/report_box.png", "Assets/ui_common/report_listbox.png");
        comboType->setIconArrow("Assets/ui_common/report_btn_down.png", "Assets/ui_common/report_btn_up.png");
        comboType->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        comboType->setPosition(125, _layerContainerContents->getContentSize().height - 125);
        comboType->setDataSelect(GAME_TEXT("t_ui_report_type_text"));
        comboType->setEnable(true);
        comboType->setTextListFont(GAME_FONT, 24, Color3B(128, 128, 128));
        comboType->setCallbackSelect(CC_CALLBACK_1(PopupUserReport::callbackComboResult, this));
        _layerContainerContents->addChild(comboType);
        
        if ( _eReportType == E_REPORT_TYPE::CHAT )
        {
            comboType->setDataSelect(GAME_TEXT("t_ui_report_type_4"));
            comboType->setEnable(false);
        }
    }
    
    auto labelText = Label::createWithTTF(GAME_TEXT("t_ui_report_text"), GAME_FONT, 18);
    labelText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    labelText->setPosition(_layerContainerContents->getContentSize().width / 2, 0);
    labelText->setDimensions(_layerContainerContents->getContentSize().width, 100);
    labelText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    labelText->setOverflow(Label::Overflow::SHRINK);
    labelText->setColor(Color3B(219, 130, 57));
    _layerContainerContents->addChild(labelText);
}

void PopupUserReport::uiButton()
{
    _layerContainerButton->removeAllChildren();
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(_layerContainerButton->getContentSize().width / 2, _layerContainerButton->getContentSize().height / 2);
    _layerContainerButton->addChild(menu);
    
    // item close
    auto itemReport = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_upgrade_bg.png"), nullptr, CC_CALLBACK_1(PopupUserReport::onClickReport, this));
    menu->addChild(itemReport);
    {
        auto labelText = Label::createWithTTF(GAME_TEXT("t_ui_report"), GAME_FONT, 26);
        labelText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        labelText->setPosition(itemReport->getContentSize().width / 2, itemReport->getContentSize().height / 2);
        labelText->setDimensions(itemReport->getContentSize().width - 10, labelText->getContentSize().height);
        labelText->setAlignment(TextHAlignment::CENTER, TextVAlignment::BOTTOM);
        labelText->setOverflow(Label::Overflow::SHRINK);
        itemReport->addChild(labelText);
        
        _labelReportCount = Label::createWithTTF(GAME_TEXTFORMAT("t_ui_report_count", 0, 5), GAME_FONT, 20);
        _labelReportCount->setPosition(itemReport->getContentSize().width / 2, itemReport->getContentSize().height * 0.25);
        _labelReportCount->setDimensions(itemReport->getContentSize().width - 10, _labelReportCount->getContentSize().height);
        _labelReportCount->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        _labelReportCount->setOverflow(Label::Overflow::SHRINK);
        itemReport->addChild(_labelReportCount);
    }
    
    
    //
    menu->alignItemsHorizontallyWithPadding(10);
    
    //
    drawButton();
}

#pragma mark - draw
void PopupUserReport::drawButton()
{
    _labelReportCount->setString(GAME_TEXTFORMAT("t_ui_report_count", AccountManager::getInstance()->getReportCount(), AccountManager::getInstance()->getReportCountMax()));
}

#pragma mark - set, get

#pragma mark - click
void PopupUserReport::onClickClose(cocos2d::Ref* sender)
{
    hide();
}

void PopupUserReport::onClickReport(cocos2d::Ref* sender)
{
    int nReportCount = AccountManager::getInstance()->getReportCount();
    int nReportCountMax = AccountManager::getInstance()->getReportCountMax();
    
    std::string strMessage = _editBox->getText();
    
    if ( nReportCount > nReportCountMax )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_report_error_1"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        drawButton();
        return;
    }
    
    if ( _eReportType == E_REPORT_TYPE::NONE || strMessage.length() <= 0 )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_report_type_text"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    //
    AccountManager::getInstance()->requestReport(true, _eReportType, strMessage, _nUserIdx, _strUserPlatform, CC_CALLBACK_2(PopupUserReport::callbackReport, this));
}

#pragma mark - callback
void PopupUserReport::callbackReport(bool bResult, int nResult)
{
    if ( bResult == false )
    {
        std::string strMessage = GAME_TEXT("t_ui_error_30");
        if ( nResult == 0 )         strMessage = GAME_TEXT("t_ui_report_error_1");
        else if ( nResult == 3 )    strMessage = GAME_TEXT("t_ui_report_error_2");
        else if ( nResult == 5 )    strMessage = GAME_TEXT("t_ui_report_error_3");
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strMessage);
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    //
    hide();
}

void PopupUserReport::callbackComboResult(std::string strResult)
{
    if ( strResult.compare(GAME_TEXT("t_ui_report_type_1")) == 0 )
    {
        _eReportType = E_REPORT_TYPE::HACK;
    }
    else if ( strResult.compare(GAME_TEXT("t_ui_report_type_2")) == 0 )
    {
        _eReportType = E_REPORT_TYPE::NICK;
    }
    else if ( strResult.compare(GAME_TEXT("t_ui_report_type_3")) == 0 )
    {
        _eReportType = E_REPORT_TYPE::CHAT;
    }
    else
    {
        _eReportType = E_REPORT_TYPE::OTHER;
    }
}
