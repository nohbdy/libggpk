#include <vector>
#include <iostream>

#include "ggpk.h"
#include "ggpk/Archive.h"
#include "ggpk/Node.h"

using namespace std;

// Recursively traverse the ggpk directory structure (depth-first) and print out all valid file paths
void PrintNode(ggpk::Node* node) {
	for (vector<ggpk::Node*>::const_iterator it = node->child_nodes().begin(); it < node->child_nodes().end(); it++) {
		ggpk::Node* c = *it;
		if (c->is_dir()) {
			PrintNode(c);
		}
	}

	for (vector<ggpk::Node*>::const_iterator it = node->child_nodes().begin(); it < node->child_nodes().end(); it++) {
		ggpk::Node* c = *it;
		if (c->is_file()) {
			cout << c->path() << endl;
		}
	}
}

// Print a list of all files that exist within the ggpk Archive
int doList(ggpk::Archive* archive) {
	ggpk::Node* root = archive->root();
	PrintNode(root);
	return 0;
}