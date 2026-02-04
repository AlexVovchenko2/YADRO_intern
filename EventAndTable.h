#pragma once
#include <fstream>
#include <sstream>
#include <cstddef>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <initializer_list>
#include "Exceptions.h"


struct Event {
	std::string time = "";
	uint8_t identifier = 0;
	std::string name = "";
	uint32_t table = 0;
	std::string error = "";

	Event(const std::string& tm, uint8_t id, const std::string& nm) : time(tm), identifier(id), name(nm) {}

	Event(const std::string& tm, uint8_t id, const std::string& nm, uint32_t tb) : time(tm), identifier(id),
		name(nm), table(tb) {}

	Event(const std::string& tm, uint8_t id, const std::string& nm, uint32_t tb, const std::string& err) 
		: time(tm), identifier(id), name(nm), table(tb), error(err) {}

	~Event() {}


	void print() {
		switch (identifier) {
		case 1: case 3: case 4: case 11:
			std::cout << time << " " << std::to_string(identifier) << " " << name << std::endl;
			break;
		case 2: case 12:
			std::cout << time << " " << std::to_string(identifier) 
				<< " " << name << " " << std::to_string(table) << std::endl;
			break;
		case 13:
			std::cout << time << " " << std::to_string(identifier) << " " << error << std::endl;
			break;
		default:
			break;
		}	
	}
};

struct Table {
	std::string currentClient = "";
	std::string timeIn = "";
	int activeTime = 0;
	bool isFree = true;
	uint32_t cash = 0;

	Table(std::string cc, std::string time)
		: currentClient(cc), timeIn(time), isFree(cc.length() == 0) {}

	void incrActiveTime(const std::string& endTime, uint32_t price) {
		
		int inHours = std::stoi(timeIn.substr(0, 2));
		int inMins = std::stoi(timeIn.substr(timeIn.length() - 2, 2));
		int totalIn = inHours * 60 + inMins;

		int outHours = std::stoi(endTime.substr(0, 2));
		int outMins = std::stoi(endTime.substr(endTime.length() - 2, 2));
		int totalOut = outHours * 60 + outMins;

		activeTime += (totalOut - totalIn);

		cash += price * static_cast<uint32_t>((totalOut - totalIn + 59) / 60);
	}

	std::string getActiveTimeStr() {
		int hours = activeTime / 60;
		int mins = activeTime % 60;
		char buffer[6];
		std::snprintf(buffer, sizeof(buffer), "%02d:%02d", hours, mins);
		return std::string(buffer);
	}

	void reset() {
		currentClient = "";
		timeIn = "";
		isFree = true;
	}
};

