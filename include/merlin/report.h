#pragma once

#include <map>
#include <string>

using std::map;
using std::string;

void readInterReport(string path, map<string, map<string, string>> *content);

void writeInterReport(string path,
                      const map<string, map<string, string>> &content);
