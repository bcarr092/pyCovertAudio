from WAVReceiver  import WAVReceiver
from Debug        import Debug

import operator

class WAVMultiCarrierReceiver( WAVReceiver ):
  def __init__( self, configuration ):
    WAVReceiver.__init__( self, configuration )

  def handleDemodulation( self, signal ):
    symbols       = []
    sampleOffset  = None
    symbolsList   = self.dataDemodulator.demodulate( signal )

    if  (
      type( symbolsList ) is list
      and 0 < len( symbolsList )
      and type( symbolsList[ 0 ] ) is list
        ):
      offsteSymbol  = None
      offsetCount   = dict()

      for symbols in symbolsList:
        offsetSymbol  = self.locateData( symbols )

        if( offsetSymbol in offsetCount ):
          offsetCount[ offsetSymbol ] += 1
        else:
          offsetCount[ offsetSymbol ] = 1

      sortedOffsetCount = \
        sorted  (
          offsetCount.items(),
          key     = operator.itemgetter( 1 ),
          reverse = True
                )

      print "SORTED OFFSETS:"
      print sortedOffsetCount

      if( 0 < len( sortedOffsetCount ) ):
        offsetSymbol = sortedOffsetCount[ 0 ][ 0 ]

      if( offsetSymbol is not None ):
        print "Signal starts at symbol %d." %( offsetSymbol )
  
      sampleOffset =  \
        ( offsetSymbol + 1 )  \
        * self.samplesPerSymbol \
        * self.symbolExpansionFactor
  
      print "Sample offset is %d." %( sampleOffset )

      symbols = \
        self.dataDemodulator.assembleSymbols  (
          ( offsetSymbol + 1 ),
          symbolsList
                                              )

      decodedSymbols = self.extractSymbols( symbols, 0 )
    else:
      print "ERROR: Expected a list of lists."

    return( symbols, sampleOffset )
