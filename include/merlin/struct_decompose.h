#pragma once
#include <map>
#include <tuple>
#include <set>
#include <vector>
#include <utility>
#include <string>
#include <unordered_map>

#include "cmdline_parser.h"
#include "codegen.h"
#include "file_parser.h"
#include "mars_ir.h"
#include "mars_ir_v2.h"
#include "mars_opt.h"
#include "program_analysis.h"
#include "xml_parser.h"

//  SgClassType->SgClassDeclaration->SgClassDefinition->SgDeclarationStatementList
//  struct expression
//  1) Pntr or array access
//  2) SgDotExp
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
  std::vector<void *> mKernelFuncs;
  std::set<void *> mKernelFuncsSet;
  std::set<void *> mTopKernel;
  //  from original struct variables to
  //  decomposed fields
  std::map<void *, std::vector<void *>> mVar2Members;
  bool mErrorOut;
  std::string mTool_Type;
  std::set<std::string> mCheckedIdentifier;
  bool mCheckDimensionSize;
  std::map<std::string, std::pair<void *, std::vector<size_t>>> mVar2ZeroDims;
  std::map<std::string, int> mBuiltInFunc;

  std::set<void *> mUsedFields;

 public:
  StructDecompose(CSageCodeGen *codegen, void *pTopFunc,
                  const CInputOptions &option)
      : m_ast(codegen), mTopFunc(pTopFunc), mCheckDimensionSize(true) {
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
    std::string type_info;
    return containsCompoundType(sg_type, &type_info);
  }

  enum type_kind containsCompoundType(void *sg_type, std::string *type_info) {
    void *base_type = m_ast->GetBaseType(sg_type);
    base_type = m_ast->GetOrigTypeByTypedef(base_type, true);
    if (m_ast->IsScalarType(base_type) || cannotDecomposeType(base_type))
      return SCALAR_TYPE;
#if 0
      void *unsupported_type = nullptr;
      std::string reason;
      if (m_ast->ContainsUnSupportedType(base_type, unsupported_type, reason)) {
        cout << "find unsupported type: " << reason << endl;
#if PROJDEBUG
        cout << m_ast->_up(unsupported_type) << endl;
#endif
        std::set<std::pair<void*, std::string> > all_unsupported_types;
        m_ast->GetAllUnSupportedType(base_type, all_unsupported_types);
        for (auto type_pair : all_unsupported_types)
          *type_info += type_pair.second + ": " +
            m_ast->_up(type_pair.first) + "\n";
        return UNSUPPORTED_COMPOUND_TYPE;
      }
#endif
    if (m_ast->IsStructureType(base_type) || m_ast->IsClassType(base_type)) {
      std::string class_type = m_ast->GetTypeNameByType(base_type, true);
      if (mTool_Type == "sdaccel") {
        if (class_type.find("ap_int") == 0 || class_type.find("ap_uint") == 0 ||
            class_type.find("ap_fixed") == 0 ||
            class_type.find("ap_ufixed") == 0 || class_type.find("hls::") == 0)
          return SCALAR_TYPE;
      }
      if (class_type.find("merlin_stream") != std::string::npos)
        return SCALAR_TYPE;

      return SUPPORTED_COMPOUND_TYPE;
    }
    //  FIXME: we only struct/class type now
    return UNSUPPORTED_COMPOUND_TYPE;
  }

  int containsUnsupportedType(void *sg_type, std::string *type_info) {
    void *base_type = m_ast->GetBaseType(sg_type);
    base_type = m_ast->GetOrigTypeByTypedef(base_type, true);
    if (m_ast->IsEnumType(base_type) || m_ast->IsUnionType(base_type) ||
        m_ast->IsIntegerType(base_type) || m_ast->IsFloatType(base_type))
      return 0;
    if (m_ast->IsStructureType(base_type) || m_ast->IsClassType(base_type)) {
      std::string class_type = m_ast->GetTypeNameByType(base_type, true);
      if (mTool_Type == "sdaccel") {
        if (class_type.find("ap_int") == 0 || class_type.find("ap_uint") == 0 ||
            class_type.find("ap_fixed") == 0 ||
            class_type.find("ap_ufixed") == 0 || class_type.find("hls::") == 0)
          return 0;
      }
      if (class_type.find("merlin_stream") != std::string::npos)
        return 0;
    }
    std::string reason;
    void *unsupported_type = nullptr;
    if (m_ast->ContainsUnSupportedType(base_type, &unsupported_type, &reason,
                                       mTool_Type != "sdaccel")) {
      cout << "find unsupported type: " << reason << endl;
#if PROJDEBUG
      cout << m_ast->_up(unsupported_type) << endl;
#endif
      std::set<std::pair<void *, std::string>> all_unsupported_types;
      m_ast->GetAllUnSupportedType(base_type, &all_unsupported_types);
      for (auto type_pair : all_unsupported_types)
        *type_info +=
            type_pair.second + ": " + m_ast->_up(type_pair.first) + "\n";
      return 1;
    }
    *type_info += "unknown type: " + m_ast->_up(base_type) + "\n";
    //  FIXME: we only struct/class type now
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
      std::string class_type = m_ast->GetTypeNameByType(base_type, true);
      if (mTool_Type == "sdaccel") {
        if (class_type.find("ap_int") == 0 || class_type.find("ap_uint") == 0 ||
            class_type.find("ap_fixed") == 0 ||
            class_type.find("ap_ufixed") == 0 || class_type.find("hls::") == 0)
          return true;
      }

      if (class_type.find("merlin_stream") != std::string::npos)
        return true;

      return false;
    }
    //  unknown type
    return true;
  }

  int decomposeParameter(void *var_init, void *scope);

  int decomposeLocalVariable(void *var_init, void *scope);

  int decomposeGlobalVariable(void *var_init, void *scope);

  void decomposePragma(void *var_init, void *scope);

  void replaceVariableRefs(void *var_init, void *scope);
  //  input: struct type
  //  output: a group of pair of name prefix and leaf node final type
  int decomposeType(
      void *sg_type,
      std::vector<std::tuple<std::string, void *, bool, bool>> *res, void *pos,
      std::string *diagnosis_info, bool filter_unused_type,
      void *sg_modifier_type = nullptr);

  int decomposeType(
      void *sg_type,
      std::vector<std::tuple<std::string, void *, bool, bool>> *res, void *pos,
      bool filter_unused_fields) {
    std::string diagnosis_info;
    return decomposeType(sg_type, res, pos, &diagnosis_info,
                         filter_unused_fields);
  }

  int canSafeDecompose(void *var_init, const std::vector<void *> &refs,
                       std::string *diagnosis_info);

  int canSafeDecompose(void *var_init, const std::vector<void *> &refs) {
    std::string diagnosis_info;
    return canSafeDecompose(var_init, refs, &diagnosis_info);
  }

  void *decomposeFunction(void *func_decl, bool *is_dead);

  void *removeUnusedStructArguments(void *func_decl);

  int decomposeExpression(void *leaf_exp, std::vector<void *> *vec_exp,
                          bool filter_unused_fields);

  int decomposeVariableRef(void *sg_var_ref, void **leaf_exp,
                           std::vector<void *> *vec_new_exp,
                           bool generate_new_expr, bool replace);

  int isKernelFunction(void *func_decl);

  int get_leaf_expression_from_var_ref(void *var_ref, void **leaf_exp,
                                       bool filter_unused_fields);
#if 0
  void *createBuiltInAssignFunc(const std::string &func_name, void *struct_type,
                                void *caller_decl, void *bindNode);
#else
  std::vector<void *> InlineBuiltInAssign(void *assign_exp, void *struct_type,
                                          void *caller_decl, void *scope,
                                          void *bindNode);
#endif
  void *convertStructReturnTypeIntoParameter(void *func);

  void replaceLocalVariableReference(void *new_body, void *orig_body);

  int decomposeInitializer(void *orig_init, std::vector<void *> *vec_new_init) {
    std::string diagnosis_info;
    return decomposeInitializer(orig_init, vec_new_init, false,
                                &diagnosis_info);
  }

  int canDecomposeInitializer(void *orig_init, std::string *diagnosis_info) {
    std::vector<void *> vec_new_init;
    return decomposeInitializer(orig_init, &vec_new_init, true, diagnosis_info);
  }

  int decomposeInitializer(void *initializer,
                           std::vector<void *> *new_initalizer, bool check,
                           std::string *diagnosis_info);

  std::vector<size_t>
  get_array_dimension(std::string field_name, int *cont_flag, void *pos,
                      const std::vector<void *> &vec_pragma);

  std::vector<std::string>
  get_array_depth(std::string field_name, void *pos,
                  const std::vector<void *> &vec_pragma);

  int has_check(const std::string &identifier) {
    if (mCheckedIdentifier.count(identifier) > 0)
      return 1;
    mCheckedIdentifier.insert(identifier);
    return 0;
  }

  bool remove_dead_code(void *func_decl);

  void getClassDataMembers(void *sg_type, std::vector<void *> *res);

  bool is_pointer_test(void *exp);

  bool check_valid_field_name(std::string field_name, void *pos);

  void collect_used_fields();

  bool contain_used_or_non_pointer_field(void *var_decl);

  std::string get_qualified_member_name(void *var_init);

  void split_expression_with_side_effect(void *exp);
};
