
/********************************************************************
    File:       Any.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

#include <algorithm>
#include <typeinfo>

namespace Demi 
{
    class DiAny
    {
    public: // constructors

        DiAny()
          : mContent(0)
        {
        }

        template<typename ValueType>
        explicit DiAny(const ValueType & value)
          : mContent(new holder<ValueType>(value))
        {
        }

        DiAny(const DiAny & other)
          : mContent(other.mContent ? other.mContent->clone() : 0)
        {
        }

        virtual ~DiAny()
        {
            destroy();
        }

    public: // modifiers

        DiAny& swap(DiAny & rhs)
        {
            std::swap(mContent, rhs.mContent);
            return *this;
        }

        template<typename ValueType>
        DiAny& operator=(const ValueType & rhs)
        {
            DiAny(rhs).swap(*this);
            return *this;
        }

        DiAny & operator=(const DiAny & rhs)
        {
            DiAny(rhs).swap(*this);
            return *this;
        }

    public: // queries

        bool isEmpty() const
        {
            return !mContent;
        }

        const std::type_info& getType() const
        {
            return mContent ? mContent->getType() : typeid(void);
        }

        void destroy()
        {
            delete mContent;
            mContent = NULL;
        }

    protected: // types

        class placeholder 
        {
        public: // structors
    
            virtual ~placeholder()
            {
            }

        public: // queries

            virtual const std::type_info& getType() const = 0;

            virtual placeholder * clone() const = 0;
        };

        template<typename ValueType>
        class holder : public placeholder
        {
        public: // structors

            holder(const ValueType & value)
              : held(value)
            {
            }

        public: // queries

            virtual const std::type_info & getType() const
            {
                return typeid(ValueType);
            }

            virtual placeholder * clone() const
            {
                return new holder(held);
            }

        public: // representation

            ValueType held;

        };



    protected: // representation
        placeholder * mContent;

        template<typename ValueType>
        friend ValueType * any_cast(DiAny *);


    public: 

        template<typename ValueType>
        ValueType operator()() const
        {
            if (!mContent) 
            {
                DI_ERROR("Any::operator() mContent is NULL");
            }
            else if(getType() == typeid(ValueType))
            {
                 return static_cast<DiAny::holder<ValueType> *>(mContent)->held;
            }
            else
            {
                DI_ERROR("Any::operator() error type : %s, %s",getType().name(),typeid(ValueType).name());
            }
        }
    };

    class AnyNumeric : public DiAny
    {
    public:
        AnyNumeric()
        : DiAny()
        {
        }

        template<typename ValueType>
        AnyNumeric(const ValueType & value)
            
        {
            mContent = new numholder<ValueType>(value);
        }

        AnyNumeric(const AnyNumeric & other)
            : DiAny()
        {
            mContent = other.mContent ? other.mContent->clone() : 0; 
        }

    protected:
        class numplaceholder : public DiAny::placeholder
        {
        public: // structors

            ~numplaceholder()
            {
            }
            virtual placeholder* add(placeholder* rhs) = 0;
            virtual placeholder* subtract(placeholder* rhs) = 0;
            virtual placeholder* multiply(placeholder* rhs) = 0;
            virtual placeholder* multiply(float factor) = 0;
            virtual placeholder* divide(placeholder* rhs) = 0;
        };

        template<typename ValueType>
        class numholder : public numplaceholder
        {
        public: // structors

            numholder(const ValueType & value)
                : held(value)
            {
            }

        public: // queries

            virtual const std::type_info & getType() const
            {
                return typeid(ValueType);
            }

            virtual placeholder * clone() const
            {
                return DI_NEW(numholder)(held);
            }

            virtual placeholder* add(placeholder* rhs)
            {
                return DI_NEW(numholder)(held + static_cast<numholder*>(rhs)->held);
            }
            virtual placeholder* subtract(placeholder* rhs)
            {
                return DI_NEW(numholder)(held - static_cast<numholder*>(rhs)->held);
            }
            virtual placeholder* multiply(placeholder* rhs)
            {
                return DI_NEW(numholder)(held * static_cast<numholder*>(rhs)->held);
            }
            virtual placeholder* multiply(float factor)
            {
                return DI_NEW(numholder)(held * factor);
            }
            virtual placeholder* divide(placeholder* rhs)
            {
                return DI_NEW(numholder)(held / static_cast<numholder*>(rhs)->held);
            }

        public: 

            ValueType held;

        };

        AnyNumeric(placeholder* pholder)
        {
            mContent = pholder;
        }

    public:
        AnyNumeric & operator=(const AnyNumeric & rhs)
        {
            AnyNumeric(rhs).swap(*this);
            return *this;
        }
        AnyNumeric operator+(const AnyNumeric& rhs) const
        {
            return AnyNumeric(
                static_cast<numplaceholder*>(mContent)->add(rhs.mContent));
        }
        AnyNumeric operator-(const AnyNumeric& rhs) const
        {
            return AnyNumeric(
                static_cast<numplaceholder*>(mContent)->subtract(rhs.mContent));
        }
        AnyNumeric operator*(const AnyNumeric& rhs) const
        {
            return AnyNumeric(
                static_cast<numplaceholder*>(mContent)->multiply(rhs.mContent));
        }
        AnyNumeric operator*(float factor) const
        {
            return AnyNumeric(
                static_cast<numplaceholder*>(mContent)->multiply(factor));
        }
        AnyNumeric operator/(const AnyNumeric& rhs) const
        {
            return AnyNumeric(
                static_cast<numplaceholder*>(mContent)->divide(rhs.mContent));
        }
        AnyNumeric& operator+=(const AnyNumeric& rhs)
        {
            *this = AnyNumeric(
                static_cast<numplaceholder*>(mContent)->add(rhs.mContent));
            return *this;
        }
        AnyNumeric& operator-=(const AnyNumeric& rhs)
        {
            *this = AnyNumeric(
                static_cast<numplaceholder*>(mContent)->subtract(rhs.mContent));
            return *this;
        }
        AnyNumeric& operator*=(const AnyNumeric& rhs)
        {
            *this = AnyNumeric(
                static_cast<numplaceholder*>(mContent)->multiply(rhs.mContent));
            return *this;
        }
        AnyNumeric& operator/=(const AnyNumeric& rhs)
        {
            *this = AnyNumeric(
                static_cast<numplaceholder*>(mContent)->divide(rhs.mContent));
            return *this;
        }
    };


    template<typename ValueType>
    ValueType * any_cast(DiAny * operand)
    {
        return operand && operand->getType() == typeid(ValueType)
                    ? &static_cast<DiAny::holder<ValueType> *>(operand->mContent)->held
                    : 0;
    }

    template<typename ValueType>
    const ValueType * any_cast(const DiAny * operand)
    {
        return any_cast<ValueType>(const_cast<DiAny *>(operand));
    }

    template<typename ValueType>
    ValueType any_cast(const DiAny & operand)
    {
        const ValueType * result = any_cast<ValueType>(&operand);
        if(!result)
        {
            DI_WARNING("any_cast failed");
        }
        return *result;
    }

    //////////////////////////////////////////////////////////////////////////

    class DiAnyInterface
    {
    public:

        DiAny&          GetUserData() { return mAnyData; }

        void            SetUserData(const DiAny& val) { mAnyData = val; }

    protected:
        DiAny           mAnyData;
    };
}