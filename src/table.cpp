#include <table.h>

// *** LinearArrTable ***

LinearArrTable::LinearArrTable(size_t size){
	table.reserve(size);
}

void LinearArrTable::addPolynomial(const std::string& polName, const polynomial& pol) {
	table.push_back({ polName, pol });
}

std::optional<polynomial> LinearArrTable::findPolynomial(const std::string& polName) {
	for (auto rec: table) 
		if (rec.key == polName)
			return std::optional<polynomial>(rec.value);
	return {};
}

void LinearArrTable::delPolynomial(const std::string& polName) {

	for (int i = 0; i < table.size(); i++) {
		Pol rec = table[i];
		if (rec.key == polName) {
			table.erase(std::next(table.begin(), i)); // needs a lot of testing
			return;
		}
	}
}

unsigned int LinearArrTable::size() {
	return table.size();
}

bool LinearArrTable::empty() {
	return table.empty();
}


