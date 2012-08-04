#include <stdio.h>
#include <locale>
#include <sstream>
#include <stdlib.h>

#include <QFile>
#include <QChar>
#include <QString>

#include "defines.h"
#include "Node.h"
#include "Archive.h"

namespace ggpk {

const int32_t FILE = 0x454C4946;
const int32_t PDIR = 0x52494450;
const int32_t FREE = 0x45455246;

#pragma pack(1)
struct NodeHeader {
	int32_t total_length;	// Total length of the node
	int32_t node_type;		// Node type identifier
	int32_t name_length;	// Length of the node's name
} PACK_STRUCT;

struct DirNode {
	int32_t unknown;	// 32-bits of something, not sure what!
	int64_t offset;		// Offset in archive to this node
} PACK_STRUCT;

// Read in the name data, and convert it from wide characters into a std::string
inline std::string read_node_name(QFile* file, int name_length) {
	QChar* name_data = new QChar[name_length];

	file->read((char*)name_data, 2 * name_length);

	QString qstr(name_data, name_length - 1); // size param shouldn't include the NULL

	delete [] name_data;
	return qstr.toStdString();
}

Node::~Node() {
	for (std::vector<ggpk::Node*>::const_iterator it = child_nodes_.begin(); it < child_nodes_.end(); it++) {
		delete *it;
	}
	child_nodes_.clear();
}

void Node::Load() {
	NodeHeader header;
	int32_t num_nodes = 0;

	if (is_loaded_) {
		return;
	}

	QFile* file = archive_->file();

	if (!file->seek(file_offset_)) {
		perror("Seek failed when loading node!");
		exit(1);
	}

	file->read((char*)&header, sizeof(header));
	node_size_ = header.total_length;
	header_size_ = sizeof(header) + 32 + 2 * header.name_length;

	switch (header.node_type) {
	case (int32_t)FILE:
		LoadFile(file);
		break;
	case (int32_t)PDIR:
		num_nodes = LoadDirectory(file);
		header_size_ += 4 + 12 * num_nodes;	// 4 bytes for the num_nodes value, and 12 bytes for each DirNode
		break;
	case (int32_t)FREE:
		// FREE nodes are deleted, no point trying to make sense of the data contained in them
		is_loaded_ = true;
		return;
	};

	data_size_ = node_size_ - header_size_;

	// Read the name
	name_ = read_node_name(file, header.name_length);

	if ((node_type_ == Directory) && (parent_ != NULL)) {
		if (parent_->parent_ == NULL) {
			path_ = name_;	// For directories just under root, avoid leading /
		} else {
			path_ = parent_->path_ + "/" + name_;
		}
	}

	// If num_nodes > 0, create unloaded Nodes for our children
	if (num_nodes > 0) {
		CreateChildren(file, num_nodes);
	}

	is_loaded_ = true;
}

void Node::LoadChildren() {
	if (are_children_loaded_) {
		return;
	}

	for (std::vector<ggpk::Node*>::const_iterator it = child_nodes_.begin(); it < child_nodes_.end(); it++) {
		(*it)->Load();
	}

	are_children_loaded_ = true;
}

void Node::LoadFile(QFile* file) {
	file->read((char*)&hash_[0], 32);

	node_type_ = File;
}

int32_t Node::LoadDirectory(QFile* file) {
	int32_t num_nodes;

	file->read((char*)&num_nodes, 4);
	file->read((char*)&hash_[0], 32);

	node_type_ = Directory;

	return num_nodes;
}

void Node::CreateChildren(QFile* file, int32_t num_nodes) {
	DirNode node_info;
	Node* child;
	for (int i = 0; i < num_nodes; i++) {
		file->read((char*)&node_info, sizeof(node_info));
		child = new Node(archive_, this, node_info.offset);
		child_nodes_.push_back(child);
	}
}

Node* Node::GetChildByName(const std::string& name) {
	if (!are_children_loaded_) {
		LoadChildren();
	}

	for (std::vector<ggpk::Node*>::const_iterator it = child_nodes_.begin(); it < child_nodes_.end(); it++) {
		if (name == (*it)->name()) {
			return *it;
		}
	}

	return NULL;
}

std::string Node::path() {
	if (node_type_ == Directory) { return path_; }
	else {
		if (parent_->parent_ == NULL) { return name_; }
		else { return parent_->path() + "/" + name_; }
	}
}

int64_t Node::ReadData(void* buffer, int64_t buffer_size, int64_t offset) {
	QFile* file = archive_->file();

	if (!file->seek(file_offset_ + header_size_ + offset)) {
		perror("Seek failed when reading data!");
		return -1;
	}

	return file->read((char*)buffer, buffer_size);
}

} // namespace ggpk
