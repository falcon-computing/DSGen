#ifndef __KERNEL_SEPARATE_H__
#define __KERNEL_SEPARATE_H__
#include "cmdline_parser.h"
#include "file_parser.h"
#include "xml_parser.h"

#include "PolyModel.h"
#include "codegen.h"
#include "mars_message.h"
#include "mars_opt.h"
#include "tldm_annotate.h"

#include "mars_ir_v2.h"
#include "tldm_annotate.h"

#include "postwrap_process.h"

int kernel_separate_top(CSageCodeGen &codegen, void *pTopFunc,
                        CInputOptions &options);
#endif
