# Check if environment exists, if not create
if ( -not (Test-Path -Path 'Environment' -PathType Container) ) {
    $ScriptToRun= $PSScriptRoot+"\SetupEnvironment.ps1"
    &$ScriptToRun
}

# Prompt output directory

# Generate solution for visual studio 2019
mkdir -p SLN
cd SLN
.\..\Environment\Applications\cmake\cmake-3.16.4-win64-x64\bin\cmake -G "Visual Studio 16" -A x64 $PSScriptRoot\..
cd ..
