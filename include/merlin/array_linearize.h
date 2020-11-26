/************************************************************************************
 *  (c) Copyright 2014-2020 Falcon Computing Solutions, Inc. All rights
 *reserved.
 *
 *  This file contains confidential and proprietary information
 *  of Falcon Computing Solutions, Inc. and is protected under U.S. and
 *  international copyright and other intellectual property laws.
 *
 ************************************************************************************/

#pragma once

#include "PolyModel.h"
#include "cmdline_parser.h"
#include "codegen.h"
#include "file_parser.h"
#include "mars_ir_v2.h"
#include "mars_message.h"
#include "mars_opt.h"
#include "tldm_annotate.h"
#include "xml_parser.h"
//  string get_task_name_by_kernel(CSageCodeGen * ast, void * kernel);
//  string get_wrapper_name_by_kernel(CSageCodeGen * ast, void * kernel);
int check_interface_pragma(CSageCodeGen *codegen, void *pTopFunc, bool);
