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

#include "param.h"
#include "param_manager.h"
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

    template <typename T,
              std::enable_if_t<std::is_base_of_v<GParam, T>, int> = 0>
    auto createParam(const std::string &key) -> Status {
        return param_manager_->create<T>(key);
    }

    template <typename T,
              std::enable_if_t<std::is_base_of_v<GParam, T>, int> = 0>
    auto getParam(const std::string &key) -> T * {
        return param_manager_->get<T>(key);
    }

private:
    void addNodeInfo(const std::set<GNode *> &inputs,
                     const std::string       &name,
                     GParamManager           *manager) {
        for (const auto &input : inputs) {
            inputs_.insert(input);
            input->run_before_.insert(this);
        }
        num_pending_ = inputs_.size();
        name_ = name;
        param_manager_ = manager;
    }

private:
    std::set<GNode *>   run_before_{};
    std::set<GNode *>   inputs_{};
    std::atomic<size_t> num_pending_{0};
    std::string         name_{};
    GParamManager      *param_manager_ = nullptr;

    friend class GPipeline;
    friend class Schedule;
};

#endif // CGRAPH_LITE_ELEMENT_H
