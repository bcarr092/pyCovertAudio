#!/usr/local/bin/python

import csignal_tests                                                              
import unittest                                                                 
import sys
                                                                                
from test_csignal import TestsCSignal
from test_bit_packer import TestsBitPacker
from test_bit_stream import TestsBitStream
from test_conv import TestsConv
from test_signal_operations import TestsSignalOperations

csignal_tests.cpc_log_set_log_level( csignal_tests.CPC_LOG_LEVEL_NO_LOGGING )

csignal_tests.csignal_initialize()

alltests = unittest.TestSuite ( [                                                   \
 unittest.TestLoader().loadTestsFromTestCase( TestsCSignal ),                       \
 unittest.TestLoader().loadTestsFromTestCase( TestsBitPacker ),                     \
 unittest.TestLoader().loadTestsFromTestCase( TestsBitStream ),                     \
 unittest.TestLoader().loadTestsFromTestCase( TestsConv ),                          \
 unittest.TestLoader().loadTestsFromTestCase( TestsSignalOperations),               \

                                ] )

result = unittest.TextTestRunner( verbosity=2 ).run( alltests )

csignal_tests.csignal_terminate()

if( len( result.errors ) == 0 and len( result.failures ) == 0 ):
    sys.exit( 0 )
else:
    sys.exit( -1 )
