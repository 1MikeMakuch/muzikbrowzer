

#if !defined(_MUSICPLAYERX_HH_)
#define _MUSICPLAYERX_HH_

//#include "resource.h"		// main symbols
#include "XaudioPlayer.h"

// uncomment the following line to use the AAC codec
// you'll also need to uncomment one of the two following
// lines to specify the linkage type for the codec
//#define MODULES_ENABLE_AAC_CODEC

// uncomment this if you want the AAC codec to be linked statically
// (if you choose this option, remember to add the xa_aac_codec.lib
//  static library in the 'link' section of the settings for your
//  project so that the linker can find the codec implementation)
//#define MODULES_CONFIG_AAC_CODEC_LINKAGE_STATIC

// uncomment this if you want the AAC codec to be linked dynamically
// (if you choose this option, remember to put xa_aac_codec.dll
//  where your executable application is being built so that it
//  can be found by the dynamic linker)
//#define MODULES_CONFIG_AAC_CODEC_LINKAGE_DYNAMIC


// uncomment the following lines to use the QDesign codec
// you'll also need to uncomment one of the two following
// lines to specify the linkage type for the codec
//#define MODULES_ENABLE_QDMC_CODEC

// uncomment this if you want the QDesign codec to be linked statically
// (if you choose this option, remember to add the xa_qdmc_codec.lib
//  static library in the 'link' section of the settings for your
//  project so that the linker can find the codec implementation)
//#define MODULES_CONFIG_QDMC_CODEC_LINKAGE_STATIC

// uncomment this if you want the QDesign codec to be linked dynamically
// uncomment this if you want the AAC codec to be linked dynamically
// (if you choose this option, remember to put xa_qdmc_codec.dll
//  where your executable application is being built so that it
//  can be found by the dynamic linker)
//#define MODULES_CONFIG_QDMC_CODEC_LINKAGE_DYNAMIC

/////////////////////////////////////////////////////////////////////////////
// MusicPlayerX:
// This class is a subclass of the generic XaudioPlayer
// It overrides some methods to implement actions triggered
// by notification messages
class CPlayerDlg;
class MusicPlayerX : public XaudioPlayer
{
public:
    // member variables
    
    XA_PlayerState m_State;

    // methods
    MusicPlayerX(HINSTANCE instance, CPlayerDlg *);
    void OnNotifyReady();
    void OnNotifyNack(XA_NackInfo *info);
    void OnNotifyProgress(XA_ProgressInfo *info);
    void OnNotifyPlayerState(XA_PlayerState state);
    void OnNotifyInputPosition(unsigned long offset, unsigned long range);
    void OnNotifyInputName(const char *name);
    void OnNotifyInputState(XA_InputState state);
    void OnNotifyInputTimecode(XA_TimecodeInfo *info);
    void OnNotifyInputModuleInfo(XA_ModuleInfo *info);
    void OnNotifyOutputState(XA_OutputState state);
    void OnNotifyOutputModuleInfo(XA_ModuleInfo *info);
    void OnNotifyOutputMasterLevel(unsigned char level);
    void OnNotifyOutputPcmLevel(unsigned char level);
    void OnNotifyOutputBalance(unsigned char balance);
    void OnNotifyStreamDuration(unsigned long duration);
    void OnNotifyStreamMimeType(const char *mime_type);
    void OnNotifyStreamProperties(XA_PropertyList *properties);
    void OnNotifyStreamParameters(XA_StreamParameters *parameters);
//    void OnNotifyCodecEqualizer(XA_EqualizerInfo *equalizer);
    void OnNotifyCodecModuleInfo(XA_ModuleInfo *info);
	void setScrolling(BOOL b) {
		m_Scrolling = b;
	}
	BOOL isScrolling() { return m_Scrolling; }
private:
    CPlayerDlg *m_PlayerDlg;
	BOOL m_Scrolling;
};

/////////////////////////////////////////////////////////////////////////////

#endif
