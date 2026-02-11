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

#include "libc/stdlib.h"
#include "kernel/msg/logk.h"
#include "kernel/msg/vsnprintk.h"

int vsnprintk(char *out, size_t out_sz, const char *fmt, va_list args)
{
    if (!out || !fmt) return 0;
    
    char *o = out;
    const char *p = fmt;
    size_t rem = out_sz ? out_sz - 1 : 0;  
    size_t written = 0;
    
    static char tmp_buf[128];
    
    while (*p && rem > 0) {
        if (*p != '%') {
            *o++ = *p++;
            rem--;
            written++;
            continue;
        }
   
        p++;  
        if (!*p) break; 
    
        int long_count = 0;
        while (*p == 'l') {
            long_count++;
            p++;
        }
        
        switch (*p) {
            case 's': {
                const char *s = va_arg(args, const char *);
                if (!s) s = "(null)";

                while (*s && rem > 0) {
                    *o++ = *s++;
                    rem--;
                    written++;
                }
                break;
            }
            
            case 'c': {
                char c = (char)va_arg(args, int);
                if (rem > 0) {
                    *o++ = c;
                    rem--;
                    written++;
                }
                break;
            }
            
            case 'd': case 'i': {
                int64_t value;
                if (long_count >= 2) {
                    value = (int64_t)va_arg(args, long long);
                } else if (long_count == 1) {
                    value = (int64_t)va_arg(args, long);
                } else {
                    value = (int64_t)va_arg(args, int);
                }
                
                itoa(value, tmp_buf, 10, sizeof(tmp_buf));
                size_t l = strlen(tmp_buf);
                if (l > rem) {
                    l = rem;
                }

                for (size_t i = 0; i < l; i++) {

                    if (o >= out + out_sz) {
                        logk(KERN_ERR"vsnprintk: o pointer out of bounds!\n");
                        break;
                    }

                    *o++ = tmp_buf[i];
                }
                rem -= l;
                written += l;
                break;
            }
            
            case 'u': {
                uint64_t value;
                if (long_count >= 2) {
                    value = (uint64_t)va_arg(args, unsigned long long);
                } else if (long_count == 1) {
                    value = (uint64_t)va_arg(args, unsigned long);
                } else {
                    value = (uint64_t)va_arg(args, unsigned int);
                }
                
                uitoa(value, tmp_buf, 10, sizeof(tmp_buf));

                size_t l = strlen(tmp_buf);
                if (l > rem) {
                    l = rem;
                }

                for (size_t i = 0; i < l; i++) {

                    if (o >= out + out_sz) {
                        logk(KERN_ERR"vsnprintk: o pointer out of bounds!\n");
                        break;
                    }

                    *o++ = tmp_buf[i];
                }

                rem -= l;
                written += l;
                break;
            }

            case 'x': case 'X': {
                uint64_t value;
                if (long_count >= 2) {
                    value = (uint64_t)va_arg(args, unsigned long long);
                }
                else if (long_count == 1) {
                    value = (uint64_t)va_arg(args, unsigned long);
                }
                else {
                    value = (uint64_t)va_arg(args, unsigned int);
                }

                uitoa(value, tmp_buf, 16, sizeof(tmp_buf));

                if (*p == 'x') {
                    for (char *c = tmp_buf; *c; c++) {
                        if (*c >= 'A' && *c <= 'F') {
                            *c = *c - 'A' + 'a';
                        }
                    }
                }

                size_t l = strlen(tmp_buf);

                if (l > rem) {
                    l = rem;
                }

                for (size_t i = 0; i < l; i++) {

                    if (o >= out + out_sz) {
                        logk(KERN_ERR"vsnprintk: o pointer out of bounds!\n");
                        break;
                    }

                    *o++ = tmp_buf[i];
                }

                rem -= l;
                written += l;
                break;
            }

            case 'p': {
                void *ptr = va_arg(args, void*);
                uitoa((uint64_t)(uintptr_t)ptr, tmp_buf, 16, sizeof(tmp_buf));

                size_t l = strlen(tmp_buf);
                
                if (l > rem) {
                    l = rem;
                }

                for (size_t i = 0; i < l; i++) {

                    if (o >= out + out_sz) {
                        logk(KERN_ERR"vsnprintk: o pointer out of bounds!\n");
                        break;
                    }

                    *o++ = tmp_buf[i];
                }

                rem -= l;
                written += l;
                break;
            }
            
            case '%': {
                if (rem > 0) {
                    *o++ = '%';
                    rem--;
                    written++;
                }
                break;
            }
            
            default: {
                if (rem > 0) {
                    *o++ = '%';
                    rem--;
                    written++;
                }
                if (rem > 0) {
                    *o++ = *p;
                    rem--;
                    written++;
                }
                break;
            }
        }
        
        p++; 
    }

    if (out_sz > 0) {
        if (written < out_sz) {
            *o = '\0';
        } else {
            out[out_sz - 1] = '\0';
        }
    }
    
    return (int)written;
}