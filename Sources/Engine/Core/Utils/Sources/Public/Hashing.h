#pragma once

template <typename T, typename... Rest>
void hash_combine(std::size_t& seed, const T& v, const Rest&... rest)
{
	seed ^= std::hash<T>{}(v)+0x9e3779b9 + (seed << 6) + (seed >> 2);
	(hash_combine(seed, rest), ...);
}

#define MAKE_HASHABLE(type, ...) \
    namespace std {\
        template<> struct hash<type> {\
            std::size_t operator()(const type &t) const {\
                std::size_t ret = 0;\
                hash_combine(ret, __VA_ARGS__);\
                return ret;\
            }\
        };\
    }