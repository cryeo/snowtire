#pragma once

#define LOG(...) Logger::log(__FUNCTION__, __VA_ARGS__)

namespace Logger {
    template<typename T, typename... Args>
    inline void log(T caller, Args... args) {
        printf("[%s] ", caller);
        printf(args...);
        printf("\n");
    }

    inline void log() {
    }
};

/*
namespace Logger {
    template<typename...Args>
    struct Log;

    template<typename T, typename...Args>
    struct Log<T, Args...>  {
        void operator()(T head, Args... tail) {
            cout << " " << head;
            Log<Args...>{}(tail...);
        }
    };

    template<>
    struct Log<> {
        void operator()() {
            cout << std::endl;
        }
    };

    template<typename T, typename... Args>
    void log(T caller, Args... args) {
        cout << "[" << caller << "]";
        Log<Args...>{}(args...);
    }
};
*/
