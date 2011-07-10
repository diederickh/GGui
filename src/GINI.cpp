#include "GINI.h"

// GINI
// -----------------------------------------------------------------------------
GINI::GINI() {
}

bool GINI::load(string sFile) {
	ifstream ifs(sFile.c_str());
	if(!ifs.is_open()) {
		cout << "error: cannot load GINI file:" << sFile << endl;
		return false;
	}
	string line;
	string section;
	while(getline(ifs,line)) {

		// empty line.
		if(line.size() == 0) {
			GINISortedEntry* se = new GINISortedEntry("", GINI_NEW_LINE);
			sorted_entries.push_back(se);
			continue;
		}
			
		// get key
		string key;
		int key_end = 0;
		for(int i = 0; i < line.size(); ++i) {
			if(line[i] == '=') {
				key_end = i+1;
				break;
			}
			else {
				if(line[i] != ' ') {
					key.push_back(line[i]);
				}
			}
		}
		
		// is it a comment?
		if(key.at(0) == '#') {
			GINISortedEntry* sorted_entry = new GINISortedEntry(line, GINI_COMMENT);
			sorted_entries.push_back(sorted_entry);
			continue;
		}
		
		// is key a section?
		if(key.at(0) == '[') {
			section = "";
			for(int i = 0; i < key.size(); ++i) {
				if(key[i] != '[' && key[i] != ']')  {
					section.push_back(key[i]);
				}
			}
			continue;
		}
		
		// get value.
		string val;
		for(int i = key_end; i < line.size(); ++i) {
			if(line[i] == '\r' || line[i] == '\n') {
				break;
			}
			val.push_back(line[i]);
		}
			

		// trim 
		string key_copy = key;
		if(section != "") {
			key = "." +key;
		}
		string dict_key = section + key;
		GINIEntry* entry = new GINIEntry(key_copy, val, section);
		dict.insert(pair<string,GINIEntry*>(dict_key,entry));
		GINISortedEntry* sorted_entry = new GINISortedEntry(dict_key, GINI_ENTRY);
		sorted_entries.push_back(sorted_entry);
	}
	return true;
}

bool GINI::save(string sFile) {
	ofstream ofs(sFile.c_str(), std::ios::out | std::ios::trunc);
	if(!ofs.is_open()) {
		cout << "error: cannot open file '" << sFile << "' for writing" << endl;
		return false;
	}
	ofs << *this;
	ofs.flush();
	ofs.close();
	return true;
}

string GINI::getString(string sKey, string sDefault) {
	string val;
	if(!get(sKey,val)) {
		cout << sKey << " not found" << endl;
		return sDefault;
	}
	return val;
}

int GINI::getInt(string sKey, int nDefault) {
	string val;
	if(!get(sKey,val)) {
		return nDefault;
	}
	return convert<int>(val);
}

float GINI::getFloat(string sKey, float nDefault) {
	string val;
	if(!get(sKey,val)) {
		return nDefault;
	}
	return convert<float>(val);
}

double GINI::getDouble(string sKey, double nDefault) {
	string val;
	if(!get(sKey,val)) {
		return nDefault;
	}
	convert<double>(val);
}

bool GINI::getBool(string sKey, bool bDefault) {
	string val;
	if(!get(sKey,val)) {
		return bDefault;
	}
	
	if(val == "n" || val == "N" || val == "no" || val == "NO" || val == "0" || val == "false" || val == "FALSE") {
		return false;
	}
	if(val == "y" || val == "Y" || val == "yes" || val == "YES" || val == "1" || val == "true" || val == "TRUE") {
		return true;
	}
	return bDefault;
}

void GINI::setString(string sKey, string sValue) {
	GINIEntry* e = getEntry(sKey);
	if(e == NULL) {
		cout << "error: entry: '" << sKey << "' not found in dictionary" << endl;
		return;
	}
	e->value = sValue;
}

void GINI::setInt(string sKey, int nValue) {
	setValue<int>(sKey,nValue);
}

void GINI::setFloat(string sKey, float nValue) {
	setValue<float>(sKey, nValue);
}

void GINI::setDouble(string sKey, double nValue) {
	setValue<double>(sKey, nValue);
}

void GINI::setBool(string sKey, bool nValue) {
	setValue<bool>(sKey, nValue);
}