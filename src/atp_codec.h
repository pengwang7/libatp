#ifndef __ATP_CODEC_H__
#define __ATP_CODEC_H__

#include <string>
#include <sstream>

#include "jsoncpp/json.h"

namespace atp {

class Codec {
public:
    explicit Codec() {}
    ~Codec() {}

public:
    std::string encode(const Json::Value& table) {
        std::string res("");
        if (table == Json::Value::null) {
            return res;
        }

        std::ostringstream out;
        Json::StreamWriterBuilder streamBuilder;
        std::unique_ptr<Json::StreamWriter> streamWriter(streamBuilder.newStreamWriter());
        streamWriter->write(table, &out);
        res = out.str();

        return res;
    }

    int decode(const std::string str, Json::Value& table) {
        JSONCPP_STRING errs;
        Json::CharReaderBuilder charBuilder;
        std::unique_ptr<Json::CharReader> charReader(charBuilder.newCharReader());
        int res = charReader->parse(str.c_str(), str.c_str() + str.length(), &table, &errs);
        if (!res || !errs.empty()) {
            return -1;
        }

        return 0;
    }
};

} /* end namespace atp */

#endif /* __ATP_CODEC_H__ */
