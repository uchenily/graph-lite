/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: element.h
@Time: 2024/9/6 20:44
@Desc:
***************************/

#ifndef CGRAPH_LITE_ELEMENT_H
#define CGRAPH_LITE_ELEMENT_H

#include <atomic>
#include <set>
#include <string>

#include "status.h"

class GNode {
public:
    explicit GNode() = default;
    virtual ~GNode() = default;

protected:
    virtual auto run() -> Status = 0;

    auto getName() const -> std::string {
        return name_;
    }

    // template <typename T,
    //           std::enable_if_t<std::is_base_of_v<GParam, T>, int> = 0>
    // auto createParam(const std::string &key) -> Status {
    //     return param_manager_->create<T>(key);
    // }
    //
    // template <typename T,
    //           std::enable_if_t<std::is_base_of_v<GParam, T>, int> = 0>
    // auto getParam(const std::string &key) -> T * {
    //     return param_manager_->get<T>(key);
    // }

private:
    void addNodeInfo(const std::set<GNode *> &inputs, const std::string &name) {
        for (const auto &input : inputs) {
            this->inputs_.insert(input);
            input->outputs_.insert(this);
        }
        num_pending_ = inputs_.size();
        name_ = name;
    }

private:
    std::set<GNode *>   inputs_{};
    std::set<GNode *>   outputs_{};
    std::atomic<size_t> num_pending_{0};
    std::string         name_{};

    friend class GPipeline;
    friend class Schedule;
};

#endif // CGRAPH_LITE_ELEMENT_H
