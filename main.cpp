#include <iostream>
#include <locale> // 2017-11-27
#include <codecvt>
typedef unsigned short unichar;

using namespace std;

wstring UTF8_To_UTF16(const string &source)
{
    try
    {
        static std::wstring_convert<std::codecvt_utf8_utf16<wchar_t> > cvt;
        return cvt.from_bytes(source);
    }
    catch (std::range_error &)
    {
        return std::wstring();
    }
}

string UTF16_To_UTF8(const wstring &source)
{
    try
    {
        static std::wstring_convert<std::codecvt_utf8_utf16<wchar_t> > cvt;
        return cvt.to_bytes(source);
    }
    catch (std::range_error &)
    {
        return std::string();
    }
}

bool isContainsEmoji(const string & strText)
{
	bool containsEmoji = false;
    
    size_t bytes = 0;
    for (std::string::const_iterator it = strText.begin(); it != strText.end(); it++)
    {
        unsigned char ch = *it;
        
		// 判断当前字节开始的字符是否 Unicode 字符(可能占 2、3、4 bytes), ANSII 字符占 1 byte
        if ((ch >> 7 & 0x01) == 0x00) {
            bytes = 1;
        } else if ((ch >> 4 & 0x0f) ==  0x0f) {
            bytes = 4;
        } else if ((ch >> 5 & 0x07) == 0x07) {
            bytes = 3;
        } else if ((ch >> 6 & 0x03) == 0x03) {
            bytes = 2;
        }
        
        if (bytes >= 2) // 是 Unicode 字符
		{
            std::string emoj(it, it+bytes); // 取多字节字符串, 可能是 emoji 表情符号
            std::wstring substring = UTF8_To_UTF16(emoj);
            size_t subsLen = substring.length();
            
            const unichar hs = substring[0]; // 高16位
            // surrogate pair
            if (0xd800 <= hs && hs <= 0xdbff)
            {
                if (subsLen > 1)
                {
                    const unichar ls = substring[1]; // 低16位
                    const int uc = ((hs - 0xd800) * 0x400) + (ls - 0xdc00) + 0x10000;
                    if (0x1d000 <= uc && uc <= 0x1f9c0)
                    {
                        containsEmoji = true;
                    }
                }
            }
            else if (subsLen > 1)
            {
                const unichar ls = substring[1];
                if (ls == 0x20e3 ||
                    ls == 0xfe0f ||
                    ls == 0xd83c)
                {
                    containsEmoji = true;
                }
            }
            else
            {
                // non surrogate
                if ((0x2100 <= hs && hs <= 0x27ff) ||
                    (0x2B05 <= hs && hs <= 0x2b07) ||
                    (0x2934 <= hs && hs <= 0x2935) ||
                    (0x3297 <= hs && hs <= 0x3299))
                {
                    containsEmoji = true;
                    break;
                }
                else if (hs == 0xa9 ||
                         hs == 0xae ||
                         hs == 0x303d ||
                         hs == 0x3030 ||
                         hs == 0x2b55 ||
                         hs == 0x2b1c ||
                         hs == 0x2b1b ||
                         hs == 0x2b50)
                {
                    containsEmoji = true;
                    break;
                }
            }

            it += bytes - 1; // 指向下一个字符
        }
    }
    
    
    return containsEmoji;
}

int main(int argc, char ** argv)
{
	string emoj("hello尼莫😀12你好");
	bool hasEmoji = isContainsEmoji(emoj);

    printf("check string:%s, containsEmoji = %s\n", emoj.c_str(), hasEmoji == true ? "true" : "false");

    string normal("尼莫40World你好");
    hasEmoji = isContainsEmoji(normal);

    printf("check string:%s, containsEmoji = %s\n", normal.c_str(), hasEmoji == true ? "true" : "false");

	return 0;
}
