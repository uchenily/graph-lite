/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: status.h
@Time: 2024/9/6 20:31
@Desc:
***************************/

#ifndef CGRAPH_LITE_STATUS_H
#define CGRAPH_LITE_STATUS_H

#include <string>
#include <utility>

class CStatus {
public:
    explicit CStatus() = default;

    explicit CStatus(std::string errorInfo)
        : error_code_(-1)
        , error_info_(std::move(errorInfo)) {}

    auto operator+=(const CStatus &cur) -> CStatus & {
        if (this->isOK() && !cur.isOK()) {
            this->error_code_ = cur.error_code_;
            this->error_info_ = cur.error_info_;
        }

        return (*this);
    }

    auto isOK() const -> bool {
        return 0 == error_code_;
    }

    auto getCode() const -> int {
        return error_code_;
    }

    auto getInfo() const -> std::string {
        return error_info_;
    }

private:
    int         error_code_{0};
    std::string error_info_;
};

#endif // CGRAPH_LITE_STATUS_H
