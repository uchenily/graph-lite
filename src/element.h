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

class GElement {
public:
    explicit GElement() = default;
    virtual ~GElement() = default;

protected:
    virtual auto init() -> CStatus {
        return CStatus();
    }

    virtual auto run() -> CStatus = 0;

    virtual auto destroy() -> CStatus {
        return CStatus();
    }

    auto getName() const -> std::string {
        return name_;
    }

    template <typename T,
              std::enable_if_t<std::is_base_of<GParam, T>::value, int> = 0>
    auto createGParam(const std::string &key) -> CStatus {
        return param_manager_->create<T>(key);
    }

    template <typename T,
              std::enable_if_t<std::is_base_of<GParam, T>::value, int> = 0>
    auto getGParam(const std::string &key) -> T * {
        return param_manager_->get<T>(key);
    }

private:
    void addElementInfo(const std::set<GElement *> &depends,
                        const std::string          &name,
                        GParamManager              *pm) {
        for (const auto &depend : depends) {
            dependence_.insert(depend);
            depend->run_before_.insert(this);
        }
        left_depend_ = dependence_.size();
        name_ = name;
        param_manager_ = pm;
    }

private:
    std::set<GElement *> run_before_{};
    std::set<GElement *> dependence_{};
    std::atomic<size_t>  left_depend_{0};
    std::string          name_{};
    GParamManager       *param_manager_ = nullptr;

    friend class GPipeline;
    friend class Schedule;
};

using GNode = GElement;

#endif // CGRAPH_LITE_ELEMENT_H
