#include <iostream>
int main() {
#ifdef _WIN64
    std::cout << "_WIN64 defined\n";
#else
    std::cout << "_WIN64 NOT defined\n";
#endif

#ifdef _M_X64
    std::cout << "_M_X64 defined\n";
#else
    std::cout << "_M_X64 NOT defined\n";
#endif

#ifdef _AMD64_
    std::cout << "_AMD64_ defined\n";
#else
    std::cout << "_AMD64_ NOT defined\n";
#endif
}
