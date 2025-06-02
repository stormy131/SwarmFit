#ifndef CONFIG_H
#define CONFIG_H

// #define DEBUG_MODE

#ifdef DEBUG_MODE
    #define DEBUG_PRINT(x) std::cout << x << std::endl
#else
    #define DEBUG_PRINT(x)
#endif

#endif