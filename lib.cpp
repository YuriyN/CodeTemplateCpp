#include "lib.hpp"

void make_dir(char* path)
{
#if _MSC_VER
    _mkdir(path);
#else
    mkdir(path, 0777);
#endif
}

std::string modify_path(char* src)
{
    std::string dst = src;
    if (dst.empty())
    {
        return "";
    }
    else
    {
        std::replace(dst.begin(), dst.end(), '\\', '/');
        if (dst.back() != '/') dst += '/';
        return dst;
    }
}