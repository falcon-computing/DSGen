#ifndef __LOOP_DISTRIBUTE_H__
#define __LOOP_DISTRIBUTE_H__
#include "cmdline_parser.h"
#include "codegen.h"
#include "mars_ir_v2.h"

class LoopDistribute {
private:
  CSageCodeGen &mCodegen;
  CMarsIrV2 mMars_ir;

public:
  LoopDistribute(CSageCodeGen &codegen, void *pTopFunc, CInputOptions &option);
  bool run();

private:
  bool process_all_scopes(vector<void *> &top_loops);

  bool lift_local_variable_out_of_loop();

  bool lift_local_variable_out_of_loop(void *loop);

  bool distribute(vector<void *> &top_loops);
};

class LoopFlatten {
private:
  CSageCodeGen &mCodegen;
  CMarsIrV2 mMars_ir;
  CInputOptions &mOptions;

public:
  LoopFlatten(CSageCodeGen &codegen, void *pTopFunc, CInputOptions &option);
  bool run();

private:
  bool flatten(vector<void *> &top_loops, set<void *> &unrolled_loops);

  int loop_flatten_nested_loops(void *sg_top_loop,
                                set<void *> &not_touched_loops,
                                bool no_final_reset);
};
#endif
