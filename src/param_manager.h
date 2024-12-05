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
              std::enable_if_t<std::is_base_of_v<GParam, T>, int> = 0>
    auto create(const std::string &key) -> Status {
        std::lock_guard<std::mutex> lk(mutex_);

        auto iter = params_.find(key);
        if (iter != params_.end()) {
            auto p = iter->second;
            return (typeid(*p).name() == typeid(T).name())
                       ? Status::OK()
                       : Status::Invalid("create [" + key
                                         + "] param duplicate");
        }

        auto param = std::make_shared<T>();
        params_.insert(std::pair<std::string, T *>(key, param));
        return Status::OK();
    }

    template <typename T,
              std::enable_if_t<std::is_base_of_v<GParam, T>, int> = 0>
    auto get(const std::string &key) -> T * {
        auto iter = params_.find(key);
        if (iter == params_.end()) {
            return nullptr;
        }

        return dynamic_cast<T *>(iter->second);
    }

    auto setup() -> Status {
        for (auto &param : params_) {
            auto status = param.second->setup();
            if (!status.isOK()) {
                return status;
            }
        }
        return Status::OK();
    }

    void reset(const Status &curStatus) {
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
