#pragma once

#include "mars_opt.h"
#include "math_gcd.h"
#include <list>
#include <map>
#define _DEBUG_PARTITION_

// Vivado_HLS partition factor threshold
#define THRESHOLD_XILINX 1024

bool partition_factor_anal(CSageCodeGen &codegen, CMarsIr *mars_ir,
                           CMirNode *fNode, CMirNode *bNode, int dim_val,
                           void *arr_init, int opt, int &factor, int &repeated,
                           map<void *, set<int>> &adjust_factors,
                           bool &dual_option);

void unroll_ref_generate(CSageCodeGen &codegen, int opt, void *curr_loop,
                         void *curr_ref,
                         vector<map<void *, int>> &index_expr_full,
                         vector<map<void *, int>> &index_expr_copy);

void unroll_ref_generate_full(
    CSageCodeGen &codegen, CMarsIr *mars_ir, int opt, void *curr_loop,
    map<void *, vector<map<void *, int>>> &index_expr_full,
    vector<map<void *, int>> &index_expr_copy, map<int, void *> &index_pos);

void remove_repeated_ref(CSageCodeGen &codegen,
                         vector<map<void *, int>> &index_expr,
                         map<int, int> &repeat_tag, void *sg_loop,
                         map<int, void *> index_pos);

bool conflict_detect_full(CSageCodeGen &codegen, int &bank_proc,
                          vector<map<void *, int>> index_expr,
                          map<int, int> repeat_tag, map<int, void *> index_pos,
                          int mod_size = 1);

void bank_renew(string prior, int &bank_proc);

void print_index(CSageCodeGen &codegen, vector<map<void *, int>> index_expr,
                 map<int, int> &repeat_tag);
void print_index(CSageCodeGen &codegen, vector<map<void *, int>> index_expr);

bool parse_full_accesses(CSageCodeGen &codegen, CMirNode *bNode, int dim,
                         void *arr_init,
                         map<void *, vector<map<void *, int>>> &index_expr_full,
                         int &mod_size, bool &self_assign);

bool check_loop_tripcount_pragma(CSageCodeGen &codegen, CMarsIr *mars_ir,
                                 void *sg_loop, int64_t &l_range);

int trace_up_to_loop_iterator(CSageCodeGen &codegen, void *ref, void *&sg_iter);

void index_trace_up(CSageCodeGen &codegen, void *scope, void *&ref);

void reportThresholdFactor(CSageCodeGen &codegen, CMirNode *node, void *arr,
                           int dim);
int has_self_assign_ref(CSageCodeGen &codegen, vector<void *> vec_refs);
