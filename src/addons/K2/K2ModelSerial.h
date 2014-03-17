/**********************************************************************
This source file is a part of Demi3D
   __  ___  __  __  __
  |  \|_ |\/||   _)|  \ 
  |__/|__|  ||  __)|__/ 

Copyright (c) 2013-2014 Demi team
https://github.com/wangyanxing/Demi3D

Released under the MIT License
https://github.com/wangyanxing/Demi3D/blob/master/License.txt
***********************************************************************/

#ifndef DiK2ModelSerial_h__
#define DiK2ModelSerial_h__

#include "K2Prerequisites.h"
#include "Serializer.h"

namespace Demi
{
    class DEMI_K2_API DiK2MdfSerial : public DiSerializer
    {
    public:

        DiK2MdfSerial();

        ~DiK2MdfSerial();

    public:

        bool    ParseMdf(DiDataStreamPtr data, DiK2Model* target);

        bool    ParseMdf(DiXMLElement data, DiK2Model* target);
        
        bool    LoadModel(DiDataStreamPtr data, DiK2Model* target);
        
    private:
        
        void    ParseAnim(DiXMLElement data, DiK2Model* target);
        
        bool    LoadBones(DiDataStreamPtr data, DiK2Model* target);
        
        bool    CheckFourcc(char* sig);
        
        bool    CheckFourcc(char* hed, char* sig);
    };
}

#endif
