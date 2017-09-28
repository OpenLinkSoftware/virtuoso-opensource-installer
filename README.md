# Virtuoso Open Source Installer

##Additional files for Git https://github.com/openlink/virtuoso-opensource.git
for create installer packages

Windows:

1. Check out https://github.com/openlink/virtuoso-opensource.git

2. Open virtuoso-opensource.sln with VS2017 and build new binary

3. Add installer sources from https://github.com/OpenLinkSoftware/virtuoso-opensource-installer.git

4. Open ~/windows/distr_src/cppdemo/cppdemo.sln and build cppdemo.exe

5. Copy cppdemo.exe to ~/windows/distr_src

6. Copy file vcredist_x64.exe from  Microsoft Visual C++ Redistributable for Visual Studio 2017
   to ~/windows/distr_scr/
   https://support.microsoft.com/en-gb/help/2977003/the-latest-supported-visual-c-downloads

7. Run script ~/windows/mkdist.bat
   It will create directory ~/windows/installer and move all required files to it.

8. Open file ~/windows/installer/OpenLink_VOS7.iss with Inno Setup tool and run `Compile` .
   It will create installer ~/windows/installer/Virtuoso_OpenSource_Server_7.20.x64.exe
