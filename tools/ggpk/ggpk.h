#pragma once

namespace ggpk_options {
	enum CommandOptions {
		None,
		Info,
		List,
		Extract
	};
}

namespace ggpk {
	class Archive;
}

int doNone(ggpk::Archive* archive);
int doInfo(ggpk::Archive* archive, const char* path);
int doList(ggpk::Archive* archive);
int doExtract(ggpk::Archive* archive, const char* path, const char* outPath);