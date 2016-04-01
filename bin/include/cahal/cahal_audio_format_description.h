/*! \file   cahal_audio_format_description.h
    \brief  All constants, typedefs and functions related to audio formats and
            sample rates are defined in this file.
 
    \author Brent Carrara
 */
#ifndef __CAHAL_AUDIO_FORMAT_H__
#define __CAHAL_AUDIO_FORMAT_H__

#include <cpcommon.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define CAHAL_AUDIO_FORMAT_STRING_MACE6                 "MACE6"
#define CAHAL_AUDIO_FORMAT_STRING_QUALCOMM              "QUALCOMM"
#define CAHAL_AUDIO_FORMAT_STRING_MPEG4TWINVQ           "MPEG4TwinVQ"
#define CAHAL_AUDIO_FORMAT_STRING_MPEG4AAC_ELD_V2       "MPEG4AAC_ELD_V2"
#define CAHAL_AUDIO_FORMAT_STRING_MPEG4AAC_LD           "MPEG4AAC_LD"
#define CAHAL_AUDIO_FORMAT_STRING_PARAMETERVALUESTREAM	"ParameterValueStream"
#define CAHAL_AUDIO_FORMAT_STRING_MPEG4AAC_SPATIAL      "MPEG4AAC_Spatial"
#define CAHAL_AUDIO_FORMAT_STRING_MICROSOFTGSM          "MicrosoftGSM"
#define CAHAL_AUDIO_FORMAT_STRING_60958AC3              "60958AC3"
#define CAHAL_AUDIO_FORMAT_STRING_MPEG4CELP             "MPEG4CELP"
#define CAHAL_AUDIO_FORMAT_STRING_MPEG4HVXC             "MPEG4HVXC"
#define CAHAL_AUDIO_FORMAT_STRING_QDESIGN2              "QDesign2"
#define CAHAL_AUDIO_FORMAT_STRING_MPEGLAYER1            "MPEGLayer1"
#define CAHAL_AUDIO_FORMAT_STRING_ILBC                  "iLBC"
#define CAHAL_AUDIO_FORMAT_STRING_APPLELOSSLESS         "AppleLossless"
#define CAHAL_AUDIO_FORMAT_STRING_MPEG4AAC_ELD          "MPEG4AAC_ELD"
#define CAHAL_AUDIO_FORMAT_STRING_AES3                  "AES3"
#define CAHAL_AUDIO_FORMAT_STRING_ALAW                  "ALaw"
#define CAHAL_AUDIO_FORMAT_STRING_MPEG4AAC_HE           "MPEG4AAC_HE"
#define CAHAL_AUDIO_FORMAT_STRING_DVIINTELIMA           "DVIIntelIMA"
#define CAHAL_AUDIO_FORMAT_STRING_QDESIGN               "QDesign"
#define CAHAL_AUDIO_FORMAT_STRING_TIMECODE              "TimeCode"
#define CAHAL_AUDIO_FORMAT_STRING_AC3                   "AC3"
#define CAHAL_AUDIO_FORMAT_STRING_AUDIBLE               "Audible"
#define CAHAL_AUDIO_FORMAT_STRING_LINEARPCM             "LinearPCM"
#define CAHAL_AUDIO_FORMAT_STRING_MPEG4AAC_ELD_SBR      "MPEG4AAC_ELD_SBR"
#define CAHAL_AUDIO_FORMAT_STRING_ULAW                  "ULaw"
#define CAHAL_AUDIO_FORMAT_STRING_MPEG4AAC_HE_V2        "MPEG4AAC_HE_V2"
#define CAHAL_AUDIO_FORMAT_STRING_APPLEIMA4             "AppleIMA4"
#define CAHAL_AUDIO_FORMAT_STRING_MIDISTREAM            "MIDIStream"
#define CAHAL_AUDIO_FORMAT_STRING_AMR                   "AMR"
#define CAHAL_AUDIO_FORMAT_STRING_MPEG4AAC              "MPEG4AAC"
#define CAHAL_AUDIO_FORMAT_STRING_MPEGLAYER2            "MPEGLayer2"
#define CAHAL_AUDIO_FORMAT_STRING_MPEGLAYER3            "MPEGLayer3"
#define CAHAL_AUDIO_FORMAT_STRING_MACE3                 "MACE3"

/*! \var    cahal_audio_format_id
    \brief  Definition for the audio format identifiers.
 */
typedef UINT32 cahal_audio_format_id;

/*! \enum   cahal_audio_format_ids
    \brief  These are the recognized audio format constants in their string
            representation form. These are platform agnostic flags and are to be
            used whenever CAHAL functions are called.
 */
enum cahal_audio_format_ids
{
  CAHAL_AUDIO_FORMAT_LINEARPCM,
  CAHAL_AUDIO_FORMAT_AC3,
  CAHAL_AUDIO_FORMAT_60958AC3,
  CAHAL_AUDIO_FORMAT_APPLEIMA4,
  CAHAL_AUDIO_FORMAT_MPEG4AAC,
  CAHAL_AUDIO_FORMAT_MPEG4CELP,
  CAHAL_AUDIO_FORMAT_MPEG4HVXC,
  CAHAL_AUDIO_FORMAT_MPEG4TWINVQ,
  CAHAL_AUDIO_FORMAT_MACE3,
  CAHAL_AUDIO_FORMAT_MACE6,
  CAHAL_AUDIO_FORMAT_ULAW,
  CAHAL_AUDIO_FORMAT_ALAW,
  CAHAL_AUDIO_FORMAT_QDESIGN,
  CAHAL_AUDIO_FORMAT_QDESIGN2,
  CAHAL_AUDIO_FORMAT_QUALCOMM,
  CAHAL_AUDIO_FORMAT_MPEGLAYER1,
  CAHAL_AUDIO_FORMAT_MPEGLAYER2,
  CAHAL_AUDIO_FORMAT_MPEGLAYER3,
  CAHAL_AUDIO_FORMAT_TIMECODE,
  CAHAL_AUDIO_FORMAT_MIDISTREAM,
  CAHAL_AUDIO_FORMAT_PARAMETERVALUESTREAM,
  CAHAL_AUDIO_FORMAT_APPLELOSSLESS,
  CAHAL_AUDIO_FORMAT_MPEG4AAC_HE,
  CAHAL_AUDIO_FORMAT_MPEG4AAC_LD,
  CAHAL_AUDIO_FORMAT_MPEG4AAC_ELD,
  CAHAL_AUDIO_FORMAT_MPEG4AAC_ELD_SBR,
  CAHAL_AUDIO_FORMAT_MPEG4AAC_ELD_V2,
  CAHAL_AUDIO_FORMAT_MPEG4AAC_HE_V2,
  CAHAL_AUDIO_FORMAT_MPEG4AAC_SPATIAL,
  CAHAL_AUDIO_FORMAT_AMR,
  CAHAL_AUDIO_FORMAT_AUDIBLE,
  CAHAL_AUDIO_FORMAT_ILBC,
  CAHAL_AUDIO_FORMAT_DVIINTELIMA,
  CAHAL_AUDIO_FORMAT_MICROSOFTGSM,
  CAHAL_AUDIO_FORMAT_AES3,
  CAHAL_AUDIO_FORMAT_UNKNOWN,
};

/*! \var    cahal_sample_rate_range
    \brief  This struct is used to represent the sample rate ranges supported
            by various audio IO hardware devices. Sample rates are generally
            defined in the form of ranges. Note that for devices that only
            support a single sample rate minimum_rate will equal maximum_rate.
 */
typedef struct cahal_sample_rate_range_t
{
  /*! \var    minimum_rate
      \brief  The minimum rate supported by the device in the associated
              configuration.
   */
  FLOAT64     minimum_rate;
  
  /*! \var    maximum_rate
      \brief  The maximum rate supported by the device in the associated
              configuration.
   */
  FLOAT64     maximum_rate;
  
} cahal_sample_rate_range;

/*! \var    cahal_audio_format_description
    \brief  Struct used to model the configuration parameters for an audio
            stream, both input and output. This struct captures stream specific
            parameters that can be used when opening a stream for input/output.
            These parameters are supported by the corresponding audio device in
            hardware.
 */
typedef struct cahal_audio_format_description_t
{
  /*! \var    sample_rate_range
      \brief  The range of sample rates supported in this configuration.
   */
  cahal_sample_rate_range sample_rate_range;
  
  /*! \var    format_id
      \brief  Platform agnostic CAHAL constant defining an audio format
              supported by the corresponding audio device.
   */
  cahal_audio_format_id   format_id;
  
  /*! \var    number_of_channels
      \brief  The number of channels supported in this configuration by the
              corresponding audio device.
   */
  UINT32                  number_of_channels;
  
  /*! \var    bit_depth
      \brief  The number of bits used to quantize samples in this configuration.
   */
  UINT32                  bit_depth;
  
} cahal_audio_format_description;

/*! \fn      CHAR* cahal_convert_audio_format_id_to_cstring  (
              cahal_audio_format_id in_audio_format_id
            )
    \brief  Converts in_audio_format_id to its string representation and returns
            the newly created string. The caller does not need to free the
            returned string as the strings are defined constants.
 
    \param  in_audio_format_id  The audio format identifier whose string
                                representation is to be returned.
    \return A newly created string representation of the input audio format id.
 */
CHAR*
cahal_convert_audio_format_id_to_cstring  (
                                       cahal_audio_format_id in_audio_format_id
                                           );

/*! \fn     void cahal_print_audio_format_description  (
              cahal_audio_format_description* in_audio_format_description
            )
    \brief  Logs a string representation of in_audio_format_description by
            printing all of its properties. The string is logged using the
            logger.
 
    \param  in_audio_format_description The format description to be converted
                                        to a string and logged.
 */
void
cahal_print_audio_format_description  (
                     cahal_audio_format_description* in_audio_format_description
                                       );

/*! \fn     void cahal_print_audio_format_id (
              CHAR*                  in_label,
              cahal_audio_format_id  in_format_id
            )
    \brief  Logs a string representation of in_format_id using the logger. The
            log is logged at CPC_LOG_LEVEL_INFO and in_label is prepended to the
            string representation of in_format_id before logging.
 
    \param  in_label  The label to prepend to the string representation of
                      in_format_id.
    \param  in_format_id  The format identifier to be logged. This is a platform
                          agnostic CAHAL constant.
 */
void
cahal_print_audio_format_id (
                             CHAR*                  in_label,
                             cahal_audio_format_id  in_format_id
                             );

#ifdef __cplusplus
}
#endif

#endif  /*  __CAHAL_AUDIO_FORMAT_H__  */
