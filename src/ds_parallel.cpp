#include "ds_generator.h"

void DsGenerator::BuildParallel(void *scope_stmt,
                                string loop_id) {
  bool isFineGrained =
    isFineGrainedLoop(m_ast, scope_stmt) && 
    !hasNonFlattenFuncCall(m_ast, scope_stmt);
  
  if (is_fine_grain_) {
    if (!isFineGrained) {
      return;
    }
  }

  string pragma_id = "__PARA__" + loop_id;
  if (!UpdateUserSpecifiedPragma(scope_stmt, PARALLEL, pragma_id)) {
    // Build a design space by analyzing loop tripcount
    int64_t tc = 0;
    if (!m_ast.IsForStatement(scope_stmt)) {
      cout << "skip non for-loop" << endl;
      return;
    } else if (!m_ast.get_loop_trip_count(scope_stmt, &tc)) {
      cout << "skip due to fail to analyze loop tripcount" << endl;
      return;
    }
  
    cout << "tripcount " << tc << ", ";
  
    if (tc <= 1) {
      cout << "skip due to abnormal loop tripcount" << endl;
      return;
    } else if (tc <= 32) {
      cout << "leave this parallel to auto parallel" << endl;
      return;
    }
  
    // Candidate parallel factors are divisors and power-of-two numbers.
    // Note that we set the maximum parallel factor to 32 due to
    // resource and design complexity consideratino.
    // We should use a better way to determine it.
    vector<int> factors = GetAllDivisors(tc, 32);
    int limit = (tc < 32 ? tc : 32);
    for (int i = 2; i <= limit; i *= 2)
      factors.push_back(i);
    if (factors.back() != tc)
      factors.push_back(tc);
  
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
