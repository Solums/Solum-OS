/*
 * Copyright (C) 2025 Roy Roy123ty@hotmail.com
 * 
 * This file is part of Solum OS
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SOLUM_H
#define SOLUM_H

// enum DEV_STATE (DON'T MODIFY UNLESS YOU KNOW WHAT YOU ARE DOING)
#define DEV_ALPHA       1
#define DEV_BETA        2
#define DEV_RC          3
#define DEV_STABLE      4
#define DEV_LTS         5
// enum DEV_STATE (DON'T MODIFY UNLESS YOU KNOW WHAT YOU ARE DOING)

// for getting rid of incorrect warn from VScode (DON'T MODIFY UNLESS YOU KNOW WHAT YOU ARE DOING)
#ifndef BUILD_DATE
#define BUILD_DATE      " unknown"
#endif
// they actually defined in makefiles (DON'T MODIFY UNLESS YOU KNOW WHAT YOU ARE DOING)

// YOU CAN ONLY MODIFY THERE SAFELY
#define NAME            "Solum OS"
#define CODENAME        "Primordium"    //Primordium(太初),Pneuma(灵息),Morphe(形范),Aether(以太),Symphasis(共融)Apotheosis(神化/臻境)
#define MAJ_VER         1
#define FEA_VER         0
#define PAT_VER         0
#define DEV_STATE       DEV_ALPHA
#define DEV_NUM         1               // alpha[1-N]/beta[1-N]/rc[1-N]

#define TTY_BUFFER_SIZE 4096  
#define MAX_SERIAL_DEVS 4    

#define LOG_SHOW_LEVEL  7
// YOU CAN ONLY MODIFY THERE SAFELY

// translate to str (DON'T MODIFY UNLESS YOU KNOW WHAT YOU ARE DOING)
#define _STR(x) #x
#define STR(x) _STR(x)

#define MAJ_VER_STR STR(MAJ_VER)
#define FEA_VER_STR STR(FEA_VER)
#define PAT_VER_STR STR(PAT_VER)
#define DEV_NUM_STR STR(DEV_NUM)

#if DEV_STATE == DEV_ALPHA
    #define DEV_SYM     "alpha." DEV_NUM_STR
    #define DEV_SYM_USR "Alpha " DEV_NUM_STR
#elif DEV_STATE == DEV_BETA
    #define DEV_SYM     "beta." DEV_NUM_STR
    #define DEV_SYM_USR "Beta " DEV_NUM_STR
#elif DEV_STATE == DEV_RC
    #define DEV_SYM     "rc." DEV_NUM_STR
    #define DEV_SYM_USR "RC " DEV_NUM_STR
#elif DEV_STATE == DEV_STABLE
    #define DEV_SYM     "stable"
    #define DEV_SYM_USR ""
#elif DEV_STATE == DEV_LTS
    #define DEV_SYM     "lts"
    #define DEV_SYM_USR "LTS"
#endif

#define BUILDID         "build" BUILD_DATE

// Base
#if PAT_VER == 0
    #define BASE_VERSION MAJ_VER_STR "." FEA_VER_STR
#else
    #define BASE_VERSION MAJ_VER_STR "." FEA_VER_STR "." PAT_VER_STR
#endif

// Full
#define VER_FUL BASE_VERSION "-" DEV_SYM "+" BUILDID

// Short
#if DEV_STATE == DEV_STABLE
    #define VERSION BASE_VERSION
#else
    #define VERSION BASE_VERSION "-" DEV_SYM
#endif

// USR
#if DEV_STATE == DEV_STABLE
    #define VER_USR CODENAME " " BASE_VERSION
#else
    #define VER_USR CODENAME " " BASE_VERSION " " DEV_SYM_USR
#endif
// translate to str (DON'T MODIFY UNLESS YOU KNOW WHAT YOU ARE DOING)

#endif