/************************************************************************************
 *  (c) Copyright 2014-2020 Falcon Computing Solutions, Inc. All rights
 *reserved.
 *
 *  This file contains confidential and proprietary information
 *  of Falcon Computing Solutions, Inc. and is protected under U.S. and
 *  international copyright and other intellectual property laws.
 *
 ************************************************************************************/

#ifndef TRUNK_SOURCE_OPT_TOOLS_INCLUDE_COARSE_PARALLEL_H_
#define TRUNK_SOURCE_OPT_TOOLS_INCLUDE_COARSE_PARALLEL_H_

#include "rose.h"

#include "PolyModel.h"
#include "cmdline_parser.h"
#include "file_parser.h"
#include "mars_opt.h"
#include "memory_burst.h"
#include "stream_ir.h"
#include "tldm_annotate.h"
#include "xml_parser.h"

int coarse_parallel_intel_top(CSageCodeGen *codegen, void *pTopFunc,
                              const CInputOptions &options);

int coarse_parallel_xilinx_top(CSageCodeGen *codegen, void *pTopFunc,
                               const CInputOptions &options);

bool generate_parallel_xilinx(CSageCodeGen *codegen, CMarsIr *mars_ir,
                              CMirNode *node);

bool generate_parallel_intel(CSageCodeGen *codegen, CMarsIr *mars_ir,
                             MerlinStreamModel::CStreamIR *stream_ir,
                             CMirNode *node, MemoryBurst *burst_instance);
#endif  // TRUNK_SOURCE_OPT_TOOLS_INCLUDE_COARSE_PARALLEL_H_
