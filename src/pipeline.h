/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: pipeline.h
@Time: 2024/9/6 20:31
@Desc:
***************************/

#ifndef CGRAPH_LITE_PIPELINE_H
#define CGRAPH_LITE_PIPELINE_H

#include <algorithm>
#include <memory>
#include <mutex>
#include <set>
#include <vector>

#include "element.h"
#include "param_manager.h"
#include "schedule.h"
#include "status.h"

class GPipeline {
public:
    /**
     * @param times
     * @return
     */
    auto process(size_t times = 1) -> Status {
        schedule_ = std::make_unique<Schedule>();
        while (((times--) != 0u) && status_.isOK()) {
            run();
        }
        return status_;
    }

    /**
     * register element(node) into pipeline
     * @tparam T
     * @param elementRef
     * @param inputs
     * @param name
     * @return
     */
    // template <typename T,
    //           std::enable_if_t<std::is_base_of<GElement, T>::value, int> = 0>
    auto registerNode(const std::string       &name,
                      GNode                   *node,
                      const std::set<GNode *> &inputs) -> Status {
        // check not null
        if (std::any_of(inputs.begin(), inputs.end(), [](GNode *ptr) {
                return ptr == nullptr;
            })) {
            return Status::Invalid("input is null"); // no allow empty input
        }

        node->addNodeInfo(inputs, name, &param_manager_);
        nodes_.emplace_back(node);
        return Status::OK();
    }

protected:
    void run() {
        setup();
        for (auto *node : nodes_) {
            if (node->inputs_.empty()) {
                schedule_->submit([this, node] {
                    execute(node);
                });
            }
        }
        reset();
    }

    void execute(GNode *node) {
        if (!status_.isOK()) {
            return;
        }

        status_ += node->run();
        for (auto *cur : node->outputs_) {
            if (--cur->num_pending_ <= 0) {
                schedule_->submit([this, cur] {
                    execute(cur);
                });
            }
        }

        std::unique_lock<std::mutex> lk(execute_mutex_);
        if (++finished_size_ >= nodes_.size() || !status_.isOK()) {
            execute_cv_.notify_one();
        }
    }

    void setup() {
        finished_size_ = 0;
        // for (auto *node : nodes_) {
        //     node->num_pending_ = node->inputs_.size();
        // }

        status_ += param_manager_.setup();
    }

    void reset() {
        // wait for all node finished
        {
            std::unique_lock<std::mutex> lk(execute_mutex_);
            execute_cv_.wait(lk, [this] {
                return finished_size_ >= nodes_.size() || !status_.isOK();
            });
        }

        param_manager_.reset(status_);
    }

private:
    std::vector<GNode *>      nodes_{};
    std::unique_ptr<Schedule> schedule_ = nullptr;
    size_t                    finished_size_{0};
    std::mutex                execute_mutex_{};
    std::condition_variable   execute_cv_{};
    Status                    status_;
    GParamManager             param_manager_;
};

#endif // CGRAPH_LITE_PIPELINE_H
