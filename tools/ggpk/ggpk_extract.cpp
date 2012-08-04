#include <iostream>
#include <fstream>

#include "ggpk.h"
#include "ggpk/Archive.h"
#include "ggpk/Node.h"

using namespace std;

// Extract a file from the archive and save it to disk
int doExtract(ggpk::Archive* archive, const char* path, const char* outPath) {
	// Ensure the source file exists in the archive
	ggpk::Node* node = archive->FindNode(path);
	if (node == NULL) {
		cerr << "Failed to extract " << path << ": File not found" << endl;
		return 2;
	}

	if (node->node_type() != ggpk::Node::File) {
		cerr << "Failed to extract " << path << ": Only files can be extracted." << endl;
		return 4;
	}

	// Create/truncate the destination file
	ofstream outFile;
	outFile.open(outPath, ios::out | ios::binary | ios::trunc);

	if (!outFile.is_open()) {
		cerr << "Failed to extract " << path << ": Could not open " << outPath << " for writing" << endl;
		return 3;
	}

	// Read the data from the archive into a buffer
	int32_t dataSize = node->data_size();
	char* buffer = new char[dataSize];
	int64_t dataRead = node->ReadData(buffer, dataSize, 0);

	// Write the buffer to the output
	outFile.write(buffer, dataSize);
	outFile.close();

	cout << path << " saved to " << outPath << endl;

	delete [] buffer;

	return 0;
}