
#include "bmi_rainrate_cpp.hpp"
#include <bmi.hxx>
#include <chrono>
#include <functional>
#include <tuple>

#define model_t BmiRainRateCpp
#define log(...) fprintf(stderr,"[%d]", __LINE__);fprintf(stderr, __VA_ARGS__);fprintf(stderr,"\n");

using duration_t = std::chrono::duration<double>;

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

int main(int argc, char *argv[]) {
    model_t model;

    log("Initializing");
    model.Initialize("../test/test_cfg.txt");

    
    duration_t elapsed = timing_wrap(&model_t::Update, 100000000, true, model);


    model.Finalize();
    log("Finalized");

    return bmi::BMI_SUCCESS;
}


