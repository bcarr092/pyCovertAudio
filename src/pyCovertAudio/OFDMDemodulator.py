from pyCovertAudio_lib      import *
from BaseDemodulator  import BaseDemodulator
from BFSKDemodulator  import BFSKDemodulator
from Debug            import Debug
from SignalFunctions  import SignalFunctions

import time
import math
import struct

class OFDMDemodulator( BaseDemodulator ):
  def __init__  (
    self, bitsPerSymbol, sampleRate, samplesPerSymbol, symbolExpansionFactor,
    separationIntervals, configuration
                ):

    configuration[ "carrierFrequency" ] = 0.0
                
    BaseDemodulator.__init__  (
      self,
      bitsPerSymbol,
      sampleRate,
      samplesPerSymbol,
      symbolExpansionFactor,
      separationIntervals,
      configuration
                            )

    (
      self.symbol0Frequency,
      self.symbol1Frequency,
      self.deltaFrequency,
      self.bandwidth
    ) = \
      python_BFSK_determine_frequencies (
        self.samplesPerSymbol,
        self.sampleRate,
        0,
        self.separationIntervals
                                        )

    try:
      self.minimumFrequency         = configuration[ "minimumFrequency" ]
      self.maximumFrequency         = configuration[ "maximumFrequency" ]
      self.symbolFrequencyBandwidth = configuration[ 'symbolFrequencyBandwidth' ]
      self.bandwidthDivisor         = configuration[ "bandwidthDivisor" ]

      self.bandwidth /= self.bandwidthDivisor

      self.carrierFrequencies   = \
        SignalFunctions.getCarrierFrequencies (
          self.minimumFrequency,
          self.maximumFrequency,
          self.bandwidth
                                              )
      self.numberOfSubChannels  = len( self.carrierFrequencies )

    except KeyError as e:
      print "ERROR: Could not find key %s" %( str( e ) )

    self.initializeSubChannelDemodulators()

  def initializeSubChannelDemodulators( self ):
    self.demodulators = []

    for carrierFrequency in self.carrierFrequencies:
      demodulator = \
        BFSKDemodulator (
          self.bitsPerSymbol,
          self.sampleRate,
          self.samplesPerSymbol,
          self.symbolExpansionFactor,
          self.separationIntervals,
          {
            "carrierFrequency"          : carrierFrequency,
            "symbolFrequencyBandwidth"  : self.symbolFrequencyBandwidth,
            "decimatedSamplesPerSymbol" : self.decimatedSamplesPerSymbol
          }
                        )

      self.demodulators.append( demodulator )

  def assembleSymbols( self, offset, symbolsList ):
    symbols = []

    if( len( symbolsList ) != len( self.demodulators ) ):
      print "ERROR: Symbols list length (%d) is not equal"  \
            "to the number of sub-channels (%d)." \
            %( len( symbolsList ), len( self.demodualtors ) )
    else:
      minimumLength = min( map( len, symbolsList ) )

      print "Min length: %d." %( minimumLength )

      for i in range( offset, minimumLength ):
        for j in range( len( symbolsList ) ):
          symbol = symbolsList[ j ][ i ]

          symbols.append( symbol )

    return( symbols )

  def demodulate( self, signal ):
    symbolsList = []
    identifier  = 1

    for demodulator in self.demodulators:
      symbols = demodulator.demodulate( signal, identifier )

      symbolsList.append( symbols )

      identifier += 1

    return( symbolsList )

  def toString( self ):
    return  (
      "Demodulator:\n\tAlgorithm:\t\t\tOFDM\n\tSymbol 0 frequency:\t\t%.02f\n\t"
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
