
#include "EmitterDefs.h"

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
        v101 = *(_DWORD *)(simplerEmitterDef + 768);
        v102 = *(_BYTE *)(simplerEmitterDef + 128);

        int v5 = *(DWORD *)(simplerEmitterDef + 120);
        int v6 = *(DWORD *)(simplerEmitterDef + 124);

        int v104 = *(DWORD *)(simplerEmitterDef + 124);
        if (v5 < v6)
            v104 = M_Randnum(v5, v6);

        int v7 = *(DWORD *)(simplerEmitterDef + 112);
        int v8 = *(DWORD *)(simplerEmitterDef + 116);
        if (v7 < v8)
            v8 = M_Randnum(v7, *(DWORD *)(simplerEmitterDef + 116));

        int v9 = *(DWORD *)(simplerEmitterDef + 96);
        int v10 = *(DWORD *)(simplerEmitterDef + 100);
        if (v9 < v10)
            v10 = M_Randnum(v9, *(DWORD *)(simplerEmitterDef + 100));

        int v11 = *(DWORD *)(simplerEmitterDef + 88);
        int v12 = *(DWORD *)(simplerEmitterDef + 92);
        if (v11 < v12)
            v12 = M_Randnum(v11, *(DWORD *)(simplerEmitterDef + 92));

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