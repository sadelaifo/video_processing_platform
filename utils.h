#include <iostream>
#include <string>
#include <assert.h>
#include <vector>
#include <stdlib.h>
using namespace std;

struct video_metadata_struct {
	string metadata_name;
	string metadata_path;
};

struct video_value_struct {
	string name;
	string path;
	vector <video_metadata_struct> metadata_array;

	video_value_struct();
	video_value_struct(string name, string path);
	~video_value_struct();
	int clear();
	int add_metadata(string metadata_name, string metadata_path);
	string marshall();
};

int unmarshall(string& str, video_value_struct* obj);
