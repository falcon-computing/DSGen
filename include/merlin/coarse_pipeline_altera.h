#ifndef _COARSE_PIPELINE_H_INCLUDED_
#define _COARSE_PIPELINE_H_INCLUDED_

#include "rose.h"

#include "PolyModel.h"
#include "cmdline_parser.h"
#include "file_parser.h"
#include "mars_ir_v2.h"
#include "mars_opt.h"
#include "tldm_annotate.h"
#include "xml_parser.h"

using namespace std;
// using namespace SageInterface;
// using namespace SageBuilder;
using namespace MarsProgramAnalysis;

// New function definition
// void CoarsePipeline_altera(CSageCodeGen & codegen, void * pTopFunc,
// CInputOptions & options);

void assign_schedule_for_marsIR_nodes(CMarsIrV2 *mars_ir,
                                      bool detect_shared_memory = false);

// void generate_schedule(CSageCodeGen &codegen, CMarsIrV2 *mars_ir,
//                       vector<CMarsNode *> &t_nodes,
//                       //	vector<void*> &t_nodes,
//                       void *t_loop);
//
// bool GetNodeDependence(CSageCodeGen &codegen, CMarsIrV2 *mars_ir,
//                       vector<CMarsNode *> &t_nodes,
//                       //	vector<void*> &t_nodes,
//                       CMarsNode *write_node, CMarsNode *read_node, int num_w,
//                       int num_r, map<void *, map<void *, dep_type>> &dep_map,
//                       void *t_loop);

// bool check_all_schedule(CSageCodeGen &codegen, CMarsIrV2 *mars_ir);

void print_schedule(vector<CMarsNode *> t_nodes);

// void update_schedule(CSageCodeGen &codegen, CMarsIrV2 *mars_ir,
//                     vector<vector<CMarsNode *>> &t_nodes_vec);

// void init_schedule(CSageCodeGen &codegen, CMarsIrV2 *mars_ir);
////////// Range analysis

bool test_range_legality(CSageCodeGen &codegen, vector<CMarsNode *> &t_nodes,
                         //	vector<void*> &t_nodes,
                         void *sg_scope_w, void *sg_scope_r, void *sg_array,
                         int num_w, int num_r, std::string &msg);

bool test_range_legality(CSageCodeGen &codegen, vector<CMarsNode *> &t_nodes,
                         void *sg_scope_w, void *sg_scope_r, void *sg_array,
                         int num_w, int num_r, string &msg, void *t_loop);

bool check_range_flatten(CSageCodeGen &codegen, // vector<void*> &t_nodes,
                         vector<CMarsNode *> &t_nodes,
                         CMarsRangeExpr &flatten_mr_r,
                         CMarsRangeExpr &flatten_mr_w, void *sg_array,
                         int num_w, int num_r, std::string &msg, void *t_loop);

bool check_range_multiple(CSageCodeGen &codegen, // vector<void*> &t_nodes,
                          vector<CMarsNode *> &t_nodes,
                          vector<CMarsRangeExpr> &vec_mr_r,
                          vector<CMarsRangeExpr> &vec_mr_w, void *sg_array,
                          int num_w, int num_r, string &msg, void *t_loop);

bool check_range_bound_v2(vector<CMarsExpression> &lbr,
                          vector<CMarsExpression> &ubr,
                          vector<CMarsExpression> &lbw,
                          vector<CMarsExpression> &ubw, string &msg);

bool union_access_range(CSageCodeGen &codegen, // vector<void*> &t_nodes,
                        vector<CMarsNode *> &t_nodes, CMarsRangeExpr &mr_merged,
                        bool &merge_tag, void *sg_array, int num_w, int num_r,
                        std::string &msg, void *t_loop);

bool union_access_range_multiple(CSageCodeGen &codegen,
                                 vector<CMarsNode *> &t_nodes,
                                 // vector<void*> &t_nodes,
                                 CMarsRangeExpr &mr_merged, bool &merge_tag,
                                 void *sg_array, int num_w, int num_r,
                                 string &msg, int dim, void *t_loop);

#endif
