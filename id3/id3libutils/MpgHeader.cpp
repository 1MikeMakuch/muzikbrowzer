////////////////////////////////////////////////////////////////////////////////
//CMpgHeader.cpp
/* CMpgHeader Copyright(C) 2001 John George K., xeonfusion@users.sourceforge.net
based on the CFrameHeader class Copyright (C) Gustav "Grim Reaper" Munkby  http://home.swipnet.se/grd/  email: grd@swipnet.se
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// #include "stdafx.h"
//#include "mp3tagtools.h"
#include "MpgHeader.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMpgHeader::CMpgHeader()
{
	
}

CMpgHeader::~CMpgHeader()
{

}

BOOL CMpgHeader::testValidHeader(const char* header)
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

BOOL CMpgHeader::getValidHeader(const char* file)
{
// 	CFile mp3file;
	long header1pos = -1;
	long header2pos;
	long vbrpos;
	char hdr1buff[5];
	char hdr2buff[5];
	BOOL bValidHdr;
	
	int hdr1posmax = (int)getSize()/4;

// 	mp3file.Open( file, CFile::modeRead | CFile::typeBinary);
	FILE *fd = fopen(file, "r");
	if (fd == NULL) {
		printf("error opening %s\n", file);
		exit(1);
	}
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

			int s = fseek(fd, header1pos, SEEK_SET);
			if (s == -1) {
				memset(hdr1buff,0,5);
				header1pos=-1;
				break;
			}

// 			TRY
// 			{
// 			  mp3file.Seek(header1pos, CFile::begin);
// 			}
// 			CATCH( CFileException, e )
// 			{
// 				if( e->m_cause == CFileException::endOfFile)
// 					{
// 					memset(hdr1buff,0,5);
// 					header1pos=-1;
// 					break;
// 					}
// 			}
// 			END_CATCH
			
			memset(hdr1buff,0,5);
// 			mp3file.Read(hdr1buff, 4);
			size_t r = fread(hdr1buff, 1, 4, fd);
			if (r != 4) {
				printf("error reading hdr1buff\n");
				exit(1);
			}
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

			int s = fseek(fd, header2pos, SEEK_SET);
			if (s == -1) {
				memset(hdr2buff,0,5);
				header2pos=-1;
				break;
			}

// 		TRY
// 		{
// 		  mp3file.Seek(header2pos, CFile::begin);
// 		}
// 		CATCH( CFileException, e )
// 		{
// 			if( e->m_cause == CFileException::endOfFile)
// 				{
// 				memset(hdr2buff,0,5);
// 				header2pos=-1;
// 				break;
// 				}
// 		}
// 		END_CATCH

		size_t r = fread(hdr2buff, 1, 4, fd);
		if (r != 4) {
			printf("error reading hdr2buff\n");
			exit(1);
		}
// 		mp3file.Read(hdr2buff, 4);
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
			int s = fseek(fd, vbrpos, SEEK_SET);
			size_t r = fread(m_vbrheaderchars, 1, 12, fd);
			if (r != 12) {
				printf("error reading m_vbrheaderchars\n");
				exit(1);
			}
// 				mp3file.Seek(vbrpos, CFile::begin);
// 				mp3file.Read(m_vbrheaderchars, 12);
				bIsVBitRate = testVBRHeader(m_vbrheaderchars);
			}
	}	
	headerpos = header1pos;
// 	mp3file.Close();
	fclose(fd);
	
	if (headerpos > -1)
		return TRUE;

	return FALSE;

}

float CMpgHeader::getVersion()
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
int CMpgHeader::getLayer()
{

    // when speaking of layers there is a 
    // cute coincidence, the Layer always
    // eauals 4 - layerIndex, so that's what
    // we will return
    return ( 4 - getLayerBits() );

}


// this returns the current bitrate [8-448 kbps]
int CMpgHeader::getBitrate()
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
int CMpgHeader::getFrequency()
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
CString CMpgHeader::getMode()
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

long CMpgHeader::getSize()
{
// 	CFile mp3file;
// 	CFileStatus status;
// 	mp3file.GetStatus(m_filepath, status);
// 	return status.m_size;

	struct stat fstat;
	int s;
	s = stat(m_filepath, &fstat);
	return fstat.st_size;
}

BOOL CMpgHeader::getProtection()
{
	return getProtectionBits();
}

BOOL CMpgHeader::getCopyright()
{
	return getCopyrightBits();
}

BOOL CMpgHeader::getOriginal()
{
	return getOriginalBits();
}

CString CMpgHeader::getEmphasis()
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

int CMpgHeader::getLengthInSeconds()
{
	int filesizeinbits = ( 8*(getSize() - headerpos) );
	if(getBitrate()) return filesizeinbits/(1000*getBitrate());
	else return 0;
}

structMpgHeader CMpgHeader::getHeaderInfoStruct()
{
	
	if(headerpos != -1)
	{
	m_structmpgheader.lSize = getSize();
	m_structmpgheader.lHeaderPosition = headerpos;
	m_structmpgheader.ilength = getLengthInSeconds();
	m_structmpgheader.fVersion = getVersion();
	m_structmpgheader.iLayer = getLayer();
	m_structmpgheader.iBitrate = getBitrate();
	m_structmpgheader.iFrames = getNumberOfFrames();
	m_structmpgheader.iFrequency = getFrequency();
	m_structmpgheader.szMode = getMode();
	m_structmpgheader.bCRC = getProtection();
	m_structmpgheader.bCopyright = getCopyright();
	m_structmpgheader.bOriginal = getOriginal();
	m_structmpgheader.szEmphasis = getEmphasis();
	m_structmpgheader.bNoHeader = FALSE;
	m_structmpgheader.bVBRHeader = bIsVBitRate;
	}
	else
	{
	m_structmpgheader.lSize = getSize();
	m_structmpgheader.lHeaderPosition = 0;
	m_structmpgheader.ilength = 0;
	m_structmpgheader.fVersion = 0;
	m_structmpgheader.iLayer = 0;
	m_structmpgheader.iBitrate = 0;
	m_structmpgheader.iFrames = 0;
	m_structmpgheader.iFrequency = 0;
	m_structmpgheader.szMode = "Invalid";
	m_structmpgheader.bCRC = 1;
	m_structmpgheader.bCopyright = 0;
	m_structmpgheader.bOriginal = 0;
	m_structmpgheader.szEmphasis = "Invalid";
	m_structmpgheader.bNoHeader = TRUE;
	m_structmpgheader.bVBRHeader = FALSE;
	}
	return m_structmpgheader;
}

int CMpgHeader::getFrameLength()
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

BOOL CMpgHeader::testVBRHeader(char vbrheader [13])
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

int CMpgHeader::getNumberOfFrames()
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

unsigned long CMpgHeader::getHeaderAsBits(const char* header)
{
	return   (unsigned long)( ((header[0] & 255) << 24) |
		            		  ((header[1] & 255) << 16) |
							  ((header[2] & 255) <<  8) |
							   (header[3] & 255) );
	
}

BOOL CMpgHeader::getFilePath(const char *file)
{
	m_filepath = file;
	bIsVBitRate = FALSE;
	return getValidHeader(m_filepath);
}
