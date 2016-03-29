class TestFactory:

  @staticmethod
  def create( sampleRate, configuration ):
    try:
      classInfo = configuration[ 'class' ]

      ( moduleName, className ) = classInfo.split( '.' )

      module      = __import__( moduleName )
      classObject = getattr( module, className )
      instance    = \
        classObject (
          sampleRate,
          configuration[ 'attributes' ]
                    )

      return( instance )
    except KeyError as e:
      print "ERROR: Could not find key %s." %( str( e ) )

  @staticmethod
  def initializeTests( sampleRate, configuration, testList ):
    for test in configuration:
      testList.append( TestFactory.create( sampleRate, test ) )
