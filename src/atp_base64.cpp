#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "atp_base64.h"

namespace atp {

static char base64_code[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

char Base64::base2Char(unsigned char n) {
    n &= 0x3F;
    if (n < 26)
        return static_cast<char>(n + 'A');
    else if (n < 52)
        return static_cast<char>(n - 26 + 'a');
    else if (n < 62)
        return static_cast<char>(n - 52 + '0');
    else if (n == 62)
        return '+';
    else
        return '/';
}

unsigned char Base64::char2Base(char c) {
    if (c >= 'A' && c <= 'Z')
        return (unsigned char)(c - 'A');
    else if (c >= 'a' && c <= 'z')
        return (unsigned char )(c - 'a' + 26);
    else if (c >= '0' && c <= '9')
        return ( unsigned char )(c - '0' + 52);
    else if (c == '+')
        return 62;
    else if (c == '/')
        return 63;
    else
        return 64;
}

bool Base64::base64Encode(const std::string& src, std::string* dst) {
    if (0 == src.size() || NULL == dst) {
        return false;
    }

    dst->resize(base64EncodeLen(src.size()));

    int c = -1;

    unsigned char* p = reinterpret_cast<unsigned char*>(&(*dst)[0]);
    unsigned char* s = p;
    unsigned char* q = reinterpret_cast<unsigned char*>(const_cast<char*>(&src[0]));

    for (size_t i = 0; i < src.size();) {
        c = q[i++];
        c *= 256;
        if (i < src.size())
            c += q[i];
        i++;
        c *= 256;
        if (i < src.size())
            c += q[i];
        i++;

        p[0] = base64_code[(c & 0x00fc0000) >> 18];
        p[1] = base64_code[(c & 0x0003f000) >> 12];
        p[2] = base64_code[(c & 0x00000fc0) >> 6];
        p[3] = base64_code[(c & 0x0000003f) >> 0];

        if (i > src.size())
            p[3] = '=';

        if (i > src.size() + 1)
            p[2] = '=';

        p += 4;
    }

    *p = 0;
    dst->resize(p - s);

    return true;
}

bool Base64::base64Decode(const std::string& src, std::string* dst) {
    if (0 == src.size() || NULL == dst) {
        return false;
    }

    dst->resize(base64DecodeLen(src.size()));

    unsigned char* p = reinterpret_cast<unsigned char*>(&(*dst)[0]);
    unsigned char* q = p;
    unsigned char c = 0;
    unsigned char t = 0;

    for (size_t i = 0; i < src.size(); i++) {
        if (src[i] == '=')
            break;

        do {
            if (src[i])
                c = char2Base(src[i]);
            else
                c = 65;
        } while (c == 64);

        if (c == 65)
            break;

        switch ( i % 4 ) {
        case 0 :
            t = c << 2;
            break;
        case 1 :
            *p = (unsigned char)(t | (c >> 4));
            p++;
            t = ( unsigned char )( c << 4 );
            break;
        case 2 :
            *p = (unsigned char)(t | (c >> 2));
            p++;
            t = ( unsigned char )(c << 6);
            break;
        case 3 :
            *p = ( unsigned char )(t | c);
            p++;
            break;
        }
    }

    dst->resize(p - q);

    return true;
}

int Base64::base64EncodeLen(int n) {
    return (n + 2) / 3 * 4 + 1;
}

int Base64::base64DecodeLen(int n) {
    return n / 4 * 3 + 2;
}

} /* end namespace atp */
