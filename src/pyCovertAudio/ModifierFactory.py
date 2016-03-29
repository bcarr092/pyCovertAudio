class ModifierFactory:

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
  def initializeModifiers( sampleRate, configuration, modifierList ):
    for modifier in configuration:
      modifierList.append( ModifierFactory.create( sampleRate, modifier ) )
