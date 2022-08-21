#pragma once
#include <string>
#include <fstream>

#include <rapidjson/writer.h>
#include <rapidjson/document.h>
#include "rapidjson/filereadstream.h"
#include <rapidjson/filewritestream.h>
#pragma warning(disable:4996)

using namespace rapidjson;

class JSONHandler {
public:

	//Read json from file
	Document load(std::string path) {
		if (path.empty()) {
			return NULL;
		}

		Document d;
		FILE* fp;

		fp = fopen(path.c_str(), "r");
		if (fp == 0) return d;

		FileReadStream is(fp, buffer, sizeof(buffer));
		d.ParseStream(is);
		fclose(fp);

		return d;
	}

	//write json to file
	bool save(std::string path, const Document& d) {
		if (path.empty()) return false;

		
		FILE* fp = fopen(path.c_str(), "w");

		if (fp == 0) return false;

		FileWriteStream os(fp, buffer, sizeof(buffer));

		Writer<FileWriteStream> writer(os);
		d.Accept(writer);

		fclose(fp);

		return true;
	}

	char buffer[65536];
};

