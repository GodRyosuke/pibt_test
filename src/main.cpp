#ifdef _MSC_BUILD 
#define _CRTDBG_MAP_ALLOC
#endif
#include <exception>
#include <iostream>
#include "game.hpp"

int main()
{
    try {
        wander_csm_test::Game game;
        game.init();
        game.run();
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }

#ifdef _MSC_BUILD 
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
    _CrtDumpMemoryLeaks();
#endif
    return 0;
}