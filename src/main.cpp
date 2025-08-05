#define _CRTDBG_MAP_ALLOC
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

    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
    _CrtDumpMemoryLeaks();
    return 0;
}