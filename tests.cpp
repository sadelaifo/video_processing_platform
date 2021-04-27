#include "tests.h"

void print_video_struct_object(video_value_struct& video_object_2) {
	cout << "Parsing metadata: \n";
	cout << "Name:          " << video_object_2.name << endl;
	cout << "path:          " << video_object_2.path << endl;
	cout << "timestamp:     " << asctime(localtime(&video_object_2.timestamp));
	for (size_t i = 0; i < video_object_2.metadata_array.size(); i++) {
		cout << "metadata:              " << video_object_2.metadata_array[i].metadata_name << endl;
		cout << "metadata path:         " << video_object_2.metadata_array[i].metadata_path << endl;
		cout << "timestamp:             " << asctime(localtime(&video_object_2.metadata_array[i].timestamp));
		cout << "access frequency:      " << video_object_2.metadata_array[i].access_frequency << endl;
	}
}

void print_video_struct_object(string obj) {
	video_value_struct video_object_2;
	unmarshall(obj, &video_object_2);
	print_video_struct_object(video_object_2);
}


int test_1(string key1, video_value_struct& video_object, leveldb::DB* db) {

	leveldb::Status status;

	cout << "Test 1: testing database R&W ...\n";

	video_object.name = key1;
	video_object.path = "path_1";
	video_object.add_metadata("human", key1 + "_human_path");
	video_object.add_metadata("car", key1 + "_car_path");
	video_object.add_metadata("apple", key1 + "_apple_path");
	video_object.add_metadata("cloud", key1 + "_cloud_path");
	video_object.add_metadata("dog", key1 + "_dog_path");
	//      video_object.add_metadata("cat", key1 + "_cat_path");

	string token1 = video_object.marshall();

	status = db->Put(leveldb::WriteOptions(), key1, token1);

	if (!status.ok()) {
		cerr << status.ToString() << endl;
		return 1;
	} else {
		cout << "Sucess: Value Stored\n";
	}

	string token2;
	cout << "Storing marshalled value into database... \n";
	cout << token1 << endl;
	status = db->Get(leveldb::ReadOptions(), key1, &token2);

	cout << "***********************************" << endl;
	cout << "Retrieving database value for video " << key1 << endl;
	cout << token2 << endl;
	video_value_struct video_object_2;

	if (unmarshall(token2, &video_object_2) != 0) {
		cerr << "unmarshall failed\n";
	};

	if (!status.ok()) {
		cerr << status.ToString() << endl;
		return 1;
	} else {
		print_video_struct_object(video_object_2);
	}

	cout << "***********************************" << endl << endl;
	return 0;
}

int test_2(string key1, leveldb::DB* db) {
	cout << "Test 2: testing metadata eviction\n";
	add_metadata(db, key1, "flower", "_flower_path");

	string token3;
	leveldb::Status status = db->Get(leveldb::ReadOptions(), key1, &token3);

	//      cout << token3 << endl;

	if (!status.ok()) {
		cerr << status.ToString() << endl;
		return 1;
	}

	print_video_struct_object(token3);

	cout << "***********************************" << endl << endl;
	return 0;
}

int test_3(string key1, leveldb::DB* db) {
	string token3;
	cout << "Test 3: testing metadata removal\n";
	cout << "...Deleting car...\n";
	delete_metadata(db, key1, "car");

	leveldb::Status status = db->Get(leveldb::ReadOptions(), key1, &token3);

	if (!status.ok()) {
		cerr << status.ToString() << endl;
		return 1;
	}

	print_video_struct_object(token3);

	cout << "***********************************" << endl << endl;
	return 0;
}

int test_4(string key1, leveldb::DB* db) {
	string token3;
	cout << "Test 4: testing metadata read\n";

	get_metadata_path_given_metadata_name(db, key1, "human", &token3);

	cout << "Reading metadata...\n";
	cout << "Video Name:    " << key1 << endl;
	cout << "metadata name: " << "human" << endl;
	cout << "metadata path: " << token3 << endl;
	cout << "***********************************" << endl << endl;

	return 0;
}

int test_5(leveldb::DB* db) {
	std::ifstream inputfile;
	inputfile.open ("metadata_file.txt", std::ifstream::in);
	return 0;
}

