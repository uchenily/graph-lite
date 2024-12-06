#include "./src/graph_lite.hpp"

#include <iostream>

using namespace std::literals;

class MyNode1 : public GNode {
    auto run() -> Status override {
        std::cout << getName() << ": sleep 1ms\n";
        std::this_thread::sleep_for(1ms);
        return Status::OK();
    }
};

class MyNode2 : public GNode {
    auto run() -> Status override {
        std::cout << getName() << ": sleep 1000ms\n";
        std::this_thread::sleep_for(1000ms);
        return Status::OK();
    }
};

auto main() -> int {
    //    A      1ms
    // +--+--+
    // B     C   1000ms/1000ms
    // +--+--+
    //    D      1ms

    MyNode1 a;
    MyNode2 b;
    MyNode2 c;
    MyNode1 d;

    auto pipeline = GPipelineFactory::create();
    pipeline->registerNode("nodeA", &a, {&b, &c});
    pipeline->registerNode("nodeB", &b, {&d});
    pipeline->registerNode("nodeC", &c, {&d});
    pipeline->registerNode("nodeD", &d, {});

    pipeline->process();
    return 0;
}
