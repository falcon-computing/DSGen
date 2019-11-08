#include <map>
#include "cmdline_parser.h"
#include "ds_generator.h"
#include "gtest/gtest.h"

// #include "mars_opt.h"

namespace{

int test_argc;
char **test_argv;

using ::testing::TestWithParam;
using ::testing::Values;

class DsGeneratorTest : public testing::Test  {
 public:
  DsGeneratorTest() {
    ROSE_INITIALIZE;

    options.set_flag("-t", 1, 0);
    // options.set_flag("-e", 1);
    // options.set_flag("-p", 1);
    options.set_flag("-o", 1, 0);
    options.set_flag("-x", 1, 0);
    options.set_flag("-a", 20, 0);
    options.set_flag("-c", 2, 0);
    options.set_flag("-fgrain", 1, 0);
    options.set_flag("", 100000, 2);

    if (!options.parse(test_argc, test_argv)) {
      exit(-1);
    }

    int ret;
    // No matter C or Cl files, we will change the file name to c file
    for (int i = 1; i < options.get_option_num(""); i++) {
      string s_file = options.get_option("", i);
      string ext = s_file.substr(s_file.find(".") + 1);
      if (ext == "cl") {
        string s_file_org = s_file;
        s_file = s_file.substr(0, s_file.find(".")) + ".c";
        string cmd = "cp " + s_file_org + " " + s_file;
        ret = system(cmd.c_str());
        if (ret != 0) {
          fprintf(stderr, "Error: command %s failed\n", cmd.c_str());
          exit(ret);
        }
      }
      vec_src_list.push_back(s_file);
    }

  }
  CInputOptions options;
  
  map<string, void*> CollectLoops(CSageCodeGen &m_ast, DsGenerator& dg);

  vector<string> vec_src_list;
};

map<string, void*> DsGeneratorTest::CollectLoops(CSageCodeGen &m_ast, 
                                                 DsGenerator &dg) {
  map<string, void*> loops;
  vector<CMirNode *> fn_loop_nodes;
  CMarsIr &mars_ir = dg.GetCMarsIr();
  mars_ir.get_topological_order_nodes(&fn_loop_nodes);
  for (auto &node: fn_loop_nodes) {
    if (node->is_function)
      continue;
    auto *loop_scope = m_ast.GetParent(node->ref);
    auto *par_stmt = m_ast.GetParent(loop_scope);
    if (m_ast.IsLabelStatement(par_stmt)) {
      string label_name =  m_ast.GetLabelName(par_stmt);
      if (label_name.find("__UT_L") == 0)
        loops[label_name] = loop_scope;
    }
  }
  return loops;
}

// bug if this enabled next frondend will have SIGABRT
TEST_F(DsGeneratorTest, InitScopePragmas) {
  CSageCodeGen m_ast;
  DsGenerator dg(m_ast, m_ast.OpenSourceFile(vec_src_list, ""), options);
  EXPECT_FALSE(dg.InitScopePragmas());
  PragmaMap &map_scope_pragmas = dg.GetMapScopePragmas();
  for (auto &scope_pragmas: map_scope_pragmas) {
    auto pragmas = scope_pragmas.second;
    if (pragmas->exist(PIPELINE)) {
      auto pipeline_pragma_info =
        boost::any_cast<PragmaInfo<string> *>(pragmas->get(PIPELINE));
      SgPragma *pipeline_pragma = (SgPragma *)pipeline_pragma_info->pragma;
      if (m_ast.is_floating_node(pipeline_pragma)) {
        delete pipeline_pragma;
      }
    }

    if (pragmas->exist(TILING)) {
      auto tiling_pragma_info =
        boost::any_cast<PragmaInfo<int> *>(pragmas->get(TILING));
      SgPragma *tiling_pragma = (SgPragma *)tiling_pragma_info->pragma;
      if (m_ast.is_floating_node(tiling_pragma)) {
        delete tiling_pragma;
      }
    }

    if (pragmas->exist(PARALLEL)) {
      auto parallel_pragma_info =
        boost::any_cast<PragmaInfo<int> *>(pragmas->get(PARALLEL));
      SgPragma *parallel_pragma = (SgPragma *)parallel_pragma_info->pragma;
      if (m_ast.is_floating_node(parallel_pragma)) {
        delete parallel_pragma;
      }
    }
  }  
}

TEST_F(DsGeneratorTest, GetAllDivisors) {
  CSageCodeGen m_ast;
  DsGenerator dg(m_ast, m_ast.OpenSourceFile(vec_src_list, ""), options);
  vector<int>ret1, ret2, ret3, ret4;
  ret1 = dg.GetAllDivisors(28, 7);
  EXPECT_EQ(1, ret1[0]);
  EXPECT_EQ(2, ret1[1]);
  EXPECT_EQ(4, ret1[2]);
  EXPECT_EQ(7, ret1[3]);
  ret2 = dg.GetAllDivisors(28, 4);
  EXPECT_EQ(1, ret2[0]);
  EXPECT_EQ(2, ret2[1]);
  EXPECT_EQ(4, ret2[2]);
  EXPECT_EQ(3, ret2.size());
  ret3 = dg.GetAllDivisors(28, 3);
  EXPECT_EQ(1, ret3[0]);
  EXPECT_EQ(2, ret3[1]);
  EXPECT_EQ(2, ret3.size());
  ret4 = dg.GetAllDivisors(28, 9);
  EXPECT_EQ(1, ret4[0]);
  EXPECT_EQ(2, ret4[1]);
  EXPECT_EQ(4, ret4[2]);
  EXPECT_EQ(7, ret4[3]);
  EXPECT_EQ(4, ret4.size());
}

TEST_F(DsGeneratorTest, AllLoopNodes) {
  CSageCodeGen m_ast;
  DsGenerator dg(m_ast, m_ast.OpenSourceFile(vec_src_list, ""), options);

  vector<CMirNode *> fn_loop_nodes;
  CMarsIr &mars_ir = dg.GetCMarsIr();
  mars_ir.get_topological_order_nodes(&fn_loop_nodes);
  for (auto &node: fn_loop_nodes) {
    if (node->is_function)
      continue;
    auto *loop_scope = m_ast.GetParent(node->ref);
    if (!IsOutermostForLoop(m_ast, loop_scope))
      continue;
    EXPECT_TRUE(node->has_loop);   
  }
}

TEST_F(DsGeneratorTest, UpdateUserSpecifiedPragma) {
  CSageCodeGen m_ast;
  DsGenerator dg(m_ast, m_ast.OpenSourceFile(vec_src_list, ""), options);
  map<string, void *> loops = CollectLoops(m_ast, dg);

  void *scope_stmt_1 = loops["__UT_L1"];
  PragmaMap &map_scope_pragmas = dg.GetMapScopePragmas();
  ScopePragmas *scope_pragmas_l1 = new ScopePragmas();
  map_scope_pragmas[scope_stmt_1] =scope_pragmas_l1;
  EXPECT_TRUE(dg.UpdateUserSpecifiedPragma(scope_stmt_1, PIPELINE, "some_id"));  
  EXPECT_FALSE(dg.UpdateUserSpecifiedPragma(scope_stmt_1, TILING, "some_id"));  
  EXPECT_FALSE(dg.UpdateUserSpecifiedPragma(scope_stmt_1, PARALLEL, "some_id"));  

  void *scope_stmt_2 = loops["__UT_L2"];
  ScopePragmas *scope_pragmas_l2 = new ScopePragmas();
  map_scope_pragmas[scope_stmt_2] =scope_pragmas_l2;
  EXPECT_FALSE(dg.UpdateUserSpecifiedPragma(scope_stmt_2, PIPELINE, "some_id"));  
  EXPECT_FALSE(dg.UpdateUserSpecifiedPragma(scope_stmt_2, TILING, "some_id"));  
  EXPECT_FALSE(dg.UpdateUserSpecifiedPragma(scope_stmt_2, PARALLEL, "some_id"));  
}

TEST_F(DsGeneratorTest, SetUserPragmaInfo) {
  CSageCodeGen m_ast;
  DsGenerator dg(m_ast, m_ast.OpenSourceFile(vec_src_list, ""), options);
  map<string, void *> loops = CollectLoops(m_ast, dg);

  void *scope_stmt_1 = loops["__UT_L1"];
  PragmaMap &map_scope_pragmas = dg.GetMapScopePragmas();
  ScopePragmas *scope_pragmas_l1 = new ScopePragmas();
  map_scope_pragmas[scope_stmt_1] =scope_pragmas_l1;

  // Pipline Test
  { 
  void *pragma_flatten, *pragma_off, *pragma_on, *pragma_rand;
  PragmaInfo<string> *pragma_info;
  string pragma_str_flatten = "ACCEL PIPELINE flatten";
  pragma_flatten = m_ast.CreatePragma(pragma_str_flatten, scope_stmt_1);
  dg.SetUserPragmaInfo(scope_stmt_1, pragma_flatten, "id", PIPELINE);
  pragma_info = boost::any_cast<PragmaInfo<string> *>(
                  map_scope_pragmas[scope_stmt_1]->get(PIPELINE));
  EXPECT_EQ(pragma_info->default_value, "flatten");

  string pragma_str_off = "ACCEL PIPELINE off";
  pragma_off = m_ast.CreatePragma(pragma_str_off, scope_stmt_1);
  dg.SetUserPragmaInfo(scope_stmt_1, pragma_off, "id", PIPELINE);
  pragma_info = boost::any_cast<PragmaInfo<string> *>(
                  map_scope_pragmas[scope_stmt_1]->get(PIPELINE));
  EXPECT_EQ(pragma_info->default_value, "off");

  string pragma_str_on = "ACCEL PIPELINE";
  pragma_on = m_ast.CreatePragma(pragma_str_on, scope_stmt_1);
  dg.SetUserPragmaInfo(scope_stmt_1, pragma_on, "id", PIPELINE);
  pragma_info = boost::any_cast<PragmaInfo<string> *>(
                  map_scope_pragmas[scope_stmt_1]->get(PIPELINE));
  EXPECT_EQ(pragma_info->default_value, "on");

  string pragma_str_rand = "ACCEL PIPELINE wtf";
  pragma_rand = m_ast.CreatePragma(pragma_str_rand, scope_stmt_1);
  dg.SetUserPragmaInfo(scope_stmt_1, pragma_rand, "id", PIPELINE);
  pragma_info = boost::any_cast<PragmaInfo<string> *>(
                  map_scope_pragmas[scope_stmt_1]->get(PIPELINE));
  EXPECT_EQ(pragma_info->default_value, "invalid");
  }

  // Tiling Test
  {
  PragmaInfo<int> *pragma_info;
  void *pragma_tiling_normal;
  // reject addtional parameters
  string pragma_str_tiling = "ACCEL TILING FACTOR=2";
  pragma_tiling_normal = m_ast.CreatePragma(pragma_str_tiling, scope_stmt_1);
  dg.SetUserPragmaInfo(scope_stmt_1, pragma_tiling_normal, "id", TILING);  
  pragma_info = boost::any_cast<PragmaInfo<int> *>(
                  map_scope_pragmas[scope_stmt_1]->get(TILING));
  EXPECT_EQ(pragma_info->default_value, 2);
  }

  // Parallel Test
  {
  PragmaInfo<int> *pragma_info;
  void *pragma_parallel_normal;
  // reject addtional parameters
  string pragma_str_parallel = "ACCEL PARALLEL FACTOR=2";
  pragma_parallel_normal = m_ast.CreatePragma(pragma_str_parallel, scope_stmt_1);
  dg.SetUserPragmaInfo(scope_stmt_1, pragma_parallel_normal, "id", PARALLEL);  
  pragma_info = boost::any_cast<PragmaInfo<int> *>(
                  map_scope_pragmas[scope_stmt_1]->get(PARALLEL));
  EXPECT_EQ(pragma_info->default_value, 2);
  }
}

TEST_F(DsGeneratorTest, GetShadowedParams) {
  CSageCodeGen m_ast;
  DsGenerator dg(m_ast, m_ast.OpenSourceFile(vec_src_list, ""), options);
  map<string, void *> loops = CollectLoops(m_ast, dg);

  void *scope_stmt_2 = loops["__UT_L2"];
  vector<string> free_vec, fixed_vec;
  free_vec.clear(), fixed_vec.clear();
  dg.UpdateUserSpecifiedPragma(scope_stmt_2, PIPELINE, "__PIPELINE__L2");
  dg.GetShadowedParams(scope_stmt_2, PIPELINE, free_vec, fixed_vec);
  EXPECT_EQ(fixed_vec.size(), 1);
  EXPECT_EQ(fixed_vec[0], "__PI");
  free_vec.clear(), fixed_vec.clear();
  dg.UpdateUserSpecifiedPragma(scope_stmt_2, TILING, "__TILING__L2");
  dg.GetShadowedParams(scope_stmt_2, TILING, free_vec, fixed_vec);
  EXPECT_EQ(fixed_vec.size(), 0);
}

} // namespace

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  test_argc = argc;
  test_argv = argv;
  return RUN_ALL_TESTS();
}
