#ifndef __INTERFACE_TRANSFORM_H__
#define __INTERFACE_TRANSFORM_H__
#include "cmdline_parser.h"
#include "file_parser.h"
#include "xml_parser.h"

#include "PolyModel.h"
#include "codegen.h"
#include "mars_opt.h"
#include "tldm_annotate.h"

#include "mars_ir.h"
#include "mars_ir_v2.h"
#include "tldm_annotate.h"

#include "postwrap_process.h"

int rename_interface_name_top(CSageCodeGen &codegen, void *pTopFunc,
                              CInputOptions &options);
int array_linearize_top(CSageCodeGen &codegen, void *pTopFunc,
                        CInputOptions &options);
int data_transfer_gen_top(CSageCodeGen &codegen, void *pTopFunc,
                          CInputOptions &options);
int struct_decompose_top(CSageCodeGen &codegen, void *pTopFunc,
                         CInputOptions &options);
int access_range_gen_top(CSageCodeGen &codegen, void *pTopFunc,
                         CInputOptions &options);
int remove_simple_pointer_alias_top(CSageCodeGen &codegen, void *pTopFunc,
                                    CInputOptions &option);
int constant_propagation_top(CSageCodeGen &codegen, void *pTopFunc,
                             CInputOptions &option);
void bool2char_top(CSageCodeGen &codegen, void *pTopFunc,
                   CInputOptions &options);
int check_non_static_pointer_interface_top(CSageCodeGen &codegen,
                                           void *pTopFunc,
                                           CInputOptions &options);
int DetectPointerCompareInterfaceTop(CSageCodeGen &codegen, void *pTopFunc,
                                     CInputOptions &options);
int DetectPointerTypeCastInterfaceTop(CSageCodeGen &codegen, void *pTopFunc,
                                      CInputOptions &options);
int DetectMultiDimPointerInterfaceTop(CSageCodeGen &codegen, void *pTopFunc,
                                      CInputOptions &options);
// helper function

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
#endif
