#ifndef MERLIN_ID_UPDATE_H
#define MERLIN_ID_UPDATE_H

#include "history.h"

vector<map<string, HistoryMarker>> loadHistories(const string &historyListLoc,
                                                 vector<string> &historyNames,
                                                 string historyLocPrefix = "");
string getUserCodeId(CSageCodeGen *ast, void *node, bool ignoreError = false);
string getUserCodeFileLocation(CSageCodeGen *ast, void *node,
                               bool include_brace = false);
string getQoRMergeMode(CSageCodeGen *ast, void *node);
void setFuncDeclUserCodeScopeId(CSageCodeGen *ast, void *funcDecl,
                                void *funcCall);

#endif /* MERLIN_ID_UPDATE_H */
