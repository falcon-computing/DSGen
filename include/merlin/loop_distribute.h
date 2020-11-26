/************************************************************************************
 *  (c) Copyright 2014-2020 Falcon Computing Solutions, Inc. All rights reserved.
 *
 *  This file contains confidential and proprietary information
 *  of Falcon Computing Solutions, Inc. and is protected under U.S. and
 *  international copyright and other intellectual property laws.
 *
 ************************************************************************************/

#ifndef TRUNK_SOURCE_OPT_TOOLS_INCLUDE_LOOP_DISTRIBUTE_H_
#define TRUNK_SOURCE_OPT_TOOLS_INCLUDE_LOOP_DISTRIBUTE_H_
#include <set>
#include <vector>

#include "codegen.h"
#include "mars_ir_v2.h"

class LoopDistribute {
 private:
  CSageCodeGen &mCodegen;
  CMarsIrV2 mMars_ir;

 public:
  LoopDistribute(CSageCodeGen *codegen, void *pTopFunc,
                 const CInputOptions &option);
  bool run();

 private:
  bool process_all_scopes(const vector<void *> &top_loops);

  bool lift_local_variable_out_of_loop();

  bool lift_local_variable_out_of_loop(void *loop);

  bool distribute(const vector<void *> &top_loops);
};

class LoopFlatten {
 private:
  CSageCodeGen &mCodegen;
  CMarsIrV2 mMars_ir;
  CInputOptions &mOptions;

 public:
  LoopFlatten(CSageCodeGen *codegen, void *pTopFunc,
              const CInputOptions &option);
  bool run();

 private:
  bool flatten(const vector<void *> &top_loops,
               const set<void *> &unrolled_loops);

  int loop_flatten_nested_loops(void *sg_loop,
                                const set<void *> &not_touched_loops,
                                bool no_final_reset);
};
#endif  // TRUNK_SOURCE_OPT_TOOLS_INCLUDE_LOOP_DISTRIBUTE_H_
