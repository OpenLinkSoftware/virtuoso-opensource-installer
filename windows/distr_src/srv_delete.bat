SET VOS_PATH=%~dp0

echo "SHUTDOWN;" | "%VOS_PATH%\bin\isql.exe" localhost
timeout /T 2
"%VOS_PATH%\bin\virtuoso.exe" +service stop +instance vos
"%VOS_PATH%\bin\virtuoso.exe" -I vos -S delete
pause

