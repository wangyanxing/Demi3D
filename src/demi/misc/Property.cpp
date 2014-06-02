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

#include "MiscPch.h"
#include "Property.h"

namespace Demi
{
    DiPropertyBase::~DiPropertyBase()
    {
        clearSubscribers(); clearDependencies();
    }

    DiPropertyBase::DiPropertyBase(const DiPropertyBase &other) : dependencies(other.dependencies)
    {
        for (DiPropertyBase *p : dependencies)
            p->subscribers.insert(this);
    }

    void DiPropertyBase::notify()
    {
        auto copy = subscribers;
        for (DiPropertyBase *p : copy) {
            p->evaluate();
        }
    }

    void DiPropertyBase::accessed()
    {
        if (current && current != this) {
            subscribers.insert(current);
            current->dependencies.insert(this);
        }
    }

    void DiPropertyBase::clearSubscribers()
    {
        for (DiPropertyBase *p : subscribers)
            p->dependencies.erase(this);
        subscribers.clear();
    }

    void DiPropertyBase::clearDependencies()
    {
        for (DiPropertyBase *p : dependencies)
            p->subscribers.erase(this);
        dependencies.clear();
    }

    DiPropertyBase * DiPropertyBase::current = nullptr;
}