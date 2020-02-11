# Check if environment exists, if not create
if ( -not (Test-Path -Path 'Environment' -PathType Container) ) {
    $ScriptToRun= $PSScriptRoot+"\SetupEnvironment.ps1"
    &$ScriptToRun
}

# Generate solution for visual studio 2017
mkdir -p SLN
cd SLN 
.\..\Environment\Applications\cmake\cmake-3.16.4-win64-x64\bin\cmake -G "Visual Studio 15" ..
cd ..