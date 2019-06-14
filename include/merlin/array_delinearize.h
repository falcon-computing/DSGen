#ifndef __ARRAY_DELINEARIZE_H__
#define __ARRAY_DELINEARIZE_H__
#include "codegen.h"
#include "program_analysis.h"

#include <map>
#include <vector>
using namespace MarsProgramAnalysis;
int analysis_delinearize(CSageCodeGen &codegen, void *array_init, int dim,
                         std::vector<size_t> &dim_split_steps,
                         std::map<void *, size_t> &mapAlias2BStep,
                         bool altera_flow);
int analysis_delinearize(CSageCodeGen &codegen, void *array_init, int dim,
                         std::vector<size_t> &dim_split_steps,
                         std::map<void *, size_t> &mapAlias2BStep,
                         bool altera_flow, bool &is_simple, void *offset,
                         void *scope);
void analysis_delinearize_postproc_alias_offset(
    CSageCodeGen &codegen, void *array_init, int dim_idx, void *scope,
    std::vector<size_t> &merged_step_all_access, // input/output
    std::map<void *, size_t> &mapAlias2BStep);   // output

int apply_delinearize(CSageCodeGen &codegen, void *array_init, int dim,
                      std::vector<size_t> &dim_split_steps,
                      std::map<void *, size_t> &mapAlias2BStep);
int apply_delinearize(CSageCodeGen &codgen, void *array_init, int dim,
                      std::vector<size_t> &dim_split_steps,
                      std::map<void *, size_t> &mapAlias2BStep, bool check_only,
                      bool &is_simple, void *offset, void *scope,
                      map<void *, vector<CMarsExpression>> &mapIndex2Delinear,
                      vector<size_t> &new_array_dims);
int check_only_delinearize(
    CSageCodeGen &ast, void *array_init, int dim,
    vector<size_t> &dim_split_steps, map<void *, size_t> &mapAlias2BStep,
    map<void *, vector<CMarsExpression>> &mapIndex2Delinear,
    vector<size_t> &new_array_dims);

#endif
