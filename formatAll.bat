for /R %%f in (.\*.cpp .\*.h) do "C:\Program Files\LLVM\bin\clang-format.exe" -style=file -i "%%f"
