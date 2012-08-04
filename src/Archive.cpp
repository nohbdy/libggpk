// This is the main DLL file.

#include <QFile>
#include <QString>
#include <QStringList>

#include "Archive.h"
#include "Node.h"

namespace ggpk {

const int32_t GGPK_MAGIC = 0x4B504747; // 'GGPK'

// Private struct to simplify reading the archive header
// We must pack the struct so the read bytes align properly with the struct members
#pragma pack(1)
struct ArchiveHeader {
	int32_t header_size;	// Size of the file header in bytes
	int32_t magic_number;	// 'GGPK'
	int32_t unknown;		// version? or number of file/dir offsets in the header? (was 0x2 as of July 29, 2012)
	int64_t root_offset;	// Pointer to the root PDIR node
	int64_t unknown_14;		// As of July 29, 2012 this points to a FREE block
} __attribute__((packed));

Archive::~Archive() {
	if (root_ != NULL) {
		delete root_;
		root_ = NULL;
	}

	if (file_ != NULL) {
		file_->close();
		delete file_;
		file_ = NULL;
	}
}

int Archive::Init() {
	ArchiveHeader header;

	// Open file for reading in binary mode
	file_ = new QFile(filename_);
	if (!file_->open(QIODevice::ReadOnly)) {
		return -1;
	}

	// Go to beginning of the file
	if (!file_->seek(0)) {
		// Seek failed?
		return -1;
	}

	// Read in Archive header
	file_->read((char*)&header, sizeof(header));

	// Validate header
	if (header.magic_number != GGPK_MAGIC) {
		file_->close();
		file_ = NULL;
		return -1;
	}

	root_ = new Node(this, NULL, header.root_offset);
	root_->Load();

	return 0;
}

Archive* Archive::Open(const char* filename) {
	if (filename == NULL) {
		return NULL;
	}

	Archive* result = new Archive(filename);
	if (result->Init() >= 0) {
		return result;
	} else {
		delete result;
		return NULL;
	}
}

Node* Archive::FindNode(const char* path) {
	QString qpath(path);
	QStringList elems = qpath.split('/', QString::SkipEmptyParts); // Skip empty parts to avoid looking up the root

	Node* ptr = root_;
	for (QList<QString>::iterator it = elems.begin(); it < elems.end(); it++) {
		ptr = ptr->GetChildByName((*it).toStdString());
		if (ptr == NULL) {
			return NULL;
		}
	}

	return ptr;
}

} // namespace ggpk
