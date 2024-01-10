#pragma once

#include "DataStruct.h"
#include <vector>

class IEditable{
private:
	std::shared_ptr<DataStructBase> dataStruct = nullptr;

public:
	virtual DataStructBase GenerateDataStruct() = 0;
	std::shared_ptr<DataStructBase> GetDataStruct(){
		if (dataStruct == nullptr){
			dataStruct = std::make_shared<DataStructBase>(GenerateDataStruct());
		}

		return dataStruct;
	}
};
