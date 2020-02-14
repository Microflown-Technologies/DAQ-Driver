# Delete olf environment
Remove-Item -LiteralPath "..\Environment" -Force -Recurse

# Create environment dir and CD to it
mkdir -p Environment
mkdir -p Environment\Downloads
mkdir -p Environment\Libraries
mkdir -p Environment\Applications

# Download doxygen from internet
Invoke-WebRequest "http://doxygen.nl/files/doxygen-1.8.17.windows.x64.bin.zip" -OutFile "Environment\Downloads\doxygen.zip"
Expand-Archive -LiteralPath "Environment\Downloads\doxygen.zip" -DestinationPath "Environment\Applications\doxygen"

# Download cmake from internet
Invoke-WebRequest "https://github.com/Kitware/CMake/releases/download/v3.16.4/cmake-3.16.4-win64-x64.zip" -OutFile "Environment\Downloads\cmake.zip"
Expand-Archive -LiteralPath "Environment\Downloads\cmake.zip" -DestinationPath "Environment\Applications\cmake"

# Get protobuf from github
Invoke-WebRequest "https://github.com/protocolbuffers/protobuf/releases/download/v3.11.3/protobuf-cpp-3.11.3.zip" -OutFile "Environment\Downloads\protobuf-cpp.zip" 
Expand-Archive -LiteralPath "Environment\Downloads\protobuf-cpp.zip" -DestinationPath "Environment\Libraries\Protobuf"

