@REM @echo off
setlocal enabledelayedexpansion
set "site="
set "name="

:parse_args
if "%1" == "-s" (
    set "site=%2"
    shift
    shift
    goto parse_args
) else if "%1" == "-n" (
    if "%2" == "" (
        echo Error: -n argument requires a value
        exit /b 1
    )
    set "name=%2"
    shift
    shift
    goto parse_args
)

:ask_again

if "!site!" == "" (
    set /p "site=Enter site: "
    set /p "name=Enter name (optional): "
)


if "!name!" == "" (
    if "!site!" neq "" (
        python cert.py -s !site! > certs.h
    ) else (
        echo "Server address not provided"
        goto ask_again
    )
)
if "!name!" neq ""  (
    if "!site!" neq "" (
        python cert.py -s !site! -n !name! > certs.h
    )
)