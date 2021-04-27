#include <iostream>
#include <string>
#include <assert.h>
#include <leveldb/db.h>
#include <vector>
#include "utils.h"
#include <fstream>
using namespace std;


int put_leveldb();
void print_video_struct_object(video_value_struct& video_object_2);
void print_video_struct_object(string obj);


int test_1(string key1, video_value_struct& video_object, leveldb::DB* db);
int test_2(string key1, leveldb::DB* db);
int test_3(string key1, leveldb::DB* db);
int test_4(string key1, leveldb::DB* db);
int test_5(leveldb::DB* db);
