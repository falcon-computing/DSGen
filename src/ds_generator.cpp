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

bool DsGenerator::CanonicalizeLoop() {
  bool ret = false;
  vector<CMirNode *> vec_nodes;
  mars_ir_.get_topological_order_nodes(&vec_nodes);

  for (int i = static_cast<int>(vec_nodes.size() - 1); i > 0; i--) {
    CMirNode *node = vec_nodes[i];
    if (node->is_function) {
      continue;
    }
    void *loop_body = node->ref;
    void *sg_loop = m_ast.GetParent(loop_body);
    int ret1 = m_ast.CanonicalizeForLoop(&sg_loop, true);

    ret |= ret1;
  }
  return ret;
}

bool DsGenerator::StandardizeLoop() {
  vector<CMirNode *> vec_nodes;
  mars_ir_.get_topological_order_nodes(&vec_nodes);
  bool ret = false;

  for (int i = static_cast<int>(vec_nodes.size() - 1); i > 0; i--) {
    CMirNode *node = vec_nodes[i];
    if (node->is_function) {
      continue;
    }
    void *loop_body = node->ref;
    void *sg_loop = m_ast.GetParent(loop_body);
    ret |= m_ast.StandardizeForLoop(&sg_loop);
  }
  return ret;
}

void DsGenerator::CanonicalizeIR() {
    map<void *, string> loop2file;
    map<void *, string> loop2line;
    clear_mars_ir();
    build_mars_ir(false, false, true);

    bool Changed = false;
    Changed |= CanonicalizeLoop();

    if (Changed) {
      m_ast.reset_func_decl_cache();
      m_ast.reset_func_call_cache();
      m_ast.init_defuse_range_analysis();
      clear_mars_ir();
      build_mars_ir(false, false, true);
    }

    Changed |= StandardizeLoop();

    if (Changed) {
      m_ast.reset_func_decl_cache();
      m_ast.reset_func_call_cache();
      m_ast.init_defuse_range_analysis();
    }
}

int DsGenerator::InitScopePragmas() {
  CanonicalizeIR();
  vector<CMirNode *> fn_loop_nodes;
  mars_ir_.get_topological_order_nodes(&fn_loop_nodes);
  for (auto &node: fn_loop_nodes) {
    if (node->is_function) 
      continue;
    auto *loop_scope = m_ast.GetParent(node->ref); 
    if (!IsLoopStatement(m_ast, loop_scope))
      continue;
    // if (!IsOutermostForLoop(m_ast, loop_scope))
    //   continue;
    

    string loop_id = "L"+ to_string(loop_index_++);
    //TODO (Min) As a showcase we disable these functions
    FnHandler build_pipeline = &DsGenerator::BuildPipeline;
    TraverseLoopDesignSpace(build_pipeline, loop_scope, loop_id);  
    FnHandler build_tiling = &DsGenerator::BuildTiling;
    TraverseLoopDesignSpace(build_tiling, loop_scope, loop_id);  
    FnHandler build_parallel = &DsGenerator::BuildParallel;
    TraverseLoopDesignSpace(build_parallel, loop_scope, loop_id);  
  }

  // As optwidth does not focus on loops but every pragmas
  // We process it separately.
  
  // TODO(Min) Potential bugs on program analysis.
  // Will reenable after the issue has been filed and fixed.
  //vector<void *> vec_kernels = mars_ir_v2_.get_top_kernels();
  //for (auto kernel : vec_kernels) {
  //  BuildBitWidthOpt(kernel);
  //}

  return 0; 
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
  tldm_pragma_parse_whole(str_pragma, &sFilter, &sCmd, &map_param);   
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

void DsGenerator::InsertCreatedPragmasAndOutputJson() {
  Json::Value config_values;
  config_values["project.name"] = "dse_project";// TODO (Min)replace with proj name;
  config_values["project.backup"] = "BACKUP_ERROR";
  config_values["project.fast-output-num"] = 4;
  config_values["timeout.exploration"] = 90;
  config_values["timeout.transform"] = 5;
  config_values["timeout.hls"] = 20;
  config_values["timeout.bitgen"] = 480;
  config_values["evaluate.command.transform"] = "make mcc_acc";
  config_values["evaluate.command.hls"] = "make mcc_estimate";
  config_values["evaluate.command.bitgen"] = "make mcc_bitgen";
  config_values["evaluate.worker-per-part"] = 2;
  config_values["evaluate.max-util.BRAM"] = 0.8;
  config_values["evaluate.max-util.DSP"] = 0.8;
  config_values["evaluate.max-util.LUT"] = 0.8;
  config_values["evaluate.max-util.FF"] = 0.8;
  config_values["search.algorithm.name"] = "gradient";
  config_values["search.algorithm.exhaustive.batch-size"] = 2;
  config_values["search.algorithm.gradient.latency-threshold"] = 64;
  config_values["search.algorithm.gradient.fine-grained-first"] = true;
  config_values["search.algorithm.gradient.quality-type"] = "performance";
  config_values["design-space.max-part-num"] = 4;
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
  config_values["design-space.definition"] = item,
  fout << config_values;
}

int ds_generator_top(CSageCodeGen &m_ast, void *pTopFunc,
                    CInputOptions &options) {

  cout << "=====================================================" << endl;
  cout << "--=# Design Space Generator Start#=--\n";
  cout << "=====================================================" << endl;

  DsGenerator dg(m_ast, pTopFunc, options);
  dg.InitScopePragmas();
  dg.InsertCreatedPragmasAndOutputJson();

  cout << "=====================================================" << endl;
  cout << "--=# Design Space Generator End#=--\n";
  cout << "=====================================================" << endl;

  return 1;
}
