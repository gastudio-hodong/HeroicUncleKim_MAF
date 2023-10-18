//
//  UtilsWidget.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#include "UtilsWidget.h"

#include "Common/ConfigDefault.h"

USING_NS_CC;

static UtilsWidget *s_SharedUtilsWidget = nullptr;

UtilsWidget* UtilsWidget::getInstance()
{
    if (!s_SharedUtilsWidget)
    {
        s_SharedUtilsWidget = new (std::nothrow) UtilsWidget();
        s_SharedUtilsWidget->init();
    }
    
    return s_SharedUtilsWidget;
}

UtilsWidget::UtilsWidget()
{
    
}

UtilsWidget::~UtilsWidget()
{
    
}

bool UtilsWidget::init()
{
    return true;
}

Node* UtilsWidget::seekWidgetParent(Node* nodeRoot)
{
    Node* nodeParent = nodeRoot->getParent();
    
    while (nodeParent != nullptr) {
        if ( dynamic_cast<Scene*>(nodeParent) != nullptr )
        {
            break;
        }

        nodeParent = nodeParent->getParent();
    }
    
    return nodeParent;
}

void UtilsWidget::sortWidgetGrid(cocos2d::Node* root, int col, double width, double height, double lineSpacing, bool trimX/* = true*/, bool trimY/* = true*/)
{
    auto listChilds = root->getChildren();
    if ( listChilds.size() <= 0 )
    {
        return;
    }
    
    //
    double cellWidth = listChilds.at(0)->getContentSize().width;
    double cellHeight = listChilds.at(0)->getContentSize().height;
    
    // width
    if ( width == 0 )
    {
        width = cellWidth;
    }
    
    // col
    if ( col == 0 )
    {
        col = width / cellWidth;
    }
    
    // padding x
    double padding = (width - cellWidth * col) / (col - 1);
    if ( trimX == false )
    {
        padding = (width - cellWidth * col) / (col + 1);
    }
    
    // row
    int row = (int)(listChilds.size() / col);
    if ( listChilds.size() % col != 0 )
    {
        row++;
    }
    
    // height
    if ( height == 0 )
    {
        height = cellHeight * row + lineSpacing * (row - 1);
        if ( trimY == false )
        {
            height = cellHeight * row + lineSpacing * (row + 1);
        }
    }
    
    // lineSpacing check
    if ( trimY == true && height < cellHeight * row + lineSpacing * (row - 1))
    {
        lineSpacing = (height - cellHeight * row) / (row - 1);
    }
    else if ( trimY == false && height < cellHeight * row + lineSpacing * (row + 1))
    {
        lineSpacing = (height - cellHeight * row) / (row + 1);
    }
    root->setContentSize(Size(width, height));
    
    //
    double posStartX = 0;
    double posStartY = root->getContentSize().height;
    if ( trimX == false )
    {
        posStartX += padding;
    }
    
    if ( trimY == false )
    {
        posStartY -= lineSpacing;
    }
    
    for ( int i = 0; i < listChilds.size(); i++ )
    {
        int x = i % col;
        int y = i / col;
        int posX = x * cellWidth + x * padding;
        int posY = y * cellHeight + y * lineSpacing;
        
        auto child = listChilds.at(i);
        child->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        child->setPosition(posStartX + posX, posStartY - posY);
    }
}

void UtilsWidget::sortWidgetGrid(Node* root, double padding/* = 10*/)
{
    auto listNode = root->getChildren();
        
    //
    ssize_t size = listNode.size();
    if ( size <= 0 )
    {
        return;
    }
    
    //
    Size sizeContent = root->getContentSize();
    
    double posStartX = 0;
    double posStartY = root->getContentSize().height;
    double posX = 0;
    double posY = 0;
    
    double heightCell = 0;
    
    auto listChilds = root->getChildren();
    for ( int i = 0; i < listChilds.size(); i++ )
    {
        auto child = listChilds.at(i);
        child->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        child->setPosition(posStartX + posX, posStartY - posY);
        
        posX = posX + child->getContentSize().width + padding;
        heightCell = heightCell > child->getContentSize().height ? heightCell : child->getContentSize().height;
        
        if ( posX >= sizeContent.width )
        {
            posX = 0;
            posY = posY + heightCell + padding;
            heightCell = 0;
        }
    }
}

cocos2d::Size UtilsWidget::getLayoutAccumulatedSize(cocos2d::ui::Layout* root)
{
    const auto& children = root->getChildren();
    Size layoutSize = Size::ZERO;
    int widgetCount =0;
    for(const auto& widget : children)
    {
        if ( widget->isVisible() == false )
        {
            continue;
        }
        
        ui::Widget *w = dynamic_cast<ui::Widget*>(widget);
        if (w)
        {
            widgetCount++;
            ui::Margin m = w->getLayoutParameter()->getMargin();
            layoutSize = layoutSize + w->getContentSize() + Size(m.right + m.left,  m.top + m.bottom);
        }
    }
    
    //subtract extra size
    ui::Layout::Type type = root->getLayoutType();
    if (type == ui::Layout::Type::HORIZONTAL)
    {
        layoutSize = layoutSize - Size(0, layoutSize.height/widgetCount * (widgetCount-1));
    }
    if (type == ui::Layout::Type::VERTICAL)
    {
        layoutSize = layoutSize - Size(layoutSize.width/widgetCount * (widgetCount-1), 0);
    }
    return layoutSize;
}

Node* UtilsWidget::createIconText(std::string strIconPath, std::string strText, double nWidthParent, int nFontSize/*= 30*/, int nIconPadding/* = 0*/)
{
    double nHeight = 0;
    double nWidth = 0;
    double nPadding = 10;
    
    auto nodeIcon = ui::ImageView::create(strIconPath);
    auto nodeText = ui::CText::create(strText, GAME_FONT, nFontSize);
    
    nWidth += nodeIcon->getContentSize().width - nIconPadding * 2;
    nWidth += nPadding;
    nWidth += nodeText->getContentSize().width;
    if ( nWidthParent != 0 )
    {
        nWidth = nWidth > nWidthParent ? nWidthParent : nWidth;
    }
    
    nHeight = nHeight > nodeIcon->getContentSize().height ? nHeight : nodeIcon->getContentSize().height;
    nHeight = nHeight > nodeText->getContentSize().height ? nHeight : nodeText->getContentSize().height;
    
    //
    auto node = ui::Layout::create();
    node->setContentSize(Size(nWidth, nHeight));
    node->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    nodeIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    nodeIcon->setPosition(Vec2(-nIconPadding, nHeight / 2));
    nodeIcon->setName("WIDGET_ICON");
    node->addChild(nodeIcon);
    
    nodeText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    nodeText->setPosition(Vec2(nodeIcon->getPositionX() + nodeIcon->getContentSize().width - nIconPadding + nPadding, nHeight / 2));
    nodeText->setTextAreaSize(Size(nWidth - (nodeIcon->getContentSize().width - nIconPadding - nIconPadding) - nPadding, nodeText->getContentSize().height));
    nodeText->setTextOverflow(Label::Overflow::SHRINK);
    nodeText->setTextVerticalAlignment(TextVAlignment::CENTER);
    nodeText->setTextHorizontalAlignment(TextHAlignment::LEFT);
    nodeText->enableWrap(false);
    nodeText->setName("WIDGET_TEXT");
    node->addChild(nodeText);
    
    return node;
}

cocos2d::Vec2 UtilsWidget::getSizeToPercent(cocos2d::Vec2 size, double posX, double posY)
{
    return Vec2(posX / size.x, posY / size.y);
}
