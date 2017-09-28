SET WIN_BUILD_PATH=%~dp0

cd ..

SET VOS_ROOT_PATH=%cd%
SET VSP_PATH=%VOS_ROOT_PATH%\binsrc\vsp

SET SDK_PATH=%VOS_ROOT_PATH%\SDK\x64

cd %WIN_BUILD_PATH%
rd /S /Q .\installer\vos

SET INSTALLER_PATH=%WIN_BUILD_PATH%\installer
SET DST_PATH=%WIN_BUILD_PATH%\installer\vos

md .\installer\vos\uninstall
md .\installer\vos\bin
md .\installer\vos\database
md .\installer\vos\doc
md .\installer\vos\hosting\php
md .\installer\vos\lib\hibernate
md .\installer\vos\lib\jdbc-3.0
md .\installer\vos\lib\jdbc-4.0
md .\installer\vos\lib\jdbc-4.1
md .\installer\vos\lib\jdbc-4.2
md .\installer\vos\lib\jena
md .\installer\vos\lib\jena2
md .\installer\vos\lib\jena3
md .\installer\vos\lib\sesame2
md .\installer\vos\lib\sesame3
md .\installer\vos\lib\sesame4
md .\installer\vos\lib\rdf4j
md .\installer\vos\vad
md .\installer\vos\vsp\admin
md .\installer\vos\vsp\images
md .\installer\vos\vsp\vsmx



cd %WIN_BUILD_PATH%\x64\Release\

copy %WIN_BUILD_PATH%\distr_src\vcredist_x64.exe         %DST_PATH%
copy %WIN_BUILD_PATH%\distr_src\Documentation.url        %DST_PATH%
copy "%WIN_BUILD_PATH%\distr_src\Virtuoso Conductor.url" %DST_PATH%
copy %WIN_BUILD_PATH%\distr_src\_create_srv.bat          %DST_PATH%\uninstall
copy %WIN_BUILD_PATH%\distr_src\_delete_srv.bat          %DST_PATH%\uninstall
copy %WIN_BUILD_PATH%\distr_src\_reg_client_dll.bat      %DST_PATH%\uninstall
copy %WIN_BUILD_PATH%\distr_src\_unreg_client_dll.bat    %DST_PATH%\uninstall

rem ============== bin ==========

copy %SDK_PATH%\sslsdk\out32dll\libeay32.dll  %DST_PATH%\bin
copy %SDK_PATH%\sslsdk\out32dll\ssleay32.dll  %DST_PATH%\bin

copy %SDK_PATH%\wbxml2\expat\libexpat.dll     %DST_PATH%\bin
copy %SDK_PATH%\wbxml2\libwbxml2.dll          %DST_PATH%\bin

copy %SDK_PATH%\php56\php5ts.dll              %DST_PATH%\bin

copy isql.exe %DST_PATH%\bin
copy virtuoso-clr-t.exe      %DST_PATH%\bin
copy virtuoso-javavm-t.exe   %DST_PATH%\bin
copy virtuoso-sample-t.exe   %DST_PATH%\bin
copy virtuoso-t.exe          %DST_PATH%\bin
copy virtuoso-t.exe          %DST_PATH%\bin\virtuoso.exe
copy virtodbc.dll            %DST_PATH%\bin
copy virtoledb.dll           %DST_PATH%\bin
copy libvirtuoso-t.dll       %DST_PATH%\bin

copy %WIN_BUILD_PATH%\distr_src\srv_create.bat       %DST_PATH%\bin
copy %WIN_BUILD_PATH%\distr_src\srv_delete.bat       %DST_PATH%\bin

rem ============== database ==========
copy %WIN_BUILD_PATH%\distr_src\virtuoso.ini         %DST_PATH%\database
copy %WIN_BUILD_PATH%\distr_src\cppdemo.exe          %DST_PATH%\bin



rem ============== doc ==========

copy %VOS_ROOT_PATH%\AUTHORS           %DST_PATH%\doc
copy %VOS_ROOT_PATH%\COPYING           %DST_PATH%\doc
copy %VOS_ROOT_PATH%\CREDITS           %DST_PATH%\doc
copy %VOS_ROOT_PATH%\INSTALL           %DST_PATH%\doc
copy %VOS_ROOT_PATH%\LICENSE           %DST_PATH%\doc
copy %VOS_ROOT_PATH%\NEWS	       %DST_PATH%\doc
copy %VOS_ROOT_PATH%\README            %DST_PATH%\doc
copy %VOS_ROOT_PATH%\README.CVS        %DST_PATH%\doc
copy %VOS_ROOT_PATH%\README.GIT        %DST_PATH%\doc
copy %VOS_ROOT_PATH%\README.UPGRADE    %DST_PATH%\doc	 
copy %VOS_ROOT_PATH%\README.WINDOWS    %DST_PATH%\doc	 
copy %VOS_ROOT_PATH%\README.hibernate  %DST_PATH%\doc
copy %VOS_ROOT_PATH%\README.jena       %DST_PATH%\doc
copy %VOS_ROOT_PATH%\README.jsse       %DST_PATH%\doc
copy %VOS_ROOT_PATH%\README.php5       %DST_PATH%\doc
copy %VOS_ROOT_PATH%\README.sesame2    %DST_PATH%\doc
copy %VOS_ROOT_PATH%\README.sesame3    %DST_PATH%\doc



rem ============== hosting ==========
copy creolewiki.dll          %DST_PATH%\hosting
copy hosting_php.dll         %DST_PATH%\hosting
copy im.dll                  %DST_PATH%\hosting
copy mediawiki.dll           %DST_PATH%\hosting
copy wbxml2.dll              %DST_PATH%\hosting
copy wikiv.dll               %DST_PATH%\hosting

copy %SDK_PATH%\php56\ext\php_bcmath.dll    %DST_PATH%\hosting\php
copy %SDK_PATH%\php56\ext\php_bz2.dll       %DST_PATH%\hosting\php
copy %SDK_PATH%\php56\ext\php_curl.dll      %DST_PATH%\hosting\php
copy %SDK_PATH%\php56\ext\php_exif.dll      %DST_PATH%\hosting\php
copy %SDK_PATH%\php56\ext\php_fileinfo.dll  %DST_PATH%\hosting\php
copy %SDK_PATH%\php56\ext\php_ftp.dll       %DST_PATH%\hosting\php
copy %SDK_PATH%\php56\ext\php_gd2.dll       %DST_PATH%\hosting\php
copy %SDK_PATH%\php56\ext\php_gmp.dll       %DST_PATH%\hosting\php
copy %SDK_PATH%\php56\ext\php_imap.dll      %DST_PATH%\hosting\php
copy %SDK_PATH%\php56\ext\php_ldap.dll      %DST_PATH%\hosting\php
copy %SDK_PATH%\php56\ext\php_mbstring.dll  %DST_PATH%\hosting\php
copy %SDK_PATH%\php56\ext\php_mysql.dll     %DST_PATH%\hosting\php
copy %SDK_PATH%\php56\ext\php_mysqli.dll    %DST_PATH%\hosting\php
copy %SDK_PATH%\php56\ext\php_opcache.dll   %DST_PATH%\hosting\php
copy %SDK_PATH%\php56\ext\php_openssl.dll   %DST_PATH%\hosting\php
copy %SDK_PATH%\php56\ext\php_phar.dll      %DST_PATH%\hosting\php
copy %SDK_PATH%\php56\ext\php_shmop.dll     %DST_PATH%\hosting\php
copy %SDK_PATH%\php56\ext\php_soap.dll      %DST_PATH%\hosting\php
copy %SDK_PATH%\php56\ext\php_sockets.dll   %DST_PATH%\hosting\php
copy %SDK_PATH%\php56\ext\php_sqlite3.dll   %DST_PATH%\hosting\php
copy %SDK_PATH%\php56\ext\php_tidy.dll      %DST_PATH%\hosting\php
copy %SDK_PATH%\php56\ext\php_xmlrpc.dll    %DST_PATH%\hosting\php
copy %SDK_PATH%\php56\ext\php_xsl.dll       %DST_PATH%\hosting\php
copy %SDK_PATH%\php56\ext\php_zip.dll       %DST_PATH%\hosting\php

rem ============== lib ==========

copy virt_http.dll           %DST_PATH%\lib
copy virtclr.dll             %DST_PATH%\lib

copy %WIN_BUILD_PATH%\Release\OpenLink.Data.Virtuoso.dll %DST_PATH%\lib\   

copy %VOS_ROOT_PATH%\binsrc\hibernate\virt_dialect.jar  %DST_PATH%\lib\hibernate\

copy %VOS_ROOT_PATH%\binsrc\jena\virt_jena.jar    %DST_PATH%\lib\jena\
copy %VOS_ROOT_PATH%\binsrc\jena2\virt_jena2.jar  %DST_PATH%\lib\jena2\
copy %VOS_ROOT_PATH%\binsrc\jena3\virt_jena3.jar  %DST_PATH%\lib\jena3\

copy %VOS_ROOT_PATH%\binsrc\sesame2\virt_sesame2.jar    %DST_PATH%\lib\sesame2\
copy %VOS_ROOT_PATH%\binsrc\sesame2\create.xsl          %DST_PATH%\lib\sesame2\
copy %VOS_ROOT_PATH%\binsrc\sesame2\create-virtuoso.xsl %DST_PATH%\lib\sesame2\
copy %VOS_ROOT_PATH%\binsrc\sesame2\virtuoso.ttl        %DST_PATH%\lib\sesame2\

copy %VOS_ROOT_PATH%\binsrc\sesame3\virt_sesame3.jar    %DST_PATH%\lib\sesame3\

copy %VOS_ROOT_PATH%\binsrc\sesame4\virt_sesame4.jar    %DST_PATH%\lib\sesame4\
copy %VOS_ROOT_PATH%\binsrc\sesame4\create.xsl          %DST_PATH%\lib\sesame4\
copy %VOS_ROOT_PATH%\binsrc\sesame4\create-virtuoso.xsl %DST_PATH%\lib\sesame4\

copy %VOS_ROOT_PATH%\binsrc\rdf4j\virt_rdf4j.jar        %DST_PATH%\lib\rdf4j\
copy %VOS_ROOT_PATH%\binsrc\rdf4j\create.xsl            %DST_PATH%\lib\rdf4j\
copy %VOS_ROOT_PATH%\binsrc\rdf4j\create-virtuoso.xsl   %DST_PATH%\lib\rdf4j\

copy %VOS_ROOT_PATH%\libsrc\JDBCDriverType4\virtjdbc3.jar    %DST_PATH%\lib\jdbc-3.0\
copy %VOS_ROOT_PATH%\libsrc\JDBCDriverType4\virtjdbc3ssl.jar %DST_PATH%\lib\jdbc-3.0\

copy %VOS_ROOT_PATH%\libsrc\JDBCDriverType4\virtjdbc4.jar    %DST_PATH%\lib\jdbc-4.0\
copy %VOS_ROOT_PATH%\libsrc\JDBCDriverType4\virtjdbc4ssl.jar %DST_PATH%\lib\jdbc-4.0\

copy %VOS_ROOT_PATH%\libsrc\JDBCDriverType4\virtjdbc4_1.jar    %DST_PATH%\lib\jdbc-4.1\
copy %VOS_ROOT_PATH%\libsrc\JDBCDriverType4\virtjdbc4_1ssl.jar %DST_PATH%\lib\jdbc-4.1\

copy %VOS_ROOT_PATH%\libsrc\JDBCDriverType4\virtjdbc4_2.jar    %DST_PATH%\lib\jdbc-4.2\
copy %VOS_ROOT_PATH%\libsrc\JDBCDriverType4\virtjdbc4_2ssl.jar %DST_PATH%\lib\jdbc-4.2\



rem ============== vad ==========
copy %WIN_BUILD_PATH%\distr_src\vad\*.vad   %DST_PATH%\vad


rem ==============  ==========
copy %VSP_PATH%\admin_style.html        %DST_PATH%\vsp
copy %VSP_PATH%\bugsfixed.html	        %DST_PATH%\vsp
copy %VSP_PATH%\default.css	        %DST_PATH%\vsp
copy %VSP_PATH%\demo-index.html         %DST_PATH%\vsp
copy %VSP_PATH%\downfr.html	        %DST_PATH%\vsp
copy %VSP_PATH%\index.html	        %DST_PATH%\vsp
copy %VSP_PATH%\index1.html	        %DST_PATH%\vsp
copy %VSP_PATH%\knownissues.html        %DST_PATH%\vsp
copy %VSP_PATH%\leftfr.html             %DST_PATH%\vsp
copy %VSP_PATH%\mainfr.html             %DST_PATH%\vsp
copy %VSP_PATH%\newfeatures.html        %DST_PATH%\vsp
copy %VSP_PATH%\releasenotes.css        %DST_PATH%\vsp
copy %VSP_PATH%\releasenotes.html       %DST_PATH%\vsp            
copy %VSP_PATH%\robots.txt              %DST_PATH%\vsp

copy %VSP_PATH%\admin\index_left.vsp    %DST_PATH%\vsp\admin
copy %VSP_PATH%\vsmx\*.*                %DST_PATH%\vsp\vsmx
copy %VSP_PATH%\images\*.*              %DST_PATH%\vsp\images

rem ====== InnoSetup Installer script ==============
copy %WIN_BUILD_PATH%\distr_src\OpenLink_VOS7.iss   %INSTALLER_PATH%\

cd %WIN_BUILD_PATH%