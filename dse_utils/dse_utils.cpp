#include <string>
#include <vector>
#include <sstream>
#include <set>
#include <map>
#include <stdlib.h>
#include <stdio.h>
#include <algorithm>
#include "codegen.h"
#include "mars_ir_v2.h"
#include "dse_utils.h"

using namespace MarsProgramAnalysis;
using namespace std;

string getDesignSpaceParamName(string pragmaStr) {
  if (pragmaStr.find("options:") == string::npos)
    return "";
  string str = pragmaStr.substr(pragmaStr.find("options:"));
  if (str.find("=") == string::npos)
    return "";
  str = str.substr(8, str.find("=") - 8);
  str.erase(remove(str.begin(), str.end(), ' '), str.end());
  return str;
}

vector<string> getShadowedPipelineParams(CSageCodeGen &m_ast, void *loop) {
  vector<string> ret;
  void *stmt = m_ast.GetParent(loop);
  while (stmt) {
    if (m_ast.IsLoopStatement(stmt)) {
      void *body = m_ast.GetLoopBody(stmt);
      for (int i = 0; i < m_ast.GetChildStmtNum(body); ++i) {
        void *child = m_ast.GetChildStmt(body, i);
        if (!m_ast.IsPragmaDecl(child))
          continue;
        string ori_str = m_ast.GetPragmaString(child);
        string str = ori_str;
        transform(str.begin(), str.end(), str.begin(), ::tolower);
        if (str.find("pipeline") == string::npos ||
            str.find("flatten") == string::npos) {
          continue;
        }
        ret.push_back(getDesignSpaceParamName(ori_str));
      }
    }
    stmt = m_ast.GetParent(stmt);
  }
  return ret;
}

void insertLoopDesignSpace(CSageCodeGen &m_ast,
                           void (*inserter)(CSageCodeGen &, void *, string),
                           void *stmt, string loop_code) {

  if (m_ast.IsForStatement(stmt) || m_ast.IsWhileStatement(stmt))
    (*inserter)(m_ast, stmt, loop_code);

  int loop_idx = 0;
  vector<void *> vecBody;

  // Collect all child basic blocks
  // FIXME: Other blocks?
  if (m_ast.IsForStatement(stmt) || m_ast.IsWhileStatement(stmt))
    vecBody.push_back(m_ast.GetLoopBody(stmt));
  else if (m_ast.IsIfStatement(stmt)) {
    vecBody.push_back(m_ast.GetIfStmtTrueBody(stmt));
    vecBody.push_back(m_ast.GetIfStmtFalseBody(stmt));
  } else {
    cout << "unknown statement " << m_ast._p(stmt) << endl;
    return;
  }

  for (auto body : vecBody) {
    int num_body = m_ast.GetChildStmtNum(body);
    for (auto i = 0; i < num_body; i++) {
      void *ch = m_ast.GetChildStmt(body, i);
      if (m_ast.IsLabelStatement(ch))
        ch = m_ast.GetStmtFromLabel(ch);
      if (m_ast.IsForStatement(ch) || m_ast.IsWhileStatement(ch) ||
          m_ast.IsIfStatement(ch)) {
        insertLoopDesignSpace(m_ast, inserter, ch,
                              loop_code + "_" + to_string(loop_idx++));
        num_body++;
        i++;
      }
    }
  }
  return;
}

bool isFineGrainedLoop(CSageCodeGen &m_ast, void *loop) {
  // Fine-grained if
  // - Innermost loop,
  // - Not innermost loop, but all its sub-loops are flatten
  // NOTE: Do not consider function calls
  vector<void *> vecSubLoops;
  m_ast.GetNodesByType(m_ast.GetLoopBody(loop), "preorder", "SgForStatement",
                       &vecSubLoops);

  if (vecSubLoops.size() == 0)
    return true;

  for (auto subLoop : vecSubLoops) {
    void *body = m_ast.GetLoopBody(subLoop);
    bool hasParallelPragma = false;
    for (int i = 0; i < m_ast.GetChildStmtNum(body); i++) {
      void *stmt = m_ast.GetChildStmt(body, i);
      if (!m_ast.IsPragmaDecl(stmt))
        continue;
      string str = m_ast.GetPragmaString(stmt);
      transform(str.begin(), str.end(), str.begin(), ::tolower);
      if (str.find("parallel") == string::npos)
        continue;
      hasParallelPragma = true;

      if (str.find("factor") != string::npos) {
        cout << "Loop " << m_ast._p(loop) << "is not fine-grained ";
        cout << "because of " << str << endl;
        return false;
      }
    }
    if (!hasParallelPragma)
      return false;
  }
  return true;
}

bool isFlattenFunction(CSageCodeGen &m_ast, void *func) {
  void *blk = m_ast.GetFuncBody(func);
  vector<void *> vecLoops;
  m_ast.GetNodesByType(blk, "preorder", "SgForStatement", &vecLoops);

  // We define the loop with tripcount smaller than 10 is a flatable loop.
  // A function with only flatable loops is defined as a flatten function.
  // This definition is only used for creating the design space, since this is
  // not a rigorous definition.
  for (auto loop : vecLoops) {
    CMarsRangeExpr mr = CMarsVariable(loop, &m_ast, func).get_range();
    CMarsExpression lb, ub;
    int ret = mr.get_simple_bound(lb, ub);
    if (!ret)
      return false;
    CMarsExpression range = ub - lb + 1;
    if (!range.IsConstant())
      return false;
    int ds = range.get_const();
    if (ds >= 10)
      return false;
  }
  return true;
}

bool hasNonFlattenFuncCall(CSageCodeGen &m_ast, void *loop) {
  void *blk = m_ast.GetLoopBody(loop);
  vector<void *> vecCalls;
  m_ast.GetNodesByType(blk, "preorder", "SgFunctionCallExp", &vecCalls);

  for (auto call : vecCalls) {
    if (m_ast.GetFuncDeclByCall(call)) {
      if (!isFlattenFunction(m_ast, m_ast.GetFuncDeclByCall(call)))
        return true;
    }
  }
  return false;
}

bool IsLoopStatement(CSageCodeGen &m_ast, void *loop) {
  if (!m_ast.IsForStatement(loop) && !m_ast.IsWhileStatement(loop) &&
      !m_ast.IsDoWhileStatement(loop))
    return false;
  return true;
}

bool IsOutermostForLoop(CSageCodeGen &m_ast, void *loop) {
  if (!IsLoopStatement(m_ast, loop))
    return false;

  while (loop) {
    loop = m_ast.GetParent(loop);
    if (IsLoopStatement(m_ast, loop))
      return false;
  }
  return true;
}

void getSubPragmas(CSageCodeGen &m_ast, void *blk, set<void *> *setPragmas) {
  vector<void *> vecPragmas;
  m_ast.GetNodesByType(blk, "preorder", "SgPragmaDeclaration", &vecPragmas);
  for (auto pragma : vecPragmas) {
    if (m_ast.GetPragmaString(pragma).find("ACCEL") != string::npos)
      setPragmas->insert(pragma);
  }

  vector<void *> vecCalls;
  m_ast.GetNodesByType(blk, "preorder", "SgFunctionCallExp", &vecCalls);
  for (auto call : vecCalls) {
    if (m_ast.GetFuncDeclByCall(call))
      getSubPragmas(m_ast, m_ast.GetFuncBody(m_ast.GetFuncDeclByCall(call)),
                    setPragmas);
  }
  return;
}

string getLoopHierfromParallelPragma(CSageCodeGen &m_ast, void *pragma) {
  string str = m_ast.GetPragmaString(pragma);
  transform(str.begin(), str.end(), str.begin(), ::tolower);
  if (str.find("_l_") == string::npos || str.find("(") == string::npos)
    return "";

  int len = str.find("(") - str.find("_l_") - 3;
  str = str.substr(str.find("_l_") + 3, len);
  string hier = str.substr(0, str.find("_"));

  return hier;
}

unordered_map<string, int>
createWeightedCallgraphForFunction(CSageCodeGen &m_ast, void *func_body) {
  unordered_map<string, int> weights;
  if (func_body == NULL || m_ast.GetChildStmtNum(func_body) <= 0)
    return weights;

  vector<void *> func_calls;
  m_ast.GetNodesByType(func_body, "preorder", "SgFunctionCallExp", &func_calls);
  for (auto func_call : func_calls) {
    void *stmt = func_call;
    int weight = 1;
    while ((stmt = m_ast.GetParent(stmt)) != func_body) {
      if (m_ast.IsForStatement(stmt)) {
        int64_t trip_count = 1;
        if (!m_ast.get_loop_trip_count(stmt, trip_count) || trip_count < 1)
          continue;
        void *loop_body = m_ast.GetLoopBody(stmt);
        for (int i = 0; i < m_ast.GetChildStmtNum(loop_body); i++) {
          void *body_stmt = m_ast.GetChildStmt(loop_body, i);
          if (m_ast.IsPragmaDecl(body_stmt)) {
            string pragma_str = m_ast.GetPragmaString(body_stmt);
            // transform the pragma string to its lower-case equivalent
            transform(pragma_str.begin(), pragma_str.end(), pragma_str.begin(),
                      ::tolower);
            if (pragma_str.find(" unroll") != string::npos) {
              weight *= trip_count;
              break;
            }
          }
        }
      }
    }
    string func_name = m_ast.GetFuncNameByCall(func_call);
    if (weights.count(func_name) == 0)
      weights[func_name] = weight;
    else
      weights[func_name] = max(weights[func_name], weight);
  }

  return weights;
}
bool GetUnrollFactorFromPragma(CSageCodeGen &codegen, void *loop,
                               int64_t &factor) {
  int64_t trip_count = 0;
  int64_t CurrentLoopTC = 0;
  codegen.get_loop_trip_count(loop, trip_count, &CurrentLoopTC);
  void *pre_stmt;
  void *label = codegen.GetParent(loop);
  if (codegen.IsLabelStatement(label)) {
    pre_stmt = codegen.GetPreviousStmt(label);
  } else {
    pre_stmt = codegen.GetPreviousStmt(loop);
  }
  if (codegen.IsPragmaDecl(pre_stmt)) {
    string pragma_s = codegen.GetPragmaString(pre_stmt);
    vector<string> v_s = str_split(pragma_s, ' ');
    if (v_s.size() > 0) {
      if (v_s[0] != "unroll") {
        return 0;
      }
      if (v_s.size() == 1) {
        factor = trip_count;
      } else if (v_s.size() == 2) {
        factor = atoi(v_s[1].c_str());
      } else {
        factor = trip_count;
      }
      return 1;
    }
  }
  return 0;
}
