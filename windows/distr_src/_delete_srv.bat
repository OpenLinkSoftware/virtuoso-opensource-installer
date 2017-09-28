echo "SHUTDOWN;" | "%*\bin\isql.exe" localhost:1111
timeout /T 2
"%*\bin\virtuoso-t.exe" +service stop +instance vos
"%*\bin\virtuoso-t.exe" -I vos -S delete
timeout /T 5

