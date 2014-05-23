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
#include "MiscPch.h"
#include "str.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stack>

#if DEMI_COMPILER == DEMI_COMPILER_MSVC
#pragma warning(disable:4267)
#pragma warning(disable:4996)
#pragma warning(disable:4244)
#endif

namespace Demi
{
    const DiString::size_type DiString::npos = -1;    
    DiString DiString::BLANK = "";
    //------------------------------------------------------------------------------
    /**
    */
    bool
    DiString::CopyToBuffer(char* buf, SizeT bufSize) const
    {
        DI_ASSERT(0 != buf);
        DI_ASSERT(bufSize > 1);

        strncpy(buf, this->AsCharPtr(), bufSize - 1);
        buf[bufSize - 1] = 0;
        return this->mStrLen < bufSize;
    }

    //------------------------------------------------------------------------------
    /**
    */
    void __cdecl
    DiString::Format(const char* fmtString, ...)
    {
        va_list argList;
        va_start(argList, fmtString);
        static char buf[40960]; // an 4 kByte buffer
        // need to use non-CRT thread safe function under Win32
        SAFE_VSPRINTF(buf, sizeof(buf), fmtString, argList);

        *this = buf;
        va_end(argList);
    }

    //------------------------------------------------------------------------------
    /**
    */
    void __cdecl
    DiString::FormatArgList(const char* fmtString, va_list argList)
    {
        static char buf[40960]; // an 4 kByte buffer
        // need to use non-CRT thread safe function under Win32
        SAFE_VSPRINTF(buf, sizeof(buf), fmtString, argList);
        //*this = buf;
        this->Set(buf, strlen(buf));
    }

    //------------------------------------------------------------------------------
    /**
        Sets a new string content. This will handle all special cases and try
        to minimize heap allocations as much as possible.
    */
    void
    DiString::Set(const char* str, SizeT length)
    {
        if (0 == str)
        {
            // a null string pointer is valid as a special case
            this->Delete();
            this->mLocalBuffer[0] = 0;
        }
        else if ((0 == this->mHeapBuffer) && (length < LocalStringSize))
        {
            // the new contents fits into the local buffer,
            // however, if there is already an external buffer
            // allocated, we use the external buffer!
            this->Delete();
            memcpy(this->mLocalBuffer,str,length);
            this->mLocalBuffer[length] = 0;
        }
        else if (length < this->mHeapBufferSize)
        {
            // the new contents fits into the existing buffer
            memcpy(this->mHeapBuffer,str, length);
            this->mHeapBuffer[length] = 0;
            this->mLocalBuffer[0] = 0;
        }
        else
        {
            // need to allocate bigger heap buffer
            this->Alloc(length + 1);
            memcpy(this->mHeapBuffer,str,length);
            this->mHeapBuffer[length] = 0;
            this->mLocalBuffer[0] = 0;
        }
        this->mStrLen = length;
    }

    //------------------------------------------------------------------------------
    void
    DiString::AppendRange(const char* append, SizeT length)
    {
        DI_ASSERT(append);
        if (length > 0)
        {
            SizeT newLength = this->mStrLen + length;
            
            // if there is an external buffer allocated, we need to stay there
            // even if the resulting string is smaller then LOCALSTRINGSIZE,
            // a small string in an external buffer may be the result
            // of a call to Reserve()
            if ((0 == this->mHeapBuffer) && (newLength < LocalStringSize))
            {
                // the result fits into the local buffer
                memcpy(this->mLocalBuffer + this->mStrLen,append,length);
                this->mLocalBuffer[newLength] = 0;
            }
            else if (newLength < this->mHeapBufferSize)
            {
                // the result fits into the existing buffer
                memcpy(this->mHeapBuffer + this->mStrLen,append,length);
                this->mHeapBuffer[newLength] = 0;
            }
            else
            {
                // need to re-allocate
                this->Realloc(newLength + newLength / 2);
                memcpy(this->mHeapBuffer + this->mStrLen,append,length);
                this->mHeapBuffer[newLength] = 0;
            }
            this->mStrLen = newLength;
        }
    }

    //------------------------------------------------------------------------------
    /**
        Tokenize the string into a DiString array. 

        @param  whiteSpace      a string containing the whitespace characters
        @param  maxSplits       The maximum number of splits to perform (0 for unlimited splits). 
            If this parameters is > 0, the splitting process will stop after this many splits, left to right.
            Modified by Dinghao Li
        @return                 outTokens.Size()
    */
    SizeT
    DiString::TokenizeEx(const DiString& whiteSpace, DiVector<DiString>& outTokens, unsigned int maxSplits) const
    {
        DiString str(*this);
        char* ptr = const_cast<char*>(str.AsCharPtr());
        const char* token;
        unsigned int numSplits = 0;
        while ( 0 != (token = strtok(ptr, whiteSpace.AsCharPtr())) )
        {
            outTokens.push_back(token);
            if (maxSplits && numSplits == maxSplits)
                break;
            ptr = 0;
            ++numSplits;
        }
        return outTokens.size();
    }

    //------------------------------------------------------------------------------
    /**
        This is the slow-but-convenient Tokenize() method. Slow since the
        returned string array will be constructed anew with every method call.
        Consider the Tokenize() method which takes a string array as input,
        since this may allow reusing of the array, reducing heap allocations.
    */
    DiVector<DiString>
    DiString::TokenizeEx(const DiString& whiteSpace, unsigned int maxSplits) const
    {
        DiVector<DiString> tokens;
        this->TokenizeEx(whiteSpace, tokens, maxSplits);
        return tokens;
    }

    //------------------------------------------------------------------------------
    /**
        Tokenize a string, but keeps the string within the fence-character
        intact. For instance for the sentence:

        He said: "I don't know."

        A Tokenize(" ", '"', tokens) would return:

        token 0:    He
        token 1:    said:
        token 2:    I don't know.
    */
    SizeT
    DiString::Tokenize(const DiString& whiteSpace, char fence, DiVector<DiString>& outTokens) const
    {
        outTokens.clear();
        DiString str(*this);
        char* ptr = const_cast<char*>(str.AsCharPtr());    
        char* end = ptr + strlen(ptr);
        while (ptr < end)
        {
            char* c;

            // skip white space
            while (*ptr && strchr(whiteSpace.AsCharPtr(), *ptr))
            {
                ptr++;
            }
            if (*ptr)
            {
                // check for fenced area
                if ((fence == *ptr) && (0 != (c = strchr(++ptr, fence))))
                {
                    *c++ = 0;
                    outTokens.push_back(ptr);
                    ptr = c;
                }
                else if (0 != (c = strpbrk(ptr, whiteSpace.AsCharPtr())))
                {
                    *c++ = 0;
                    outTokens.push_back(ptr);
                    ptr = c;
                }
                else
                {
                    outTokens.push_back(ptr);
                    break;
                }
            }
        }
        return outTokens.size();
    }

    //------------------------------------------------------------------------------
    /**
        Slow version of Tokenize() with fence character. See above Tokenize()
        for details.
    */
    DiVector<DiString>
    DiString::Tokenize(const DiString& whiteSpace, char fence) const
    {
        DiVector<DiString> tokens;
        this->Tokenize(whiteSpace, fence, tokens);
        return tokens;
    }

    //------------------------------------------------------------------------------
    /**
        Extract a substring range.
    */
    DiString
    DiString::ExtractRange(IndexT fromIndex, SizeT numChars) const
    {
    //     DI_ASSERT(from <= this->strLen);
    //     DI_ASSERT((from + numChars) <= this->strLen);
    //     const char* str = this->AsCharPtr();
    //     DiString newString;
    //     newString.Set(&(str[from]), numChars);
    //     return newString;

        return this->SubString(fromIndex, numChars);
    }

    //------------------------------------------------------------------------------
    /**
        Extract a substring until the end of the original string.
    */
    DiString
    DiString::ExtractToEnd(IndexT fromIndex) const
    {
    //    return this->ExtractRange(fromIndex, this->strLen - fromIndex);

        return this->SubString(fromIndex/*, this->strLen - fromIndex*/);
    }

    //------------------------------------------------------------------------------
    /**
        Terminates the string at the first occurance of one of the characters
        in charSet.
    */
    void
    DiString::Strip(const DiString& charSet)
    {
        char* str = const_cast<char*>(this->AsCharPtr());
        char* ptr = strpbrk(str, charSet.AsCharPtr());
        if (ptr)
        {
            *ptr = 0;
        }
        this->mStrLen = (SizeT) strlen(this->AsCharPtr());
    }

    //------------------------------------------------------------------------------
    /**
        return start index of substring of c-style, or InvalidIndex if not found   Added  by  Li Dinghao
    */
    IndexT
    DiString::FindStringIndex(const char* s, IndexT startIndex) const
    {
        DI_ASSERT(startIndex < this->mStrLen);

        const char* this_ptr = this->AsCharPtr();
        int s_len = strlen(s);

        DI_ASSERT(s_len);

        for (int i = startIndex; i < this->mStrLen - s_len ; ++i)
        {
            for (int idx = 0; this_ptr[i+idx] == s[idx]; ++idx)
            {
                if (idx == s_len - 1)
                {
                    return i;
                }
            }
        }

        return InvalidIndex;
    }

    //------------------------------------------------------------------------------
    /**
        return index(first occurrence) of character in string, or InvalidIndex if not found.
    */
    IndexT
    DiString::FindCharIndex(char c, IndexT startIndex) const
    {
        if (this->mStrLen > 0)
        {
            DI_ASSERT(startIndex < this->mStrLen);
            const char* ptr = strchr(this->AsCharPtr() + startIndex, c);
            if (ptr)
            {
                return IndexT(ptr - this->AsCharPtr());
            }
        }
        return InvalidIndex;
    }

    //------------------------------------------------------------------------------
    /**
        return index of the first occurrence in the string of any character from 
        charSet searched for.  Added  by  Li Dinghao
    */
    IndexT
    DiString::FindCharIndexFromSet(const DiString& charSet) const
    {
        char* str = const_cast<char*>(this->AsCharPtr());
        char* ptr = strpbrk(str, charSet.AsCharPtr());
        if (ptr)
        {
            return IndexT(ptr - this->AsCharPtr());
        }
        return InvalidIndex;
    }

    //------------------------------------------------------------------------------
    /**
        return index(last occurrence) of character in string, or InvalidIndex if not
        found     Added  by  Li Dinghao
    */
    IndexT
    DiString::FindCharLastIndex(char c) const
    {
        if (this->mStrLen > 0)
        {
            const char* ptr = strrchr(this->AsCharPtr(), c);
            if (ptr)
            {
                return IndexT(ptr - this->AsCharPtr());
            }
        }
        return InvalidIndex;
    }

    //------------------------------------------------------------------------------
    /**
        Removes all characters in charSet from the left side of the string.
    */
    void
    DiString::TrimLeft(const DiString& charSet)
    {
        DI_ASSERT(charSet.IsValid());
        if (this->IsValid())
        {
            SizeT charSetLen = charSet.mStrLen;
            IndexT thisIndex = 0;
            bool stopped = false;
            while (!stopped && (thisIndex < this->mStrLen))
            {
                IndexT charSetIndex;
                bool match = false;
                for (charSetIndex = 0; charSetIndex < charSetLen; charSetIndex++)
                {
                    if ((*this)[thisIndex] == charSet[charSetIndex])
                    {
                        // a match
                        match = true;
                        break;
                    }
                }
                if (!match)
                {
                    // stop if no match
                    stopped = true;
                }
                else
                {
                    // a match, advance to next character
                    ++thisIndex;
                }
            }
            DiString trimmedString(&(this->AsCharPtr()[thisIndex]));
            *this = trimmedString;
        }
    }

    //------------------------------------------------------------------------------
    /**
        Removes all characters in charSet from the right side of the string.
    */
    void
    DiString::TrimRight(const DiString& charSet)
    {
        DI_ASSERT(charSet.IsValid());
        if (this->IsValid())
        {
            SizeT charSetLen = charSet.mStrLen;
            int thisIndex = this->mStrLen - 1;   // NOTE: may not be unsigned (thus not IndexT!)
            bool stopped = false;
            while (!stopped && (thisIndex >= 0))
            {
                IndexT charSetIndex;
                bool match = false;
                for (charSetIndex = 0; charSetIndex < charSetLen; charSetIndex++)
                {
                    if ((*this)[thisIndex] == charSet[charSetIndex])
                    {
                        // a match
                        match = true;
                        break;
                    }
                }
                if (!match)
                {
                    // stop if no match
                    stopped = true;
                }
                else
                {
                    // a match, advance to next character
                    --thisIndex;
                }
            }
            DiString trimmedString;
            trimmedString.Set(this->AsCharPtr(), thisIndex + 1);
            *this = trimmedString;
        }
    }

    //------------------------------------------------------------------------------
    /**
        Trim both sides of a string.
    */
    void
    DiString::Trim(const DiString& charSet)
    {
        this->TrimLeft(charSet);
        this->TrimRight(charSet);
    }

    //------------------------------------------------------------------------------
    /**
        Substitute every occurance of matchStr with substStr.
    */
    void
    DiString::SubstituteString(const char* matchStr, const char* substStr)
    {
        DI_ASSERT( matchStr && substStr);

        const char* ptr = this->AsCharPtr();
        SizeT matchStrLen = strlen(matchStr);
        SizeT substStrLen = strlen(substStr);
        DiString dest;

        // walk original string for occurances of str
        const char* occur;
        while (0 != (occur = strstr(ptr, matchStr )))
        {
            // append string fragment until match
            dest.AppendRange(ptr, SizeT(occur - ptr));

            // append replacement string
            if (substStrLen > 0)
                dest.Append(substStr);

            // adjust source pointer
            ptr = occur + matchStrLen;
        }
        dest.Append(ptr);
        *this = dest;
    }

    //------------------------------------------------------------------------------
    /**
        Substitute every occurance of matchStr with substStr.
    */
    void
    DiString::SubstituteString(const DiString& matchStr, const DiString& substStr)
    {
        DI_ASSERT(matchStr.IsValid());

        const char* ptr = this->AsCharPtr();
        SizeT matchStrLen = matchStr.mStrLen;
        DiString dest;

        // walk original string for occurances of str
        const char* occur;
        while (0 != (occur = strstr(ptr, matchStr.AsCharPtr())))
        {
            // append string fragment until match
            dest.AppendRange(ptr, SizeT(occur - ptr));

            // append replacement string
            if (substStr.length() > 0)
            {
                dest.Append(substStr);
            }

            // adjust source pointer
            ptr = occur + matchStrLen;
        }
        dest.Append(ptr);
        *this = dest;
    }

    //------------------------------------------------------------------------------
    /**
        Return a DiString object containing the last directory of the path, i.e.
        a category.

        - 17-Feb-04     floh    fixed a bug when the path ended with a slash
    */
    DiString
    DiString::ExtractLastDirName() const
    {
        DiString pathString(*this);
        char* lastSlash = pathString.GetLastSlash();
        
        // special case if path ends with a slash
        if (lastSlash)
        {
            if (0 == lastSlash[1])
            {
                *lastSlash = 0;
                lastSlash = pathString.GetLastSlash();
            }

            char* secLastSlash = 0;
            if (0 != lastSlash)
            {
                *lastSlash = 0; // cut filename
                secLastSlash = pathString.GetLastSlash();
                if (secLastSlash)
                {
                    *secLastSlash = 0;
                    return DiString(secLastSlash+1);
                }
            }
        }
        return "";
    }

    //------------------------------------------------------------------------------
    /**
        Return a DiString object containing the part before the last
        directory separator.
        
        NOTE: I left my fix in that returns the last slash (or colon), this was 
        necessary to tell if a dirname is a normal directory or an assign.     

        - 17-Feb-04     floh    fixed a bug when the path ended with a slash
    */
    DiString
    DiString::ExtractDirName() const
    {
        DiString pathString(*this);
        char* lastSlash = pathString.GetLastSlash();

        // special case if path ends with a slash
        if (lastSlash)
        {
            if (0 == lastSlash[1])
            {
                *lastSlash = 0;
                lastSlash = pathString.GetLastSlash();
            }
            if (lastSlash)
            {
                *++lastSlash = 0;
            }
        }
        pathString.mStrLen = (SizeT) strlen(pathString.AsCharPtr());
        return pathString;
    }

    //------------------------------------------------------------------------------
    /**
        Pattern-matching, TCL-style.
    */
    bool
    DiString::MatchPattern(const DiString& string, const DiString& pattern)
    {
        const char* str = string.AsCharPtr();
        const char* pat = pattern.AsCharPtr();

        char c2;
        bool always = true;
        while (always != false)
        {
            if (*pat == 0) 
            {
                if (*str == 0) return true;
                else           return false;
            }
            if ((*str == 0) && (*pat != '*')) return false;
            if (*pat=='*') 
            {
                pat++;
                if (*pat==0) return true;
                while (always)
                {
                    if (DiString::MatchPattern(str, pat)) return true;
                    if (*str==0) return false;
                    str++;
                }
            }
            if (*pat=='?') goto match;
            if (*pat=='[') 
            {
                pat++;
                while (always) 
                {
                    if ((*pat==']') || (*pat==0)) return false;
                    if (*pat==*str) break;
                    if (pat[1] == '-') 
                    {
                        c2 = pat[2];
                        if (c2==0) return false;
                        if ((*pat<=*str) && (c2>=*str)) break;
                        if ((*pat>=*str) && (c2<=*str)) break;
                        pat+=2;
                    }
                    pat++;
                }
                while (*pat!=']') 
                {
                    if (*pat==0) 
                    {
                        pat--;
                        break;
                    }
                    pat++;
                }
                goto match;
            }
        
            if (*pat=='\\') 
            {
                pat++;
                if (*pat==0) return false;
            }
            if (*pat!=*str) return false;

    match:
            pat++;
            str++;
        }
        // can't happen
        return false;
    }

    //------------------------------------------------------------------------------
    /**
        pattern matching extended   Added by Dinghao Li
    */
    bool
    DiString::MatchPatternEx(const DiString& str, const DiString& pattern, bool caseSensitive)
    {
        DiString tmpStr(str);
        DiString tmpPattern(pattern);

        if (!caseSensitive)
        {
            tmpStr.ToLower();
            tmpPattern.ToLower();
        }

        return MatchPattern(tmpStr, tmpPattern);
    }

    //------------------------------------------------------------------------------
    bool
    DiString::StartsWith(const DiString& str, const DiString& pattern, bool caseSensitive)
    {
        DiString tmpStr(str);
        DiString tmpPattern(pattern);

        if (!caseSensitive)
        {
            tmpStr.ToLower();
            tmpPattern.ToLower();
        }

        uint32 thisLen = tmpStr.length();
        uint32 patternLen = tmpPattern.length();

        if (thisLen < patternLen || 0 == patternLen )
            return false;

        return ( tmpStr.ExtractRange(0, patternLen) == tmpPattern );
    }
    //------------------------------------------------------------------------------
    bool
    DiString::EndsWith(const DiString& str, const DiString& pattern, bool caseSensitive)
    {
        DiString tmpStr(str);
        DiString tmpPattern(pattern);

        if (!caseSensitive)
        {
            tmpStr.ToLower();
            tmpPattern.ToLower();
        }

        uint32 thisLen = tmpStr.length();
        uint32 patternLen = tmpPattern.length();

        if (thisLen < patternLen || 0 == patternLen )
            return false;

        return ( tmpStr.ExtractToEnd(thisLen - patternLen) == tmpPattern );

    }

    //------------------------------------------------------------------------------
    /**
    */
    void
    DiString::ReplaceChars(const DiString& charSet, char replacement)
    {
        DI_ASSERT(charSet.IsValid());
        char* ptr = const_cast<char*>(this->AsCharPtr());
        char c;
        while (0 != (c = *ptr))
        {
            if (strchr(charSet.AsCharPtr(), c))
            {
                *ptr = replacement;
            }
            ptr++;
        }
    }

    DiVec3 
    DiString::AsVector3() const
    {
        DiVector<DiString> tokens; 
        this->Tokenize(", \t\n", tokens);

        if (tokens.size() >= 3)
        {
            DiVec3 v(tokens[0].AsFloat(), tokens[1].AsFloat(), tokens[2].AsFloat());
            return v;
        }
        else if (tokens.size() == 2)
        {
            DiVec3 v(tokens[0].AsFloat(), tokens[1].AsFloat(), 0);
            return v;
        }
        else if (tokens.size() == 1)
        {
            DiVec3 v(tokens[0].AsFloat(), tokens[0].AsFloat(), tokens[0].AsFloat());
            return v;
        }
        else
        {
            return DiVec3::ZERO;
        }
    }

    DiVec2 
    DiString::AsVector2() const
    {
        DiVector<DiString> tokens; 
        this->Tokenize(", \t\n", tokens);

        if (tokens.size() >= 2)
        {
            DiVec2 v(tokens[0].AsFloat(), tokens[1].AsFloat());
            return v;
        }
        else if (tokens.size() == 1)
        {
            DiVec2 v(tokens[0].AsFloat(), tokens[0].AsFloat());
            return v;
        }
        else
        {
            return DiVec2::ZERO;
        }
    }

    DiVec4 
    DiString::AsVector4() const
    {
        DiVector<DiString> tokens; 
        this->Tokenize(", \t\n", tokens);

        if (tokens.size() >= 4)
        {
            DiVec4 v(tokens[0].AsFloat(), tokens[1].AsFloat(), tokens[2].AsFloat(), tokens[3].AsFloat());
            return v;
        }
        else if (tokens.size() == 3)
        {
            DiVec4 v(tokens[0].AsFloat(), tokens[1].AsFloat(), tokens[2].AsFloat(), 0);
            return v;
        }
        else if (tokens.size() == 2)
        {
            DiVec4 v(tokens[0].AsFloat(), tokens[1].AsFloat(), 0, 0);
            return v;
        }
        else if (tokens.size() == 1)
        {
            DiVec4 v(tokens[0].AsFloat(), tokens[0].AsFloat(), tokens[0].AsFloat(), tokens[0].AsFloat());
            return v;
        }
        else
        {
            return DiVec4::ZERO;
        }
    }

    DiQuat 
    DiString::AsQuaternion(bool wFirst) const
    {
        DiVector<DiString> tokens; 
        this->Tokenize(", \t\n", tokens);

        DI_ASSERT (tokens.size() == 4);

        DiQuat v;
        if (wFirst)
        {
            v = DiQuat(tokens[0].AsFloat(),  tokens[1].AsFloat(),tokens[2].AsFloat(),  tokens[3].AsFloat());
        }
        else
        {
            v = DiQuat(tokens[3].AsFloat(),  tokens[1].AsFloat(),tokens[2].AsFloat(),  tokens[0].AsFloat());
        }

        return v;
    }

    DiMat3 
    DiString::AsMatrix3() const
    {
        DiVector<DiString> tokens; 
        this->Tokenize(", \t\n", tokens);

        DI_ASSERT (tokens.size() == 9);

        DiMat3 v(tokens[0].AsFloat(),  tokens[1].AsFloat(),  tokens[2].AsFloat(),  
                        tokens[3].AsFloat(),  tokens[4].AsFloat(),  tokens[5].AsFloat(),
                        tokens[6].AsFloat(),  tokens[7].AsFloat(),  tokens[8].AsFloat());
        return v;
    }

    DiMat4 
    DiString::AsMatrix4() const
    {
        DiVector<DiString> tokens; 
        this->Tokenize(", \t\n", tokens);

        DI_ASSERT (tokens.size() == 16);

        DiMat4 v(tokens[0].AsFloat(),  tokens[1].AsFloat(),  tokens[2].AsFloat(),  tokens[3].AsFloat(),
                        tokens[4].AsFloat(),  tokens[5].AsFloat(),  tokens[6].AsFloat(),  tokens[7].AsFloat(),
                        tokens[8].AsFloat(),  tokens[9].AsFloat(),  tokens[10].AsFloat(),  tokens[11].AsFloat(),
                        tokens[12].AsFloat(),  tokens[13].AsFloat(),  tokens[14].AsFloat(),  tokens[15].AsFloat());
        return v;
    }

    DiColor 
    DiString::AsColourValue() const
    {
        DiVector<DiString> tokens; 
        this->Tokenize(", \t\n", tokens);

        if (tokens.size() == 1)
        {
            DiColor v(tokens[0].AsFloat(), tokens[0].AsFloat(), tokens[0].AsFloat());
            return v;
        }
        else if (tokens.size() == 3)
        {
            DiColor v(tokens[0].AsFloat(), tokens[1].AsFloat(), tokens[2].AsFloat());
            return v;
        }
        else
        {
            DI_ASSERT(tokens.size() == 4);
            DiColor v(tokens[0].AsFloat(), tokens[1].AsFloat(), tokens[2].AsFloat(), tokens[3].AsFloat());
            return v;
        }
    } 
    //------------------------------------------------------------------------------
    DiString
    DiString::Concatenate(const DiVector<DiString>& strArray, const DiString& whiteSpace)
    {
        DiString res;
        res.reserve(256);
        size_t i;
        size_t num = strArray.size();
        for (i = 0; i < num; i++)
        {
            res.Append(strArray[i]);
            if ((i + 1) < num)
            {
                res.Append(whiteSpace);
            }
        }
        return res;
    }

    //------------------------------------------------------------------------------
    void
    DiString::SetCharPtr(const char* s)
    {
        SizeT len = 0;
        if (s)
        {
            len = (SizeT) strlen((const char*)s);
        }
        this->Set(s, len);
    }

    //------------------------------------------------------------------------------
    /**
        Terminates the string at the given index.
    */
    void
    DiString::TerminateAtIndex(IndexT index)
    {
        DI_ASSERT(index < this->mStrLen);
        char* ptr = const_cast<char*>(this->AsCharPtr());
        ptr[index] = 0;
        this->mStrLen = (SizeT) strlen(ptr);
    }

    //------------------------------------------------------------------------------
    void
    DiString::StripFileExtension()
    {
        int pointpos = rfind(".");
        if (pointpos == npos)
        {
            return;
        }

        // ����һ��б��֮������û��.����û���ļ���չ
        DiString temp = *this;
        temp.Replace("/","\\");
        int pos = temp.rfind("\\");
        if (pos != npos && pointpos < pos)
        {
            return;
        }

        *this = substr(0,pointpos);
    }
    //------------------------------------------------------------------------------
    void
    DiString::ToLower()
    {
        char* str = const_cast<char*>(this->AsCharPtr());
        char c;
        while (0 != (c = *str))
        {
            *str++ = (char) tolower(c);
        }
    }

    //------------------------------------------------------------------------------
    void
    DiString::ToUpper()
    {
        char* str = const_cast<char*>(this->AsCharPtr());
        char c;
        while (0 != (c = *str))
        {
            *str++ = (char) toupper(c);
        }
    }

    //------------------------------------------------------------------------------
    void
    DiString::FirstCharToUpper()
    {
        char* str = const_cast<char*>(this->AsCharPtr());    
        *str = (char) toupper(*str);
    }

    //------------------------------------------------------------------------------
    /**
        Returns true if string contains one of the characters from charset.
    */
    bool
    DiString::ContainsCharFromSet(const DiString& charSet) const
    {
        char* str = const_cast<char*>(this->AsCharPtr());
        char* ptr = strpbrk(str, charSet.AsCharPtr());
        return (0 != ptr);
    }

    //------------------------------------------------------------------------------
    /**
        @return     string representing the filename extension (maybe empty)
    */
    DiString
    DiString::GetFileExtension() const
    {
        const char* str = this->AsCharPtr();
        const char* ext = strrchr(str, '.');
        if (ext)
        {
            ext++;
            return DiString(ext);
        }
        return DiString("");
    }

    //------------------------------------------------------------------------------
    /**
        Get a pointer to the last directory separator.
    */
    char*
    DiString::GetLastSlash() const
    {
        const char* s = this->AsCharPtr();
        const char* lastSlash = strrchr(s, '/');
        if (0 == lastSlash) lastSlash = strrchr(s, '\\');
        if (0 == lastSlash) lastSlash = strrchr(s, ':');
        return const_cast<char*>(lastSlash);
    }

    //------------------------------------------------------------------------------
    /**
    */
    bool
    DiString::IsValidBool() const
    {
        static const char* bools[] = {
            "no", "yes", "off", "on", "false", "true", 0
        };
        IndexT i = 0;
        while (bools[i] != 0)
        {
            if (0 == stricmp(bools[i], this->AsCharPtr()))
            {
                return true;
            }
            i++;
        }
        return false;
    }

    //------------------------------------------------------------------------------
    /**
        Returns content as integer. Note: this method doesn't check whether the
        contents is actually a valid integer. Use the IsValidInteger() method
        for this!
    */
    int
    DiString::AsInt() const
    {
        return atoi(this->AsCharPtr());
    }

    //------------------------------------------------------------------------------
    /**
        Returns content as unsigned long integer.      Added by Li Dinghao
    */
    unsigned long
    DiString::AsUint32() const
    {
        return strtoul(this->AsCharPtr(), NULL, 10);
    }

    //------------------------------------------------------------------------------
    /**
        Returns content as float. 
    */
    float
    DiString::AsFloat() const
    {
        return float(atof(this->AsCharPtr()));
    }

    //------------------------------------------------------------------------------
    /**
    */
    bool
    DiString::AsBool() const
    {
        static const char* bools[] = {
            "no", "yes", "off", "on", "false", "true", "0","1",0
        };
        IndexT i = 0;
        while (bools[i] != 0)
        {
            if (0 == stricmp(bools[i], this->AsCharPtr()))
            {
                return 1 == (i & 1);
            }
            i++;
        }
        DI_ASSERT_FAIL;
        return false;
    }

    //------------------------------------------------------------------------------
    /**
    */
    bool
    DiString::IsDigit(char c)
    {
        return (0 != isdigit(int(c)));
    }

    //------------------------------------------------------------------------------
    /**
    */
    bool
    DiString::IsAlpha(char c)
    {
        return (0 != isalpha(int(c)));
    }

    //------------------------------------------------------------------------------
    /**
    */
    bool
    DiString::IsAlNum(char c)
    {
        return (0 != isalnum(int(c)));
    }

    //------------------------------------------------------------------------------
    /**
    */
    bool
    DiString::IsLower(char c)
    {
        return (0 != islower(int(c)));
    }

    //------------------------------------------------------------------------------
    /**
    */
    bool
    DiString::IsUpper(char c)
    {
        return (0 != isupper(int(c)));
    }

    //------------------------------------------------------------------------------
    /**
    */
    int
    DiString::StrCmp(const char* str0, const char* str1)
    {
        DI_ASSERT(str0 && str1);
        return strcmp(str0, str1);
    }

    //------------------------------------------------------------------------------
    int
    DiString::StrLen(const char* str)
    {
        DI_ASSERT(str);
        return strlen(str);
    }

    //------------------------------------------------------------------------------
    const char*
    DiString::StrChr(const char* str, int c)
    {
        DI_ASSERT(str);
        return strchr(str, c);
    }

    //------------------------------------------------------------------------------
    const char*
    DiString::StrChrEx(const char *first, SizeT n, const char& ch)
    {    // look for _Ch in [_First, _First + _Count)
        DI_ASSERT( first );
        for (; 0 < n; --n, ++first)
            if ( *first == ch )
                return (first);
        return (0);
    }

    //------------------------------------------------------------------------------
    void 
    DiString::ChangeFileExtension(const DiString& newExt)
    {
        this->StripFileExtension();
        this->Append("." + newExt);
    }

    //------------------------------------------------------------------------------
    DiString::size_type
    DiString::rfind(const char* ptr, size_type offset, size_type n) const
    {
        DI_ASSERT( ptr && 0<=n );

        if (n == 0)
            return (offset < this->mStrLen ? offset : this->mStrLen);    // null always matches

        if (n <= this->mStrLen)
        {    // room for match, look for it
            const char* u_ptr = this->AsCharPtr() + (offset < this->mStrLen - n ? this->mStrLen : this->mStrLen - n);
            for (; ; --u_ptr)
                if ( (*u_ptr == *ptr) && memcmp(u_ptr, ptr, n) == 0 )
                    return (u_ptr - this->AsCharPtr());    // found a match
                else if (u_ptr == this->AsCharPtr())
                    break;    // at beginning, no more chance for match
        }
        return (npos);    // no match
    }

    //------------------------------------------------------------------------------
    DiString::size_type
    DiString::find( const char* ptr, size_type offset, size_type n ) const
    {
        DI_ASSERT( ptr && 0<=n );

        if ( n == 0 && offset <= this->mStrLen )
            return (offset);    // null string always matches (if inside string)

        size_type num;
        if (offset < this->mStrLen  && n <= (num = this->mStrLen - offset))
        {    // room for match, look for it
            const char *u_ptr, *v_ptr;
            for (num -= n - 1, v_ptr = this->AsCharPtr() + offset;
                (u_ptr = StrChrEx(v_ptr, num, *ptr)) != 0;
                num -= u_ptr - v_ptr + 1, v_ptr = u_ptr + 1)
                if (memcmp(u_ptr, ptr, n)== 0)
                    return (u_ptr - this->AsCharPtr());    // found a match
        }

        return (npos);    // no match
    }

    void DiString::Replace( char replaceThis, char replaceWith )
    {
        char* ptr = const_cast<char*>(this->AsCharPtr());
        for (SizeT i = 0; i < mStrLen; ++i)
        {
            if (ptr[i] == replaceThis)
            {
                ptr[i] = replaceWith;
            }
        }
    }

    void DiString::SimplifyPath()
    {
        if (size() < 2)
            return;

        Replace('\\', '/');

        int path_len, vec_len;

        DiString temp;
        DiString out_str;
        if ((*this)[1] == ':')
            out_str.Set(&(*this)[0], 1);
        else
            out_str = "/";

        DiVector <DiString> path_vec;
        int i, is = 1;
        if ((*this) == "/../") 
            SetCharPtr("/");

        append("/");
        path_len = size();
        for (i = 1; i <= path_len - 1; i++)
        {
            if ((*this)[i] == '/') {
                temp = substr(is, i - is);
                if (temp != "" && temp != "." && temp != "..")
                    path_vec.push_back(temp);
                if (temp == ".." && path_vec.size() != 0) path_vec.pop_back();
                is = i + 1;
            }
        }

        vec_len = path_vec.size();
        for (i = 0; i <= vec_len - 1; i++)
        {
            out_str += path_vec[i];
            if (i != vec_len - 1) out_str += "/";
        }

        *this = out_str;
    }

    void DiString::Replace( const char* replaceThis, const char* replaceWith )
    {
        std::string result = AsCharPtr();
        std::string::size_type pos = 0;
        while(true)
        {
            pos = result.find(replaceThis,pos);
            if (pos == std::string::npos)
            {
                break;
            }
            result.replace(pos,strlen(replaceThis),replaceWith);
            pos += strlen(replaceWith);
        }
        *this = result.c_str();
    }

    DiString DiString::ExtractBaseName() const
    {
        DiString basename;
        int i = rfind(".");
        if (i == npos)
        {
            basename = *this;
        }
        else
        {
            basename = substr(0, i);
        }
        return basename;
    }

    int DiString::CompareNoCase( const char* o ) const
    {
        return stricmp( c_str(),o );
    }

    DiString DiString::AsLower()
    {
        DiString ret = *this;
        ret.ToLower();
        return ret;
    }

    DiString DiString::AsUpper()
    {
        DiString ret = *this;
        ret.ToUpper();
        return ret;
    }

    bool DiString::CheckFileExtension(const DiString& ext) const
    {
        DiString l = GetFileExtension();
        return stricmp(l.c_str(), ext.c_str()) == 0;
    }
}