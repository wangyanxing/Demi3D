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

#ifndef DiStr_h__
#define DiStr_h__

#include "math/vector2.h"
#include "math/vector3.h"
#include "math/vector4.h"
#include "math/quaternion.h"
#include "math/matrix3.h"
#include "math/matrix4.h"
#include "color.h"

#include "vec.h"
#include <string>

#if DEMI_COMPILER == DEMI_COMPILER_MSVC
#   pragma warning( push )
#   pragma warning( disable : 4267 )
#endif

namespace Demi
{
    class DI_MISC_API DiString
    {
    public:

        DiString();
        DiString(const DiString& rhs);
        DiString(const char* cStr);
        DiString(const char* c_ptr, SizeT len);    //added by Li
        ~DiString();

        void operator=(const DiString& rhs);
        void operator=(const char* cStr);
        void operator +=(const DiString& rhs);

        char operator[](IndexT i) const;
        char& operator[](IndexT i);

        operator size_t();
        operator const size_t() const;

        DI_MISC_API friend bool operator ==(const DiString& a, const DiString& b);
        DI_MISC_API friend bool operator ==(const DiString& a, const char* cStr);
        DI_MISC_API friend bool operator ==(const char* cStr, const DiString& a);
        DI_MISC_API friend bool operator !=(const DiString& a, const DiString& b);
        DI_MISC_API friend bool operator <(const DiString& a, const DiString& b);
        DI_MISC_API friend bool operator >(const DiString& a, const DiString& b);
        DI_MISC_API friend bool operator <=(const DiString& a, const DiString& b);
        DI_MISC_API friend bool operator >=(const DiString& a, const DiString& b);

        void                reserve(SizeT newSize);
        void                resize(SizeT newSize);
        size_t              capacity() { return size_t(mHeapBufferSize); }

        void                clear();                                                              // clear the string
        bool                empty() const;                                                        // return true if string object is empty
        bool                IsValid() const;                                                      // return true if string object is not empty
        bool                CopyToBuffer(char* buf, SizeT bufSize) const;                         // copy to char buffer (return false if buffer is too small)

        void                AppendRange(const char* str, SizeT numChars);                         // append a range of characters
        void                Append(const DiString& str);                                          // append string
        void                Append(const char* str);                                              // append c-string

        void                ToLower();                                                            // convert string to lower case
        void                ToUpper();                                                            // convert string to upper case
        DiString            AsLower();
        DiString            AsUpper();
        void                FirstCharToUpper();                                                   // convert first char of string to upper case

        DiString            SubString(IndexT offset = 0, SizeT n = InvalidIndex) const;           // this substring is the character sequence that starts at character position pos and has a length of n characters   by  Li
        DiString            ExtractRange(IndexT fromIndex, SizeT numChars) const;                 // extract substring, the same as SubString  by  Li
        DiString            ExtractToEnd(IndexT fromIndex) const;                                 // extract substring to end of this string
        void                Strip(const DiString& charSet);                                       // terminate string at first occurence of character in set

        IndexT              FindStringIndex(const DiString& s, IndexT startIndex = 0) const;      // return start index of substring, or InvalidIndex if not found   Modified  by  Li
        IndexT              FindStringIndex(const char* s, IndexT startIndex = 0) const;          // return start index of substring of c-style, or InvalidIndex if not found   Added  by  Li
        IndexT              FindCharIndex(char c, IndexT startIndex = 0) const;                   // return index(first occurrence) of character in string, or InvalidIndex if not found
        IndexT              FindCharIndexFromSet(const DiString& charSet) const;                  // return index of the first occurrence in the string of any character from charSet searched for    Added  by  Li
        IndexT              FindCharLastIndex(char c) const;                                      // return index(last occurrence) of character in string, or InvalidIndex if not found     Added  by  Li

        void                TerminateAtIndex(IndexT index);                                       // terminate string at given index
        bool                ContainsCharFromSet(const DiString& charSet) const;                   // returns true if string contains any character from set
        void                TrimLeft(const DiString& charSet);                                    // delete characters from charset at left side of string
        void                TrimRight(const DiString& charSet);                                   // delete characters from charset at right side of string
        void                Trim(const DiString& charSet);                                        // trim characters from charset at both sides of string

        void                SubstituteString(const char* matchStr, const char* substStr);         // substitute every occurance of a string with another string  TODO: needs optimization
        void                SubstituteString(const DiString& matchStr, const DiString& substStr); // substitute every occurance of a string with another string
        void                SubstituteChar(char c, char subst);                                   // substiture every occurance of a character with another character
        bool                CheckValidCharSet(const DiString& charSet) const;                     // return true if string only contains characters from charSet argument
        
        void                ReplaceChars(const DiString& charSet, char replacement);

        void                Replace(char replaceThis, char replaceWith);
        void                Replace(const char* replaceThis, const char* replaceWith);

        void                SimplifyPath();

        SizeT               Tokenize(const DiString& whiteSpace, DiVector<DiString>& outTokens) const;                               // tokenize string into a provided String array (faster if tokens array can be reused)   
        DiVector<DiString>  Tokenize(const DiString& whiteSpace) const;                                                              // tokenize string into a provided String array, SLOW since new array will be constructed
        SizeT               TokenizeEx(const DiString& whiteSpace, DiVector<DiString>& outTokens, unsigned int maxSplits = 0) const; // tokenize string into a provided String array (faster if tokens array can be reused)   Added by Li
        DiVector<DiString>  TokenizeEx(const DiString& whiteSpace, unsigned int maxSplits = 0) const;                                // tokenize string into a provided String array, SLOW since new array will be constructed    Added by Li
        SizeT               Tokenize(const DiString& whiteSpace, char fence, DiVector<DiString>& outTokens) const;                   // tokenize string, keep strings within fence characters intact (faster if tokens array can be reused)
        DiVector<DiString>  Tokenize(const DiString& whiteSpace, char fence) const;                                                  // tokenize string, keep strings within fence characters intact, SLOW since new array will be constructed
                                                                                                                                     
        void __cdecl        Format(const char* fmtString, ...);                                                                      // format string printf-style
        void __cdecl        FormatArgList(const char* fmtString, va_list argList);                                                   // format string printf-style with varargs list
                                                                                                                                     
        static DiString     Concatenate(const DiVector<DiString>& strArray, const DiString& whiteSpace);                             // concatenate array of strings into new string
        static bool         MatchPattern(const DiString& str, const DiString& pattern);                                              // pattern matching, case sensitive true
        static bool         MatchPatternEx(const DiString& str, const DiString& pattern, bool caseSensitive = true);                 // pattern matching extended   Added by Li
        static bool         StartsWith(const DiString& str, const DiString& pattern, bool caseSensitive = true);                     // returns whether the string begins with the pattern passed in    Added by Li
        static bool         EndsWith(const DiString& str, const DiString& pattern, bool caseSensitive = true);                       // returns whether the string ends with the pattern passed in   Added by Li        
        static DiString     BLANK;

        size_t              ToHash() const;

        void                SetCharPtr(const char* s);                    // set content to char ptr
        void                Set(const char* ptr, SizeT length);           // set as char ptr, with explicit length
        const char*         AsCharPtr() const;                            // return contents as character pointer

        void                SetInt64(int64 val);                          // set as int value
        void                SetInt(int val);                              // set as int value
        void                SetUint(int val);                             // set as uint value
        void                SetFloat(float val);                          // set as float value
        void                SetBool(bool val);                            // set as bool value

        void                SetVector2(const DiVec2& v);
        void                SetVector3(const DiVec3& v);
        void                SetVector4(const DiVec4& v);
        void                SetQuaternion(const DiQuat& v);
        void                SetMatrix3(const DiMat3& v);
        void                SetMatrix4(const DiMat4& v);
        void                SetColourValue(const DiColor& v);
        void                SetFloatRect(const DiFloatRect& v);

        /// generic setter
        template<typename T> void Set(const T& t);

        void                AppendInt(int val);
        void                AppendFloat(float val);
        void                AppendBool(bool val);

        void                AppendVector2(const DiVec2& v);
        void                AppendVector3(const DiVec3& v);
        void                AppendVector4(const DiVec4& v);
        void                AppendQuaternion(const DiQuat& v);
        void                AppendMatrix3(const DiMat3& v);
        void                AppendMatrix4(const DiMat4& v);
        void                AppendColourValue(const DiColor& v);
        void                AppendFloatRect(const DiFloatRect& v);

        /// generic append
        template<typename T> void Append(const T& t);
        
        int                 AsInt() const;            // return contents as integer
        unsigned long       AsUint32() const;         // return contents as unsigned long integer
        float               AsFloat() const;          // return contents as float
        bool                AsBool() const;           // return contents as bool

        DiVec2              AsVector2() const;
        DiVec3              AsVector3() const;
        DiVec4              AsVector4() const;
        DiQuat              AsQuaternion(bool wFirst = true) const;//w x y z OR x y z w
        DiMat3              AsMatrix3() const;
        DiMat4              AsMatrix4() const;
        DiColor             AsColourValue() const;
        DiFloatRect         AsFloatRect() const;


        /// convert to "anything"
        template<typename T> T As() const;

        /// return true if the content is a valid integer
        bool IsValidInt() const;
        /// return true if the content is a valid float
        bool IsValidFloat() const;
        /// return true if the content is a valid bool
        bool IsValidBool() const;

        bool IsValidVector2() const;
        bool IsValidVector3() const;
        bool IsValidVector4() const;
        bool IsValidQuaternion() const;
        bool IsValidMatrix3() const;
        bool IsValidMatrix4() const;
        bool IsValidColourValue() const;

        //#endif
        /// generic valid checker
        template<typename T> bool IsValid() const;

        
        static DiString    FromUint(int i);            // construct a string from an int
        static DiString    FromInt(int i);             // construct a string from an int
        static DiString    FromFloat(float f);         // construct a string from a float
        static DiString    FromBool(bool b);           // construct a string from a bool

        static DiString    FromVector2(const DiVec2& v);
        static DiString    FromVector3(const DiVec3& v);
        static DiString    FromVector4(const DiVec4& v);
        static DiString    FromQuaternion(const DiQuat& v);
        static DiString    FromMatrix3(const DiMat3& v);
        static DiString    FromMatrix4(const DiMat4& v);
        static DiString    FromColourValue(const DiColor& v);
        static DiString    FromFloatRect(const DiFloatRect& v);

        /// convert from "anything"
        template<typename T> 
        static DiString From(const T& t);

        
        DiString    GetFileExtension() const;
        bool        CheckFileExtension(const DiString& ext) const;
        void        ConvertBackslashes();
        void        StripFileExtension();
        void        ChangeFileExtension(const DiString& newExt);
        DiString    ExtractFileName() const;
        DiString    ExtractBaseName() const;
        DiString    ExtractLastDirName() const;
        DiString    ExtractDirName() const;
        DiString    ExtractToLastSlash() const;
        void        SplitFilename(DiString& outBasename, DiString& outPath);
        void        ReplaceIllegalFilenameChars(char replacement);
        int         CompareNoCase(const char* o) const;

        static bool               IsDigit(char c);
        static bool               IsAlpha(char c);
        static bool               IsAlNum(char c);
        static bool               IsLower(char c);
        static bool               IsUpper(char c);

        static int                StrCmp(const char* str0, const char* str1);
        static int                StrLen(const char* str);
        static const char*        StrChr(const char* str, int c);
        static const char*        StrChrEx(const char *first, SizeT n, const char& ch);


        //--------------------------------------------------------------------------------------------------------------------------------------------
        // methods in std style, the PERFORMANCE is GOOD, but NOT RECOMMENDED in PHILOTES !! by Li
        //--------------------------------------------------------------------------------------------------------------------------------------------
        typedef SizeT size_type;
        typedef char value_type;
        static const size_type npos;

        DiString(const std::string& rhs);//TODO: construct from String, LEFT TEMPORARILY !!! by Li
        DiString AsStdString() const{ return DiString(this->AsCharPtr()); }    //TODO: return String, LEFT TEMPORARILY !!! by Li

        const char*      c_str() const { return this->AsCharPtr(); }
        size_type        length() const { return this->mStrLen; }
        size_type        size() const { return this->mStrLen; }
        DiString&        append ( const DiString& str ) { Append(str); return(*this); }
        DiString&        append ( const char* s, size_type n ) { AppendRange(s, n); return(*this); }
        DiString&        append ( const char* s ) { Append(s); return(*this); }
        DiString         substr( size_type offset = 0, size_type n = npos) const { return SubString(offset, n); }
        size_type        rfind( const char* ptr, size_type offset, size_type n) const; //BEST PERFORMANCE!
        size_type        rfind( char ch, size_type offset = npos ) const { return ( rfind((const char *)&ch, offset, 1) ); }
        size_type        rfind( const char* ptr, size_type offset = npos) const { return ( rfind(ptr, offset, strlen(ptr)) ); }
        size_type        rfind( const DiString& right, size_type offset = npos) const { return ( rfind(right.AsCharPtr(), offset, right.size()) ); }
        size_type        find( const char* ptr, size_type offset, size_type n) const;
        size_type        find( const char* ptr, size_type offset = 0) const { return ( find(ptr, offset, strlen(ptr)) ); }
        size_type        find( char ch, size_type offset = 0) const { return ( FindCharIndex(ch, offset) ); }
        size_type        find( const DiString& right, size_type offset = 0) const { return ( find(right.AsCharPtr(), offset, right.size()) ); }
        size_type        find_first_of( char ch, size_type offset = 0) const { return FindCharIndex(ch, offset); }

    private:
        
        void        Delete();                // delete contents
        char*       GetLastSlash() const;    // get pointer to last directory separator
        void        Alloc(SizeT size);       // allocate the string buffer (discards old content)
        void        Realloc(SizeT newSize);  // (re-)allocate the string buffer (copies old content)

        enum
        {
            LocalStringSize = 32,
        };
        char*    mHeapBuffer;
        char    mLocalBuffer[LocalStringSize];
        SizeT    mStrLen;
        SizeT    mHeapBufferSize;
    };

    //------------------------------------------------------------------------------
    inline
    DiString::DiString() :
        mHeapBuffer(0),
        mStrLen(0),
        mHeapBufferSize(0)
    {
        this->mLocalBuffer[0] = 0;
    }

    //------------------------------------------------------------------------------
    inline void
    DiString::Delete()
    {
        if (this->mHeapBuffer)
        {
            free((void*) this->mHeapBuffer);
            this->mHeapBuffer = 0;
        }
        this->mLocalBuffer[0] = 0;
        this->mStrLen = 0;
        this->mHeapBufferSize = 0;
    }

    //------------------------------------------------------------------------------
    inline 
    DiString::~DiString()
    {
        this->Delete();
    }

    //------------------------------------------------------------------------------
    /**
        Allocate a new heap buffer, discards old contents.
    */
    inline void
    DiString::Alloc(SizeT newSize)
    {
        DI_ASSERT(newSize > (this->mStrLen + 1));
        DI_ASSERT(newSize > this->mHeapBufferSize);

        // free old buffer
        if (this->mHeapBuffer)
        {
            free((void*) this->mHeapBuffer);
            this->mHeapBuffer = 0;
        }

        // allocate new buffer
        this->mHeapBuffer = (char*) malloc (newSize);
        this->mHeapBufferSize = newSize;
        this->mLocalBuffer[0] = 0;
    }

    //------------------------------------------------------------------------------
    /**
        (Re-)allocate external buffer and copy existing string contents there.
    */
    inline void
    DiString::Realloc(SizeT newSize)
    {
        DI_ASSERT(newSize > (this->mStrLen + 1));
        DI_ASSERT(newSize > this->mHeapBufferSize);

        // allocate a new buffer
        char* newBuffer = (char*) malloc (newSize);

        // copy existing contents there...
        if (this->mStrLen > 0)
        {
            const char* src = this->AsCharPtr();
            memcpy(newBuffer, src, this->mStrLen);
        }
        newBuffer[this->mStrLen] = 0;

        // assign new buffer
        if (this->mHeapBuffer)
        {
            free ((void*) this->mHeapBuffer);
            this->mHeapBuffer = 0;
        }
        this->mLocalBuffer[0] = 0;
        this->mHeapBuffer = newBuffer;
        this->mHeapBufferSize = newSize;
    }

    //------------------------------------------------------------------------------
    /**
        Reserves internal space to prevent excessive heap re-allocations.
        If you plan to do many Append() operations this may help alot.
    */
    inline void
    DiString::reserve(SizeT newSize)
    {
        if (newSize > LocalStringSize)
        {
            if (newSize > this->mHeapBufferSize)
            {
                this->Realloc(newSize);
            }
        }
    }


    inline void
    DiString::resize( SizeT newSize )
    {
        reserve(newSize);
        mStrLen = newSize;
    }

    inline void
    DiString::SetInt(int val)
    {
        this->Format("%d", val);
    }

    inline void 
    DiString::SetUint( int val )
    {
        this->Format("%u", val);
    }

    //------------------------------------------------------------------------------
    inline void
    DiString::SetInt64(int64 val)
    {
        this->Format("%I64d", val);
    }

    //------------------------------------------------------------------------------
    inline void
    DiString::SetFloat(float val)
    {
        this->Format("%g", val);
    }

    //------------------------------------------------------------------------------
    inline void
    DiString::SetBool(bool val)
    {
        if (val)
        {
            this->SetCharPtr("true");
        }
        else
        {
            this->SetCharPtr("false");
        }
    }

    inline void
    DiString::SetVector2(const DiVec2& v)
    {
        this->Format("%g %g",v.x,v.y);
    }

    inline void
    DiString::SetVector3(const DiVec3& v)
    {
        //this->Format("%g %g %g",v.x,v.y,v.z);
        this->Format("%g %g %g",v.x,v.y,v.z);
    }

    inline void
    DiString::SetVector4(const DiVec4& v)
    {
        this->Format("%g %g %g %g",v.x,v.y,v.z,v.w);
    }

    inline void
    DiString::SetQuaternion(const DiQuat& v)
    {
        this->Format("%g %g %g %g",v.w,v.x,v.y,v.z);
    }

    inline void
    DiString::SetMatrix3(const DiMat3& val)
    {
        this->Format("%g %g %g "
                     "%g %g %g "
                     "%g %g %g ",
            val[0][0],
            val[0][1],           
            val[0][2],            
            val[1][0],            
            val[1][1],            
            val[1][2],            
            val[2][0],            
            val[2][1],            
            val[2][2]);
    }

    inline void
    DiString::SetMatrix4(const DiMat4& val)
    {
        this->Format("%g %g %g %g "
                     "%g %g %g %g "
                     "%g %g %g %g "
                     "%g %g %g %g",
            val[0][0],
            val[0][1],
            val[0][2],
            val[0][3],
            val[1][0],
            val[1][1],
            val[1][2],
            val[1][3],
            val[2][0],
            val[2][1],
            val[2][2],
            val[2][3],
            val[3][0],
            val[3][1],
            val[3][2],
            val[3][3]);
    }

    inline void
    DiString::SetColourValue(const DiColor& v)
    {
        this->Format("%g %g %g %g",v.r,v.g,v.b,v.a);
    }

    inline void DiString::SetFloatRect(const DiFloatRect& v)
    {
        this->Format("%g %g %g %g", v.left, v.top, v.right, v.bottom);
    }
        
    //------------------------------------------------------------------------------
    inline
    DiString::DiString(const char* str) :
        mHeapBuffer(0),
        mStrLen(0),
        mHeapBufferSize(0)
    {
        this->mLocalBuffer[0] = 0;
        this->SetCharPtr(str);
    }
    //------------------------------------------------------------------------------
    inline
    DiString::DiString(const DiString& rhs) :
        mHeapBuffer(0),
        mStrLen(0),
        mHeapBufferSize(0)
    {
        this->mLocalBuffer[0] = 0;
        this->SetCharPtr(rhs.AsCharPtr());
    }
    //------------------------------------------------------------------------------
    inline
    DiString::DiString(const std::string& rhs) :
    mHeapBuffer(0),
    mStrLen(0),
    mHeapBufferSize(0)
    {
        this->mLocalBuffer[0] = 0;
        this->SetCharPtr(rhs.c_str());
    }
    //------------------------------------------------------------------------------
    inline
    DiString::DiString(const char* c_ptr, SizeT len)://added by Li
    mHeapBuffer(0),
    mStrLen(0),
    mHeapBufferSize(0)
    {
        this->mLocalBuffer[0] = 0;
        this->Set(c_ptr, len);
    }
    //------------------------------------------------------------------------------
    inline const char*
    DiString::AsCharPtr() const
    {
        if (this->mHeapBuffer)
        {
            return this->mHeapBuffer;
        }
        else
        {
            return this->mLocalBuffer;
        }
    }
    //------------------------------------------------------------------------------
    /**
    */
    inline void
    DiString::operator=(const DiString& rhs)
    {
        if (&rhs != this)
        {
            this->SetCharPtr(rhs.AsCharPtr());
        }
    }

    //------------------------------------------------------------------------------
    /**
    */
    inline void
    DiString::operator=(const char* cStr)
    {
        this->SetCharPtr(cStr);
    }

    //------------------------------------------------------------------------------
    /**
    */
    inline void
    DiString::Append(const DiString& str)
    {
        this->AppendRange(str.AsCharPtr(), str.mStrLen);
    }

    //------------------------------------------------------------------------------
    /**
    */
    inline void
    DiString::Append(const char* str)
    {
        DI_ASSERT(0 != str);
        this->AppendRange(str, (SizeT) strlen(str));
    }

    //------------------------------------------------------------------------------
    /**
    */
    inline void
    DiString::operator += (const DiString& rhs)
    {
        this->Append(rhs);    
    }

    //------------------------------------------------------------------------------
    /**
    */
    inline char
    DiString::operator[](IndexT i) const
    {
        DI_ASSERT(i <= this->mStrLen);
        return this->AsCharPtr()[i];
    }

    //------------------------------------------------------------------------------
    /**
        NOTE: unlike the read-only indexer, the terminating 0 is NOT a valid
        part of the string because it may not be overwritten!!!
    */
    inline char&
    DiString::operator[](IndexT i)
    {
        DI_ASSERT(i <= this->mStrLen);
        return (char&)(this->AsCharPtr())[i];
    }

    //------------------------------------------------------------------------------
    inline void
    DiString::clear()
    {
        this->Delete();
    }

    //------------------------------------------------------------------------------
    inline bool
    DiString::empty() const
    {
        return (0 == this->mStrLen);
    }

    //------------------------------------------------------------------------------
    inline bool
    DiString::IsValid() const
    {
        return (0 != this->mStrLen);
    }

    //------------------------------------------------------------------------------
    inline DiString
    DiString::SubString(IndexT offset/* = 0*/, SizeT n/* = InvalidIndex*/) const
    {
        DI_ASSERT( this->mStrLen >= offset && 0 <= offset && InvalidIndex <= n );

        const char* str = this->AsCharPtr();

        if (InvalidIndex == n)
            return DiString( &(str[offset]) );

        return DiString( &(str[offset]), n );
    }

    //------------------------------------------------------------------------------
    /**
        computes a hash code for the string by pseudorandomizing transform in std style. 
        (std::hash xhash hash_compare)  Added by Li
    */
    inline size_t
    DiString::ToHash() const
    {
        size_t _Val = 2166136261U;
        const char* c_ptr = this->AsCharPtr();
        const char* c_ptr_end = c_ptr + this->mStrLen;
        while(c_ptr != c_ptr_end)
            _Val = 16777619U * _Val ^ (size_t)*c_ptr++;

        ldiv_t _Qrem = (ldiv_t)(ldiv(_Val, 127773));
        _Qrem.rem = 16807 * _Qrem.rem - 2836 * _Qrem.quot;
        if (_Qrem.rem < 0)
            _Qrem.rem += 2147483647;

        return (size_t)_Qrem.rem;
    }

    //------------------------------------------------------------------------------
    /**
        Modified by Dinghao Li
    */
    static inline DiString
    operator+(const DiString& s0, const DiString& s1)
    {
        //String newString = s0;
        DiString newString(s0);
        //newString.Append(s1);
        newString.AppendRange(s1.AsCharPtr(), s1.length());
        return newString;
    }

    //------------------------------------------------------------------------------
    /**
        Added by Dinghao Li
    */
    static inline DiString
    operator+(const char* cStr, const DiString& str)
    {
        DiString newString(cStr);
        newString.AppendRange(str.AsCharPtr(), str.length());
        return newString;
    }

    //------------------------------------------------------------------------------
    /**
        Added by Dinghao Li
    */
    static inline DiString
    operator+(const DiString& str, const char* cStr)
    {
        DiString newString(str);
        newString.AppendRange(cStr, (SizeT)strlen(cStr));
        return newString;
    }

    //------------------------------------------------------------------------------
    /**
    */
    inline SizeT
        DiString::Tokenize(const DiString& whiteSpace, DiVector<DiString>& outTokens) const
    {
        return this->TokenizeEx(whiteSpace, outTokens);
    }

    //------------------------------------------------------------------------------
    /**
    */
    inline DiVector<DiString>
    DiString::Tokenize(const DiString& whiteSpace) const
    {
        return this->TokenizeEx(whiteSpace);
    }

    //------------------------------------------------------------------------------
    /**
        Return the index of a substring, or InvalidIndex if not found   Modified  by  Li Dinghao
    */
    inline IndexT
    DiString::FindStringIndex(const DiString& s, IndexT startIndex) const
    {
        return FindStringIndex(s.AsCharPtr(), startIndex);
    }

    //------------------------------------------------------------------------------
    /**
        Replace character with another.
    */
    inline void
    DiString::SubstituteChar(char c, char subst)
    {
        char* ptr = const_cast<char*>(this->AsCharPtr());
        IndexT i;
        for (i = 0; i <= this->mStrLen; ++i)
        {
            if (ptr[i] == c)
            {
                ptr[i] = subst;
            }
        }
    }

    //------------------------------------------------------------------------------
    /**
        Converts backslashes to slashes.
    */
    inline void
    DiString::ConvertBackslashes()
    {
        this->SubstituteChar('\\', '/');
    }

    //------------------------------------------------------------------------------
    /**
        Return a String object containing the part after the last
        path separator.
    */
    inline DiString
    DiString::ExtractFileName() const
    {
        DiString pathString;
        const char* lastSlash = this->GetLastSlash();
        if (lastSlash)
        {
            pathString = &(lastSlash[1]);
        }
        else
        {
            pathString = *this;
        }
        return pathString;
    }

    //------------------------------------------------------------------------------
    /**
        Return a path string object which contains of the complete path
        up to the last slash. Returns an empty string if there is no
        slash in the path.
    */
    inline DiString
    DiString::ExtractToLastSlash() const
    {
        DiString pathString(*this);
        char* lastSlash = pathString.GetLastSlash();
        if (lastSlash)
        {
            lastSlash[1] = 0;
        }
        else
        {
            pathString = "";
        }
        return pathString;
    }

    //------------------------------------------------------------------------------
    /**
        split filename to path and basename  Added by Li Dinghao
    */
    inline void
    DiString::SplitFilename(DiString& outBasename, DiString& outPath)
    {
        this->ReplaceChars("\\", '/');

        outPath = this->ExtractToLastSlash();
        outBasename = this->ExtractFileName();
    }

    //------------------------------------------------------------------------------
    /**
        Return true if the string only contains characters which are in the defined
        character set.
    */
    inline bool
    DiString::CheckValidCharSet(const DiString& charSet) const
    {
        IndexT i;
        for (i = 0; i < this->mStrLen; i++)
        {
            if (InvalidIndex == charSet.FindCharIndex((*this)[i], 0))
            {
                return false;
            }
        }
        return true;
    }

    //------------------------------------------------------------------------------
    /**
    */
    inline bool
    DiString::IsValidInt() const
    {
        return this->CheckValidCharSet(" \t-+01234567890");
    }

    //------------------------------------------------------------------------------
    /**
        Note: this method is not 100% correct, it just checks for invalid characters.
    */
    inline bool
    DiString::IsValidFloat() const
    {
        return this->CheckValidCharSet(" \t-+.e1234567890");
    }

    inline bool
    DiString:: IsValidVector2() const
    {
        return this->CheckValidCharSet(" \t-+.,e1234567890");
    }

    inline bool
    DiString:: IsValidVector3() const
    {
        return this->CheckValidCharSet(" \t-+.,e1234567890");
    }

    inline bool
    DiString:: IsValidVector4() const
    {
        return this->CheckValidCharSet(" \t-+.,e1234567890");
    }

    inline bool
    DiString:: IsValidQuaternion() const
    {
        return this->CheckValidCharSet(" \t-+.,e1234567890");
    }

    inline bool
    DiString:: IsValidMatrix3() const
    {
        return this->CheckValidCharSet(" \t-+.,e1234567890");
    }

    inline bool
    DiString:: IsValidMatrix4() const
    {
        return this->CheckValidCharSet(" \t-+.,e1234567890");
    }

    inline bool
    DiString:: IsValidColourValue() const
    {
        return this->CheckValidCharSet(" \t-+.,e1234567890");
    }


    //------------------------------------------------------------------------------
    /**
    */
    inline void
    DiString::ReplaceIllegalFilenameChars(char replacement)
    {
        this->ReplaceChars("\\/:*?\"<>|", replacement);
    }

    //------------------------------------------------------------------------------
    /**
    */
    inline DiString
    DiString::FromInt(int i)
    {    
        DiString str;
        str.SetInt(i);
        return str;
    }

    inline DiString 
    DiString::FromUint( int i )
    {
        DiString str;
        str.SetUint(i);
        return str;
    }

    //------------------------------------------------------------------------------
    /**
    */
    inline DiString
    DiString::FromFloat(float f)
    {
        DiString str;
        str.SetFloat(f);
        return str;
    }

    //------------------------------------------------------------------------------
    /**
    */
    inline DiString
    DiString::FromBool(bool b)
    {
        DiString str;
        str.SetBool(b);
        return str;
    }

    inline DiString
    DiString::FromVector2(const DiVec2& v)
    {
        DiString str;
        str.SetVector2(v);
        return str;
    }

    inline DiString
    DiString::FromVector3(const DiVec3& v)
    {
        DiString str;
        str.SetVector3(v);
        return str;
    }

    inline DiString
    DiString::FromVector4(const DiVec4& v)
    {
        DiString str;
        str.SetVector4(v);
        return str;
    }

    inline DiString
    DiString::FromQuaternion(const DiQuat& v)
    {
        DiString str;
        str.SetQuaternion(v);
        return str;
    }

    inline DiString
    DiString::FromMatrix3(const DiMat3& v)
    {
        DiString str;
        str.SetMatrix3(v);
        return str;
    }

    inline DiString
    DiString::FromMatrix4(const DiMat4& v)
    {
        DiString str;
        str.SetMatrix4(v);
        return str;
    }

    inline DiString
    DiString::FromColourValue(const DiColor& v)
    {
        DiString str;
        str.SetColourValue(v);
        return str;
    }

    inline
    DiString DiString::FromFloatRect(const DiFloatRect& v)
    {
        DiString ret;
        ret.SetFloatRect(v);
        return ret;
    }

    //#endif
        
    //------------------------------------------------------------------------------
    /**
    */
    inline void
    DiString::AppendInt(int val)
    {
        this->Append(FromInt(val));
    }

    //------------------------------------------------------------------------------
    /**
    */
    inline void
    DiString::AppendFloat(float val)
    {
        this->Append(FromFloat(val));
    }

    //------------------------------------------------------------------------------
    /**
    */
    inline void
    DiString::AppendBool(bool val)
    {
        this->Append(FromBool(val));
    }

    inline void
    DiString::AppendVector2(const DiVec2& v)
    {
        this->Append(FromVector2(v));
    }

    inline void
    DiString::AppendVector3(const DiVec3& v)
    {
        this->Append(FromVector3(v));
    }

    inline void
    DiString::AppendVector4(const DiVec4& v)
    {
        this->Append(FromVector4(v));
    }

    inline void
    DiString::AppendQuaternion(const DiQuat& v)
    {
        this->Append(FromQuaternion(v));
    }

    inline void
    DiString::AppendMatrix3(const DiMat3& v)
    {
        this->Append(FromMatrix3(v));
    }

    inline void
    DiString::AppendMatrix4(const DiMat4& v)
    {
        this->Append(FromMatrix4(v));
    }

    inline void
    DiString::AppendColourValue(const DiColor& v)
    {
        this->Append(FromColourValue(v));
    }

    inline void 
    DiString::AppendFloatRect(const DiFloatRect& v)
    {
        this->Append(FromFloatRect(v));
    }

    inline bool 
    operator==(const DiString& a, const DiString& b)
    {
        return strcmp(a.AsCharPtr(), b.AsCharPtr()) == 0;
    }

    inline bool 
    operator==(const DiString& a, const char* cStr)
    {
        DI_ASSERT(0 != cStr);
        return strcmp(a.AsCharPtr(), cStr) == 0;
    }

    inline bool 
    operator==(const char* cStr, const DiString& b)
    {
        DI_ASSERT(0 != cStr);
        return strcmp(cStr, b.AsCharPtr()) == 0;
    }

    inline bool 
    operator != (const DiString& a, const DiString& b)
    {
        return strcmp(a.AsCharPtr(), b.AsCharPtr()) != 0;
    }

    inline bool
    operator < (const DiString& a, const DiString& b)
    {
        return strcmp(a.AsCharPtr(), b.AsCharPtr()) < 0;
    }

    inline bool
    operator > (const DiString& a, const DiString& b)
    {
        return strcmp(a.AsCharPtr(), b.AsCharPtr()) > 0;
    }

    inline bool
    operator <= (const DiString& a, const DiString& b)
    {
        return strcmp(a.AsCharPtr(), b.AsCharPtr()) <= 0;
    }

    inline bool
    operator >= (const DiString& a, const DiString& b)
    {
        return strcmp(a.AsCharPtr(), b.AsCharPtr()) >= 0;
    }

    inline DiString::operator size_t()
    {
        return ToHash();
    }

    inline DiString::operator const size_t() const
    {
        return ToHash();
    }

    typedef DiVector<DiString> StringVec;
} 

#if DEMI_COMPILER == DEMI_COMPILER_MSVC
#   pragma warning( pop ) 
#endif

#endif // Str_h__