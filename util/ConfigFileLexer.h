
#ifndef _MBConfigFileParser_h
#define _MBConfigFileParser_h
#include <io.h>
#include <stack>
using namespace std;

//typedef struct {
//	char * key;
//	char * val;
//} KVPAIR,*PKVPAIR;

class KVPair {
public:
	KVPair():m_key(NULL),m_val(NULL){};
	KVPair(char * key, char * val) {
		m_key = strdup(key);
		m_val = strdup(val);
	}
	KVPair(const KVPair & kvpair) {
		m_key = 
			strdup(kvpair.m_key);
		m_val = 
			strdup(kvpair.m_val);
	}


	~KVPair() {
		if (m_key) free(m_key);
		if (m_val) free(m_val);
	}
	char * key() { return m_key; }
	char * val() { return m_val; }

private:
	char * m_key;
	char * m_val;
};

void ConfigFileParser(const char * file, stack<KVPair> * kvstack);
BOOL ConfigFontParser(const char * str);

#endif
