#!/bin/sh

grep '^u16 opr_[0-9][0-9]' opr-functions.c |
sort | uniq |
sed 's/ {$/;/
   1i#pragma once
' > opr-functions.h
