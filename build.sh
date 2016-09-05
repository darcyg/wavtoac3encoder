#!/usr/bin/env bash

SCRIPT_DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
TOOLS_DIR=$SCRIPT_DIR/tools
NUGET_EXE=$TOOLS_DIR/nuget.exe
CAKE_EXE=$TOOLS_DIR/Cake/Cake.exe
PACKAGES_CONFIG=$TOOLS_DIR/packages.config
PACKAGES_CONFIG_MD5=$TOOLS_DIR/packages.config.md5sum

MD5_EXE=
if [[ "$(uname -s)" == "Darwin" ]]; then
    MD5_EXE="md5 -r"
else
    MD5_EXE="md5sum"
fi

SCRIPT="build.cake"
TARGET="Default"
VERBOSITY="verbose"
DRYRUN=
SHOW_VERSION=false
SCRIPT_ARGUMENTS=()

for i in "$@"; do
    case $1 in
        -s|--script) SCRIPT="$2"; shift ;;
        -t|--target) TARGET="$2"; shift ;;
        -v|--verbosity) VERBOSITY="$2"; shift ;;
        -d|--dryrun) DRYRUN="-dryrun" ;;
        --version) SHOW_VERSION=true ;;
        --) shift; SCRIPT_ARGUMENTS+=("$@"); break ;;
        *) SCRIPT_ARGUMENTS+=("$1") ;;
    esac
    shift
done

if [ ! -d "$TOOLS_DIR" ]; then
  mkdir "$TOOLS_DIR"
fi

if [ ! -f "$TOOLS_DIR/packages.config" ]; then
    echo "Downloading packages.config..."
    curl -Lsfo "$TOOLS_DIR/packages.config" http://cakebuild.net/download/bootstrapper/packages
    if [ $? -ne 0 ]; then
        echo "An error occured while downloading packages.config."
        exit 1
    fi
fi

if [ ! -f "$NUGET_EXE" ]; then
    echo "Downloading NuGet..."
    curl -Lsfo "$NUGET_EXE" https://dist.nuget.org/win-x86-commandline/latest/nuget.exe
    if [ $? -ne 0 ]; then
        echo "An error occured while downloading nuget.exe."
        exit 1
    fi
fi

pushd "$TOOLS_DIR" >/dev/null
if [ ! -f $PACKAGES_CONFIG_MD5 ] || [ "$( cat $PACKAGES_CONFIG_MD5 | sed 's/\r$//' )" != "$( $MD5_EXE $PACKAGES_CONFIG | awk '{ print $1 }' )" ]; then
    find . -type d ! -name . | xargs rm -rf
fi

mono "$NUGET_EXE" install -ExcludeVersion
if [ $? -ne 0 ]; then
    echo "Could not restore NuGet packages."
    exit 1
fi

$MD5_EXE $PACKAGES_CONFIG | awk '{ print $1 }' >| $PACKAGES_CONFIG_MD5

popd >/dev/null

if [ ! -f "$CAKE_EXE" ]; then
    echo "Could not find Cake.exe at '$CAKE_EXE'."
    exit 1
fi

if $SHOW_VERSION; then
    exec mono "$CAKE_EXE" -version
else
    exec mono "$CAKE_EXE" $SCRIPT -verbosity=$VERBOSITY -target=$TARGET $DRYRUN "${SCRIPT_ARGUMENTS[@]}"
fi
