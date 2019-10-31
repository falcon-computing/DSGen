#ifndef MERLIN_HISTORY_H
#define MERLIN_HISTORY_H

#include <algorithm>
#include <sstream>
#include <string>
#include <vector>

using std::string;
using std::vector;

#include "locations.h"
#include "mars_opt.h"

#include "rapidjson/document.h"
#include "rapidjson/rapidjson.h"

using rapidjson::Document;
using rapidjson::kArrayType;
using rapidjson::SizeType;
using rapidjson::Value;

#define HIS_CHECK 1

#if 0 // control history log
#define HIS_WARNING(x)                                                         \
  cout << "[HIS WARNING][" << __func__ << ", " << __LINE__ << "]" << x << endl;
#else
#define HIS_WARNING(x)
#endif

#if 0
#define HIS_ERROR(x)                                                           \
  if (!g_muteHISError) {                                                       \
    cerr << "[HIS ERROR][" << __func__ << ", " << __LINE__ << "]" << x         \
         << endl;                                                              \
  } else {                                                                     \
    cerr << "[HIS WARNING][" << __func__ << ", " << __LINE__ << "]" << x       \
         << endl;                                                              \
  }

#define HIS_ERROR_COND(cond, x)                                                \
  if (!g_muteHISError && cond) {                                               \
    cerr << "[HIS ERROR][" << __func__ << ", " << __LINE__ << "]" << x         \
         << endl;                                                              \
  } else {                                                                     \
    cerr << "[HIS WARNING][" << __func__ << ", " << __LINE__ << "]" << x       \
         << endl;                                                              \
  }
#else
#define HIS_ERROR(x)
#define HIS_ERROR_COND(cond, x)
#endif

int getIndex(SgNode *node);

class CSageCodeGen;

enum Action { INSERT, DELETE, COPY, BUILD };

class HistoryEntry { // all history are noted in their parents
public:
  HistoryEntry(Action action, const int loc) : action(action), location(loc) {}

  string toString();

  Action getAction() { return this->action; }
  const Action getAction() const { return this->action; }

  int getLocation() { return this->location; }

  HistoryEntry() : action(INSERT), location(-1) {}

  void dumpToJson(Value *value, Document::AllocatorType &allocator) const;

  void loadFromJson(const Value &value);

private:
  Action action;
  int location; // topological location: the index of its parent
};

class HistoryMarker {
public:
  HistoryMarker(Action action, const int loc)
      : org_identifier(""), histories({HistoryEntry(action, loc)}),
        applied_index(-1), children_num(-1), propagate(1), no_hierarchy(0),
        qor_merge_mode(""), parallel(false) {}
  HistoryMarker()
      : org_identifier(""), applied_index(-1), children_num(-1), propagate(1),
        no_hierarchy(0), qor_merge_mode(""), parallel(false) {}

  vector<HistoryEntry> &getHistories() { return histories; }
  const vector<HistoryEntry> &getHistories() const { return histories; }
  vector<string> &getMessages() { return messages; }

  string &getOrgIdentifier() { return this->org_identifier; }

  void setOrgIdentifier(const string org_identifier) {
    this->org_identifier = org_identifier;
  }

  void setParallel(bool isParallel) { this->parallel = isParallel; }

  int getAppliedIndex() { return this->applied_index; }

  void setAllApplied() { this->applied_index = this->histories.size() - 1; }
  void increaseAppliedNum(int num);

  int getChildrenNum() { return this->children_num; }

  void setChildrenNum(int children_num) { this->children_num = children_num; }

  string toString(string leadingString);

  void dumpToJson(Value *d, Document::AllocatorType &allocator) const;

  void loadFromJson(const Value &d);

  void setUserCodeScopeId(string userCodeScopeId) {
    this->user_code_scope_id = userCodeScopeId;
  }

  string getUserCodeScopeId() const { return this->user_code_scope_id; }

  void setPropagate(int isPropagate) { this->propagate = isPropagate; }

  int getPropagate() const { return this->propagate; }

  void setNoHierarchy(int isNoHierarchy) { this->no_hierarchy = isNoHierarchy; }

  int getNoHierarchy() const { return this->no_hierarchy; }

  void setQoRMergeMode(string qorMergeMode) {
    this->qor_merge_mode = qorMergeMode;
  }

  string getQoRMergeMode() const { return this->qor_merge_mode; }

  bool getParallel() const { return this->parallel; }

private:
  string org_identifier; // position string or “created_#”
  string user_code_scope_id;
  vector<HistoryEntry> histories;
  vector<string> messages;
  int applied_index;
  int children_num;
  int propagate;    // the children will share the same org_identifier with it
  int no_hierarchy; // passed to Shan, not used now according to his opinion
  string
      qor_merge_mode; // set major for minor for node when lc has multiple
                      // nodes corresponding to user node if set major the node,
                      // the user node cycle will be the cycle of this node
  bool parallel;
};

/******************************************************************************/
/* Helper functions ***********************************************************/
/******************************************************************************/

string getParent(const string &id);
string printNodeInfo(CSageCodeGen *ast, void *node, int len = 80);

/******************************************************************************/
/* History APIs ***************************************************************/
/******************************************************************************/

bool isOptPass(const string &passName);

void markInsert(CSageCodeGen &ast, SgNode *, const int loc,
                const string &debugInfo = "");
void markDelete(CSageCodeGen &ast, SgNode *, const int loc,
                const string &debugInfo = "", SgNode *removeNode = nullptr);
void markCopy(CSageCodeGen &ast, SgNode *, SgNode *,
              const string &debugInfo = "", const bool passMajor = false);
void markBuild(CSageCodeGen &ast, void *, const string &debugInfo = "",
               void *bindNode = nullptr);
void markIncludeFile(CSageCodeGen &ast, SgNode *scope, string filename,
                     int before);
void markIncludeMacro(CSageCodeGen &ast, SgNode *scope, string macro,
                      int before);
void markIncludeExpand(CSageCodeGen &ast);

void setNoHierarchy(CSageCodeGen &ast, void *node_, int isNoHierarchy);
void setQoRMergeMode(CSageCodeGen &ast, void *node_, string qorMergeMode,
                     bool isRecursivelySet = false);
void setParallel(CSageCodeGen &ast, void *node_, bool isParallel,
                 bool isRecursivelySet = false);
string bindNodeOrgId(CSageCodeGen &ast, void *node_, void *bindNode,
                     bool force = false);
void insertMessage(CSageCodeGen &ast, void *node, map<string, string> &msg);

string getOriginalTopoLocation(CSageCodeGen *ast,
                               map<SgNode *, HistoryMarker> &histories,
                               string topoLocation, bool ignoreError = false);
string getOriginalTopoLocation(map<string, HistoryMarker> &histories,
                               string topoLocation, bool ignoreError = false);

void historyModuleInit(CSageCodeGen &ast, const string &passName);
map<SgNode *, string> saveOrgId(CSageCodeGen &ast, string passName);
void checkReferenceInduction(CSageCodeGen &ast, map<SgNode *, string> &orgIdMap,
                             string passName);
map<string, map<string, string>> saveForInterPasses(CSageCodeGen &ast,
                                                    string passName,
                                                    bool isForNextPass = false);
void checkBetweenPasses(CSageCodeGen &ast,
                        map<string, map<string, string>> &inputReferReport);

/******************************************************************************/
/* Json API *******************************************************************/
/******************************************************************************/

map<string, HistoryMarker> readHistoriesFromJson(const string histories_url);
void writeHistoriesToJson(const string histories_url,
                          const map<string, HistoryMarker> &);
map<string, HistoryMarker>
getSerializableHistories(CSageCodeGen &ast, map<SgNode *, HistoryMarker> &);

#endif /* MERLIN_HISTORY_H */
