#ifndef __UTILS_H__
#define __UTILS_H__

#include <iostream>
#include <string>
#include <assert.h>
#include <vector>
#include <stdlib.h>
#include <ctime>
#include <time.h>
#include <chrono>
#include <ratio>
#include <leveldb/db.h>

using namespace std;

extern int verbose;

const std::string delimiter = "::";
const double weights[2] = {0.5, 0.5};
const int max_num_metadata_types = 5;
const std::string replacement_policy = "Earlest created eviction";

struct video_metadata_struct {
	string metadata_name;
	string metadata_path;
	long int timestamp;
	int access_frequency;
};

struct video_value_struct {
	string name;
	string path;
	long int timestamp;
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
int add_metadata(leveldb::DB* db, string key, string metadata_name, string metadata_path);
int delete_metadata(leveldb::DB* db, string key, string metadata_name);
double compute_f_score(size_t timestamp, size_t access_frequency);
std::string decide_eviction_victim(video_value_struct* obj);
int get_metadata_path_given_metadata_name(leveldb::DB* db, string key, string metadata_name, string* metadata_path);
#endif
