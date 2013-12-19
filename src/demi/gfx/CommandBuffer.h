
/********************************************************************
    File:       CommandBuffer.h
    Creator:    demiwangya
*********************************************************************/

namespace Demi
{
    enum DiCommandID
    {
        C_CREATE_DEVICE = 0,

        C_COMMAND_NUM
    };

    class DiCommandBuffer : public DiBase
    {
    public:

        DiCommandBuffer();

        ~DiCommandBuffer();

    public:


    };
}