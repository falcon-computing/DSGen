//////////////////////////////////////
// Program Analysis Functionalities
// E.g. Linear expression analysis,
// range analysis and dependence analysis
//
// Create date: 2015-07-16 (pengzhang@falcon-computing.com)
//////////////////////////////////////

#pragma once

#include "file_parser.h"
#include "rose.h"
#include <assert.h>
#include <map>
#include <unordered_map>
#include <string>
#include <tuple>
#include <vector>
using namespace std;
#include "codegen.h"
#include "common.h"

class CSageCodeGen;
typedef CSageCodeGen CMarsAST_IF;

enum access_type {
  NO_ACCESS = 0,
  READ = 1,
  WRITE = 2,
  READWRITE = 3,
};

namespace MarsProgramAnalysis {

class CMarsExpression;
class CMarsRangeExpr;

CMarsRangeExpr get_variable_range(void *sg_node, CMarsAST_IF *ast, void *pos,
                                  void *scope = nullptr);
CMarsRangeExpr get_variable_range(CMarsAST_IF *ast, const CMarsExpression &me);
void clear_variable_range(void *sg_node);
void set_variable_range(void *sg_node, CMarsRangeExpr mr);

// ZP: 20170314 to support range copy for assert variables
void copy_variable_range(void *sg_from, void *sg_to);

bool has_variable_range(CMarsAST_IF *ast, void *sg_expr, void *pos,
                        bool induct_check);

bool has_variable_range(CMarsAST_IF *ast, const CMarsExpression &);

unordered_map<void *, CMarsRangeExpr> get_range_analysis(CMarsAST_IF *ast);
// ZP: 20150822: for temporay insersion of the statements
class CTempASTStmt {
public:
  CTempASTStmt(void *temp_expr, CMarsAST_IF *ast, void *pos);
  ~CTempASTStmt();
  void reset_defuse();

protected:
  void *m_temp_stmt;
  CMarsAST_IF *m_ast;
  int m_reset_defuse;
};

class CMarsVariable {
public:
  CMarsVariable(void *var, CMarsAST_IF *ast, void *pos = nullptr,
                void *scope = nullptr);
  CMarsVariable(CMarsAST_IF *ast,
                CMarsRangeExpr mr); // a dummy variable with a given range

  // Can not clear the range because the range is shared by othre objects
  //  ~CMarsVariable() { if (m_ast->IsBasicBlock(m_var)) clear_range(); }

  friend class CMarsExpression;
  friend class DUMMY;

public:
  string unparse() const;
  // 0 iterator, 1 local, 2 expression, 3 argument
  string GetTypeString();

  CMarsRangeExpr get_range() const;

  // Utility function: used for temporary CMarsExpression/CMarsRangeExpr objects
  void set_range(CMarsRangeExpr mr);

  // Variable ranges
  int GetBoundAbs(int64_t &lb, int64_t &ub);

  void *get_pointer() const { return m_var; }
  CMarsAST_IF *get_ast() const { return m_ast; }
  void *get_pos() { return m_pos; }

  string print_var() const;
  string print() const;

  static bool is_tmp_var(void *var) {
    return s_map_tmpvar_name.find(var) != s_map_tmpvar_name.end();
  }
  static string get_tmp_var_name(void *var) {
    if (s_map_tmpvar_name.find(var) == s_map_tmpvar_name.end())
      return "";
    else
      return s_map_tmpvar_name[var];
  }

  bool operator<(const CMarsVariable &var) const;

#if KEEP_UNUSED
  int GetType();
  void clear_range();
  int IsConstantRange();
  int64_t GetLBAbs();
  int64_t GetUBAbs();
  int GetBoundExpr(CMarsExpression &lb, CMarsExpression &ub);
#endif
protected:
  void *m_var;
  CMarsAST_IF *m_ast;
  void *m_pos;
  void *m_scope;
  static map<void *, string> s_map_tmpvar_name;
};

// Initialized_name ->
extern map<void *, CMarsRangeExpr> g_variable_range_cache;
void analyze_all_variable_ranges(CMarsAST_IF *ast);

// map iv vector to order vector
class CMarsInterleaveOrderMap {
public:
  // lexigraphic order comparison

protected:
  vector<int> m_vec_even;
  vector<void *> m_vec_odd;
};

class DUMMY;

class CMarsExpression {
  friend class DUMMY;

public:
  CMarsExpression(void *expr, CMarsAST_IF *ast, void *pos = nullptr,
                  void *scope = nullptr);
  CMarsExpression(CMarsAST_IF *ast, int64_t val);
  CMarsExpression(CMarsAST_IF *ast, int val);
  CMarsExpression(const CMarsExpression &op);

  CMarsExpression();

  CMarsExpression(CMarsVariable mv);

  CMarsExpression(int64_t dummy0, int64_t dummy1, int64_t dummy2,
                  int64_t dummy3, int64_t dummy4, int finite_flag = 0) {
    // dont care for other fields
    m_finite = finite_flag;
  }

protected:
  CMarsExpression(void *expr, map<void *, CMarsExpression> coeff,
                  CMarsAST_IF *ast, int64_t const_i, void *pos, void *scope,
                  int finite_flag = 0);

  void print_debug();

public:
  void *get_expr_from_coeff(bool final_expr = false) const;

public:
  int is_INF() const { return m_finite != 0; }
  int is_pINF() const { return (m_finite & 1) == 1; }
  int is_nINF() const { return (m_finite & 2) == 2; }
  int is_fINF() const { return is_nINF() && is_pINF(); }

public:
  string unparse() const;
  string print(int level = 0) const;
  string print_s() const { return unparse(); }
  string print_coeff(int level = 0) const;
  void *get_pointer() const { return m_expr; }
  CMarsAST_IF *get_ast() const { return m_ast; }
  void set_ast(CMarsAST_IF *ast) { m_ast = ast; }
  void *get_pos() { return m_pos; }
  void set_pos(void *new_pos);
  void set_scope(void *new_scope) { m_scope = new_scope; }
  void get_vars(vector<void *> &vars);
  vector<void *> get_vars();

  // zero-order
  int IsConstant() const;
  int64_t GetConstant() const { return m_const; }
  void SetConstant(int64_t value) { m_const = value; }
  int64_t get_const() const { return GetConstant(); }
  void set_const(int64_t value) { SetConstant(value); }
  int is_zero() const {
    return !is_INF() && IsConstant() && 0 == GetConstant();
  };

  int IsAtomic() const;

  int IsAtomicWithNonDeterminedRange() const;

  int IsNormal() const; // all the coefficient are constant

  // first-order
  int IsLinearToIterator() const; //
  int GetConstantCoeff(
      void *var,
      int64_t &value_out) const; // return 0 if not a constant
  int64_t GetConstantCoeff(void *var) const;

  bool IsLoopInvariant(void *for_loop) const;

  bool IsStandardForm(void *for_loop, map<void *, int64_t> &iterators,
                      map<void *, int64_t> &invariants) const;

  // high-order
  map<void *, CMarsExpression> &get_coeff() { return m_coeff; }
  CMarsExpression get_coeff(void *var);
  int64_t get_const_coeff(void *var);
  void set_coeff(void *var, CMarsExpression coeff) {
    m_coeff[var] = coeff;
    reduce();
  }
  vector<vector<int64_t>> get_coeff_range_simple();
  int has_coeff_range_simple();

  CMarsRangeExpr get_range() const;
  bool has_range() const;
  int any_var_located_in_scope(void *scope);
  int is_movable_to(void *pos);

  void operator=(const CMarsExpression &op);

  void ParseVarBoundSimple(map<void *, void *> &lb, map<void *, void *> &ub);

  friend CMarsExpression operator-(const CMarsExpression &op);
  friend CMarsExpression operator+(const CMarsExpression &op1,
                                   const CMarsExpression &op2);
  friend CMarsExpression operator-(const CMarsExpression &op1,
                                   const CMarsExpression &op2);
  friend CMarsExpression operator+(const CMarsExpression &op1, int64_t op2);
  friend CMarsExpression operator-(const CMarsExpression &op1, int64_t op2);
  friend CMarsExpression operator*(const CMarsExpression &op1,
                                   const CMarsExpression &op2);
  friend CMarsExpression operator*(const CMarsExpression &op1, int64_t op2);
  friend CMarsExpression operator/(const CMarsExpression &op1,
                                   const CMarsExpression &op2);
  friend CMarsExpression operator/(const CMarsExpression &op1, int64_t op2);

  friend CMarsExpression operator<<(const CMarsExpression &op1,
                                    const CMarsExpression &op2);
  friend CMarsExpression operator>>(const CMarsExpression &op1,
                                    const CMarsExpression &op2);
  friend int dividable(const CMarsExpression &op1, int64_t op2);
  friend int operator==(const CMarsExpression &op1, const CMarsExpression &op2);
  friend int operator!=(const CMarsExpression &op1, const CMarsExpression &op2);
  friend int operator==(const CMarsExpression &op1, int64_t op2);
  friend int operator!=(const CMarsExpression &op1, int64_t op2);
  friend int operator>(const CMarsExpression &op1, const CMarsExpression &op2);
  friend int operator<(const CMarsExpression &op1, const CMarsExpression &op2);
  friend int operator>=(const CMarsExpression &op1, int64_t op2);

  int IsNonNegative() const;
  int IsNonPositive() const;

  int substitute(void *sg_var, CMarsExpression sg_expr);
  int substitute(void *sg_var, int64_t value);

//   bool hasOnlyIteratorsInScope(void *scope);
//
#if KEEP_UNUSED
  CMarsRangeExpr get_range_in_scope(void *range_scope);
  friend int operator>=(const CMarsExpression &op1, const CMarsExpression &op2);
  friend int operator<=(const CMarsExpression &op1, const CMarsExpression &op2);
  friend int operator>(const CMarsExpression &op1, int64_t op2);
  friend int operator<(const CMarsExpression &op1, int64_t op2);
  friend int operator<=(const CMarsExpression &op1, int64_t op2);
#endif
protected:
  // m_expr, m_ast -> m_coeff
  void analysis();
  void reduce(); // remove the zero terms
  void induct(); // find substitutions for variables

  // youxiang: 2016-09-22, the api can be replaced by operator +
  // which can handle infinite expression
  // void AddValue(CMarsExpression exp);

protected:
  CMarsAST_IF *m_ast;

  void *m_expr; // The corresponding AST node, nullptr if no correspondence
  void *m_pos;  // used for scope or position purpose
  void *m_scope;
  int m_float_pos;

  map<void *, CMarsExpression> m_coeff;
  int64_t m_const;

  int m_finite; // the flag for m_finite: 0 for normal, 1 for pos_infinite, 2
                // for neg_infinite, 3 for infinite
};

extern const CMarsExpression pos_INF;
extern const CMarsExpression neg_INF;
extern const CMarsExpression full_INF;

class CMarsRangeExpr {
public:
  static const int64_t MARS_INT64_MAX = 0x7fffffffffffffffl;
  static const int64_t MARS_INT64_MIN = 0x8000000000000000l;
  CMarsRangeExpr(CMarsExpression lb, CMarsExpression ub, int is_exact = 0);
  CMarsRangeExpr(vector<CMarsExpression> lb, vector<CMarsExpression> ub,
                 int is_exact = 0);
  CMarsRangeExpr(CMarsAST_IF *ast = nullptr) {
    m_is_exact = 0;
    m_ast = ast;
  }                                             // full range
  CMarsRangeExpr(CMarsAST_IF *ast, void *type); // full range according to type
  CMarsRangeExpr(int64_t lb, int64_t ub, CMarsAST_IF *ast,
                 int is_exact = 0);
  CMarsRangeExpr(int64_t ublb, CMarsAST_IF *ast, int is_exact = 0);

  // union operation
  void AppendLB(CMarsExpression lb);
  void AppendUB(CMarsExpression ub);
  void set_lb(CMarsExpression lb);
  void set_ub(CMarsExpression ub);
  void append_lb(CMarsExpression lb) { AppendLB(lb); }
  void append_ub(CMarsExpression ub) { AppendUB(ub); }

  int has_lb() const { return m_lb.size() > 0; }
  int has_ub() const { return m_ub.size() > 0; }
  int has_bound() const { return has_lb() || has_ub(); }

  void update_position(const t_func_call_path &fn_path, void *pos);
  void set_pos(void *pos);
  // FIXME: to be deprecated, using the set based one
  CMarsExpression get_lb(int idx = 0) const;
  CMarsExpression get_ub(int idx = 0) const;

  vector<CMarsExpression> get_lb_set() const { return m_lb; }
  vector<CMarsExpression> get_ub_set() const { return m_ub; }

  int get_simple_bound(CMarsExpression &lb, CMarsExpression &ub);

  int is_simple_bound() {
    CMarsExpression lb, ub;
    return get_simple_bound(lb, ub);
  }

  void get_vars(vector<void *> &non_duplicated_vars);

  // conservative
  int is_empty() const;

  int is_infinite() const { return m_lb.size() <= 0 || m_ub.size() <= 0; }

  void set_empty(CMarsAST_IF *ast = nullptr);

  int any_var_located_in_scope(void *scope);
  int is_movable_to(void *pos);
  int has_var(void *var);

  int substitute(void *var, void *pos);
  int substitute(void *var, CMarsExpression lb, CMarsExpression ub,
                 int is_exact = 0);

  string print_const_bound() const;
  string print() const;
  string print_e() const;

  bool length_is_greater_or_equal(int64_t len) const;

  bool IsConstantLength(int64_t &len) const;

  bool is_single_access() const;

  bool is_full_access(int64_t size) const;

protected:
  string print(vector<CMarsExpression> bound, int is_lb) const;

  void reduce();

public:
  CMarsAST_IF *get_ast() const;
  void set_ast(CMarsAST_IF *ast) { m_ast = ast; }

public:
  friend CMarsRangeExpr operator-(const CMarsRangeExpr &op);
  friend CMarsRangeExpr operator+(const CMarsRangeExpr &op1,
                                  const CMarsRangeExpr &op2);
  friend CMarsRangeExpr operator-(const CMarsRangeExpr &op1,
                                  const CMarsRangeExpr &op2);
  friend CMarsRangeExpr operator*(const CMarsRangeExpr &op1, int64_t op2);
  friend CMarsRangeExpr operator/(const CMarsRangeExpr &op1,
                                  const CMarsExpression &op2);
  friend CMarsRangeExpr min(const CMarsRangeExpr &op1,
                            const CMarsRangeExpr &op2);
  friend CMarsRangeExpr max(const CMarsRangeExpr &op1,
                            const CMarsRangeExpr &op2);
  friend CMarsRangeExpr min(const CMarsExpression &op1,
                            const CMarsExpression &op2);
  friend CMarsRangeExpr max(const CMarsExpression &op1,
                            const CMarsExpression &op2);
  friend CMarsRangeExpr operator+(const CMarsRangeExpr &op1,
                                  const CMarsExpression &op2);
#if KEEP_UNUSED
  friend CMarsRangeExpr operator*(const CMarsRangeExpr &op1,
                                  const CMarsExpression &op2);
  friend CMarsRangeExpr operator*(const CMarsRangeExpr &op1,
                                  const CMarsRangeExpr &op2);
#endif

  int IsNonNegative() const;
  int IsNonPositive() const;

  int IsConstantBound() const { return is_const_bound(); }
  int is_const_bound() const { return is_const_lb() && is_const_ub(); }
  int is_const_lb() const;
  int is_const_ub() const;
  int64_t get_const_lb() const;
  int64_t get_const_ub() const;

  int IsConstant() { return get_const_lb() == get_const_ub(); }

  CMarsExpression GetConstant();

  // These operations are done in a conservative way
  CMarsRangeExpr Union(CMarsRangeExpr range);
  CMarsRangeExpr Intersect(CMarsRangeExpr range);
  // return true if *this cover mr
  // update the position to create new merged range
  int is_cover(CMarsRangeExpr &mr, CMarsRangeExpr &res);

  int is_exact() const { return m_is_exact; }
  void set_exact(int is_exact) { m_is_exact = is_exact; }

  void refine_range_in_scope(void *range_scope, void *pos);
  void refine_range_in_scope_v2(void *range_scope, void *pos, void *array,
                                int is_write, t_func_call_path &path);
  void refine_range_in_scope_v2(void *range_scope, void *pos,
                                t_func_call_path &path);
  void update_control_flag(void *range_scope, void *pos, void *array,
                           int is_write, t_func_call_path &path);
  void refine_range_get_remain_substitutes(void *range_scope, void *pos,
                                           t_func_call_path &fn_path,
                                           vector<void *> &vec_sub_vars,
                                           vector<CMarsExpression> &vec_sub_lbs,
                                           vector<CMarsExpression> &vec_sub_ubs,
                                           vector<int> &vec_sub_is_exact);
  int refine_range_in_scope_v2_exact(void *range_scope, void *pos,
                                     t_func_call_path &fn_path);

protected:
#if KEEP_UNUSED
  int refine_range_in_scope_one_variable(void *range_scope,
                                         vector<void *> &loops_in_scope);
#endif

protected:
  // Note: vector is used to represent multiple bounds
  // each bound is a requirement to satisfy
  // and these conditinos are combined together by "or" operator
  vector<CMarsExpression> m_lb;
  vector<CMarsExpression> m_ub;
  int m_is_exact; // used to indicate full access of the range, ZP: 20160205
  CMarsAST_IF *m_ast;
};

typedef pair<int, int> CMarsRangeInt;

class CMarsDepDistSet {
public:
  CMarsDepDistSet() { set_empty(); }
  CMarsDepDistSet(int64_t lb, int64_t ub) {
    m_lb = lb;
    m_ub = ub;
  }

  int has_lb() { return m_lb != CMarsRangeExpr::MARS_INT64_MIN; }
  int has_ub() { return m_ub != CMarsRangeExpr::MARS_INT64_MAX; }

  int64_t get_lb() { return m_lb; }
  int64_t get_ub() { return m_ub; }
  vector<int64_t> get_set();
  int is_empty();
  void set_empty() {
    m_lb = CMarsRangeExpr::MARS_INT64_MAX;
    m_ub = CMarsRangeExpr::MARS_INT64_MIN;
  }
  int64_t size();

  string print();

protected:
  int64_t m_lb;
  int64_t m_ub;
};

class CMarsScope {
public:
  CMarsScope(void *scope) { m_scope = scope; }

  int IsLoop();
  int IsFuncbody();
  void *get_pointer() { return m_scope; }

protected:
  void *m_scope;
};

class CMarsResultFlags {
public:
  void set_flag(string flag_name, int64_t value) {
    m_flags[flag_name] = value;
  }
  int64_t get_flag(string flag_name) { return m_flags[flag_name]; }

protected:
  map<string, int64_t> m_flags;
};

class CMarsAccess {
public:
  CMarsAccess(void *ref, CMarsAST_IF *ast, void *scope, void *pos = nullptr);

  void set_scope(void *scope);

  void set_pos(void *pos);

  // each CMarsRangeExpr for a dimension
  vector<CMarsRangeExpr> GetRangeExpr();
  CMarsRangeExpr GetRangeExpr(int idx);

  void *GetRef() { return m_ref; }
  void *GetArray() { return m_array; }
  void *get_ref() { return m_ref; }
  CMarsAST_IF *get_ast() { return m_ast; }
  void *get_array() { return m_array; }
  void *get_scope() { return m_scope; }
  vector<CMarsExpression> GetIndexes() { return m_expr_list; }
  CMarsExpression GetIndex(size_t i) {
    assert(i < m_expr_list.size());
    return m_expr_list[i];
  }
  int get_dim() { return m_expr_list.size(); }

#if KEEP_UNUSED
  CMarsRangeExpr GetRangeExpr_old(int idx);
#endif

public:
  // Simple Access Type: Single Variable Continous access
  // For each dimension: the index is like "i+1" or "i-2"
  // 1. each dimension has at most one non-zero variable
  // 2. the coefficient of the variable is constant 1 or -1
  // 3. each variable exist in at most one dimension
  bool is_simple_type_v1();
  static bool is_simple_type_v1(vector<CMarsExpression> &indexes);
  // for each array dim: tuple<var, coeff, const>
  vector<tuple<void *, int64_t, int64_t>> simple_type_v1_get_dim_to_var();
  // for each variable: array_dim(-1 if not found), coeff, const
  // is compact: if true, only return the tuples that matches the array index
  vector<tuple<int, int64_t, int64_t>>
  simple_type_v1_get_var_to_dim(vector<void *> vec_vars, int is_compact = 0);

public:
  string print();
  string print_s();
  friend int GetDependence(CMarsAccess access1, CMarsAccess access2,
                           CMarsDepDistSet &dist_set,
                           CMarsResultFlags &results_flags);

protected:
  void analysis_exp();
  void analysis_var();

protected:
  void *m_ref;
  void *m_array;
  CMarsAST_IF *m_ast;
  vector<CMarsExpression> m_expr_list;

  // CMarsScope m_scope;
  void *m_scope;
  vector<void *> m_iv;
  vector<void *> m_pv;

  // TODO: not implemented
  CMarsInterleaveOrderMap m_order_vector_map;

  // for fake access nodes;
  void *m_pos;
};

class CMarsArrayRangeInScope {
public:
  CMarsArrayRangeInScope(void *array, CMarsAST_IF *ast, void *ref_scope,
                         void *range_scope, bool eval_access_size,
                         bool check_access_bound);
  CMarsArrayRangeInScope(void *array, CMarsAST_IF *ast,
                         const list<t_func_call_path> &vec_call_path,
                         void *ref_scope, void *range_scope,
                         bool eval_access_size, bool check_access_bound);
#if KEEP_UNUSED
  CMarsArrayRangeInScope(CMarsAST_IF *ast, void *ref, void *range_scope,
                         bool eval_access_size, bool check_access_bound);
#endif
  vector<CMarsRangeExpr> GetRangeExprWrite() { return m_range_w; }
  vector<CMarsRangeExpr> GetRangeExprRead() { return m_range_r; }

  CMarsRangeExpr GetFlattenRangeExprWrite() { return m_flatten_range_w; }
  CMarsRangeExpr GetFlattenRangeExprRead() { return m_flatten_range_r; }
  int has_read();
  int has_write();

  string print();
  string print_s();

  int64_t get_access_size() {
    assert(m_eval_access_size);
    return m_access_size;
  }

protected:
  void *m_array;
  CMarsAST_IF *m_ast;
  void *m_scope;
  CMarsRangeExpr m_flatten_range_w;
  CMarsRangeExpr m_flatten_range_r;
  vector<CMarsRangeExpr> m_range_w;
  vector<CMarsRangeExpr> m_range_r;
  int64_t m_access_size;
  bool m_eval_access_size;
  bool m_check_access_bound;
};

void GetAccessRangeInScope(void *array, void *ref_scope, void *range_scope,
                           CMarsAST_IF *ast, vector<CMarsRangeExpr> &r_range,
                           vector<CMarsRangeExpr> &w_range,
                           CMarsRangeExpr &r_flatten_range,
                           CMarsRangeExpr &w_flatten_range);

////////////////////////////////////////
int CheckAccessSeparabilityInScope(CMarsAST_IF *ast, void *array,
                                   void *ref_scope, void *boundary_scope,
                                   CMerlinMessage *msg);

void *CopyAccessInScope(CMarsAST_IF *ast, void *array, void *ref_scope,
                        void *boundary_scope, int &is_write,
                        map<void *, void *> *map_spref2snref = nullptr,
                        vector<void *> *vec_params = nullptr);
////////////////////////////////////////

int GetDependence(CMarsAccess access1, CMarsAccess access2,
                  CMarsDepDistSet &dist_set, CMarsResultFlags &results_flags);

vector<CMarsRangeExpr> RangeUnioninVector(vector<CMarsRangeExpr> vec1,
                                          vector<CMarsRangeExpr> vec2);

int is_exact_condition(CMarsAST_IF *ast, void *cond_stmt, void *scope,
                       bool true_branch);

void *GetAccessRangeFromIntrinsic(void *array, void *ref_scope,
                                  void *range_scope, CMarsAST_IF *ast,
                                  vector<CMarsRangeExpr> &range, bool &valid);

class DUMMY {
  vector<long> v;

public:
  DUMMY(const CMarsExpression *exp);

  DUMMY(const CMarsVariable *var);

  DUMMY(CMarsAST_IF *ast, void *expr, void *pos, void *scope);

  bool operator<(const DUMMY &var) const;

  string toString();
};

extern map<DUMMY, CMarsRangeExpr> cache_get_range_exp;
extern map<DUMMY, tuple<bool, int64_t>> cache_consts;
extern map<DUMMY, bool> cache_IsMatchedFuncAndCall;
} // namespace MarsProgramAnalysis
