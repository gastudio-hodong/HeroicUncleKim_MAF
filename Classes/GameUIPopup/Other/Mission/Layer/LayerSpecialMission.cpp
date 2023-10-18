//
//  LayerSpecialMission.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/08/24.
//

#include "LayerSpecialMission.h"

#include "Common/ConfigGameNormal.h"

#include "GameUI/UI/UIReddot.h"

#include "ManagerGame/TutorialManager.h"
#include "ManagerGame/SpecialMissionManager.h"

USING_NS_CC;

LayerSpecialMission* LayerSpecialMission::create(const Size size)
{
    LayerSpecialMission* pRet = new(std::nothrow) LayerSpecialMission();
    if ( pRet && pRet->init(size) )
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
LayerSpecialMission::LayerSpecialMission():
_lyTable(nullptr)
,_table(nullptr)
,_callbackHide(nullptr)
,_callbackRefresh(nullptr)
{
    _listInfoSpecial.clear();
}
LayerSpecialMission::~LayerSpecialMission()
{
    _listInfoSpecial.clear();
}
bool LayerSpecialMission::init(const Size size)
{
    if ( !LayerColor::initWithColor(Color4B()) )
    {
        return false;
    }

    setIgnoreAnchorPointForPosition(false);
    setContentSize(size);
    setMissionOder();
    initVar();
    initUI();

    return true;
}
#pragma mark - table
void LayerSpecialMission::tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)
{

}

Size LayerSpecialMission::tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)
{
    return Size(_lyTable->getContentSize().width - 10, C_BAR_MISSION_SIZE + 10);
}

extension::TableViewCell* LayerSpecialMission::tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)
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

    auto objMission = _listInfoSpecial.at(idx);
    if(objMission == nullptr)
        return cell;
    auto objItem = objMission->getListMissionReward().at(0);
    
    auto sprBox = Sprite::create("Assets/ui/mission/mission_normal_2.png");
    sprBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprBox->setPosition(cell->getContentSize()/2);
    cell->addChild(sprBox);
    
    auto sprRewardBox = Sprite::create("Assets/ui/mission/task_box_special.png");
    sprRewardBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    sprRewardBox->setPosition(10, sprBox->getContentSize().height/2);
    sprBox->addChild(sprRewardBox);
    if(objItem != nullptr)
    {
        auto spReward = Sprite::create(objItem->getPath());
        spReward->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        spReward->setPosition(sprRewardBox->getContentSize()/2);
        spReward->setScale(87 / spReward->getContentSize().width);
        sprRewardBox->addChild(spReward);
        
        if(objItem->getIdx() == (int)E_ITEMS::PTICKET)
        {
            spReward->setScale(spReward->getScale() / 1.5f);
        }
    }
    
    auto lbTextTop = Label::createWithTTF(GAME_TEXTFORMAT("t_new_mission_title", objMission->getIdx()), GAME_FONT, 20);
    lbTextTop->setAnchorPoint(Vec2(0,0));
    lbTextTop->setPosition(Vec2(sprBox->getContentSize().width*0.2, sprBox->getContentSize().height*0.74));
    lbTextTop->setDimensions(400, lbTextTop->getContentSize().height);
    lbTextTop->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
    lbTextTop->setOverflow(Label::Overflow::SHRINK);
    sprBox->addChild(lbTextTop);
    
    auto lbContent = Label::createWithTTF(objMission->getContent(), GAME_FONT, 28);//28로 맞출지는 보고 확인
    lbContent->setAnchorPoint(Vec2(0,0));
    lbContent->setPosition(Vec2(sprBox->getContentSize().width*0.2, sprBox->getContentSize().height*0.47));
    lbContent->setDimensions(370, lbContent->getContentSize().height);
    lbContent->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
    lbContent->setOverflow(Label::Overflow::SHRINK);
    lbContent->setColor(Color3B(92,203,255));
    sprBox->addChild(lbContent);
    
    std::string strTextBottom = "";
    if(objItem != nullptr)
    {
        strTextBottom = MafUtils::format("%s +%s", objItem->getText().c_str(), objItem->getCount().c_str());
    }
    auto lbTextBottom = Label::createWithTTF(strTextBottom, GAME_FONT, 19); //타이틀 내용은 비어있어야 한다.
    lbTextBottom->setColor(Color3B(167, 188, 208));
    lbTextBottom->setAnchorPoint(Vec2(0,0));
    lbTextBottom->setPosition(Vec2(sprBox->getContentSize().width*0.2, sprBox->getContentSize().height*0.1));
    lbTextBottom->setDimensions(400, lbTextBottom->getContentSize().height);
    lbTextBottom->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
    lbTextBottom->setOverflow(Label::Overflow::SHRINK);
    sprBox->addChild(lbTextBottom);
    
    if ( objMission->getIdx() <= SpecialMissionManager::getInstance()->getSpecialMissionClear() )
    {
        lbContent->enableStrikethrough();
        lbTextBottom->setVisible(false);
    }
    else
    {
        auto spriteBlack = ui::Scale9Sprite::create(PIXEL,Rect(0,0,sprBox->getContentSize().width, sprBox->getContentSize().height));
        spriteBlack->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        spriteBlack->setPosition(0, sprBox->getContentSize().height);
        spriteBlack->setColor(Color3B::BLACK);
        spriteBlack->setOpacity(150);
        sprBox->addChild(spriteBlack);
        
        auto sprLock = Sprite::create("Assets/ui_common/tap_lock_2.png");
        sprLock->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        sprLock->setPosition(sprRewardBox->getPositionX() - sprRewardBox->getContentSize().width/2 + sprLock->getContentSize().width * 1 / 4, sprRewardBox->getPositionY() + sprRewardBox->getContentSize().height/2 - sprLock->getContentSize().height * 1 / 3   );
        sprBox->addChild(sprLock);
        
        std::string strNotice = GAME_TEXT("t_ui_mission_msg_8");
        
        auto lbNotice = Label::createWithTTF(strNotice, GAME_FONT, 19);
        lbNotice->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        lbNotice->setDimensions(sprBox->getContentSize().width - 20, sprBox->getContentSize().height);
        lbNotice->setOverflow(Label::Overflow::SHRINK);
        lbNotice->setPosition(sprBox->getContentSize().width, sprBox->getContentSize().height/2);
        lbNotice->setAlignment(TextHAlignment::RIGHT, TextVAlignment::CENTER);
        sprBox->addChild(lbNotice);
    }
    return cell;
}

ssize_t LayerSpecialMission::numberOfCellsInTableView(cocos2d::extension::TableView *table)
{
    return _listInfoSpecial.size();
}

#pragma mark -init
void LayerSpecialMission::initVar()
{
}

void LayerSpecialMission::initUI()
{
    _lyTop = LayerColor::create();
    _lyTop->setContentSize(Size(getContentSize().width, 170));
    _lyTop->setIgnoreAnchorPointForPosition(false);
    _lyTop->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lyTop->setPosition(getContentSize().width/2, getContentSize().height);
    addChild(_lyTop);
    
    _lyTable = LayerColor::create();
    _lyTable->setContentSize( Size(getContentSize().width, getContentSize().height - _lyTop->getContentSize().height - 15));
    _lyTable->setIgnoreAnchorPointForPosition(false);
    _lyTable->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lyTable->setPosition(_lyTop->getPosition() + Vec2::DOWN * _lyTop->getContentSize().height + Vec2::RIGHT * 4);
    addChild(_lyTable);
    
    uiTop();
    uiTable();
}

#pragma mark -ui
void LayerSpecialMission::uiReload()
{
    uiTop();
    setMissionOder();
    _table->reloadData();
}
void LayerSpecialMission::uiTop()
{
    _lyTop->removeAllChildren();
    
    int mission = SpecialMissionManager::getInstance()->getSpecialMissionClear()+1;
    auto objMission = SpecialMissionManager::getInstance()->getInfoSpecialMission(mission);
    
    auto sprInfo = Sprite::create("Assets/ui/mission/mission_list.png");
    sprInfo->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    sprInfo->setPosition(_lyTop->getContentSize().width/2, _lyTop->getContentSize().height);
    _lyTop->addChild(sprInfo);
    {
        auto labelTitle1 = Label::createWithTTF(GAME_TEXT("t_ui_mission_title"), GAME_FONT, 24);
        labelTitle1->setPosition( Vec2(210, sprInfo->getContentSize().height / 2) );
        sprInfo->addChild(labelTitle1);
        
        auto labelTitle2 = Label::createWithTTF(GAME_TEXT("t_ui_reward"), GAME_FONT, 24);
        labelTitle2->setPosition( Vec2(565, sprInfo->getContentSize().height / 2) );
        sprInfo->addChild(labelTitle2);
    }
    
    auto sprBox = ui::Scale9Sprite::create("Assets/ui/mission/mission_special_2.png");
    sprBox->setContentSize(Size(655,C_BAR_MISSION_SIZE));
    sprBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    sprBox->setPosition(Vec2(_lyTop->getContentSize().width/2, 0));
    _lyTop->addChild(sprBox);
    
    if(objMission == nullptr)
    {
        auto lbContent = Label::createWithTTF(GAME_TEXT("t_ui_mission_msg_1"), GAME_FONT, 28);
        lbContent->setAnchorPoint(Vec2(0,0));
        lbContent->setPosition(Vec2(sprBox->getContentSize().width*0.2, sprBox->getContentSize().height*0.47));
        lbContent->setDimensions(370, lbContent->getContentSize().height);
        lbContent->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
        lbContent->setOverflow(Label::Overflow::SHRINK);
        lbContent->setColor(Color3B(92,203,255));
        sprBox->addChild(lbContent);
        return;
    }
    
    auto objItem = objMission->getListMissionReward().at(0);
    auto sprRewardBox = Sprite::create("Assets/ui/mission/task_box_special.png");
    sprRewardBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    sprRewardBox->setPosition(10, sprBox->getContentSize().height/2);
    sprBox->addChild(sprRewardBox);
    if(objItem != nullptr)
    {
        auto spReward = Sprite::create(objItem->getPath());
        spReward->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        spReward->setPosition(sprRewardBox->getContentSize()/2);
        spReward->setScale(87 / spReward->getContentSize().width);
        sprRewardBox->addChild(spReward);
        
        if(objItem->getIdx() == (int)E_ITEMS::PTICKET)
        {
            spReward->setScale(spReward->getScale() / 1.5f);
        }
    }
    
    auto lbTextTop = Label::createWithTTF(GAME_TEXTFORMAT("t_new_mission_title", objMission->getIdx()), GAME_FONT, 20);
    lbTextTop->setAnchorPoint(Vec2(0,0));
    lbTextTop->setPosition(Vec2(sprBox->getContentSize().width*0.2, sprBox->getContentSize().height*0.74));
    lbTextTop->setDimensions(400, lbTextTop->getContentSize().height);
    lbTextTop->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
    lbTextTop->setOverflow(Label::Overflow::SHRINK);
    sprBox->addChild(lbTextTop);
    
    auto lbContent = Label::createWithTTF(objMission->getContent(), GAME_FONT, 28);//28로 맞출지는 보고 확인
    lbContent->setAnchorPoint(Vec2(0,0));
    lbContent->setPosition(Vec2(sprBox->getContentSize().width*0.2, sprBox->getContentSize().height*0.47));
    lbContent->setDimensions(370, lbContent->getContentSize().height);
    lbContent->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
    lbContent->setOverflow(Label::Overflow::SHRINK);
    lbContent->setColor(Color3B(92,203,255));
    sprBox->addChild(lbContent);
    
    std::string strTextBottom = "";
    if(objItem != nullptr)
    {
        strTextBottom = MafUtils::format("%s +%s", objItem->getText().c_str(), objItem->getCount().c_str());
    }
    auto lbTextBottom = Label::createWithTTF(strTextBottom, GAME_FONT, 19); //타이틀 내용은 비어있어야 한다.
    lbTextBottom->setColor(Color3B(167, 188, 208));
    lbTextBottom->setAnchorPoint(Vec2(0,0));
    lbTextBottom->setPosition(Vec2(sprBox->getContentSize().width*0.2, sprBox->getContentSize().height*0.1));
    lbTextBottom->setDimensions(400, lbTextBottom->getContentSize().height);
    lbTextBottom->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
    lbTextBottom->setOverflow(Label::Overflow::SHRINK);
    sprBox->addChild(lbTextBottom);
    
    std::string strReceive = "Assets/ui/mission/btn_shortcuts_1.png";
    auto func = CC_CALLBACK_1(LayerSpecialMission::onClickShortCut, this);
    if(SpecialMissionManager::getInstance()->getSpecialMissionComplete() == 1)
    {
        func = CC_CALLBACK_1(LayerSpecialMission::onClickComplete, this);
        strReceive = "Assets/ui/mission/btn_acheive_1.png";
    }
    
    auto btnReceive = ui::Button::create(strReceive);
    btnReceive->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    btnReceive->setPosition(Vec2(sprBox->getContentSize().width - 20, sprBox->getContentSize().height/2));
    btnReceive->setTag(objMission->getIdx());
    btnReceive->addClickEventListener(func);
    sprBox->addChild(btnReceive);

    if ( TutorialManager::getInstance()->getTutorialProgress().second == SUB_REWARD_BUTTON || TutorialManager::getInstance()->getTutorialProgress().second == SUB_SHORTCUT_BUTTON)
    {
        if(TutorialManager::getInstance()->getTutorialProgress().first == PROGRESS_END)
        {
            TutorialManager::getInstance()->removeAllEffect(TutorialManager::getInstance()->getTutorialProgress());
            TutorialManager::getInstance()->endTutorial();
        }
        auto nowlayer = UtilsScene::getInstance()->getRunningLayer();
        if (nowlayer != nullptr)
        {
            TutorialManager::getInstance()->attachEffect(EFFECT_FINGER, TutorialManager::getInstance()->getTutorialProgress(), btnReceive);
            TutorialManager::getInstance()->setPosition(Vec2(btnReceive->getContentSize().width/2+10, btnReceive->getContentSize().height/2));
        }
    }
}
void LayerSpecialMission::uiTable()
{
    _lyTable->removeAllChildren();

    _table = extension::TableView::create(this, Size(_lyTable->getContentSize().width, _lyTable->getContentSize().height));
    _table->setIgnoreAnchorPointForPosition(false);
    _table->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _table->setPosition(_lyTable->getContentSize().width / 2, _lyTable->getContentSize().height - 5);
    _table->setDirection(extension::TableView::Direction::VERTICAL);
    _table->setVerticalFillOrder(extension::TableView::VerticalFillOrder::TOP_DOWN);
    _table->setDelegate(this);
    _table->setBounceable(false);
    _lyTable->addChild(_table);

    _table->reloadData();
}

#pragma mark -utils
void LayerSpecialMission::onClickShortCut(Ref* sender)
{
    auto node = (Node*)sender;
    
    //씬이동이 아니면 닫는다
    if(SpecialMissionManager::getInstance()->onShortCut(node->getTag()) != E_SHORTCUTS::SHORTCUT_PETEQUIP ||
       SpecialMissionManager::getInstance()->onShortCut(node->getTag()) != E_SHORTCUTS::SHORTCUT_DEVILDOM)
    {
        if(_callbackHide != nullptr)
        {
            _callbackHide();
        }
    }
}
void LayerSpecialMission::onClickComplete(Ref* sender)
{
    auto node = (Node*)sender;
    auto objMission = SpecialMissionManager::getInstance()->getInfoSpecialMission(node->getTag());
    if(objMission == nullptr)
        return;
    
    PopupLoading::show();
    SpecialMissionManager::getInstance()->requestMissionReward(CC_CALLBACK_2(LayerSpecialMission::callbackComplete, this), objMission->getIdx());
}

void LayerSpecialMission::setCallbackHide(const std::function<void()>& pCallback)
{
    _callbackHide = pCallback;
}
void LayerSpecialMission::setCallbackRefresh(const std::function<void()>& pCallback)
{
    _callbackRefresh = pCallback;
}

void LayerSpecialMission::callbackComplete(bool bResult, int nResult)
{
    PopupLoading::hide();

    if(bResult == false || nResult != 1)
    {
        std::string strTextKey = "t_ui_error_2";
        if(nResult == 0)
            strTextKey = "t_ui_mail_msg_3";
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT(strTextKey));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();

    }
    SpecialMissionManager::getInstance()->checkSpeicalMission(E_PLACE::DG_NORMAL);
    if(_callbackRefresh != nullptr)
        _callbackRefresh();
    
    uiReload();
    
    //
    if(TutorialManager::getInstance()->getTutorialProgress().first < PROGRESS_END)
    {
        TutorialManager::getInstance()->refreshTutorialProgress();
    }
}

void LayerSpecialMission::setMissionOder()
{
    auto tmp = SpecialMissionManager::getInstance()->getListSpecialMission();
    Vector<InfoSpecialMission*> listClear;
    Vector<InfoSpecialMission*> listOther;
    
    int mission = SpecialMissionManager::getInstance()->getSpecialMissionClear();
    for(int i = 0; i < mission; ++i)
    {
        listClear.pushBack(tmp.at(i));
    }
    for(int i = mission + 1; i < tmp.size(); ++i)
    {
        listOther.pushBack(tmp.at(i));
    }
    
    _listInfoSpecial.clear();
    
    _listInfoSpecial = listOther;
    _listInfoSpecial.pushBack(listClear);
}
