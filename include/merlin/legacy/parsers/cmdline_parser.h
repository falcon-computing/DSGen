#ifndef TRUNK_LEGACY_TOOLS_PARSERS_SRC_CMDLINE_PARSER_H_
#define TRUNK_LEGACY_TOOLS_PARSERS_SRC_CMDLINE_PARSER_H_

#include <assert.h>
#include <vector>
#include <map>
#include <string>
#include <utility>

using std::map;
using std::pair;
using std::string;
using std::vector;

class CInputOptions {
 public:
  // return 0 if constraints fail
  int parse(int argc, char *argv[]);
  string get_option(string flag, int idx = 0) const;
  string get_option_key_value(string flag, string key) const;
  int get_option_num(string flag) const;

  // min == -1 means min = max (fixed number of arguments for this flag)
  // max == -1 means it is a binary flag (no flag argument is needed)
  // for example:
  // options.set_flag("-o", 2, 0);  // it has 0~2 "-o" flag argument (-o arg1 -o
  // arg2)
  // options.set_flag("", 1);  // it has exact one non-flag
  // argument (flag "") options.set_flag("-f");  // -f is a binary
  // flag (no flag argument followed)
  void set_flag(string flag, int max = -1, int min = -1) {
    if (min == -1)
      min = max;
    m_flags[flag] = pair<int, int>(min, max);
    vector<string> temp;
    m_options.insert(pair<string, vector<string>>(flag, temp));
  }
  void add_option(string flag, string str) {
    assert(m_options.find(flag) !=
           m_options.end());  // if failed here, pls call set_flag first
    m_options[flag].push_back(str);
  }

  void remove_option(string flag) {
    assert(m_options.find(flag) != m_options.end());
    m_options[flag].clear();
  }

 protected:
  map<string, vector<string>> m_options;
  map<string, pair<int, int>> m_flags;
};

#endif  // TRUNK_LEGACY_TOOLS_PARSERS_SRC_CMDLINE_PARSER_H_
