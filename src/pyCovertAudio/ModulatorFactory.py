class ModulatorFactory:

    @staticmethod
    def create(
            bitsPerSymbol, sampleRate, samplesPerSymbol, symbolExpansionFactor,
            separationIntervals, configuration,
    ):
        try:
            classInfo = configuration['class']

            (moduleName, className) = classInfo.split('.')

            module = __import__(moduleName)
            classObject = getattr(module, className)
            instance    = \
                classObject(
                    bitsPerSymbol,
                    sampleRate,
                    samplesPerSymbol,
                    symbolExpansionFactor,
                    separationIntervals,
                    configuration['attributes']
                )

            return(instance)
        except KeyError as e:
            print "ERROR: Could not find key %s." % (str(e))
