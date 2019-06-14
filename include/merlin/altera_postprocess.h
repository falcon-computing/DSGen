#pragma once

#include "cmdline_parser.h"
#include "file_parser.h"
#include "mars_opt.h"
#include "xml_parser.h"

#include "bsuNode_v2.h"
#include "codegen.h"
#include "mars_ir.h"
#include "mars_ir_v2.h"
#include "program_analysis.h"
#include "rose.h"

int pragma_spread_top(CSageCodeGen &codegen, void *pTopFunc,
                      CInputOptions &options);
int remove_redundant_function_and_global_variables(CSageCodeGen &codegen,
                                                   void *pTopFunc);
bool check_legal_complete_unroll(CSageCodeGen &codegen, void *sg_loop,
                                 int64_t &l_range);

void check_false_dependency_altera(CSageCodeGen &codegen, void *pTopFunc,
                                   CInputOptions &options);
bool check_false_dependency_altera(CSageCodeGen &codegen, void *sg_pragma,
                                   void *target_var, bool check_all = false);
