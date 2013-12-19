
#include "CodeConverter.h"

namespace Demi
{
    void DiCodeConvert::MultiByte2Unicode( const char* in,std::wstring& out,int inlen )
    {
        Char2WChar(CP_ACP,in,out,inlen);
    }

    void DiCodeConvert::MultiByte2Unicode( const char* in,wchar_t out[],int outsize,int inlen )
    {
        Char2WChar(CP_ACP,in,out,outsize,inlen);
    }

    void DiCodeConvert::Unicode2MultiByte( const wchar_t* in,std::string& out,int inlen )
    {
        WChar2Char(CP_ACP,in,out,inlen);
    }

    void DiCodeConvert::Unicode2MultiByte( const wchar_t* in,char out[],int outsize,int inlen/*=-1*/ )
    {
        WChar2Char(CP_ACP,in,out,outsize,inlen);
    }

    void DiCodeConvert::Unicode2UTF8( const wchar_t* in,std::string& out,int inlen/*=-1*/ )
    {
        WChar2Char(CP_UTF8,in,out,inlen);
    }

    void DiCodeConvert::Unicode2UTF8( const wchar_t* in,char out[],int outsize,int inlen/*=-1*/ )
    {
        WChar2Char(CP_UTF8,in,out,outsize,inlen);
    }

    void DiCodeConvert::UTF8_2Unicode( const char* in,std::wstring& out,int inlen/*=-1*/ )
    {
        Char2WChar(CP_UTF8,in,out,inlen);
    }

    void DiCodeConvert::UTF8_2Unicode( const char* in,wchar_t out[],int outsize,int inlen/*=-1*/ )
    {
        Char2WChar(CP_UTF8,in,out,outsize,inlen);
    }

    void DiCodeConvert::WChar2Char( uint32 codepage,const wchar_t* in,std::string& out,int inlen/*=-1*/ )
    {
        int size=WideCharToMultiByte(codepage, 0,in,inlen,NULL,0,NULL, 0);
        if(inlen>0)
        {
            size++;
        }
        char* words;
        words = DI_NEW char[size];
        WideCharToMultiByte(codepage, 0,in,inlen,words,size,NULL, 0);    
        out.assign(words,size-1);
        DI_DELETE (words);
    }

    void DiCodeConvert::WChar2Char( uint32 codepage,const wchar_t* in,char out[],int outsize,int inlen/*=-1*/ )
    {
        WideCharToMultiByte(codepage, 0,in,inlen,out,outsize, 0, 0);
    }
    void DiCodeConvert::Char2WChar( uint32 codepage,const char* in,std::wstring& out,int inlen/*=-1*/ )
    {
        int size=MultiByteToWideChar(codepage, 0,in,inlen,NULL,0);
        if(inlen>0)
        {
            size++;
        }
        wchar_t* twords;
        twords = DI_NEW wchar_t[size];
        MultiByteToWideChar(codepage, 0,in,inlen,twords,size);
        out.assign(twords,size-1);
        DI_DELETE (twords);
    }

    void DiCodeConvert::Char2WChar( uint32 codepage,const char* in,wchar_t out[],int outsize,int inlen/*=-1*/ )
    {
        MultiByteToWideChar(codepage, 0,in, inlen,out,outsize);
    }

    bool DiCodeConvert::ExistChinese(const std::wstring& tstr)
    {    
        std::string str;
        Unicode2MultiByte(tstr.c_str(),str);
        return tstr.size()!=str.size();
    }

    void DiCodeConvert::UTF8_2GB2132( const char* in,std::string& out,int inlen/*=-1*/ )
    {
        std::wstring un;
        UTF8_2Unicode(in,un);
        Unicode2MultiByte(un.c_str(),out);
    }

    void DiCodeConvert::GB2132_2UTF8( const char* in,std::string& out,int inlen/*=-1*/ )
    {
        std::wstring un;
        MultiByte2Unicode(in,un);
        Unicode2UTF8(un.c_str(),out);
    }
}