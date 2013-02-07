/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "_Marshal.h"
#include "String.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace clix;

Flood::StringHash::StringHash(::StringHash* native)
{
    // TODO: Struct marshaling
}

Flood::StringHash::StringHash(System::IntPtr native)
{
    // TODO: Struct marshaling
}

Flood::StringHash::StringHash(System::String^ str, unsigned int size)
{
}

Flood::StringHash Flood::FloodString::HashString(System::String^ _0)
{
    auto arg0 = marshalString<E_UTF8>(_0);
    auto ret = ::HashString(arg0);
    return Flood::StringHash((::StringHash*)&ret);
}

int Flood::FloodString::StringCompareInsensitive(System::String^ s1, System::String^ s2)
{
    auto arg0 = marshalString<E_UTF8>(s1);
    auto arg1 = marshalString<E_UTF8>(s2);
    auto ret = ::StringCompareInsensitive(arg0, arg1);
    return ret;
}

System::String^ Flood::FloodString::StringFormat(System::String^ s)
{
    auto _arg0 = clix::marshalString<clix::E_UTF8>(s);
    auto arg0 = _arg0.c_str();
    auto ret = ::StringFormat(arg0);
    return marshalString<E_UTF8>(ret);
}

System::String^ Flood::FloodString::StringFromWideString(System::String^ ws)
{
    auto arg0 = marshalString<E_UTF16>(ws);
    auto ret = ::StringFromWideString(arg0);
    return marshalString<E_UTF8>(ret);
}

System::String^ Flood::FloodString::StringToWideString(System::String^ s)
{
    auto arg0 = marshalString<E_UTF8>(s);
    auto ret = ::StringToWideString(arg0);
    return marshalString<E_UTF16>(ret);
}

System::String^ Flood::FloodString::StringToLowerCase(System::String^ s)
{
    auto arg0 = marshalString<E_UTF8>(s);
    auto ret = ::StringToLowerCase(arg0);
    return marshalString<E_UTF8>(ret);
}

System::String^ Flood::FloodString::StringToUpperCase(System::String^ s)
{
    auto arg0 = marshalString<E_UTF8>(s);
    auto ret = ::StringToUpperCase(arg0);
    return marshalString<E_UTF8>(ret);
}

void Flood::FloodString::StringReplace(System::String^ s, System::String^ from, System::String^ to)
{
    auto arg0 = marshalString<E_UTF8>(s);
    auto arg1 = marshalString<E_UTF8>(from);
    auto arg2 = marshalString<E_UTF8>(to);
    ::StringReplace(arg0, arg1, arg2);
}

System::String^ Flood::FloodString::StringTrim(System::String^ s, System::String^ trim)
{
    auto arg0 = marshalString<E_UTF8>(s);
    auto _arg1 = clix::marshalString<clix::E_UTF8>(trim);
    auto arg1 = _arg1.c_str();
    auto ret = ::StringTrim(arg0, arg1);
    return marshalString<E_UTF8>(ret);
}

System::String^ Flood::FloodString::StringFromFloat(float n, unsigned char precision)
{
    auto arg1 = (uint8)precision;
    auto ret = ::StringFromFloat(n, arg1);
    return marshalString<E_UTF8>(ret);
}

System::String^ Flood::FloodString::StringMatch(System::String^ s, System::String^ pattern)
{
    auto arg0 = marshalString<E_UTF8>(s);
    auto arg1 = marshalString<E_UTF8>(pattern);
    auto ret = ::StringMatch(arg0, arg1);
    return clix::marshalString<clix::E_UTF8>(ret);
}

System::String^ Flood::FloodString::RawStringMatch(System::String^ s, unsigned int len, System::String^ p)
{
    auto _arg0 = clix::marshalString<clix::E_UTF8>(s);
    auto arg0 = _arg0.c_str();
    auto arg1 = (size_t)len;
    auto _arg2 = clix::marshalString<clix::E_UTF8>(p);
    auto arg2 = _arg2.c_str();
    auto ret = ::RawStringMatch(arg0, arg1, arg2);
    return clix::marshalString<clix::E_UTF8>(ret);
}

System::String^ Flood::FloodString::PathGetFileBase(System::String^ path)
{
    auto arg0 = marshalString<E_UTF8>(path);
    auto ret = ::PathGetFileBase(arg0);
    return marshalString<E_UTF8>(ret);
}

System::String^ Flood::FloodString::PathGetFileExtension(System::String^ path)
{
    auto arg0 = marshalString<E_UTF8>(path);
    auto ret = ::PathGetFileExtension(arg0);
    return marshalString<E_UTF8>(ret);
}

System::String^ Flood::FloodString::PathGetBase(System::String^ path)
{
    auto arg0 = marshalString<E_UTF8>(path);
    auto ret = ::PathGetBase(arg0);
    return marshalString<E_UTF8>(ret);
}

System::String^ Flood::FloodString::PathGetFile(System::String^ path)
{
    auto arg0 = marshalString<E_UTF8>(path);
    auto ret = ::PathGetFile(arg0);
    return marshalString<E_UTF8>(ret);
}

System::String^ Flood::FloodString::PathNormalize(System::String^ path)
{
    auto arg0 = marshalString<E_UTF8>(path);
    auto ret = ::PathNormalize(arg0);
    return marshalString<E_UTF8>(ret);
}

System::String^ Flood::FloodString::PathGetCurrentDir()
{
    auto ret = ::PathGetCurrentDir();
    return marshalString<E_UTF8>(ret);
}

System::String^ Flood::FloodString::PathGetSeparator()
{
    auto ret = ::PathGetSeparator();
    return marshalString<E_UTF8>(ret);
}

System::String^ Flood::FloodString::PathCombine(System::String^ base, System::String^ extra)
{
    auto arg0 = marshalString<E_UTF8>(base);
    auto arg1 = marshalString<E_UTF8>(extra);
    auto ret = ::PathCombine(arg0, arg1);
    return marshalString<E_UTF8>(ret);
}

