//
//  PopupOtherUserManaAMPList.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/09/13.
//

#include "PopupOtherUserManaAMPList.h"

#include "GameObject/Defense/InfoDefenseManaAMP.h"
#include "GameObject/Defense/InfoDefenseManaAmpStone.h"
#include "GameObject/InfoSetting.h"

#include "GameUI/UI/UIReddot.h"
#include "GameUIPopup/Common/PopupToast.h"
#include "GameUIPopup/Other/Defense/ManaAMP/PopupManaAMPDefault.h"

#include "ManagerGame/DefenseManaAMPManager.h"
#include "ManagerGame/DefenseManager.h"


USING_NS_CC;


PopupOtherUserManaAMPList* PopupOtherUserManaAMPList::create(Vector<InfoDefenseManaAMP*> list)
{
    PopupOtherUserManaAMPList* pRet = new(std::nothrow) PopupOtherUserManaAMPList();
    if ( pRet && pRet->init(list) )
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

PopupOtherUserManaAMPList::PopupOtherUserManaAMPList(void):
_lyTop(nullptr),
_lyTopInfo(nullptr),
_lyMiddle(nullptr),
_lyBottom(nullptr),
_table(nullptr)
{
    _listAMP.clear();
}

PopupOtherUserManaAMPList::~PopupOtherUserManaAMPList(void)
{
    _listAMP.clear();
}

#pragma mark - table
void PopupOtherUserManaAMPList::tableCellTouched(extension::TableView* table, extension::TableViewCell* cell)
{
    
}

Size PopupOtherUserManaAMPList::tableCellSizeForIndex(extension::TableView *table, ssize_t idx)
{
    return Size(table->getContentSize().width, 150);
}

extension::TableViewCell* PopupOtherUserManaAMPList::tableCellAtIndex(extension::TableView *table, ssize_t idx)
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
    cell->setContentSize(size);
    
    if(idx >= _listAMP.size())
        return cell;
    
    auto currentAmpData = _listAMP.at(idx);
    Color3B color;
    std::string textPower = "";
    
    auto sprBG = Sprite::create("Assets/ui/defense/Horse_power_amplifier_listbox1.png");
    sprBG->setPosition(cell->getContentSize()/2);
    cell->addChild(sprBG);
    
    //130, 27
    auto sprGear = Sprite::create("Assets/ui/defense/Horse_power_amplifier_listbox_gear.png");
    sprGear->setPosition(Vec2(180, 32));
    cell->addChild(sprGear);
    
    auto sprLine = Sprite::create("Assets/ui/defense/Horse_power_amplifier_listbox2.png");
    sprLine->setPosition(Vec2(cell->getContentSize()/2) + Vec2::RIGHT * 12);
    cell->addChild(sprLine);
    
    auto sprGraph = Sprite::create("Assets/ui/defense/Horse_power_amplifier_listbox_graph00.png");
    sprGraph->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    sprGraph->setPosition(22, 8);
    sprLine->addChild(sprGraph);
    
    //32, 71
    auto sprNumber = Sprite::create(MafUtils::format("Assets/ui/defense/Horse_power_amplifier_num%d.png", idx + 1));
    sprNumber->setPosition(Vec2(32, 71));
    sprBG->addChild(sprNumber);
    
    //-35, 14
    auto sprPowerBox = Sprite::create("Assets/ui/defense/Horse_power_amplifier_listbox3.png");
    sprPowerBox->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    sprPowerBox->setPosition(Vec2(size.width - 32, 14));
    cell->addChild(sprPowerBox);
    
    Vec2 iconLocation = Vec2(90, cell->getContentSize().height - 20);

    auto layerBG = Layer::create();
    layerBG->setContentSize(size - Size(15, 15));
    layerBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    layerBG->setPosition(size/2);
    cell->addChild(layerBG);
    
    if(currentAmpData->isComplete())
    {
        if(currentAmpData->getRedStoneCount() == 10)
        {
            color = Color3B(235,200,0);
        }
        else
        {
            color = Color3B(0,225,245);
        }
        
        sprLine->setColor(color);
        sprGraph->setColor(color);
        sprNumber->setColor(color);
        sprGear->setColor(color);
        
        auto lbPowerText = Label::createWithTTF(GAME_TEXT("t_ui_town_msg_2") , GAME_FONT, 22);
        lbPowerText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        lbPowerText->setPosition(Vec2(42, sprPowerBox->getContentSize().height/2));
        lbPowerText->setDimensions(sprPowerBox->getContentSize().width/2, lbPowerText->getContentSize().height);
        lbPowerText->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
        lbPowerText->setOverflow(Label::Overflow::SHRINK);
        sprPowerBox->addChild(lbPowerText);
        
        std::string strEffect = "%s%%";
        
        std::string strEffectTmp = MafUtils::doubleToString(currentAmpData->getManaAMPPower() * 100);
        //strEffectTmp = MafUtils::addCommaToString(strEffectTmp, 3, ',');
        
        MafUtils::convertNumberToShort(strEffectTmp);
        strEffect = MafUtils::format(strEffect.c_str(), strEffectTmp.c_str());
        
        auto lbPower = Label::createWithTTF(strEffect, GAME_FONT, 22);
        lbPower->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        lbPower->setPosition(Vec2(sprPowerBox->getContentSize().width - 42, sprPowerBox->getContentSize().height/2));
        lbPower->setDimensions(sprPowerBox->getContentSize().width/2, lbPowerText->getContentSize().height);
        lbPower->setAlignment(TextHAlignment::RIGHT, TextVAlignment::CENTER);
        lbPower->setOverflow(Label::Overflow::SHRINK);
        sprPowerBox->addChild(lbPower);
        
        sprGear->runAction(RepeatForever::create(RotateBy::create(10, 360)));
        
        auto anim = Animation::create();
        for(int i = 0; i <= 5; ++i)
        {
            std::string strPath = MafUtils::format("Assets/ui/defense/Horse_power_amplifier_listbox_graph%02d.png", i);
            auto sprite = Sprite::create(strPath);
            sprite->getTexture()->setTexParameters(texParams);
            anim->addSpriteFrame(sprite->getSpriteFrame());
        }
        anim->setDelayPerUnit(0.1f);
        auto animate = Animate::create(anim);
        sprGraph->runAction(RepeatForever::create(animate));
    }
    else
    {
        color = Color3B(110,110,110);
        sprLine->setColor(color);
        sprGraph->setColor(color);
        sprNumber->setColor(color);
        sprGear->setColor(color);
        
        auto lbIncomplete = Label::createWithTTF(GAME_TEXT("t_ui_defense_manaamp_incomlete_1") , GAME_FONT, 22);
        lbIncomplete->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        lbIncomplete->setPosition(sprPowerBox->getContentSize()/2);
        lbIncomplete->setDimensions(sprPowerBox->getContentSize().width - 10, lbIncomplete->getContentSize().height);
        lbIncomplete->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        lbIncomplete->setOverflow(Label::Overflow::SHRINK);
        sprPowerBox->addChild(lbIncomplete);
    }
    
    
    
    bool isLeftFail = false;
    for(int i = 0; i < 10; ++i)
    {
        std::string stdValue = "";
        std::string iconPath = "";
        bool isSuccess = false;
        bool isCurrentNumber = false;
        E_DF_AMP_STONE_Type stoneType = (E_DF_AMP_STONE_Type)currentAmpData->getStoneIndexByOrder(i+1);
        
        switch(stoneType)
        {
            case E_DF_AMP_STONE_Type::NONE:
            {
                if(i == currentAmpData->getStoneCount())
                    isCurrentNumber = true;
                iconPath = "Assets/ui/defense/Horse_power_stone_mini_off.png";
            }break;
            case E_DF_AMP_STONE_Type::BLACK:
            {
                iconPath = "Assets/ui/defense/Horse_power_stone_mini_gray.png";
                stdValue = "+";
            }break;
            case E_DF_AMP_STONE_Type::RED:
            {
                iconPath = "Assets/ui/defense/Horse_power_stone_mini_red.png";
                isSuccess = true;
                if(auto data = DefenseManaAMPManager::getInstance()->getManaAmpStoneData((int)E_DF_AMP_STONE_Type::RED))
                    stdValue = MafUtils::format("%d", (int)data->getOutputQuantity());
            }break;
            case E_DF_AMP_STONE_Type::YELLO:
            {
                iconPath = "Assets/ui/defense/Horse_power_stone_mini_yellow.png";
                isSuccess = true;
                if(auto data = DefenseManaAMPManager::getInstance()->getManaAmpStoneData((int)E_DF_AMP_STONE_Type::YELLO))
                    stdValue = MafUtils::format("%.1f", data->getOutputQuantity());
            }break;
        }
        
        auto icon = Sprite::create(iconPath);
        icon->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        icon->setPosition(iconLocation);
        cell->addChild(icon);
        
        if(isSuccess)
        {
            auto successEffect = Sprite::create("Assets/ui/defense/Horse_power_stone_mini_blue.png");
            successEffect->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            successEffect->setPosition(icon->getContentSize()/2);
            icon->addChild(successEffect);
            
            if(currentAmpData->getRedStoneCount() == 10)
            {
                successEffect->setColor(Color3B(255,240,0));
            }
            else
            {
                successEffect->setColor(Color3B(0,225,255));
            }
            
            if(i != 0 && !isLeftFail)
            {
                auto successLine = Sprite::create("Assets/ui/defense/Horse_power_stone_line_blue.png");
                successLine->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
                successLine->setPosition(icon->getPosition() + Vec2::DOWN * icon->getContentSize().height/2);
                successLine->getTexture()->setTexParameters(texParams);
                cell->addChild(successLine);
                
                
                if(currentAmpData->getRedStoneCount() == 10)
                {
                    successLine->setColor(Color3B(255,240,0));
                }
                else
                {
                    successLine->setColor(Color3B(0,225,255));
                }
                
                auto lbOperator = MafNode::createBmFont("X", GAME_BM_FONT_AL, 20);
                lbOperator->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
                lbOperator->setAlignment(cocos2d::TextHAlignment::RIGHT, cocos2d::TextVAlignment::TOP);
                lbOperator->setPosition(icon->getPosition() + Vec2::DOWN * icon->getContentSize().height);
                cell->addChild(lbOperator);
                
            }
            if(isLeftFail)
                isLeftFail = false;
        }
        
        auto lbValue = MafNode::createBmFont(stdValue, GAME_BM_FONT_AL, 20);
        lbValue->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        lbValue->setAlignment(cocos2d::TextHAlignment::LEFT, cocos2d::TextVAlignment::TOP);
        lbValue->setPosition(icon->getPosition() + Vec2::DOWN * icon->getContentSize().height + Vec2::RIGHT * icon->getContentSize().width/2);
        cell->addChild(lbValue);
        
        if(stoneType == E_DF_AMP_STONE_Type::BLACK)
            isLeftFail = true;
        
        iconLocation += Vec2::RIGHT * (icon->getContentSize().width + 15);
    }
    
    return cell;
}

ssize_t PopupOtherUserManaAMPList::numberOfCellsInTableView(extension::TableView *table)
{
    return _listAMP.size();
}


#pragma mark -init
bool PopupOtherUserManaAMPList::init(Vector<InfoDefenseManaAMP*> list)
{
    if ( !PopupBase::init())
    {
        return false;
    }
    
    _listAMP = list;
    
    initVar();
    initUI();
    
    
    return true;
}

void PopupOtherUserManaAMPList::onEnter()
{
    PopupBase::onEnter();
    
    DefenseManaAMPManager::getInstance()->requestInfo();
}

void PopupOtherUserManaAMPList::initVar()
{
    _spriteContainer->setVisible(false);
}

void PopupOtherUserManaAMPList::initUI()
{
    // title
    Size size = Size(750, 1170);
    
    auto lyParent = LayerColor::create();
    lyParent->setContentSize(size);
    lyParent->setIgnoreAnchorPointForPosition(false);
    lyParent->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    lyParent->setPosition(getContentSize().width/2, 0);
    addChild(lyParent);
    
    auto sprBG = Sprite::create("Assets/ui/defense/Horse_power_amplifier_popup_box.png");
    sprBG->getTexture()->setTexParameters(texParams);
    sprBG->setScale(3);
    sprBG->setPosition(lyParent->getContentSize()/2);
    lyParent->addChild(sprBG);
    
    
    // UI
    _lyTop = LayerColor::create();
    _lyTop->setContentSize(Size(lyParent->getContentSize().width, 90));
    _lyTop->setIgnoreAnchorPointForPosition(false);
    _lyTop->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _lyTop->setPosition(lyParent->getContentSize().width/2, lyParent->getContentSize().height);
    lyParent->addChild(_lyTop);
    
    _lyTopInfo = LayerColor::create();
    _lyTopInfo->setContentSize(Size(lyParent->getContentSize().width, 67));
    _lyTopInfo->setIgnoreAnchorPointForPosition(false);
    _lyTopInfo->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _lyTopInfo->setPosition(_lyTop->getPosition() + Vec2::DOWN * (_lyTop->getContentSize().height));
    lyParent->addChild(_lyTopInfo);
    
    _lyBottom = LayerColor::create();
    _lyBottom->setContentSize(Size(lyParent->getContentSize().width, 105));
    _lyBottom->setIgnoreAnchorPointForPosition(false);
    _lyBottom->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _lyBottom->setPosition(lyParent->getContentSize().width/2, 0);
    lyParent->addChild(_lyBottom);
    
    _lyMiddle = LayerColor::create();
    _lyMiddle->setContentSize(Size(706, lyParent ->getContentSize().height - _lyTop->getContentSize().height - _lyBottom->getContentSize().height - _lyTopInfo->getContentSize().height));
    _lyMiddle->setIgnoreAnchorPointForPosition(false);
    _lyMiddle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _lyMiddle->setPosition(_lyTopInfo->getPosition() + Vec2::DOWN * (_lyTopInfo->getContentSize().height));
    lyParent->addChild(_lyMiddle);
    
    uiTop();
    uiTopInfo();
    uiTable();
    uiBottom();
}

#pragma mark -ui
void PopupOtherUserManaAMPList::uiTop()
{
    _lyTop->removeAllChildren();
    
    auto lbTitle = Label::createWithTTF(GAME_TEXT("t_ui_defense_manaamp"), GAME_FONT, 28);
    lbTitle->setColor(COLOR_TITLE);
    lbTitle->setPosition( Vec2(_lyTop->getContentSize().width / 2, _lyTop->getContentSize().height/2) );
    _lyTop->addChild(lbTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _lyTop->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/defense/Btn_defense_close.png"), nullptr, CC_CALLBACK_1(PopupOtherUserManaAMPList::onClickClose, this));
    itemClose->setPosition( Vec2(_lyTop->getContentSize().width - 118, _lyTop->getContentSize().height/2) );
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
}
void PopupOtherUserManaAMPList::uiTopInfo()
{
    _lyTopInfo->removeAllChildren();
    
    auto lbTitle = Label::createWithTTF(GAME_TEXT("t_ui_defense_manaamp_select"), GAME_FONT, 26);
    lbTitle->setPosition( Vec2(_lyTopInfo->getContentSize().width / 2, _lyTopInfo->getContentSize().height/2 + 7) );
    lbTitle->setDimensions(450, lbTitle->getContentSize().height);
    lbTitle->setAlignment(cocos2d::TextHAlignment::CENTER,cocos2d::TextVAlignment::CENTER);
    lbTitle->setOverflow(cocos2d::Label::Overflow::SHRINK);
    _lyTopInfo->addChild(lbTitle);
}
void PopupOtherUserManaAMPList::uiTable()
{
    _lyMiddle->removeAllChildren();
    
    auto lyEffectInfo = LayerColor::create();
    lyEffectInfo->setContentSize( Size(_lyMiddle->getContentSize().width, _lyMiddle->getContentSize().height - 10));
    lyEffectInfo->setIgnoreAnchorPointForPosition(false);
    lyEffectInfo->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    lyEffectInfo->setPosition( Vec2(_lyMiddle->getContentSize().width / 2, 20));
    _lyMiddle->addChild(lyEffectInfo);
    
    _table = extension::TableView::create(this, Size(lyEffectInfo->getContentSize().width, lyEffectInfo->getContentSize().height - 10));
    _table->setIgnoreAnchorPointForPosition(false);
    _table->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _table->setPosition(lyEffectInfo->getContentSize().width / 2, 0);
    _table->setDirection(extension::TableView::Direction::VERTICAL);
    _table->setVerticalFillOrder(extension::TableView::VerticalFillOrder::TOP_DOWN);
    _table->setDelegate(this);
    _table->setBounceable(false);
    lyEffectInfo->addChild(_table);
    _table->reloadData();
}

void PopupOtherUserManaAMPList::uiBottom()
{
    _lyBottom->removeAllChildren();
    
    auto sprBG = Sprite::create("Assets/ui/defense/Horse_power_amplifier_popup_bottom_box.png");
    sprBG->setPosition(_lyBottom->getContentSize()/2);
    _lyBottom->addChild(sprBG);
    
    auto lbPowerText = Label::createWithTTF(GAME_TEXT("t_ui_town_msg_2") , GAME_FONT, 22);
    lbPowerText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    lbPowerText->setPosition(Vec2(40, sprBG->getContentSize().height/2));
    lbPowerText->setDimensions(sprBG->getContentSize().width/2 - 40, lbPowerText->getContentSize().height);
    lbPowerText->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
    lbPowerText->setOverflow(Label::Overflow::SHRINK);
    sprBG->addChild(lbPowerText);
    
    std::string strEffect = "%s%%";
    
    std::string power = "0";
    for(auto info : _listAMP)
    {
        if(!info->isComplete())
            continue;
        
        if(power == "0")
        {
            power = MafUtils::doubleToString(info->getManaAMPPower());
            continue;
        }
        
        auto tt = MafUtils::bigMulNum(power, MafUtils::doubleToString(info->getManaAMPPower()));
        
        if(tt.length() > 0)
            power = MafUtils::bigAddNum(power, tt);
    }
    
    std::string strEffectTmp = MafUtils::bigMulNum(power, "100");
    
    MafUtils::convertNumberToShort(strEffectTmp);
    strEffect = MafUtils::format(strEffect.c_str(), strEffectTmp.c_str());
    
    auto lbPower = Label::createWithTTF(strEffect, GAME_FONT, 22);
    lbPower->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    lbPower->setPosition(Vec2(sprBG->getContentSize().width - 40, sprBG->getContentSize().height/2));
    lbPower->setDimensions(sprBG->getContentSize().width/2, lbPowerText->getContentSize().height);
    lbPower->setAlignment(TextHAlignment::RIGHT, TextVAlignment::CENTER);
    lbPower->setOverflow(Label::Overflow::SHRINK);
    sprBG->addChild(lbPower);
}

void PopupOtherUserManaAMPList::uiRefresh()
{
    uiTop();
    uiTopInfo();
    uiBottom();
    
    auto offset = _table->getContentOffset();
    _table->reloadData();
    _table->setContentOffset(offset);
}

#pragma mark -utils
void PopupOtherUserManaAMPList::onClickClose(Ref* sender)
{
    hide();
}
