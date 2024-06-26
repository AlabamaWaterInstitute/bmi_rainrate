
#include "bmi_rainrate_cpp.hpp"
#include <bmi.hxx>
#include <chrono>
#include <functional>
#include <tuple>
#include <cassert>

#define model_t BmiRainRateCpp
#define log(...) fprintf(stderr,"[%d]", __LINE__);fprintf(stderr, __VA_ARGS__);fprintf(stderr,"\n");

using duration_t = std::chrono::duration<double>;

// std::map<std::string, std::map<std::string, int>> anytype_allocs;
// size_t anytype_uuid = 0;

//template <typename cls, typename R, typename ...Args>
//using func_t = R (cls::*)(Args...);
template <typename func_t, typename ...Args>
duration_t timing_wrap(func_t func, int n, bool verbose, Args&&... args) {
    auto start = std::chrono::high_resolution_clock::now();
    std::_Mem_fn<func_t> mf(func);
    for (int i = 0; i < n; i++) {
        mf(args...);
    }
    auto end = std::chrono::high_resolution_clock::now();
    duration_t elapsed = end - start;
    if (verbose) {
        log("Elapsed time: %f", elapsed.count());
    }
    return elapsed;
}

//Testing ShiftQueue
void test_shift_queue() {
    int init_val = 0;
    int max_size = 5;
    ShiftQueue sq("test", init_val, max_size);
    for (int i = 0; i < max_size; i++) {
        assert(sq[i] == init_val);
    }
    std::cout << std::string(sq) << std::endl;
    for (int i = 0; i < max_size; i++) {
        sq.push(i);
    }
    std::cout << std::string(sq) << std::endl;
    for (int i = 0; i < max_size; i++) {
        assert(sq[i] == i);
    }
    for (int i = 0; i < max_size; i++) {
        std::cout << sq.get().to_string() << std::endl;
        sq.shift();
    }
    std::cout << std::string(sq) << std::endl;
    for (int i = 0; i < max_size; i++) {
        assert(sq[i] == init_val);
    }
}

//Testing AnyType flexibility
void test_any_type() {
    AnyType at;
    int i = 5;
    float f = 5.0;
    double d = 5.0;
    short s = 5;
    long l = 5;
    std::string str = "5";
    at = i;
    assert(at.to_string() == std::to_string(i));
    at = f;
    assert(at.to_string() == std::to_string(f));
    at = d;
    assert(at.to_string() == std::to_string(d));
    at = s;
    assert(at.to_string() == std::to_string(s));
    at = l;
    assert(at.to_string() == std::to_string(l));
    at = str;
    assert(at.to_string() == str);
}

int main(int argc, char *argv[]) {
    model_t model;

    log("Initializing");
    model.Initialize("../test/test_cfg.txt");

    
    // duration_t elapsed = timing_wrap(&model_t::Update, 100000000, true, model);

    // Test ShiftQueue
    // duration_t elapsed = timing_wrap(test_shift_queue, 1, true);
    test_shift_queue();
    test_any_type();


    model.Finalize();
    log("Finalized");

    return bmi::BMI_SUCCESS;
}


