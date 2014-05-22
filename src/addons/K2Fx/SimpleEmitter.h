
#include "EmitterDefs.h"

typedef unsigned long _DWORD;
typedef unsigned char _BYTE;

int __cdecl M_Randnum(int a1, int a2)
{
    int result; // eax@2

    if (a2 <= a1)
        result = a1;
    else
        result = rand() % (a2 + 1 - a1) + a1;
    return result;
}

class CSimpleEmitter
{
public:

    CSimpleEmitter(uint var, CParticleSystem* particleSys, IEmitter* emitter, CSimpleEmitterDef const& simplerEmitterDef)
    {
        _DWORD v101 = *(_DWORD *)(simplerEmitterDef + 768);
        _BYTE v102 = *(_BYTE *)(simplerEmitterDef + 128);

        _DWORD v5 = *(_DWORD *)(simplerEmitterDef + 120);
        _DWORD v6 = *(_DWORD *)(simplerEmitterDef + 124);
        _DWORD v104 = *(_DWORD *)(simplerEmitterDef + 124);
        if (v5 < v6)
            v104 = M_Randnum(v5, v6);

        _DWORD v7 = *(_DWORD *)(simplerEmitterDef + 112);
        _DWORD v8 = *(_DWORD *)(simplerEmitterDef + 116);
        if (v7 < v8)
            v8 = M_Randnum(v7, *(_DWORD *)(simplerEmitterDef + 116));

        _DWORD v9 = *(_DWORD *)(simplerEmitterDef + 96);
        _DWORD v10 = *(_DWORD *)(simplerEmitterDef + 100);
        if (v9 < v10)
            v10 = M_Randnum(v9, *(_DWORD *)(simplerEmitterDef + 100));

        _DWORD v11 = *(_DWORD *)(simplerEmitterDef + 88);
        _DWORD v12 = *(_DWORD *)(simplerEmitterDef + 92);
        if (v11 < v12)
            v12 = M_Randnum(v11, *(_DWORD *)(simplerEmitterDef + 92));

        IEmitter::IEmitter(
            this,
            v12,
            v10,
            v8,
            v104,
            v102,
            *(int *)((char *)&dword_209C87F + 1198005),
            simplerEmitterDef + 16,
            simplerEmitterDef + 780,
            simplerEmitterDef + 852,
            simplerEmitterDef + 864,
            v101,
            simplerEmitterDef + 4, //vector<simpleemitterdef*>
            particleSys,
            emitter,
            var);
    }
};