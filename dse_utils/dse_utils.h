#ifndef __DSE_UTILS__
#define __DSE_UTILS__
#include <iterator>
#include <vector>
#include <set>
#include <algorithm>
#include <fstream>
#include <unordered_map>

//#include "xml_parser.h"
//#include "cmdline_parser.h"
//#include "file_parser.h"
//
//#include "mars_opt.h"
//#include "PolyModel.h"
//#include "tldm_annotate.h"
//#include "codegen.h"
//#include "program_analysis.h"
//
//#include "mars_ir.h"
//#include "dse_utils.h"

using namespace MarsProgramAnalysis;
using namespace std;

string getDesignSpaceParamName(string pragmaStr);
vector<string> getShadowedPipelineParams(CSageCodeGen &m_ast, void *loop);

void insertLoopDesignSpace(CSageCodeGen &m_ast,
                           void (*inserter)(CSageCodeGen &, void *, string),
                           void *stmt, string loop_code);

bool isFineGrainedLoop(CSageCodeGen &m_ast, void *loop);
bool isFlattenFunction(CSageCodeGen &m_ast, void *func);
bool hasNonFlattenFuncCall(CSageCodeGen &m_ast, void *loop);

bool IsLoopStatement(CSageCodeGen &m_ast, void *loop);
bool IsOutermostForLoop(CSageCodeGen &m_ast, void *loop);
void getSubPragmas(CSageCodeGen &m_ast, void *blk, set<void *> *setPragmas);
string getLoopHierfromParallelPragma(CSageCodeGen &m_ast, void *pragma);

unordered_map<string, int>
createWeightedCallgraphForFunction(CSageCodeGen &m_ast, void *func_body);
bool GetUnrollFactorFromPragma(CSageCodeGen &codegen, void *loop,
                               long long &factor);
#endif
