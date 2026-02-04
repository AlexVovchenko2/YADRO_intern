#include "EventAndTable.h"


void inputData(std::ifstream& fin,
	uint32_t& tablesNumber,
	std::string& timeBegin,
	std::string& timeEnd,
	uint32_t& price,
	std::vector<Event>& eventsIn);



void generateOutEvent(std::vector<Event>& eventsAll,
	Event& currentEvent,
	std::vector<Table>& tables,
	std::vector<Event>& queue,
	std::vector<std::string>& clientsInClub,
	std::string timeBegin,
	std::string timeEnd,
	uint32_t tablesNumber,
	uint32_t price);

void clubClosed(std::vector<Event>& eventsAll,
	std::vector<Table>& tables,
	std::vector<std::string>& clientsInClub,
	std::string timeEnd,
	uint32_t price);

int main(int argc, char* argv[])
{
	if (argc != 2) {
        std::cerr << "Not enough or too many arguments" << std::endl;
        return 1;
    }
	
	uint32_t tablesNumber, price;
	std::string timeBegin, timeEnd;
	std::vector<Event> eventsIn;
	std::vector<Event> eventsAll;

	std::ifstream fin;
	std::string file = argv[1];  //"input2.txt";
	fin.open(file);

	try {
		if (!fin.is_open()) {
			throw UserException("Cannot open the file: " + file);
		}
		inputData(fin, tablesNumber, timeBegin, timeEnd, price, eventsIn);
	}
	catch (UserException& e) {
		std::cerr << e.what() << std::endl;
		return 1;
	}
	fin.close();

	std::vector<Table> tables;
	for (int i = 0; i < tablesNumber; i++) {
		tables.push_back(Table("", ""));
	}
	std::vector<Event> queue;
	std::vector<std::string> clientsInClub;
	std::cout << timeBegin << std::endl;

	for (int i = 0; i < eventsIn.size(); i++) {
		eventsAll.push_back(eventsIn[i]);
		eventsAll.back().print();

		generateOutEvent(eventsAll, eventsIn[i],
			tables, queue, clientsInClub, timeBegin, timeEnd, tablesNumber, price);
	}

	clubClosed(eventsAll, tables, clientsInClub, timeEnd, price);

	for (size_t i = 0; i < tables.size(); i++) {
		std::cout << i + 1 << " " << tables[i].cash << " " << tables[i].getActiveTimeStr() << std::endl;
	}

	return 0;
}
