#include "ds_generator.h"

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
    if (is_fine_grain_) {
      if (!isFineGrained) {
        return;
      }
    }
    if (isFineGrained) {
      // options.pop_back();
      // let auto-parallel do the job
      return;
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
