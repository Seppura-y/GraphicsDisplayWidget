#pragma once
#include <QGraphicsItem>
enum ItemsType
{
	eBorderDot = QGraphicsItem::UserType + 1,//65537
	eBackgroundItem = QGraphicsItem::UserType + 2,
};

enum OperationState
{
	eMouseMoving = 0x0000,
	eMouseSelection = 0x0001,
	eGroupItemMoving = 0x0002,
	eGroupItemResizing = 0x0003,
};

struct ItemsAspectRatio
{
	int width;
	int height;
};

enum HandleFlags {
    Top_Handle = 0x01,
    Bottom_Handle = 0x02,
    Left_Handle = 0x04,
    Right_Handle = 0x08,
    TopLeft_Handle = Top_Handle | Left_Handle,
    TopRight_Handle = Top_Handle | Right_Handle,
    BottomLeft_Handle = Bottom_Handle | Left_Handle,
    BottomRight_Handle = Bottom_Handle | Right_Handle
};