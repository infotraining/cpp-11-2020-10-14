#include "catch.hpp"
#include <array>
#include <future>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>

using namespace std;
using namespace Catch::Matchers;

void may_throw1(int id)
{
    if (id % 2 == 0)
        throw std::runtime_error("Error#Even");
}

void may_throw2(int id)
{
    if (id % 2 != 0)
        throw std::invalid_argument("Error#Odd");
}

void background_work(size_t id, std::shared_ptr<const std::string> text, std::chrono::milliseconds delay,
    int& counter,
    std::exception_ptr& excpt)
{
    std::cout << "bw#" << id << " has started..." << std::endl;

    for (const auto& c : *text)
    {
        std::cout << "bw#" << id << ": " << c << std::endl;

        std::this_thread::sleep_for(delay);
    }

    for (int i = 0; i < 1'000'000; ++i)
        counter++;

    try
    {
        may_throw1(id);
        may_throw2(id);
    }
    catch (...)
    {
        excpt = std::current_exception();
    }

    std::cout << "bw#" << id << " is finished..." << std::endl;
}

class BackgroundTask
{
public:
    void operator()(const std::string& text) const
    {
        std::cout << "Hello from a thread..." << text << std::endl;
    }
};

int calculate_square(int x)
{
    cout << "Starting calculation for " << x << endl;

    int interval = rand() % 5000 + 1000;

    this_thread::sleep_for(chrono::milliseconds(interval));

    if (x == 13)
        throw runtime_error("Error#13");

    return x * x;
}

void save()
{
    std::cout << "start save..." << std::endl;
    this_thread::sleep_for(1s);
    std::cout << "end save..." << std::endl;
}

TEST_CASE("async bug")
{
    std::async(std::launch::async, save);
    std::async(std::launch::async, save);
    std::async(std::launch::async, save);
    std::async(std::launch::async, save);

    SECTION("async - proper way")
    {
        auto f1 = std::async(std::launch::async, save);
        auto f2 = std::async(std::launch::async, save);
        auto f3 = std::async(std::launch::async, save);
        auto f4 = std::async(std::launch::async, save);
    }
}

TEST_CASE("futures")
{
    std::future<int> f1 = std::async(std::launch::deferred, calculate_square, 42);
    std::future<int> f2 = std::async(std::launch::async, calculate_square, 3);
    std::future<int> f3 = std::async(std::launch::async, calculate_square, 13);

    this_thread::sleep_for(100ms);

    std::cout << "f1: " << f1.get() << std::endl;
    std::cout << "f2: " << f2.get() << std::endl;
    try
    {
        std::cout << "f3: " << f3.get() << std::endl;
    }
    catch (const runtime_error& e)
    {
        std::cout << "Caught: " << e.what() << std::endl;
    }

    //////////////////////////////////////////////

    std::vector<std::future<int>> fs;

    for (int i = 1; i < 10; ++i)
        fs.push_back(std::async(std::launch::async, [i] { return calculate_square(i); }));

    for (auto& f : fs)
        std::cout << f.get() << std::endl;
}

// TEST_CASE("threads")
// {
//     std::thread not_a_thread;

//     std::cout << not_a_thread.get_id() << "\n";

//     auto text_sp = make_shared<const std::string>("Hello world...");

//     std::array<std::exception_ptr, 3> exceptions;
//     std::array<int, 3> counters{};

//     std::thread thd1{&background_work, 1, text_sp, 250ms, std::ref(counters[0]), std::ref(exceptions[0])};
//     std::thread thd2{&background_work, 2, text_sp, 100ms, std::ref(counters[1]), std::ref(exceptions[1])};
//     assert(text_sp.use_count() == 3);

//     std::thread thd3{BackgroundTask{}, "#######"};
//     std::thread thd4{[&, text_sp]{ background_work(3, text_sp, 500ms, std::ref(counters[2]), std::ref(exceptions[2])); }};
//     text_sp.reset();

//     thd4.detach();

//     std::vector<std::thread> thds;
//     thds.push_back(std::move(thd1));
//     thds.push_back(std::move(thd2));
//     thds.push_back(std::move(thd3));
//     thds.push_back(std::move(thd4));

//     for(auto& t : thds)
//         if (t.joinable())
//             t.join();

//     ///////////
//     // Error handling
//     for(auto& e : exceptions)
//     {
//         if (e)
//         {
//             try
//             {
//                 std::rethrow_exception(e);
//             }
//             catch(const std::exception& e)
//             {
//                 std::cout << "Exception caught: " << e.what() << std::endl;
//             }
//         }
//     }
// }