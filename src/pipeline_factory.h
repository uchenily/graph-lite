/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: pipeline_factory.h
@Time: 2024/9/6 23:29
@Desc:
***************************/

#ifndef CGRAPH_LITE_PIPELINE_FACTORY_H
#define CGRAPH_LITE_PIPELINE_FACTORY_H

#include "pipeline.h"

class GPipelineFactory {
public:
    static auto create() {
        auto pipeline = std::make_shared<GPipeline>();
        return pipeline;
    }
};

#endif // CGRAPH_LITE_PIPELINE_FACTORY_H
