# Homework 2 - Floating Point Overflow Checker
Theo Kliewer
CS3339 Spring 2026

## Description
This program takes a loop bound and loop counter as inputs and checks if floating-point overflow will occur. It also displays the IEEE 754 bit representation of the inputs and the overflow threshold if one exists.

## How to Build
g++ -std=c++17 -o fp_overflow_checker fp_overflow_checker.cpp

## How to Run
./fp_overflow_checker <loop_bound> <loop_counter>

Example:
./fp_overflow_checker 1e+08 1.0

## Known Bugs / Limitations
- Only works with positive float values
- Does not handle denormalized floats or special values like inf or NaN
