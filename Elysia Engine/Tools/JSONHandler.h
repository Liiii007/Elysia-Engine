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
	static Document load(std::string path) {
		if (path.empty()) {
			return NULL;
		}

		Document d;
		FILE* fp;

		fp = fopen(path.c_str(), "r");
		if (fp == 0) return d;
		char readBuffer[65536];
		FileReadStream is(fp, readBuffer, sizeof(readBuffer));
		d.ParseStream(is);
		fclose(fp);

		return d;
	}

	//write json to file
	static bool save(std::string path, const Document& d) {
		if (path.empty()) return false;

		
		FILE* fp = fopen(path.c_str(), "w");

		if (fp == 0) return false;

		char writeBuffer[65536];
		FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));

		Writer<FileWriteStream> writer(os);
		d.Accept(writer);

		fclose(fp);

		return true;
	}
};

