#include "ds_generator.h" 

int main(int argc, char* argv[]) {
  ROSE_INITIALIZE;
  CInputOptions options;

  options.set_flag("-t", 1, 0);
  // options.set_flag("-e", 1);
  // options.set_flag("-p", 1);
  options.set_flag("-o", 1, 0);
  options.set_flag("-x", 1, 0);
  options.set_flag("-a", 20, 0);
  options.set_flag("-c", 2, 0);
  options.set_flag("-fgrain", 1, 0);
  options.set_flag("", 100000, 2);

  if (!options.parse(argc, argv)) {
    exit(-1);
  }

  int ret;
  // No matter C or Cl files, we will change the file name to c file
  CSageCodeGen m_ast;
  vector<string> vec_src_list;
  for (int i = 1; i < options.get_option_num(""); i++) {
    string s_file = options.get_option("", i);
    string ext = s_file.substr(s_file.find(".") + 1);
    vec_src_list.push_back(s_file);
  }

  void *sg_project = m_ast.OpenSourceFile(vec_src_list);
  if (sg_project == NULL)
    throw std::exception();
  m_ast.InitBuiltinTypes();
  void * p_top_func = sg_project;
  ds_generator_top(m_ast, p_top_func, options);
  m_ast.remove_double_brace();
  m_ast.GenerateCode();
  return 0;
}


