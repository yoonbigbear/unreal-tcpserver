
rmdir /s /q packet
flatc2.0.exe -c -b -o packet fbs/protocol.fbs
flatc2.0.exe -c -b -o packet fbs/result_code.fbs
flatc2.0.exe -c -b -o packet fbs/common.fbs
flatc2.0.exe -c -b -o packet fbs/account.fbs
flatc2.0.exe -c -b -o packet fbs/world.fbs

flatc.exe -n -b -o client2/packet fbs/protocol.fbs
flatc.exe -n -b -o client2/packet fbs/result_code.fbs
flatc.exe -n -b -o client2/packet fbs/common.fbs
flatc.exe -n -b -o client2/packet fbs/account.fbs
flatc.exe -n -b -o client2/packet fbs/world.fbs

flatc.exe -n -b -o client2/client2_1/Assets/packet fbs/protocol.fbs
flatc.exe -n -b -o client2/client2_1/Assets/packet fbs/result_code.fbs
flatc.exe -n -b -o client2/client2_1/Assets/packet fbs/common.fbs
flatc.exe -n -b -o client2/client2_1/Assets/packet fbs/account.fbs
flatc.exe -n -b -o client2/client2_1/Assets/packet fbs/world.fbs
pause