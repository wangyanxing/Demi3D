
/********************************************************************
    File:       CodeConverter.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

namespace Demi
{
    class DiCodeConvert
    {
    public:
        
        static void MultiByte2Unicode(const char* in,std::wstring& out,int inlen=-1);

        static void MultiByte2Unicode(const char* in,wchar_t out[],int outsize,int inlen=-1);

        static void Unicode2MultiByte(const wchar_t* in,std::string& out,int inlen=-1);
        
        static void Unicode2MultiByte(const wchar_t* in,char out[],int outsize,int inlen=-1);

        static void Unicode2UTF8(const wchar_t* in,std::string& out,int inlen=-1);
        
        static void Unicode2UTF8(const wchar_t* in,char out[],int outsize,int inlen=-1);
        
        static void UTF8_2Unicode(const char* in,std::wstring& out,int inlen=-1);
        
        static void UTF8_2Unicode(const char* in,wchar_t out[],int outsize,int inlen=-1);

        static void UTF8_2GB2132(const char* in,std::string& out,int inlen=-1);

        static void GB2132_2UTF8(const char* in,std::string& out,int inlen=-1);

        static bool ExistChinese(const std::wstring& tstr);

    protected:

        static void WChar2Char(uint32 codepage,const wchar_t* in,std::string& out,int inlen=-1);
        
        static void WChar2Char(uint32 codepage,const wchar_t* in,char out[],int outsize,int inlen=-1);
        
        static void Char2WChar(uint32 codepage,const char* in,std::wstring& out,int inlen=-1);
        
        static void Char2WChar(uint32 codepage,const char* in,wchar_t out[],int outsize,int inlen=-1);
    };
}