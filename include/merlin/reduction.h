/************************************************************************************
 *  (c) Copyright 2014-2020 Falcon Computing Solutions, Inc. All rights reserved.
 *
 *  This file contains confidential and proprietary information
 *  of Falcon Computing Solutions, Inc. and is protected under U.S. and
 *  international copyright and other intellectual property laws.
 *
 ************************************************************************************/

#pragma once

#include <vector>
#include <set>
#include <string>

#include "rose.h"
#include "mars_opt.h"
#include "mars_ir.h"

void reduction_run(CSageCodeGen *codegen, void *pTopFunc,
                   const CInputOptions &options);
bool insert_reduction(CSageCodeGen *codegen, void *pTopFunc, CMarsIr *mars_ir,
                      CMirNode *bNode, int node_num, string str_scheme,
                      set<void *> *rdc_buf);
bool anal_reduction(CSageCodeGen *codegen, CMirNode *bNode, CMirNode *cNode,
                    SgExpression *arr_ref);
int lift_location(CSageCodeGen *codegen, CMirNode *bNode, CMirNode *cNode,
                  std::vector<void *> pntr_to_reduce);
int lift_location_internal(CSageCodeGen *codegen, CMirNode *bNode,
                           CMirNode *cNode, SgExpression *arr_ref);
bool build_reduction(CSageCodeGen *codegen, CMirNode *bNode, CMirNode *cNode,
                     SgExpression *curr_op, int lift_level,
                     std::vector<void *> pntr_to_reduce, string str_scheme,
                     set<void *> *rdc_buf);
int get_ref_num(CSageCodeGen *codegen, void *sg_bb, void *arr_init);

bool check_loop_initializer(CSageCodeGen *codegen, void *sg_scope,
                            int lift_level);

void remove_reduction_pragma(CSageCodeGen *codegen, void *pTopFunc,
                             set<void *> rdc_buf);
void remove_labels(CSageCodeGen *codegen, void *pTopFunc);

void reportMessage(CSageCodeGen *codegen, CMirNode *cNode, void *arr_ref,
                   std::vector<void *> pntr_to_reduce);
void reportNoBenefit(CSageCodeGen *codegen, void *curr_op);
void reportUnCanonicalLoop(CSageCodeGen *codegen, void *loop);
void reportLoopLowerBound(CSageCodeGen *codegen, void *loop);
