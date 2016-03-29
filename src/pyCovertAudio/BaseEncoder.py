class BaseEncoder:
  def toString( self ):
    string =  \
      "BaseEncoder:\n\tBlock length (n):\t%d\n\tMessage length (k):\t%d" \
      "\n\tRate:\t\t\t%.2f" \
        %(
          self.blockLength,
          self.messageLength,
          ( float( self.messageLength ) / float( self.blockLength ) )
        )

    return( string )

  def __init__( self, configuration ):
    try:
      self.blockLength    = configuration[ "blockLength" ]
      self.messageLength  = configuration[ "messageLength" ]

    except KeyError as e:
      print "ERROR: Could not find key %s." %( str( e ) )

  def encode( self, data ):
    print "ERROR: 'encode' must be overridden."

    sys.exit( -1 )
  
  def decode( self, data, errorPositions=None ):
    print "ERROR: 'decode' must be overridden."

    sys.exit( -1 )

  def checkParamterIsString( self, parameter ):
    if( isinstance( parameter, str ) ):
      return( True )
    else:
      print "ERROR: Parameter data must be a string."

      return( False )
