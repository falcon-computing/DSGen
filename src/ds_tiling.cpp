#include "ds_generator.h"

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
