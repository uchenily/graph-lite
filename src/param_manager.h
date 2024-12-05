/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: param_manager.h
@Time: 2024/9/7 16:38
@Desc:
***************************/

#ifndef CGRAPH_LITE_PARAM_MANAGER_H
#define CGRAPH_LITE_PARAM_MANAGER_H

#include <memory>
#include <mutex>
#include <typeinfo>
#include <unordered_map>

#include "param.h"

class GParamManager {
protected:
    template <typename T,
              std::enable_if_t<std::is_base_of<GParam, T>::value, int> = 0>
    auto create(const std::string &key) -> CStatus {
        std::lock_guard<std::mutex> lk(mutex_);

        auto iter = params_.find(key);
        if (iter != params_.end()) {
            auto p = iter->second;
            return (typeid(*p).name() == typeid(T).name())
                       ? CStatus()
                       : CStatus("create [" + key + "] param duplicate");
        }

        auto param = std::make_shared<T>();
        params_.insert(std::pair<std::string, T *>(key, param));
        return CStatus();
    }

    template <typename T,
              std::enable_if_t<std::is_base_of<GParam, T>::value, int> = 0>
    auto get(const std::string &key) -> T * {
        auto iter = params_.find(key);
        if (iter == params_.end()) {
            return nullptr;
        }

        return dynamic_cast<T *>(iter->second);
    }

    auto setup() -> CStatus {
        CStatus status;
        for (auto &param : params_) {
            status += param.second->setup();
        }
        return status;
    }

    void reset(const CStatus &curStatus) {
        for (auto &param : params_) {
            param.second->reset(curStatus);
        }
    }

    ~GParamManager() {
        params_.clear();
    }

private:
    std::unordered_map<std::string, std::shared_ptr<GParam>> params_{};
    std::mutex                                               mutex_{};

    friend class GParam;
    friend class GElement;
    friend class GPipeline;
};

#endif // CGRAPH_LITE_PARAM_MANAGER_H
