class EncoderFactory:

    @staticmethod
    def create(configuration):
        try:
            classInfo = configuration['class']

            (moduleName, className) = classInfo.split('.')

            module = __import__(moduleName)
            classObject = getattr(module, className)
            instance = \
                classObject(
                    configuration['attributes']
                )

            return(instance)
        except KeyError as e:
            print "ERROR: Could not find key %s." % (str(e))
