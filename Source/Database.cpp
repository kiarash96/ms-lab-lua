#include "Database.h"
#include "Console.h"
#include "MainFrame.h"
#include "SDLDialog.h"
#include <SDL/SDL_gfxPrimitives.h>
#include <algorithm>
#include <fstream>
#include <boost/lexical_cast.hpp>
using namespace std;

Database::Database() {

}

Database::~Database() {
	clear();
}

Database::Database(const Database &db) {
	for(size_t i = 0; i < db.table.size(); i ++) {
		Column *column = newColumn(db.table[i].first.c_str());
		for(size_t j = 0; j < db.table[i].second->getSize(); j ++)
			column->push_back(db.table[i].second->at(j));
	}
}

void Database::operator= (const Database &db) {
	for(size_t i = 0; i < db.table.size(); i ++) {
		Column *column = newColumn(db.table[i].first.c_str());
		for(size_t j = 0; j < db.table[i].second->getSize(); j ++)
			column->push_back(db.table[i].second->at(j));
	}
}

void Database::clear() {
	for(size_t i = 0; i < table.size(); i ++)
		delete table[i].second;
	table.clear();
}

Column* Database::newColumn(const char name[]) {
	table.push_back(make_pair(name, new Column()));
	return table.back().second;
}

void Database::addColumn(const char name[], Column *column) {
	table.push_back(make_pair(name, column));
}

Database Database::random(int n) const {
	int minC = table[0].second->getSize();
	for(size_t i = 0; i < table.size(); i ++)
		if(table[i].second->getSize() < minC)
			minC = table[i].second->getSize();

	srand(time(0));
	vector<int> nums;
	for(int i = 0; i < n; i ++) {
		bool duplicate = false;
		int tmp = 0;
		do {
			duplicate = false;
			tmp = rand() % minC;
			for(size_t j = 0; j < nums.size(); j ++)
				if(tmp == nums[j])
					duplicate = true;
		} while(duplicate);

		nums.push_back(tmp);
	}
	
	Database result;
	for(size_t i = 0; i < table.size(); i ++) {
		Column *tmp = result.newColumn(table[i].first.c_str());
		for(int j = 0; j < nums.size(); j ++)
			tmp->push_back(table[i].second->at(nums[j]));
	}
	return result;
}

void Database::print() const {
	for(size_t i = 0; i < table.size(); i ++) {
		Console::GetSingleton() << wxString(table[i].first.c_str(), wxConvUTF8) << wxT(": ");
		table[i].second->print();
	}
}

void Database::save(const string &filename) const {
	if(filename.substr(filename.find_last_of('.')) == ".xlsx")
		saveExcel(filename);
}

void Database::load(const string &filename) {
	if(filename.substr(filename.find_last_of('.')) == ".xlsx")
		loadExcel(filename);
}

void Database::drawPie(int width, int height) const {
	SDLDialog *dlg = new SDLDialog(MainFrame::GetSingletonPtr(), width, height);
	
	int sum = 0;
	for(int i = 0; i < table[0].second->getSize(); i ++)
		sum += boost::lexical_cast<int>(table[0].second->at(i));

	float start = 0;
	for(int i = 0; i < table[0].second->getSize(); i ++) {
		float data = boost::lexical_cast<int>(table[0].second->at(i));
		float end = start + (data * 360 / sum);
		filledPieRGBA(dlg->getSurface(), width/2, height/2, min(width, height)/2 - 50, start, end, rand()%255, rand()%255, rand()%255, 255);
		start = end;
	}
	
	dlg->Show();
}

void Database::saveExcel(const string &filename) const {
	// TODO: Write save code here
}

void Database::loadExcel(const string &filename) {
	// TODO: Write load code here
}
