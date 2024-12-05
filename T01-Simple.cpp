/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: T01-simple.cpp
@Time: 2024/9/6 20:24
@Desc:
***************************/

#include <chrono>
#include <iostream>

#include "./src/CGraph-lite.h"

class MyNode1 : public GNode {
    auto run() -> CStatus override {
        std::cout << getName() << ": sleep 1s.\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));
        return CStatus();
    }
};

class MyNode2 : public GNode {
    auto run() -> CStatus override {
        std::cout << getName() << ": sleep 2s.\n";
        std::this_thread::sleep_for(std::chrono::seconds(2));
        return CStatus();
    }
};

void tutorial_simple() {
    // GElement *a, *b, *c, *d = nullptr;
    MyNode1 a;
    MyNode2 b;
    MyNode1 c;
    MyNode2 d;

    auto pipeline = GPipelineFactory::create();
    pipeline->registerGElement("nodeA", &a, {});
    pipeline->registerGElement("nodeB", &b, {&a});
    pipeline->registerGElement("nodeC", &c, {&a});
    pipeline->registerGElement("nodeD", &d, {&b, &c});

    pipeline->process();
}

auto main() -> int {
    tutorial_simple();
    return 0;
}
