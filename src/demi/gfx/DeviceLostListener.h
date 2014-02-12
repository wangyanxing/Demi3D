


#ifndef DiDeviceLostListener_h__
#define DiDeviceLostListener_h__


namespace Demi 
{
    class DI_GFX_API DiDeviceLostListener : public DiBase
    {
    public:

        DiDeviceLostListener()
        {
            mDeviceLost = false;
        }

        virtual void    OnDeviceLost()    = 0;

        virtual void    OnDeviceReset()    = 0;

    protected:

        bool            mDeviceLost;
    };
}

#endif
