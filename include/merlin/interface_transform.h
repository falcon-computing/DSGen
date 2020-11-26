/************************************************************************************
 *  (c) Copyright 2014-2020 Falcon Computing Solutions, Inc. All rights reserved.
 *
 *  This file contains confidential and proprietary information
 *  of Falcon Computing Solutions, Inc. and is protected under U.S. and
 *  international copyright and other intellectual property laws.
 *
 ************************************************************************************/

#ifndef TRUNK_SOURCE_OPT_TOOLS_INCLUDE_INTERFACE_TRANSFORM_H_
#define TRUNK_SOURCE_OPT_TOOLS_INCLUDE_INTERFACE_TRANSFORM_H_
#include <vector>
#include <string>

#include "cmdline_parser.h"
#include "file_parser.h"
#include "xml_parser.h"

#include "PolyModel.h"
#include "codegen.h"
#include "mars_opt.h"
#include "tldm_annotate.h"

#include "mars_ir.h"
#include "mars_ir_v2.h"

#include "postwrap_process.h"

int rename_interface_name_top(CSageCodeGen *codegen, void *pTopFunc,
                              const CInputOptions &options);
int array_linearize_top(CSageCodeGen *codegen, void *pTopFunc,
                        const CInputOptions &options);
int data_transfer_gen_top(CSageCodeGen *codegen, void *pTopFunc,
                          const CInputOptions &options);
int struct_decompose_top(CSageCodeGen *codegen, void *pTopFunc,
                         const CInputOptions &options);
int access_range_gen_top(CSageCodeGen *codegen, void *pTopFunc,
                         const CInputOptions &options);
int remove_simple_pointer_alias_top(CSageCodeGen *codegen, void *pTopFunc,
                                    const CInputOptions &option);
int constant_propagation_top(CSageCodeGen *codegen, void *pTopFunc,
                             const CInputOptions &option,
                             bool assert_generation);
void bool2char_top(CSageCodeGen *codegen, void *pTopFunc,
                   const CInputOptions &options);
int check_non_static_pointer_interface_top(CSageCodeGen *codegen,
                                           void *pTopFunc,
                                           const CInputOptions &options);
int DetectMultiDimPointerInterfaceTop(CSageCodeGen *codegen, void *pTopFunc,
                                      const CInputOptions &options);
//  helper function

void *get_unique_wrapper_pragma(void *sg_init, CSageCodeGen *ast,
                                CMarsIrV2 *mars_ir);

vector<void *> get_unique_interface_pragma(void *sg_init, CSageCodeGen *ast,
                                           CMarsIrV2 *mars_ir);
vector<string> get_unique_interface_attribute(void *sg_init, string attribute,
                                              CSageCodeGen *ast,
                                              CMarsIrV2 *mars_ir);
int is_interface_related(vector<string> depths, void *kernel,
                         CSageCodeGen *ast);

int IsStructElement(string str, void *kernel, CSageCodeGen *ast);

int get_total_size_from_string(string s_depth);

int is_number_string(string str);
#endif  // TRUNK_SOURCE_OPT_TOOLS_INCLUDE_INTERFACE_TRANSFORM_H_
