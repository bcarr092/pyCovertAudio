from pyCovertAudio_lib      import *
from BaseDemodulator  import BaseDemodulator
from Debug            import Debug
from SignalFunctions  import SignalFunctions

import time
import math
import struct

class BFSKDemodulator( BaseDemodulator ):
  def __init__  (
    self, bitsPerSymbol, sampleRate, samplesPerSymbol, symbolExpansionFactor,
    separationIntervals, configuration
                ):
                
    BaseDemodulator.__init__  (
      self,
      bitsPerSymbol,
      sampleRate,
      samplesPerSymbol,
      symbolExpansionFactor,
      separationIntervals,
      configuration
                            )

    self.symbolFrequencyBandwidth = configuration[ 'symbolFrequencyBandwidth' ]

    self.initializeSignals()

    self.initializeFilters()

  def initializeSignals( self ):
    (
      self.symbol0Frequency,
      self.symbol1Frequency,
      self.deltaFrequency,
      self.bandwidth
    ) =  \
      python_BFSK_determine_frequencies (
        self.samplesPerSymbol,
        self.sampleRate,
        self.carrierFrequency,
        self.separationIntervals
                                        )

    print "Symbol 0: %.02f\tSymbol 1: %.02f\tDelta: %.02f\tBandwidth: %.02f"  \
      %(
        self.symbol0Frequency,
        self.symbol1Frequency,
        self.deltaFrequency,
        self.bandwidth
      )

    self.interpolationGap =  \
      int (
        2.0
        *(
          ( float( self.sampleRate ) / 2.0 )
          - ( self.carrierFrequency + ( self.bandwidth / 2.0 ) )
        )
          )

    print "Gap: %d" %( self.interpolationGap )

    self.decimationFactor =  \
      int (  
        math.floor  (
          ( 2.0 * self.samplesPerSymbol )
           / float( self.decimatedSamplesPerSymbol )
                    )
          )
    self.decimatedSampleRate = \
      int (
        math.ceil (
          ( 2.0 * self.sampleRate )
          / float( self.decimationFactor )
                  )
          )

  def initializeFilters( self ):
    passbandSymbol0Frequency = self.carrierFrequency + self.symbol0Frequency
    passbandSymbol1Frequency = self.carrierFrequency + self.symbol1Frequency

    frequencySeparation = passbandSymbol1Frequency - passbandSymbol0Frequency

    self.filter0 = \
     python_initialize_kaiser_filter (
        passbandSymbol0Frequency - frequencySeparation,
        passbandSymbol0Frequency -
          ( float( self.symbolFrequencyBandwidth ) / 2.0 ), 
        passbandSymbol0Frequency +
          ( float( self.symbolFrequencyBandwidth ) / 2.0 ), 
        passbandSymbol0Frequency + frequencySeparation,
        0.1,
        80,
        self.sampleRate
                                      )

    self.filter1 = \
     python_initialize_kaiser_filter (
        passbandSymbol1Frequency - frequencySeparation,
        passbandSymbol1Frequency - 
          ( float( self.symbolFrequencyBandwidth ) / 2.0 ),
        passbandSymbol1Frequency + 
          ( float( self.symbolFrequencyBandwidth ) / 2.0 ),
        passbandSymbol1Frequency + frequencySeparation,
        0.1,
        80,
        self.sampleRate
                                      )

    self.filterDelay0 = python_filter_get_group_delay( self.filter0 )
    self.filterDelay1 = python_filter_get_group_delay( self.filter1 )

    print "Symbol filter delays:\t1=%d\t2=%d" \
      %( self.filterDelay0, self.filterDelay1 )

  def combineSignal( self, signal, identifier=0 ):
    filterStartTime = time.time()

    filteredSignal0 = python_filter_signal( self.filter0, signal )
    filteredSignal0 = filteredSignal0[ self.filterDelay0 : ]

    filteredSignal1 = python_filter_signal( self.filter1, signal )
    filteredSignal1 = filteredSignal1[ self.filterDelay1 : ]

    filterTime = time.time() - filterStartTime

    print "Filter time: %.04f" %( filterTime )

    Debug.instance.debugSignal  (
      'filteredSignal0_%d.WAV' %( identifier ),
      filteredSignal0,
      self.sampleRate
                                )   
    Debug.instance.debugSignal  (
      'filteredSignal1_%d.WAV' %( identifier ),
      filteredSignal1,
      self.sampleRate
                                )

    interpolatorStartTime = time.time()
    
    interpolated0 = \
      SignalFunctions.interpolateSignal (
        filteredSignal0,
        self.sampleRate,
        self.interpolationGap
                                        )
    interpolated1 = \
      SignalFunctions.interpolateSignal (
        filteredSignal1,
        self.sampleRate,
        self.interpolationGap
                                        )

    interpolatorTime = time.time() - interpolatorStartTime

    print "Interpolation time: %.04f" %( interpolatorTime )

    Debug.instance.debugSignal  (
      'interpolatedSignal0_%d.WAV' %( identifier ),
      interpolated0,
      2 * self.sampleRate
                                )   
    Debug.instance.debugSignal  (
      'interpolatedSignal1_%d.WAV' %( identifier ),
      interpolated1,
      2 * self.sampleRate
                                )

    squareStartTime = time.time()

    squared0 =  \
      SignalFunctions.squareSignal  (
        interpolated0,
        2 * self.sampleRate,
        self.symbolFrequencyBandwidth,
        self.interpolationGap
                                    )
    squared1 =  \
      SignalFunctions.squareSignal  (
        interpolated1,
        2 * self.sampleRate,
        self.symbolFrequencyBandwidth,
        self.interpolationGap
                                    )

    squareTime = time.time() - squareStartTime

    print "Square time: %.04f" %( squareTime )

    Debug.instance.debugSignal  (
      'squaredSignal0_%d.WAV' %( identifier ),
      squared0,
      2 * self.sampleRate
                                )   
    Debug.instance.debugSignal  (
      'squaredSignal1_%d.WAV' %( identifier ),
      squared1,
      2 * self.sampleRate
                                )

    decimateStartTime = time.time()

    decimated0 = SignalFunctions.decimate( squared0, self.decimationFactor )
    decimated1 = SignalFunctions.decimate( squared1, self.decimationFactor )

    decimateTime = time.time() - decimateStartTime

    print "Decimate time: %.04f" %( decimateTime )

    Debug.instance.debugSignal  (
      'signal0_%d.WAV' %( identifier ),
      decimated0,
      self.decimatedSampleRate
                                )   
    Debug.instance.debugSignal  (
      'signal1_%d.WAV' %( identifier ),
      decimated1,
      self.decimatedSampleRate
                                )

    signal0 = SignalFunctions.normalizeSignal( decimated0 )
    signal1 = SignalFunctions.normalizeSignal( decimated1 )

    signal1 = [ -1.0 * x for x in signal1 ]

    signal0Length  = len( signal0 )
    signal1Length  = len( signal1 )

    length = min( signal0Length, signal1Length )

    result = [ 0.0 for i in range( length ) ]

    for i in range( length ):
      if( abs( signal0[ i ] ) >= abs( signal1[ i ] ) ):
        result[ i ] = signal0[ i ]
      else:
        result[ i ] = signal1[ i ]

    Debug.instance.debugSignal  (
      'combined_%d.WAV' %( identifier ),
      result,
      self.decimatedSampleRate
                                )

    return( result )

  def demodulate( self, signal, identifier=0 ):
    combined  = self.combineSignal( signal, identifier )
    symbols   = self.getSymbols( combined, identifier )

    return( symbols )

  def toString( self ):
    return  (
      "Demodulator:\n\tAlgorithm:\t\t\tBFSK\n\tSymbol 0 frequency:\t\t%.02f\n\t"
      "Symbol 1 frequency:\t\t%.02f\n\tMin frequency separation:\t%.02f\n\t"
      "Bandwidth:\t\t\t%.02f\n\tFrequency bandwidth:\t\t%d\n%s" \
      %(
        self.symbol0Frequency,
        self.symbol1Frequency,
        self.deltaFrequency,
        self.bandwidth,
        self.symbolFrequencyBandwidth,
        BaseDemodulator.toString( self )
      )
            )

  def getSymbols( self, signal, identifier=0 ):
    symbols = []

    averaged =  \
      SignalFunctions.movingAverage (
        signal,
        self.symbolExpansionFactor * self.decimatedSamplesPerSymbol
                                    )

    Debug.instance.debugSignal  (
      'averaged_%d.WAV' %( identifier ),
      averaged,
      self.decimatedSampleRate
                                )

    averaged = SignalFunctions.removeBias( averaged )

    Debug.instance.debugSignal  (
      'unbiasedAveraged_%d.WAV' %( identifier ),
      averaged,
      self.decimatedSampleRate
                                )

    averaged =  \
      SignalFunctions.interpolateSignal (
        averaged,
        self.decimatedSampleRate,
        1
                                        )

    Debug.instance.debugSignal  (
      'averagedInterpolated_%d.WAV' %( identifier ),
      averaged,
      2 * self.decimatedSampleRate
                                )

    averaged = SignalFunctions.normalizeSignal( averaged )

    Debug.instance.debugSequence( 'averaged.dat', averaged )

    samplePoints =  \
      self.runGardnerAlgorithm  (
        averaged,
        2 * self.symbolExpansionFactor * self.decimatedSamplesPerSymbol,
        2 * self.decimatedSampleRate
                                )

    Debug.instance.debugSequence( 'samplePoints.dat', samplePoints )

    for point in samplePoints:
      symbol = 0 if( averaged[ point ] >= 0.0 ) else 1

      symbols.append( symbol )

    return( symbols )

  def runGardnerAlgorithm( self, signal, samplesPerSymbol, sampleRate ):
    n       = 1
    offset  = 0

    samplePoints = []

    print "Samples per symbol: %d\tSample rate: %d." \
      %( samplesPerSymbol, sampleRate )

    while( ( ( n * samplesPerSymbol ) + offset ) < len( signal ) ):
      nextPoint     = n * samplesPerSymbol + offset
      previousPoint = ( n - 1 ) * samplesPerSymbol + offset
      midPoint      = \
        int( nextPoint - int( math.floor( samplesPerSymbol / 2.0 ) ) )

      samplePoints.append( nextPoint )

      e = \
        ( signal[ nextPoint ] - signal[ previousPoint ] ) * signal[ midPoint ]

      if( ( signal[ nextPoint ] * signal[ previousPoint ] ) < 0 ):
        if( e < -0.0001 ):
          offset += 1
        elif( e > 0.0001 ):
          offset -= 1

      #print \
        #"Time: %.04f s\tn: %d (%d)\tNext: %d (%.04f)\tPrev: %d (%.04f)\t" \
        #"Mid: %d (%.04f)\tError: %e\tOffset: %d" \
        #%(
          #float( nextPoint ) / sampleRate,
          #n,
          #n * samplesPerSymbol,
          #nextPoint,
          #signal[ nextPoint ],
          #previousPoint,
          #signal[ previousPoint],
          #midPoint,
          #signal[ midPoint ],
          #e,
          #offset
        #) 

      n += 1

    return( samplePoints )
