#pragma once
#include <json/json.h>
#include <fstream>

class Offset {
public:
	static Json::Value offset;
	static Json::Value clientOffset;
};
