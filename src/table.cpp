#include <table.h>


LinearArrTable::LinearArrTable(size_t size = sz_ar){
	table = new Pol[size];
	size_array = size;
}

void LinearArrTable::addPolynomial(const std::string& polName, const polynomial& pol) {
	if (current_ind_array < size_array) {
		table[current_ind_array].key = polName;
		table[current_ind_array].value = pol;
	}
	current_ind_array++;
}

polynomial LinearArrTable::findPolynomial(const std::string& polName) {
	for (int i = 0; i < size_array; i++) 
		if (table[i].key == polName)
			return table[i].value;
	polynomial A;
	return A;
}

void LinearArrTable::delPolynomial(const std::string& polName) {
	int i = 0;
	bool f = false;
	for (i; i < current_ind_array + 1; i++) 
		if (table[i].key == polName) {
			f = true;
			break;
		}
	if (f == true) {
		for (i++; i < current_ind_array + 1; i++)
			table[i - 1] = table[i];
		current_ind_array--;
	}
}

unsigned int LinearArrTable::size() {
	return size_array;
}

bool LinearArrTable::empty() {
	return current_ind_array == 0;
}


