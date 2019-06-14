
#include "mars_ir.h"

int loop_tiling_top(CSageCodeGen &codegen, void *pTopFunc,
                    CInputOptions &options);
int loop_tiling_top(CSageCodeGen &codegen, void *pTopFunc,
                    CInputOptions &options, bool pragma_in_loop);

void collect_tripcount_pragma(CSageCodeGen &codegen, CMarsIr *mars_ir,
                              void *scope, void *&sg_bound_pragma,
                              int &orig_factor);
