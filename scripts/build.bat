@ECHO OFF

SetLocal EnableDelayedExpansion

pushd ..\src
SET files=
FOR /R %%f in (*.cpp) do (
    SET files=!files! %%f
)
popd

pushd ..\
SET vendorFiles=vendor/stb_image/*.cpp vendor/glad/src/*.c
SET includeDirs=-Ivendor/glm/ -Isrc -Ivendor/Glad/include -Ivendor/GLFW/include -Ivendor/stb_image/
SET linkerFlags=-lkernel32 -luser32 -lgdi32 -lwinspool -lcomdlg32 -ladvapi32 -lshell32 -lole32 -loleaut32 -luuid -lodbc32 -lopengl32 -Lvendor/GLFW/lib -lglfw3 -lmsvcrtd.lib
SET compilerFlags=-g -Wvarargs -Wall -o bin/snake.exe 
SET defines=-D_DEBUG -DGLFW_INCLUDE_NONE 

echo Building repository...

clang++ %compilerFlags% %files% %vendorFiles% %defines% %includeDirs% %linkerFlags% %compilerFlags%
popd

if %ERRORLEVEL% NEQ 0 (echo Error:%ERRORLEVEL% && exit)

echo Built repository successfully.
