#pragma once
#include <string>
#include <fstream>
#include <filesystem>

#include <rapidjson/writer.h>
#include <rapidjson/document.h>
#include "rapidjson/filereadstream.h"
#include <rapidjson/filewritestream.h>
#pragma warning(disable:4996)

using namespace rapidjson;

class JSONHandler {
public:

	//Read json from file
	static Document load(const std::filesystem::path& path) {
		if (path.empty()) {
			return NULL;
		}

		Document d;
		FILE* fp;
		char buffer[256];

		fp = fopen(path.string().c_str(), "rb");
		if (fp == 0) return d;

		FileReadStream is(fp, buffer, sizeof(buffer));
		d.ParseStream(is);
		fclose(fp);

		return d;
	}

	//write json to file
	static bool save(std::string path, const Document& d) {
		if (path.empty()) return false;
		
		FILE* fp = fopen(path.c_str(), "w");
		char buffer[256];

		if (fp == 0) return false;

		FileWriteStream os(fp, buffer, sizeof(buffer));

		Writer<FileWriteStream> writer(os);
		d.Accept(writer);

		fclose(fp);

		return true;
	}

};

