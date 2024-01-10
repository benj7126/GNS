#pragma once

#include <unordered_map>
#include <string>
#include <typeinfo>
#include <memory>

struct DataStructBase {};

template<typename T>
struct DataStruct : public DataStructBase {
	std::string type;
    T* object;

    DataStruct(T* objectIn) {
		object = objectIn;
		type = typeid(T).name();
    }
};

struct DataStructHolder : public DataStructBase {
	std::unordered_map<std::string, std::shared_ptr<DataStructBase>> subStruct{};
	void insert(std::string str, std::shared_ptr<DataStructBase> data){
		subStruct.insert({str, data});
	}

	void merge(std::shared_ptr<DataStructHolder> mMap){
		for (const auto& kv : mMap->subStruct) {
			insert(kv.first, kv.second);
		}
	}
};

