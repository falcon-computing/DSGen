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

void Reduction(CSageCodeGen &codegen, void *pTopFunc, CInputOptions &options);
bool insert_reduction(CSageCodeGen &codegen, void *pTopFunc, CMarsIr *mars_ir,
                      CMirNode *bNode, int node_num, string str_scheme,
                      set<void *> &rdc_buf);
bool anal_reduction(CSageCodeGen &codegen, CMirNode *bNode, CMirNode *cNode,
                    SgExpression *arr_ref);
int lift_location(CSageCodeGen &codegen, CMirNode *bNode, CMirNode *cNode,
                  vector<void *> pntr_to_reduce);
int lift_location_internal(CSageCodeGen &codegen, CMirNode *bNode,
                           CMirNode *cNode, SgExpression *arr_ref);
bool build_reduction(CSageCodeGen &codegen, CMirNode *bNode, CMirNode *cNode,
                     SgExpression *curr_op, int lift_level,
                     vector<void *> pntr_to_reduce, string str_scheme,
                     set<void *> &rdc_buf);
int get_ref_num(CSageCodeGen &codegen, void *sg_bb, void *arr_init);

bool check_loop_initializer(CSageCodeGen &codegen, void *sg_scope,
                            int lift_level);

void remove_reduction_pragma(CSageCodeGen &codegen, void *pTopFunc,
                             set<void *> rdc_buf);
void remove_labels(CSageCodeGen &codegen, void *pTopFunc);

void reportMessage(CSageCodeGen &codegen, CMirNode *cNode, void *arr_ref,
                   vector<void *> pntr_to_reduce);
void reportNoBenefit(CSageCodeGen &codegen, void *curr_op);
void reportUnCanonicalLoop(CSageCodeGen &codegen, void *loop);
void reportLoopLowerBound(CSageCodeGen &codegen, void *loop);
