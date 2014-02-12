


#ifndef DiInfo_h__
#define DiInfo_h__


#if 0

#include "MyGUI.h"

namespace Demi
{
    class DEMI_DEMO_API DiInfo : public DiBase
    {
    public:

        DiInfo();

        virtual                     ~DiInfo();

    public:

        template <typename T>
        void                        Change(const std::string& _key, const T& _value)
        {
            for (MyGUI::VectorStringPairs::iterator iter = mParams.begin(); iter != mParams.end(); ++iter)
            {
                if (iter->first == _key)
                {
                    iter->second = MyGUI::utility::toString<T>(_value);
                    return;
                }
            }
            mParams.push_back(std::make_pair(_key, MyGUI::utility::toString<T>(_value)));
        }

        void                        Remove(const std::string& key);

        void                        Update(float delta);

        void                        Clear()
        {
            mParams.clear();
        }

        void                        Clear(const std::string& key);

        void                        SetVisible(bool _value)
        {
            if (mInfo != nullptr)
                mInfo->setVisible(_value);
        }

        bool                        GetVisible()
        {
            if (mInfo != nullptr)
                return mInfo->getVisible();
            return false;
        }

        void                        SetOffset(const MyGUI::IntPoint& value)
        {
            mOffset = value;
        }

    private:

        MyGUI::TextBox*             mInfo;
        
        MyGUI::VectorStringPairs    mParams;

        MyGUI::IntPoint             mOffset;
    };
}

#endif

#endif
