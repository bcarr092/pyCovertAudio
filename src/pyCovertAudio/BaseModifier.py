import sys

class BaseModifier:
  def __init__( self, sampleRate, configuration ):
    self.sampleRate = sampleRate

  def modify( self, signal ):
    print "ERROR: Modify must be implemented."

    sys.exit( -1 )

  def toString( self ):
    return( "BaseModifier:\n\tSample rate: %d\n" %( self.sampleRate ) )

  def getOffset( self ):
    print "ERROR: GetOffset must be implemented."

    sys.exit( -1 )
