//===----------------------------------------------------------------------===//
//
// The Merlin(TM) Compiler Design Space Explorer
//
// Copyright (C) Falcon Computing Solutions, Inc - All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
//
//===----------------------------------------------------------------------===//
//
// Owner: Min Gao, Cody Yu
///
/// \file ds_generator.cpp
/// \brief Automatic design space pragma generation for user code.
/// The design space generator automatically generate/append pipeline, tiling
/// and parallel pragmas for each loop scope. The dependency order of the three
/// sorts are pipleline <-- tiling <-- parallel.
//
//===----------------------------------------------------------------------===//

#include "ds_generator.h"

int DsGenerator::InitScopePragmas() {
  vector<CMirNode *> fn_loop_nodes;
  mars_ir_.get_topological_order_nodes(fn_loop_nodes);
  for (auto &node: fn_loop_nodes) {
    if (node->is_function) 
      continue;
    auto *loop_scope = m_ast.GetParent(node->ref); 
    if (!IsOutermostForLoop(m_ast, loop_scope))
      continue;
    

    string loop_id = "L"+ to_string(loop_index_++);
    FnHandler build_pipeline = &DsGenerator::BuildPipeline;
    TraverseLoopDesignSpace(build_pipeline, loop_scope, loop_id);  
    FnHandler build_tiling = &DsGenerator::BuildTiling;
    TraverseLoopDesignSpace(build_tiling, loop_scope, loop_id);  
    FnHandler build_parallel = &DsGenerator::BuildParallel;
    TraverseLoopDesignSpace(build_parallel, loop_scope, loop_id);  
  }

  // As optwidth does not focus on loops but every pragmas
  // We process it separately.
  
  vector<void *> vec_kernels = mars_ir_v2_.get_top_kernels();
  for (auto kernel : vec_kernels) {
    BuildBitWidthOpt(kernel);
  }

  return 0; 
} 

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

void DsGenerator::TraverseLoopDesignSpace(FnHandler fn,
                                          void *stmt, string loop_id) {
  if (m_ast.IsForStatement(stmt) || m_ast.IsWhileStatement(stmt)){
    if (!map_scope_pragmas_.count(stmt)) {
      ScopePragmas *scope_pragmas = new ScopePragmas();
      map_scope_pragmas_[stmt] = scope_pragmas;
    }   
    (this->*fn)(stmt, loop_id);
  }

  int level_idx = 0;
  vector<void *> vec_body;

  // Collect all child basic blocks
  // FIXME: Other blocks?
  if (m_ast.IsForStatement(stmt) || m_ast.IsWhileStatement(stmt))
    vec_body.push_back(m_ast.GetLoopBody(stmt));
  else if (m_ast.IsIfStatement(stmt)) {
    vec_body.push_back(m_ast.GetIfStmtTrueBody(stmt));
    vec_body.push_back(m_ast.GetIfStmtFalseBody(stmt));
  } else {
    cout << "unknown statement " << m_ast._p(stmt) << endl;
    return;
  }

  for (auto body : vec_body) {
    int num_body = m_ast.GetChildStmtNum(body);
    for (auto i = 0; i < num_body; i++) {
      void *child = m_ast.GetChildStmt(body, i);
      if (m_ast.IsLabelStatement(child))
        child = m_ast.GetStmtFromLabel(child);
      if (m_ast.IsForStatement(child) || m_ast.IsWhileStatement(child) ||
          m_ast.IsIfStatement(child)) {
        TraverseLoopDesignSpace(fn, child,
                                loop_id + "_" + to_string(level_idx++));
      }
    }
  }
  return;
    
}

vector<int> DsGenerator::GetAllDivisors(int num, int limit) {
  vector<int> ret;

  int upper = (limit > sqrt(num)) ? sqrt(num) : limit;
  for (int i = 1; i <= upper; ++i) {
    if (num % i != 0)
      continue;

    ret.push_back(i);

    int divisor = num / i;
    if (divisor != i && divisor <= limit) {
      // Avoid duplicates by square root and too large numbers
      ret.push_back(divisor);
    }
  }
  return ret;
}

long DsGenerator::GetTotalDepth(string depth) {
  long ret = 1;
  replace(depth.begin(), depth.end(), ',', ' ');
  istringstream iss(depth);
  vector<string> tokens{istream_iterator<string>{iss},
                        istream_iterator<string>{}};
  for (auto d : tokens)
    ret *= stoi(d);
  return ret;
}

void DsGenerator::SetUserPragmaInfo(void *scope_stmt,
                                    void *pragma_stmt, 
                                    string pragma_id,
                                    PragmaType pragma_ty) {
  map<string, pair<vector<string>, vector<string>>> map_param;
  string str_pragma = m_ast.GetPragmaString(pragma_stmt);
  string sFilter, sCmd;
  tldm_pragma_parse_whole(str_pragma, sFilter, sCmd, map_param);   
  switch (pragma_ty) {
    case PIPELINE: {
      PragmaInfo<string> *pragma_info = new PragmaInfo<string>();
      pragma_info->SetBasicValue(pragma_stmt, pragma_id, pragma_ty);
      if (map_param.count("flatten")) {
        pragma_info->AppendOption("'flatten'");
        pragma_info->SetDefault("flatten");
      } else if (map_param.count("off")) {
        pragma_info->AppendOption("'off'");
        pragma_info->SetDefault("off");
      } else if (map_param.size() > 0){
        pragma_info->SetDefault("invalid");
      } else {
        pragma_info->AppendOption("'on'");
        pragma_info->SetDefault("on");
      }
      ScopePragmas *scope_pragmas = map_scope_pragmas_[scope_stmt];
      scope_pragmas->set(pragma_ty, pragma_info); 
      break;
    } 
    case TILING: 
    case PARALLEL: {
      PragmaInfo<int> *pragma_info = new PragmaInfo<int>();
      pragma_info->SetBasicValue(pragma_stmt, pragma_id, pragma_ty);
      if (map_param.count("factor")) { 
        string factor_str = map_param["factor"].first[0];
        int factor = stoi(factor_str);
        pragma_info->AppendOption(factor);
        pragma_info->SetDefault(factor);
      } else if (map_param.count("FACTOR")) { 
        string factor_str = map_param["FACTOR"].first[0];
        int factor = stoi(factor_str);
        pragma_info->AppendOption(factor);
        pragma_info->SetDefault(factor);
      }
      ScopePragmas *scope_pragmas = map_scope_pragmas_[scope_stmt];
      scope_pragmas->set(pragma_ty, pragma_info); 
      break;
    }
    case INTERFACE: {
      break;
    }
    default: {
      assert(0 && "SetPragmaInfo: unknown keyword.\n");
    }
  }
}

bool DsGenerator::UpdateUserSpecifiedPragma(void *scope_stmt, 
                                            PragmaType pragma_ty,
                                            string pragma_id) {
  string key_word = GetKeyWord(pragma_ty);
  void *label_stmt = scope_stmt;
  void *par_stmt = m_ast.GetParent(scope_stmt); 
  while (m_ast.IsLabelStatement(par_stmt)) {
    label_stmt = par_stmt;
    par_stmt = m_ast.GetParent(label_stmt);
  }
  void *pragma_stmt = m_ast.GetPreviousStmt(label_stmt);
  bool has_key_word = false;
  while (pragma_stmt && m_ast.IsPragmaDecl(pragma_stmt)) {
    string ori_str = m_ast.GetPragmaString(pragma_stmt);
    string str = ori_str;
    transform(str.begin(), str.end(), str.begin(), ::tolower);
    if (str.find(key_word) != string::npos &&
        str.find("accel") != string::npos) {
      has_key_word = true;
      break;
    }
    pragma_stmt = m_ast.GetPreviousStmt(pragma_stmt);
  }
  if (has_key_word) {
    SetUserPragmaInfo(scope_stmt, pragma_stmt, pragma_id, pragma_ty);
    return true;
  }  
  return false;
}

void DsGenerator::GetShadowedParams(void *stmt,
                                    PragmaType pragma_ty,
                                    vector<string> &free_vec,
                                    vector<string> &fixed_vec) {
  void *curr_stmt = m_ast.GetParent(stmt);
  while (curr_stmt) {
    if (m_ast.IsLoopStatement(curr_stmt)) {
      if (!map_scope_pragmas_.count(curr_stmt)) {
        assert(0 && "Parent Loop is not traversed");
      }
      ScopePragmas *scope_pragmas = map_scope_pragmas_[curr_stmt];
      if (scope_pragmas->exist(pragma_ty)) {
        switch(pragma_ty) {
          case PIPELINE: {
            auto pipeline_info = 
              boost::any_cast<PragmaInfo<string> *>(scope_pragmas
                                                    ->get(PIPELINE));
            for (auto &option : pipeline_info->options) {
              if (option == "'flatten'") {
                if (pipeline_info->options.size() > 1)
                  free_vec.push_back(pipeline_info->id);
                else 
                  fixed_vec.push_back(pipeline_info->id);
              }
            }
            break;
          }
          case TILING: {
            auto tiling_info = 
              boost::any_cast<PragmaInfo<int> *>(scope_pragmas
                                                 ->get(TILING));
            if (!tiling_info->options.empty()) {
              free_vec.push_back(tiling_info->id);
            }
            break; 
          }           
          case PARALLEL: {
            auto parallel_info = 
              boost::any_cast<PragmaInfo<int> *>(scope_pragmas
                                                 ->get(PARALLEL));
            if (!parallel_info->options.empty()) {
              free_vec.push_back(parallel_info->id);
            }
            break; 
          }           
          default: {
            ;
          }
        }
      }
    }
    curr_stmt = m_ast.GetParent(curr_stmt);
  } 
}

void DsGenerator::BuildPipeline(void *scope_stmt, 
                               string loop_id) {
  string pragma_id = "__PIPE__" + loop_id; 

  if (!UpdateUserSpecifiedPragma(scope_stmt, PIPELINE, pragma_id)) {
    vector<string> free_ids, fixed_ids; 
    GetShadowedParams(scope_stmt, PIPELINE, free_ids, fixed_ids);
    bool isFineGrained =
      isFineGrainedLoop(m_ast, scope_stmt) && 
      !hasNonFlattenFuncCall(m_ast, scope_stmt);
    
    vector<string> options = {"'off'", "''", "'flatten'"};
    if (isFineGrained) {
      options.pop_back();
    }
 
    /// options refers to Design Space.
    // Build a design space for coarse-grained loops
    // TODO need to check inner most, off at innermost
    string cond = "";
    if (fixed_ids.size() > 0) {
      return;
    } else if (free_ids.size() > 0) { // check parent loop flatten from options
      cond += " if x=='off' or ";
      bool first = true;
      for (auto label : free_ids) {
        if (!first)
          cond += " and ";
        first = false;
        cond += label + "!='flatten'";
      }
    }
  
    // order
    string order = "0 if x!='flatten' else 1";
    if (isFineGrained) 
      order.clear();
    string pragma_str = "ACCEL PIPELINE auto{" + pragma_id + "}";
    string default_val = "off";
    void *new_pragma = m_ast.CreatePragma(pragma_str, scope_stmt);
    
    PragmaInfo<string> *pragma_info = new PragmaInfo<string>();
    pragma_info->SetBasicValue(new_pragma, pragma_id, PIPELINE);
    pragma_info->AppendOption(options);
    pragma_info->SetCond(cond);
    pragma_info->SetOrder(order);
    pragma_info->SetDefault(default_val);
    ScopePragmas *scope_pragmas = map_scope_pragmas_[scope_stmt];
    scope_pragmas->set(PIPELINE, pragma_info);
  } 
}

void DsGenerator::BuildTiling(void *scope_stmt,
                              string loop_id) {
  string pragma_id = "__TILE__" + loop_id;
  if (!UpdateUserSpecifiedPragma(scope_stmt, TILING, pragma_id)) {

    // Build a design space by analyzing loop tripcount
    int64_t tc = 0;
    if (!m_ast.IsForStatement(scope_stmt)) {
      cout << "skip non for-loop" << endl;
      return;
    } else if (!m_ast.get_loop_trip_count(scope_stmt, tc)) {
      cout << "skip due to fail to analyze loop tripcount" << endl;
      return;
    }

    bool is_fine_grained =
        (isFineGrainedLoop(m_ast, scope_stmt) && 
         !hasNonFlattenFuncCall(m_ast, scope_stmt));

    if (!IsTilable(scope_stmt, tc, is_fine_grained)) {
      cout << "skip due to non-tilable" << endl;
      return;
    }

    tc = (tc > MAX_TILE_SIZE) ? MAX_TILE_SIZE : tc;

    // Setup tiling design space
    vector<int> options;
    options.push_back(1);
    for (int i = 2; i < tc; i *= 2)
      options.push_back(i);
    if ((tc & (tc - 1)) != 0) {
      // The upper-bound is not power of two, cover it separately
      options.push_back(tc);
    } 

    vector<string> free_ids, fixed_ids; 
    GetShadowedParams(scope_stmt, TILING, free_ids, fixed_ids);

    string cond = "";
    bool first = true;
    if (free_ids.size() > 0) {
      cond = " if x==1 or ";
      for (auto &other : free_ids) {
        if (!first) {
          cond += " and ";
        }
        first = false;
        cond += other + "==1";
      }
    }
 
    free_ids.clear();
    fixed_ids.clear();
    GetShadowedParams(scope_stmt, PIPELINE, free_ids, fixed_ids);
    if (fixed_ids.size() > 0) {
      return;
    } else if (free_ids.size() > 0) {
      if (first)
        cond = " if x==0 or ";
      for (auto label : free_ids) {
        if (!first)
          cond += " and ";
        first = false;
        cond += label + "!='flatten'";
      }
    } 

    // TODO(mingao) Overwrite for now, will clean up later for json
    string pragmaStr = "ACCEL TILE FACTOR=auto{" + pragma_id + "}";
    void *new_pragma = m_ast.CreatePragma(pragmaStr, scope_stmt);
    int default_val = 1;
    string order = "";

    PragmaInfo<int> *pragma_info = new PragmaInfo<int>();
    pragma_info->SetBasicValue(new_pragma, pragma_id, TILING);
    pragma_info->AppendOption(options);
    pragma_info->SetCond(cond);
    pragma_info->SetOrder(order);
    pragma_info->SetDefault(default_val);
    ScopePragmas *scope_pragmas = map_scope_pragmas_[scope_stmt];
    scope_pragmas->set(TILING, pragma_info); 
  }
}

void DsGenerator::BuildParallel(void *scope_stmt,
                                string loop_id) {
  string pragma_id = "__PARA__" + loop_id;
  if (!UpdateUserSpecifiedPragma(scope_stmt, PARALLEL, pragma_id)) {
    // Build a design space by analyzing loop tripcount
    int64_t tc = 0;
    if (!m_ast.IsForStatement(scope_stmt)) {
      cout << "skip non for-loop" << endl;
      return;
    } else if (!m_ast.get_loop_trip_count(scope_stmt, tc)) {
      cout << "skip due to fail to analyze loop tripcount" << endl;
      return;
    }
  
    cout << "tripcount " << tc << ", ";
  
    if (tc <= 1) {
      cout << "skip due to abnormal loop tripcount" << endl;
      return;
    }
  
    // Candidate parallel factors are divisors and power-of-two numbers.
    // Note that we set the maximum parallel factor to 128 due to
    // resource and design complexity consideratino.
    // We should use a better way to determine it.
    vector<int> factors = GetAllDivisors(tc, 128);
    int limit = (tc < 128) ? tc : 128;
    for (int i = 2; i <= limit; i *= 2)
      factors.push_back(i);
  
    sort(factors.begin(), factors.end());
    bool hasNonPowerOfTwo = false;
    vector<int> options;
    for (int i = 0; i < factors.size(); ++i) {
      if (i == 0 || factors[i] != factors[i - 1]) {
        if ((factors[i] & (factors[i] - 1)) != 0)
          hasNonPowerOfTwo = true;
        options.push_back(factors[i]);
      }
    }

    string cond = "";
    // Setup conditions by looking for the corresponding tiling pragmas
    // Condition 1: tiling factor * parallel factor <= tc
    bool hasTile = false;
    ScopePragmas *tiling_scope_pragmas = map_scope_pragmas_[scope_stmt];
    if (tiling_scope_pragmas->exist(TILING)) {
      auto tiling_info = 
        boost::any_cast<PragmaInfo<int> *>(tiling_scope_pragmas->get(TILING));
      string tile_ds_name = tiling_info->id;
      cond += " if x*" + tile_ds_name + "<=" + to_string(tc);
      hasTile = true;
    }

    // Condition 2: valid only when pipeline pragma at higher loop levels
    // are not set to flatten
    vector<string> free_ids, fixed_ids; 
    GetShadowedParams(scope_stmt, PIPELINE, free_ids, fixed_ids);
    if (fixed_ids.size() > 0) {
      return;
      if (!hasTile)
        cond += " if x==1";
      else
        cond += " and x==1";
    }
    if (free_ids.size() > 0) {
      if (!hasTile)
        cond += " if x==1 or ";
      else
        cond += " and (x==1 or ";
      bool first = true;
      for (auto label : free_ids) {
        if (!first)
          cond += " and ";
        first = false;
        cond += label + "!='flatten'";
      }
      if (hasTile)
        cond += ")";
    } 
  
    // Condition 3: valid only when the activate tiling pragma is at the
    // higher loop level (TODO)

    string order = "0 if x&(x-1)==0 else 1";

    string pragmaStr = "ACCEL PARALLEL FACTOR=auto{" + pragma_id + "}";
    void *new_pragma = m_ast.CreatePragma(pragmaStr, scope_stmt);
    int default_val = 1;

    PragmaInfo<int> *pragma_info = new PragmaInfo<int>();
    pragma_info->SetBasicValue(new_pragma, pragma_id, PARALLEL);
    pragma_info->AppendOption(options);
    pragma_info->SetCond(cond);
    pragma_info->SetOrder(order);
    pragma_info->SetDefault(default_val);
    ScopePragmas *scope_pragmas = map_scope_pragmas_[scope_stmt];
    scope_pragmas->set(PARALLEL, pragma_info); 
  }
}

void DsGenerator::InsertCreatedPragmasAndOutputJson() {
  for (auto &scope_pragmas: map_scope_pragmas_) {
    auto pragmas = scope_pragmas.second;
    if (pragmas->exist(PIPELINE)) {
      auto pipeline_pragma_info = 
        boost::any_cast<PragmaInfo<string> *>(pragmas->get(PIPELINE));
      auto pipeline_pragma = pipeline_pragma_info->pragma;
      if (m_ast.is_floating_node(pipeline_pragma)) {
        m_ast.InsertStmt(pipeline_pragma, scope_pragmas.first);
        pipeline_pragma_info->PrintToJson(fout, item);
      }
    }
    
    if (pragmas->exist(TILING)) {
      auto tiling_pragma_info = 
        boost::any_cast<PragmaInfo<int> *>(pragmas->get(TILING));
      auto tiling_pragma = tiling_pragma_info->pragma;
      if (m_ast.is_floating_node(tiling_pragma)) {
        m_ast.InsertStmt(tiling_pragma, scope_pragmas.first);
        tiling_pragma_info->PrintToJson(fout, item);
      }
    }

    if (pragmas->exist(PARALLEL)) {
      auto parallel_pragma_info = 
        boost::any_cast<PragmaInfo<int> *>(pragmas->get(PARALLEL));
      auto parallel_pragma = parallel_pragma_info->pragma;
      if (m_ast.is_floating_node(parallel_pragma)) {
        m_ast.InsertStmt(parallel_pragma, scope_pragmas.first);
        parallel_pragma_info->PrintToJson(fout, item);
      }
    }

    if (pragmas->exist(INTERFACE)) {
      auto interface_pragma_info = 
        boost::any_cast<PragmaInfo<int> *>(pragmas->get(INTERFACE));
      auto interface_pragma = interface_pragma_info->pragma;
      if (m_ast.is_floating_node(interface_pragma)) {
        void *kernel = interface_pragma_info->pos;
        m_ast.InsertStmt(interface_pragma, 
                         m_ast.GetChildStmt(m_ast.GetFuncBody(kernel), 0)
                        );
        interface_pragma_info->PrintToJson(fout, item);
      }
    }
  }
  fout << item;
}

int ds_generator_top(CSageCodeGen &m_ast, void *pTopFunc,
                    CInputOptions &options) {

  cout << "=====================================================" << endl;
  cout << "--=# Design Space Generator Start#=--\n";
  cout << "=====================================================" << endl;

  DsGenerator dg(m_ast, pTopFunc);
  dg.InitScopePragmas();
  dg.InsertCreatedPragmasAndOutputJson();

  cout << "=====================================================" << endl;
  cout << "--=# Design Space Generator End#=--\n";
  cout << "=====================================================" << endl;

  return 1;
}
