#pragma once

#include "cmdline_parser.h"
#include "file_parser.h"
#include "rose.h"
#include "xml_parser.h"

#include "bsuNode.h"
#include "codegen.h"
#include "mars_ir.h"
#include "mars_opt.h"
#include "program_analysis.h"

#include "stream_ir.h"

using namespace std;
using namespace MarsProgramAnalysis;
using namespace MerlinStreamModel;

int memory_partition_top(CSageCodeGen &codegen, void *pTopFunc,
                         CInputOptions &options);

int memory_partition_xilinx_top(CSageCodeGen &codegen, void *pTopFunc,
                                CInputOptions &options);

void insert_partition(CSageCodeGen &codegen, void *pTopFunc,
                      CInputOptions &options, CMarsIr *mars_ir,
                      map<CMirNode *, bool> &node_actions,
                      map<void *, map<int, int>> &p_results);

void partition_analysis(CSageCodeGen &codegen, CMarsIr *mars_ir,
                        map<CMirNode *, map<void *, map<int, int>>> &p_factors,
                        map<CMirNode *, bool> &node_actions,
                        map<void *, set<int>> &adjust_factors,
                        bool dual_option);

bool partition_analysis_node(CSageCodeGen &codegen, CMarsIr *mars_ir,
                             CMirNode *fNode, CMirNode *bNode,
                             map<void *, map<int, int>> &var2partition,
                             map<void *, set<int>> &adjust_factors,
                             bool dual_option);

void partition_merge(CSageCodeGen &codegen, CMarsIr *mars_ir,
                     map<CMirNode *, map<void *, map<int, int>>> &p_factors,
                     map<void *, map<int, int>> &p_results,
                     map<CMirNode *, bool> &node_actions,
                     map<void *, map<int, int>> &prior_factors,
                     bool dual_option);

void partition_evaluation(map<void *, map<int, int>> &var2partition);

void partition_transform(CSageCodeGen &codegen, CMarsIr *mars_ir,
                         map<void *, map<int, int>> &var2partition,
                         map<CMirNode *, bool> &node_actions,
                         map<void *, map<int, int>> &prior_factors,
                         map<void *, set<int>> &adjust_factors);

void partition_codegen(CSageCodeGen &codegen, void *arr_init, int dim,
                       int factor);

bool factor_extract(CMarsIr *mars_ir, void *sg_pragma,
                    map<int, int> &hls_factors);

void index_transform(CSageCodeGen &codegen, CMarsIr *mars_ir, CMirNode *bNode,
                     map<void *, map<int, int>> var2partition,
                     string transform_level = "low");

void reportPartitionMessage(CSageCodeGen &codegen,
                            map<void *, map<int, int>> var2partition);

void partial_partition_parsing(map<int, int> &factor_map,
                               vector<int> &arr_size);

bool total_bank_evalue(map<int, int> factor_map, int factor_t, int dim);

void local_bank_evalue(map<int, int> &factor_map, vector<int> &arr_size);

void delete_existing_pragma(CSageCodeGen &codegen, CMarsIr *mars_ir);

void print_partition(CSageCodeGen &codegen,
                     map<void *, map<int, int>> &var2partition);

bool reorganize_factor(CSageCodeGen &codegen, CMarsIr *mars_ir,
                       map<CMirNode *, map<void *, map<int, int>>> &p_factors,
                       map<void *, vector<map<int, int>>> &v_factors,
                       map<void *, map<int, int>> &prior_factors);

void choose_factor(vector<size_t> arr_size, vector<map<int, int>> &o_factors,
                   map<int, int> &m_factors, map<int, int> &p_factors);

void copy_factors(vector<map<int, int>> &original,
                  vector<map<int, int>> &target, vector<size_t> arr_size_org,
                  vector<size_t> arr_size_target, int partial_dim);

void copy_factors(map<int, int> &original, map<int, int> &target);

bool check_action(int f1, int f2);

void reportInvalidFactor(CSageCodeGen &codegen, CMirNode *node,
                         string str_factor);

void reportInvalidFactor1(CSageCodeGen &codegen, CMirNode *node, int old_factor,
                          int range);

int memory_partition_intel_top(CSageCodeGen &codegen, void *pTopFunc,
                               CInputOptions &options);

void partition_analysis_intel(CSageCodeGen &codegen, CMarsIr *mars_ir,
                              CStreamIR *stream_ir);

bool partition_analysis_var(CSageCodeGen &codegen, void *var, CMarsIr *mars_ir,
                            map<int, int> &m_factors);

bool analyze_channel(CSageCodeGen &codegen, void *var, void *scope,
                     map<int, int> &m_factors, int tmp_dim);

void insert_internal_pragma(CSageCodeGen &codegen,
                            map<void *, map<int, int>> p_channels,
                            map<void *, map<int, int>> p_buffers);

void partition_transform_intel(CSageCodeGen &codegen, CMarsIr *mars_ir,
                               CStreamIR *stream_ir);

bool parse_internal_pragma(CSageCodeGen &codegen, void *sg_pragma,
                           CMarsIr *mars_ir, CStreamIR *stream_ir,
                           map<void *, map<int, int>> &p_channels,
                           map<void *, map<int, int>> &p_buffers,
                           vector<void *> &ch_pragma);

bool transform_channel(CSageCodeGen &codegen, CMarsIr *mars_ir,
                       CStreamIR *stream_ir,
                       map<void *, map<int, int>> &p_channels,
                       vector<void *> &user_channels);

void transform_buffer(CSageCodeGen &codegen);

void delete_internal_pragma(CSageCodeGen &codegen, vector<void *> &ch_pragma);

bool parse_iterator_for_call(CSageCodeGen &codegen, CStreamIR *stream_ir,
                             void *sg_loop, vector<void *> &vec_new_expr,
                             map<CStreamBase *, set<int>> &new_attribute,
                             vector<void *> funcs, vector<void *> calls);

bool check_user_channel(CSageCodeGen &codegen, void *sg_ref,
                        vector<void *> vec_new_expr);

void reportDisableFGPIP(CSageCodeGen &codegen, CMirNode *node, void *arr);

void reportSuboptimalFGPIP(CSageCodeGen &codegen, CMirNode *node, void *arr);

void array_index_transform(CSageCodeGen &codegen, CMarsIr *mars_ir,
                           map<CMirNode *, bool> &node_actions,
                           map<void *, map<int, int>> &var2partition,
                           string transform_level = "low");

void dual_port_adjust(map<void *, map<int, int>> &p_results,
                      map<void *, int> v_size);

static const std::string VIVADO_str("HLS");
static const std::string AP_str("AP"); // previous vivado pragma
static const std::string ACCEL_str("ACCEL");
static const std::string UNROLL_str("unroll");
static const std::string PIPE_str("pipeline");
static const std::string PARTITION_str("array_partition");
static const std::string factor_str("factor");
static const std::string II_str("II");
static const std::string dim_str("dim");
static const std::string var_str("variable");
static const std::string type_str("type");
static const std::string HLS_TRIPCOUNT_str("LOOP_TRIPCOUNT");
