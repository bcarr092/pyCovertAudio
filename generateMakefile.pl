#!/usr/bin/perl -w

use strict;

use Switch;
use Data::Dumper;
use Getopt::Mixed;

# The possible errors returned by this process
use constant
{
  ERROR_SUCCESS               =>  0,
  ERROR_PARSING_CMD_LINE      =>  -1, 
  ERROR_APPLE_TOOLCHAIN_DIR   =>  -2,
  ERROR_MODE                  =>  -3,
  ERROR_SOURCE                =>  -4,
  ERROR_APPLE_ROOT            =>  -5,
  ERROR_ANDROID_ROOT          =>  -6,
  ERROR_DEVELOPER_ROOT        =>  -7,
  ERROR_ANDROID_TOOLCHAIN_DIR =>  -8,
  ERROR_DIRECTORY             =>  -9,
  ERROR_BINARIES              =>  -10,
  ERROR_WINDOWS_ROOT          =>  -11,
  ERROR_BUILD                 =>  -12,
  ERROR_INSTALL               =>  -13,

  DEFAULT_GENERATOR           =>  'Unix Makefiles',

  DEFAULT_APPLE_DEV_DIR_REL           =>  'Developer',
  DEFAULT_APPLE_SYS_DIR_REL           =>  'Developer/SDKs',
  DEFAULT_APPLE_BIN_DIR_REL           =>  'usr/bin',
  DEFAULT_APPLE_LIB_DIR_REL           =>  'usr/lib',
  DEFAULT_APPLE_GENERIC_TOOLCHAIN_DIR =>
    'Toolchains/XcodeDefault.xctoolchain',

  DEFAULT_ANDROID_BIN_DIR_REL =>  'bin',
  DEFAULT_ANDROID_SYS_DIR_REL =>  'sysroot',
  DEFAULT_ANDROID_MAKE_TOOLCHAIN_BIN_REL  =>
    'build/tools/make-standalone-toolchain.sh',

  DEFAULT_WINDOWS_BIN_DIR_REL =>  'bin',
  DEFAULT_WINDOWS_SYS_DIR_REL =>  'mingw',

  CMAKE_SYSTEM_NAME_OSX       =>  'Darwin',
  CMAKE_SYSTEM_NAME_WINDOWS   =>  'Windows',
  CMAKE_SYSTEM_NAME_IOS       =>  'Darwin',
  CMAKE_SYSTEM_NAME_ANDROID   =>  'Generic',
  CMAKE_SYSTEM_NAME_GENERIC   =>  'Generic',
};

# Configuration for Getopt::Mixed
my $COMMAND_LINE_OPTIONS  =   "p:s t=s a=s m=s s=s r=s g=s b=s i=s c=s d h";
$COMMAND_LINE_OPTIONS     .=  " platform>p target>t architecture>a mode>m";
$COMMAND_LINE_OPTIONS     .=  " source>s generator>g build>b install>i";
$COMMAND_LINE_OPTIONS     .=  " cache>c debug>d help>h";

my %TARGETS               = ();
my %MODES                 = (
                              Release           =>  1,
                              Debug             =>  1,
                              RelWithDebInfo    =>  1,
                              MinSizRel         =>  1
                            );
my $DEBUG                 = 0;
my $PLATFORM;
my $TARGET;
my $ARCHITECTURE;
my $MODE;
my $SOURCE_DIRECTORY;
my $BUILD_DIRECTORY;
my $INSTALL_DIRECTORY;
my $CACHE_FILE;
my $GENERATOR             = DEFAULT_GENERATOR;

# Usage statement printed on error or help switch being defined
my $USAGE =<<OUT;
Usage: generateMakefile.pl
  -p, --platform [<arg>]    The platform to generate the Makefile for. Passing
                            this parameter with no argument will print all
                            supported build targets. Just specifying a platform
                            will generate a Makefile for all targets and
                            architectures for that platform. Specifying 'all'
                            will generate Makefiles for all platforms, targets
                            and architectures (Required).
  -t, --target <arg>        The deployment target to generate the Makefile for.
                            Just specifying a platform and target will generate
                            a Makefile for all architectures for the platform
                            and target specified.
  -a, --architecture=<arg>  The architecture to generate the Makefile for
  -m, --mode=<arg>          The mode to generate the Makefile for (Required).
  -s, --source=<arg>        The directory where the source code is located.
                            This must be an absolute path and the directory
                            this points to must contain a CMake file (i.e.
                            CMakeLists.txt) - (Required).
  -b, --build=<arg>         The directory where to store the CMake build
                            artefacts. This must be an absolute path and be a
                            existing direcxtory. If this parameter is not set
                            the build directory will be the source directory.
  -i, --install=<arg>       The directory where artefacts will be installed to.
  -c, --cache=<arg>         Location of a CMake cache file.
  -d, --debug               Print debug messages to standard out
  -g, --generator=<arg>     The generator to use when building the project. The
                            default value for this is "Unix Makefiles."
  -h, --help                Print this message  

  The following environment variables must be set:

  DEVELOPER_ROOT            The install location that the generated Android
                            toolchains will generated in. This path also
                            added to the search path for libraries and
                            includes.
  APPLE_TOOLCHAIN_DIR       The root directory where the 'Platforms' directory
                            is located (e.g. /Applications/XCode.app/Contents
                            /Developer).
  ANDROID_TOOLCHAIN_DIR     The root directory where the 'platforms' directory
                            is located (e.g. the android-ndk directory ).
  WINDOWS_TOOLCHAIN_DIR     The root directory where the mingw sub-directories
                            are located (e.g. mingw-w32, mingw-w64).
OUT

sub enumerateBuildTargets
{

  if( defined( $ENV{ 'APPLE_TOOLCHAIN_DIR' } ) )
  {
    &enumerateAppleTargets();
  }

  if  (
          defined( $ENV{ 'DEVELOPER_ROOT' } )
          && defined( $ENV{ 'ANDROID_TOOLCHAIN_DIR' } )
      )
  {
    &enumerateAndroidTargets();
  }

  if( defined( $ENV{ 'WINDOWS_TOOLCHAIN_DIR' } ) )
  {
    &enumerateWindowsTargets();
  }
}

sub enumerateWindowsTargets
{
  my $toolchainLocation = $ENV{ 'WINDOWS_TOOLCHAIN_DIR' };

  &printDebug( "Using Windows toolchain directory: '".$toolchainLocation."'" );

  if( opendir( PLATFORMS, $toolchainLocation ) )
  {
    foreach my $directory ( readdir( PLATFORMS ) )
    {
      next if( $directory =~ /^(\.|\.\.)$/ ); 
      next if( $directory !~ /^mingw-w/ );
  
      &printDebug( "\tExamining platform: ".$directory );

      if( $directory =~ /^(mingw-(w\d+))/ )
      {
        my $platform      = CMAKE_SYSTEM_NAME_WINDOWS;
        my $target        = $1;
        my $architecture  = $2;

        &printDebug (
          "\t\tExamining target: ".$target.", architecture=".$architecture
                    );

        my $binDirectory      = $toolchainLocation.'/'.$directory.'/'.
                                  DEFAULT_WINDOWS_BIN_DIR_REL;
        my $sysRootDirectory  = $toolchainLocation.'/'.$directory.'/'.
                                  DEFAULT_WINDOWS_SYS_DIR_REL;
        my ( $gcc, $gxx, $ar )=
          &findBinaries( $binDirectory, "gcc", "g++", "ar" );
        my $archDirectory     = readlink( $sysRootDirectory );

        if( $archDirectory =~ /^(.*?)-w\d+-mingw/ )
        {
          $architecture = $1;
        }

        $TARGETS{ $platform }->{ $target }->{ $architecture } =
            {
              BIN_DIRECTORY =>  $binDirectory,
              SYSTEM_NAME   =>  $platform,
              AR            =>  $binDirectory.'/'.$ar,
              C_COMPILER    =>  $binDirectory.'/'.$gcc,
              CXX_COMPILER  =>  $binDirectory.'/'.$gxx,
              SYS_ROOT      =>  $sysRootDirectory,
              VERSION       =>  1,
              C_FLAGS       =>  [
                                  "-pedantic-errors",
                                  "-Wall",
                                ],
              CXX_FLAGS     =>  [
                                  "-pedantic-errors",
                                  "-Wall",
                                ],
              LD_FLAGS      =>  [
                                ],
            };
      }
    }

    closedir( PLATFORMS );
  }
  else
  {
    &printUsageAndExit  (
                      ERROR_APPLE_TOOLCHAIN_DIR,
                      "Could not open directory '".$toolchainLocation."': ".$!
                        );
  }
}

sub makeAndroidToolchain
{
  my $platform      = shift;
  my $architecture  = shift;

  my $systemName          = lc( `uname` );
  my $systemArchitecture  = lc( `uname -m` );

  chomp( $systemName );
  chomp( $systemArchitecture );

  my $command =
    $ENV{ 'ANDROID_TOOLCHAIN_DIR' }.'/'.DEFAULT_ANDROID_MAKE_TOOLCHAIN_BIN_REL.
    " --platform=".$platform." --arch=".$architecture.
    " --system=".$systemName.'-'.$systemArchitecture.
    " --ndk-dir=".$ENV{ 'ANDROID_TOOLCHAIN_DIR' }.
    " --install-dir=".$ENV{ 'DEVELOPER_ROOT' }.'/toolchains/'.
      $platform.'--'.$architecture;

  print( "\t\t\tMaking toolchain..." );

  my $output = `$command`;

  print ( "\t\t\tDone.\n" );
}

sub findBinaries
{
  my ( $directory, @binarySuffixes ) = @_;
  
  my %binaries = ();

  if( opendir( DIR, $directory ) )
  {
    foreach my $binary ( readdir( DIR ) )
    {
      next if( $binary =~ /^(\.|\.\.)$/ ); 

      foreach my $suffix ( @binarySuffixes )
      {
        if  (
              $binary =~ /\Q$suffix\E$/
              && ! defined( $binaries{ $suffix } )
            )
        {
          $binaries{ $suffix } = $binary;
        }
      }
    }

    if( scalar( keys( %binaries ) ) != scalar( @binarySuffixes ) )
    {
      &printUsageAndExit  (
        ERROR_BINARIES,
        "Could not find all binaries in '".$directory."'"
                          );
    }

    closedir( DIR );
  }
  else
  {
    &printUsageAndExit( ERROR_DIRECTORY, "Could not open '".$directory."'" );
  }

  my @binaries = ();

  foreach my $suffix ( @binarySuffixes )
  {
    push( @binaries, $binaries{ $suffix } );
  }

  return( @binaries );
}

sub enumerateAndroidTargets
{
  &generateAndroidToolchains();

  my $toolchainLocation = $ENV{ 'DEVELOPER_ROOT' }.'/toolchains';

  &printDebug( "Using Android toolchain directory: '".$toolchainLocation."'" );

  if( opendir( PLATFORMS, $toolchainLocation ) )
  {
    foreach my $directory ( readdir( PLATFORMS ) )
    {
      next if( $directory =~ /^(\.|\.\.)$/ ); 
      next if( $directory !~ /^android-/ );
  
      &printDebug( "\tExamining platform: ".$directory );

      if( $directory =~ /^(.*?)--(.*?)$/ )
      {
        my $version;
        my $platform      = CMAKE_SYSTEM_NAME_ANDROID;
        my $target        = $1;
        my $architecture  = $2;

        if( $target =~ /^android-(.*?)$/ )
        {
          $version = $1;
        }
      
        &printDebug (
          "\t\tExamining target: ".$target.", architecture=".$architecture.
          ", version=".$version
                    );

        my $binDirectory  = $toolchainLocation.'/'.$directory.'/'.
                              DEFAULT_ANDROID_BIN_DIR_REL;
  
        my $sysRootDirectory  = $toolchainLocation.'/'.$directory.'/'.
                                  DEFAULT_ANDROID_SYS_DIR_REL;
        my ( $gcc, $gxx, $ar )=
          &findBinaries( $binDirectory, "gcc", "g++", "ar" );

        $TARGETS{ $platform }->{ $target }->{ $architecture } =
            {
              BIN_DIRECTORY =>  $binDirectory,
              SYSTEM_NAME   =>  CMAKE_SYSTEM_NAME_ANDROID,
              AR            =>  $binDirectory.'/'.$ar,
              C_COMPILER    =>  $binDirectory.'/'.$gcc,
              CXX_COMPILER  =>  $binDirectory.'/'.$gxx,
              SYS_ROOT      =>  $sysRootDirectory,
              VERSION       =>  $version,
              C_FLAGS       =>  [
                                  "-std=c99",
                                  "-Wall",
                                ],
              CXX_FLAGS     =>  [
                                  "-Wall",
                                ],
              LD_FLAGS      =>  [
                                ],
            };
      }
    }

    closedir( PLATFORMS );
  }
  else
  {
    &printUsageAndExit  (
                      ERROR_APPLE_TOOLCHAIN_DIR,
                      "Could not open directory '".$toolchainLocation."': ".$!
                        );
  }
}

sub generateAndroidToolchains
{
  my $toolchainLocation = $ENV{ 'ANDROID_TOOLCHAIN_DIR' };

  &printDebug( "Using Android toolchain directory: '".$toolchainLocation."'" );

  my $platformDirectory = $toolchainLocation."/platforms";

  if( opendir( PLATFORMS, $platformDirectory ) )
  {
    foreach my $platform ( readdir( PLATFORMS ) )
    {
      next if( $platform =~ /^(\.|\.\.)$/ );
      next if (
                $platform !~ /^android-/
                || ! -d $platformDirectory.'/'.$platform
              );

      &printDebug( "\tExamining platform: ".$platform );

      if( opendir( PLATFORM, $platformDirectory.'/'.$platform ) )
      {
        foreach my $architecture ( readdir( PLATFORM ) )
        {
          next if( $architecture =~ /^(\.|\.\.)$/ );
          next if (
                    $architecture !~ /^arch-/
                    || ! -d $platformDirectory.'/'.$platform.'/'.$architecture
                  );

          if( $architecture =~ /^arch-(.*?)$/ )
          {
            $architecture = $1;

            &printDebug( "\t\tExamining architecture: ".$architecture );

            if  (
              ! -d $ENV{ 'DEVELOPER_ROOT' }.'/toolchains/'.$platform.
                    '--'.$architecture
                )
            {
              &makeAndroidToolchain( $platform, $architecture );
            }
            else
            {
              &printDebug( "\t\tToolchain exists." );
            }
          }
        }

        closedir( PLATFORM );
      }
      else
      {
        &printUsageAndExit  (
          ERROR_ANDROID_TOOLCHAIN_DIR,
          "Could not open '".$platformDirectory.'/'.$platform
                            );
      }
    }

    closedir( PLATFORMS );
  }
  else
  {
    &printUsageAndExit  (
      ERROR_ANDROID_TOOLCHAIN_DIR,
      "Could not open '".$toolchainLocation."/platforms': ".$!
                        );
  }
}

sub enumerateAppleArchitectures
{
  my $platform      = lc( shift );
  my $sdkDirectory  = shift;
  my @architectures = ();

  my $XCodeRunCommand = "xcrun -sdk $platform lipo".
                        " -info  $sdkDirectory'/usr/lib/libc.dylib'";

  &printDebug( "Testing architecture of libc: ".$XCodeRunCommand );

  my $output          = `$XCodeRunCommand`;

  &printDebug( "Resulting arcvhitectures: ".$output );

  if( $output =~ /:\s+([^:]+)$/ )
  {
    my $architectures = $1;

    @architectures = split( /\s+/, $architectures );

    &printDebug( "\t\t\tFound architectures: ".join( ' ', @architectures ) );
  }

  return( @architectures );
}

sub enumerateAppleDeploymentTargets
{
  my $platform          = shift;
  my $platformDirectory = shift;
  my $parameters        = shift;
  my $sdksDirectory     = $platformDirectory.'/'.DEFAULT_APPLE_SYS_DIR_REL;

  if( opendir( TARGETS, $sdksDirectory ) )
  {
    foreach my $directory ( readdir( TARGETS ) )
    {
      next if( $directory =~ /^(\.|\.\.)$/ );
      next if( $directory !~ /\.sdk$/ );

      if( $directory =~ /^($platform(.*?))\.sdk$/ )
      {
        my $target  = $1;
        my $version = $2;

        my $genericToolchainDir = $ENV{ 'APPLE_TOOLCHAIN_DIR' }.'/'.
                                    DEFAULT_APPLE_GENERIC_TOOLCHAIN_DIR;
        my $platformSpecificToolchainDir = $platformDirectory.'/'.
                                            DEFAULT_APPLE_DEV_DIR_REL;

        &printDebug( "\t\tExamining target: '".$target."', version=".$version );
                     
        $parameters->{ SYS_ROOT } = $sdksDirectory.'/'.$directory;
        $parameters->{ VERSION }  = $version;
        $parameters->{ AR }       = $genericToolchainDir.'/usr/bin/ar';

        my $cFlags    = [
                          '-pedantic-errors',
                          '-Wall',
                        ];

        my $cxxFlags  = [
                          '-pedantic-errors',
                          '-Wall',
                        ];
        my $ldFlags   = [
                        ];

        if( $platform =~ /^iPhoneSimulator/ )
        {

          push( @{ $cFlags }, lc( "-miphoneos-version-min=$version" ) );
          push( @{ $cxxFlags }, lc( "-miphoneos-version-min=$version" ) );
        }
        elsif( $platform =~ /^iPhoneOS/ )
        {
          push( @{ $cFlags }, lc( "-m$platform-version-min=$version" ) );
          push( @{ $cxxFlags }, lc( "-m$platform-version-min=$version" ) );
        }
        elsif( $platform =~ /^MacOSX/ )
        {
          push( @{ $cFlags }, lc( "-m$platform-version-min=$version" ) );
          push( @{ $cxxFlags }, lc( "-m$platform-version-min=$version" ) );
        }

        my @architectures = &enumerateAppleArchitectures  (
                                                  $platform,
                                                  $sdksDirectory.'/'.$directory
                                                          );

        foreach my $architecture ( @architectures )
        {
          my @cFlags   = @{ $cFlags };
          my @cxxFlags = @{ $cxxFlags };
          my @ldFlags  = @{ $ldFlags };

          push( @cFlags, '-arch '.$architecture );
          push( @cxxFlags, '-arch '.$architecture );
          push( @ldFlags, '-arch '.$architecture );

          $parameters->{ C_FLAGS }    = \@cFlags;
          $parameters->{ CXX_FLAGS }  = \@cxxFlags;
          $parameters->{ LD_FLAGS }   = \@ldFlags;

          &printDebug( "Adding flags for architecture: ".$architecture );
          &printDebug( "\tFlags: @cFlags" );

          my %finalParameters = %{ $parameters };
          
          $TARGETS{ $platform }->{ $target }->{ $architecture } =
            \%finalParameters; 
        }
      }
    }

    closedir( TARGETS );
  }
  else
  {
    &printUsageAndExit  (
                      ERROR_APPLE_TOOLCHAIN_DIR,
                      "Could not open directory '".$platformDirectory."': ".$!
                        );
  }
}

sub getSystemName
{
  my $platform  = shift;
  my $system;

  if( $platform eq 'MacOSX' )
  {
    $system = CMAKE_SYSTEM_NAME_OSX;
  }
  elsif( $platform eq 'iPhoneOS' || $platform eq 'iPhoneSimulator' )
  {
    $system = CMAKE_SYSTEM_NAME_IOS;
  }
  elsif( $platform eq 'Android' )
  {
    $system = CMAKE_SYSTEM_NAME_ANDROID;
  }
  else
  {
    $system = CMAKE_SYSTEM_NAME_GENERIC;
  }

  return( $system );
}

sub enumerateAppleTargets 
{
  my $toolchainLocation = $ENV{ 'APPLE_TOOLCHAIN_DIR' };

  &printDebug( "Using Apple toolchain directory: '".$toolchainLocation."'" );

  my $platformDirectory = $toolchainLocation."/Platforms";

  if( opendir( PLATFORMS, $platformDirectory ) )
  {
    foreach my $directory ( readdir( PLATFORMS ) )
    {
      next if( $directory =~ /^(\.|\.\.)$/ ); 
      next if( $directory !~ /\.platform$/ );

      if( $directory =~ /^(.*?)\.platform$/ )
      {
        my $platform      = $1;
        my $binDirectory  = $toolchainLocation."/".DEFAULT_APPLE_BIN_DIR_REL;

        &printDebug( "\tExamining platform: '".$platform."'" );

        my $parameters =  {  
                            BIN_DIRECTORY =>  $binDirectory,
                            SYSTEM_NAME   =>  &getSystemName( $platform ),
                            C_COMPILER    =>  $binDirectory.'/gcc',
                            CXX_COMPILER  =>  $binDirectory.'/g++',
                          };

        &enumerateAppleDeploymentTargets (
                                            $platform,
                                            $platformDirectory.'/'.$directory,
                                            $parameters
                                         );
      }
    }
      
    closedir( PLATFORMS );
  }
  else
  {
    &printUsageAndExit  (
                      ERROR_APPLE_TOOLCHAIN_DIR,
                      "Could not open directory '".$toolchainLocation."': ".$!
                        );
  }
}

sub checkInstallDirectory
{
  if( ! -e $INSTALL_DIRECTORY || ! -d $INSTALL_DIRECTORY )
  {
    return( "'".$INSTALL_DIRECTORY."' does not exist or is not a directory.\n" );
  }

  return( "" );
}

sub checkBuildDirectory
{
  if( ! -e $BUILD_DIRECTORY || ! -d $BUILD_DIRECTORY )
  {
    return( "'".$BUILD_DIRECTORY."' does not exist or is not a directory.\n" );
  }

  return( "" );
}

sub checkSourceDirectory
{
  if( ! -e $SOURCE_DIRECTORY || ! -d $SOURCE_DIRECTORY )
  {
    return( "'".$SOURCE_DIRECTORY."' does not exist or is not a directory.\n" );
  }
  elsif( ! -e $SOURCE_DIRECTORY."/CMakeLists.txt" )
  {
    return( "'".$SOURCE_DIRECTORY."/CMakeLists.txt' does not exist.\n" );
  }

  return( "" );
}

sub main
{
  my $error;

  Getopt::Mixed::init( $COMMAND_LINE_OPTIONS );

  while( my ( $option, $value ) = Getopt::Mixed::nextOption () )
  {
    switch( $option )
    {
      case "p"
      {
        $PLATFORM = $value;
      }
      case "t"
      {
        $TARGET = $value;
      }
      case "a"
      {
        $ARCHITECTURE = $value;
      }
      case "m"
      {
        $MODE = $value;
      }
      case "g"
      {
        $GENERATOR = $value;
      }
      case "s"
      {
        $SOURCE_DIRECTORY = glob( $value );
      }
      case "b"
      {
        $BUILD_DIRECTORY = glob( $value );
      }
      case "i"
      {
        $INSTALL_DIRECTORY = glob( $value );
      }
      case "c"
      {
        $CACHE_FILE = glob( $value );
      }
      case "d"
      {
        $DEBUG = 1;
      }
      case "h"
      {
        &printUsageAndExit( ERROR_SUCCESS );
      }
    }
  }

  if( ! defined( $BUILD_DIRECTORY ) )
  {
    $BUILD_DIRECTORY = $SOURCE_DIRECTORY;
  }

  if( $SOURCE_DIRECTORY )
  {
    &printDebug( "Source directory: ".$SOURCE_DIRECTORY )
  }

  if( $INSTALL_DIRECTORY ) 
  {
    &printDebug( "Install directory: ".$INSTALL_DIRECTORY );
  }

  &printDebug( "Build directory: ".$BUILD_DIRECTORY ) if( $BUILD_DIRECTORY );
  &printDebug( "Cache file: ".$CACHE_FILE ) if( $CACHE_FILE );
  &printDebug( "Mode: ".$MODE ) if( $MODE );
  &printDebug( "Architecture: ".$ARCHITECTURE ) if( $ARCHITECTURE );
  &printDebug( "Platform: ".$PLATFORM ) if( $PLATFORM );
  &printDebug( "Target: ".$TARGET ) if( $TARGET );
  &printDebug( "Generator: ".$GENERATOR ) if( $GENERATOR );
  &printDebug( "Debug: ".$DEBUG ) if( $DEBUG );

  Getopt::Mixed::cleanup();

  if  (
        ! defined( $MODE )
        || ! defined( $PLATFORM )
        || ! defined( $SOURCE_DIRECTORY )
      )
  {
    print "ERROR: Mode not set.\n" unless( $MODE );
    print "ERROR: Platform not set.\n" unless( $PLATFORM );
    print "ERROR: Source directory not set.\n" unless( $SOURCE_DIRECTORY );

    &printUsageAndExit( ERROR_PARSING_CMD_LINE );
  }
  elsif( ( $error = &checkSourceDirectory() ) )
  {
    &printUsageAndExit( ERROR_SOURCE, $error );
  }
  elsif( ( $error = &checkBuildDirectory() ) )
  {
    &printUsageAndExit( ERROR_BUILD, $error );
  }
  elsif(  defined( $INSTALL_DIRECTORY )
          && ( $error = &checkInstallDirectory() )
       )
  {
    &printUsageAndExit( ERROR_INSTALL, $error );
  }
  elsif( ! defined( $MODES{ $MODE } ) )
  {
    &printUsageAndExit( ERROR_MODE, "Unrecognized mode '".$MODE."'" );
  }

  if( ! defined( $ENV{ 'DEVELOPER_ROOT' } ) )
  {
    print   (
      "WARNING: Environment variable 'DEVELOPER_ROOT' is not set.".
      " Android build targets will not be available.\n"
            );
  }

  if( ! defined( $ENV{ 'APPLE_TOOLCHAIN_DIR' } ) )
  {
    print   (
        "WARNING: Environment variable 'APPLE_TOOLCHAIN_DIR' is not set.".
        " Mac OS X and iOS build targets will not be available.\n"
            );
  }

  if( ! defined( $ENV{ 'ANDROID_TOOLCHAIN_DIR' } ) )
  {
    print   (
        "WARNING: Environment variable 'ANDROID_TOOLCHAIN_DIR' is not set.".
        " Android build targets will not be available.\n"
            );
  }

  if( ! defined( $ENV{ 'WINDOWS_TOOLCHAIN_DIR' } ) )
  {
    print   (
        "WARNING: Environment variable 'WINDOWS_TOOLCHAIN_DIR' is not set.".
        " Windows build targets will not be available.\n"
            );
  }

  &enumerateBuildTargets();

  if( $PLATFORM eq '' )
  {
    &printBuildTargets();    
  }
  else
  {
    &buildTargets();
  }

  exit( ERROR_SUCCESS );
}

sub buildTargets
{
  my $buildCount  = 0;
  my @platforms   = sort { $a cmp $b } keys( %TARGETS );

  foreach my $platform ( @platforms )
  {
    next unless( $PLATFORM eq 'all' || $platform eq $PLATFORM );

    my @targets = sort { $a cmp $b } keys( %{ $TARGETS{ $platform } } );

    foreach my $target ( @targets )
    {
      next unless( ! defined( $TARGET ) || $target eq $TARGET );

      my @architectures =
        sort
          { $a cmp $b }
          keys( %{ $TARGETS{ $platform }->{ $target } } );

      foreach my $architecture ( @architectures )
      {
        next unless (
                      ! defined( $ARCHITECTURE )
                      || $architecture eq $ARCHITECTURE
                    );

        &build( $platform, $target, $architecture );

        $buildCount++;
      }
    }
  }

  print "Built for ".$buildCount." deployment targets.";
}

sub build
{
  my ( $platform, $target, $architecture ) = @_;

  my $parametersHash = $TARGETS{ $platform }->{ $target }->{ $architecture };

  my $systemName  = $parametersHash->{ SYSTEM_NAME };

  my $CC  = $parametersHash->{ C_COMPILER };
  my $CXX = $parametersHash->{ CXX_COMPILER };

  my $version = $parametersHash->{ VERSION };

  my @C_FLAGS  = (
    "-isysroot ".$parametersHash->{ SYS_ROOT },
                );

  push( @C_FLAGS, @{ $parametersHash->{ C_FLAGS } } );

  my @CXX_FLAGS  = (
    "-isysroot ".$parametersHash->{ SYS_ROOT },
                  );

  push( @CXX_FLAGS, @{ $parametersHash->{ CXX_FLAGS } } );

  my @LD_FLAGS  = (
    "-isysroot ".$parametersHash->{ SYS_ROOT },
                  );

  push( @LD_FLAGS, @{ $parametersHash->{ LD_FLAGS } } );

  my $generator = $GENERATOR;

  $generator =~ s/\s+/_/g;

  my $buildDirectory =
    lc  (
          'build-'.$generator.'-'.$platform.'-'.$target.'-'.
          $architecture.'-'.$MODE
        );

  my @cmakeParameters = (
    "-G \"".$GENERATOR."\"",
    "\"-DCMAKE_CROSS_COMPILING=TRUE\"",
    "\"-DCMAKE_SYSTEM_NAME=".$systemName."\"",
    "\"-DCMAKE_SYSTEM_VERSION=".$version."\"",
    "\"-DCMAKE_SYSTEM_PROCESSOR=".$architecture."\"",
    "\"-DCMAKE_AR=".$parametersHash->{ AR }."\"",
    "\"-DCMAKE_C_COMPILER=".$CC."\"",
    "\"-DCMAKE_CXX_COMPILER=".$CXX."\"",
    "\"-DCMAKE_C_FLAGS=".join( ' ', @C_FLAGS )."\"",
    "\"-DCMAKE_CXX_FLAGS=".join( ' ', @CXX_FLAGS )."\"",
    "\"-DCMAKE_MODULE_LINKER_FLAGS=".join( ' ', @LD_FLAGS )."\"",
    "\"-DCMAKE_BUILD_TYPE=".$MODE."\"",
    "\"-DCMAKE_FIND_ROOT_PATH=".$parametersHash->{ SYS_ROOT }."\"",
    "\"-DCMAKE_SYSROOT=".$parametersHash->{ SYS_ROOT }."\"",
    "\"-DCMAKE_FIND_ROOT_PATH_MODE_PROGRAM=NEVER\"",
    "\"-DCMAKE_FIND_ROOT_PATH_MODE_LIBRARY=ONLY\"",
    "\"-DCMAKE_FIND_ROOT_PATH_MODE_INCLUDE=ONLY\"",
    "\"-DPLATFORM=".$platform."\"",
    "\"-DTARGET=".$target."\"",
    "\"-DARCHITECTURE=".$architecture."\"",
    $SOURCE_DIRECTORY
                        );

  &addPlatformSpecificParameters  (
                                    $platform,
                                    $target,
                                    $architecture,
                                    \@cmakeParameters
                                  );

  &addGeneratorSpecificParameters (
                                    $platform,
                                    $target,
                                    $architecture,
                                    \@cmakeParameters
                                  );

  if( defined( $INSTALL_DIRECTORY ) )
  {
    push  (
            @cmakeParameters, 
            "\"-DCMAKE_INSTALL_PREFIX=".$INSTALL_DIRECTORY."\""
          );
  }

  if( defined( $CACHE_FILE ) )
  {
    push  (
            @cmakeParameters,
            "\"-C".$CACHE_FILE."\""
          );
  }

  my $cmakeParameters = join( ' ', @cmakeParameters );

  &printDebug( "CMake parameters:" );
  foreach my $parameter ( @cmakeParameters )
  {
    &printDebug( "\t".$parameter );
  }

  print "Generating Makefile for: ".$buildDirectory."\n";

  my $cmakeFlags  = "";

  $cmakeFlags .=  '--debug-output' if( $DEBUG );

  my $script =<<SCRIPT;
mkdir -p $BUILD_DIRECTORY/$buildDirectory;
cd $BUILD_DIRECTORY/$buildDirectory;
cmake $cmakeFlags $cmakeParameters;
SCRIPT

  print $script."\n";
 
  my $output = `$script`;

  print $output;
  
  &logConfig( $buildDirectory, \@cmakeParameters );
}

sub logConfig
{
  my $buildDirectory  = shift;
  my $parameters      = shift;

  my $configurationFile = $BUILD_DIRECTORY."/".$buildDirectory."/cmake.conf";

  if( open( CONF, ">".$configurationFile ) )
  {
    foreach my $param ( @{ $parameters } )
    {
      print CONF $param."\n";
    }

    close( CONF );
  }
  else
  {
    print "ERROR: Could not create configuration file ".
          "('".$configurationFile."'): ".$!;
  }
}

sub addPlatformSpecificParameters
{
  my $platform      = shift;
  my $target        = shift;
  my $architecture  = shift;
  my $parameters    = shift;

  my $parametersHash = $TARGETS{ $platform }->{ $target }->{ $architecture };

  if( $platform eq 'MacOSX' || $platform eq 'iPhoneOS' )
  {
    push  (
            @{ $parameters },
            "\"-DCMAKE_OSX_ARCHITECTURES=".$architecture."\""
          );

    push  (
            @{ $parameters },
            "\"-DCMAKE_OSX_SYSROOT=".$parametersHash->{ SYS_ROOT }."\""
          );
  }
  elsif( $platform eq "Generic" && $target =~ /^android/ )
  {
    push  (
            @{ $parameters },
            "\"-DTARGET_SUPPORTS_SHARED_LIBS=TRUE\""
          );
  }
}

sub addGeneratorSpecificParameters
{
  my $platform      = shift;
  my $target        = shift;
  my $architecture  = shift;
  my $parameters    = shift;

  my $parametersHash = $TARGETS{ $platform }->{ $target }->{ $architecture };

  if( $GENERATOR eq 'Xcode' )
  {
    push  (
            @{ $parameters },
            "\"-DCMAKE_XCODE_EFFECTIVE_PLATFORMS=-".lc( $platform )."\"",
            "\"-DCMAKE_C_COMPILER_WORKS=TRUE\"",
            "\"-DCMAKE_CXX_COMPILER_WORKS=TRUE\"",
          );
  }
  elsif( $GENERATOR =~ /^Eclipse/ )
  {
    push  (
            @{ $parameters },
            "\"-DCMAKE_ECLIPSE_VERSION=4.4\""
          );

    if( $SOURCE_DIRECTORY ne $BUILD_DIRECTORY )
    {
      push  (
              @{ $parameters },
              "\"-DCMAKE_ECLIPSE_GENERATE_SOURCE_PROJECT=FALSE\""
            );
    }
  }
}

sub printBuildTargets
{
  my @platforms = sort { $a cmp $b } keys( %TARGETS );

  print $USAGE."\n";
  &printModes();
  print "\n  Available build targets:\n";
  print "    --platform=all\n";

  foreach my $platform ( @platforms )
  {
    print "    --platform=".$platform."\n";

    my @targets = sort { $a cmp $b } keys( %{ $TARGETS{ $platform } } );

    foreach my $target ( @targets )
    {
      next if( $target eq 'PARAMETERS' );

      print "    --platform=".$platform." --target=".$target."\n";

      my @architectures =
        sort
          { $a cmp $b }
          keys( %{ $TARGETS{ $platform }->{ $target } } );

      foreach my $architecture ( @architectures )
      {
        print "    --platform=".$platform." --target=".
              $target." --architecture=".$architecture."\n";
      }
    }
  }
}

sub printDebug
{
  my $message = shift;

  if( $DEBUG )
  {
    printf( "%s\n", $message );
  }
}

sub printModes
{
  my @modes = sort { $a cmp $b } keys( %MODES );

  print "\n  Available modes:\n";

  foreach my $mode ( @modes )
  {
    print "    ".$mode."\n";
  }
}

sub printUsageAndExit
{
  my ( $exitCode, $exitMessage ) = @_;

  print "Error: ".$exitMessage."\n" if( $exitMessage );
  print $USAGE;

  &printModes();

  exit( $exitCode ); 
}

&main();
