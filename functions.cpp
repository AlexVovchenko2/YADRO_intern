#include "EventAndTable.h"


std::vector<char> symb = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h','i', 'j', 'k', 'l', 'm', 'n',
					'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '-', '_',
					'0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };

void rtrim(std::string& s) {
	if (!s.empty() && s.back() == '\r') s.pop_back();
}

template <typename T>
bool in(const T& value, std::vector<T> set) {
	return std::find(set.begin(), set.end(), value) != set.end();
}

bool isFormat(const std::string(&words)[4]) {
	std::string line = words[0] + " " + words[1] + " " + words[2] + ((words[3].empty()) ? "" : " " + words[3]);
	if (words[0].length() != 5) {
		throw UserException("Incorrect time format. Line: " + line);
		return false;
	}
	
	uint8_t id;
	try {
		id = std::stoi(words[1]);
	}
	catch (const std::invalid_argument&) {
		throw UserException("Invalid line: " + line);
		return false;
	}
	

	if (!in(id, { 1, 2, 3, 4, 11, 12, 13 })) {
		throw UserException("Incorrect ID format. Line: " + line);
		return false;
	}

	for (char ch : words[2]) {
		if (!in(ch, symb)) {
			throw UserException("Incorrect name/error format. Line: " + line);
			return false;
		}
	}

	if (id == 2 || id == 12) {
		try {
			std::stoi(words[3]);
		}
		catch (const std::invalid_argument&) {
			throw UserException("Invalid line: " + line);
			return false;
		}
		if (std::stoi(words[3]) <= 0) {
			throw UserException("Value should be positive. Line: " + line);
			return false;
		}
	}

	return true;
}

void inputData(std::ifstream& fin,
	uint32_t& tablesNumber,
	std::string& timeBegin,
	std::string& timeEnd,
	uint32_t& price,
	std::vector<Event>& eventsIn) {

	std::string tempLine;
	std::getline(fin, tempLine);
	rtrim(tempLine);
	if (std::stoi(tempLine) <= 0) throw UserException("Value should be positive. Line: " + tempLine);
	tablesNumber = std::stoi(tempLine);


	std::getline(fin, timeBegin, ' ');
	rtrim(timeBegin);
	if (timeBegin.length() != 5) throw UserException("Incorrect time format. Line: " + timeBegin);
	std::getline(fin, timeEnd);
	rtrim(timeEnd);
	if (timeEnd.length() != 5) throw UserException("Incorrect time format. Line: " + timeEnd);

	if (timeBegin > timeEnd) throw UserException("Time not sequential. Line: " + timeBegin + " " + timeEnd);

	std::getline(fin, tempLine);
	rtrim(tempLine);
	if (std::stoi(tempLine) <= 0) throw UserException("Price should be positive. Line: " + tempLine);
	price = std::stoi(tempLine);

	while (std::getline(fin, tempLine)) {
		rtrim(tempLine);
		std::istringstream iss(tempLine);

		std::string words[4];
		if (!(iss >> words[0] >> words[1] >> words[2])) {
			throw UserException("Invalid line: " + tempLine);
		}

		rtrim(words[1]);
		rtrim(words[2]);


		if (iss >> words[3]) {
			rtrim(words[3]);
			if (isFormat(words)) {
				if (!eventsIn.empty() && words[0] < eventsIn.back().time)
					throw UserException("Incorrect time format. Line: " + words[0]);
				else
					eventsIn.emplace_back(words[0], std::stoi(words[1]),
						words[2], std::stoi(words[3]));
			}

		}
		else {
			if (isFormat(words))
				if (!eventsIn.empty() && words[0] < eventsIn.back().time)
					throw UserException("Time not sequential. Line:  " + words[0]);
				else {
					if (std::stoi(words[1]) == 13)
						eventsIn.emplace_back(words[0], std::stoi(words[1]), "", 0, words[2]);
					else
						eventsIn.emplace_back(words[0], std::stoi(words[1]), words[2]);
				}
		}

	}

}

//template <typename T>
std::vector<Table>::iterator find(std::vector<Table>& vect, const std::string& what) {
	return std::find_if(vect.begin(), vect.end(),
		[&what](const Table& t) { return t.currentClient == what; });
}

std::vector<Table>::iterator find(std::vector<Table>& vect, bool what) {
	return std::find_if(vect.begin(), vect.end(),
		[&what](const Table& t) { return t.isFree == what; });
}


void generateOutEvent(std::vector<Event>& eventsAll,
	Event& currentEvent,
	std::vector<Table>& tables,
	std::vector<Event>& queue,
	std::vector<std::string>& clientsInClub,
	std::string timeBegin,
	std::string timeEnd,
	uint32_t tablesNumber,
	uint32_t price) {

	switch (currentEvent.identifier) {
	case 1:
	{
		if (find(tables, currentEvent.name) != tables.end()) {
			eventsAll.push_back(Event(currentEvent.time, 13, "", 0, "YouShallNotPass"));
			eventsAll.back().print();
			return;
		}

		if (currentEvent.time < timeBegin) {
			eventsAll.push_back(Event(currentEvent.time, 13, "", 0, "NotOpenYet"));
			eventsAll.back().print();
			return;
		}

		clientsInClub.push_back(currentEvent.name);

		break;
	}
	case 2:
	{
		if (!tables[currentEvent.table - 1].isFree) {
			eventsAll.push_back(Event(currentEvent.time, 13, "", 0, "PlaceIsBusy"));
			eventsAll.back().print();
			return;
		}

		if (std::find(clientsInClub.begin(), clientsInClub.end(), currentEvent.name)
			== clientsInClub.end()) {
			eventsAll.push_back(Event(currentEvent.time, 13, "", 0, "ClientUnknown"));
			eventsAll.back().print();
			return;
		}

		if (!queue.empty()) {
			if (currentEvent.name == queue.front().name) {
				tables.at(currentEvent.table - 1) = Table(queue.front().name, currentEvent.time);
				queue.erase(queue.begin());
			}
		}
		else {
			tables.at(currentEvent.table - 1) = Table(currentEvent.name, currentEvent.time);
		}

		break;
	}
	case 3:
	{
		if (queue.size() > tablesNumber) {
			clientsInClub.pop_back();
			eventsAll.push_back(Event(currentEvent.time, 11, currentEvent.name));
			eventsAll.back().print();
			return;
		}

		queue.push_back(currentEvent);

		if (find(tables, true) != tables.end()) {
			eventsAll.push_back(Event(currentEvent.time, 13, "", 0, "ICanWaitNoLonger!"));
			eventsAll.back().print();
			return;
		}

		break;
	}
	case 4:
	{
		if (std::find(clientsInClub.begin(), clientsInClub.end(), currentEvent.name)
			== clientsInClub.end()) {
			eventsAll.push_back(Event(currentEvent.time, 13, "", 0, "ClientUnknown"));
			eventsAll.back().print();
			return;
		}

		auto it = find(tables, currentEvent.name);

		it->incrActiveTime(currentEvent.time, price);
		it->reset();

		clientsInClub.erase(std::find(clientsInClub.begin(), clientsInClub.end(), currentEvent.name));

		if (!queue.empty()) {
			it->currentClient = queue.front().name;
			it->timeIn = currentEvent.time;
			it->isFree = false;
			queue.erase(queue.begin());
			eventsAll.push_back(Event(currentEvent.time, 12, it->currentClient,
				std::distance(tables.begin(), it) + 1));
			eventsAll.back().print();
			return;
		}

		break;
	}
	default:break;
	}
}

void clubClosed(std::vector<Event>& eventsAll,
	std::vector<Table>& tables,
	std::vector<std::string>& clientsInClub,
	std::string timeEnd,
	uint32_t price) {
	if (!clientsInClub.empty()) {
		std::sort(clientsInClub.begin(), clientsInClub.end());
		for (auto it = clientsInClub.begin(); it != clientsInClub.end(); it++) {
			auto tables_it = find(tables, *it);
			if (tables_it != tables.end()) {
				tables_it->incrActiveTime(timeEnd, price);
				tables_it->reset();
			}
			eventsAll.push_back(Event(timeEnd, 11, *it));
			eventsAll.back().print();
		}

		clientsInClub.clear();
	}

	std::cout << timeEnd << std::endl;
}