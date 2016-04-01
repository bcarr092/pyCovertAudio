import cahal_tests
import unittest
import string
import types
import struct
import platform
import re

recorded_samples  = []

global_input_device_name  = None                                                
global_output_device_name = None                                                
global_number_of_channels = 0                                                   
global_sample_rate        = 0                                                   
global_bit_depth          = 0                                                   
global_flags              = 0                                                   
                                                                                
def recorder( in_device, in_buffer, in_buffer_length ):                         
  global recorded_samples

  recorded_samples.append( in_buffer )                                          
                                                                                
def playback( in_device, in_buffer_length ):                                    
  global recorded_samples

  if( 0 < len( recorded_samples ) ):                                            
    out_buffer = recorded_samples[ 0 ]                                          
                                                                                
    del recorded_samples[ 0 ]                                                   
  else:                                                                         
    out_buffer = ''                                                             
                                                                                
  return( out_buffer )                                                          
                                                                                
def determine_platform():                                                       
  platform_info = platform.uname()                                              
                                                                                
  iphone_re = re.compile( "^iPhone", re.IGNORECASE )                            
                                                                                
  if( platform_info[ 0 ] == "Darwin" ):                                         
    if( iphone_re.search( platform_info[ 4 ] ) ):                               
      return( "iPhone" )                                                        
    else:                                                                       
      return( "Mac OSX" )                                                       
  elif( platform_info[ 0 ] == "Linux" ):                                        
    return( "Android" )                                                         
  elif( platform_info[ 0 ] == "Windows" ):
    return( "Windows" )
                                                                                
class TestsCAHALDevice( unittest.TestCase ):                                    
  def setUp( self ):                                                            
    global global_input_device_name                                             
    global global_output_device_name                                            
    global global_number_of_channels                                            
    global global_sample_rate                                                   
    global global_bit_depth                                                     
    global global_flags                                                         
                                                                                
    platform = determine_platform()                                             
                                                                                
    if( platform == "iPhone" ):                                                 
      global_input_device_name  = "MicrophoneBuiltIn"
      global_output_device_name = "Speaker"
      global_number_of_channels = 1
      global_sample_rate        = 44100
      global_bit_depth          = 16
      global_flags              = \
        cahal_tests.CAHAL_AUDIO_FORMAT_FLAGISSIGNEDINTEGER |  \
          cahal_tests.CAHAL_AUDIO_FORMAT_FLAGISPACKED                                                                                

    elif( platform == "Mac OSX" ):                                              
      global_input_device_name  = "Built-in Microphone"
      global_output_device_name = "Built-in Output"
      global_number_of_channels = 1
      global_sample_rate        = 44100
      global_bit_depth          = 16
      global_flags              = \
        cahal_tests.CAHAL_AUDIO_FORMAT_FLAGISSIGNEDINTEGER |  \
          cahal_tests.CAHAL_AUDIO_FORMAT_FLAGISPACKED
                                                                                
    elif( platform == "Android" ):                                              
      global_input_device_name  = "MicGeneric"
      global_output_device_name = "DefaultOutput"
      global_number_of_channels = 1
      global_sample_rate        = 48000
      global_bit_depth          = 16
      global_flags              = 0

    elif( platform == "Windows" ):                                              
      global_input_device_name  = "Microphone"
      global_output_device_name = "Speakers"
      global_number_of_channels = 2
      global_sample_rate        = 44100
      global_bit_depth          = 16
      global_flags              = 0
                                                                                
    else:                                                                       
      self.fail( "Unable to determine platform" )                               
                                                                                
  def test_cahal_playback_record( self ):                                       
    global global_input_device_name                                             
    global global_output_device_name                                            
    global global_number_of_channels                                            
    global global_sample_rate                                                   
    global global_bit_depth                                                     
    global global_flags                                                         
                                                                                
    device_list = cahal_tests.cahal_get_device_list()                           
    index       = 0                                                             
    device      = cahal_tests.cahal_device_list_get( device_list, index )       
                                                                                
    built_in_output_device  = None                                              
    built_in_input_device   = None                                              
                                                                                
    while( device ):                                                            
      if( device.device_name == global_input_device_name          \
          and cahal_tests.cahal_test_device_direction_support (   \
                device,                                           \
                cahal_tests.CAHAL_DEVICE_INPUT_STREAM             \
                                                              )   \
        ):                                                         
        built_in_input_device = device                             
                                                                                
      if( device.device_name == global_output_device_name         \
          and cahal_tests.cahal_test_device_direction_support (   \
                device,                                           \
                cahal_tests.CAHAL_DEVICE_OUTPUT_STREAM            \
                                                              )   \
        ):                                                         
        built_in_output_device = device                            
                                                                                
      index   += 1                                                              
      device  = cahal_tests.cahal_device_list_get( device_list, index )         
                                                                                
    self.assert_( built_in_input_device is not None )                           
    self.assert_( built_in_output_device is not None )                          

    self.assertTrue (                                     \
          cahal_tests.start_recording (                   \
            built_in_input_device,                        \
            cahal_tests.CAHAL_AUDIO_FORMAT_LINEARPCM,     \
            global_number_of_channels,                    \
            global_sample_rate,                           \
            global_bit_depth,                             \
            recorder,                                     \
            global_flags                                  \
                                      )                   \
                    )                                     
                                                                                
    cahal_tests.cahal_sleep( 5000 )                                                

    self.assertTrue( cahal_tests.cahal_stop_recording() )

    self.assertTrue (                                     \
          cahal_tests.start_playback  (                   \
            built_in_output_device,                       \
            cahal_tests.CAHAL_AUDIO_FORMAT_LINEARPCM,     \
            global_number_of_channels,                    \
            global_sample_rate,                           \
            global_bit_depth,                             \
            1.0,                                          \
            playback,                                     \
            global_flags                                  \
                                      )                   \
                    )
                                                                                
    cahal_tests.cahal_sleep( 5000 )

    self.assertTrue( cahal_tests.cahal_stop_playback() )

  def test_cahal_test_device_direction_support( self ):
    self.assertFalse(                                             \
      cahal_tests.cahal_test_device_direction_support( None, 0 )  \
                    )

    device_list = cahal_tests.cahal_get_device_list()
    index       = 0;                                                                    
    device      = cahal_tests.cahal_device_list_get( device_list, index )               
                                                                                
    while( device ):                                                                
      stream_index  = 0                                                             
      stream        =                               \
        cahal_tests.cahal_device_stream_list_get  ( \
          device.device_streams,                    \
          stream_index                              \
                                                  )
      self.assertFalse(                                               \
        cahal_tests.cahal_test_device_direction_support( device, 2 )  \
                      )

      self.assertFalse(                                                 \
        cahal_tests.cahal_test_device_direction_support( device, 20 )   \
                      )
                                                                               
      supports_input  = False
      supports_output = False

      while( stream ):                                                              
        if( cahal_tests.CAHAL_DEVICE_OUTPUT_STREAM == stream.direction ):
          self.assertTrue (                                   \
            cahal_tests.cahal_test_device_direction_support ( \
              device,                                         \
              cahal_tests.CAHAL_DEVICE_OUTPUT_STREAM          \
                                                            ) \
                          )

          supports_output = True

        if( cahal_tests.CAHAL_DEVICE_INPUT_STREAM == stream.direction ):
          self.assertTrue (                                   \
            cahal_tests.cahal_test_device_direction_support ( \
              device,                                         \
              cahal_tests.CAHAL_DEVICE_INPUT_STREAM           \
                                                            ) \
                          )

          supports_input = True

        stream_index += 1
        stream        =                               \
          cahal_tests.cahal_device_stream_list_get  ( \
            device.device_streams,                    \
            stream_index                              \
                                                    )

      if( not supports_input ):
        self.assertFalse (                                    \
          cahal_tests.cahal_test_device_direction_support (   \
            device,                                           \
            cahal_tests.CAHAL_DEVICE_INPUT_STREAM             \
                                                          )   \
                          )       

      if( not supports_output ):
        self.assertFalse  (                                   \
          cahal_tests.cahal_test_device_direction_support (   \
            device,                                           \
            cahal_tests.CAHAL_DEVICE_OUTPUT_STREAM            \
                                                          )   \
                          )

      index   += 1
      device  = cahal_tests.cahal_device_list_get( device_list, index )

  def test_get_device_list( self ):
    device_list = cahal_tests.cahal_get_device_list()

    self.assertNotEqual( device_list, None )

    index   = 0;                                                                    
    device  = cahal_tests.cahal_device_list_get( device_list, index )               
                                                                                
    while( device ):                                                                
      self.assertNotEqual( device, None )

      index += 1                                                                    

      device = cahal_tests.cahal_device_list_get( device_list, index )

  def test_print_cahal_device_list( self ):
    cahal_tests.cahal_print_device_list( None )

    device_list = cahal_tests.cahal_get_device_list()

    cahal_tests.cahal_print_device_list( device_list )

  def test_print_cahal_device( self ):
    cahal_tests.cahal_print_device( None )

    device_list = cahal_tests.cahal_get_device_list()
    index       = 0;                                                                    
    device      = cahal_tests.cahal_device_list_get( device_list, index )               
                                                                                
    while( device ):                                                                
      self.assertNotEqual( device, None )

      cahal_tests.cahal_print_device( device )

      index += 1                                                                    

      device = cahal_tests.cahal_device_list_get( device_list, index )

  def test_access_cahal_device_members( self ):
    device_list = cahal_tests.cahal_get_device_list()
    index       = 0;                                                                    
    device      = cahal_tests.cahal_device_list_get( device_list, index )               
                                                                                
    while( device ):                                                                
      self.assertNotEqual( device, None )

      device.handle
      device.device_name
      device.model                                               
      device.manufacturer                                 
      device.serial_number                                    
      device.version                                           
      device.device_uid                                     
      device.model_uid                                       
      device.preferred_sample_rate
      device.preferred_number_of_channels
      device.is_alive
      device.is_running
      device.supported_sample_rates
      device.device_streams

      sample_rate_index = 0
      sample_rate       =                               \
        cahal_tests.cahal_sample_rate_range_list_get  ( \
          device.supported_sample_rates,                \
          sample_rate_index                             \
                                                      )

      while( sample_rate ):
        sample_rate.minimum_rate
        sample_rate.maximum_rate

        sample_rate_index += 1

        sample_rate =                                     \
          cahal_tests.cahal_sample_rate_range_list_get  ( \
            device.supported_sample_rates,                \
            sample_rate_index                             \
                                                        )

      stream_index  = 0                                                             
      stream        =                               \
        cahal_tests.cahal_device_stream_list_get  ( \
          device.device_streams,                    \
          stream_index                              \
                                                  )
                                                                                
      while( stream ):                                                              
        stream.handle
        stream.direction
        stream.preferred_format
        stream.supported_formats

        format_description_index  = 0
        format_description        =                             \
          cahal_tests.cahal_audio_format_description_list_get ( \
            stream.supported_formats,                           \
            format_description_index                            \
                                                              )

        while( format_description ):
          format_description.format_id
          format_description.number_of_channels
          format_description.bit_depth
          format_description.sample_rate_range
          format_description.sample_rate_range.minimum_rate
          format_description.sample_rate_range.maximum_rate

          format_description_index  += 1 
          format_description        =                             \
            cahal_tests.cahal_audio_format_description_list_get ( \
              stream.supported_formats,                           \
              format_description_index                            \
                                                                )

        stream_index += 1
        stream        =                               \
          cahal_tests.cahal_device_stream_list_get  ( \
            device.device_streams,                    \
            stream_index                              \
                                                    )

      index += 1                                                                    

      device = cahal_tests.cahal_device_list_get( device_list, index )

if __name__ == '__main__':
  try:
    import threading as _threading
  except ImportError:
    import dummy_threading as _threading

  cahal_tests.cpc_log_set_log_level( cahal_tests.CPC_LOG_LEVEL_ERROR )

  cahal_tests.python_cahal_initialize()

  unittest.main()
  
  cahal_tests.cahal_terminate()
