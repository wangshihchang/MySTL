#ifndef _MYSTL_ASTRING_H_
#define _MYSTL_ASTRING_H_

// 定义了 string, wstring, u16string, u32string 类型

#include "basic_string.h"

namespace MySTL {

using string = MySTL::basic_string<char>;
using wstring = MySTL::basic_string<wchar_t>;
using u16string = MySTL::basic_string<char16_t>;
using u32string = MySTL::basic_string<char32_t>;

}  // namespace MySTL
#endif
