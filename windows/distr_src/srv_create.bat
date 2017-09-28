SET VOS_PATH=%~dp0
"%VOS_PATH%\bin\virtuoso.exe" +service create +instance vos +configfile "%VOS_PATH%\database\virtuoso.ini"
"%VOS_PATH%\bin\virtuoso.exe" +service start +instance vos
pause