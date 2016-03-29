from pyCovertAudio_lib import *

class CAHALSampleRateRange:
  def __init__( self, minimumRate, maximumRate ):
    self.minimum = minimumRate
    self.maximum = maximumRate

  def printMe( self, indent ):
    if( self.minimum != self.maximum ):
      print "\t\t%d Hz - %d Hx" \
        %( self.minimum, self.maximum )
    else:
      print "\t\t%d Hz" %( self.minimum )
