#include <unencode.hh>
#include <split.hh>
#include <cstdio>


using std::unordered_map;
using std::string;
using std::sscanf;


unordered_map<string, string> unencode(string const& encoded)
{
    string unencoded{};

    for (auto i = 0; i < encoded.size(); ++i) {
        auto const& ch = encoded[i];
        switch (ch) {
        case '+':
            unencoded.push_back(' ');
            break;
        case '%': {
            int code;
            if (sscanf(encoded.c_str() + i + 1, "%2x", &code) != 1) {
                code = '?';
            }
            unencoded.push_back(code);
            i += 2;
            break;
        }
        default:
            unencoded.push_back(ch);
        }
    }

    unordered_map<string, std::string> r{};
    auto params = split(unencoded, "&");
    for (auto const& kv : params) {
        auto kvs = split(kv, "=");
        auto key = kvs[0];
        r[key] = "";

        if (kvs.size() == 2) {
            auto value = kvs[1];
            r[key] = value;
        }
    }

    return r;
}
