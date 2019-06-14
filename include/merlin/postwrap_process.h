#pragma once

#include "cmdline_parser.h"
#include "file_parser.h"
#include "mars_opt.h"
#include "xml_parser.h"

#include "bsuNode_v2.h"
#include "codegen.h"
#include "mars_ir_v2.h"
#include "program_analysis.h"
#include "rose.h"

#define RETURN_VAR "merlin_return_value"
void return_replace_top(CSageCodeGen &codegen, void *pTopFunc,
                        CInputOptions &options);
void typedef_spread_top(CSageCodeGen &codegen, void *pTopFunc,
                        CInputOptions &options, bool castonly = false,
                        bool skip_builtin = false, void *scope = nullptr,
                        bool skip_include = true);
int reference_replace_top(CSageCodeGen &codegen, void *pTopFunc,
                          CInputOptions &options);

void func_typedef_spread(CSageCodeGen &codegen, void *func_decl,
                         set<void *> &visited_funcs, set<void *> &visited_type,
                         bool skip_builtin, void *scope = nullptr);

void func_typedef_spread(CSageCodeGen &codegen, void *func_decl,
                         set<void *> &visited_funcs, bool skip_builtin);
void func_reference_replace(CSageCodeGen &codegen, void *func_decl);
void structure_typedef_spread(CSageCodeGen &codegen, void *sg_type,
                              bool skip_builtin, void *scope = nullptr);
void castonly_typedef_spread(CSageCodeGen &codegen, void *func_decl,
                             set<void *> &visited_funcs, bool skip_builtin);
void sizeof_typedef_spread(CSageCodeGen &codegen, void *scope,
                           void *input_scope, bool skip_builtin);

void intype_expr_typedef_spread(CSageCodeGen &codegen, void *sg_type,
                                void *&sg_new_type, void *pos,
                                bool skip_builtin);

void init_typedef_spread(CSageCodeGen &codegen, void *sg_init,
                         set<void *> &visited_type, bool skip_builtin,
                         void *scope);

int remove_init_typedef(CSageCodeGen &codegen, void *sg_init,
                        void *&sg_new_type, bool skip_builtin);

int global_variable_conversion_checker(CSageCodeGen &codegen, void *pTopFunc,
                                       CInputOptions &options);

void collect_used_global_variable(void *scope, set<void *> &s_global,
                                  map<void *, set<void *>> &var2refs,
                                  CSageCodeGen *ast);
