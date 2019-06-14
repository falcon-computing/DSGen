#ifndef _AUTO_PIPELINE_H_INCLUDED_
#define _AUTO_PIPELINE_H_INCLUDED_
#include "PolyModel.h"
#include "cmdline_parser.h"
#include "codegen.h"
#include "file_parser.h"
#include "mars_ir.h"
#include "mars_opt.h"
#include "tldm_annotate.h"
#include "xml_parser.h"

void AutoPipeline(CSageCodeGen &codegen, void *pTopFunc,
                  CInputOptions &options);
void auto_insert_pipeline(CMirNode *bNode, CSageCodeGen &codegen);

// static const std::string ACCEL_PRAGMA ("ACCEL");  //current version
// static const std::string PIPE_str ("pipeline");
#endif
