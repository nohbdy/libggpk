#include <iostream>
#include <iomanip>

#include "ggpk.h"
#include "ggpk/Archive.h"
#include "ggpk/Node.h"

using namespace std;

// Translate a NodeType enum value into string for printing
const char* nodeTypeToName(ggpk::Node::NodeType t) {
	switch (t) {
	case ggpk::Node::File:
		return "File";
	case ggpk::Node::Directory:
		return "Directory";
	case ggpk::Node::Unknown:
		return "Unknown";
	default:
		return "???";
	}
}

// Write information about a given file within the archive
int doInfo(ggpk::Archive* archive, const char* path) {
	// Open the node if it exists
	ggpk::Node* node = archive->FindNode(path);
	if (node == NULL) {
		cerr << "File not found" << endl;
		return 2;
	}

	// Print the following information:
	// <path>
	// offset <file_offset>
	// node_type <File|Directory|Unknown>
	// data_size <data_size>
	// hash <fedcba0123456789...>
	cout << path << endl;
	cout << "offset " << node->file_offset() << endl;
	cout << "node_type " << nodeTypeToName(node->node_type()) << endl;
	cout << "data_size " << node->data_size() << endl;

	cout << "hash " << hex << setfill('0');
	const unsigned char* hash = node->hash();
	for (int i = 0; i < 32; i++) {
		cout << setw(2) << (unsigned)hash[i];
	}
	cout << endl;

	return 0;
}