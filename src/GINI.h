#pragma once

#include <stdint.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <map>

using namespace std;

struct GINIEntry {
	GINIEntry(string sKey, string sVal, string sSection = "")
		:key(sKey)
		,value(sVal)
		,section(sSection) 
	{
	}
	string key;
	string value;
	string section;
	
};

enum GINITypes {
 	 GINI_COMMENT
	,GINI_NEW_LINE
	,GINI_ENTRY
};

struct GINISortedEntry {
	GINISortedEntry(string sValue, int nType)
		:value(sValue)
		,type(nType)
	{
	}
	string value;
	int type;
};

class GINI {
public:	
	GINI();
	
	// storage
	bool load(string sFile);
	bool save(string sFile);
	
	
	// retrieve settings
	string	getString(string sKey, string sDefault = "");
	int		getInt(string sKey, int nDefault = 0);
	float	getFloat(string sKey, float nDefault = 0.0f);
	double	getDouble(string sKey, double nDefault = 0.0);
	bool	getBool(string sKey, bool bDefault = false);


	// store settings.
	void	setString(string sKey, string sValue);
	void	setInt(string sKey, int nValue);
	void	setFloat(string sKey, float nValue);
	void	setDouble(string sKey, double nValue);
	void	setBool(string sKey, bool nValue);
	
	// helpers
	// -------------------------------------------------------------------------
	inline bool get(string sKey, string& rFound) {
		map<string, GINIEntry*>::iterator it = dict.find(sKey);
		if(it == dict.end()) {
			//cout << "notice:" << sKey << " not found. " << endl;
			return false;
		}
		rFound = (*it->second).value;
		return true;
	}
	
	template<typename T>
	T convert(string sIn) {
		T result;
		stringstream ss;
		ss << sIn;
		ss >> result;
		return result;
	}
	
	template<typename T>
	void setValue(string sKey, T v) {
		GINIEntry* e = getEntry(sKey);
		if(e == NULL) {
			//cout << "notice: entry: '" << sKey << "' not found in dictionary, creating new one" << endl;
			e = createNewEntry(sKey,v);
			if(e == NULL) {
				//cout << "error: cannot create new entry: " << sKey << endl;
				return;
			}
		}
	
		stringstream ss;
		ss << v;
		ss >> e->value;
	}

	inline GINIEntry* getEntry(string sKey) {
		map<string, GINIEntry*>::iterator it = dict.find(sKey);
		if(it == dict.end()) {
			return NULL;
		}
		return it->second;
	}
	
	template<typename T> 
	inline GINIEntry* createNewEntry(string sDictKey, T v) {
		stringstream ss(sDictKey);
		string s;
		vector<string> toks;
		while(getline(ss,s,'.')) {
			toks.push_back(s);
		}
		if(toks.size() != 2) {
//			cout << "error: dictionary key is incorrect. should be [section].[key]" << endl;
			return NULL;
		}
		return createNewEntry(toks[0], toks[1],v);
	}
	
	template<typename T>
	inline GINIEntry* createNewEntry(string sSection, string sKey, T v) {
		// just a check.
		GINIEntry* e = getEntry(sKey);
		if(e != NULL) {
//			cout << "error: " << sKey << " already exists; cant add it" << endl;
			return NULL;
		}
		
		stringstream ss;
		ss << v;
		
		GINIEntry* entry = new GINIEntry(sKey, ss.str(), sSection);
		string dict_key = sSection +sKey;
		dict.insert(pair<string,GINIEntry*>(dict_key,entry));
		GINISortedEntry* sorted_entry = new GINISortedEntry(dict_key, GINI_ENTRY);
		sorted_entries.push_back(sorted_entry);
		return entry;
	}
	
	friend ostream& operator<<(ostream& os, const GINI& GINI);
	
private: 
	map<string, GINIEntry*> dict;
	vector<GINISortedEntry*> sorted_entries;
};


inline ostream& operator<<(ostream& os, const GINI& GINI) {
	string curr_section = "";
	vector<GINISortedEntry*>::const_iterator it = GINI.sorted_entries.begin();
	while(it != GINI.sorted_entries.end()) {
		const GINISortedEntry* entry = (*it);
		if(entry->type == GINI_ENTRY) {
			map<string, GINIEntry*>::const_iterator map_it = GINI.dict.find(entry->value);
			if(map_it == GINI.dict.end()) {
				cout << "error: trying to store GINI but key is removed: '" << (*it)->value << endl;
				++it;
				continue;
			}
			GINIEntry& e = (*map_it->second);
			if(curr_section != e.section) {
				curr_section = e.section;
				os << "[" << e.section << "]" << endl;
			}
			os << e.key << "=" << e.value << endl;
		}
		else if(entry->type == GINI_COMMENT) {
			os << entry->value << endl;
		}
		else if(entry->type == GINI_NEW_LINE) {
			os << endl;
		}
		++it;
	}
	return os;
}