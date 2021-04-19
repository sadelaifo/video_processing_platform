#ifndef __UTILS_H__
#define __UTILS_H__

#include <iostream>
#include <string>
#include <assert.h>
#include <vector>
#include <stdlib.h>
#include <ctime>
#include <chrono>
#include <ratio>
#include <leveldb/db.h>

using namespace std;

const delimiter = "::";

struct video_metadata_struct {
	string metadata_name;
	string metadata_path;
	std::chrono::system_clock::time_point timestamp;
};

struct video_value_struct {
	string name;
	string path;
	std::chrono::system_clock::time_point timestamp;
	vector <video_metadata_struct> metadata_array;

	video_value_struct();
	video_value_struct(string name, string path);
	~video_value_struct();
	int clear();
	int add_metadata(string metadata_name, string metadata_path);
	string marshall();
};

int unmarshall(string& str, video_value_struct* obj);
int marshall(string& str, video_value_struct* obj);
int add_metadata(leveldb::DB* db, string metadata_name, string metadata_path);


#endif
