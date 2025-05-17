#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <functional>
#include <utility>
#include <mutex>
#include <stdexcept>
#include <cstddef>


#ifdef _WIN32
    #ifdef IMENGINE_ENGINE_EXPORTS
        #define IMENGINE_CORE_API __declspec(dllexport)
    #else
        #define IMENGINE_CORE_API __declspec(dllimport)
    #endif
#else
    #define IMENGINE_API
#endif