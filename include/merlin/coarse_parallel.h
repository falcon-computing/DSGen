#ifndef _COARSE_PARALLEL_H_INCLUDED_
#define _COARSE_PARALLEL_H_INCLUDED_

#include "rose.h"

#include "PolyModel.h"
#include "cmdline_parser.h"
#include "file_parser.h"
#include "mars_opt.h"
#include "memory_burst.h"
#include "stream_ir.h"
#include "tldm_annotate.h"
#include "xml_parser.h"

using namespace std;
// using namespace SageInterface;
// using namespace SageBuilder;
using namespace MarsProgramAnalysis;
using namespace MerlinStreamModel;

int coarse_parallel_intel_top(CSageCodeGen &codegen, void *pTopFunc,
                              CInputOptions &options);

int coarse_parallel_xilinx_top(CSageCodeGen &codegen, void *pTopFunc,
                               CInputOptions &options);

bool generate_parallel_xilinx(CSageCodeGen &codegen, CMarsIr *mars_ir,
                              CMirNode *node);

bool generate_parallel_intel(CSageCodeGen &codegen, CMarsIr *mars_ir,
                             CStreamIR *stream_ir, CMirNode *node,
                             MemoryBurst &burst_instance);
#endif
