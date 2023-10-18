//
//  PopupCalArtifact.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#include "PopupLanguage.h"

#include "Common/ConfigGameNormal.h"

#include "ManagerGame/SaveManager.h"

USING_NS_CC;

PopupLanguage* PopupLanguage::create()
{
    PopupLanguage* pRet = new(std::nothrow) PopupLanguage();
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

PopupLanguage::PopupLanguage(void) :
_strLangChange(""),

_layerContainerContents(nullptr),
_layerContainerButton(nullptr),
_table(nullptr),
_labelProgress(nullptr)
{
    
}

PopupLanguage::~PopupLanguage(void)
{
    
}

bool PopupLanguage::init(void)
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    // init
    initVal();
    initUi();
    
    return true;
}

#pragma mark - table
void PopupLanguage::tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)
{
    
}

Size PopupLanguage::tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)
{
    return Size(table->getContentSize().width, 75);
}

extension::TableViewCell* PopupLanguage::tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)
{
    auto cell = table->dequeueCell();
    if ( cell == nullptr )
    {
        cell = new extension::TableViewCell();
        cell->autorelease();
    }
    cell->removeAllChildren();
    
    // size
    Size size = tableCellSizeForIndex(table, idx);
    
    // obj
    auto obj = _list.at(idx);
    
    //
    auto menu = MafNode::MafMenu::create();
    menu->setPosition(Vec2::ZERO);
    cell->addChild(menu);
    
    std::string strBGPath = "Assets/ui/language/btn_language_off.png";
    if ( _strLangChange.compare(obj.strCode) == 0 )
    {
        strBGPath = "Assets/ui/language/btn_language_on.png";
    }
    auto itemSelect = MafNode::MafMenuItemSprite::create(Sprite::create(strBGPath), nullptr, CC_CALLBACK_1(PopupLanguage::onClickSelect, this));
    itemSelect->setPosition(size.width / 2, size.height / 2);
    itemSelect->setTag((int)idx);
    menu->addChild(itemSelect);
    
    std::string strLanguage = GAME_TEXT(MafUtils::format("t_ui_language_%s", obj.strCode.c_str()));
    auto labelLanguage = Label::createWithTTF(strLanguage, GAME_FONT, 28);
    labelLanguage->setPosition(itemSelect->getContentSize().width / 2, itemSelect->getContentSize().height / 2);
    labelLanguage->setColor(_strLangChange == obj.strCode ? Color3B(246,155,5) : Color3B(128,128,128));
    itemSelect->addChild(labelLanguage);
    
    auto spriteBeta = Sprite::create("Assets/ui/language/icon_beta.png");
    spriteBeta->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    spriteBeta->setPosition(itemSelect->getContentSize().width-30, itemSelect->getContentSize().height);
    spriteBeta->setVisible(obj.bBeta);
    itemSelect->addChild(spriteBeta);
    
    return cell;
}

ssize_t PopupLanguage::numberOfCellsInTableView(cocos2d::extension::TableView *table)
{
    return _list.size();
}

#pragma mark - init
void PopupLanguage::initVal()
{
    //
    _strLangChange = TextManager::getInstance()->getLang();
    
    //
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::LANGUAGE_LIST);

    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)strText.c_str());
    if( jsonParser.HasParseError() == false )
    {
        
        _list.clear();
        for ( int i = 0; i < jsonParser.Size(); i++ )
        {
            const rapidjson::Value& jsonValue = jsonParser[i];
            
            if ( jsonValue.HasMember("lang") == false || jsonValue.HasMember("beta") == false )
            {
                break;
            }
            
            element obj;
            obj.strCode = jsonValue["lang"].GetString();;
            obj.bBeta = jsonValue["beta"].GetInt();;
            
            if ( obj.strCode.compare(_strLangChange) == 0 )
            {
                _list.insert(_list.begin(), obj);
            }
            else
            {
                _list.push_back(obj);
            }
        }
    }
}

void PopupLanguage::initUi()
{
    Size size = Size(596, 850);
    
    //
    _spriteContainer->setContentSize( size );
 
    //
    _layerContainerButton = Layer::create();
    _layerContainerButton->setContentSize( Size(size.width - 50, 139) );
    _layerContainerButton->setIgnoreAnchorPointForPosition(false);
    _layerContainerButton->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _layerContainerButton->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, 25) );
    _spriteContainer->addChild(_layerContainerButton);
    
    _layerContainerContents = Layer::create();
    _layerContainerContents->setContentSize( Size(size.width - 50, size.height - 75 - 25 - 25 - _layerContainerButton->getContentSize().height - 25) );
    _layerContainerContents->setIgnoreAnchorPointForPosition(false);
    _layerContainerContents->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerContents->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75 - 25) );
    _spriteContainer->addChild(_layerContainerContents);
    
    // title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_menu_language"), GAME_FONT, 28);
    labelTitle->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5);
    labelTitle->setColor(COLOR_TITLE);
    _spriteContainer->addChild(labelTitle);
    
    
    //
    uiContents();
    uiButton();
}

#pragma mark - ui
void PopupLanguage::uiContents()
{
    //
    _layerContainerContents->removeAllChildren();
    
    _table = extension::TableView::create(this, Size(_layerContainerContents->getContentSize().width, _layerContainerContents->getContentSize().height) );
    _table->setDirection(extension::ScrollView::Direction::VERTICAL);
    _table->setVerticalFillOrder(extension::TableView::VerticalFillOrder::TOP_DOWN);
    _table->setIgnoreAnchorPointForPosition(false);
    _table->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _table->setPosition(_layerContainerContents->getContentSize().width / 2, 0);
    _table->setDelegate(this);
    _table->setBounceable(false);
    _layerContainerContents->addChild(_table);
    _table->reloadData();
}

void PopupLanguage::uiButton()
{
    //
    _layerContainerButton->removeAllChildren();
    
    _labelProgress = Label::createWithTTF("", GAME_FONT, 20);
    _labelProgress->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _labelProgress->setPosition(_layerContainerButton->getContentSize().width / 2, _layerContainerButton->getContentSize().height);
    _layerContainerButton->addChild(_labelProgress);
    
    //
    auto menu = Menu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(_layerContainerButton->getContentSize().width / 2, _layerContainerButton->getContentSize().height / 2 - 25);
    _layerContainerButton->addChild(menu);
    
    auto itemCancel = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_empty.png"), nullptr, CC_CALLBACK_1(PopupLanguage::onClickCancel, this));
    menu->addChild(itemCancel);
    {
        auto label = Label::createWithTTF(GAME_TEXT("t_ui_cancel"), GAME_FONT, 34);
        label->setPosition( itemCancel->getContentSize().width / 2, itemCancel->getContentSize().height / 2);
        itemCancel->addChild(label);
    }
    
    auto itemApply = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_empty.png"), nullptr, CC_CALLBACK_1(PopupLanguage::onClickApply, this));
    menu->addChild(itemApply);
    {
        auto label = Label::createWithTTF(GAME_TEXT("t_ui_apply"), GAME_FONT, 34);
        label->setPosition( itemApply->getContentSize().width / 2, itemApply->getContentSize().height / 2);
        itemApply->addChild(label);
    }
    
    //
    menu->alignItemsHorizontallyWithPadding(20);
}

void PopupLanguage::drawContents()
{
    Vec2 pos = _table->getContentOffset();
    _table->reloadData();
    _table->setContentOffset(pos);
}

#pragma mark - set, get

#pragma mark - click
void PopupLanguage::onClickSelect(cocos2d::Ref* sender)
{
    auto item = (MenuItem*)sender;
    int nIdx = (int)item->getTag();
    
    auto obj = _list.at(nIdx);
    _strLangChange = obj.strCode;
    
    // ui
    drawContents();
}

void PopupLanguage::onClickCancel(Ref* sender)
{
    hide();
}

void PopupLanguage::onClickApply(Ref* sender)
{
    PopupLoadingLock::show();
    TableManager::getInstance()->requestLanguage(_strLangChange, CC_CALLBACK_1(PopupLanguage::callbackTable, this), CC_CALLBACK_3(PopupLanguage::callbackTableProgress, this));
}

#pragma mark - callback
void PopupLanguage::callbackTable(bool bResult)
{
    PopupLoadingLock::hide();
    
    if ( bResult == true )
    {
        TextManager::getInstance()->setLang(_strLangChange);
        SaveManager::saveAllData();
        
        //
        MafHttpNew::_vecRequest.clear();
        
        //
        auto scheduler = cocos2d::Director::getInstance()->getScheduler();
        scheduler->unscheduleAllWithMinPriority(Scheduler::PRIORITY_NON_SYSTEM_MIN);
        
        MafGoogleRewardAds::HideGoogleBanner();
        Director::getInstance()->pause();
        UtilsScene::getInstance()->scenePopRoot();
        Director::getInstance()->resume();
    }
    else
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_2"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
    }
}

void PopupLanguage::callbackTableProgress(int nTotal, int nSuccess, int nProgress)
{
    std::string strProgress = MafUtils::format("Download - %d%%", nProgress);
    _labelProgress->setString(strProgress);
}
