/* Copyright (C) 2013 Olivier Goffart <ogoffart@woboq.com>
   http://woboq.com/blog/property-bindings-in-cpp.html

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
associated documentation files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial
portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef DiProperty_h__
#define DiProperty_h__

#include <functional>
#include <unordered_set>
#include <type_traits>

namespace Demi
{
    enum DiPropertyType
    {
        PROPERTY_STRING,
        PROPERTY_INT,
        PROPERTY_FLOAT,
        PROPERTY_BOOL,
        PROPERTY_VEC2,
        PROPERTY_VEC3,
        PROPERTY_VEC4,
        PROPERTY_QUAT,
        PROPERTY_COLOR,
        PROPERTY_DYN_ATTR,

        PROPERTY_MAX,
    };

    class DI_MISC_API DiPropertyBase
    {
        /* Set of properties which are subscribed to this one.
           When this property is changed, subscriptions are refreshed */
        std::unordered_set<DiPropertyBase *> subscribers;

        /* Set of properties this property is depending on. */
        std::unordered_set<DiPropertyBase *> dependencies;

    public:
        virtual ~DiPropertyBase();

        // re-evaluate this property
        virtual void evaluate() = 0;

        DiPropertyBase() = default;
        DiPropertyBase(const DiPropertyBase &other);
        DiPropertyType getType() { return type; }

    protected:
        /* This function is called by the derived class when the property has changed
           The default implementation re-evaluates all the property subscribed to this one. */
        virtual void notify();

        /* Derived class call this function whenever this property is accessed.
           It register the dependencies. */
        void accessed();

        void clearSubscribers();
        void clearDependencies();

        /* Helper class that is used on the stack to set the current property being evaluated */
        struct evaluation_scope {
            evaluation_scope(DiPropertyBase *prop) : previous(current) {
                current = prop;
            }
            ~evaluation_scope() { current = previous; }
            DiPropertyBase *previous;
        };

        DiPropertyType type;

    private:
        friend struct evaluation_scope;
        /* thread_local */ static DiPropertyBase *current;
    };

    /** The property class represents a property of type T that can be assigned a value, or a bindings.
        When assigned a bindings, the binding is re-evaluated whenever one of the property used in it
        is changed */
    template <typename T, DiPropertyType p_type = PROPERTY_MAX>
    struct DiProperty : DiPropertyBase{
        typedef std::function<T()> getter_t;
        typedef std::function<void(T&)> setter_t;

        DiProperty() { type = p_type; }
        DiProperty(const T &t) : value(t) { type = p_type; }
        DiProperty(const getter_t &gter) : getter(gter) { type = p_type; evaluate(); }
        DiProperty(const getter_t &gter, const setter_t &ster) : getter(gter), setter(ster) { type = p_type; evaluate(); }

        void operator=(T &t) {
            value = t;
            if (setter){
                setter(t);
            }
            clearDependencies();
            notify();
        }
        void operator=(const getter_t &b) {
            getter = b;
            evaluate();
        }

        //make it possible to initialize directly with lamda without any casts
        template<typename B> DiProperty(const B &b, typename std::enable_if<std::is_constructible<T, B>::value, int*>::type = nullptr) : DiProperty(T(b)) {}
        template<typename B> typename std::enable_if<std::is_constructible<T, B>::value>::type operator= (const B &b) { *this = T(b); }
        template<typename B> DiProperty(const B &b, typename std::enable_if<std::is_constructible<getter_t, B>::value && !std::is_constructible<T, B>::value, int*>::type = nullptr) : DiProperty(getter_t(b)) {}
        template<typename B> typename std::enable_if<std::is_constructible<getter_t, B>::value && !std::is_constructible<T, B>::value>::type operator= (const B &b) { *this = getter_t(b); }

        const T &get() const {
            const_cast<DiProperty*>(this)->accessed();
            return value;
        }

        //automatic conversions
        const T &operator()() const { return get(); }
        operator const T&() const { return get(); }

        void evaluate() override {
            if (getter) {
                clearDependencies();
                evaluation_scope scope(this);
                value = getter();
            }
            notify();
        }

    protected:
        T value;
        getter_t getter;
        setter_t setter;
    };

    template<typename T, DiPropertyType p_type = PROPERTY_MAX>
    struct DiPropertyHook : DiProperty<T,p_type> {
        typedef std::function<void()> hook_t;
        typedef typename DiProperty<T, p_type>::getter_t binding_t;
        void notify() override {
            DiProperty<T, p_type>::notify();
            hook();
        }
        DiPropertyHook(hook_t h) : hook(h) { }
        DiPropertyHook(hook_t h, const T &t) : DiProperty<T, p_type>(t), hook(h) { }
        DiPropertyHook(hook_t h, binding_t b) : DiProperty<T, p_type>(b), hook(h) { }
        using DiProperty<T, p_type>::operator=;
    private:
        hook_t hook;
    };

    /** property_wrapper do not own the property, but use a getter and a setter */
    template <typename T, DiPropertyType p_type = PROPERTY_MAX>
    struct DiPropertyWrapper : DiPropertyBase {
        typedef std::function<T()> binding_t;
        typedef std::function<void(const T&)> write_hook_t;
        typedef std::function<T()> read_hook_t;
        explicit DiPropertyWrapper(write_hook_t w, read_hook_t r) : write_hook(std::move(w)), read_hook(std::move(r)) { }

        T get() const {
            const_cast<DiPropertyWrapper*>(this)->accessed();
            return read_hook();
        }
        void operator=(const T &t) {
            write_hook(t);
            notify();
        }

        void operator=(const binding_t &b) {
            binding = b;
            evaluate();
        }

        T operator()() const { return get(); }
        operator T() const { return get(); }

        using DiPropertyBase::notify;
    protected:
        void evaluate() override {
            if (binding) {
                clearDependencies();
                evaluation_scope scope(this);
                write_hook(binding());
            }
            notify();
        }
    private:
        const write_hook_t write_hook;
        const read_hook_t read_hook;
        binding_t binding;
    };
}

#endif // Property_h__
