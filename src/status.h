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

class Status {
public:
    explicit Status() = default;

    explicit Status(std::string errorInfo)
        : error_code_(-1)
        , error_info_(std::move(errorInfo)) {}

public:
    static auto OK() {
        return Status();
    }

    static auto Invalid(std::string errorInfo) {
        return Status(std::move(errorInfo));
    }

    auto operator+=(const Status &cur) -> Status & {
        if (this->ok() && !cur.ok()) {
            this->error_code_ = cur.error_code_;
            this->error_info_ = cur.error_info_;
        }

        return *this;
    }

    auto ok() const -> bool {
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
