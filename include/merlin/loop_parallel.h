#pragma once

#include "PolyModel.h"
#include "cmdline_parser.h"
#include "file_parser.h"
#include "xml_parser.h"

#include "codegen.h"
#include "mars_ir.h"
#include "mars_opt.h"

typedef std::map<int, int> dim_map;
typedef std::map<SgInitializedName *, dim_map> array_dim_map;

void LoopParallel(CSageCodeGen &codegen, void *pTopFunc, CInputOptions &options,
                  CMarsIr &mars_ir);
void insert_pipeline(CSageCodeGen &codegen, CMirNode *bNode, bool action);
void insert_unroll(CSageCodeGen &codegen, CMirNode *bNode);
bool is_unroll_dim(CMirNode *bNode, int dim_val, string arr_name,
                   bool full_tag);
void reportInproperDataLayout(CSageCodeGen &codegen, void *var);

void reportCoarseGrainedParallel(CSageCodeGen &codegen, CMirNode *new_node);

void reportCompleteUnroll(CSageCodeGen &codegen, void *sg_loop, int range);

void reportCompleteUnrollWarn(CSageCodeGen &codegen, CMirNode *node);

void reportUnrollFactor(CSageCodeGen &codegen, void *sg_loop, int factor);

void reportDataDependence(CSageCodeGen &codegen, void *var, void *sg_loop);

void reportPipeline(CSageCodeGen &codegen, void *sg_loop, string II_factor,
                    string msg_dep);

void reportNonCanonical(CSageCodeGen &codegen, CMirNode *node);

void reportIgnorePragmaDependence(CSageCodeGen &codegen, void *sg_pragma,
                                  void *var);

void removePragmaStatement(CMirNode *new_node, CSageCodeGen &codegen);

void add_loop_tripcount_pragma(CSageCodeGen &codegen, void *pTopFunc,
                               CInputOptions &options, CMarsIr &mars_ir);

void *test_false_dependency(CSageCodeGen &codegen, void *sg_loop,
                            void *sg_array);

bool collect_tripcount_pragma(CSageCodeGen &codegen, CMarsIr *mars_ir,
                              void *scope, void *&sg_bound_pragma,
                              int64_t &l_range);

void check_false_dependency_xilinx(CSageCodeGen &codegen, void *sg_pragma,
                                   void *target_var);

void check_false_dependency_xilinx(CSageCodeGen &codegen, void *pTopFunc,
                                   CInputOptions &options);

void check_dependency_msg(CSageCodeGen &codegen, CMirNode *bNode,
                          string &msg_dep);

bool preprocess(CSageCodeGen &codegen, void *pTopFunc, CInputOptions &options,
                CMarsIr &mars_ir);

void reportApplyPragmaDependence(CSageCodeGen &codegen, void *var,
                                 void *sg_loop, bool check_all = false);

void reportRiskyFalseDependence(CSageCodeGen &codegen, void *sg_pragma,
                                void *var);

void remove_loop_pragmas(CSageCodeGen &codegen, void *pTopFunc,
                         CInputOptions &options, CMarsIr &mars_ir);
