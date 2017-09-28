#define AppVersion "7.20"
#define AppVersionFull "7.20.3217.4"
#define AppCopyright="Copyright (C) 1998-2017 OpenLink Software"
;#define AppVersion GetFileVersion(AddBackslash(SourcePath) + "vos\bin\virtuoso-t.exe")



[Setup]
AppName=OpenLink Virtuoso OpenSource Server (x64)(v{#AppVersionFull})
AppVersion={#AppVersion}
AppCopyright={#AppCopyright}
AppPublisher=OpenLink Software
AppPublisherURL=http://virtuoso.openlinksw.com
AppSupportURL=http://virtuoso.openlinksw.com
AppUpdatesURL=http://virtuoso.openlinksw.com
DefaultDirName={pf}\OpenLink Software\Virtuoso OpenSource {#AppVersion}
OutputBaseFilename=Virtuoso_OpenSource_Server_{#AppVersion}.x64
OutputDir=installer
Compression=lzma2
SolidCompression=yes
;UninstallDisplayIcon={app}\MyProg.exe
ArchitecturesInstallIn64BitMode=x64
ArchitecturesAllowed=x64
AlwaysShowDirOnReadyPage=True
AppId={{36EC02B5-E3FB-402C-858A-8EF4ADDD6EFC}
PrivilegesRequired=admin
DisableStartupPrompt=False
LicenseFile=vos\doc\LICENSE
VersionInfoVersion={#AppVersionFull}
VersionInfoCompany=OpenLink Software
VersionInfoTextVersion={#AppVersionFull}
VersionInfoCopyright={#AppCopyright}
VersionInfoProductName=OpenLink Virtuoso OpenSource Server {#AppVersionFull}(x64)
VersionInfoProductVersion={#AppVersionFull}
VersionInfoProductTextVersion={#AppVersionFull}
SetupMutex=OpenLink_VOS,Global\OpenLink_VOS
AllowNoIcons=True
DefaultGroupName=OpenLink Software\Virtuoso OpenSource {#AppVersion}
RestartIfNeededByRun=False

[Types]
Name: "full"; Description: "Full installation"
Name: "server"; Description: "Virtuoso OpenSource Server"
Name: "client"; Description: "Client Connectivity Kit"

[Components]
Name: "srv_comp"; Description: "Virtuoso OpenSource Server"; Types: full server
Name: "cln_comp"; Description: "Client Connectivity Kit"; Types: full client


[Tasks]
Name: "vos"; Description: "Initialize empty DB and start it"; Components: srv_comp

[Files]
Source: "vos\vcredist_x64.exe"; DestDir: "{app}"; Flags: 64bit; Components: cln_comp srv_comp; Check: IsWin64
Source: "vos\Documentation.url"; DestDir: "{app}"; Flags: 64bit; Components: cln_comp srv_comp; Check: IsWin64
Source: "vos\Virtuoso Conductor.url"; DestDir: "{app}"; Flags: 64bit; Components: srv_comp; Tasks: vos; Check: IsWin64
Source: "vos\uninstall\_create_srv.bat"; DestDir: "{app}\uninstall"; Flags: 64bit; Components: srv_comp; Tasks: vos; Check: IsWin64
Source: "vos\uninstall\_delete_srv.bat"; DestDir: "{app}\uninstall"; Flags: 64bit; Components: srv_comp; Tasks: vos; Check: IsWin64
Source: "vos\uninstall\_reg_client_dll.bat"; DestDir: "{app}\uninstall"; Flags: 64bit; Components: cln_comp; Check: IsWin64
Source: "vos\uninstall\_unreg_client_dll.bat"; DestDir: "{app}\uninstall"; Flags: 64bit; Components: cln_comp; Check: IsWin64

Source: "vos\bin\*"; DestDir: "{app}\bin"; Flags: recursesubdirs 64bit; Components: srv_comp; Check: IsWin64
Source: "vos\database\*"; DestDir: "{app}\database"; Flags: recursesubdirs 64bit; Components: srv_comp; Check: IsWin64
Source: "vos\doc\*"; DestDir: "{app}\doc"; Flags: recursesubdirs 64bit; Components: srv_comp cln_comp; Check: IsWin64
Source: "vos\hosting\*"; DestDir: "{app}\hosting"; Flags: recursesubdirs 64bit; Components: srv_comp; Check: IsWin64
Source: "vos\vad\*"; DestDir: "{app}\vad"; Flags: recursesubdirs 64bit; Components: srv_comp; Check: IsWin64
Source: "vos\vsp\*"; DestDir: "{app}\vsp"; Flags: recursesubdirs 64bit; Components: srv_comp; Check: IsWin64

Source: "vos\lib\virt_http.dll"; DestDir: "{app}\lib"; Flags: recursesubdirs 64bit; Components: srv_comp; Check: IsWin64
Source: "vos\lib\virtclr.dll"; DestDir: "{app}\lib"; Flags: recursesubdirs 64bit; Components: srv_comp; Check: IsWin64

Source: "vos\lib\hibernate\*"; DestDir: "{app}\lib\hibernate"; Flags: recursesubdirs 64bit; Components: cln_comp; Check: IsWin64
Source: "vos\lib\jdbc-3.0\*"; DestDir: "{app}\lib\jdbc-3.0"; Flags: recursesubdirs 64bit; Components: cln_comp; Check: IsWin64
Source: "vos\lib\jdbc-4.0\*"; DestDir: "{app}\lib\jdbc-4.0"; Flags: recursesubdirs 64bit; Components: cln_comp; Check: IsWin64
Source: "vos\lib\jdbc-4.1\*"; DestDir: "{app}\lib\jdbc-4.1"; Flags: recursesubdirs 64bit; Components: cln_comp; Check: IsWin64
Source: "vos\lib\jdbc-4.2\*"; DestDir: "{app}\lib\jdbc-4.2"; Flags: recursesubdirs 64bit; Components: cln_comp; Check: IsWin64
Source: "vos\lib\jena\*"; DestDir: "{app}\lib\jena"; Flags: recursesubdirs 64bit; Components: cln_comp; Check: IsWin64
Source: "vos\lib\jena2\*"; DestDir: "{app}\lib\jena2"; Flags: recursesubdirs 64bit; Components: cln_comp; Check: IsWin64
Source: "vos\lib\jena3\*"; DestDir: "{app}\lib\jena3"; Flags: recursesubdirs 64bit; Components: cln_comp; Check: IsWin64
Source: "vos\lib\rdf4j\*"; DestDir: "{app}\lib\rdf4j"; Flags: recursesubdirs 64bit; Components: cln_comp; Check: IsWin64
Source: "vos\lib\sesame2\*"; DestDir: "{app}\lib\sesame2"; Flags: recursesubdirs 64bit; Components: cln_comp; Check: IsWin64
Source: "vos\lib\sesame3\*"; DestDir: "{app}\lib\sesame3"; Flags: recursesubdirs 64bit; Components: cln_comp; Check: IsWin64
Source: "vos\lib\sesame4\*"; DestDir: "{app}\lib\sesame4"; Flags: recursesubdirs 64bit; Components: cln_comp; Check: IsWin64
Source: "vos\lib\OpenLink.Data.Virtuoso.dll"; DestDir: "{app}\lib"; Flags: recursesubdirs 64bit gacinstall; StrongAssemblyName: "OpenLink.Data.Virtuoso"; Components: cln_comp; Check: IsWin64

Source: "vos\bin\libvirtuoso-t.dll"; DestDir: "{app}\bin"; Flags: recursesubdirs 64bit; Components: srv_comp; Check: IsWin64
Source: "vos\bin\virtodbc.dll"; DestDir: "{app}\bin"; Flags: recursesubdirs 64bit replacesameversion ignoreversion; Components: cln_comp; Check: IsWin64
Source: "vos\bin\virtoledb.dll"; DestDir: "{app}\bin"; Flags: recursesubdirs 64bit replacesameversion ignoreversion; Components: cln_comp; Check: IsWin64

[Icons]
Name: "{group}\Interactive SQL"; Filename: "{app}\bin\isql.exe"; WorkingDir: "{app}\bin"
Name: "{group}\ODBC C++ Demo"; Filename: "{app}\bin\cppdemo.exe"; WorkingDir: "{app}\bin"
Name: "{group}\Product Documentation"; Filename: "{app}\Documentation.url"; WorkingDir: "{app}"
Name: "{group}\Virtuoso Conductor"; Filename: "{app}\Virtuoso Conductor.url"; WorkingDir: "{app}"

[Run]
Filename: "{app}\vcredist_x64.exe"; Parameters: "/install /passive"; WorkingDir: "{app}"; Flags: waituntilterminated 64bit runminimized runascurrentuser hidewizard; Description: "Install MS VC++ 2017 Redistributable (x64) - 14.10.25008"; StatusMsg: "Install MS VC++ 2017 Redistributable (x64)"; Components: cln_comp srv_comp; Check: IsWin64
Filename: "{app}\uninstall\_reg_client_dll.bat"; Parameters: "{app}"; WorkingDir: "{app}"; Flags: waituntilterminated 64bit runminimized runascurrentuser; Description: "Register clients DLLs"; StatusMsg: "Register clients DLLs"; Components: cln_comp; Check: IsWin64
Filename: "{app}\uninstall\_create_srv.bat"; Parameters: "{app}"; WorkingDir: "{app}"; Flags: waituntilterminated 64bit runminimized runascurrentuser; Description: "Create Windows service for VOS instance and start it"; StatusMsg: "Create Windows service and start it"; Components: srv_comp; Tasks: vos; Check: IsWin64

[UninstallRun]
Filename: "{app}\uninstall\_delete_srv.bat"; Parameters: "{app}"; WorkingDir: "{app}"; Flags: waituntilterminated 64bit runminimized; Tasks: vos; Check: IsWin64
Filename: "{app}\uninstall\_unreg_client_dll.bat"; Parameters: "{app}"; WorkingDir: "{app}"; Flags: waituntilterminated 64bit runminimized runascurrentuser; StatusMsg: "Unregister clients DLLs"; Components: cln_comp; Check: IsWin64
