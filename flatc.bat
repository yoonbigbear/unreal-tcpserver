
rmdir /s /q packet
flatc.exe -c -b -o packet fbs/protocol.fbs
flatc.exe -c -b -o packet fbs/result_code.fbs
flatc.exe -c -b -o packet fbs/common.fbs
flatc.exe -c -b -o packet fbs/account.fbs
pause