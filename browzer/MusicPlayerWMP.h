
#ifndef _MUSICPLAYERWMP_H_
#define _MUSICPLAYERWMP_H_

class CWMPPlayer4;
class CWMPError;

class MusicPlayerWMP
{
public:
	MusicPlayerWMP(CWMPPlayer4 *);
	~MusicPlayerWMP();
	int init();
	int InputOpen(const char *name);
	int InputClose();
	int Pause();
	int Play();
	int Seek(long pos, BOOL endpos = FALSE);
	int Stop();
	int SetVolume(int level);
	long GetVolume(int &);
	BOOL isPlaying();
	BOOL isStopped();
	HRESULT HandleGraphEvent(void) { return NULL; }
	long GetFileDuration();
	long ReadMediaPosition(void);
	void ReadMediaPosition(long &, long &, long &);
	CString file() { return m_file; }
	CString GetStatus();
	CString GetError();
private:
	CWMPPlayer4 * m_MP;
	int m_State;
	BOOL m_FileLoaded;
	CString m_file;
};

#endif // ifndef _MUSICPLAYER_H_
