#include "ds_generator.h" 

#include <string>
#include <sstream>

string exec(const char* cmd) {
    array<char, 512> buffer;
    string result;
    unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

int main(int argc, char* argv[]) {
  string str_prj;
  int i = 1;
  string str_args;
  for (; i < argc; i++) {
    if (strstr(argv[i], " ") != nullptr) {
      str_args += "\"" + string(argv[i]) + "\" ";
    } else {
      str_args += string(argv[i]) + " ";
    }
  }

  string cmd =
      "mars_opt -e c -p lower_separate -a cstd=c99 -a cxxstd=c++11 "
      "-a skip_syntax_check= -a enable_cpp=  -a impl_tool=sdaccel "
      "-a effort=standard -a generate_l2_api=on "
      "-I $MERLIN_COMPILER_HOME/source-opt/include/apint_include "
      "-I $MERLIN_COMPILER_HOME/build/..//mars-gen/lib/merlin "
      "-I $MERLIN_COMPILER_HOME/mars-gen/lib/merlin "
      "-a cflags=\"-D XILINX  -D MC_BASELINE  -D __SDSVHLS__  "
      "-D MERLIN_OPT -D__SYNTHESIS__  -D_MARS_COMPILER_FLOW_ \" " +
      str_args;    
  cout<<cmd<<"\n"; 
  system(cmd.c_str());

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
  CSageCodeGen m_ast;
  vector<string> vec_src_list;
  bool prev_I = false;
  for (int i = 1; i < options.get_option_num(""); i++) {
    string s_file = options.get_option("", i);
    string ext = s_file.substr(s_file.find(".") + 1);
    if (s_file.find("-I") == 0 || prev_I) {
      prev_I = false;
      vec_src_list.push_back(s_file);
    }
    if (s_file == "-I") {
      prev_I = true;
    }
  }

  for (int i = 0; i < vec_src_list.size(); i++) {
    cout<<vec_src_list[i]<<" ";
  }
  cout<<"\n";

  // vec_src_list.clear();
  string merlin_env_cmd = "echo $MERLIN_COMPILER_HOME";
  string merlin_path = exec(merlin_env_cmd.c_str());
  vec_src_list.push_back("-I"+merlin_path+"/source-opt/include/apint_include");
  vec_src_list.push_back("-I"+merlin_path+"/build/..//mars-gen/lib/merlin");
  vec_src_list.push_back("-I"+merlin_path+"/mars-gen/lib/merlin");
  
  string find_cmd = 
    "find ./ -maxdepth 1 -name \"__merlinkernel*.c*\"";
  string kernel_files = exec(find_cmd.c_str());
  vector<string> vec_kernel_files;
  istringstream iss(kernel_files);
  string kernel_file;
  while(getline(iss, kernel_file)) {
   vec_kernel_files.push_back(kernel_file);
  }
    
  for (auto &kernel_file: vec_kernel_files) {
    string noprefix_kernel_file = kernel_file.substr(4); // ./__merlinxxxx
    string rename_cmd =
      "mv " + kernel_file + " " + noprefix_kernel_file;
    cout << rename_cmd<< "\n";
    system(rename_cmd.c_str());
    vec_src_list.push_back(noprefix_kernel_file); 

    void *sg_project = m_ast.OpenSourceFile(vec_src_list);
    if (sg_project == NULL)
      throw std::exception();
    vec_src_list.pop_back(); 
    m_ast.InitBuiltinTypes();
    void * p_top_func = sg_project;
    ds_generator_top(m_ast, p_top_func, options);
    m_ast.remove_double_brace();
    m_ast.GenerateCode();
    string rm_extern_cmd = "sed -i \"/^extern int __merlin_include__GB/d\" rose_"
                           + noprefix_kernel_file; 
    system(rm_extern_cmd.c_str());
  }
  return 0;
}
