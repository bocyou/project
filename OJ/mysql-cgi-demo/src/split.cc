#include <split.hh>


using std::vector;
using std::string;


vector<string> split(string const& s, string const& delimiter)
{
    vector<string> r{};
    size_t last = 0;
    size_t pos;

    do {
        pos = s.find_first_of(delimiter, last);
        r.push_back(s.substr(last, pos - last));
        last = pos + 1;
    } while (pos != string::npos);

    return r;
}

vector<string> split(string const& s)
{
    return split(s, " ");
}
