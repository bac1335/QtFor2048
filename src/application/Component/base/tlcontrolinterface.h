#ifndef TLCONTROLINTERFACE_H
#define TLCONTROLINTERFACE_H

/*
 * 用于定义滑动相关的枚举、通用接口
 * 定义这个应该是由于Widget和Graphic都会用到吧
 */

class TLControlInterface
{
public:
    TLControlInterface();

    enum MoveFlag {
        Flag_MoveSelf,
        Flag_MoveByOutSide,
        Flag_NoMove
    };
    void setMoveFlag(const MoveFlag& flag);
    MoveFlag moveFlag() const;

    enum DragMode {
        Mode_NoDrag = 0x01,
        Mode_Click = 0x02,
        Mode_DragX_ToLeft = 0x04,
        Mode_DragX_ToRight,
        Mode_DragY_ToTop,
        Mode_DragY_ToBottom
    };
    void setDragMode(const DragMode& mode);
    DragMode dragMode() const;

    enum MoveLimit {
        MoveLimit_X = 0x01,
        MoveLimit_Y = 0x02,
        MoveLimit_Free = MoveLimit_X | MoveLimit_Y
    };
    void setMoveLimit(MoveLimit direction);
    MoveLimit moveLimit();

    void setModeChangeThreshold(int threshold);
    int modeChangeThreshold();

private:
    int m_nModeChangeThreshold = 20;   //模式改变阈值
    MoveFlag m_moveFlag = Flag_NoMove;
    DragMode m_dragMode = Mode_NoDrag;
    MoveLimit m_moveDirection = MoveLimit_Free;
};

#endif // TLCONTROLINTERFACE_H
