
#ifndef __BSU_GROUP_H__
#define __BSU_GROUP_H__

#include <map>
#include <vector>

#include "analPragmas.h"
#include "bsuNode.h"
#include "codegen.h"
#include "ir_types.h"
#include "rose.h"

typedef std::vector<CMirNode *> mirnode_table_vec;

class CMirNodeSet : public mirnode_table_vec {

public:
  CMirNodeSet(){};
  CMirNodeSet(const CMirNodeSet &node_set)
      : top_node(node_set.top_node), SgNode2CMirNode(node_set.SgNode2CMirNode),
        m_while_nodes(node_set.m_while_nodes) {
    insert(begin(), node_set.begin(), node_set.end());
  }
  bool read_from_AST(CSageCodeGen &codegen, SgFunctionDeclaration *kernel,
                     bool &errorOut, bool pragma_in_loop, bool report = false);
  // public interface
  string get_mir_node(int index);
  string get_function_name(SgFunctionDeclaration *kernel_decl);
  CMirNode *get_top_node() { return top_node; };
  vector<CMirNode *> get_while_nodes() { return m_while_nodes; };
  CMirNode *get_parent_node(CMirNode *bNode);
  CMirNode *get_parent_node(CSageCodeGen &codegen, CMirNode *bNode,
                            CMirNode *fNode);
  CMirNode *get_node(void *target_ref);
  void build_parent_relation();
  void print();
  void printindex();
  void set_kernel() { top_node->set_kernel(); }

  void releaseMemory() {
    for (iterator it = begin(); it != end(); ++it)
      delete *it;
    clear();
  }

protected:
  CMirNode *top_node;
  map<SgBasicBlock *, CMirNode *> SgNode2CMirNode;
  vector<CMirNode *> m_while_nodes;

  void nodeParsing(CSageCodeGen &codegen, SgFunctionDeclaration *kernel_decl,
                   SgBasicBlock *body, CMirNode *flnode, CMirNode *lnode,
                   bool &errorOut, bool pragma_in_loop);
  bool isOuttermostLoop(CSageCodeGen &codegen, void *sg_loop, void *sg_scope);
  CMirNode *init_loop_node(CSageCodeGen &codegen,
                           SgFunctionDeclaration *kernel_decl, void *sg_node);
  bool check_valid_reduction(CSageCodeGen &codegen, void *func_decl);
};

typedef std::vector<CMirNodeSet> mirnodeset_table_vec;

class CMirNodeFuncSet : public mirnodeset_table_vec {
public:
  CMirNodeFuncSet(CSageCodeGen &codegen, SgSourceFile *file,
                  const set<void *> &all_kernel_funcs, bool &errorOut,
                  bool pragma_in_loop, bool report = false);
  int get_func_num() { return size(); };
  void releaseMemory() {
    for (iterator it = begin(); it != end(); ++it)
      it->releaseMemory();
    clear();
  }
  CMirNode *get_node(void *body_ref) {
    for (iterator it = begin(); it != end(); ++it) {
      CMirNode *node = it->get_node(body_ref);
      if (node)
        return node;
    }
    return nullptr;
  }
  CMirNodeFuncSet() {}
  CMirNodeFuncSet(const CMirNodeFuncSet &node_set) {
    insert(begin(), node_set.begin(), node_set.end());
  }
};

#endif
