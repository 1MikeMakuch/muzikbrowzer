
#include "stdafx.h"
#include "Mp3Header.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


Mp3Header::Mp3Header()
{
	
}

Mp3Header::~Mp3Header()
{

}

BOOL Mp3Header::testValidHeader(const char* header)
{
	headerbits = getHeaderAsBits(header);
	
	return ( ((getFrameSyncBits() & 2047)==2047) &&
			 ((getVersionBits()   &    3)!=   1) &&
             ((getLayerBits()     &    3)!=   0) && 
             ((getBitrateBits()   &   15)!=   0) &&  
			 ((getBitrateBits()   &   15)!=  15) &&  
             ((getFrequencyBits() &    3)!=   3) &&
             ((getEmphasisBits()  &    3)!=   2)    ) ;
	
	

}

BOOL Mp3Header::getValidHeader(const char* file)
{
	CFile mp3file;
	long header1pos = -1;
	long header2pos;
	long vbrpos;
	char hdr1buff[5];
	char hdr2buff[5];
	BOOL bValidHdr;
	
	int hdr1posmax = (int)getSize()/4;

	TRY {
		mp3file.Open( file, CFile::modeRead | CFile::typeBinary);
	} CATCH (CFileException, e) {
		return FALSE;
	}
	END_CATCH

	//find the first valid header
	do
	{
		do	
		{
			header1pos++;
			if(header1pos >= hdr1posmax)
			{
				header1pos=-1;
				break;
			}
			
			TRY
			{
				mp3file.Seek(header1pos, CFile::begin);
			}
			CATCH( CFileException, e )
			{
				if( e->m_cause == CFileException::endOfFile)
				{
					memset(hdr1buff,0,5);
					header1pos=-1;
					break;
				} else {
					return FALSE;
				}
			}
			END_CATCH
				
			memset(hdr1buff,0,5);
			mp3file.Read(hdr1buff, 4);
		}
		while(!testValidHeader(hdr1buff) );
		
		if(header1pos == -1) break;
		
		//confirm that the header we found is indeed the correct one
		header2pos = header1pos + getFrameLength();
		
		if(header2pos >= hdr1posmax)
		{
			header1pos=-1;
			break;
		}
		TRY
		{
			mp3file.Seek(header2pos, CFile::begin);
		}
		CATCH( CFileException, e )
		{
			if( e->m_cause == CFileException::endOfFile)
			{
				memset(hdr2buff,0,5);
				header2pos=-1;
				break;
			}
		}
		END_CATCH
			
		mp3file.Read(hdr2buff, 4);
	}
	while(!testValidHeader(hdr2buff) );
	
	if(header1pos != -1)
	{
		//test if VBR header
		bValidHdr = testValidHeader(hdr1buff);
		if(bValidHdr)
		{
			vbrpos = header1pos+4;
			
			
			/******************************************************/
			/* check for an vbr-header, to ensure the info from a */
			/* vbr-mp3 is correct                                 */
			/******************************************************/
			// determine offset of vbr header from first frame-header
			// it depends on two things, the mpeg-version
			// and the mode(stereo/mono)
			
			if(getVersionBits()==3 )
			{ // mpeg version 1
				
				if(getModeBits()==3 ) vbrpos += 17; // Single Channel
				else                  vbrpos += 32;
				
			}
			else
			{ // mpeg version 2 or 2.5
				
				if(getModeBits()==3 ) vbrpos +=  9; // Single Channel
				else                  vbrpos += 17;
			}
			
			// read next twelve bits in
			mp3file.Seek(vbrpos, CFile::begin);
			mp3file.Read(m_vbrheaderchars, 12);
			bIsVBitRate = testVBRHeader(m_vbrheaderchars);
		}
	}	
	headerpos = header1pos;
	mp3file.Close();
	
	if (headerpos > -1) 
		return TRUE;
	
	return FALSE;
	
}

float Mp3Header::getVersion()
{

    // a table to convert the indexes into
    // something informative...
    float table[4] = {
                      2.5, 0.0, 2.0, 1.0
                     };

    // return modified value
    return table[getVersionBits()];

}


// this returns the Layer [1-3]
int Mp3Header::getLayer()
{

    // when speaking of layers there is a 
    // cute coincidence, the Layer always
    // eauals 4 - layerIndex, so that's what
    // we will return
    return ( 4 - getLayerBits() );

}


// this returns the current bitrate [8-448 kbps]
int Mp3Header::getBitrate()
{
	if (bIsVBitRate)
	{

     // get average frame size by dividing fileSize by the number of frames
        
     float avrFrameSize = (float)(getSize() - headerpos) / (float)getNumberOfFrames();
        
        
     //   BitRate = FrameSize*SampleRate/(mpeg1?12:144)* 1000;
        

     return (int)( 
                     ( avrFrameSize * (float)getFrequency() ) / 
                     ( 1000.0 * ( (getLayerBits()==3) ? 12.0 : 144.0))
                 );

    }
	else
	{

		// a table to convert the indexes into
		// something informative...
		const int table[2][3][16] =
		{
			{         //MPEG 2 & 2.5
				{0,  8, 16, 24, 32, 40, 48, 56, 64, 80, 96,112,128,144,160,0}, //Layer III
				{0,  8, 16, 24, 32, 40, 48, 56, 64, 80, 96,112,128,144,160,0}, //Layer II
				{0, 32, 48, 56, 64, 80, 96,112,128,144,160,176,192,224,256,0}  //Layer I
			},{       //MPEG 1
				{0, 32, 40, 48, 56, 64, 80, 96,112,128,160,192,224,256,320,0}, //Layer III
				{0, 32, 48, 56, 64, 80, 96,112,128,160,192,224,256,320,384,0}, //Layer II
				{0, 32, 64, 96,128,160,192,224,256,288,320,352,384,416,448,0}  //Layer I
			}
		};

		// the bitrate is not only dependent of the bitrate index,
		// the bitrate also varies with the MPEG version and Layer version
		return table[(getVersionBits() & 1)][(getLayerBits() -1)][getBitrateBits()];
	}

}


// this returns the current frequency [8000-48000 Hz]
int Mp3Header::getFrequency()
{

    // a table to convert the indexes into
    // something informative...
    int table[4][3] = {

        {11025, 12000,  8000}, //MPEG 2.5
        {    0,     0,     0}, //reserved
        {22050, 24000, 16000}, //MPEG 2
        {44100, 48000, 32000}  //MPEG 1

    };

    // the frequency is not only dependent of the bitrate index,
    // the bitrate also varies with the MPEG version
    return table[getVersionBits()][getFrequencyBits()];

}

// the purpose of getMode is to get information about
// the current playing mode, such as:
// "Joint Stereo"
CString Mp3Header::getMode()
{
    switch(getModeBits())
	{
        default:
            modestring = "Stereo";
            break;

        case 1:
            modestring = "Joint Stereo";
            break;

        case 2:
            modestring = "Dual Channel";
            break;

        case 3:
            modestring = "Mono";
            break;
    }
	return modestring;

}

long Mp3Header::getSize()
{
	CFile mp3file;
	CFileStatus status;
	mp3file.GetStatus(m_filepath, status);
	return status.m_size;
	
}

BOOL Mp3Header::getProtection()
{
	return getProtectionBits();
}

BOOL Mp3Header::getCopyright()
{
	return getCopyrightBits();
}

BOOL Mp3Header::getOriginal()
{
	return getOriginalBits();
}

CString Mp3Header::getEmphasis()
{
	switch(getEmphasisBits())
	{
        default:
            emphasisstring = "None";
            break;

        case 1:
            emphasisstring = "50/15 ms";
            break;

        case 2:
            emphasisstring = "reserved";
            break;

        case 3:
            emphasisstring = "CCIT J.17";
            break;
    }
	return emphasisstring;
}

int Mp3Header::getLengthInSeconds()
{
	int filesizeinbits = ( 8*(getSize() - headerpos) );
	if(getBitrate()) return filesizeinbits/(1000*getBitrate());
	else return 0;
}

structMp3Header Mp3Header::getHeaderInfoStruct()
{
	
	if(headerpos != -1)
	{
	m_structMp3Header.lSize = getSize();
	m_structMp3Header.lHeaderPosition = headerpos;
	m_structMp3Header.ilength = getLengthInSeconds();
	m_structMp3Header.fVersion = getVersion();
	m_structMp3Header.iLayer = getLayer();
	m_structMp3Header.iBitrate = getBitrate();
	m_structMp3Header.iFrames = getNumberOfFrames();
	m_structMp3Header.iFrequency = getFrequency();
	m_structMp3Header.szMode = getMode();
	m_structMp3Header.bCRC = getProtection();
	m_structMp3Header.bCopyright = getCopyright();
	m_structMp3Header.bOriginal = getOriginal();
	m_structMp3Header.szEmphasis = getEmphasis();
	m_structMp3Header.bNoHeader = FALSE;
	m_structMp3Header.bVBRHeader = bIsVBitRate;
	}
	else
	{
	m_structMp3Header.lSize = getSize();
	m_structMp3Header.lHeaderPosition = 0;
	m_structMp3Header.ilength = 0;
	m_structMp3Header.fVersion = 0;
	m_structMp3Header.iLayer = 0;
	m_structMp3Header.iBitrate = 0;
	m_structMp3Header.iFrames = 0;
	m_structMp3Header.iFrequency = 0;
	m_structMp3Header.szMode = "Invalid";
	m_structMp3Header.bCRC = 1;
	m_structMp3Header.bCopyright = 0;
	m_structMp3Header.bOriginal = 0;
	m_structMp3Header.szEmphasis = "Invalid";
	m_structMp3Header.bNoHeader = TRUE;
	m_structMp3Header.bVBRHeader = FALSE;
	}
	return m_structMp3Header;
}

int Mp3Header::getFrameLength()
{
/*	Read the BitRate, SampleRate and Padding of the frame header. 

	For Layer I files us this formula: 

	FrameLengthInBytes = (12 * BitRate / SampleRate + Padding) * 4 

	For Layer II & III files use this formula: 

	FrameLengthInBytes = 144 * BitRate / SampleRate + Padding 
	For Layer I slot is 32 bits long, for Layer II and Layer III slot is 8 bits long. 
	*/
	if(getFrequency())
	{
	int iframelength;	
	iframelength =(int)(	
							((getLayerBits()==3) ? 4 : 1 )*
							(	
								(
								  ((getLayerBits()==3) ? 12 : 144)*
								 
								  ((1000.0* (float)getBitrate()) / ((float)getFrequency()))
								)
							)
							+
							(            
								(getPaddingBits()*((getLayerBits()==3) ? 4:1))
							)
						);
	//If its MPEG V2 or V2.5 then the correct length will be obtained by division by 2
	if(getVersion() == 1.0)	return iframelength;
	else return (int)(iframelength/2);
	}
	else return 0;        

}

BOOL Mp3Header::testVBRHeader(char vbrheader [13])
{
	// The Xing VBR headers always begin with the four
    // chars "Xing" so this tests wether we have a VBR
    // header or not
    if( memcmp(vbrheader, "Xing", 4) ) {

        iVbrframes = -1;
        return false;

    }
	else
	{
		iVbrframes = (int) (
							( (vbrheader[ 8] & 255) << 24) |
							( (vbrheader[ 9] & 255) << 16) |
							( (vbrheader[10] & 255) <<  8) |
							( (vbrheader[11] & 255)      )
						   ); 
		return true;
	}

}

int Mp3Header::getNumberOfFrames()
{
	if(bIsVBitRate)
	{
		return iVbrframes;
	}
	else
	{
		if(getFrameLength())
		{
		return (int)( (float)(getSize() - headerpos)/ (float)getFrameLength() );
		}
		else return 0;

	}
}

unsigned long Mp3Header::getHeaderAsBits(const char* header)
{
	return   (unsigned long)( ((header[0] & 255) << 24) |
		            		  ((header[1] & 255) << 16) |
							  ((header[2] & 255) <<  8) |
							   (header[3] & 255) );
	
}

BOOL Mp3Header::getFilePath(const char *file)
{
	CString File = file;
	CString temp;
	temp = File.Right(4);
	if(temp == ".mp3" || temp == ".Mp3" || temp == ".mP3" || temp == ".MP3")
	{
		m_filepath = file;
		bIsVBitRate = FALSE;
		return getValidHeader(m_filepath);
	}
	return FALSE;
}
