#pragma once

#include <rose.h>
#include "CallGraph.h"

#include "PolyModel.h"
#include <file_parser.h>

#include "history.h"
#include "id_update.h"
#include "report.h"

#include <boost/optional.hpp>

#if PROJDEBUG
#define XDEBUG(stmts)                                                          \
  do {                                                                         \
    stmts;                                                                     \
  } while (false)
#else
#define XDEBUG(stmts)
#endif
#if 0 // control log
#define MERLIN_ALGO(x)                                                         \
  cout << "[MERLIN ALGO][" << __func__ << ", " << __LINE__ << "]" << x << endl;
#else
#define MERLIN_ALGO(x)
#endif

extern bool g_muteMERLINError;
#if 0
#define MERLIN_ERROR(x)                                                        \
  if (!g_muteMERLINError) {                                                    \
    cerr << "[MERLIN ERROR][" << __func__ << ", " << __LINE__ << "]" << x      \
         << endl;                                                              \
  } else {                                                                     \
    cerr << "[MERLIN WARNING][" << __func__ << ", " << __LINE__ << "]" << x    \
         << endl;                                                              \
  }

#define MERLIN_ERROR_COND(cond, x)                                             \
  if (!g_muteMERLINError && cond) {                                            \
    cerr << "[MERLIN ERROR][" << __func__ << ", " << __LINE__ << "]" << x      \
         << endl;                                                              \
  } else {                                                                     \
    cerr << "[MERLIN WARNING][" << __func__ << ", " << __LINE__ << "]" << x    \
         << endl;                                                              \
  }
#else
#define MERLIN_ERROR(x)
#define MERLIN_ERROR_COND(cond, x)
#endif

typedef void (*t_func_rose_simple_traverse)(void *sg_node, void *pArg);
typedef void *(*t_func_trace_up)(void *sg_node, void *pArg);

// typedef map<void *, void *> t_func_call_path;
// pair<func_decl, func_call>
typedef list<pair<void *, void *>> t_func_call_path;

template <typename T> class SetVector {
  std::set<T> m_set;
  std::vector<T> m_vec;

public:
  SetVector() {}
  void clear() {
    m_set.clear();
    m_vec.clear();
  }
  void insert(T t) {
    if (m_set.count(t) > 0)
      return;
    m_set.insert(t);
    m_vec.push_back(t);
  }
  size_t count(T t) { return m_set.count(t); }
  void erase(T t) {
    for (auto I = m_vec.begin(); I != m_vec.end(); ++I)
      if (*I == t) {
        m_vec.erase(I);
        break;
      }
    m_set.erase(t);
  }
  // Determine if the SetVector is empty or not.
  bool empty() const { return m_vec.empty(); }

  /// Determine the number of elements in the SetVector.
  size_t size() const { return m_vec.size(); }

  /// Get an iterator to the beginning of the SetVector.
  auto begin() { return m_vec.begin(); }

  /// Get a const_iterator to the beginning of the 	SetVector.
  auto begin() const { return m_vec.begin(); }

  /// Get an iterator to the end of the SetVector.
  auto end() { return m_vec.end(); }

  /// Get a const_iterator to the end of the SetVector.
  auto end() const { return m_vec.end(); }
  template <class InputIterator> void insert(InputIterator S, InputIterator E) {
    InputIterator I = S;
    while (I != E) {
      insert(*I++);
    }
  }
};
class CMerlinMessage {
protected:
  int m_code;
  string m_msg;

public:
  CMerlinMessage() {}
  CMerlinMessage(int code, string msg) {
    m_code = code;
    m_msg = msg;
  }
  int get_code() { return m_code; }
  string get_message() { return m_msg; }
};

// An AST visiter that accepts C++11 lambdas.
template <typename Fn> class CSageWalker : public AstSimpleProcessing {
  Fn lambda;

public:
  CSageWalker(Fn &&lambda) : lambda(lambda) {}
  void visit(SgNode *node) override { lambda(*node); }
};

class CSageCodeGen_TraverseSimple : public AstSimpleProcessing {
public:
  CSageCodeGen_TraverseSimple() {
    m_func = 0;
    m_pArg = 0;
  }
  virtual void visit(SgNode *sgnode) {
    assert(m_func);
    assert(m_pArg);
    (*m_func)(sgnode, m_pArg);
    return;
  }
  void setFuncArg(t_func_rose_simple_traverse pFunc, void *pArg) {
    m_func = pFunc;
    m_pArg = pArg;
  }

protected:
  t_func_rose_simple_traverse m_func;
  void *m_pArg;
};

class CSageCodeGen {
  enum DIM_SIZE {
    EMPTY_SIZE = -1,
    UNKNOWN_SIZE = -2,
    POINTER_SIZE = 0,
  };

public:
  bool IsUnknownSize(size_t dim) { return dim == ((size_t)UNKNOWN_SIZE); }
  bool IsDeterminedSize(size_t dim) {
    return dim != ((size_t)UNKNOWN_SIZE) && dim != ((size_t)POINTER_SIZE) &&
           dim != ((size_t)EMPTY_SIZE);
  }

public:
  CSageCodeGen(SgProject *sg = nullptr);
  CSageCodeGen(string sFileName);
  ~CSageCodeGen();
  /*********** Analyze json file ************/
  //  vector<string> GetFirstDim(string file_name);
  /*********** COMPILER OPTION ******/
  map<string, string> get_input_macro() { return m_inputMacros; }
  /*********** FILE ************/
  void *CreateSourceFile(string sFileName, void *bindNode = nullptr);
  void *GetProject() { return m_sg_project; }
  void *OpenSourceFile(vector<string> sFileName, string cflags = "");
  // TODO
  int GetGlobalNum() { return m_sg_project->numberOfFiles(); }
  void *GetGlobal(int idx);
  // global scope
  void *GetGlobal(void *sg_pos);
  vector<void *> GetGlobals();
  // TODO END
  // File Info
  string GetFileInfo_filename(void *sgnode, int simple = 0);

  int GetFileInfo_line(void *sgnode) {
    if (!sgnode)
      return 0;
    return ((SgNode *)sgnode)->get_file_info()->get_line();
  }
  int GetFileInfo_col(void *sgnode) {
    if (!sgnode)
      return 0;
    return ((SgNode *)sgnode)->get_file_info()->get_col();
  }
  string get_file(void *sgnode);
  int get_line(void *sgnode);
  int get_col(void *sgnode);
  string get_location(void *sgNode, bool simple, bool line_num);
  string get_location_from_metadata(void *sgNode);
  int IsMixedCAndCPlusPlusDesign();
  int IsFromInputFile(void *sgnode);
  int IsSystemFunction(void *sg_func);

  bool isWithInCppFile(void *sgnode);
  bool isCppLinkage(void *sgnode);
  bool isWithInHeaderFile(void *sgnode);

  bool IsNULLFile(void *sg_exp_or_stat);
  bool IsCompilerGenerated(void *sg_exp_or_stat);
  bool IsTransformation(void *sg_exp_or_stat);

  /********* VARIABLE *****************/
  /**** Get variable info *****/
  vector<string> GetOpenCLName();
  void *GetVariableSymbol(void *sg_var_ref_); // NOT SUGGESTED, needreview
  void *GetVariableInitializedName(void *sg_var_ref_decl);
  void *GetVariableDefinition(void *decl_, void *name_ = nullptr);
  void GetVariableInitializedName(void *sg_var_ref_decl,
                                  vector<void *> &vec_var);
  void *find_interface_by_name(string var, void *kernel);
  void *find_variable_by_name(
      string var, void *pos,
      bool inclusive_scope =
          false); // ZP: 20160901 new function for pragma analysis
  bool check_valid_field_name(string var, void *pos);
  void *GetInitializer(void *sg_name);
  void *GetInitializerOperand(void *sg_input);
  void *GetVariableDecl(void *sg_var_ref_or_name);
  void *GetVariableDefinitionByName(void *name_);
  void *get_actual_var_decl_by_ref(void *var_ref);
  void *
  TraceVariableInitializedName(void *sg_name); // trace up cross function calls
  string GetVariableTypeName(void *sg_var_);
  string GetVariableName(void *sg_var_, bool qualified = false);
  bool IsStatic(void *sg_decl_);
  bool IsMutable(void *sg_decl_);
  bool IsExtern(void *sg_decl_);
  // storage modifier
  int is_register(void *var_decl);
  void *GetTypebyVar(void *var_init_name);
  int IsLocalInitName(void *sg_node);
  int IsForwardInitName(void *sg_node);
  int IsGlobalInitName(void *sg_node);
  int IsArgumentInitName(void *sg_node);
  bool IsMemberVariable(void *var_initname);

  /***** updated variable info ******/
  void SetTypetoVar(void *var_init_name, void *sg_type);
  void SetInitializer(void *sg_name, void *sg_init);
  void SetVariableName(void *sg_var_, string new_name);
  void SetStatic(void *sg_decl_);
  void UnSetStatic(void *sg_decl_);
  void SetExtern(void *sg_decl_);
  void set_register(void *var_decl);
  void unset_register(void *var_decl);
#if USED_CODE_IN_COVERAGE_TEST
  void SetMutable(void *sg_decl_);
#endif

  /**** ceated variable *******/
  void *CreateVariable(string sType, string sVar, void *bindNode = nullptr);
  void *CreateVariable(void *sg_type, string sVar, void *bindNode = nullptr);
  void *CreateVariableDecl(string sType, string sVar, void *sg_init_exp_,
                           void *sg_scope_, void *bindNode = nullptr);
  void *CreateVariableDecl(void *sg_type_, string sVar, void *sg_init_exp_,
                           void *sg_scope_, void *bindNode = nullptr);
  void *CreateInitializer(void *sg_init_exp_, void *bindNode = nullptr);

  /******** FUNCTION *******************/
  /***** get function info ***********/
  bool UnSupportedMemberFunction(void *func);
  bool IsMemberFunction(void *func_decl);
  int IsSupportedFunction(void *sg_func);
  int IsVarArgFunction(void *func_decl);

  string GetFuncName(void *sg_decl_, bool qualified = false);
  string DemangleName(string name);
  string GetMangledFuncName(void *sg_decl_);
  int GetFuncParamNum(void *sg_func_decl_);
  void *GetFuncParam(void *sg_func_decl_,
                     int i); // the SgInitializedName object
  vector<void *> GetFuncParams(void *sg_func_decl_);
  int GetFuncParamIndex(void *sg_arg, void *sg_scope = nullptr);
  void *GetFuncParamFromCallArg(void *sg_arg);
  void *GetFuncReturnType(void *sg_func_decl_);
  vector<void *> GetFuncReturnStmt(void *sg_scope);
  void *GetFuncBody(void *sg_func_decl_);
  void *GetFuncDeclByCall(void *sg_func_call_, int require_body = 1);
  void *GetFuncDeclByRef(void *sg_func_ref);
  void *GetFuncDeclByName(string sFuncName, int require_body = 1);
  void *GetFuncDeclByDefinition(void *func_def);
  void GetAllFuncDeclToCall(const set<void *> &funcs,
                            map<void *, vector<void *>> &func2calls);
  void *GetAssociatedFuncDeclByCall(void *sg_func_call_); // not suggested
  vector<void *> GetAllFuncDeclByCall(void *sg_func_call_,
                                      int require_body = 0);
  vector<void *> GetAllFuncDeclByName(string sFuncName, int require_body = 0);
  vector<void *> GetAllFuncCallByName(string sFuncName);
  vector<void *> GetAllFuncDecl(); // get all the function decls with body
  vector<void *> GetAllFuncDecl(void *func_decl); // get all the function decls
  // pointed to the same
  // functions

  void GetSubFuncDeclsRecursively(void *sg_func_decl, set<void *> &sub_decls,
                                  set<void *> &sub_calls,
                                  bool required_body = true);

  string GetFuncNameByCall(void *sg_func_call_, bool qualified = false);
  string GetFuncNameByRef(void *sg_func_ref_, bool qualified = false);
  void *GetFuncReturnTypeByCall(void *sg_func_call_);
  void *GetUniqueFuncDeclByBody(void *sg_func_body);
#if USED_CODE_IN_COVERAGE_TEST
  void *GetUniqueFuncDeclByCall(void *sg_call);
#endif
  void *GetFuncDefinitonByDecl(void *sg_func_decl);
  void *GetUniqueFuncDecl(void *sg_func_decl);

  /****** update function info **********/
  int SetFuncName(void *sg_decl_, string new_name);
  void SetFuncReturnType(void *func_decl, void *sg_type);
  int SetFuncBody(void *sg_func_decl_, void *sg_body);
  void insert_param_to_decl(void *decl, void *name, bool to_all_decl = false);

  /***** create function info **********/
  void *CreateFuncParamList(vector<void *> param_list,
                            void *bindNode = nullptr); // not suggested
  void *CreateFuncDecl(string sType, string sFuncName, vector<void *> fp_list,
                       void *sg_scope_, bool definition, void *bindNode);
  void *CreateFuncDecl(void *ret_type, string sFuncName, vector<void *> fp_list,
                       void *sg_scope_, bool definitio, void *bindNode);
  void *CreateFuncDecl(string sType, string sFuncName, void *sg_fp_list_,
                       void *sg_scope_, bool definition, void *bindNode);
  void *CreateFuncDecl(string sType, string sFuncName, vector<string> vec_name,
                       vector<string> vec_type, void *sg_scope, bool definition,
                       void *bindNode);
  void *CreateFuncDecl(void *ret_type, string sFuncName,
                       vector<string> vec_name, vector<void *> vec_type,
                       void *sg_scope, bool definitione, void *bindNode);

  /******** clone function info ***********/
  void *CloneFuncDecl(void *func_decl, void *sg_scope, bool definition = true,
                      bool bind = true);
  void *CloneFuncDecl(void *func_decl, void *sg_scope, string new_func_name,
                      bool definition = true);

  // Given a function declaration, clone each of its SgInitializedName
  // parameters. Returns a vector of the cloned parameters and a mapping from
  // old to new parameters.
  // TODO: New parameters may need to bound, similar to what CloneFuncDecl does.
  pair<vector<SgInitializedName *>,
       map<SgInitializedName *, SgInitializedName *>>
  cloneFuncParams(SgFunctionDeclaration &);

  // Clones a func decl like CloneFuncDecl, except that the clone will be given
  // the parameter list specified by the vector argument, and all references to
  // old params in the func body will be remapped according to the map argument.
  SgFunctionDeclaration &
  cloneFuncWith(SgFunctionDeclaration &, SgScopeStatement &, SgType &,
                const string &, const vector<SgInitializedName *> &,
                const map<SgInitializedName *, SgInitializedName *> &);
  void *
  CopyFuncDecl(void *sg_decl_,
               string sFuncName); // only support function with no argument

  /******* FUNCTION CALL ********************/
  /****** get function call info ****************/
  void GetFuncCallsFromDecl(void *sg_func_decl, void *sg_scope,
                            vector<void *> &vec_calls);
  void GetFuncCallsByName(string s_name, void *sg_scope,
                          vector<void *> &vec_calls);
  int GetFuncCallParamNum(void *sg_func_call_);
  void *GetFuncCallParam(void *sg_func_call_,
                         int i); // the SgInitializedName object
  vector<void *> GetFuncCallParamList(void *sg_func_call_);
  int GetFuncCallParamIndex(void *sg_arg);
  int GetFuncCallParamIndexRecur(void *sg_arg);
  int is_altera_channel_read(void *ref);
  int is_altera_channel_write(void *ref);
  int is_altera_channel_call(void *call);
  int is_merlin_channel_read(void *ref);
  int is_merlin_channel_write(void *ref);
  int is_merlin_channel_call(void *call);
  int IsFunctionPointerCall(void *sg_node) {
    SgFunctionCallExp *sg_call = isSgFunctionCallExp((SgNode *)sg_node);
    if (!sg_call)
      return 0;
    void *sg_decl = GetAssociatedFuncDeclByCall(sg_call);
    return nullptr == sg_decl;
  }
  int IsSupportedMemberFunctionCall(void *sg_node);

  bool IsAssertFailCall(void *sg_call);
  bool IsMemCpy(void *sg_call);
  // parse the memcpy function
  int parse_memcpy(void *memcpy_call, void *&length_exp, void *&sg_array1,
                   void *&sg_pntr1, vector<void *> &sg_index1,
                   int &pointer_dim1, void *&sg_array2, void *&sg_pntr2,
                   vector<void *> &sg_index2, int &pointer_dim2,
                   void *pos = nullptr);

  int is_memcpy_with_small_const_trip_count(void *memcpy_call, int threshold);

  /******** update function call ***********************/
  void insert_argument_to_call(void *call, void *exp);
  /***** create function call *******************/
  void *CreateFuncCall(string sFunc, void *sg_ret_type_,
                       vector<void *> param_list_, void *sg_scope_,
                       void *bindNode = nullptr);
  void *CreateFuncCall(void *sg_exp_, vector<void *> param_list_,
                       void *bindNode = nullptr);
  void *CreateFuncCallStmt(void *sg_decl_, void *param_list_,
                           void *bindNode = nullptr);
  void *CreateFuncCallStmt(void *sg_decl_, vector<void *> param_list_,
                           void *bindNode = nullptr);

  /*********  COMPARE AST NODE *************/
  bool isSameFunction(void *func1, void *func2, int ignore_static = 0);
  bool isSameType(void *type1, void *type2);
  bool is_in_same_file(void *sg1, void *sg2);
  bool is_in_same_function(void *sg1, void *sg2);
  bool IsMatchedFuncAndCall(void *decl, void *call,
                            void *callee = nullptr); // light weighted function
  int IsCompatibleType(void *sg_type1, void *sg_type2, void *pos, bool strict);
  bool is_identical_expr(void *sg_exp1, void *sg_exp2, void *sg_pos1,
                         void *sg_pos2);
  bool is_identical_base_type(void *sg_array1, void *sg_array2);
  // youxiang: check whether 'target_init' is alias of 'src_init' by traversing
  // along call path
  int is_same_instance(void *target_init, void *src_init,
                       t_func_call_path call_path);
  int IsomorphicLoops(void *one_loop, void *other_loop);
  bool isDom(void *pos1, void *pos2);

  /*******   PRAGMA *************************/
  /********** get pragma info **************/
  string GetPragmaString(void *sg_node_);
#if USED_CODE_IN_COVERAGE_TEST
  string GetPragmaStringAbove(void *pos);
#endif
  void *GetPragmaAbove(void *pos);
  /********* update pragma info ************/
  void *SetPragmaString(void *sg_node_, string str); // return the new pointer
  /********  create pragma info ***********/
  void *CreatePragma(string sPragma, void *sg_scope_, void *bindNode = nullptr);

  /********** EXPRESSION & STATEMENT *************/
  /********* get expression info *************************/
  int IsSimpleAggregateInitializer(void *sg_initer_);
  void *GetAggrInitializerFromCompoundLiteralExp(void *sg_exp);
  void parse_aggregate(void *scope = nullptr);
  void GetExpOperand(void *sg_exp, void *&op0, void *&op1);
  void *GetExpUniOperand(void *sg_exp);
  void *GetExpLeftOperand(void *sg_exp);
  void *GetExpRightOperand(void *sg_exp);
  void *GetFirstFuncDeclInGlobal(void *pos);
  void *GetFirstNonTrivalStmt(void *blk);
  void *GetLastNonTrivalStmt(void *blk);
  bool IsConditionStmt(void *stmt);
  void *GetSwitchStmtSelector(void *sg_switch);
  void GetExprList(void *sg_exp_list, vector<void *> &vec_expr);
  void GetExprListFromCommaOp(void *sg_comma, vector<void *> &vec_expr);
  void GetExprListFromAndOp(void *sg_and, vector<void *> &vec_expr);
  void GetExprListFromOrOp(void *sg_and, vector<void *> &vec_expr);
  bool IsConditionalExp(void *exp, void *&cond_exp, void *&true_exp,
                        void *&false_exp);
  void *GetNextStmt(void *sg_stmt_, bool includingCompilerGenerated = true);
  void *GetNextStmt_v1(void *sg_stmt_);
  void *GetPreviousStmt(void *sg_stmt_, bool includingCompilerGenerated = true);
  void *GetPreviousStmt_v1(void *sg_stmt_);
  void *GetParent(void *sg_node_);
  void *GetFirstInsertPosInGlobal(void *sg_node);

  void *GetChildStmt(void *sg_scope_, int i);
  vector<void *> GetChildStmts(void *sg_scope_);
  int GetChildStmtNum(void *sg_scope_);
  int GetChildStmtIdx(void *sg_scope_, void *sg_child_); // -1 for not found
  void
  GetChildStmtList(void *sg_scope_,
                   SgStatementPtrList &stmt_lst); // add by Yuxin, May 26 2015
  // Mo, not suggested
  bool GetChildTaskIdx(void *sg_scope_, void *sg_child_, int &index, int HGT);
  // void GetForStmt(void * sg_scope, vector<void*>& vecLoops);

  void *GetLocation(void *start_stmt, bool forward = true);
  int IsLoopStatement(void *sg_node) {
    return IsForStatement(sg_node) || IsWhileStatement(sg_node) ||
           IsDoWhileStatement(sg_node);
  }
  void *GetGotoLabel(void *sg_node) {
    SgGotoStatement *goto_stmt = isSgGotoStatement((SgNode *)sg_node);
    if (!goto_stmt)
      return nullptr;
    return goto_stmt->get_label();
  }

  void *GetStmtFromLabel(void *sg_node) {
    SgLabelStatement *label = isSgLabelStatement((SgNode *)sg_node);
    if (!label)
      return nullptr;
    return label->get_statement();
  }

  string GetLabelName(void *sg_node);
  int IsControlStatement(void *sg_node) {
    return IsIfStatement(sg_node) || IsLoopStatement(sg_node) ||
           IsSwitchStatement(sg_node);
  }
  void *GetReturnExp(void *sg_node) {
    SgReturnStmt *ret_stmt = isSgReturnStmt((SgNode *)sg_node);
    if (!ret_stmt)
      return nullptr;
    return ret_stmt->get_expression();
  }

  int IsDummyReturnStmt(void *sg_node) {

    SgReturnStmt *ret_stmt = isSgReturnStmt((SgNode *)sg_node);
    if (!ret_stmt)
      return false;
    SgExpression *ret_val = ret_stmt->get_expression();
    if (isSgNullExpression(ret_val))
      return true;
    return false;
  }

  int IsOverideAssignOp(void *sg_exp);
  // 0 for prefix, 1 for suffix
  int GetUnaryOpMode(void *sg_exp) {
    return isSgUnaryOp((SgNode *)sg_exp)->get_mode();
  }
  int IsCompareOp(void *sg_exp);
  int GetCompareOp(void *sg_exp);
  int IsIncrementOp(void *sg_node);
  int GetBinaryOperationFromCompoundAssignOp(void *exp);
  bool isSimpleOp(void *op, void *var_name);
  int IsValueExp(void *sg_node);
  bool IsConstantInt(void *sgExp, int64_t &value, bool simplify_cast = true);
  bool IsConstantTrue(void *sgExp);
  bool IsConstantFalse(void *sgExp);
  bool GetCastValue(int64_t &val, void *cast_type);
  int IsNullPointerCast(void *sg_exp);
  void *get_sizeof_value(void *sizeof_exp);
  void ConvertBigIntegerIntoSignedInteger(int64_t &val, void *unsigned_type);
  /************* update expression ********************/
  int AddCastToExp(void *sg_exp, string s_type);
  int AddCastToExp(void *sg_exp, void *sg_type);
  void remove_cast(void *&sg_node);
  void *RemoveCast(void *sg_exp);
  int standardize_pntr_ref_from_array_ref(void *sg_ref, void *pos = nullptr);

  void *splitExpression(void *from, string newName = "");
  /************* create const expression **************/
  void *CreateConst(void *val_, int const_type = V_SgIntVal,
                    void *bindNode = nullptr);
  void *CreateConst(int value_, void *bindNode = nullptr);
  void *CreateConst(long value_, void *bindNode = nullptr);
  void *CreateConst(short value_, void *bindNode = nullptr);
  void *CreateConst(size_t value_, void *bindNode = nullptr);
  void *CreateStringExp(string val, void *bindNode = nullptr);
  void *CreateAndOpFromExprList(vector<void *> vec_expr,
                                void *bindNode = nullptr);

  /************ create reference ************************/
  void *CreateVariableRef(void *var_initname_or_decl, void *scope = nullptr,
                          void *bindNode = nullptr);
  void *CreateVariableRef(string var_name, void *scope = nullptr,
                          void *bindNode = nullptr);
  void *CreateArrayRef(void *array_var_ref, vector<void *> vec_index,
                       void *bindNode = nullptr);

  /************ create expression & statement  ***********/
  void *CreateCastExp(void *sg_exp, string s_type, void *bindNode = nullptr);
  void *CreateCastExp(void *sg_exp, void *sg_type, void *bindNode = nullptr);
  void *CreateConditionExp(void *op0, void *op1, void *op2,
                           void *bindNode = nullptr);
  void *CreateExp(int opcode, void *op0 = nullptr, void *op1 = nullptr,
                  int unary_mode_ = 0, void *bindNode = nullptr);
  void *CreateStmt(int v_type, void *op0_ = nullptr, void *op1_ = nullptr,
                   void *op2_ = nullptr, void *op3_ = nullptr,
                   void *op4_ = nullptr, void *op5_ = nullptr,
                   void *bindNode = nullptr);
#if USED_CODE_IN_COVERAGE_TEST
  void *CreateLabel(string sName, void *bindNode = nullptr);
#endif
  void *CreateBasicBlock(void *stmt_list = nullptr, bool is_list = false,
                         void *bindNode = nullptr);
  void *CreateExpList(vector<void *> exp_list_, void *bindNode = nullptr);
  void *CreateExpList(vector<size_t> dims, void *sg_var,
                      void *bindNode = nullptr);
  void *CreateAggregateInitializer(vector<void *> vec_exp, void *sg_type);
  void *CreateAggregateInitializer(void *expr_list, void *sg_type);

  /********* TYPE  ****************************************************/

  /*************** get type or class info  *****************************/
  int IsScalarType(void *sg_name);
  int IsStructureType(void *sg_type_);
  int IsUnionType(void *sg_type_);
  int IsEnumType(void *sg_type_);
  int IsAnonymousStructureType(void *sg_type_);
  int IsAnonymousType(void *sg_type_);
  int IsAnonymousName(string);
  int IsClassType(void *sg_type_);
  int IsCompoundType(void *sg_type);
  int IsRecursiveType(void *sg_type_, void *&rec_type);
  int IsRecursiveType(void *sg_type_);
  // primitive type, class/struct without function member, non-recusive type
  int IsCStyleSimpleType(void *sg_type_, void *&unsupported_type,
                         string &reason);
  int IsConstType(void *sg_type);
  int IsVolatileType(void *sg_type);
  int IsGeneralCharType(void *sg_type);
  int get_type_unit_size(void *sg_type, void *pos);
  int get_type_size(void *sg_type, void *pos, bool report = true);
  void *get_type_size_exp(void *sg_type, void *pos, bool report);
  int get_pointer_dimension(void *sg_type_, void *&basic_type,
                            vector<size_t> &nSizes);
  int get_arrays_dim(void *sg_bb_or_func_,
                     map<string, vector<int>> &arr_name2dim);

  // not across function (stable)
  // Note
  // The order of the return dims is from low_dim to high_dim (right to left)
  // E.g. int a[1][2][3] => ( 3,2,1 )
  //      int (*a)[10]   => ( 10, 0 )
  //      int *a[10]     => ( 0, 10 )
  int get_type_dimension(void *sg_type_, void *&basic_type,
                         vector<size_t> &nSizes, void *pos = nullptr,
                         bool stop_pointer = false);
  int get_type_dimension_exp(void *sg_type_, void *&basic_type,
                             vector<void *> &nSizes, void *pos,
                             bool stop_pointer);
  int get_type_dimension_new(void *sg_type_, void *&basic_type,
                             vector<size_t> &nSizes, void *pos = nullptr,
                             bool stop_pointer = false);

  void *get_array_base_type(void *sg_array);

  // not across function
  int get_dim_num_from_var(void *sg_array);

  // the flagship for cross function analysis
  int get_type_dimension_by_ref(void *sg_varref_or_pntr, void *&sg_array,
                                void *&basic_type, vector<size_t> &nSizes);

  int get_bitwidth_from_type(void *sg_type, bool report = true);

  int IsAnonymousTypeDecl(void *sg_decl);
  int IsTypeDecl(void *sg_decl);

  int ContainsUnSupportedType(void *sg_type_, void *&unsupported_type,
                              string &reason);
  void GetAllUnSupportedType(void *sg_type_, set<pair<void *, string>> &res);
  void *ContainsClassType(void *sg_type_);

  string get_var_ref_base_type(void *var_ref);
  void *GetTypebyVarRef(void *var_ref_);
  void *GetTypebyNewExp(void *new_exp_);
  void *GetTypeByExp(void *sg_exp);
  void *GetTypeByEnumVal(void *sg_exp);
#if USED_CODE_IN_COVERAGE_TEST
  void *GetTypeByDecl(void *sg_class_decl);
#endif
  void *GetTypeDeclByType(void *sg_type, int require_def = 1);
  void *GetTypeDefDecl(void *sg_type);
  void *GetClassDefinition(void *sg_class_decl);
  void *GetTypeDeclByDefinition(void *sg_class_decl);
  void *GetTypeDeclByMemberFunction(void *sg_mem_func);
  void *GetTypeByName(string type_name);
  string GetTypeName(void *sg_class_decl, bool qualified = false);
  string GetTypeNameByType(void *sg_class_type, bool qualified = false);
  string GetStringByType(void *sg_type);
  void *GetBaseTypeRecur(void *sg_type) {
    return isSgType((SgNode *)sg_type)->findBaseType();
  }
  void *GetOrigTypeByTypedef(void *sg_type, bool skip_anonymous_struct = false);

  void *GetBaseTypeByModifierType(void *sg_type);

  // Type related
  void InitBuiltinTypes();
  int IsBuiltinType(void *type) {
    string type_name = GetStringByType(type);
    return IsBuiltinType(type_name);
  }
  int IsBuiltinType(string sType) { return m_builtin_types.count(sType) > 0; }
  void *GetBaseType(void *sg_type, bool skip_anonymous_struct = true) {
    return trace_base_type(sg_type, skip_anonymous_struct);
  }
  void *GetBaseTypeOneLayer(void *sg_type);

  void *GetElementType(void *sg_type);

  void GetClassMembers(void *sg_class_type, vector<void *> &members);
  void GetClassMembersByDecl(void *sg_class_decl, vector<void *> &members);
  void *GetClassMemberByName(string s_var, void *sg_class_type);
  void GetClassDataMembers(void *sg_class_type, vector<void *> &data_members);
  void GetClassDataMembersByDecl(void *sg_class_decl,
                                 vector<void *> &data_members);
  void GetClassFunctionMembers(void *sg_class_type,
                               vector<void *> &func_members);

  /***** update type *******************************/
  void *RemoveConstType(void *sg_type, void *pos, bool &changed);
  void *RemoveConstVolatileType(void *sg_type_, void *pos);
  // return 1 if typedef replacement happened
  // return 0 otherwise
  int RemoveTypedef(void *sg_type, void *&new_type, void *pos,
                    bool skip_builtin = false, string msg = "");
  void *ConvertBool2Char(void *sg_type_, void *pos);
  /**************************** create type or class ************/
  void *GetTypeByString(string sType);
  void *ArrayToPointerRecur(void *sg_type, bool skip_typedef = false);
  void RegisterType(string sType, void *pos = nullptr);
  void *CreateArrayType(void *base_type, vector<size_t> vec_dim,
                        void *bindNode = nullptr);
  void *CreatePointerType(void *base_type, void *bindNode = nullptr);
  void *CreateCombinedPointerArrayType(void *base_type, vector<size_t> vec_dim,
                                       void *bindNode = nullptr);
  void *CreatePointerType(void *base_type, int levels,
                          void *bindNode = nullptr);
  void *CreateReferenceType(void *base_type, void *bindNode = nullptr);
  void *CreateModifierType(void *new_base_type, void *orig_modifier,
                           void *bindNode = nullptr);
  void *CreateConstType(void *sg_type, void *pos);

  // create a class declation by rose API SageBuilder::buildClassDeclaration_nfi
  // also, mark the change in history
  void *CreateClassDecl(const SgName &name,
                        SgClassDeclaration::class_types kind,
                        SgScopeStatement *scope,
                        SgClassDeclaration *nonDefiningDecl,
                        bool buildTemplateInstantiation,
                        SgTemplateArgumentPtrList *templateArgumentsList,
                        void *bindNode = nullptr);

  // Min: convert SgTypeString to Pointer of char
  void *ConvertTypeStrToPtrChar(void *sg_type);

  /******************************* LOOP ************************/
  /********** get loop info **********************************/
  void *GetLoopBody(void *for_loop);
  void *GetLoopTest(void *sg_for); // return the statement
  void *GetLoopInit(void *sg_for);
  int IsLoopInit(void *sg_stmt);
  int IsLoopTest(void *sg_stmt);
  int IsLoopBody(void *sg_stmt);
  void *GetLoopIncrementExpr(void *sg_for); // return the expression
  void *GetLoopIterator(void *for_loop);
  void *GetLoopFromIteratorByPos(void *sg_name, void *sg_pos,
                                 int cross_func = 0, int trace_back = 1);
  void *GetLoopFromIteratorRef(void *sg_var_ref);
  void *GetWhileCondition(void *sg_loop);
  int64_t get_average_loop_trip_count(void *for_loop, t_func_call_path &fn_path,
                                      void *scope);
  int64_t get_average_loop_trip_count(void *pos, void *scope,
                                      t_func_call_path &fn_path);
  bool get_loop_trip_count_simple(void *sg_loop, int64_t &trip_count);
  int get_loop_trip_count(void *for_loop, int64_t &trip_count,
                          int64_t *trip_count_ub = nullptr);
  int get_loop_trip_count(void *for_loop, void *&trip_count);
  // Across-Function Analysis
  int get_loop_nest_in_scope(void *sg_pos, void *sg_scope,
                             vector<void *> &sg_loops);
  int get_loop_nest_in_scope(void *sg_pos, void *sg_scope,
                             t_func_call_path func_path,
                             vector<void *> &sg_loops);
  int get_loop_nest_in_scope(void *sg_pos, void *sg_scope,
                             t_func_call_path func_path,
                             vector<void *> &sg_loops, set<void *> &visited);

  int hasLabelWithGoto(void *for_loop);

  ////////////////////////////////////////////////////////////////////////
  // A canonical form is defined as : one initialization statement,
  // a test expression, and an increment expression , loop index variable
  // should be of an integer type. IsInclusiveUpperBound is true when <= or
  // >= is used for loop condition.
  // E.g. the usage in CSageCodeGen::init_range_analysis()
  ////////////////////////////////////////////////////////////////////////
  int IsCanonicalForLoop(void *for_loop, void **ivar = nullptr,
                         void **lb = nullptr, void **ub = nullptr,
                         void **step = nullptr, void **body = nullptr,
                         bool *hasIncrementalIterationSpace = nullptr,
                         bool *isInclusiveUpperBound = nullptr);

  // ZP: 20150725: this func is deprecated, recommend to use IsCanonicalForLoop
  // instead
  int ParseOneForLoop(void *sg_for_stmt, void *&var, void *&start, void *&end,
                      void *&step, void *&cond_op, void *&body, int &ulimit);

  int GetLoopStepValueFromExpr(
      void *step_expr,
      int64_t &value); // return 0 if not available (e.g. not constant)
  int is_innermost_for_loop(void *for_loop);
  void *GetSpecifiedLoopByPragma(void *sg_pragma);
  int is_perfectly_nested(void *sg_loop);
  void get_perfectly_nested_loops(void *sg_top_loop,
                                  vector<void *> &nested_loops);
  void get_perfectly_nested_loops_from_bottom(void *sg_bottom_loop,
                                              vector<void *> &nested_loops);
  vector<int> get_loop_order_vector_in_function(void *sg_loop);
  string get_internal_loop_label(void *sg_loop);
  /************** update loop info ***************************************/
  // start is the real lower bound
  // end is the real upper bound + 1
  int CanonicalizeForLoop(void *&sg_for_loop, bool transform);
  int StandardizeForLoop(void *&sg_for_loop_);

  /****************************** Create loop  ********************/
  void *CreateWhileLoop(void *sg_cond_exp, void *sg_body,
                        void *bindNode = nullptr);
  // sg_init SgStatement, sg_test SgStatement, incr_exp SgExpression,
  // sg_body SgBasicBlock
  void *CreateForLoop(void *sg_init, void *sg_test, void *incr_exp,
                      void *sg_body, void *bindNode = nullptr);

  /************************IF STATEMENT *************************************/
  /************************* If statement info ****************************/
  void *GetIfStmtTrueBody(void *sg_if);
  void *GetIfStmtCondition(void *sg_if);
  void *GetIfStmtFalseBody(void *sg_if);

  /****************** update If statement *********************************/
  void SetIfStmtTrueBody(void *sg_if, void *new_body);
  void SetIfStmtFalseBody(void *sg_if, void *new_body);

  /***************** create If statement **********************************/
  void *CreateIfStmt(void *sg_cond_exp_, void *sg_if_, void *sg_else_,
                     void *bindNode = nullptr);

  /********************* TRACE TREE  ***************************************/
  void TraverseSimple(void *sg_scope, string sOrder,
                      t_func_rose_simple_traverse pFunc, void *pFuncArg);

  template <typename Fn> void TraversePre(SgNode &n, Fn &&func) {
    CSageWalker<Fn> *walker = new CSageWalker<Fn>(std::forward<Fn>(func));
    walker->traverse(&n, preorder);
    delete walker;
  }

  template <typename Fn> void TraversePost(SgNode &n, Fn &&func) {
    CSageWalker<Fn> *walker = new CSageWalker<Fn>(std::forward<Fn>(func));
    walker->traverse(&n, postorder);
    delete walker;
  }

  /**************** Get all the type and derived type *******************/
  void GetNodesByType_compatible(void *sg_scope, string str_type,
                                 vector<void *> &vec_nodes);
  // 'str_type' is ROSE AST class name, such as 'SgVarRefExp'
  void GetNodesByType(void *sg_scope, string sOrder, string str_type,
                      vector<void *> *vecNodes, bool compatible = false,
                      bool cross_func = false, bool keep_org = false);
  // 'v_type' is ROSE AST class enum number, such as 'V_SgVarRefExp'
  void GetNodesByType_int_compatible(void *sg_scope, int v_type,
                                     vector<void *> &vec_nodes);
  // internal func to implement GetNodeByType(..., cross_func = 1, ...);
  int GetNodesByType_recur(void *sg_scope, string order, string str_type,
                           vector<void *> *vecNodes, bool compatible /*=false*/,
                           set<void *> &visited_decl);

  void GetNodesByType_int(void *sg_scope, string sOrder, int v_type,
                          vector<void *> *vecNodes, bool compatible = false);

  /************************* Trace up from bottom ***************************/
  // from the parent of sg_node_
  void *GetEnclosingNode(string type_, void *sg_node_);
  void *TraceUpByType(void *sg_pos, string str_type);
  void *TraceUpByTypeCompatible(void *sg_pos,
                                int V_sg_type = V_SgScopeStatement);
  void *TraceUpToStatement(void *sg_pos) {
    void *ret = TraceUpByTypeCompatible(sg_pos, V_SgStatement);
    if (!ret)
      return sg_pos;
    return ret;
  }
  void *GetScope(void *stmt_or_exp);
  void *TraceUpToGlobal(void *sg_pos) {
    return TraceUpByTypeCompatible(sg_pos, V_SgGlobal);
  }
  void *TraceUpToScope(void *sg_pos) {
    return TraceUpByTypeCompatible(sg_pos, V_SgScopeStatement);
  }
  void *TraceUpToFuncDecl(void *sg_pos) {
    return TraceUpByTypeCompatible(sg_pos, V_SgFunctionDeclaration);
  }
  void *TraceUpToClassDecl(void *sg_pos) {
    return TraceUpByTypeCompatible(sg_pos, V_SgClassDeclaration);
  }
  void *TraceUpToFuncCall(void *sg_pos) {
    return TraceUpByTypeCompatible(sg_pos, V_SgFunctionCallExp);
  }
  void *TraceUpToForStatement(void *sg_pos) {
    return TraceUpByTypeCompatible(sg_pos, V_SgForStatement);
  }
  void *TraceUpToBasicBlock(void *sg_pos) {
    return TraceUpByTypeCompatible(sg_pos, V_SgBasicBlock);
  }
  // youxiang 20161004 handle condition statement of if-statement
  void *TraceUpToInsertPosition(void *sg_pos);
  // trace up to either for- while- or dowhile- loop statement
  void *TraceUpToLoopScope(void *sg_pos, bool exclusive = false);

  void *TraceUpToAccess(void *sg_pos);
  void *TraceUpToIndex(void *sg_pos);
  void *TraceUpToExpression(void *sg_pos) {
    return TraceUpByTypeCompatible(sg_pos, V_SgExpression);
  }
  void *TraceUpToRootExpression(void *sg_pos);
  void *TraceUpToInitializedName(void *sg_exp);

  void TraceUpFuncCallsGeneric(void *sg_scope, void *sg_func_decl_arg,
                               t_func_trace_up pFunc, void *pFuncArg);
  void TraceUpFuncArgSource(void *sg_scope, void *sg_func_decl_arg,
                            vector<void *> &vec_source);

  /******************* get specific nodes ********************/
  void get_ref_in_scope(void *var_init, void *scope, vector<void *> &refs,
                        bool skip_access_intrinsic = false,
                        bool is_preorder = true);
  void get_ref_in_scope_recursive(void *var_init, void *scope,
                                  vector<void *> &vec_refs);

  void get_ref_in_scope(void *var_init, void *scope, t_func_call_path call_path,
                        vector<void *> &refs,
                        bool skip_access_intrinsic = false,
                        bool is_preorder = true);

  vector<void *> GetAllRefInScope(void *sg_name, void *scope);
  // YX: 20180212 get variable references from floating node, such as array
  // index
  void get_floating_var_refs(void *scope, vector<void *> &vec_refs);
  void get_refs_from_type(void *type, vector<void *> &vec_refs);

  /************************** ANALYSIS ****************************/
  /************** def/use *********************************/
  // return nullptr, if not single assignment.
  // sg_var is an init_name: global, argument or local
  // This function is conservative
  void *GetSingleAssignExpr(void *sg_var, void *pos = nullptr,
                            int reset_defuse = 0, void *scope = nullptr,
                            bool cross_function = true,
                            bool skip_range = false);
  void *GetSingleAssignExpr_path(void *sg_var, t_func_call_path &fn_path,
                                 void *&new_pos, void *pos = nullptr,
                                 int reset_defuse = 0, void *scope = nullptr);

  std::vector<std::pair<void *, void *>> // a multi-map an init_name to its
                                         // defs, at a certain position
  GetAllDefbyPosition(void *pos);

  std::vector<std::pair<void *, void *>> // a multi-map an init_name to its
                                         // defs, at a certain position
  GetAllUsebyPosition(void *pos);

  std::vector<void *> // defs of an init_name , at a certain position
  GetVarDefbyPosition(void *init_name, void *pos, int simple_iterator = 0);

  // youxiang support pointer converative and accurate analysis
#if USED_CODE_IN_COVERAGE_TEST
  std::vector<void *> // defs of an init_name , at a certain position
  GetVarDefbyPosition_v2(void *init_name, void *pos);
#endif

#if USED_CODE_IN_COVERAGE_TEST
  std::vector<void *> // defs of an init_name , at a certain position
  GetVarUsebyPosition(void *init_name, void *pos);
#endif

  void set_function_for_liveness(void *func_decl);
  void get_liveness_in(
      void *sg_pos,
      vector<void *> &ret); // get the init_name of the alive variables
  void get_liveness_out(
      void *sg_pos,
      vector<void *> &ret); // get the init_name of the alive variables

  void get_liveness_for_loop(void *for_loop, vector<void *> &in,
                             vector<void *> &out);

  /******************* read/write analysis ************************/
  bool is_interface_alias_write(void *ref, const set<void *> ports_alias,
                                bool top);
  int is_write_ref(void *ref);
  int is_write_type(void *ref); // add by Yuxin May 8 2015
  // ZP: 20150818
  // only work for variable ref or pntr ref
  int is_write_conservative(void *ref, bool recursive = true);
  int is_write_conservative(void *ref, void *&new_def, bool recursive = true);
  // int is_read_conservative(void *ref);
  int has_read_conservative(
      void *ref, bool recursive = true); // no matter if there is write
  int has_write_conservative(void *ref); // no matter if there is read
  // array_ref: A (in A[e1][e2])
  // cross-function
  bool array_ref_has_write_conservative(void *array_ref);
  bool array_ref_has_read_conservative(void *array_ref);
  int array_ref_is_write_conservative(
      void *array_ref); // 0 for read, 1 for write, 2 for read & write
  // ZP: standard function for both scalar and pointer, conservative,
  // cross-function
  int has_write_in_scope(void *array, void *scope);
  int has_read_in_scope(void *array, void *scope);

  // YX: only used in movable test for only pointer, conservative
  // non-cross-function
  int has_write_in_scope_fast(void *array, void *scope);
  int has_read_in_scope_fast(void *array, void *scope);

  int has_side_effect(void *exp);
  //#if  USED_CODE_IN_COVERAGE_TEST
  int has_side_effect_v2(void *exp, void *init_name = nullptr);
  //#endif
  int has_lvalue_ref_in_scope(void *scope, void *name,
                              vector<void *> *v_ref = nullptr);
  int is_lvalue(void *ref);
  /******************* movable test *****************************************/
  int is_movable_test(void *exp, void *move_to, void *move_from = nullptr);

  // to check if any side effect on sg_ref if move sg_from to sg_to (before)
  // This function is conservative
  int CheckMovable(void *sg_from, void *sg_to, void *sg_ref);

  /******************** Loop Invariant *********************************/

  bool IsLoopInvariant(void *for_loop, void *expr, void *pos);
  bool IsContinuousAccess(void *expr, void *for_loop, void *pos = nullptr);

  /************* Pointer and Array Analysis ***************************/
  // This is the most advanced function
  // sg_ref: A[e1][e2]  =>  sg_array=A  sg_indeces=e1,e2
  void parse_pntr_ref_new(void *sg_ref, void *&sg_array,
                          vector<void *> &sg_indexes);

  // pntr : A[e1][e2]  =>  array_ref=A
  int parse_array_ref_from_pntr(void *pntr, void *&array_ref,
                                void *pos = nullptr, bool simplify_idx = true,
                                int adjusted_array_dim = -1);

  // sg_ref: A (in A[e1][e2])  =>  sg_array=A  sg_indeces=e1,e2 // stable and
  // flagship simplify_idx: if true, CMarsExpress==0 will used in express
  // simplification ignore_dim_limit: if true, pntr will be extracted without
  // consider the array dim number
  void parse_pntr_ref_by_array_ref(void *sg_ref, void *&sg_array,
                                   void *&sg_pntr, vector<void *> &sg_indexes,
                                   int &pointer_dim, void *pos = nullptr,
                                   bool simplify_idx = true,
                                   int adjusted_array_dim = -1);

  void parse_pntr_ref_by_func_call(void *sg_ref, void *&sg_func, void *&sg_pntr,
                                   vector<void *> &sg_indexes, int &pointer_dim,
                                   void *pos = nullptr);

  int parse_standard_pntr_ref_from_array_ref(
      void *sg_ref, void *&sg_array, void *&sg_pntr, void *&sg_base_type,
      vector<void *> &sg_indexes, int &pointer_dim, void *pos = nullptr);
  // array_ref: A (in A[e1][e2])
  int is_array_ref_full_dimension(void *array_ref);

  // stable, call parse_pntr_ref_new
  void *get_array_from_pntr(void *pntr);

  // stable, call parse_pntr_ref_by_array_ref
  void *get_pntr_from_var_ref(void *var_ref, void *pos = nullptr);

  // stable
  int get_pntr_pntr_dim(void *pntr);
  int get_pntr_init_dim(void *pntr);
  //

  // The simple version of parse_pntr_ref_by_array_ref
#if USED_CODE_IN_COVERAGE_TEST
  void get_pntr_ref_by_array_ref(void *sg_ref, void *&sg_array, void *&sg_pntr,
                                 vector<void *> &sg_indexes);
#endif

  // ZP:20170606: Not stable!!! use get_pntr_from_var_ref() instead
  // ref: A (in A[e1][e2])  =>  sg_pntr = A[e1][e2]
  void get_pntr_from_var(void *ref, void *&sg_pntr);
  void *parse_pntr_type_in_function(void *sg_pntr, void *&sg_array,
                                    void *&base_type, vector<size_t> &vecSizes,
                                    int &pointer_dim);

  // sg_ref: A[e1][e2]  =>  sg_array=A  sg_indeces=e1,e2
  void parse_pntr_ref(void *sg_ref, void *&sg_array,
                      vector<void *> &sg_indexes);
  // pntr is the whole PntrRefExp and var is the innermost VarRefExp
  int get_var_from_pntr_ref(void *pntr, void *&var);
  // pntr is the whole PntrRefExp and indexes is from rightmost dimension to the
  // left
  int get_index_from_pntr_ref(void *pntr, vector<void *> &indexes);
  // sub functions for get_dimension_by_ref
  int analyze_type_with_new_and_malloc(void *sg_array, void *sg_scope,
                                       vector<size_t> &vecSizes);

  bool is_assign_to_pntr(void *parent, void *&var_name, void *&value);
  int parse_assign(void *parent, void *&var_name, void *&value);

  int get_ref_dimension(void *ref, vector<void *> &indexes);

  /*********************** scope analysis ******************************/

  int IsInScope(void *sg_node, void *sg_scope);
  bool IsScopeBreak(void *sg_break, void *sg_scope);
  // The major function for induction analysis, call GetSingleAssignExpr
  // [Notice] The input should be original in the AST node, because def/use
  // result is used in analysis
  int analysis_induct_map_in_scope(void *ref_scope, void *boundary_scope,
                                   map<void *, void *> &map_var2exp,
                                   vector<void *> *inner_terms = nullptr);
  void *GetCommonScope(void *sg_pos1, void *sg_pos2);
  void *GetCommonScope(vector<void *> &vec_pos);
  // this function is similar with is_located_in_scope, but it
  // will not do cross function analysis
  int is_located_in_scope_simple(void *sg_pos, void *sg_scope);
  // Across funciton
  int is_located_in_scope(void *sg_pos, void *sg_scope);
  int is_located_in_scope(void *sg_pos, void *sg_scope,
                          const t_func_call_path &func_path);
  int is_located_in_scope(void *sg_pos, void *sg_scope,
                          const list<t_func_call_path> &func_path);

  /*********************  call path analysis *****************************/
#if USED_CODE_IN_COVERAGE_TEST
  void filter_vec_node_in_path(t_func_call_path &fn_path,
                               vector<void *> &vec_in);
#endif
  // each map is a possible path, there are multiple paths because there
  // multiple calls of a func
  // map<func_decl, func_call>
  int get_call_paths(void *sg_pos, void *sg_scope,
                     list<t_func_call_path> &vec_paths);
  int get_call_paths_int(void *sg_pos, void *sg_scope, set<void *> &visited,
                         list<t_func_call_path> &vec_paths);
  string print_func_path(t_func_call_path fn_path);

  void get_all_func_path_in_scope(void *range_scope,
                                  list<t_func_call_path> &vec_paths);
  void get_all_func_path_in_scope_int(void *range_scope,
                                      list<t_func_call_path> &vec_paths);

  void get_all_func_path_in_scope(void *array, void *range_scope,
                                  list<t_func_call_path> &vec_paths);
  void get_all_func_path_in_scope_int(void *array, void *range_scope,
                                      list<t_func_call_path> &vec_paths);
  // YX: support simple pointer alias
  void get_all_func_path_in_scope_int_v2(void *array, void *range_scope,
                                         list<t_func_call_path> &vec_paths,
                                         set<void *> &visited);
  int is_located_in_path(void *sg_pos, t_func_call_path func_path);

  // for array variable reference
  int is_located_in_path_v2(void *ref, t_func_call_path func_path);
  int is_included_in_path(const pair<void *, void *> &call_site,
                          t_func_call_path func_path);

  int is_all_var_declared_in_scope(void *sg_pos, void *sg_scope,
                                   t_func_call_path func_path);

  void *get_call_in_path(void *func_decl, const t_func_call_path &func_path);

  void *GetCommonPosition(void *sg_pos1, void *sg_pos2);
  void *GetCommonPosition(void *sg_pos1, t_func_call_path &path1, void *sg_pos2,
                          t_func_call_path &path2);
  void *GetCommonPosition(vector<void *> &vec_pos);
  t_func_call_path GetCommonPath(const t_func_call_path &path1,
                                 const t_func_call_path &path2);

  /*********************** Range analysis *************************************/
  int GetVarRangeFromCompareOp(void *expr, map<void *, void *> &lbs,
                               map<void *, void *> &ubs,
                               bool true_value = true);

  int GetVarDefRangeFromCompareOp(void *expr, map<void *, int64_t> &lbs,
                                  map<void *, int64_t> &ubs,
                                  bool true_value = true);

  int GetExprRangeFromCompareOp(void *expr, map<void *, int64_t> &lbs,
                                map<void *, int64_t> &ubs,
                                bool true_value = true);

  void reset_range_analysis();
  void reset_expr_range_analysis();
  void init_defuse_range_analysis() {
    init_defuse();
    reset_range_analysis();
  }
  void init_range_analysis_for_loop(void *for_loop);
  /**********************  Pointer alias analysis *****************************/
  void *get_alias_array_from_assignment_expression(void *sg_pntr);
  void *get_alias_array_from_assignment_expression(void *sg_pntr,
                                                   int &is_memcpy);
  int IsRecursivePointerAlias(void *var_init, bool &confidence_of_negative);
  set<void *> GetAssociatedAlias(void *array, bool &confidence);
  int IsRecursivePointerAlias(void *var_init, bool &confidence_of_negative,
                              set<void *> &visited);
  vector<void *> get_alias_source(void *target_init, void *pos);

  /*********************** TREE OPERATION
   * **************************************/
  /****************** Insert *****************************/
  void InsertStmt(void *sg_stmt_, void *sg_ref_, bool avoid_pragma = false);
  void InsertAfterStmt(void *sg_stmt_, void *sg_ref_);
  void InsertStmtAvoidLabel(void *sg_stmt_, void *&sg_before_);
  void PrependChild(void *sg_scope_, void *sg_child_);
  void AppendChild(void *sg_scope_, void *sg_child_);
  /*****************  Copy ******************************/
  void *CopyChild(void *sg_node);
  void *CopyStmt(void *sg_stmt_, bool passMajor = false);
  void *CopyVarDeclStmt(void *sg_stmt_, void *scope);
  void *CopyInitName(void *sg_init);
  void *CopyScope(void *sg_scope_);
  void *CopyExp(void *sg_exp_, bool passMajor = false);
#if USED_CODE_IN_COVERAGE_TEST
  void CopyPreprocInfo(void *sg_to_, void *sg_from_);
#endif

  /**************  Replace ******************************/
  void ReplaceChild(void *sg_old_, void *sg_new_,
                    bool replace_prep_info = true);
  void ReplaceExp(void *sg_old_, void *sg_new_, bool keepOldExp = true);
  void ReplaceExpWithStmt(void *sg_old_);
  void ReplaceStmt(void *sg_old, void *sg_new, bool replace_prep_info = true) {
    ReplaceChild(sg_old, sg_new, replace_prep_info);
  }

  void *replace_var_refs_in_type(void *type, void *pos);
  void replace_floating_var_refs(void *scope);
  // replace the variable reference in a scope
  void replace_variable_references_in_scope(void *sg_init_old,
                                            void *sg_init_new, void *scope);
  void replace_with_dummy_decl(void *stmt);

  /*************  Remove or Delete ************************/
  void DeleteNode(void *sg_old_);
  void RemoveChild(void *sg_child_,
                   bool remove_prep_info = true); // for a statement
  void DiscardPreprocInfo(void *sg_stmt_or_exp);
  void DiscardIncludePreprocInfo(void *sg_stmt_or_exp);
  void RemoveStmt(void *sg_stmt, bool remove_prep_info = true) {
    RemoveChild(sg_stmt, remove_prep_info);
  }
  void RemoveLabel(void *sg_node);

  /***********   Complex operation **********************/
  bool dead_stmts_removal(void *scope);
  bool remove_dead_parameters(void *&func_decl);
  void fix_defuse_issues(void *scope = nullptr, bool parse_aggregate = true);
  int add_missing_brace(void *scope);
  void remove_double_brace(void *sg_scope = nullptr);
  int add_label_scope(void *sg_loop);
  bool replace_sizeof(void *scope);
  int index_switch_transform(void *input_array, void *input_loop,
                             vector<int> switch_factor, bool read_only = true);
  void clean_empty_aggregate_initializer(void *sg_scope);
  void function_outline(void *node_stmt, void *&node_call, void *&node_decl,
                        string func_name_prefix, bool add_number_suffix,
                        bool dead_code_removal, bool full, bool keep_orig_stmt,
                        bool keep_global_var);
  /*********** PREPROCESS DIRECTIVE *******************************/

  // insert macros, can be used any text, but no syntax meaning (not recommend)
  void AddDirectives(string sDirective, void *sg_node_before_, int before = 1);
#if USED_CODE_IN_COVERAGE_TEST
  void PrependDirectives(string sDirective, void *sg_scope);
#endif
  void AddHeader(string sDirective, void *sg_pos); // sg_pos specify the file to
  // add the header, sg_pos
  // should be in the file
  void InsertHeader(string head_file, void *sg_stmt, int before = 1,
                    int issystem_header = 0);
  void AddComment(string sDirective, void *sg_node_before);

  /********************message infastructure
   * *************************************/
  string get_identifier_from_ast(void *sg_node);
  string GetLocationFromIdentifier(string id);
  void InsertIdentifierInformation(string id, string location);
  void InsertIdentifierInformation(const map<string, string> &id2location);
#if USED_CODE_IN_COVERAGE_TEST
  void UpdateLocationFromIdentifier(string id, string location);
#endif

  /*************  ROSE AST CLASS WRAPPER***********************************/
  bool IsMemberFunctionRefExp(void *ref_exp) {
    return isSgMemberFunctionRefExp((SgNode *)ref_exp) != nullptr;
  }
  int IsCompoundLiteralExp(void *sg_exp) {
    return isSgCompoundLiteralExp((SgNode *)sg_exp) != nullptr;
  }
  int IsBasicBlock(void *sg_node) {
    return isSgBasicBlock((SgNode *)sg_node) != 0;
  }
  int IsScopeStatement(void *sg_node) {
    return isSgScopeStatement((SgNode *)sg_node) != 0;
  }
  int IsPragma(void *sg_node) { return isSgPragma((SgNode *)sg_node) != 0; }

  int IsPragmaDecl(void *sg_node) {
    return isSgPragmaDeclaration((SgNode *)sg_node) != 0;
  }

  int IsStatement(void *sg_node) {
    return isSgStatement((SgNode *)sg_node) != 0;
  }
  int IsExprStatement(void *sg_node) {
    return isSgExprStatement((SgNode *)sg_node) != 0;
  }
  int IsExprList(void *sg_node) {
    return isSgExprListExp((SgNode *)sg_node) != 0;
  }
  int IsCommaExp(void *sg_node) {
    return isSgCommaOpExp((SgNode *)sg_node) != 0;
  }
  int IsInitName(void *sg_node) {
    return isSgInitializedName((SgNode *)sg_node) != 0;
  }

  int IsInitializer(void *sg_node) {
    return isSgInitializer((SgNode *)sg_node) != 0;
  }
  int IsAggregateInitializer(void *sg_node) {
    return isSgAggregateInitializer((SgNode *)sg_node) != 0;
  }
  int IsAssignInitializer(void *sg_node) {
    return isSgAssignInitializer((SgNode *)sg_node) != 0;
  }
  // Not compilable in the current ROSE, but it is specified in the latest ROSE
  // doc
  //  int IsBracedInitializer(void *sg_node) {
  //    return isSgBracedInitializer((SgNode *)sg_node) != 0;
  //  }
  int IsCompoundInitializer(void *sg_node) {
    return isSgCompoundInitializer((SgNode *)sg_node) != 0;
  }
  int IsConstructorInitializer(void *sg_node) {
    return isSgConstructorInitializer((SgNode *)sg_node) != 0;
  }
  int IsDesignatedInitializer(void *sg_node) {
    return isSgDesignatedInitializer((SgNode *)sg_node) != 0;
  }
  int IsForStatement(void *sg_node) {
    return isSgForStatement((SgNode *)sg_node) != 0;
  }
  int IsSwitchStatement(void *sg_node) {
    return isSgSwitchStatement((SgNode *)sg_node) != 0;
  }
  int IsCaseOptionStmt(void *sg_node) {
    return isSgCaseOptionStmt((SgNode *)sg_node) != 0;
  }
  int IsDefaultOptionStmt(void *sg_node) {
    return isSgDefaultOptionStmt((SgNode *)sg_node) != 0;
  }
  int IsForInitStatement(void *sg_node) {
    return isSgForInitStatement((SgNode *)sg_node) != 0;
  }
  int IsWhileStatement(void *sg_node) {
    return isSgWhileStmt((SgNode *)sg_node) != 0;
  }
  int IsDoWhileStatement(void *sg_node) {
    return isSgDoWhileStmt((SgNode *)sg_node) != 0;
  }
  int IsBreakStatement(void *sg_node) {
    return isSgBreakStmt((SgNode *)sg_node) != 0;
  }
  int IsContinueStatement(void *sg_node) {
    return isSgContinueStmt((SgNode *)sg_node) != 0;
  }
  int IsLabelStatement(void *sg_node) {
    return isSgLabelStatement((SgNode *)sg_node) != 0;
  }
  int IsGotoStatement(void *sg_node) {
    return isSgGotoStatement((SgNode *)sg_node) != 0;
  }

  int IsIfStatement(void *sg_node) {
    return isSgIfStmt((SgNode *)sg_node) != 0;
  }

  int IsReturnStatement(void *sg_node) {
    return isSgReturnStmt((SgNode *)sg_node) != 0;
  }

  int IsNullExp(void *sg_node) {
    return isSgNullExpression((SgNode *)sg_node) != nullptr;
  }

  int IsFunctionDefinition(void *sg_node);

  int IsFunctionCall(void *sg_node) {
    return isSgFunctionCallExp((SgNode *)sg_node) != 0;
  }
  //  int IsConstructorInitializer(void *sg_node) {
  //    return isSgConstructorInitializer((SgNode *)sg_node) != 0;
  //  }
  int IsFunctionDeclaration(void *sg_node) {
    return isSgFunctionDeclaration((SgNode *)sg_node) != 0;
  }
  int IsFunctionRefExp(void *sg_node) {
    return isSgFunctionRefExp((SgNode *)sg_node) != nullptr;
  }
  int IsRecursiveFunction(void *func_decl, void *&rec_func);

  int IsTemplateInstantiationFunctionDecl(void *sg_node) {
    return isSgTemplateInstantiationFunctionDecl((SgNode *)sg_node) != 0;
  }

  int IsTemplateFunctionDecl(void *sg_node) {
    return isSgTemplateFunctionDeclaration((SgNode *)sg_node) != 0;
  }

  int IsTemplateInstantiationDirectiveStatement(void *sg_node) {
    return isSgTemplateInstantiationDirectiveStatement((SgNode *)sg_node) != 0;
  }

  int IsUsingDirectiveStmt(void *sg_node) {
    return isSgUsingDirectiveStatement((SgNode *)sg_node) != nullptr;
  }

  int IsNameSpaceDeclStmt(void *sg_node) {
    return isSgNamespaceDeclarationStatement((SgNode *)sg_node) != nullptr;
  }

  int IsNameSpaceDefStmt(void *sg_node) {
    return isSgNamespaceDefinitionStatement((SgNode *)sg_node) != nullptr;
  }

  int IsTemplateInstClassDecl(void *sg_node) {
    return isSgTemplateInstantiationDecl((SgNode *)sg_node) != 0;
  }

  int IsReturnStmt(void *sg_node) {
    return isSgReturnStmt((SgNode *)sg_node) != 0;
  }

  int IsTemplateClassDecl(void *sg_node) {
    return isSgTemplateClassDeclaration((SgNode *)sg_node) != 0;
  }

  int IsVariableDecl(void *sg_node) {
    return isSgVariableDeclaration((SgNode *)sg_node) != 0;
  }

  int IsVarRefExp(void *sg_node) {
    return isSgVarRefExp((SgNode *)sg_node) != 0;
  }
  int IsPntrArrRefExp(void *sg_node) {
    return isSgPntrArrRefExp((SgNode *)sg_node) != 0;
  }
  int IsDotExp(void *sg_node) { return isSgDotExp((SgNode *)sg_node) != 0; }
  int IsArrowExp(void *sg_node) { return isSgArrowExp((SgNode *)sg_node) != 0; }
  int IsAddOp(void *sg_type) { return isSgAddOp((SgNode *)sg_type) != 0; }
  int IsMultiplyOp(void *sg_type) {
    return isSgMultiplyOp((SgNode *)sg_type) != 0;
  }
  int IsDivideOp(void *sg_type) { return isSgDivideOp((SgNode *)sg_type) != 0; }
  int IsSubtractOp(void *sg_type) {
    return isSgSubtractOp((SgNode *)sg_type) != 0;
  }

  int IsFunctionType(void *sg_type) {
    return isSgFunctionType((SgNode *)sg_type) != 0;
  }
  int IsReferenceType(void *sg_type) {
    return isSgReferenceType((SgNode *)sg_type) != 0;
  }
  int IsPointerType(void *sg_type) {
    return isSgPointerType((SgNode *)sg_type) != 0;
  }
  int IsRestrictType(void *sg_type);

  int IsArrayType(void *sg_type) {
    return isSgArrayType((SgNode *)sg_type) != 0;
  }
  int IsTypedefDecl(void *sg_stmt) {
    return isSgTypedefDeclaration((SgNode *)sg_stmt) != 0;
  }
  int IsTypedefType(void *sg_type) {
    return (isSgTypedefType((SgNode *)sg_type)) != 0;
  }
  int IsModifierType(void *sg_type) {
    return (isSgModifierType((SgNode *)sg_type)) != 0;
  }

  int IsBoolType(void *sg_type) { return isSgTypeBool((SgNode *)sg_type) != 0; }
  // if the type is char, short, int, long, int64_t
  int IsIntegerType(void *sg_type) {
    SgType *type = isSgType((SgNode *)sg_type);
    return type && type->isIntegerType();
  }
  // if the type is (signed) int
  int IsIntType(void *sg_type) {
    return isSgTypeSignedInt((SgNode *)sg_type) != 0 ||
           isSgTypeInt((SgNode *)sg_type) != 0;
  }
  int IsUnsignedIntType(void *sg_type) {
    return isSgTypeUnsignedInt((SgNode *)sg_type) != 0;
  }
  int IsVoidType(void *sg_type) { return isSgTypeVoid((SgNode *)sg_type) != 0; }

  int IsFloatType(void *sg_type) {
    SgType *type = isSgType((SgNode *)sg_type);
    return type && type->isFloatType();
  }
  int IsShortType(void *sg_type) {
    return isSgTypeShort((SgNode *)sg_type) != 0 ||
           isSgTypeSignedShort((SgNode *)sg_type) != 0;
  }
  int IsUnsignedShortType(void *sg_type) {
    return isSgTypeUnsignedShort((SgNode *)sg_type) != 0;
  }
  int IsCharType(void *sg_type) {
    return isSgTypeChar((SgNode *)sg_type) != 0 ||
           isSgTypeSignedChar((SgNode *)sg_type) != 0;
  }

  int IsUnsignedCharType(void *sg_type) {
    return isSgTypeUnsignedChar((SgNode *)sg_type) != 0;
  }

  int IsSgTypeString(void *sg_type) {
    return isSgTypeString((SgNode *)sg_type) != 0;
  }

  int IsType(void *sg_type) { return isSgType((SgNode *)sg_type) != 0; }

  int IsLongLongType(void *sg_type) {
    return isSgTypeLongLong((SgNode *)sg_type) != 0;
  }
  int IsUnsignedLongLongType(void *sg_type) {
    return isSgTypeUnsignedLongLong((SgNode *)sg_type) != 0;
  }
  int IsLongType(void *sg_type) { return isSgTypeLong((SgNode *)sg_type) != 0; }
  int IsUnsignedLongType(void *sg_type) {
    return isSgTypeUnsignedLong((SgNode *)sg_type) != 0;
  }
  int IsGeneralLongLongType(void *sg_type_) {
    SgType *sg_type = isSgType((SgNode *)sg_type_);
    return isSgTypeLongLong(sg_type) || isSgTypeUnsignedLongLong(sg_type) ||
           isSgTypeSignedLongLong(sg_type);
  }
  int IsIntegerTypeRecur(void *sg_type) {
    return IsIntegerType(GetBaseTypeRecur(sg_type));
  }

  int IsEnumVal(void *sg_exp) {
    return isSgEnumVal((SgNode *)sg_exp) != nullptr;
  }

  int IsEnumDecl(void *sg_stmt) {
    return isSgEnumDeclaration((SgNode *)sg_stmt) != 0;
  }

  int IsDefaultType(void *sg_type) {
    return isSgTypeDefault((SgNode *)sg_type) != 0;
  }

  int IsClassDecl(void *sg_stmt) {
    return isSgClassDeclaration((SgNode *)sg_stmt) != 0;
  }
  int IsClassDefinition(void *sg_stmt) {
    return isSgClassDefinition((SgNode *)sg_stmt) != 0;
  }
  int IsPlusAssignOp(void *sg_exp) {
    return isSgPlusAssignOp((SgNode *)sg_exp) != 0;
  }
  int IsAssignOp(void *sg_exp) { return isSgAssignOp((SgNode *)sg_exp) != 0; }
  int IsCompoundAssignOp(void *sg_exp) {
    return isSgCompoundAssignOp((SgNode *)sg_exp) != 0;
  }
  int IsPlusPlusOp(void *sg_exp) {
    return isSgPlusPlusOp((SgNode *)sg_exp) != 0;
  }
  int IsMinusMinusOp(void *sg_exp) {
    return isSgMinusMinusOp((SgNode *)sg_exp) != 0;
  }
  int IsMinusAssignOp(void *sg_exp) {
    return isSgMinusAssignOp((SgNode *)sg_exp) != 0;
  }
  int IsUnaryOp(void *sg_exp) { return isSgUnaryOp((SgNode *)sg_exp) != 0; }
  int IsBinaryOp(void *sg_exp) { return isSgBinaryOp((SgNode *)sg_exp) != 0; }
  int IsLessThanOp(void *sg_exp) {
    return isSgLessThanOp((SgNode *)sg_exp) != 0;
  }
  int IsLessOrEqualOp(void *sg_exp) {
    return isSgLessOrEqualOp((SgNode *)sg_exp) != 0;
  }
  int IsGreaterThanOp(void *sg_exp) {
    return isSgGreaterThanOp((SgNode *)sg_exp) != 0;
  }
  int IsGreaterOrEqualOp(void *sg_exp) {
    return isSgGreaterOrEqualOp((SgNode *)sg_exp) != 0;
  }
  int IsEqualOp(void *sg_exp) { return isSgEqualityOp((SgNode *)sg_exp) != 0; }
  int IsNonEqualOp(void *sg_exp) {
    return isSgNotEqualOp((SgNode *)sg_exp) != 0;
  }
  int IsNotOp(void *sg_exp) { return isSgNotOp((SgNode *)sg_exp) != 0; }
  int IsAndOp(void *sg_exp) { return isSgAndOp((SgNode *)sg_exp) != 0; }
  int IsOrOp(void *sg_exp) { return isSgOrOp((SgNode *)sg_exp) != 0; }
  int IsCastExp(void *sg_exp) { return isSgCastExp((SgNode *)sg_exp) != 0; }
  int IsAddressOfOp(void *sg_exp) {
    return isSgAddressOfOp((SgNode *)sg_exp) != 0;
  }
  int IsPointerDerefExp(void *sg_exp) {
    return isSgPointerDerefExp((SgNode *)sg_exp) != 0;
  }
  int IsGlobal(void *sg_node) {
    return isSgGlobal((SgNode *)sg_node) != nullptr;
  }
  int IsFile(void *sg_node) { return isSgFile((SgNode *)sg_node) != nullptr; }
  int IsProject(void *sg_node) {
    return isSgProject((SgNode *)sg_node) != nullptr;
  }
  int IsExpression(void *sg_node) {
    return isSgExpression((SgNode *)sg_node) != 0;
  }

  /************************** Misc
   * **********************************************/
  /********************** reverse arguments ********************************/
  void InsertStmt_v1(void *sg_ref_, void *sg_stmt_) {
    InsertStmt(sg_stmt_, sg_ref_);
  }
  void InsertAfterStmt_v1(void *sg_ref_, void *sg_stmt_) {
    InsertAfterStmt(sg_stmt_, sg_ref_);
  }
  void AddComment_v1(void *sg_node_before, string sDirective) {
    AddComment(sDirective, sg_node_before);
  }
  void AppendChild_v1(void *sg_child_, void *sg_scope_) {
    AppendChild(sg_scope_, sg_child_);
  }
  void PrependChild_v1(void *sg_child_, void *sg_scope_) {
    PrependChild(sg_scope_, sg_child_);
  }
  // Output
  void GeneratePDF();
  void GenerateCode();
  void sanity_check();
  // floating node means the temporarily generated code that has no been insert
  // to AST
  int is_floating_node(void *sg_node);

  // get valid name
  bool check_declaration(void *kernel_decl, string arr_name);
  void get_valid_local_variable_name(void *kernel_decl, string &var_name);
  bool check_declaration_in_global_scope(void *global_scope, string arr_name);
  void get_valid_global_variable_name(void *global_scope, string &var_name);

  void get_loop_nest_from_ref(void *curr_loop, void *ref, set<void *> &loops,
                              bool include_while = false);
  string legalizeName(const string &name);

  void PropagatePragma(void *var, void *scope, string new_var,
                       bool third_party = false,
                       vector<void *> *new_pragmas = nullptr);

  // Across-Function Analysis
  int get_surrounding_control_stmt(void *sg_pos, void *sg_scope,
                                   vector<void *> &sg_loops_ifs);
  int get_surrounding_control_stmt(void *sg_pos, void *sg_scope,
                                   t_func_call_path func_path,
                                   vector<void *> &sg_loops_ifs,
                                   set<void *> &visited);

  void set_kernel_attribute(void *func_decl, string attr, string val);
  // Add by Yuxin
  SgVariableSymbol *getSymbolFromName(void *scope, string varStr);
  SgInitializedName *getInitializedNameFromName(void *scope, string varStr);
  string GetSgPntrArrName(void *expr);
  bool IsIncludeVar(void *expr, void *var);
  /////////////////////////////////
  vector<void *> get_var_decl_in_for_loop(void *sg_for_loop);
  void *create_for_loop_for_reset_array(void *var_int, void *base_type,
                                        vector<size_t> nSizes, void *val_var,
                                        void *bindNode);
#if USED_CODE_IN_COVERAGE_TEST
  int fix_scope_issue(void *scope = nullptr);
#endif
  void init_defuse();

  void init_callgraph();

#if USED_CODE_IN_COVERAGE_TEST
  int is_single_def_expression(void *exp, void *pos);
#endif
  void InitRenameList(
      map<string, vector<string>>
          &mapRefRenameList); // map<string port, vector<string ref_name_list> >
  void ResetRefRenameList();
  string GetRefRenameAndInc(string port);

#if USED_CODE_IN_COVERAGE_TEST
  void clean_floating_expr_nodes(void *scope,
                                 string str_sg_type = "SgExpression");
#endif

  bool GetTopoOrderFunctions(void *top_func, vector<void *> &functions);

  int GetPntrReferenceOfArray(void *array, void *sg_scope,
                              vector<void *> &vec_access,
                              int allow_artifical_ref = 0);
  int analysis_array_flatten(void *array_init, vector<size_t> &old_dims);
  int apply_array_flatten(void *array_init, vector<size_t> &new_dims);
  int analysis_lift_scalar(void *scalar_init);
  int apply_lift_scalar(void *scalar_init);

  void *rebuild_pntr_ref(
      void *pntr_ref,
      void *sg_pos = nullptr); // rebuild the pntr by value propagation

  void *find_kernel_call(void *sg_pragma);
  string get_task_name_by_kernel(void *kernel);
  string get_wrapper_name_by_kernel(void *kernel);

  void splitString(string str, string delim, vector<string> &results);

#if USED_CODE_IN_COVERAGE_TEST
  int ConvertFuncParamTypePointer2Array(void *sg_func_arg);
#endif

  vector<void *> CopyExprList(vector<void *> org_exp_list);

  ///////////////////////////
  // ZP: 20170817: temporary functions to be removed
  // Replace by CreateFuncDecl and CreateFuncCall
  void appendExp(void *arg_list, void *exp) {
    SageInterface::appendExpression((SgExprListExp *)arg_list,
                                    (SgExpression *)exp);
  }

  void appendArg(void *arg_list, void *init) {
    SageInterface::appendArg((SgFunctionParameterList *)arg_list,
                             (SgInitializedName *)init);
  }
  void *buildFunctionParameterList() {
    return SageBuilder::buildFunctionParameterList();
  }
  void *buildExprListExp() { return SageBuilder::buildExprListExp(); }
  SgInitializedName *CreateVariable_v1(SgName sVar, void *sg_type,
                                       void *bindNode = nullptr);

  ///////////////////////////
  //

  // arr_name[ivar0+offset0][ivar1+offset1][ivar2+offset2]...
#if USED_CODE_IN_COVERAGE_TEST
  void *CreateSimpleArrayRef(void *array_var_ref, vector<void *> vec_var,
                             vector<int> offsets, void *bindNode = nullptr);
#endif
#if USED_CODE_IN_COVERAGE_TEST
  void *BuildExpressFromPolyVector(PolyVector vecDim, vector<void *> vecVarDecl,
                                   int &bit_out); // not suggested
#endif
  int SetScope(void *stmt_, void *scope_);
#if USED_CODE_IN_COVERAGE_TEST
  void *ParseLoopNest(void *sg_stmt, int loop_level, vector<void *> &vec_iter);
#endif

  int get_block_pragma_val(void *loop, string key, long &val);

  // int IsCanonicalForLoop(
  //         SgNode * for_loop,
  //         SgInitializedName **  ivar = nullptr,
  //         SgExpression **  lb = nullptr,
  //         SgExpression **  ub = nullptr,
  //         SgExpression **  step = nullptr,
  //         SgStatement **  body = nullptr,
  //         bool *  hasIncrementalIterationSpace = nullptr,
  //         bool *  isInclusiveUpperBound = nullptr
  //        );

  void *copy_loop_for_c_syntax(void *sg_for_loop, void *sg_scope = nullptr);

  int has_access_in_scope(void *for_loop, vector<string> &vec_names,
                          string &found); // not suggested

  bool ContainsUnsupportedLoop(void *sg_scope, bool recursive = false,
                               bool while_support = false);

  void *GetFuncCallInStmt(void *stmt);
  vector<void *> TraceDownToFuncCall(void *sg_pos);

  // support cross function induct
  bool get_map_ref_to_exp(void *expr, void *bound_scope,
                          map<void *, void *> &map_ref2exp);
  // The major function for copy and replace var reference
  // [Notice] The input will be better in the AST node, because of print
  // debugging
  void *copy_and_replace_var_ref(void *sg_node,
                                 const map<void *, void *> &map_var2exp,
                                 map<void *, void *> *map_replaced = nullptr,
                                 int no_copy = false);

#if USED_CODE_IN_COVERAGE_TEST
  void *copy_node_and_replace_reference(void *sg_node, void *sg_init,
                                        void *new_exp);
#endif

  void *GetFuncDeclByInstDirectiveStmt(void *sg_node);

  void *GetTemplateFuncDecl(void *sg_node);

  void reset_tfn_syms();

  void *GetTemplateClassDecl(void *sg_node);

  void reset_tc_syms();
  string get_template_string(void *sg_node);

  int is_all_dim_array(void *sg_type_in);

  void *GetExprFromStmt(void *sg_node);

  /////////////////////////////////////////////////////////////////////////////////////
  // ZP: 2015_04-30: The formal function to get linear coefficients from an
  // expression.
  // Note:
  // 1. sg_exp is the input expression
  // 2. key of the map item is the name of the variable, if that term is a
  // linear term
  //    otherwise, key of the item is "nullptr", and the value is "1" (nonzero)
  //    a special variable has a key of "1", and the coefficient is the constant
  //    term;
  // 3. the terms of the same variables will be merged, and the
  // 4. Return value: 1 if the expression is linear of loop iters and
  // loop-invariant variables, and 0 for nonlinear
  int analyze_linear_expression(void *sg_exp, map<string, int> &mapVar2Coeff);
#if USED_CODE_IN_COVERAGE_TEST
  int analyze_linear_expression_new(void *sg_exp,
                                    map<void *, int> &mapVar2Coeff);
#endif
  /////////////////////////////////////////////////////////////////////////////////////
  int get_expression_bound_integer(void *sg_exp, int &upper, int &lower);

  // lower is the real lower bound
  // upper is the real upper bound + 1
  int analyze_iteration_domain_order(void *sg_scope_, void *sg_stmt_,
                                     vector<string> &iterators,
                                     vector<int> &loop_indices,
                                     vector<string> &condition,
                                     vector<string> &lower,
                                     vector<string> &upper, int HGT);
  int analyze_iteration_domain_order(void *sg_scope_, void *sg_stmt_,
                                     vector<string> &iterators,
                                     vector<int> &loop_indices,
                                     vector<string> &condition,
                                     vector<void *> &lower,
                                     vector<void *> &upper, int HGT);
  int analyze_iteration_domain(void *sg_scope_, void *sg_stmt_,
                               vector<string> &iterators, vector<string> &lower,
                               vector<string> &upper);
  int analyze_iteration_domain(void *sg_scope_, void *sg_stmt_,
                               vector<string> &iterators, vector<void *> &lower,
                               vector<void *> &upper);
  int analyze_iteration_domain(void *sg_scope_, void *sg_stmt_,
                               PolyMatrix &matrix);
  int get_io_matrix_of_function(
      void *sg_func_decl, map<string, vector<poly_access_pattern>> &mapMatrix,
      vector<string> iterators, vector<string> &params,
      vector<string> &orderVar2Matrix);
  void get_ref_map_from_iter_map(void *scope, map<void *, void *> map_iter2expr,
                                 map<void *, void *> &map_to_insert);

  // not suggested, use CMarsExpression instead
  int simplify_intermediate_variables(void *sg_exp_, void *sg_scope_,
                                      PolyVector &poly_in,
                                      vector<string> parameters);
  int simplify_intermediate_variables(void *sg_exp_, void *sg_scope_,
                                      string &poly_in, vector<string> &vec_vars,
                                      vector<string> parameters);

  // not suggested
  int linearize_arrays(void *sg_bb_or_func_);
  int linearize_arrays_detect(void *sg_bb_or_func_, string var,
                              vector<void *> &bb_collected,
                              vector<void *> &func_collected);
  int linearize_arrays_apply(void *sg_bb_or_func_, string var, void *sg_type_,
                             vector<void *> &bb_collected,
                             vector<void *> &func_collected);

  // not suggested
  int access_wrapper(void *sg_func,
                     map<string, vector<string>> &mapRefRenameList,
                     string func_prefix,
                     map<string, vector<string>> &mapArray2Dim); // wrap all the
  // access of
  // the pointer
  // arguments
  // with
  // function
  // calls
  int one_access_wrapper(void *sg_func_, string sVar, string sPort,
                         string func_prefix,
                         map<string, vector<string>> &mapArray2Dim,
                         map<string, bool> &mapFuncVisited);

  // Outline
#if USED_CODE_IN_COVERAGE_TEST
  void *outline(void *sg_bb_, string func_name, void **sg_new_decl = nullptr);
#endif

  void InsertPipelinePragma(void *scope, vector<string> &vec_bus_e1);

  string GetMallocVarName(void *exp); //, int& elem_size, int& total_size);

public: // for debug
  string _p(void *sgnode,
            int len = 0); // brief form of UnparseToString, with type info
  string _up(void *sgnode, int len = 0) { return UnparseToString(sgnode, len); }
  void *_pa(void *sgnode); // brief form of GetParent()
  void dump_all_func();
  string UnparseToString(void *sg_node, int len = 0);
  string GetASTTypeString(void *sg_node) {
    return ((SgNode *)sg_node)->class_name();
  }
  void reset_func_call_cache() { s_func_call_cache.clear(); }
  void reset_func_decl_cache() { s_func_decl_cache.clear(); }
  void reset_var_ref_cache() { s_var_ref_cache.clear(); }
  void reset_global_decl_cache() { s_global_decl_cache.clear(); }
  void reset_type_decl_cache() { s_type_decl_cache.clear(); }
  void reset_name_type_cache() { s_name_type_cache.clear(); }
  void reset_loop_info_cache() {
    s_loop_iterator_cache.clear();
    s_loop_label_cache.clear();
  }
  void reset_message_metadata_cache() { s_message_metadata_cache.clear(); }
  void reset_access_in_scope_cache() {
    s_write_in_scope_cache.clear();
    s_write_in_scope_fast_cache.clear();
    s_read_in_scope_cache.clear();
  }
  map<SgNode *, HistoryMarker> &getHistories() { return histories; }

  void *get_induct_expr_in_scope(void *var_ref, void *pos, void *boundary_scope,
                                 vector<void *> *inner_terms /* = nullptr*/);
#if USED_CODE_IN_COVERAGE_TEST
  void SetToJson(string filename, string key, const vector<string> &values,
                 string level_1, string level_2);

  vector<string> GetFromJson(string filename, string key, string level_1,
                             string level_2);
#endif

  // If we have a type that could have bitwidth, return it. Otherwise, return
  // boost::none. The second bool param controls whether debug output is
  // emitted upon failure.
  boost::optional<unsigned> get_bitwidth(SgType &, bool report = true);

  boost::optional<unsigned> get_xilinx_bitwidth(SgTemplateInstantiationDecl &,
                                                bool report = true);

  // TODO: This should return an unsigned.
  int get_bitwidth_from_type_str(string str_type, bool report = true);

  bool isMultiAssignHaveSideEffect(void *node);
  bool SplitMultiAssign(void *scope);
  void *convertAssign(void *node);
  bool IsSafeIntConversion(void *exp, void *pos, void *scope, void *type_a,
                           void *type_b);

  string GetSafeNewVarName(string prefix, void *scope);

protected:
#if USED_CODE_IN_COVERAGE_TEST
  vector<string>
  outline_simplify_get_deref_vars(SgFunctionDeclaration *sg_func_decl);
#endif
  // void outline_simplify_one_var(string sFuncName, string sVar);
  void outline_simplify_one_var(SgFunctionDeclaration *sg_func_decl, int i);
  int outline_check_false_deref(SgFunctionDeclaration *sg_func_decl,
                                SgInitializedName *arg);

  int get_io_matrix_of_function_recur(
      void *sg_func_decl, string sPort, int nDim,
      map<string, vector<poly_access_pattern>> &mapMatrix,
      vector<string> iterators);

#if USED_CODE_IN_COVERAGE_TEST
  int function_argument_name_replace(void *sg_call, void *sg_decl,
                                     vector<string> &parameters);
#endif
  void function_argument_volatile_type(void *func_decl, int arg_idx);

  void *pointer_to_array_recur(void *sg_type);

  void *match_var_in_expression(string sVar, void *sg_exp);

  void *trace_base_type(void *sg_type, bool skip_anonymous_struct = true);

  void *is_pointer_ref_in_memcpy(void *var_ref);

  void add_exp_to_vector(vector<void *> &vec, int idx, void *exp,
                         void *pos = nullptr, bool simplify_idx = true);

  int ParseForLoop(SgForStatement *target_loop, SgInitializedName **ivar,
                   SgExpression **lb, SgExpression **ub, SgExpression **step,
                   SgBinaryOp **cond_op, SgStatement **orig_body, int &ulimit);

  // int Expr2Vector(SgExpression * sgExp, PolyVector & poly_vec, int
  // force_linear=0);
  int Expr2Vector(void *sgExp, PolyVector &poly_vec, int force_linear = 0);
  int Expr2Vector(SgExpression *sgExp, string &poly_vec,
                  vector<string> &vec_vars, int force_linear = 0);

  // int Expr2Vector(void * sgExp, vector<void *, int> & m_coeff);

  int get_const_value(void *ub, int &a);

  int is_write_analysis(SgNode *sgnode);

  map<string, vector<string>>
      m_mapRefRenameList; // map<string port, vector<string ref_name> >
  map<string, int>
      m_mapRefRenameIdx; // map<string port, int current_ref_idx_of_the_port>;

  map<void *, vector<void *>> s_func_call_cache;
  map<void *, vector<void *>> s_var_ref_cache;
  map<void *, void *> s_func_decl_cache;
  map<void *, void *> s_global_decl_cache;
  map<void *, void *> s_type_decl_cache;
  map<void *, void *> s_loop_iterator_cache;
  map<void *, bool> s_loop_label_cache;
  map<string, void *> s_name_type_cache;
  map<pair<void *, void *>, bool> s_write_in_scope_cache;
  map<pair<void *, void *>, bool> s_write_in_scope_fast_cache;
  map<pair<void *, void *>, bool> s_read_in_scope_cache;
  map<pair<void *, void *>, bool> s_read_in_scope_fast_cache;
  set<void *> s_def_use_cache;
  map<void *, vector<void *>> s_g_funcs_cache;
  map<pair<void *, void *>, bool> s_location_cache;
  map<string, string> s_message_metadata_cache;
  map<SgNode *, HistoryMarker> histories;
  set<string> m_inputFiles;
  set<string> m_inputIncDirs;
  map<string, string> m_inputMacros;
  map<void *, void *> m_tfn_syms;
  map<void *, void *> m_tc_syms;
  bool m_tfn_syms_valid;
  bool m_tc_syms_valid;

  SgProject *m_sg_project;
  map<string, void *> m_builtin_types;
  set<SgType *> m_new_types;
  vector<SgTypedefDeclaration *> m_builtin_type_decls;

  DefUseAnalysis *m_defuse;
  LivenessAnalysis *m_liveness;
  void *s_liveness_previous_func; // used as cache

  CallGraphBuilder *m_callgraph;

  // for range anlaysis
  void init_range_analysis();

private:
  void HandleCondition(void *cond_exp, bool true_value);
  int IsRecursiveFunction(void *func_decl, void *&recu_func,
                          set<void *> &visited);

  int IsRecursiveType(void *sg_type, void *&rec_type, set<void *> &visited);

  int GetPntrReferenceOfArray(void *array, void *sg_scope,
                              vector<void *> &vec_access, set<void *> &visited,
                              int allow_artifical_ref = 0);

  void replaceExpressionWithStatement(SgExpression *from,
                                      SageInterface::StatementGenerator *to);

  void replaceSubexpressionWithStatement(SgExpression *from,
                                         SageInterface::StatementGenerator *to);

  void
  replaceAssignmentStmtWithStatement(SgExprStatement *from,
                                     SageInterface::StatementGenerator *to);

  void pushTestIntoBody(SgScopeStatement *loopStmt);

  void pushExpressionIntoBody(SgExpression *from);

  SgExpression *getRootOfExpression(SgExpression *exp);

  void myStatementInsert(SgStatement *target, SgStatement *newstmt, bool before,
                         bool allowForInit = false);

  void replaceExpressionWithExpression(SgExpression *from, SgExpression *to);

  void myRemoveStatement(SgStatement *stmt);

  SgAssignOp *convertInitializerIntoAssignment(SgAssignInitializer *init);
};
typedef CSageCodeGen CMarsAST_IF;

int is_cmost_pragma(string sFilter, bool allow_hls = 0);
int tldm_pragma_parse_whole(
    string sPragma, string &sFilter, string &sCmd,
    map<string, pair<vector<string>, vector<string>>> &mapParam,
    bool allow_hls = false);
void all_pragma_parse_whole(
    string sPragma, string &sFilter, string &sCmd,
    map<string, pair<vector<string>, vector<string>>> &mapParam);

class CNameConflictSolver {
public:
  CNameConflictSolver(CSageCodeGen *ast = nullptr, void *scope = nullptr);
  string get_and_assign(string prefix);

protected:
  set<string> m_record;
};
