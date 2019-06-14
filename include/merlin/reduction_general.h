#pragma once

#include "rose.h"

#include "PolyModel.h"
#include "cmdline_parser.h"
#include "file_parser.h"
#include "mars_opt.h"
#include "tldm_annotate.h"
#include "xml_parser.h"

using namespace std;
using namespace MarsProgramAnalysis;

void reduction_general(CSageCodeGen &codegen, void *pTopFunc,
                       CInputOptions &options);
void reduction_analysis(CSageCodeGen &codegen, void *pTopFunc,
                        CMarsIr *mars_ir);
int analyze_variables(CSageCodeGen &codegen, CMarsIr *mars_ir, CMirNode *bNode,
                      CMirNode *cNode, void *target_var, void *arr_pntr_ref,
                      int &lift_level);
bool transform_variables(CSageCodeGen &codegen, CMarsIr *mars_ir,
                         CMirNode *bNode, CMirNode *cNode, void *target_var,
                         void *curr_op, vector<void *> &pntr_to_reduce,
                         int &lift_level, string scheme_str);
void reduction_postprocessing(CSageCodeGen &codegen, void *pTopFunc,
                              CMarsIr *mars_ir);
bool build_reduction_general(CSageCodeGen &codegen, void *target_var,
                             void *curr_op, int lift_level,
                             vector<void *> pntr_to_reduce,
                             int scheme_1_enabled,
                             vector<void *> vec_reduct_loops,
                             vector<void *> vec_parallel_loops,
                             vector<void *> loop_nest);

bool anal_reduction(CSageCodeGen &codegen, CMarsIr *mars_ir, void *arr_pntr_ref,
                    void *curr_op, void *target_var, string scheme_str,
                    int &scheme_1_enabled, int &lift_level,
                    vector<void *> &vec_reduct_loops,
                    vector<void *> &vec_parallel_loops,
                    vector<void *> &loop_nest);

void insert_reduc_function(CSageCodeGen &codegen, string func_type, void *var,
                           void *sg_copy, void *sg_location, string buf_type,
                           map<void *, void *> &all_func_call_map);

void parsing_reduction_pragma(CSageCodeGen &codegen, CMirNode *bNode,
                              vector<void *> &reduc_vars, string &scheme_str);

void replace_reference_by_var(CSageCodeGen &codegen, void *old_init,
                              void *new_init, void *sg_scope);
void check_reduction_op_var(CSageCodeGen &codegen, void *sg_scope,
                            void *target_var, vector<void *> &op_list,
                            vector<void *> &pntr_to_reduce);

bool anal_reduction(CSageCodeGen &codegen, CMarsIr *mars_ir, void *arr_pntr_ref,
                    void *curr_op, void *target_var, string scheme_str,
                    int &scheme_1_enabled, map<void *, int> &loop_is_reduct,
                    map<void *, int> &loop_is_parallel,
                    vector<void *> &vec_reduct_loops,
                    vector<void *> &vec_parallel_loops,
                    vector<void *> &loop_nest);
void reportWarnBenefit(CSageCodeGen &codegen, void *target_var, void *sg_loop);
