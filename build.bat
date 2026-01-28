@echo off
echo ========== 8051编译工具 ==========
echo.

REM 设置Keil路径
set KEIL_PATH=E:\keil_c51\C51\BIN
set C51=%KEIL_PATH%\C51.EXE
set BL51=%KEIL_PATH%\BL51.EXE
set OH51=%KEIL_PATH%\OH51.EXE

REM 检查Keil路径
if not exist "%C51%" (
    echo 错误: 未找到Keil C51编译器!
    echo 请确认Keil安装在 E:\keil_c51
    pause
    exit /b 1
)

REM 清理旧文件
echo 正在清理旧文件...
del *.obj *.lst *.m51 *.hex 2>nul
del public\*.obj public\*.lst 2>nul
del bluetooth\*.obj bluetooth\*.lst 2>nul
del monitor\*.obj monitor\*.lst 2>nul
del sonic\*.obj sonic\*.lst 2>nul
del lcd\*.obj lcd\*.lst 2>nul

REM 编译选项: INCDIR 指定头文件搜索路径
set C51_FLAGS=INCDIR(.)

REM 编译 public.c
echo 正在编译 public\public.c...
%C51% public\public.c %C51_FLAGS% > compile_public.log 2>&1
if errorlevel 1 (
    echo 编译 public.c 失败，查看 compile_public.log 获取详情
    type compile_public.log
    pause
    exit /b 1
)

REM 编译 lcd.c
echo 正在编译 lcd\lcd.c...
%C51% lcd\lcd.c %C51_FLAGS% > compile_lcd.log 2>&1
if errorlevel 1 (
    echo 编译 lcd.c 失败，查看 compile_lcd.log 获取详情
    type compile_lcd.log
    pause
    exit /b 1
)

REM 编译 bluetooth.c
echo 正在编译 bluetooth\bluetooth.c...
%C51% bluetooth\bluetooth.c %C51_FLAGS% > compile_bluetooth.log 2>&1
if errorlevel 1 (
    echo 编译 bluetooth.c 失败，查看 compile_bluetooth.log 获取详情
    type compile_bluetooth.log
    pause
    exit /b 1
)

REM 编译 monitor.c
echo 正在编译 monitor\monitor.c...
%C51% monitor\monitor.c %C51_FLAGS% > compile_monitor.log 2>&1
if errorlevel 1 (
    echo 编译 monitor.c 失败，查看 compile_monitor.log 获取详情
    type compile_monitor.log
    pause
    exit /b 1
)

REM 编译 sonic.c
echo 正在编译 sonic\sonic.c...
%C51% sonic\sonic.c %C51_FLAGS% > compile_sonic.log 2>&1
if errorlevel 1 (
    echo 编译 sonic.c 失败，查看 compile_sonic.log 获取详情
    type compile_sonic.log
    pause
    exit /b 1
)

REM 编译 main.c
echo 正在编译 main.c...
%C51% main.c %C51_FLAGS% > compile_main.log 2>&1
if errorlevel 1 (
    echo 编译 main.c 失败，查看 compile_main.log 获取详情
    type compile_main.log
    pause
    exit /b 1
)

REM 链接所有模块
echo 正在链接...
%BL51% main.obj, public\public.obj, lcd\lcd.obj, bluetooth\bluetooth.obj, monitor\monitor.obj, sonic\sonic.obj TO main > link.log 2>&1
if errorlevel 1 (
    echo 链接失败，查看 link.log 获取详情
    type link.log
    pause
    exit /b 1
)

REM 生成HEX
echo 正在生成HEX文件...
%OH51% main > hex.log 2>&1
if errorlevel 1 (
    echo 生成HEX失败，查看 hex.log 获取详情
    type hex.log
    pause
    exit /b 1
)

echo.
echo ========== 编译成功! ==========
echo 生成的文件:
echo   main.hex - 可烧录文件
echo   main.m51 - 内存映射文件
echo.
if exist main.hex (
    for %%A in (main.hex) do echo   main.hex 大小: %%~zA 字节
)
echo ==============================

echo.
pause