# SearchEmojiOnString

一个例子程序，判断给定的字符串中是否包含 emoji 表情符号。
用标准 C++ 实现，支持跨平台。

UTF8_To_UTF16 和 UTF16_To_UTF8 这两个方法来自 http://blog.csdn.net/charlessimonyi/article/details/8722859

isContainsEmoji 方法中检查是否 emoji 符号的代码来自网络上一个基于 NSString 的实现，我只是把它修改成了标准 C++ 的实
现，以支持跨平台的需求。

最好在 Unix like 的系统上用 g++ 编译，一个字儿——方便。
g++ main.cpp -o testEmoji