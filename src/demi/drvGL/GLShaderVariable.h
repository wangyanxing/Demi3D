
/********************************************************************
    File:       GLShaderVariable.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

#include "GpuProgram.h"

namespace Demi
{
    class DI_GLDRV_API DiGLGpuVariable : public DiGpuVariable
    {
    public:

        DiGLGpuVariable(const DiString& name, Type type);

        ~DiGLGpuVariable(void);

    public:

        void        Bind() const;

    private:

    };
}