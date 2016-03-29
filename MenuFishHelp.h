#ifndef FS_MenuSelect_h
#define FS_MenuSelect_h
#include "Header.h"
class SelectItem : public jai::MyNode
{
    int mIndex;
public:
    SelectItem(int index, std::function<void(Ref*)> selectCall);
};

class MenuSelect : public jai::MyNode
{
    std::vector<SelectItem* > mItems;
    int mCurrentItem;
    CC::Vec2 originalPosition;
public:
    MenuSelect();
    void selectLevel(int index);
    void updateScale();
    void updatePosition();
};

