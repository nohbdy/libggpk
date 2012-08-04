#include <iostream>

#include "ggpk.h"
#include "ggpk/Archive.h"
#include "ggpk/Node.h"

using namespace std;

ggpk::Node* TestFile(ggpk::Archive* archive, const char* path) {
	ggpk::Node* node = archive->FindNode(path);
	if (node != NULL) {
		cout << "Found " << path << " at 0x" << hex << node->file_offset() << endl;
		cout << " Data Size: " << dec << node->data_size() << " bytes" << endl;
		return node;
	} else {
		cout << "Unable to find " << path << endl;
		return NULL;
	}
}

int doNone(ggpk::Archive* archive) {
	ggpk::Node* root = archive->root();
	cout << "Archive " << archive->filename() << endl;
	cout << "Root Offset: 0x" << hex << root->file_offset() << endl;

	for (vector<ggpk::Node*>::const_iterator it = root->child_nodes().begin(); it < root->child_nodes().end(); it++) {
		ggpk::Node* c = *it;

		if (c->node_type() == ggpk::Node::File) {
			cout << "FILE ";
		} else {
			cout << "PDIR ";
		}

		cout << c->name() << endl;
	}

	// TestFile(archive, "Audio/Music/Act3_Gardens/Act3_Gardens.ogg");

	return 0;
}