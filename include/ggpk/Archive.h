#ifndef _GGPK_ARCHIVE_H_
#define _GGPK_ARCHIVE_H_

#pragma once

#include <stdint.h>

class QFile;

namespace ggpk {

class Node;

// Encapsulates a GGPK Archive
class Archive
{
public:
	~Archive();

	// Create an Archive object for the ggpk archive located at the provided file path
	static Archive* Open(const char* filename);

	// Retrieve a file or directory from the archive with the given path
	// If no nodes exist at the given path, FindNode will return NULL.
	Node* FindNode(const char* path);

	// Get the root directory
	Node* root() const { return root_; }

	// Get the archive file descriptor
	// Using the file descriptor is not thread safe
	QFile* file() const { return file_; }

	// Returns the path to the archive file
	const char* filename() const { return filename_; }

private:
	explicit Archive(const char* filename) : filename_(filename), root_(NULL) { };

	Archive(const Archive& a) { }

	// Initialize the Archive object by reading in the file header and some initial data
	int Init();

	// File descriptor for the open archive file
	QFile* file_;

	// Filepath of the ggpk archive
	const char* filename_;
		
	// Directory node of the PDIR of the root of the file system (i.e. '/')
	Node* root_;
};

}	// namespace ggpk

#endif
