#pragma once

#include "cmdline_parser.h"
#include "codegen.h"
#include "file_parser.h"
#include "mars_ir.h"
#include "mars_ir_v2.h"
#include "mars_opt.h"
#include "program_analysis.h"
#include "xml_parser.h"

// SgClassType->SgClassDeclaration->SgClassDefinition->SgDeclarationStatementList
// struct expression
// 1) Pntr or array access
// 2) SgDotExp
//   SgArrowExp or field access
enum type_kind {
  SCALAR_TYPE = 0,
  SUPPORTED_COMPOUND_TYPE = 1,
  UNSUPPORTED_COMPOUND_TYPE = 2,
};
class StructDecompose {
  CMarsIr mMarsIr;
  CMarsIrV2 mMarsIrV2;
  CSageCodeGen *m_ast;
  void *mTopFunc;
  vector<void *> mKernelFuncs;
  set<void *> mKernelFuncsSet;
  set<void *> mTopKernel;
  // from original struct variables to
  // decomposed fields
  map<void *, vector<void *>> mVar2Members;
  bool mErrorOut;
  string mTool_Type;
  set<string> mCheckedIdentifier;
  bool mCheckDimensionSize;
  map<string, pair<void *, vector<size_t>>> mVar2ZeroDims;
  map<string, int> mBuiltInFunc;

  set<void *> mUsedFields;

public:
  StructDecompose(CSageCodeGen &codegen, void *pTopFunc, CInputOptions &option)
      : m_ast(&codegen), mTopFunc(pTopFunc), mCheckDimensionSize(true) {
    mTool_Type = option.get_option_key_value("-a", "impl_tool");
    if (option.get_option_key_value("-a", "ignore_unknown_dim") != "")
      mCheckDimensionSize = false;
    init();
  }

  void init();

  int run();

  void check();

  int preprocess();

  int fail() { return mErrorOut; }

private:
  enum type_kind containsCompoundType(void *sg_type) {
    string type_info;
    return containsCompoundType(sg_type, type_info);
  }

  enum type_kind containsCompoundType(void *sg_type, string &type_info) {
    void *base_type = m_ast->GetBaseType(sg_type);
    base_type = m_ast->GetOrigTypeByTypedef(base_type, true);
    if (m_ast->IsScalarType(base_type) || cannotDecomposeType(base_type))
      return SCALAR_TYPE;
#if 0
      void *unsupported_type = nullptr;
      string reason;
      if (m_ast->ContainsUnSupportedType(base_type, unsupported_type, reason)) {
        cout << "find unsupported type: " << reason << endl;
#if PROJDEBUG
        cout << m_ast->_up(unsupported_type) << endl;
#endif
        set<pair<void*, string> > all_unsupported_types;
        m_ast->GetAllUnSupportedType(base_type, all_unsupported_types);
        for (auto type_pair : all_unsupported_types)
          type_info += type_pair.second + ": " + m_ast->_up(type_pair.first) + "\n";
        return UNSUPPORTED_COMPOUND_TYPE;
      }
#endif
    if (m_ast->IsStructureType(base_type) || m_ast->IsClassType(base_type)) {

      string class_type = m_ast->GetTypeNameByType(base_type, true);
      if (mTool_Type == "sdaccel") {
        if (class_type.find("ap_int") == 0 || class_type.find("ap_uint") == 0 ||
            class_type.find("ap_fixed") == 0 ||
            class_type.find("ap_ufixed") == 0 || class_type.find("hls::") == 0)
          return SCALAR_TYPE;
      }
      if (class_type.find("merlin_stream") != string::npos)
        return SCALAR_TYPE;

      return SUPPORTED_COMPOUND_TYPE;
    }
    // FIXME: we only struct/class type now
    return UNSUPPORTED_COMPOUND_TYPE;
  }

  int containsUnsupportedType(void *sg_type, string &type_info) {
    void *base_type = m_ast->GetBaseType(sg_type);
    base_type = m_ast->GetOrigTypeByTypedef(base_type, true);
    if (m_ast->IsEnumType(base_type) || m_ast->IsUnionType(base_type) ||
        m_ast->IsIntegerType(base_type) || m_ast->IsFloatType(base_type))
      return 0;
    if (m_ast->IsStructureType(base_type) || m_ast->IsClassType(base_type)) {

      string class_type = m_ast->GetTypeNameByType(base_type, true);
      if (mTool_Type == "sdaccel") {
        if (class_type.find("ap_int") == 0 || class_type.find("ap_uint") == 0 ||
            class_type.find("ap_fixed") == 0 ||
            class_type.find("ap_ufixed") == 0 || class_type.find("hls::") == 0)
          return 0;
      }
      if (class_type.find("merlin_stream") != string::npos)
        return 0;
    }
    string reason;
    void *unsupported_type = nullptr;
    if (m_ast->ContainsUnSupportedType(base_type, unsupported_type, reason)) {
      cout << "find unsupported type: " << reason << endl;
#if PROJDEBUG
      cout << m_ast->_up(unsupported_type) << endl;
#endif
      set<pair<void *, string>> all_unsupported_types;
      m_ast->GetAllUnSupportedType(base_type, all_unsupported_types);
      for (auto type_pair : all_unsupported_types)
        type_info +=
            type_pair.second + ": " + m_ast->_up(type_pair.first) + "\n";
      return 1;
    }
    type_info += "unknown type: " + m_ast->_up(base_type) + "\n";
    // FIXME: we only struct/class type now
    return 1;
  }

  bool cannotDecomposeType(void *sg_type) {
    void *base_type = m_ast->GetBaseType(sg_type);
    base_type = m_ast->GetOrigTypeByTypedef(base_type, true);
    if (m_ast->IsScalarType(base_type))
      return true;
    if (m_ast->IsRecursiveType(base_type))
      return true;
    if (m_ast->IsFunctionType(base_type))
      return true;
    if (m_ast->IsStructureType(base_type) || m_ast->IsClassType(base_type)) {

      string class_type = m_ast->GetTypeNameByType(base_type, true);
      if (mTool_Type == "sdaccel") {
        if (class_type.find("ap_int") == 0 || class_type.find("ap_uint") == 0 ||
            class_type.find("ap_fixed") == 0 ||
            class_type.find("ap_ufixed") == 0 || class_type.find("hls::") == 0)
          return true;
      }

      if (class_type.find("merlin_stream") != string::npos)
        return true;

      return false;
    }
    // unknown type
    return true;
  }

  int decomposeParameter(void *arg, void *scope);

  int decomposeLocalVariable(void *var_decl, void *scope);

  int decomposeGlobalVariable(void *global_decl, void *scope);

  void decomposePragma(void *var_init, void *scope);

  void replaceVariableRefs(void *var_init, void *scope);
  // input: struct type
  // output: a group of pair of name prefix and leaf node final type
  int decomposeType(void *sg_type,
                    vector<tuple<string, void *, bool, bool>> &res, void *pos,
                    string &diagnosis_info, bool filter_unused_fields,
                    void *sg_modifier_type = nullptr);

  int decomposeType(void *sg_type,
                    vector<tuple<string, void *, bool, bool>> &res, void *pos,
                    bool filter_unused_fields) {
    string diagnosis_info;
    return decomposeType(sg_type, res, pos, diagnosis_info,
                         filter_unused_fields);
  }

  int canSafeDecompose(void *var_init, const vector<void *> &refs,
                       string &diagnosis_info);

  int canSafeDecompose(void *var_init, const vector<void *> &refs) {
    string diagnosis_info;
    return canSafeDecompose(var_init, refs, diagnosis_info);
  }

  void *decomposeFunction(void *func_decl, bool &is_dead);

  void *removeUnusedStructArguments(void *func_decl);

  int decomposeExpression(void *sg_exp, vector<void *> &vec_exp,
                          bool filter_unused_fields);

  int decomposeVariableRef(void *sg_var_ref, void *&orig_exp,
                           vector<void *> &vec_new_exp, bool generate_new_expr,
                           bool replace);

  int isKernelFunction(void *func_decl);

  int get_leaf_expression_from_var_ref(void *var_ref, void *&leaf_exp,
                                       bool filter_unused_fields);

  void *createBuiltInAssignFunc(const string &func_name, void *struct_type,
                                void *caller_func, void *bindNode);

  void *convertStructReturnTypeIntoParameter(void *func);

  void replaceLocalVariableReference(void *new_body, void *orig_body);

  int decomposeInitializer(void *orig_init, vector<void *> &vec_new_init) {
    string diagnosis_info;
    return decomposeInitializer(orig_init, vec_new_init, false, diagnosis_info);
  }

  int canDecomposeInitializer(void *orig_init, string &diagnosis_info) {
    vector<void *> vec_new_init;
    return decomposeInitializer(orig_init, vec_new_init, true, diagnosis_info);
  }

  int decomposeInitializer(void *orig_init, vector<void *> &vec_new_init,
                           bool check, string &diagnosis_info);

  vector<size_t> get_array_dimension(string field_name, int &cont_flag,
                                     void *pos, vector<void *> &vec_pragma);

  vector<string> get_array_depth(string field_name, void *pos,
                                 vector<void *> &vec_pragma);

  int has_check(const string &identifier) {
    if (mCheckedIdentifier.count(identifier) > 0)
      return 1;
    mCheckedIdentifier.insert(identifier);
    return 0;
  }

  bool remove_dead_code(void *func_decl);

  void getClassDataMembers(void *sg_type, vector<void *> &res);

  bool is_pointer_test(void *exp);

  bool check_valid_field_name(string field_name, void *pos);

  void collect_used_fields();

  bool contain_used_or_non_pointer_field(void *var_decl);

  string get_qualified_member_name(void *var_init);

  void split_expression_with_side_effect(void *exp);
};
