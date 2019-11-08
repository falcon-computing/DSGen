#ifndef TRUNK_SOURCE_OPT_TOOLS_INCLUDE_ID_UPDATE_H_
#define TRUNK_SOURCE_OPT_TOOLS_INCLUDE_ID_UPDATE_H_
#include <string>
#include <vector>
#include <map>
#include "history.h"

std::vector<std::map<std::string, HistoryMarker>>
loadHistories(const std::string &historyListLoc,
              std::vector<std::string> *p_historyNames,
              const std::string &historyLocPrefix = "");
std::string getUserCodeId(CSageCodeGen *ast, void *node,
                          bool ignoreError = false);
std::string getUserCodeFileLocation(CSageCodeGen *ast, void *node,
                                    bool include_brace = false);
std::string getQoRMergeMode(CSageCodeGen *ast, void *node);
void setFuncDeclUserCodeScopeId(CSageCodeGen *ast, void *funcDecl,
                                void *funcCall);

#endif  // TRUNK_SOURCE_OPT_TOOLS_INCLUDE_ID_UPDATE_H_
