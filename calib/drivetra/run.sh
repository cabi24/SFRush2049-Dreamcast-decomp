#!/bin/sh
# usage: sh run.sh <label> [shc flags...]   (default flags: -speed -noinline)
# Copies drivetra.c to the slot file drivetra_tu.c, compiles through run_calib.py, compares per function.
L=$1; shift
[ $# -eq 0 ] && set -- -speed -noinline
D=/c/rush2049-dreamcast/calib
cp $D/drivetra/drivetra.c $D/drivetra/drivetra_tu.c
cd $D && python run_calib.py -v shc-v5.1r11 -s drivetra_tu --src drivetra --label "$L" --no-default -- "$@" || exit 1
python $D/unitcmp.py $D/out/$L/shc-v5.1r11/drivetra_tu.elf drivetrain=8C022CA4:F0 whatslips=8C022D94:168 autoshift=8C022EFC:9A transmission=8C022F96:56 find_best_gear=8C022FEC:5C upshift=8C023048:2C downshift=8C023074:40 enginetorque=8C0230B0:122 interp=8C0231D2:14 engine=8C0231E6:112
