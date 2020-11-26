/************************************************************************************
 *  (c) Copyright 2014-2020 Falcon Computing Solutions, Inc. All rights reserved.
 *
 *  This file contains confidential and proprietary information
 *  of Falcon Computing Solutions, Inc. and is protected under U.S. and
 *  international copyright and other intellectual property laws.
 *
 ************************************************************************************/

#ifndef TRUNK_SOURCE_OPT_TOOLS_INCLUDE_ID_UPDATE_H_
#define TRUNK_SOURCE_OPT_TOOLS_INCLUDE_ID_UPDATE_H_
#include <string>
#include <vector>
#include <map>
#include "history.h"

struct UCodeInfo {
  const string name;
  const string file_location;
  explicit UCodeInfo(const string &na = "", const string &loc = "")
      : name(na), file_location(loc) {}
};

std::vector<std::map<std::string, HistoryMarker>>
loadHistories(const std::string &historyListLoc,
              std::vector<std::string> *p_historyNames,
              const std::string &historyLocPrefix = "");
std::string getUserCodeId(CSageCodeGen *ast, void *node,
                          bool ignoreError = false);
std::string getUserCodeFileLocation(CSageCodeGen *ast, void *node,
                                    bool include_brace = false);
UCodeInfo getUserCodeInfo(CSageCodeGen *cg, void *node);
std::string getQoRMergeMode(CSageCodeGen *ast, void *node);
void setFuncDeclUserCodeScopeId(CSageCodeGen *ast, void *funcDecl,
                                void *funcCall);

#endif  // TRUNK_SOURCE_OPT_TOOLS_INCLUDE_ID_UPDATE_H_
