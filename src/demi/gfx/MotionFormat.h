


#ifndef DiMotionFormat_h__
#define DiMotionFormat_h__


namespace Demi
{
    enum DiMotionChunkID
    {
        DI_SKELETON                         = 0x1000,
            DI_BONES                        = 0x1100,
            DI_BONES_PARENT                 = 0x1200,

        DI_ATTACHSET                        = 0x2000,
            DI_ATTACHS                      = 0x2100,
            DI_ATTACHS_PARENT               = 0x2200,

        DI_ANIMATION                        = 0x3000,
            DI_CLIPS_NODE                   = 0x3100,
            DI_CLIPS_POSE                   = 0x3200,
            DI_CLIPS_ATTACH                 = 0x3300,
        
        DI_POSE                             = 0x4000,
    };
}

#endif
