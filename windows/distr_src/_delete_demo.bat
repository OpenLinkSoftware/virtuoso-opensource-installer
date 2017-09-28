echo "SHUTDOWN;" | "%*\bin\isql.exe" localhost:1112
timeout /T 2
"%*\bin\virtuoso-t.exe" +service stop +instance vos_demo
"%*\bin\virtuoso-t.exe" -I vos_demo -S delete
timeout /T 5

