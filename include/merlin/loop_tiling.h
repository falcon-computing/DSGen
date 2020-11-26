/************************************************************************************
 *  (c) Copyright 2014-2020 Falcon Computing Solutions, Inc. All rights reserved.
 *
 *  This file contains confidential and proprietary information
 *  of Falcon Computing Solutions, Inc. and is protected under U.S. and
 *  international copyright and other intellectual property laws.
 *
 ************************************************************************************/

#ifndef TRUNK_SOURCE_OPT_TOOLS_INCLUDE_LOOP_TILING_H_
#define TRUNK_SOURCE_OPT_TOOLS_INCLUDE_LOOP_TILING_H_
#include "mars_ir.h"

int loop_tiling_top(CSageCodeGen *codegen, void *pTopFunc,
                    const CInputOptions &options);
int loop_tiling_top(CSageCodeGen *codegen, void *pTopFunc,
                    const CInputOptions &options, bool pragma_in_loop);

void collect_tripcount_pragma(CSageCodeGen *codegen, CMarsIr *mars_ir,
                              void *scope, void **sg_bound_pragma,
                              int64_t *trip_count);
#endif  // TRUNK_SOURCE_OPT_TOOLS_INCLUDE_LOOP_TILING_H_
