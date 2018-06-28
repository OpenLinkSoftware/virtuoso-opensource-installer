#define AppVersion "7.2"
#define AppVersionFull "7.2"
#define AppCopyright="Copyright (C) 1998-2018 OpenLink Software"
#define AppId="{36EC02B5-E3FB-402C-858A-8EF4ADDD6EFC}"
;#define AppVersion GetFileVersion(AddBackslash(SourcePath) + "vos\bin\virtuoso-t.exe")

#define bin_path "..\x64\Release"
#define bin_net_path "..\Release"
#define src_path "..\.."
#define sdk_path "..\..\SDK\x64"
#define support_path "..\distr_src"


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
OutputDir=.
Compression=lzma2
SolidCompression=yes
;UninstallDisplayIcon={app}\MyProg.exe
ArchitecturesInstallIn64BitMode=x64
ArchitecturesAllowed=x64
AlwaysShowDirOnReadyPage=True
AppId={{#AppId}
PrivilegesRequired=admin
DisableStartupPrompt=False
LicenseFile={#src_path}\LICENSE
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


[Registry]
Root: HKLM; Subkey: "SOFTWARE\Microsoft\Windows NT\CurrentVersion\AppCompatFlags\Layers"; \
    ValueType: String; ValueName: "{app}\bin\virtSCM.exe"; ValueData: "RUNASADMIN"; \
    Flags: uninsdeletekeyifempty uninsdeletevalue
Root: HKCU; Subkey: "SOFTWARE\Microsoft\Windows NT\CurrentVersion\AppCompatFlags\Layers"; \
    ValueType: String; ValueName: "{app}\bin\virtSCM.exe"; ValueData: "RUNASADMIN"; \
    Flags: uninsdeletekeyifempty uninsdeletevalue


[Files]
Source: "{#support_path}\srv_create.bat"; DestDir: "{app}"; Flags: 64bit; Components: srv_comp; Tasks: vos; Check: IsWin64
Source: "{#support_path}\srv_delete.bat"; DestDir: "{app}"; Flags: 64bit; Components: srv_comp; Tasks: vos; Check: IsWin64
Source: "{#support_path}\vcredist_x64.exe"; DestDir: "{app}"; Flags: 64bit; Components: cln_comp srv_comp; Check: IsWin64
Source: "{#support_path}\Documentation.url"; DestDir: "{app}"; Flags: 64bit; Components: cln_comp srv_comp; Check: IsWin64
Source: "{#support_path}\Virtuoso Conductor.url"; DestDir: "{app}"; Flags: 64bit; Components: srv_comp; Tasks: vos; Check: IsWin64

Source: "{#support_path}\cppdemo.exe"; DestDir: "{app}\bin"; Flags: 64bit; Components: cln_comp; Check: IsWin64
Source: "{#support_path}\virtSCM.exe"; DestDir: "{app}\bin"; Flags: 64bit; Components: srv_comp; Check: IsWin64
Source: "{#bin_path}\isql.exe"; DestDir: "{app}\bin"; Flags: 64bit; Components: srv_comp; Check: IsWin64
Source: "{#bin_path}\virtuoso-clr-t.exe"; DestDir: "{app}\bin"; Flags: 64bit; Components: srv_comp; Check: IsWin64
Source: "{#bin_path}\virtuoso-javavm-t.exe"; DestDir: "{app}\bin"; Flags: 64bit; Components: srv_comp; Check: IsWin64
Source: "{#bin_path}\virtuoso-sample-t.exe"; DestDir: "{app}\bin"; Flags: 64bit; Components: srv_comp; Check: IsWin64
Source: "{#bin_path}\virtuoso-t.exe"; DestDir: "{app}\bin"; Flags: 64bit; Components: srv_comp; Check: IsWin64
Source: "{#bin_path}\virtuoso-t.exe"; DestDir: "{app}\bin"; DestName: "virtuoso.exe"; Flags: 64bit; Components: srv_comp; Check: IsWin64

Source: "{#bin_path}\libvirtuoso-t.dll"; DestDir: "{app}\bin"; Flags: recursesubdirs 64bit; Components: srv_comp; Check: IsWin64
Source: "{#bin_path}\virtodbc.dll"; DestDir: "{app}\bin"; Flags: 64bit replacesameversion ignoreversion; Components: cln_comp; Check: IsWin64
Source: "{#bin_path}\virtoledb.dll"; DestDir: "{app}\bin"; Flags: 64bit replacesameversion ignoreversion; Components: cln_comp; Check: IsWin64

Source: "{#sdk_path}\sslsdk\out32dll\libeay32.dll"; DestDir: "{app}\bin"; Flags: 64bit; Components: srv_comp; Check: IsWin64
Source: "{#sdk_path}\sslsdk\out32dll\ssleay32.dll"; DestDir: "{app}\bin"; Flags: 64bit; Components: srv_comp; Check: IsWin64
Source: "{#sdk_path}\wbxml2\expat\libexpat.dll"; DestDir: "{app}\bin"; Flags: 64bit; Components: srv_comp; Check: IsWin64
Source: "{#sdk_path}\wbxml2\libwbxml2.dll"; DestDir: "{app}\bin"; Flags: 64bit; Components: srv_comp; Check: IsWin64
Source: "{#sdk_path}\php56\php5ts.dll"; DestDir: "{app}\bin"; Flags: 64bit; Components: srv_comp; Check: IsWin64

Source: "{#support_path}\virtuoso.ini"; DestDir: "{app}\database"; Flags: 64bit; Components: srv_comp; Check: IsWin64

Source: "{#src_path}\AUTHORS"; DestDir: "{app}\doc"; Flags: 64bit; Components: srv_comp cln_comp; Check: IsWin64
Source: "{#src_path}\COPYING"; DestDir: "{app}\doc"; Flags: 64bit; Components: srv_comp cln_comp; Check: IsWin64
Source: "{#src_path}\CREDITS"; DestDir: "{app}\doc"; Flags: 64bit; Components: srv_comp cln_comp; Check: IsWin64
Source: "{#src_path}\INSTALL"; DestDir: "{app}\doc"; Flags: 64bit; Components: srv_comp cln_comp; Check: IsWin64
Source: "{#src_path}\LICENSE"; DestDir: "{app}\doc"; Flags: 64bit; Components: srv_comp cln_comp; Check: IsWin64
Source: "{#src_path}\NEWS"; DestDir: "{app}\doc"; Flags: 64bit; Components: srv_comp cln_comp; Check: IsWin64
Source: "{#src_path}\README"; DestDir: "{app}\doc"; Flags: 64bit; Components: srv_comp cln_comp; Check: IsWin64
Source: "{#src_path}\README.CVS"; DestDir: "{app}\doc"; Flags: 64bit; Components: srv_comp cln_comp; Check: IsWin64
Source: "{#src_path}\README.GIT"; DestDir: "{app}\doc"; Flags: 64bit; Components: srv_comp cln_comp; Check: IsWin64
Source: "{#src_path}\README.UPGRADE"; DestDir: "{app}\doc"; Flags: 64bit; Components: srv_comp cln_comp; Check: IsWin64
Source: "{#src_path}\README.WINDOWS"; DestDir: "{app}\doc"; Flags: 64bit; Components: srv_comp cln_comp; Check: IsWin64
Source: "{#src_path}\README.hibernate"; DestDir: "{app}\doc"; Flags: 64bit; Components: srv_comp cln_comp; Check: IsWin64
Source: "{#src_path}\README.jena"; DestDir: "{app}\doc"; Flags: 64bit; Components: srv_comp cln_comp; Check: IsWin64
Source: "{#src_path}\README.jsse"; DestDir: "{app}\doc"; Flags: 64bit; Components: srv_comp cln_comp; Check: IsWin64
Source: "{#src_path}\README.php5"; DestDir: "{app}\doc"; Flags: 64bit; Components: srv_comp cln_comp; Check: IsWin64
Source: "{#src_path}\README.sesame2"; DestDir: "{app}\doc"; Flags: 64bit; Components: srv_comp cln_comp; Check: IsWin64
Source: "{#src_path}\README.sesame3"; DestDir: "{app}\doc"; Flags: 64bit; Components: srv_comp cln_comp; Check: IsWin64

Source: "{#bin_path}\creolewiki.dll"; DestDir: "{app}\hosting"; Flags: 64bit replacesameversion ignoreversion; Components: srv_comp; Check: IsWin64
Source: "{#bin_path}\hosting_php.dll"; DestDir: "{app}\hosting"; Flags: 64bit replacesameversion ignoreversion; Components: srv_comp; Check: IsWin64
Source: "{#bin_path}\im.dll"; DestDir: "{app}\hosting"; Flags: 64bit replacesameversion ignoreversion; Components: srv_comp; Check: IsWin64
Source: "{#bin_path}\mediawiki.dll"; DestDir: "{app}\hosting"; Flags: 64bit replacesameversion ignoreversion; Components: srv_comp; Check: IsWin64
Source: "{#bin_path}\wbxml2.dll"; DestDir: "{app}\hosting"; Flags: 64bit replacesameversion ignoreversion; Components: srv_comp; Check: IsWin64
Source: "{#bin_path}\wikiv.dll"; DestDir: "{app}\hosting"; Flags: 64bit replacesameversion ignoreversion; Components: srv_comp; Check: IsWin64
Source: "{#sdk_path}\php56\ext\php_bcmath.dll"; DestDir: "{app}\hosting\php"; Flags: 64bit; Components: srv_comp; Check: IsWin64
Source: "{#sdk_path}\php56\ext\php_bz2.dll"; DestDir: "{app}\hosting\php"; Flags: 64bit; Components: srv_comp; Check: IsWin64
Source: "{#sdk_path}\php56\ext\php_curl.dll"; DestDir: "{app}\hosting\php"; Flags: 64bit; Components: srv_comp; Check: IsWin64
Source: "{#sdk_path}\php56\ext\php_exif.dll"; DestDir: "{app}\hosting\php"; Flags: 64bit; Components: srv_comp; Check: IsWin64
Source: "{#sdk_path}\php56\ext\php_fileinfo.dll"; DestDir: "{app}\hosting\php"; Flags: 64bit; Components: srv_comp; Check: IsWin64
Source: "{#sdk_path}\php56\ext\php_ftp.dll"; DestDir: "{app}\hosting\php"; Flags: 64bit; Components: srv_comp; Check: IsWin64
Source: "{#sdk_path}\php56\ext\php_gd2.dll"; DestDir: "{app}\hosting\php"; Flags: 64bit; Components: srv_comp; Check: IsWin64
Source: "{#sdk_path}\php56\ext\php_gmp.dll"; DestDir: "{app}\hosting\php"; Flags: 64bit; Components: srv_comp; Check: IsWin64
Source: "{#sdk_path}\php56\ext\php_imap.dll"; DestDir: "{app}\hosting\php"; Flags: 64bit; Components: srv_comp; Check: IsWin64
Source: "{#sdk_path}\php56\ext\php_ldap.dll"; DestDir: "{app}\hosting\php"; Flags: 64bit; Components: srv_comp; Check: IsWin64
Source: "{#sdk_path}\php56\ext\php_mbstring.dll"; DestDir: "{app}\hosting\php"; Flags: 64bit; Components: srv_comp; Check: IsWin64
Source: "{#sdk_path}\php56\ext\php_mysql.dll"; DestDir: "{app}\hosting\php"; Flags: 64bit; Components: srv_comp; Check: IsWin64
Source: "{#sdk_path}\php56\ext\php_mysqli.dll"; DestDir: "{app}\hosting\php"; Flags: 64bit; Components: srv_comp; Check: IsWin64
Source: "{#sdk_path}\php56\ext\php_opcache.dll"; DestDir: "{app}\hosting\php"; Flags: 64bit; Components: srv_comp; Check: IsWin64
Source: "{#sdk_path}\php56\ext\php_openssl.dll"; DestDir: "{app}\hosting\php"; Flags: 64bit; Components: srv_comp; Check: IsWin64
Source: "{#sdk_path}\php56\ext\php_phar.dll"; DestDir: "{app}\hosting\php"; Flags: 64bit; Components: srv_comp; Check: IsWin64
Source: "{#sdk_path}\php56\ext\php_shmop.dll"; DestDir: "{app}\hosting\php"; Flags: 64bit; Components: srv_comp; Check: IsWin64
Source: "{#sdk_path}\php56\ext\php_soap.dll"; DestDir: "{app}\hosting\php"; Flags: 64bit; Components: srv_comp; Check: IsWin64
Source: "{#sdk_path}\php56\ext\php_sockets.dll"; DestDir: "{app}\hosting\php"; Flags: 64bit; Components: srv_comp; Check: IsWin64
Source: "{#sdk_path}\php56\ext\php_sqlite3.dll"; DestDir: "{app}\hosting\php"; Flags: 64bit; Components: srv_comp; Check: IsWin64
Source: "{#sdk_path}\php56\ext\php_tidy.dll"; DestDir: "{app}\hosting\php"; Flags: 64bit; Components: srv_comp; Check: IsWin64
Source: "{#sdk_path}\php56\ext\php_xmlrpc.dll"; DestDir: "{app}\hosting\php"; Flags: 64bit; Components: srv_comp; Check: IsWin64
Source: "{#sdk_path}\php56\ext\php_xsl.dll"; DestDir: "{app}\hosting\php"; Flags: 64bit; Components: srv_comp; Check: IsWin64
Source: "{#sdk_path}\php56\ext\php_zip.dll"; DestDir: "{app}\hosting\php"; Flags: 64bit; Components: srv_comp; Check: IsWin64

Source: "{#support_path}\vad\*"; DestDir: "{app}\vad"; Flags: recursesubdirs 64bit; Components: srv_comp; Check: IsWin64

Source: "{#src_path}\binsrc\vsp\admin_style.html"; DestDir: "{app}\vsp"; Flags: 64bit; Components: srv_comp; Check: IsWin64
Source: "{#src_path}\binsrc\vsp\bugsfixed.html"; DestDir: "{app}\vsp"; Flags: 64bit; Components: srv_comp; Check: IsWin64
Source: "{#src_path}\binsrc\vsp\default.css"; DestDir: "{app}\vsp"; Flags: 64bit; Components: srv_comp; Check: IsWin64
Source: "{#src_path}\binsrc\vsp\demo-index.html"; DestDir: "{app}\vsp"; Flags: 64bit; Components: srv_comp; Check: IsWin64
Source: "{#src_path}\binsrc\vsp\downfr.html"; DestDir: "{app}\vsp"; Flags: 64bit; Components: srv_comp; Check: IsWin64
Source: "{#src_path}\binsrc\vsp\index.html"; DestDir: "{app}\vsp"; Flags: 64bit; Components: srv_comp; Check: IsWin64
Source: "{#src_path}\binsrc\vsp\index1.html"; DestDir: "{app}\vsp"; Flags: 64bit; Components: srv_comp; Check: IsWin64
Source: "{#src_path}\binsrc\vsp\knownissues.html"; DestDir: "{app}\vsp"; Flags: 64bit; Components: srv_comp; Check: IsWin64
Source: "{#src_path}\binsrc\vsp\leftfr.html"; DestDir: "{app}\vsp"; Flags: 64bit; Components: srv_comp; Check: IsWin64
Source: "{#src_path}\binsrc\vsp\mainfr.html"; DestDir: "{app}\vsp"; Flags: 64bit; Components: srv_comp; Check: IsWin64
Source: "{#src_path}\binsrc\vsp\newfeatures.html"; DestDir: "{app}\vsp"; Flags: 64bit; Components: srv_comp; Check: IsWin64
Source: "{#src_path}\binsrc\vsp\releasenotes.css"; DestDir: "{app}\vsp"; Flags: 64bit; Components: srv_comp; Check: IsWin64
Source: "{#src_path}\binsrc\vsp\releasenotes.html"; DestDir: "{app}\vsp"; Flags: 64bit; Components: srv_comp; Check: IsWin64
Source: "{#src_path}\binsrc\vsp\robots.txt"; DestDir: "{app}\vsp"; Flags: 64bit; Components: srv_comp; Check: IsWin64
Source: "{#src_path}\binsrc\vsp\admin\index_left.vsp"; DestDir: "{app}\vsp\admin"; Flags: 64bit; Components: srv_comp; Check: IsWin64
Source: "{#src_path}\binsrc\vsp\vsmx\*"; DestDir: "{app}\vsp\vsmx"; Flags: recursesubdirs 64bit; Components: srv_comp; Check: IsWin64
Source: "{#src_path}\binsrc\vsp\images\*"; DestDir: "{app}\vsp\images"; Flags: recursesubdirs 64bit; Components: srv_comp; Check: IsWin64


Source: "{#bin_path}\virt_http.dll"; DestDir: "{app}\lib"; Flags: 64bit; Components: srv_comp; Check: IsWin64
Source: "{#bin_path}\virtclr.dll"; DestDir: "{app}\lib"; Flags: 64bit; Components: srv_comp; Check: IsWin64
Source: "{#bin_net_path}\OpenLink.Data.Virtuoso.dll"; DestDir: "{app}\lib"; Flags: 64bit gacinstall; StrongAssemblyName: "OpenLink.Data.Virtuoso"; Components: cln_comp; Check: IsWin64

Source: "{#src_path}\binsrc\hibernate\virt_dialect.jar"; DestDir: "{app}\lib\hibernate"; Flags: 64bit; Components: cln_comp; Check: IsWin64
Source: "{#src_path}\binsrc\jena\virt_jena.jar"; DestDir: "{app}\lib\jena"; Flags: 64bit; Components: cln_comp; Check: IsWin64
Source: "{#src_path}\binsrc\jena2\virt_jena2.jar"; DestDir: "{app}\lib\jena2"; Flags: 64bit; Components: cln_comp; Check: IsWin64
Source: "{#src_path}\binsrc\jena3\virt_jena3.jar"; DestDir: "{app}\lib\jena3"; Flags: 64bit; Components: cln_comp; Check: IsWin64
Source: "{#src_path}\binsrc\rdf4j\virt_rdf4j.jar"; DestDir: "{app}\lib\rdf4j"; Flags: 64bit; Components: cln_comp; Check: IsWin64
Source: "{#src_path}\binsrc\rdf4j\create.xsl"; DestDir: "{app}\lib\rdf4j"; Flags: 64bit; Components: cln_comp; Check: IsWin64
Source: "{#src_path}\binsrc\rdf4j\create-virtuoso.xsl"; DestDir: "{app}\lib\rdf4j"; Flags: 64bit; Components: cln_comp; Check: IsWin64
Source: "{#src_path}\binsrc\sesame2\virt_sesame2.jar"; DestDir: "{app}\lib\sesame2"; Flags: 64bit; Components: cln_comp; Check: IsWin64
Source: "{#src_path}\binsrc\sesame2\create.xsl"; DestDir: "{app}\lib\sesame2"; Flags: 64bit; Components: cln_comp; Check: IsWin64
Source: "{#src_path}\binsrc\sesame2\create-virtuoso.xsl"; DestDir: "{app}\lib\sesame2"; Flags: 64bit; Components: cln_comp; Check: IsWin64
Source: "{#src_path}\binsrc\sesame2\virtuoso.ttl"; DestDir: "{app}\lib\sesame2"; Flags: 64bit; Components: cln_comp; Check: IsWin64
Source: "{#src_path}\binsrc\sesame3\virt_sesame3.jar"; DestDir: "{app}\lib\sesame3"; Flags: 64bit; Components: cln_comp; Check: IsWin64
Source: "{#src_path}\binsrc\sesame4\virt_sesame4.jar"; DestDir: "{app}\lib\sesame4"; Flags: 64bit; Components: cln_comp; Check: IsWin64
Source: "{#src_path}\binsrc\sesame4\create.xsl"; DestDir: "{app}\lib\sesame4"; Flags: 64bit; Components: cln_comp; Check: IsWin64
Source: "{#src_path}\binsrc\sesame4\create-virtuoso.xsl"; DestDir: "{app}\lib\sesame4"; Flags: 64bit; Components: cln_comp; Check: IsWin64
Source: "{#src_path}\libsrc\JDBCDriverType4\virtjdbc3.jar"; DestDir: "{app}\lib\jdbc-3.0"; Flags: 64bit; Components: cln_comp; Check: IsWin64
Source: "{#src_path}\libsrc\JDBCDriverType4\virtjdbc3ssl.jar"; DestDir: "{app}\lib\jdbc-3.0"; Flags: 64bit; Components: cln_comp; Check: IsWin64
Source: "{#src_path}\libsrc\JDBCDriverType4\virtjdbc4.jar"; DestDir: "{app}\lib\jdbc-4.0"; Flags: 64bit; Components: cln_comp; Check: IsWin64
Source: "{#src_path}\libsrc\JDBCDriverType4\virtjdbc4ssl.jar"; DestDir: "{app}\lib\jdbc-4.0"; Flags: 64bit; Components: cln_comp; Check: IsWin64
Source: "{#src_path}\libsrc\JDBCDriverType4\virtjdbc4_1.jar"; DestDir: "{app}\lib\jdbc-4.1"; Flags: 64bit; Components: cln_comp; Check: IsWin64
Source: "{#src_path}\libsrc\JDBCDriverType4\virtjdbc4_1ssl.jar"; DestDir: "{app}\lib\jdbc-4.1"; Flags: 64bit; Components: cln_comp; Check: IsWin64
Source: "{#src_path}\libsrc\JDBCDriverType4\virtjdbc4_2.jar"; DestDir: "{app}\lib\jdbc-4.2"; Flags: 64bit; Components: cln_comp; Check: IsWin64
Source: "{#src_path}\libsrc\JDBCDriverType4\virtjdbc4_2ssl.jar"; DestDir: "{app}\lib\jdbc-4.2"; Flags: 64bit; Components: cln_comp; Check: IsWin64

[Icons]
Name: "{group}\Interactive SQL"; Filename: "{app}\bin\isql.exe"; WorkingDir: "{app}\bin"; Components: srv_comp
Name: "{group}\ODBC C++ Demo"; Filename: "{app}\bin\cppdemo.exe"; WorkingDir: "{app}\bin"; Components: cln_comp
Name: "{group}\Product Documentation"; Filename: "{app}\Documentation.url"; WorkingDir: "{app}"; Components: srv_comp
Name: "{group}\Virtuoso Conductor"; Filename: "{app}\Virtuoso Conductor.url"; WorkingDir: "{app}"; Components: srv_comp
Name: "{group}\Virtuoso Services Control"; Filename: "{app}\bin\virtSCM.exe"; WorkingDir: "{app}\bin"; Components: srv_comp

[Run]
Filename: "{app}\vcredist_x64.exe"; Parameters: "/install /passive"; WorkingDir: "{app}"; Flags: waituntilterminated 64bit runminimized runascurrentuser hidewizard; Description: "Install MS VC++ 2017 Redistributable (x64) - 14.10.25008"; StatusMsg: "Install MS VC++ 2017 Redistributable (x64)"; Components: cln_comp srv_comp; Check: IsWin64
Filename: "regsvr32.exe"; Parameters: "/s ""{app}/bin/virtodbc.dll"""; WorkingDir: "{app}/bin"; Flags: waituntilterminated 64bit runminimized runascurrentuser; Description: "Register Virtuoso ODBC driver"; StatusMsg: "Register Virtuoso ODBC driver"; Components: cln_comp; Check: IsWin64
Filename: "regsvr32.exe"; Parameters: "/s ""{app}/bin/virtoledb.dll"""; WorkingDir: "{app}/bin"; Flags: waituntilterminated 64bit runminimized runascurrentuser; Description: "Register Virtuoso OLEDB driver"; StatusMsg: "Register Virtuoso OLEDB driver"; Components: cln_comp; Check: IsWin64

[UninstallRun]
Filename: "{cmd}"; Parameters: "/C ""taskkill /im virtSCM.exe /f /t"""; Flags: waituntilterminated 64bit runascurrentuser runhidden; Components: srv_comp
Filename: "regsvr32.exe"; Parameters: "/s /u ""{app}/bin/virtodbc.dll"""; WorkingDir: "{app}/bin"; Flags: waituntilterminated 64bit runminimized runascurrentuser; StatusMsg: "Unregister Virtuoso ODBC driver"; Components: cln_comp; Check: IsWin64
Filename: "regsvr32.exe"; Parameters: "/s /u ""{app}/bin/virtoledb.dll"""; WorkingDir: "{app}/bin"; Flags: waituntilterminated 64bit runminimized runascurrentuser; StatusMsg: "Unregister Virtuoso OLEDB driver"; Components: cln_comp; Check: IsWin64

[Code]
#include "services_unicode.iss"

const
  VOS_INSTANCE = 'vos';
  SERVICE_NAME = 'Virtuoso_'+VOS_INSTANCE;
  SERVICE_DISPLAY_NAME = 'OpenLink Virtuoso Server ['+VOS_INSTANCE+']';
  SERVICE_DESCRIPTION = 'Virtuoso '+VOS_INSTANCE+' instance';

procedure CurStepChanged(CurStep: TSetupStep);                                                   
var
  ErrorCode: Integer;
begin
  Log('CurStepChanged(' + IntToStr(Ord(CurStep)) + ') called');

  if IsComponentSelected('srv_comp') then
  begin
    if CurStep = ssInstall then begin
      if ServiceExists(SERVICE_NAME) then begin
        if SimpleQueryService(SERVICE_NAME, True) = SERVICE_RUNNING then begin
          SimpleStopService(SERVICE_NAME, True, False);
        end;
        SimpleDeleteService(SERVICE_NAME);
      end;
    end
    else if CurStep = ssPostInstall then begin
      SimpleCreateService(
        SERVICE_NAME, 
        SERVICE_DISPLAY_NAME, 
        ExpandConstant('{app}\bin\virtuoso.exe')+' -I "'+SERVICE_NAME+'" -c "'+ExpandConstant('{app}\database\virtuoso.ini')+'"',
        SERVICE_AUTO_START, '', '', False, False);

      ShellExec ('', 'sc.exe','description '+SERVICE_NAME+' "'+SERVICE_DESCRIPTION+'"','', SW_HIDE, ewNoWait, ErrorCode);
      SimpleStartService(SERVICE_NAME, True, False);
    end;
  end;
end;

procedure CurUninstallStepChanged(CurUninstallStep: TUninstallStep);
begin
  Log('CurUninstallStepChanged(' + IntToStr(Ord(CurUninstallStep)) + ') called');

    if CurUninstallStep = usUninstall then begin
      if ServiceExists(SERVICE_NAME) then begin
        if SimpleQueryService(SERVICE_NAME, True) = SERVICE_RUNNING then begin
          SimpleStopService(SERVICE_NAME, True, False);
        end;
        SimpleDeleteService(SERVICE_NAME);
      end;
    end;
end;

//
//function InitializeSetup(): Boolean;
//begin
//  Result := True;
//  if RegKeyExists(HKEY_LOCAL_MACHINE,
//       'SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\{#AppId}_is1') or
//     RegKeyExists(HKEY_CURRENT_USER,
//       'SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\{#AppId}_is1') then
//  begin
//    MsgBox('The application is installed already.', mbInformation, MB_OK);
//    Result := False;
//  end;
//end;
//
