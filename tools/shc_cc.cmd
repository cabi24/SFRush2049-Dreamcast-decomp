@echo off
rem Frozen pilot configuration (2026-10-17): SHC 5.1r11 with the decomp.me base flags minus its extras, plus -speed -noinline.
rem Usage: shc_cc.cmd <source.c> <output.elf>
python -I "%~dp0shc_cc.py" -v shc-v5.1r11 "%~1" -o "%~2" --no-default-flags -- -speed -noinline
