import csignal_tests
import unittest
import string
import random
import types
import os
import tempfile
import re
import math
import struct

def touch_random_file():
  file_handle = tempfile.NamedTemporaryFile()

  file_name = file_handle.name

  return( file_handle, file_name )

class TestsCSignal( unittest.TestCase ):
  def test_get_group_delay( self ):
    delay = csignal_tests.python_filter_get_group_delay( None )

    self.assertEquals( None, delay )

    filter = csignal_tests.python_initialize_kaiser_lowpass_filter( 10000, 12000, 0.1, 80, 48000 )

    delay = csignal_tests.python_filter_get_group_delay( filter )

    self.assertNotEquals( None, filter )
    self.assertTrue( delay > 0 )

    self.assertEquals( csignal_tests.csignal_destroy_passband_filter( filter ), csignal_tests.CPC_ERROR_CODE_NO_ERROR )

    filter = csignal_tests.python_initialize_kaiser_filter( 8000, 10000, 12000, 14000, 0.1, 80, 48000 )

    delay = csignal_tests.python_filter_get_group_delay( filter )

    self.assertNotEquals( None, filter )
    self.assertTrue( delay > 0 )

    self.assertEquals( csignal_tests.csignal_destroy_passband_filter( filter ), csignal_tests.CPC_ERROR_CODE_NO_ERROR )

  def test_fft_of_lowpass_filter( self ):
    bits_per_symbol     = 1
    constellation_size  = 2 ** bits_per_symbol
    sample_rate         = 48000
    baseband_amplitude  = 32767
    carrier_frequency   = 21000
    symbol_duration     = 24000
    chip_duration       = 24 
    symbolExpansionFactor = 1
    separationInterval    = 1

    passband = sample_rate / chip_duration
    stopband = passband + 1000

    passband_attenuation = 0.1
    stopband_attenuation = 80

    inphase_gold_code = csignal_tests.python_initialize_gold_code( 7, 0x12000000, 0x1E000000, 0x12345678, 0x12345678 )
    quadrature_gold_code = csignal_tests.python_initialize_gold_code( 7, 0x12000000, 0x1E000000, 0x12345678, 0x12345678 )

    filter = csignal_tests.python_initialize_kaiser_lowpass_filter( passband, stopband, passband_attenuation, stopband_attenuation, sample_rate )

    data = ''.join( random.choice( string.ascii_lowercase ) for _ in range( 10 ) )
    
    symbol_tracker = csignal_tests.python_bit_stream_initialize( False, data )

    self.assertNotEquals( symbol_tracker, None )

    ( numberOfBits, buffer )  = csignal_tests.python_bit_stream_get_bits( symbol_tracker, bits_per_symbol ) 

    self.assertEquals( numberOfBits, bits_per_symbol )
    self.assertNotEquals( buffer, None )

    symbol = struct.unpack( "B", buffer )[ 0 ] >> ( 8 - bits_per_symbol )

    self.assertNotEquals( symbol, None )

    signal = []

    while( symbol != None ):
      signal_components = csignal_tests.python_csignal_modulate_BFSK_symbol (
        symbol,
        symbol_duration,
        sample_rate,
        carrier_frequency,
        separationInterval,
        symbolExpansionFactor
                                                              )

      self.assertNotEquals( signal_components, None )
      self.assertEquals( len( signal_components ), 2 )
      self.assertNotEquals( signal_components[ 0 ], None )
      self.assertNotEquals( signal_components[ 1 ], None )
      self.assertEquals( len( signal_components[ 0 ] ), symbol_duration )
      self.assertEquals( len( signal_components[ 1 ] ), symbol_duration )

      inphase_part = csignal_tests.python_spread_signal (
          inphase_gold_code,
          chip_duration,
          signal_components[ 0 ]
                                                        )

      self.assertNotEquals( inphase_part, None )

      quadrature_part = csignal_tests.python_spread_signal  (
          quadrature_gold_code,
          chip_duration,
          signal_components[ 1 ]
                                                            )

      self.assertNotEquals( quadrature_part, None )

      part = []

      for index in range( symbol_duration ):
        part.append( inphase_part[ index ] - quadrature_part[ index ] )

      self.assertNotEquals( part, None )
      self.assertEquals( len( part ), symbol_duration )

      signal = signal + part

      self.assertNotEquals( signal, None )

      ( numberOfBits, buffer )  = csignal_tests.python_bit_stream_get_bits( symbol_tracker, bits_per_symbol ) 

      if( 0 == numberOfBits ):
        symbol = None
      else:
        symbol = struct.unpack( "B", buffer )[ 0 ] >> ( 8 - bits_per_symbol )

    carrierSignal = []

    for i in range( len( signal ) ):
      inphase     = math.cos( 2.0 * math.pi * carrier_frequency * ( ( i * 1.0 ) / ( sample_rate * 1.0 ) ) )
      quadrature  = math.sin( 2.0 * math.pi * carrier_frequency * ( ( i * 1.0 ) / ( sample_rate * 1.0 ) ) )

      carrierSignal.append( inphase + quadrature )

    signal = csignal_tests.python_csignal_multiply_signals( signal, carrierSignal )
    signal = csignal_tests.python_filter_signal( filter, signal )

    self.assertNotEquals( signal, None )

    fft = csignal_tests.python_calculate_FFT( signal )

    self.assertNotEquals( fft, None )

    fft_mag = map( lambda x: abs( x ), fft )

    N = len( fft_mag )
    delta = 1.0 / sample_rate

    max_value = -1

    for magnitude in fft_mag:
      if( magnitude > max_value ):
        max_value = magnitude
    
    fft_mag = map( lambda x: 10**-12 if x == 0 else x, fft_mag )
    fft_mag = map( lambda x: 10 * math.log10( x / max_value ), fft_mag )

    for index in range( len( fft_mag ) ):
      if( index > ( N / 2 ) ):
        n = index - N
      else:
        n = index

      frequency = n / ( delta * N )

      if( abs( frequency ) > stopband ):
        self.assertTrue( fft_mag[ index ] < -1.0 * ( ( stopband_attenuation * 0.90 ) / 2 ) )

    ( file_handle, file_name ) = touch_random_file()

    file_handle.close()

    maxValue = -1 

    for sample in signal:
      if( abs( sample ) > maxValue ):
        maxValue = abs( sample )

    signal = map( lambda x: x / maxValue, signal )

    samples = [ signal ]

    error = csignal_tests.python_write_FLOAT_wav (
      file_name,
      len( samples ), 
      sample_rate,
      len( signal ),
      samples
                                                )

    self.assertEquals( error, csignal_tests.CPC_TRUE )

    if( os.path.exists( file_name ) ):
      os.unlink( file_name )

    self.assertEquals( csignal_tests.bit_stream_destroy( symbol_tracker ), csignal_tests.CPC_ERROR_CODE_NO_ERROR )

    self.assertEquals( csignal_tests.csignal_destroy_passband_filter( filter ), csignal_tests.CPC_ERROR_CODE_NO_ERROR )

    self.assertEquals( csignal_tests.csignal_destroy_gold_code( inphase_gold_code ), csignal_tests.CPC_ERROR_CODE_NO_ERROR )

    self.assertEquals( csignal_tests.csignal_destroy_gold_code( quadrature_gold_code ), csignal_tests.CPC_ERROR_CODE_NO_ERROR )

  def test_fft_of_spread_signal( self ):
    bits_per_symbol     = 1
    constellation_size  = 2 ** bits_per_symbol
    sample_rate         = 48000
    baseband_amplitude  = 32767
    carrier_frequency   = 21000
    symbol_duration     = 24000
    chip_duration       = 24 
    symbolExpansionFactor = 1
    separationInterval    = 1

    first_stopband  = 19000
    first_passband  = 20000
    second_passband = 22000
    second_stopband = 23000

    passband_attenuation = 0.1
    stopband_attenuation = 80

    inphase_gold_code = csignal_tests.python_initialize_gold_code( 7, 0x12000000, 0x1E000000, 0x12345678, 0x12345678 )
    quadrature_gold_code = csignal_tests.python_initialize_gold_code( 7, 0x12000000, 0x1E000000, 0x12345678, 0x12345678 )

    filter    = csignal_tests.python_initialize_kaiser_filter( first_stopband, first_passband, second_passband, second_stopband, passband_attenuation, stopband_attenuation, sample_rate )

    data = ''.join( random.choice( string.ascii_lowercase ) for _ in range( 10 ) )
    
    symbol_tracker = csignal_tests.python_bit_stream_initialize( False, data )

    self.assertNotEquals( symbol_tracker, None )

    ( numberOfBits, buffer )  = csignal_tests.python_bit_stream_get_bits( symbol_tracker, bits_per_symbol ) 

    self.assertEquals( numberOfBits, bits_per_symbol )
    self.assertNotEquals( buffer, None )

    symbol = struct.unpack( "B", buffer )[ 0 ] >> ( 8 - bits_per_symbol )

    self.assertNotEquals( symbol, None )

    signal = []

    while( symbol != None ):
      signal_components = csignal_tests.python_csignal_modulate_BFSK_symbol (
        symbol,
        symbol_duration,
        sample_rate,
        carrier_frequency,
        separationInterval,
        symbolExpansionFactor
                                                              )

      self.assertNotEquals( signal_components, None )
      self.assertEquals( len( signal_components ), 2 )
      self.assertNotEquals( signal_components[ 0 ], None )
      self.assertNotEquals( signal_components[ 1 ], None )
      self.assertEquals( len( signal_components[ 0 ] ), symbol_duration )
      self.assertEquals( len( signal_components[ 1 ] ), symbol_duration )

      inphase_part = csignal_tests.python_spread_signal (
          inphase_gold_code,
          chip_duration,
          signal_components[ 0 ]
                                                        )

      self.assertNotEquals( inphase_part, None )

      quadrature_part = csignal_tests.python_spread_signal  (
          quadrature_gold_code,
          chip_duration,
          signal_components[ 1 ]
                                                            )

      self.assertNotEquals( quadrature_part, None )

      part = []

      for index in range( symbol_duration ):
        part.append( inphase_part[ index ] - quadrature_part[ index ] )

      self.assertNotEquals( part, None )
      self.assertEquals( len( part ), symbol_duration )

      signal = signal + part

      self.assertNotEquals( signal, None )

      ( numberOfBits, buffer )  = csignal_tests.python_bit_stream_get_bits( symbol_tracker, bits_per_symbol ) 

      if( 0 == numberOfBits ):
        symbol = None
      else:
        symbol = struct.unpack( "B", buffer )[ 0 ] >> ( 8 - bits_per_symbol )

    signal = csignal_tests.python_filter_signal( filter, signal )

    self.assertNotEquals( signal, None )

    fft = csignal_tests.python_calculate_FFT( signal )

    self.assertNotEquals( fft, None )

    fft_mag = map( lambda x: abs( x ), fft )

    N = len( fft_mag )
    delta = 1.0 / sample_rate

    max_value = -1

    for magnitude in fft_mag:
      if( magnitude > max_value ):
        max_value = magnitude
    
    fft_mag = map( lambda x: 10**-12 if x == 0 else x, fft_mag )
    fft_mag = map( lambda x: 10 * math.log10( x / max_value ), fft_mag )

    for index in range( len( fft_mag ) ):
      if( index > ( N / 2 ) ):
        n = index - N
      else:
        n = index

      frequency = n / ( delta * N )

      if( abs( frequency ) < first_stopband or abs( frequency ) > second_stopband ):
        self.assertTrue( fft_mag[ index ] < -1.0 * ( ( stopband_attenuation * 0.90 ) / 2 ) )

    samples = [ signal ]

    ( file_handle, file_name ) = touch_random_file()

    file_handle.close()

    error = csignal_tests.python_write_FLOAT_wav (
      file_name,
      len( samples ), 
      sample_rate,
      len( signal ),
      samples
                                                )

    self.assertEquals( error, csignal_tests.CPC_TRUE )

    if( os.path.exists( file_name ) ):
      os.unlink( file_name )

    self.assertEquals( csignal_tests.bit_stream_destroy( symbol_tracker ), csignal_tests.CPC_ERROR_CODE_NO_ERROR )

    self.assertEquals( csignal_tests.csignal_destroy_passband_filter( filter ), csignal_tests.CPC_ERROR_CODE_NO_ERROR )

    self.assertEquals( csignal_tests.csignal_destroy_gold_code( inphase_gold_code ), csignal_tests.CPC_ERROR_CODE_NO_ERROR )

    self.assertEquals( csignal_tests.csignal_destroy_gold_code( quadrature_gold_code ), csignal_tests.CPC_ERROR_CODE_NO_ERROR )

  def test_filter( self ):
    sample_rate     = 48000

    first_stopband  = 19000
    first_passband  = 20000
    second_passband = 22000
    second_stopband = 23000

    passband_attenuation = 0.1
    stopband_attenuation = 80

    signal = []

    for i in range( 100 ):
      part = []

      for j in range( 200 ):
        part.append( 32767 * random.normalvariate( 0, 1 ) )

      signal = signal + part

      self.assertNotEquals( signal, None )

    filter = csignal_tests.python_initialize_kaiser_filter( first_stopband, first_passband, second_passband, second_stopband, passband_attenuation, stopband_attenuation, sample_rate )

    signal = csignal_tests.python_filter_signal( filter, signal )

    self.assertNotEquals( signal, None )

    signal_max = -1;

    for sample in signal:
      if( abs( sample ) > signal_max ):
        signal_max = abs( sample )

    normalized_signal = map( lambda x: x / signal_max, signal )

    fft = csignal_tests.python_calculate_FFT( signal )

    self.assertNotEquals( fft, None )

    fft_mag = map( lambda x: abs( x ), fft )

    N = len( fft_mag )
    delta = 1.0 / sample_rate

    max_value = -1

    for magnitude in fft_mag:
      if( magnitude > max_value ):
        max_value = magnitude

    fft_mag = map( lambda x: 10**-12 if x == 0 else x, fft_mag )
    fft_mag = map( lambda x: 10 * math.log10( x / max_value ), fft_mag )

    for index in range( len( fft_mag ) ):
      if( index > ( N / 2 ) ):
        n = index - N
      else:
        n = index

      frequency = n / ( delta * N )

      if( abs( frequency ) < first_stopband or abs( frequency ) > second_stopband ):
        self.assertTrue( fft_mag[ index ] < -1.0 * ( ( stopband_attenuation * 0.90 ) / 2 ) )

    self.assertEquals( csignal_tests.csignal_destroy_passband_filter( filter ), csignal_tests.CPC_ERROR_CODE_NO_ERROR )

  def test_filter_signal( self ): 
    bits_per_symbol     = 1
    constellation_size  = 2 ** bits_per_symbol
    sample_rate         = 48000
    baseband_amplitude  = 32767 
    carrier_frequency   = 12000
    symbol_duration     = 1000
    chip_duration       = 10
    symbolExpansionFactor = 1
    separationInterval    = 1

    first_stopband  = 10000
    first_passband  = 11000
    second_passband = 13000
    second_stopband = 14000

    passband_attenuation = 0.1
    stopband_attenuation = 80

    ( file_handle, file_name ) = touch_random_file()

    file_handle.close()

    filter = csignal_tests.python_initialize_kaiser_filter( first_stopband, first_passband, second_passband, second_stopband, passband_attenuation, stopband_attenuation, sample_rate )

    self.assertNotEquals( filter, None )

    data = ''.join( random.choice( string.ascii_lowercase ) for _ in range( 10 ) )
    
    symbol_tracker = csignal_tests.python_bit_stream_initialize( False, data )

    self.assertNotEqual( symbol_tracker, None )

    ( numberOfBits, buffer )  = csignal_tests.python_bit_stream_get_bits( symbol_tracker, bits_per_symbol ) 

    self.assertEquals( numberOfBits, bits_per_symbol )
    self.assertNotEquals( buffer, None )

    symbol = struct.unpack( "B", buffer )[ 0 ] >> ( 8 - bits_per_symbol )

    self.assertNotEquals( symbol, None )

    inphase_gold_code = csignal_tests.python_initialize_gold_code( 7, 0x12000000, 0x1E000000, 0x40000000, 0x40000000 )
    quadrature_gold_code = csignal_tests.python_initialize_gold_code( 7, 0x12000000, 0x1E000000, 0x40000000, 0x40000000 )

    self.assertNotEquals( inphase_gold_code, None )
    self.assertNotEquals( quadrature_gold_code, None )

    signal = []

    while( symbol != None ):
      signal_components = csignal_tests.python_csignal_modulate_BFSK_symbol (
        symbol,
        symbol_duration,
        sample_rate,
        carrier_frequency,
        separationInterval,
        symbolExpansionFactor
                                                              )
  
      self.assertNotEquals( signal_components, None )
      self.assertEquals( len( signal_components ), 2 )
      self.assertNotEquals( signal_components[ 0 ], None )
      self.assertNotEquals( signal_components[ 1 ], None )
      self.assertEquals( len( signal_components[ 0 ] ), symbol_duration )
      self.assertEquals( len( signal_components[ 1 ] ), symbol_duration )

      inphase_part = csignal_tests.python_spread_signal (
          inphase_gold_code,
          chip_duration,
          signal_components[ 0 ]
                                                        )

      self.assertNotEquals( inphase_part, None )

      quadrature_part = csignal_tests.python_spread_signal  (
          quadrature_gold_code,
          chip_duration,
          signal_components[ 1 ]
                                                            )

      self.assertNotEquals( quadrature_part, None )

      part = []

      for index in range( symbol_duration ):
        sampleValue = inphase_part[ index ] - quadrature_part[ index ]

        self.assertTrue( abs( sampleValue ) <= baseband_amplitude )

        part.append( sampleValue )

      self.assertNotEquals( part, None )
      self.assertEquals( len( part ), symbol_duration )

      signal = signal + part

      self.assertNotEquals( signal, None )

      ( numberOfBits, buffer )  = csignal_tests.python_bit_stream_get_bits( symbol_tracker, bits_per_symbol ) 

      if( 0 == numberOfBits ):
        symbol = None
      else:
        symbol = struct.unpack( "B", buffer )[ 0 ] >> ( 8 - bits_per_symbol )

    self.assertNotEquals( signal, None )

    self.assertNotEqual( len( signal ), None )

    signal = csignal_tests.python_filter_signal( filter, signal )

    for index in range( len( signal ) ):
      signal[ index ] /= baseband_amplitude

    self.assertNotEquals( signal, None )

    self.assertNotEquals( len( signal ), 0 )

    samples = [ signal, signal ]

    error = csignal_tests.python_write_FLOAT_wav (
      file_name,
      len( samples ),
      sample_rate,
      len( signal ),
      samples
                                                )

    self.assertEquals( error, csignal_tests.CPC_TRUE )

    self.assertEquals( csignal_tests.csignal_destroy_passband_filter( filter ), csignal_tests.CPC_ERROR_CODE_NO_ERROR )

    self.assertEquals( csignal_tests.bit_stream_destroy( symbol_tracker ), csignal_tests.CPC_ERROR_CODE_NO_ERROR )

    self.assertEquals( csignal_tests.csignal_destroy_gold_code( inphase_gold_code ), csignal_tests.CPC_ERROR_CODE_NO_ERROR )

    self.assertEquals( csignal_tests.csignal_destroy_gold_code( quadrature_gold_code ), csignal_tests.CPC_ERROR_CODE_NO_ERROR )

    if( os.path.exists( file_name ) ):
      os.unlink( file_name )

  def test_write_wav_basic( self ):
    bits_per_symbol     = 1
    constellation_size  = 2 ** bits_per_symbol
    sample_rate         = 48000
    baseband_amplitude  = 32000
    carrier_frequency   = 2000
    symbolExpansionFactor = 1
    separationInterval    = 1

    data = '\x12'
    
    symbol_tracker = csignal_tests.python_bit_stream_initialize( False, data )

    self.assertNotEquals( symbol_tracker, None )

    ( numberOfBits, buffer )  = csignal_tests.python_bit_stream_get_bits( symbol_tracker, bits_per_symbol ) 

    self.assertEquals( numberOfBits, bits_per_symbol )
    self.assertNotEquals( buffer, None )

    symbol = struct.unpack( "B", buffer )[ 0 ] >> ( 8 - bits_per_symbol )

    self.assertNotEquals( symbol, None )

    signal_components = csignal_tests.python_csignal_modulate_BFSK_symbol (
      symbol,
      sample_rate,
      sample_rate,
      carrier_frequency,
      separationInterval,
      symbolExpansionFactor
                                                            )

    self.assertNotEquals( signal_components, None )
    self.assertEquals( len( signal_components ), 2 )
    self.assertNotEquals( signal_components[ 0 ], None )
    self.assertNotEquals( signal_components[ 1 ], None )
    self.assertEquals( len( signal_components[ 0 ] ), sample_rate )
    self.assertEquals( len( signal_components[ 1 ] ), sample_rate )

    signal = []

    for index in range( sample_rate ):
      sampleValue = signal_components[ 0 ][ index ] - signal_components[ 1 ][ index ]

      self.assertTrue( abs( sampleValue ) <= 1.0 )

      signal.append( sampleValue )

    samples = [ signal, signal ]

    ( file_handle, file_name ) = touch_random_file()
    
    file_handle.close()

    error = csignal_tests.python_write_FLOAT_wav (
      file_name,
      len( samples ),
      sample_rate,
      len( signal ),
      samples
                                                )

    self.assertEquals( error, csignal_tests.CPC_TRUE )

    if( os.path.exists( file_name ) ):
      os.unlink( file_name )

    self.assertEquals( csignal_tests.bit_stream_destroy( symbol_tracker ), csignal_tests.CPC_ERROR_CODE_NO_ERROR )

  def test_spread_signal( self ):
    bits_per_symbol     = 1
    constellation_size  = 2 ** bits_per_symbol
    sample_rate         = 48000
    baseband_amplitude  = 32000
    carrier_frequency   = 12000
    symbol_duration     = 1000
    chip_duration       = 10
    symbolExpansionFactor = 1
    separationInterval    = 1

    ( file_handle, file_name ) = touch_random_file()

    file_handle.close()

    data = ''.join( random.choice( string.ascii_lowercase ) for _ in range( 10 ) )
   
    symbol_tracker = csignal_tests.python_bit_stream_initialize( False, data )

    self.assertNotEquals( symbol_tracker, None )

    ( numberOfBits, buffer )  = csignal_tests.python_bit_stream_get_bits( symbol_tracker, bits_per_symbol ) 

    self.assertEquals( numberOfBits, bits_per_symbol )
    self.assertNotEquals( buffer, None )

    symbol = struct.unpack( "B", buffer )[ 0 ] >> ( 8 - bits_per_symbol )

    self.assertNotEquals( symbol, None )

    inphase_gold_code = csignal_tests.python_initialize_gold_code( 7, 0x12000000, 0x1E000000, 0x40000000, 0x40000000 )
    quadrature_gold_code = csignal_tests.python_initialize_gold_code( 7, 0x12000000, 0x1E000000, 0x40000000, 0x40000000 )

    self.assertNotEquals( inphase_gold_code, None )
    self.assertNotEquals( quadrature_gold_code, None )

    signal = []

    while( symbol != None ):
      signal_components = csignal_tests.python_csignal_modulate_BFSK_symbol (
        symbol,
        symbol_duration,
        sample_rate,
        carrier_frequency,
        separationInterval,
        symbolExpansionFactor
                                                              )

      self.assertNotEquals( signal_components, None )
      self.assertEquals( len( signal_components ), 2 )
      self.assertNotEquals( signal_components[ 0 ], None )
      self.assertNotEquals( signal_components[ 1 ], None )
      self.assertEquals( len( signal_components[ 0 ] ), symbol_duration )
      self.assertEquals( len( signal_components[ 1 ] ), symbol_duration )

      inphase_part = csignal_tests.python_spread_signal (
          inphase_gold_code,
          chip_duration,
          signal_components[ 0 ]
                                                        )

      self.assertNotEquals( inphase_part, None )

      quadrature_part = csignal_tests.python_spread_signal  (
          quadrature_gold_code,
          chip_duration,
          signal_components[ 1 ]
                                                            )

      self.assertNotEquals( quadrature_part, None )

      part = []

      for index in range( symbol_duration ):
        part.append( inphase_part[ index ] - quadrature_part[ index ] )

      self.assertEquals( len( part ), symbol_duration )

      signal = signal + part

      self.assertNotEquals( signal, None )

      ( numberOfBits, buffer )= csignal_tests.python_bit_stream_get_bits( symbol_tracker, bits_per_symbol ) 

      if( 0 == numberOfBits ):
        symbol = None
      else:
        symbol = struct.unpack( "B", buffer )[ 0 ] >> ( 8 - bits_per_symbol )

    self.assertNotEquals( signal, None )

    samples = [ signal, signal ]

    error = csignal_tests.python_write_FLOAT_wav (
      file_name,
      len( samples ),
      sample_rate,
      len( signal ),
      samples
                                                )

    self.assertEquals( error, csignal_tests.CPC_TRUE )

    if( os.path.exists( file_name ) ): 
      os.unlink( file_name )

    self.assertEquals( csignal_tests.bit_stream_destroy( symbol_tracker ), csignal_tests.CPC_ERROR_CODE_NO_ERROR )

    self.assertEquals( csignal_tests.csignal_destroy_gold_code( inphase_gold_code ), csignal_tests.CPC_ERROR_CODE_NO_ERROR )

    self.assertEquals( csignal_tests.csignal_destroy_gold_code( quadrature_gold_code ), csignal_tests.CPC_ERROR_CODE_NO_ERROR )

  def test_basic_fft( self ):
    signal = []

    for i in range( 256 ):
      signal.append( 32767 * random.normalvariate( 0, 1 ) )

    self.assertNotEquals( signal, None )

    fft = csignal_tests.python_calculate_FFT( signal )

    self.assertNotEquals( fft, None )
    self.assertEquals( len( fft ), 256 )

    signal.append( 0.0 )

    fft = csignal_tests.python_calculate_FFT( signal )

    self.assertNotEquals( fft, None )
    self.assertEquals( len( fft ), 512 )

  def test_initialize_kaiser_filter( self ):
    filter = csignal_tests.python_initialize_kaiser_filter( 3000, 4000, 6000, 5000, 0.1, 80, 0 )

    self.assertEquals( filter, None )

    filter = csignal_tests.python_initialize_kaiser_filter( 3000, 4000, 6000, 5000, 0.1, 80, 20000 )

    self.assertEquals( filter, None )

    filter = csignal_tests.python_initialize_kaiser_filter( 3000, 4000, 6000, 6000, 0.1, 80, 20000 )

    self.assertEquals( filter, None )

    filter = csignal_tests.python_initialize_kaiser_filter( 3000, 4000, 9000, 8000, 0.1, 80, 20000 )

    self.assertEquals( filter, None )

    filter = csignal_tests.python_initialize_kaiser_filter( 3000, 4000, 8000, 8000, 0.1, 80, 20000 )

    self.assertEquals( filter, None )

    filter = csignal_tests.python_initialize_kaiser_filter( 3000, 2000, 6000, 8000, 0.1, 80, 20000 )

    self.assertEquals( filter, None )

    filter = csignal_tests.python_initialize_kaiser_filter( 3000, 3000, 6000, 8000, 0.1, 80, 20000 )

    self.assertEquals( filter, None )

    filter = csignal_tests.python_initialize_kaiser_filter( 5000, 4000, 6000, 8000, 0.1, 80, 20000 )

    self.assertEquals( filter, None )

    filter = csignal_tests.python_initialize_kaiser_filter( 4000, 4000, 6000, 8000, 0.1, 80, 20000 )

    self.assertEquals( filter, None )

    filter = csignal_tests.python_initialize_kaiser_filter( 3000, 4000, 6000, 8000, 0.1, 80, 20000 )

    self.assertNotEquals( filter, None )

    csignal_tests.csignal_destroy_passband_filter( filter )

  def test_get_gold_code_degree_7( self ):
    gold_code = csignal_tests.python_initialize_gold_code( 7, 0x12000000, 0x1E000000, 0x40000000, 0x40000000 )

    self.assertNotEquals( gold_code, None )

    codes = csignal_tests.python_get_gold_code( gold_code, 80000 )

    self.assertNotEquals( codes, None )
    self.assertEquals( len( codes ), 10000 )

    index = 0

    file = open( 'GoldSequenceTestVector_7_89_8F_20_20.dat' )

    if( file ):
      for line in file:
        expression = re.compile( '^(\d+)$' )
  
        result = re.match( expression, line )

        if( result ):
          self.assertEquals( ord( codes[ index ] ), int( result.group( 1 ) ) )

          index += 1

      file.close() 

    result = csignal_tests.csignal_destroy_gold_code( gold_code )

    self.assertEquals( result, csignal_tests.CPC_ERROR_CODE_NO_ERROR )

  def test_get_gold_code_size( self ):
    gold_code = csignal_tests.python_initialize_gold_code( 2, 0xC0000000, 0xC0000000, 0x40000000, 0x80000000 )

    self.assertNotEquals( gold_code, None )

    codes = csignal_tests.python_get_gold_code( gold_code, 0 )

    self.assertEquals( codes, None )

    codes = csignal_tests.python_get_gold_code( gold_code, 8 )

    self.assertNotEquals( codes, None )
    self.assertEquals( len( codes ), 1 )

    codes = csignal_tests.python_get_gold_code( gold_code, 16 )

    self.assertNotEquals( codes, None )
    self.assertEquals( len( codes ), 2 )

    result = csignal_tests.csignal_destroy_gold_code( gold_code )

    self.assertEquals( result, csignal_tests.CPC_ERROR_CODE_NO_ERROR )

    gold_code = csignal_tests.python_initialize_gold_code( 7, 0x12000000, 0x1E000000, 0x40000000, 0x40000000 )

    self.assertNotEquals( gold_code, None )

    codes = csignal_tests.python_get_gold_code( gold_code, 0 )

    self.assertEquals( codes, None )

    codes = csignal_tests.python_get_gold_code( gold_code, 8 )

    self.assertNotEquals( codes, None )
    self.assertEquals( len( codes ), 1 )

    codes = csignal_tests.python_get_gold_code( gold_code, 16 )

    self.assertNotEquals( codes, None )
    self.assertEquals( len( codes ), 2 )

    result = csignal_tests.csignal_destroy_gold_code( gold_code )

    self.assertEquals( result, csignal_tests.CPC_ERROR_CODE_NO_ERROR )

  def test_initialize_gold_code( self ):
    gold_code = csignal_tests.python_initialize_gold_code( 1, 0x10000000, 0x10000000, 0x10000000, 0x10000000 )

    self.assertEquals( gold_code, None )

    gold_code = csignal_tests.python_initialize_gold_code( 33, 0x10000000, 0x10000000, 0x10000000, 0x10000000 )

    self.assertEquals( gold_code, None )

    gold_code = csignal_tests.python_initialize_gold_code( 2, 0x80000000, 0x10000000, 0xC0000000, 0x10000000 )

    self.assertEquals( gold_code, None )

    gold_code = csignal_tests.python_initialize_gold_code( 2, 0xC0000000, 0x10000000, 0x80000000, 0x10000000 )

    self.assertEquals( gold_code, None )

    gold_code = csignal_tests.python_initialize_gold_code( 2, 0xE0000000, 0x10000000, 0xC0000000, 0x10000000 )

    self.assertEquals( gold_code, None )

    gold_code = csignal_tests.python_initialize_gold_code( 2, 0xC0000000, 0x10000000, 0xE0000000, 0x10000000 )

    self.assertEquals( gold_code, None )

    gold_code = csignal_tests.python_initialize_gold_code( 2, 0xC0000000, 0x40000000, 0xC0000000, 0x40000000 )

    self.assertNotEquals( gold_code, None )

    result = csignal_tests.csignal_destroy_gold_code( gold_code )

    self.assertEquals( result, csignal_tests.CPC_ERROR_CODE_NO_ERROR )

  def test_get_spreading_code_degree_32( self ):
    spreading_code = csignal_tests.python_initialize_spreading_code( 32, 0x00200007, 0x40000000 )

    self.assertNotEquals( spreading_code, None )

    codes = csignal_tests.python_get_spreading_code( spreading_code, 80000 )

    self.assertNotEquals( codes, None )
    self.assertEquals( len( codes ), 10000 )

    index = 0

    file = open( 'PNSequenceTestVector_32_100200007_40000000.dat' )

    if( file ):
      for line in file:
        expression = re.compile( '^(\d+)$' )

        result = re.match( expression, line )

        if( result ):
          self.assertEquals( codes[ index ], int( result.group( 1 ) ) )

          index += 1

      file.close()

    self.assertEquals( csignal_tests.csignal_destroy_spreading_code( spreading_code ), csignal_tests.CPC_ERROR_CODE_NO_ERROR )

  def test_get_spreading_code_degree_7( self ):
    spreading_code = csignal_tests.python_initialize_spreading_code( 7, 0x12000000, 0x40000000 )

    self.assertNotEquals( spreading_code, None )

    codes = csignal_tests.python_get_spreading_code( spreading_code, 80000 )

    self.assertNotEquals( codes, None )
    self.assertEquals( len( codes ), 10000 )

    index = 0

    file = open( 'PNSequenceTestVector_7_89_20.dat' )

    if( file ):
      for line in file:
        expression = re.compile( '^(\d+)$' )

        result = re.match( expression, line )

        if( result ):
          self.assertEquals( codes[ index ], int( result.group( 1 ) ) )

          index += 1

      file.close()

    self.assertEquals( csignal_tests.csignal_destroy_spreading_code( spreading_code ), csignal_tests.CPC_ERROR_CODE_NO_ERROR )

  def test_get_spreading_code_degree_2( self ):
    spreading_code = csignal_tests.python_initialize_spreading_code( 2, 0xC0000000, 0x40000000 )

    self.assertNotEquals( spreading_code, None )

    codes = csignal_tests.python_get_spreading_code( spreading_code, 80000 )

    self.assertNotEquals( codes, None )
    self.assertEquals( len( codes ), 10000 )

    index = 0

    file = open( 'PNSequenceTestVector_2_7_1.dat' )

    if( file ):
      for line in file:
        expression = re.compile( '^(\d+)$' )

        result = re.match( expression, line )

        if( result ):
          self.assertEquals( codes[ index ], int( result.group( 1 ) ) )

          index += 1

      file.close()

    self.assertEquals( csignal_tests.csignal_destroy_spreading_code( spreading_code ), csignal_tests.CPC_ERROR_CODE_NO_ERROR )

  def test_initialize_spreading_code( self ):
    spreading_code = csignal_tests.python_initialize_spreading_code( 1, 0x10000000, 0x10000000 )

    self.assertEquals( spreading_code, None )

    spreading_code = csignal_tests.python_initialize_spreading_code( 33, 0x10000000, 0x10000000 )

    self.assertEquals( spreading_code, None )

    spreading_code = csignal_tests.python_initialize_spreading_code( 2, 0x80000000, 0x10000000 )

    self.assertEquals( spreading_code, None )

    spreading_code = csignal_tests.python_initialize_spreading_code( 2, 0xE0000000, 0x10000000 )

    self.assertEquals( spreading_code, None )

    spreading_code = csignal_tests.python_initialize_spreading_code( 2, 0xC0000000, 0x40000000 )

    self.assertNotEquals( spreading_code, None )

    self.assertEquals( csignal_tests.csignal_destroy_spreading_code( spreading_code ), csignal_tests.CPC_ERROR_CODE_NO_ERROR )

  def test_get_spreading_code_size( self ):
    spreading_code = csignal_tests.python_initialize_spreading_code( 2, 0xC0000000, 0x40000000 )

    self.assertNotEquals( spreading_code, None )

    codes = csignal_tests.python_get_spreading_code( spreading_code, 0 )

    self.assertEquals( codes, None )

    codes = csignal_tests.python_get_spreading_code( spreading_code, 8 )

    self.assertNotEquals( codes, None )
    self.assertEquals( len( codes ), 1 )

    codes = csignal_tests.python_get_spreading_code( spreading_code, 16 )

    self.assertNotEquals( codes, None )
    self.assertEquals( len( codes ), 2 )

    self.assertEquals( csignal_tests.csignal_destroy_spreading_code( spreading_code ), csignal_tests.CPC_ERROR_CODE_NO_ERROR )

    spreading_code = csignal_tests.python_initialize_spreading_code( 7, 0x12000000, 0x40000000 )

    self.assertNotEquals( spreading_code, None )

    codes = csignal_tests.python_get_spreading_code( spreading_code, 0 )

    self.assertEquals( codes, None )

    codes = csignal_tests.python_get_spreading_code( spreading_code, 8 )

    self.assertNotEquals( codes, None )
    self.assertEquals( len( codes ), 1 )

    codes = csignal_tests.python_get_spreading_code( spreading_code, 16 )

    self.assertNotEquals( codes, None )
    self.assertEquals( len( codes ), 2 )

    self.assertEquals( csignal_tests.csignal_destroy_spreading_code( spreading_code ), csignal_tests.CPC_ERROR_CODE_NO_ERROR )

    spreading_code = csignal_tests.python_initialize_spreading_code( 32, 0x00200007, 0x40000000 )

    self.assertNotEquals( spreading_code, None )

    codes = csignal_tests.python_get_spreading_code( spreading_code, 0 )

    self.assertEquals( codes, None )

    codes = csignal_tests.python_get_spreading_code( spreading_code, 8 )

    self.assertNotEquals( codes, None )
    self.assertEquals( len( codes ), 1 )

    codes = csignal_tests.python_get_spreading_code( spreading_code, 16 )

    self.assertNotEquals( codes, None )
    self.assertEquals( len( codes ), 2 )

    self.assertEquals( csignal_tests.csignal_destroy_spreading_code( spreading_code ), csignal_tests.CPC_ERROR_CODE_NO_ERROR )

  def test_write_wav_random( self ):
    bits_per_symbol     = 1
    constellation_size  = 2 ** bits_per_symbol
    sample_rate         = 48000
    baseband_amplitude  = 32000
    carrier_frequency   = 22000
    symbolExpansionFactor = 1
    separationInterval    = 1

    ( file_handle, file_name ) = touch_random_file()

    file_handle.close()

    data = ''.join( random.choice( string.ascii_lowercase ) for _ in range( 20 ) )
    
    symbol_tracker = csignal_tests.python_bit_stream_initialize( False, data )

    self.assertNotEquals( symbol_tracker, None )

    ( numberOfBits, buffer )  = csignal_tests.python_bit_stream_get_bits( symbol_tracker, bits_per_symbol ) 

    self.assertEquals( numberOfBits, bits_per_symbol )
    self.assertNotEquals( buffer, None )

    symbol = struct.unpack( "B", buffer )[ 0 ] >> ( 8 - bits_per_symbol )

    self.assertNotEquals( symbol, None )

    signal = []

    while( symbol != None ):
      signal_components = csignal_tests.python_csignal_modulate_BFSK_symbol (
        symbol,
        sample_rate,
        sample_rate,
        carrier_frequency,
        separationInterval,
        symbolExpansionFactor
                                                              )
  
      self.assertNotEquals( signal_components, None )
      self.assertEquals( len( signal_components ), 2 )
      self.assertNotEquals( signal_components[ 0 ], None )
      self.assertNotEquals( signal_components[ 1 ], None )
      self.assertEquals( len( signal_components[ 0 ] ), sample_rate )
      self.assertEquals( len( signal_components[ 1 ] ), sample_rate )

      part = []

      for index in range( sample_rate ):
        part.append( signal_components[ 0 ][ index ] - signal_components[ 1 ][ index ] )

      signal = signal + part

      ( numberOfBits, buffer )  = csignal_tests.python_bit_stream_get_bits( symbol_tracker, bits_per_symbol ) 

      if( 0 == numberOfBits ):
        symbol = None
      else:
        symbol = struct.unpack( "B", buffer )[ 0 ] >> ( 8 - bits_per_symbol )

    samples = [ signal, signal ]

    error = csignal_tests.python_write_FLOAT_wav (
      file_name,
      len( samples ),
      sample_rate,
      len( signal ),
      samples
                                                )

    self.assertEquals( error, csignal_tests.CPC_TRUE )

    self.assertEquals( csignal_tests.bit_stream_destroy( symbol_tracker ), csignal_tests.CPC_ERROR_CODE_NO_ERROR )

  def test_write_wav( self ):
    bits_per_symbol     = 1
    constellation_size  = 2 ** bits_per_symbol
    sample_rate         = 48000
    baseband_amplitude  = 32000
    carrier_frequency   = 22000
    symbolExpansionFactor = 1
    separationInterval    = 1

    ( file_handle, file_name ) = touch_random_file()

    file_handle.close()

    data = '\x12'
    
    symbol_tracker = csignal_tests.python_bit_stream_initialize( False, data )

    self.assertNotEquals( symbol_tracker, None )

    ( numberOfBits, buffer )  = csignal_tests.python_bit_stream_get_bits( symbol_tracker, bits_per_symbol ) 

    self.assertEquals( numberOfBits, bits_per_symbol )
    self.assertNotEquals( buffer, None )

    symbol = struct.unpack( "B", buffer )[ 0 ] >> ( 8 - bits_per_symbol )

    self.assertNotEquals( symbol, None )

    signal_components = csignal_tests.python_csignal_modulate_BFSK_symbol (
      symbol,
      sample_rate,
      sample_rate,
      carrier_frequency,
      separationInterval,
      symbolExpansionFactor
                                                            )

    self.assertNotEquals( signal_components, None )
    self.assertEquals( len( signal_components ), 2 )
    self.assertNotEquals( signal_components[ 0 ], None )
    self.assertNotEquals( signal_components[ 1 ], None )
    self.assertEquals( len( signal_components[ 0 ] ), sample_rate )
    self.assertEquals( len( signal_components[ 1 ] ), sample_rate )

    signal = []

    for index in range( sample_rate ):
      signal.append( signal_components[ 0 ][ index ] - signal_components[ 1 ][ index ] )

    samples = [ signal, signal ]

    error = csignal_tests.python_write_FLOAT_wav (
      file_name,
      len( samples ),
      sample_rate,
      sample_rate,
      samples
                                                )

    self.assertEquals( error, csignal_tests.CPC_TRUE )

    if( os.path.exists( file_name ) ):
      os.unlink( file_name )

    self.assertEquals( csignal_tests.bit_stream_destroy( symbol_tracker ), csignal_tests.CPC_ERROR_CODE_NO_ERROR )

  def test_generate_signal_random( self ):
    data = ''.join( random.choice( string.ascii_lowercase ) for _ in range( 10 ) )

    symbol_tracker = csignal_tests.python_bit_stream_initialize( False, data )

    self.assertNotEquals( symbol_tracker, None )

    ( numberOfBits, buffer )  = csignal_tests.python_bit_stream_get_bits( symbol_tracker, 1 ) 

    self.assertEquals( numberOfBits, 1 )
    self.assertNotEquals( buffer, None )

    symbol = struct.unpack( "B", buffer )[ 0 ] >> 7

    self.assertNotEquals( symbol, None )

    while( symbol != None ):
      signal = csignal_tests.python_csignal_modulate_BFSK_symbol (
        symbol,
        100,
        48000,
        22000,
        1,
        1
                                                              )

      self.assertNotEquals( signal, None )
      self.assertEquals( len( signal ), 2 )
      self.assertNotEquals( signal[ 0 ], None )
      self.assertNotEquals( signal[ 1 ], None )
      self.assertEquals( len( signal[ 0 ] ), 100 )
      self.assertEquals( len( signal[ 1 ] ), 100 )

      ( numberOfBits, buffer )  = csignal_tests.python_bit_stream_get_bits( symbol_tracker, 1 ) 

      if( 0 == numberOfBits ):
        symbol = None
      else:
        symbol = struct.unpack( "B", buffer )[ 0 ] >> 7

    self.assertEquals( csignal_tests.bit_stream_destroy( symbol_tracker ), csignal_tests.CPC_ERROR_CODE_NO_ERROR )

  def test_generate_signal( self ):
    self.assertEquals (
      csignal_tests.python_csignal_modulate_BFSK_symbol( 2, 10, 48000, 22000, 1, 1 ),
      None
                      )

    self.assertEquals (
      csignal_tests.python_csignal_modulate_BFSK_symbol( 1, 0, 48000, 22000, 1, 1 ),
      None
                      )

    self.assertEquals (
      csignal_tests.python_csignal_modulate_BFSK_symbol( 1, 10, 0, 22000, 1, 1 ),
      None
                      )

    self.assertNotEquals (
      csignal_tests.python_csignal_modulate_BFSK_symbol( 1, 10, 48000, 0, 1, 1 ),
      None
                      )

    self.assertNotEquals (
      csignal_tests.python_csignal_modulate_BFSK_symbol( 1, 10, 48000, -22000, 1, 1 ),
      None
                      )

  def test_generate_signal_basic( self ):
    data = '\x12\x34'
    
    symbol_tracker = csignal_tests.python_bit_stream_initialize( False, data )

    self.assertNotEquals( symbol_tracker, None )

    ( numberOfBits, buffer ) =  \
      csignal_tests.python_bit_stream_get_bits( symbol_tracker, 1 ) 

    self.assertEquals( numberOfBits, 1 )
    self.assertNotEquals( buffer, None )

    symbol = struct.unpack( "B", buffer )[ 0 ]

    self.assertNotEquals( symbol, None )

    symbol >>= 7

    while( symbol != None ):
      signal = \
        csignal_tests.python_csignal_modulate_BFSK_symbol (
          symbol,
          10,
          48000,
          22000,
          1,
          1
                                                          )

      self.assertNotEquals( signal, None )
      self.assertEquals( len( signal ), 2 )
      self.assertNotEquals( signal[ 0 ], None )
      self.assertNotEquals( signal[ 1 ], None )

      ( numberOfBits, buffer ) =  \
        csignal_tests.python_bit_stream_get_bits( symbol_tracker, 1 ) 

      if( 0 == numberOfBits ):
        symbol = None
      else:
        symbol = struct.unpack( "B", buffer )[ 0 ] >> 7

    self.assertEquals( csignal_tests.bit_stream_destroy( symbol_tracker ), csignal_tests.CPC_ERROR_CODE_NO_ERROR )

if __name__ == '__main__':
  csignal_tests.cpc_log_set_log_level( csignal_tests.CPC_LOG_LEVEL_ERROR )

  csignal_tests.csignal_initialize()

  unittest.main()

  csignal_tests.csignal_terminate()
