#ifndef _GGPK_NODE_H_
#define _GGPK_NODE_H_

#pragma once
#include <stdint.h>
#include <string>
#include <vector>

class QFile;

namespace ggpk {

class Archive;

class Node {
public:
	enum NodeType {
		Unknown,
		File,
		Directory
	};

	Node(Archive* archive, Node* parent, int64_t offset)
		: archive_(archive),
		  parent_(parent),
		  file_offset_(offset),
		  is_loaded_(false),
		  node_type_(Unknown) { }

	virtual ~Node();

	void Load();

	void LoadChildren();

	Node* GetChildByName(const std::string& name);

	const Archive* archive() const { return archive_; }

	NodeType node_type() { Load(); return node_type_; }

	const unsigned char* hash() { Load(); return &hash_[0]; }

	const std::string& name() { Load(); return name_; }

	std::string path();

	int64_t file_offset() const { return file_offset_; }

	int32_t data_size() { Load(); return data_size_; }

	const std::vector<Node*>& child_nodes() { Load(); return child_nodes_; }

	bool is_dir() { Load(); return node_type_ == Directory; }

	bool is_file() { Load(); return node_type_ == File; }

	int64_t ReadData(void* buffer, int64_t buffer_size, int64_t offset);

private:
	Node(const Node& n) { }

	// Load information for a FILE node
	void LoadFile(QFile* file);

	// Load information for a PDIR node
	int32_t LoadDirectory(QFile* file);

	// If this is a directory, create child nodes
	void CreateChildren(QFile* file, int32_t num_nodes);

	// The ggpk archive to which this node belongs
	Archive* archive_;

	// This node's parent (directory), or NULL for the root node
	Node* parent_;

	// What type of node this is
	NodeType node_type_;

	// 64-bit offset within the archive where this node begins
	int64_t file_offset_;

	// Size, in bytes, of the node within the file
	// This number includes the header and all file data
	int32_t node_size_;

	// Size, in bytes, of the file data within the archive
	int32_t data_size_;

	// Size, in bytes, of the node header within the file
	int32_t header_size_;

	// Name of the node
	std::string name_;

	// Path of the directory
	std::string path_;

	// Hash for the file read from the archive
	unsigned char hash_[32];

	// Whether the node header has been loaded from the file
	// Initially, only the file_offset is known.  Seeking to the offset and reading the header are deferred until it is necessary.
	bool is_loaded_;

	// List of child nodes.  This list is only valid if this node represents a directory (node_type() == NodeType::Directory)
	std::vector<Node*> child_nodes_;

	// Whether the child nodes have been loaded
	bool are_children_loaded_;
};

} // namespace ggpk

#endif
