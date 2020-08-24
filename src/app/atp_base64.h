/*
 * MIT License
 *
 * Copyright (c) 2019 pengwang7(https://github.com/pengwang7/libatp)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef __ATP_BASE64_H__
#define __ATP_BASE64_H__

#include <string>

namespace atp {

class Base64 {
public:
    Base64() {}

    ~Base64() {}

public:
    static bool base64Encode(const std::string& src, std::string* dst);

    static bool base64Decode(const std::string& src, std::string* dst);

private:
    static inline char base2Char(unsigned char n);

    static inline unsigned char char2Base(char c);

    inline static int base64EncodeLen(int n);

    inline static int base64DecodeLen(int n);
};

} /* end namespace atp */

#endif /* __ATP_BASE64_H__ */
