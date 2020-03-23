#pragma once

#include <map>
#include <string>
#include <vector>

#include "rose.h"
#include "mars_opt.h"
#include "mars_ir.h"

void reduction_general(CSageCodeGen *codegen, void *pTopFunc,
                       const CInputOptions &options);
void reduction_analysis(CSageCodeGen *codegen, void *pTopFunc,
                        const CInputOptions &options, CMarsIr *mars_ir);
void reduction_postprocessing(CSageCodeGen *codegen, void *pTopFunc,
                              CMarsIr *mars_ir);

bool build_reduction_general(CSageCodeGen *codegen,
                             const CInputOptions &options, CMarsIr *mars_ir,
                             void *target_var, void *curr_op, int lift_level,
                             std::vector<void *> pntr_to_reduce,
                             int scheme_1_enabled,
                             std::vector<void *> vec_reduct_loops,
                             std::vector<void *> vec_parallel_loops,
                             const std::map<void *, int> &loop_is_related,
                             const std::vector<void *> &loop_nest);

bool anal_reduction(CSageCodeGen *codegen, CMarsIr *mars_ir,
                    CMirNode *bNode, void *arr_pntr_ref,
                    void *curr_op, void *target_var, string scheme_str,
                    int *scheme_1_enabled, int *lift_level,
                    std::vector<void *> *vec_reduct_loops,
                    std::vector<void *> *vec_parallel_loops,
                    std::map<void *, int> *loop_is_related,
                    std::vector<void *> *loop_nest);

void insert_reduc_function(CSageCodeGen *codegen, string func_type, void *var,
                           void *sg_copy, void *sg_location, string buf_type,
                           std::map<void *, void *> *all_func_call_map);

void parsing_reduction_pragma(CSageCodeGen *codegen, CMirNode *bNode,
                              std::vector<void *> *reduc_vars,
                              string *scheme_str);

void replace_reference_by_var(CSageCodeGen *codegen, void *old_init,
                              void *new_init, void *sg_scope);

void check_reduction_op_var(CSageCodeGen *codegen, void *sg_scope,
                            void *target_var, std::vector<void *> *op_list,
                            std::vector<void *> *pntr_to_reduce);

void reportWarnBenefit(CSageCodeGen *codegen, void *target_var, void *sg_loop);
void reportVariableTripCount(CSageCodeGen *codegen, void *target_var,
                             void *sg_loop);

bool create_reduction_buf_and_refs(
    CSageCodeGen *codegen, int scheme_1_enabled,
    const vector<void *> &pntr_to_reduce, vector<void *> *new_decl,
    vector<vector<void *>> *new_decl_ln, vector<void *> *new_decl_local,
    vector<void *> *vec_new_refs_local, vector<void *> *vec_new_refs_local_init,
    vector<void *> *vec_old_refs, vector<void *> *vec_new_refs,
    vector<vector<void *>> *vec_new_refs_ln,
    vector<vector<void *>> *vec_new_refs_ln_init,
    const vector<void *> &vec_reduct_loops,
    const vector<void *> &vec_parallel_loops, const vector<void *> &loop_nest,
    const std::map<void *, int> &loop_is_related,
    map<void *, int> *loop_is_reduct, map<void *, int> *loop_is_parallel,
    void **insert_pos_local, void *insertLoop, void *target_var);

void display_scheme(CSageCodeGen *codegen, int scheme_1_enabled, void *curr_op);
