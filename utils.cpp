#include "utils.h"

video_value_struct::video_value_struct() {
	name = '\0';
	path = '\0';
	metadata_array.clear();
	timestamp = std::chrono::system_clock::now();
}

video_value_struct::video_value_struct(string name, string path) {
	this->name = name;
	this->path = path;
	metadata_array.clear();
	timestamp = std::chrono::system_clock::now();
}

video_value_struct::~video_value_struct(){
	name.clear();
	path.clear();
	metadata_array.clear();

}

int video_value_struct::clear() {
	name.clear();
	path.clear();
	metadata_array.clear();
	return 0;
}

int video_value_struct::add_metadata(string metadata_name, string metadata_path) {
	video_metadata_struct metadata;
	metadata.metadata_name = metadata_name;
	metadata.metadata_path = metadata_path;
	metadata_array.push_back(metadata);
	return 0;
}

string video_value_struct::marshall() {
	typedef duration <int, std::ratio<1> > seconds_type;

	std::string token;
	token =  this->name + delimiter;
       	token += this->path + delimiter;
	size_t counter = 0;
	while (this->metadata_array.size() > counter) {
		token += this->metadata_array[counter].metadata_name + delimiter;
		token += this->metadata_array[counter].metadata_path + delimiter;
		counter++;
	}
	
	return token;
}

int unmarshall(string& s, video_value_struct* obj) {

	size_t pos = 0;
	std::string token;

	// get name
	if ((pos = s.find(delimiter)) == std::string::npos) {	
		return 1;
	}
	token = s.substr(0, pos);
	obj->name = token;
	s.erase(0, pos + delimiter.length());

	// get path
	if ((pos = s.find(delimiter)) == std::string::npos) {
		return 1;
	}	
	token = s.substr(0, pos);
	obj->path = token;
	s.erase(0, pos + delimiter.length());

	while ((pos = s.find(delimiter)) != std::string::npos) {
		video_metadata_struct tmp;

		// get metadata 1 name
		token = s.substr(0, pos);
		tmp.metadata_name = token;
		s.erase(0, pos + delimiter.length());

		// get metadata 1 path
		if ((pos = s.find(delimiter)) == std::string::npos) {
			return 1;
		}
		token = s.substr(0, pos);
		tmp.metadata_path = token;
		s.erase(0, pos + delimiter.length());

		obj->metadata_array.push_back(tmp);
	}
	return 0;
}

int marshall(string& token, video_value_struct* obj) {
	token =  obj->name + delimiter;
	token += obj->path + delimiter;

	size_t counter = 0;
	while (obj->metadata_array.size() > counter) {
		token += obj->metadata_array[counter].metadata_name + delimiter;
		token += obj->metadata_array[counter].metadata_path + delimiter;
		counter++;
	}

	return 0;
}

int add_metadata(leveldb::DB* db, string key, string metadata_name, string metadata_path) {
	std::string token;
	leveldb::Status s = db->Get(leveldb::ReadOptions(), key, token);

	if (!s.ok()) {
		return 1;
	}
	token += metadata_name + delimiter;
	token += metadata_path + delimiter;

	s = db->Delete(leveldb::WriteOptions(), key);
	if (!s.ok()) {
		return 1;
	}

	s = db->Put(leveldb::WriteOptions(), key, token);
	if (!s.ok()) {
		return 1;
	}

	return 0;
}

int delete_metadata(leveldb::DB* db, string key, string metadata_name) {
	
}
