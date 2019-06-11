#ifndef ANALPRAG_H
#define ANALPRAG_H

#include "codegen.h"
#include "ir_types.h"
#include "rose.h"

typedef std::list<SgNode *> task_list_type;

struct pragma_tag {
  bool pipeline;
  bool parallel;
  bool loop_tiling;
  bool pipe_parallel;
  bool burst;
  bool task;
  bool kernel;
  bool reduction;
  bool line_buffer;
  bool interface;
  bool attribute;
  bool task_wait;
  bool skip_sync;
  bool spawn;          // ZP: 2017-05-10
  bool false_dep;      // YX: 2017-09-13
  bool coalescing;     // YX: 2017-12-05
  bool loop_flatten;   // YXin: 2018-04-17
  bool loop_tiled_tag; // YX: 2018-03-07
  bool cache;          // YX: 2018-12-30
  bool hls_unroll;
};

class CAnalPragma {
private:
  // YX: 20180228 do not use it because we need a pointer to AST_IF
  CAnalPragma();
  map<string, string> pragma_table;
  CSageCodeGen *m_ast;
  void *m_ref;
  SgNode *location;
  pragma_tag opt_tag;
  string m_pragma_type;
  bool m_hls_fgopt;

public:
  CAnalPragma(CSageCodeGen *codegen)
      : m_ast(codegen), m_ref(nullptr), location(nullptr) {
    opt_clear();
  }
  bool PragmasFrontendProcessing(void *decl, bool &errorOut,
                                 bool report = false);
  void opt_clear();
  std::string get_task_name();
  bool is_hls_fg_opt() { return m_hls_fgopt; }
  // Interface
  bool add_attribute(string attr, string val);
  bool append_attribute(string attr, string val);
  void *update_pragma(void *decl, bool keep_old, void *scope);
  string print_attribute();
  string get_attribute(string attr);
  void remove_attribute(string attr);
  bool is_hls_pragma(string filter);
  void set_pragma_type(string pragma_type);
  map<string, string> get_attribute();

  bool is_loop_related() const {
    return opt_tag.pipeline || opt_tag.parallel || opt_tag.pipe_parallel ||
           opt_tag.loop_tiling || opt_tag.spawn || opt_tag.false_dep ||
           opt_tag.loop_flatten || opt_tag.loop_tiled_tag ||
           opt_tag.reduction || opt_tag.line_buffer;
  }

  bool is_interface() { return opt_tag.interface; }
  bool is_loop_tiled_tag() { return opt_tag.loop_tiled_tag; }
  bool is_cache() { return opt_tag.cache; }
  bool is_pipeline() { return opt_tag.pipeline; }
  bool is_parallel() { return opt_tag.parallel; }
  bool is_pipe_para() { return opt_tag.pipe_parallel; }
  bool is_loop_tiling() { return opt_tag.loop_tiling; }
  bool is_loop_flatten() { return opt_tag.loop_flatten; }
  bool is_false_dep() { return opt_tag.false_dep; }
  bool is_reduction() { return opt_tag.reduction; }
  bool is_line_buffer() { return opt_tag.line_buffer; }
  bool is_spawn() { return opt_tag.spawn; }
  bool is_task() { return opt_tag.task; }
  bool is_kernel() { return opt_tag.kernel; }
  bool is_burst() { return opt_tag.burst; }
  bool is_attribute() { return opt_tag.attribute; }
  bool is_task_wait() { return opt_tag.task_wait; }
  bool is_skip_sync() { return opt_tag.skip_sync; }
  bool is_coalescing() { return opt_tag.coalescing; }
  bool is_hls_unroll() { return opt_tag.hls_unroll; }

  void splitString(string str, string delim, vector<string> &results);
  string get_string() const { return m_ast->GetPragmaString(m_ref); }
  void *get_ref() const { return m_ref; }

  bool contain_auto_value() const;
  bool regex_match_auto(string str) const;

protected:
  bool parse_pragma(void *decl, string &diagnosis_info, bool report = false);
  bool isPipelinePragma(SgPragmaDeclaration *decl);
  bool isParallelPragma(SgPragmaDeclaration *decl, bool &report_complete);
  bool isLoopTilingPragma(SgPragmaDeclaration *decl);
  bool isPipe_ParallelPragma(SgPragmaDeclaration *decl);
  bool isBurstPragma(SgPragmaDeclaration *decl);
  bool isTaskPragma(SgPragmaDeclaration *decl);
  bool isKernelPragma(SgPragmaDeclaration *decl);
  bool isInterfacePragma(SgPragmaDeclaration *decl);
  bool isSkipSynchronizationPragma(SgPragmaDeclaration *decl);
  bool check_attribute_legality(void *decl, string &diagnosis_info,
                                bool report);
};

#endif