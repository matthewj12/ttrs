cd obj && ^
g++ -c ..\src\*.cpp -std=c++17 -g -w -m64 -Wall ^
-I ..\include ^
-I C:\Dev\C++\Libraries\SDL2-w64\include ^
-I C:\Dev\C++\Libraries\SDL2_image\include ^
-I C:\Dev\C++\Libraries\SDL2_ttf\include && ^
g++ *.o -o ..\bin\debug\main.exe ^
-L C:\Dev\C++\Libraries\SDL2-w64\lib ^
-L C:\Dev\C++\Libraries\SDL2_image\lib ^
-L C:\Dev\C++\Libraries\SDL2_ttf\lib ^
-lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf && ^
cd .. && ^
start bin\debug\main.exe