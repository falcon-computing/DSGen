/************************************************************************************
 *  (c) Copyright 2014-2020 Falcon Computing Solutions, Inc. All rights reserved.
 *
 *  This file contains confidential and proprietary information
 *  of Falcon Computing Solutions, Inc. and is protected under U.S. and
 *  international copyright and other intellectual property laws.
 *
 ************************************************************************************/

#pragma once

#include "cmdline_parser.h"
#include "mars_ir.h"

int loop_flatten_for_pipeline(CSageCodeGen *codegen, void *pTopFunc,
                              const CInputOptions &options, CMarsIr *mars_ir);
