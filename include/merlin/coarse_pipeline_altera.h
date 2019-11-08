#ifndef TRUNK_SOURCE_OPT_TOOLS_INCLUDE_COARSE_PIPELINE_ALTERA_H_
#define TRUNK_SOURCE_OPT_TOOLS_INCLUDE_COARSE_PIPELINE_ALTERA_H_
#include <vector>
#include <string>

#include "rose.h"
#include "mars_opt.h"
#include "mars_ir_v2.h"
using MarsProgramAnalysis::CMarsAccess;
using MarsProgramAnalysis::CMarsExpression;
using MarsProgramAnalysis::CMarsRangeExpr;

void assign_schedule_for_marsIR_nodes(CMarsIrV2 *mars_ir,
                                      bool detect_shared_memory = false);

void print_schedule(vector<CMarsNode *> t_nodes);

#endif  // TRUNK_SOURCE_OPT_TOOLS_INCLUDE_COARSE_PIPELINE_ALTERA_H_
