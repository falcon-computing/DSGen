#ifndef TRUNK_SOURCE_OPT_TOOLS_INCLUDE_LOOP_TILING_H_
#define TRUNK_SOURCE_OPT_TOOLS_INCLUDE_LOOP_TILING_H_
#include "mars_ir.h"

int loop_tiling_top(CSageCodeGen *codegen, void *pTopFunc,
                    const CInputOptions &options);
int loop_tiling_top(CSageCodeGen *codegen, void *pTopFunc,
                    const CInputOptions &options, bool pragma_in_loop);

void collect_tripcount_pragma(CSageCodeGen *codegen, CMarsIr *mars_ir,
                              void *scope, void **sg_bound_pragma,
                              int *orig_factor);
#endif  // TRUNK_SOURCE_OPT_TOOLS_INCLUDE_LOOP_TILING_H_
