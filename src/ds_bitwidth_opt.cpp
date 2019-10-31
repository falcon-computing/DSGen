#include "ds_generator.h"

void DsGenerator::BuildBitWidthOpt(void *kernel) {
  set<string> set_ignored;
  vector<void *> vecPragmas;
  m_ast.GetNodesByType(kernel, "preorder", "SgPragmaDeclaration", &vecPragmas);
  for (auto pragma : vecPragmas) {
    string s = m_ast.GetPragmaString(pragma);
    string s_lower = s;
    transform(s_lower.begin(), s_lower.end(), s_lower.begin(), ::tolower);

    if (s_lower.find("bus_bitwidth") == string::npos &&
        s_lower.find("accel interface") == string::npos)
      continue;
    int l = s.find(" ", s.find("variable=")) - s.find("variable=") - 9;
    string varName = s.substr(s.find("variable=") + 9, l);

    // Ignore predefined fixed value
    if (s_lower.find("bus_bitwidth") != string::npos) {
      set_ignored.insert(varName);
      cout << "Ignore " << varName << " due to user-specified bitwidth";
      cout << endl;
      continue;
    }

    // Ignore array size < 1024
    long depth = 0;
    if (s_lower.find("max_depth") != string::npos) {
      int l = s.find(" ", s.find("max_depth=")) - s.find("max_depth=") - 10;
      depth = GetTotalDepth(s.substr(s.find("max_depth=") + 10, l));
    } else if (s_lower.find("depth") != string::npos) {
      int l = s.find(" ", s.find("depth=")) - s.find("depth=") - 6;
      depth = GetTotalDepth(s.substr(s.find("depth=") + 6, l));
    } else {
      cout << "Both max_depth and depth are missing for " << varName;
      cout << endl;
      return;
    }

    if (depth < 1024) {
      set_ignored.insert(varName);
      cout << "Ignore " << varName << " due to array length < 1024";
      cout << endl;
    }
  }

  // TODO (Min Gao): Filter out params in interface that has array size < 512
  // Insert pragmas for function interface
  int bw_idx = 0;
  for (int i = 0; i < m_ast.GetFuncParamNum(kernel); i++) {
    void *param = m_ast.GetFuncParam(kernel, i);
    void *ptype = m_ast.GetTypebyVar(param);
    string argName = m_ast.GetVariableName(param);

    // Ignore scalar
    if (!m_ast.IsPointerType(ptype) && !m_ast.IsArrayType(ptype)) {
      cout << "Ignore " << argName << " due to scalar type";
      cout << endl;
      continue;
    }

    // Ignore other types
    void *ptypeBase = m_ast.get_array_base_type(param);
    if (!m_ast.IsIntType(ptypeBase) && !m_ast.IsFloatType(ptypeBase) &&
        !m_ast.IsCharType(ptypeBase) && !m_ast.IsLongLongType(ptypeBase)) {
      cout << "Ignore " << argName << " due to non-primitive types";
      cout << endl;
      continue;
    }

    if (set_ignored.find(argName) != set_ignored.end())
      continue;

    void *arr;
    vector<void *> arr_indices;
    SgArrayType *arr_type = (SgArrayType *)ptype;
    SgExpression *arr_idx = arr_type->get_index();
    CMarsExpression arr_size(arr_idx, &m_ast);
    if (!arr_size.IsConstant())
      continue;
    if (arr_size.GetConstant() < 512)
      continue;

    // Only explore the min (do nothing) and the max (coalescing)
    string options = "[x for x in [32, 512]]";
    int default_val = 32;
    string pragma_id = "__BW__" + m_ast.GetFuncName(kernel) + "_"
                       + to_string(bw_idx++);
    string DS = "auto{" + pragma_id + "}";
    void *new_pragma = m_ast.CreatePragma("ACCEL interface variable=" + argName +
                                             " bus_bitwidth=" + DS,
                                         m_ast.GetGlobal(kernel));

    if (!map_scope_pragmas_.count(param)) {
      ScopePragmas *scope_pragmas = new ScopePragmas();
      map_scope_pragmas_[param] = scope_pragmas;
    }   
    PragmaInfo<int> *pragma_info = new PragmaInfo<int>();
    pragma_info->SetBasicValue(new_pragma, pragma_id, INTERFACE, kernel);
    pragma_info->AppendOption(32);
    pragma_info->AppendOption(512);
    pragma_info->SetDefault(default_val);
    ScopePragmas *scope_pragmas = map_scope_pragmas_[param];
    scope_pragmas->set(INTERFACE, pragma_info);
  } 
}

