/************************************************************************************
 *  (c) Copyright 2014-2020 Falcon Computing Solutions, Inc. All rights
 *reserved.
 *
 *  This file contains confidential and proprietary information
 *  of Falcon Computing Solutions, Inc. and is protected under U.S. and
 *  international copyright and other intellectual property laws.
 *
 ************************************************************************************/

#ifndef TRUNK_SOURCE_OPT_TOOLS_INCLUDE_ARRAY_DELINEARIZE_H_
#define TRUNK_SOURCE_OPT_TOOLS_INCLUDE_ARRAY_DELINEARIZE_H_

#include <map>
#include <vector>
#include "codegen.h"
#include "program_analysis.h"
int analysis_delinearize(CSageCodeGen *ast, void *array_init, int dim,
                         std::vector<size_t> *dim_split_steps,
                         std::map<void *, size_t> *mapAlias2BStep,
                         bool altera_flow);
int analysis_delinearize(CSageCodeGen *ast, void *array_init, int dim,
                         std::vector<size_t> *dim_split_steps,
                         std::map<void *, size_t> *mapAlias2BStep,
                         bool altera_flow, bool *is_simple, void *offset,
                         void *scope);
void analysis_delinearize_postproc_alias_offset(
    CSageCodeGen *ast, void *array_init, int dim_idx, void *scope,
    std::vector<size_t> *merged_step_all_access,  //  input/output
    std::map<void *, size_t> *mapAlias2BStep);    //  output

int apply_delinearize(CSageCodeGen *ast, void *array_init, int dim,
                      const std::vector<size_t> &dim_split_steps,
                      std::map<void *, size_t> *mapAlias2BStep);
int apply_delinearize(
    CSageCodeGen *ast, void *array_init, int dim,
    const std::vector<size_t> &dim_split_steps,
    std::map<void *, size_t> *mapAlias2BStep, bool check_only, bool *is_simple,
    void *offset, void *scope,
    std::map<void *, std::vector<MarsProgramAnalysis::CMarsExpression>>
        *mapIndex2Delinear,
    std::vector<size_t> *new_array_dims);
int check_only_delinearize(
    CSageCodeGen *ast, void *array_init, int dim,
    const std::vector<size_t> &dim_split_steps,
    std::map<void *, size_t> *mapAlias2BStep,
    std::map<void *, std::vector<MarsProgramAnalysis::CMarsExpression>>
        *mapIndex2Delinear,
    std::vector<size_t> *new_array_dims);

#endif  // TRUNK_SOURCE_OPT_TOOLS_INCLUDE_ARRAY_DELINEARIZE_H_
