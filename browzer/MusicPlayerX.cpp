
// PlayerApp.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "PlayerDlg.h"
#include "InitDlg.h"
#include "MyLog.h"
#include "MusicPlayerX.h"




#include "stream_input.h"
#ifdef MODULES_ENABLE_AAC_CODEC
#include "aac_codec.h"
#endif
#ifdef MODULES_ENABLE_QDMC_CODEC
#include "qdmc_codec.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// MusicPlayerX constructor
MusicPlayerX::MusicPlayerX(HINSTANCE instance, CPlayerDlg *player): 
    XaudioPlayer(instance),
        m_Scrolling(FALSE),
        m_State(XA_PLAYER_STATE_STOPPED),
        m_PlayerDlg(player)
{
#if defined(MODULES_ENABLE_AAC_CODEC)
#if defined(MODULES_CONFIG_AAC_CODEC_LINKAGE_STATIC)
    XA_MAKE_SYMBOL_NAME_FROM_STATIC(aac_codec_name, aac_codec_module_register);
    CodecModuleRegister(aac_codec_name);
#elif defined(MODULES_CONFIG_AAC_CODEC_LINKAGE_DYNAMIC)
    CodecModuleRegister("xa_aac_codec.dll");
#else
#error need to configure AAC codec linkage type
#endif
#endif

#if defined(MODULES_ENABLE_QDMC_CODEC)
#if defined(MODULES_CONFIG_QDMC_CODEC_LINKAGE_STATIC)
    XA_MAKE_SYMBOL_NAME_FROM_STATIC(qdmc_codec_name, qdmc_codec_module_register);
    CodecModuleRegister(qdmc_codec_name);
#elif defined(MODULES_CONFIG_QDMC_CODEC_LINKAGE_DYNAMIC)
    CocdecModuleRegister("xa_qdmc_codec.dll");
#else
#error need to configure QDesign codec linkage type
#endif
#endif

    // this is the place to add other module registration calls
    // as well as initial player settings
}

/////////////////////////////////////////////////////////////////////////////
// MusicPlayerX::OnNotifyReady
void
MusicPlayerX::OnNotifyReady()
{
    // set some of the player's global options
    SetPlayerMode(XA_PLAYER_MODE_OUTPUT_AUTO_CLOSE_ON_STOP);
//    m_PlayerDlg->appendStatus(CString("Xaudio Ready"));
    m_PlayerDlg->appendStatus(CString(""));
//    SetFeedbackAudioEventRate(100);
//    SetFeedbackHandlerEnvironmentInteger("CLIENT.FEEDBACK-WINDOW", 
//                                         (long)m_PlayerDlg->m_FeedbackWindow.m_hWnd);
//    {
//        char name[128];
//        sprintf(name, "function@%08lx", feedback_module_register);
//        FeedbackHandlerModuleRegister(name);
//    }
//    SetFeedbackHandlerName("sample-spectrum");
//    FeedbackHandlerStart();
}

/////////////////////////////////////////////////////////////////////////////
// MusicPlayerX::OnNotifyNack
void
MusicPlayerX::OnNotifyNack(XA_NackInfo *info)
{
    char message[256];
    sprintf(message,"Failed [%s]", 

            xaudio_error_string(info->code));

       CString m(message);
       if (m == "Failed [no such device]") {
               m = "No Sound Card!";
       }
     m_PlayerDlg->appendStatus(m);

//    Stop();

}

/////////////////////////////////////////////////////////////////////////////
// MusicPlayerX::OnNotifyProgress
void
MusicPlayerX::OnNotifyProgress(XA_ProgressInfo *info)
{
    switch (info->code) {
      case XA_PROGRESS_CODE_STREAM_CONNECTING:
        m_PlayerDlg->appendStatus("CONNECTING");
        break;

      case XA_PROGRESS_CODE_STREAM_CONNECTED:
        m_PlayerDlg->appendStatus("CONNECTED");
        break;

      case XA_PROGRESS_CODE_STREAM_SENDING_REQUEST:
        m_PlayerDlg->appendStatus("SENDING REQUEST");
        break;

      case XA_PROGRESS_CODE_STREAM_BUFFER_UNDERFLOW:
        m_PlayerDlg->appendStatus("BUFFER UNDERFLOW");
        break;

      case XA_PROGRESS_CODE_STREAM_BUFFER_OK:
        m_PlayerDlg->appendStatus("BUFFER REFILLED");
//        m_PlayerDlg->m_NetworkProgressBar1.SetRange(0, 100);
//        m_PlayerDlg->m_NetworkProgressBar1.SetPos(100);
        break;

      case XA_PROGRESS_CODE_STREAM_PREBUFFER_FULLNESS:
//        m_PlayerDlg->m_NetworkProgressBar1.SetRange(0, 100);
//        m_PlayerDlg->m_NetworkProgressBar1.SetPos(info->value);
        break;

      case XA_PROGRESS_CODE_STREAM_BUFFER_FULLNESS:
//        m_PlayerDlg->m_NetworkProgressBar2.SetRange(0, 100);
//        m_PlayerDlg->m_NetworkProgressBar2.SetPos(info->value);
        break;

      case XA_PROGRESS_CODE_STREAM_REACHED_EOS:
        m_PlayerDlg->appendStatus("END OF STREAM");
		  break;
    }
}

/////////////////////////////////////////////////////////////////////////////
// MusicPlayerX::OnNotifyPlayerState
void 
MusicPlayerX::OnNotifyPlayerState(XA_PlayerState state)
{
    CString state_name;
    // remember the state
    m_State = state;

    // compute state name
    switch (state) {
      case XA_PLAYER_STATE_PLAYING:
        state_name = "PLAYING";
        // change the name of the play button to "Pause"
//        m_PlayerDlg->m_PlayButton.SetWindowText("Pause");
    m_PlayerDlg->appendStatus(CString("Playing"));
        break;

      case XA_PLAYER_STATE_EOS:
        state_name = "EOF";
        m_PlayerDlg->appendStatus(CString("EOS"));
        // close and reopen the input
        InputClose();
        InputOpen(NULL);
        break;

      case XA_PLAYER_STATE_STOPPED:
        state_name = "STOPPED";
        // change the name of the play button to "Play"
//        m_PlayerDlg->m_PlayButton.SetWindowText("Play");
            m_PlayerDlg->appendStatus(CString("Stopped"));
            m_PlayerDlg->m_PlayingLabel.setText("");
// Moved this call to Playloop to inside PlayerDlg at each place where we do a stop
//            m_PlayerDlg->PlayLoop();
        break;

      case XA_PLAYER_STATE_PAUSED:
        // if we're sliding the bar, ignore this message
        if (isScrolling()) return;
        state_name = "PAUSED";
        // change the name of the play button to "Play"
//        m_PlayerDlg->m_PlayButton.SetWindowText("Play");
            m_PlayerDlg->appendStatus(CString("Paused"));
        break;

      default: return;
    }



    // display the current state
//    m_PlayerDlg->m_StateLabel.SetWindowText(state_name);
}

/////////////////////////////////////////////////////////////////////////////
// MusicPlayerX::OnNotifyInputName
void 
MusicPlayerX::OnNotifyInputName(const char *name)
{
    // a file has been openned
//    m_PlayerDlg->m_NameLabel.SetWindowText(name);
//        m_PlayerDlg->appendStatus(CString(name));
}

/////////////////////////////////////////////////////////////////////////////
// MusicPlayerX::OnNotifyInputPosition
void 
MusicPlayerX::OnNotifyInputPosition(unsigned long offset, unsigned long range)
{
    // show position on the slider
//    m_PlayerDlg->m_Progress.SetRange32(0,range );
//    m_PlayerDlg->m_Progress.SetPos(offset);
    m_PlayerDlg->m_Slider.SetPos(offset);
}

/////////////////////////////////////////////////////////////////////////////
// MusicPlayerX::OnNotifyInputState
void 
MusicPlayerX::OnNotifyInputState(XA_InputState state)
{
    BOOL enabled;

    switch (state) {
      case XA_INPUT_STATE_OPEN:
        // enable the play and stop buttons, and the slider
        enabled = TRUE;
        // autoplay
//        if (m_PlayerDlg->m_AutoPlayCheckBox.GetCheck()) Play();
            m_PlayerDlg->appendStatus(CString("Open"));
        break;

      case XA_INPUT_STATE_CLOSED:
        // disable the play and stop buttons, and the slider
        enabled = FALSE;
            m_PlayerDlg->appendStatus(CString("Closed"));
        break;
    }

//    m_PlayerDlg->m_PlayButton.EnableWindow(enabled);
//    m_PlayerDlg->m_StopButton.EnableWindow(enabled);
//    m_PlayerDlg->m_Progress.EnableWindow(enabled);
}

/////////////////////////////////////////////////////////////////////////////
// MusicPlayerX::OnNotifyInputTimecode
void 
MusicPlayerX::OnNotifyInputTimecode(XA_TimecodeInfo *info)
{
//    char caption[128];

//    sprintf(caption, "%02d:%02d", 
//            info->h,
//            info->m,
//            info->s);
//    m_PlayerDlg->m_TimecodeLabel.SetWindowText(caption);
 
}

/////////////////////////////////////////////////////////////////////////////
// MusicPlayerX::OnNotifyInputModuleInfo

void 
MusicPlayerX::OnNotifyInputModuleInfo(XA_ModuleInfo *info)
{
//    if (InfoDialog == NULL) return;
//    InfoDialog->m_InputList.AddString(info->name);
}


/////////////////////////////////////////////////////////////////////////////
// MusicPlayerX::OnNotifyOutputState
void 
MusicPlayerX::OnNotifyOutputState(XA_OutputState state)
{
    switch (state) {
      case XA_OUTPUT_STATE_OPEN:
        // enable the volume button
//        m_PlayerDlg->m_VolumeButton.EnableWindow(TRUE);
        break;

      case XA_INPUT_STATE_CLOSED:
        // disable volume button
//        m_PlayerDlg->m_VolumeButton.EnableWindow(FALSE);
        break;
    }
}

/////////////////////////////////////////////////////////////////////////////
// MusicPlayerX::OnNotifyOutputModuleInfo

void 
MusicPlayerX::OnNotifyOutputModuleInfo(XA_ModuleInfo *info)
{
//    if (InfoDialog == NULL) return;
//    InfoDialog->m_OutputList.AddString(info->name);    
}


/////////////////////////////////////////////////////////////////////////////
// MusicPlayerX::OnNotifyOutputMasterLevel
void 
MusicPlayerX::OnNotifyOutputMasterLevel(unsigned char level)
{
//    if (VolumeDialog == NULL) return;
//    VolumeDialog->m_MasterSlider.SetPos(level);
}

/////////////////////////////////////////////////////////////////////////////
// MusicPlayerX::OnNotifyOutputPcmLevel
void 
MusicPlayerX::OnNotifyOutputPcmLevel(unsigned char level)
{
//    if (VolumeDialog == NULL) return;
//    VolumeDialog->m_PcmSlider.SetPos(level);
}

/////////////////////////////////////////////////////////////////////////////
// MusicPlayerX::OnNotifyOutputBalance
void 
MusicPlayerX::OnNotifyOutputBalance(unsigned char balance)
{
//    if (VolumeDialog == NULL) return;
//    VolumeDialog->m_BalanceSlider.SetPos(balance);
}

/////////////////////////////////////////////////////////////////////////////
// MusicPlayerX::OnNotifyStreamDuration
void 
MusicPlayerX::OnNotifyStreamDuration(unsigned long duration)
{
//    char caption[128];
//    unsigned long min,sec;
//    min = duration / 60;
//    sec = duration % 60;

//    sprintf(caption, "%d:%d", min,sec);
//    m_PlayerDlg->m_DurationLabel.SetWindowText(caption);
}

/////////////////////////////////////////////////////////////////////////////
// MusicPlayerX::OnNotifyStreamMimeType
void 
MusicPlayerX::OnNotifyStreamMimeType(const char *mime_type)
{
}

/////////////////////////////////////////////////////////////////////////////
// MusicPlayerX::OnNotifyStreamProperties
void 
MusicPlayerX::OnNotifyStreamProperties(XA_PropertyList *properties)
{
    // remove all property strings
//    m_PlayerDlg->m_StreamPropertyList.ResetContent();
//	m_PlayerDlg->m_Mylist.ResetContent();

    // add new properties
    for (unsigned int i=0; i<properties->nb_properties; i++) {
        char property[256];
//		char prop2[256];

        switch (properties->properties[i].type) {
          case XA_PROPERTY_TYPE_INTEGER:
            sprintf(property, "%s: %d", 
                    properties->properties[i].name,
                    properties->properties[i].value.integer);
            break;

          case XA_PROPERTY_TYPE_STRING:
            sprintf(property, "%s: %s", 
                    properties->properties[i].name,
                    properties->properties[i].value.string);
            break;

          default:
            continue;
        }
//        m_PlayerDlg->m_StreamPropertyList.AddString(property);
//		sprintf(prop2, "%d test", i);
//        m_PlayerDlg->m_Mylist.AddString(prop2);
//		m_PlayerDlg->m_Mylist.AddString(prop2);
    }
}

/////////////////////////////////////////////////////////////////////////////
// MusicPlayerX::OnNotifyStreamParameters
void 
MusicPlayerX::OnNotifyStreamParameters(XA_StreamParameters *parameters)
{
    char caption[128];

    sprintf(caption, "STREAM: %d kbps, %d Hz, %s", 
            parameters->bitrate,
            parameters->frequency,
            parameters->nb_channels == 1 ? "mono" : "stereo");
//    m_PlayerDlg->m_TypeLabel.SetWindowText(caption);
}

/////////////////////////////////////////////////////////////////////////////
// MusicPlayerX::OnNotifyCodecEqualizer
#ifdef asdf
void 
MusicPlayerX::OnNotifyCodecEqualizer(XA_EqualizerInfo *equalizer)
{
    if (EqualizerDialog == NULL) return;

    // if the equalizer if NULL, it means it's disabled
    if (equalizer == NULL) {
        EqualizerDialog->m_EnableEQCheckBox.SetCheck(FALSE);
    } else {
        EqualizerDialog->m_EnableEQCheckBox.SetCheck(TRUE);

        // set the slider positions
        EqualizerDialog->m_Slider1.SetPos(-equalizer->left[ 0]);
        EqualizerDialog->m_Slider2.SetPos(-equalizer->left[ 1]);
        EqualizerDialog->m_Slider3.SetPos(-equalizer->left[ 2]);
        EqualizerDialog->m_Slider4.SetPos(-equalizer->left[ 4]);
        EqualizerDialog->m_Slider5.SetPos(-equalizer->left[ 6]);
        EqualizerDialog->m_Slider6.SetPos(-equalizer->left[ 8]);
        EqualizerDialog->m_Slider7.SetPos(-equalizer->left[14]);
        EqualizerDialog->m_Slider8.SetPos(-equalizer->left[20]);
    }
}
#endif
/////////////////////////////////////////////////////////////////////////////
// MusicPlayerX::OnNotifyCodecModuleInfo
void 
MusicPlayerX::OnNotifyCodecModuleInfo(XA_ModuleInfo *info)
{
//    if (InfoDialog == NULL) return;
//    InfoDialog->m_CodecList.AddString(info->name);
}
