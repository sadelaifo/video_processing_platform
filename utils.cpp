#include "utils.h"

video_value_struct::video_value_struct() {
	name = '\0';
	path = '\0';
	metadata_array.clear();
}

video_value_struct::video_value_struct(string name, string path) {
	this->name = name;
	this->path = path;
	metadata_array.clear();
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
	std::string delimiter = "::";
	std::string token;
	token =  this->name + delimiter;
       	token += this->path + delimiter;

	size_t counter = 0;
	while (this->metadata_array.size() > counter) {
		token += this->metadata_array[0].metadata_name + delimiter;
		token += this->metadata_array[0].metadata_path + delimiter;
		counter++;
	}

	return token;
}

int unmarshall(string& s, video_value_struct* obj) {
	std::string delimiter = "::";

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
	if ((pos = s.find(delimiter)) = std::string::npos) {
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
		if ((pos = s.find(delimiter)) = std::string::npos) {
			return 1;
		}
		token = s.substr(0, pos);
		tmp.metadata_path = token;
		s.erase(0, pos + delimiter.length());

		obj->metadata_array.push_back(tmp);
	}
	return 0;
}
