/*
 * Copyright (C) 2025 Roy Roy123ty@hotmail.com
 * 
 * This file is part of Solum OS
 * 
 * It is free software licensed under the
 * GNU General Public License version 3 (GPLv3).
 * 
 * This file is based on the ViudiraTech Uinxed-Kernel project
 * (https://github.com/ViudiraTech/Uinxed-Kernel), which was licensed under the
 * Apache License, Version 2.0, Neo Aether OS project 
 * (https://github.com/aether-os-studio/naos), which was licensed under the 
 * GNU General Public License version 3 (GPLv3), CoolPot OS project
 * (https://github.com/plos-clan/CoolPotOS), which was licensed under the
 * MIT License.
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

#include "libc/string.h"
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

void *memcpy(void *dest, const void *src, size_t n) {
    if (n == 0 || dest == src)
        return dest;

    uint8_t *d = (uint8_t *)dest;
    const uint8_t *s = (const uint8_t *)src;

    while (n > 0 && ((uintptr_t)d & 7)) {
        *d++ = *s++;
        n--;
    }

    if (((uintptr_t)s & 7) == 0) {
        uint64_t *d64 = (uint64_t *)d;
        const uint64_t *s64 = (const uint64_t *)s;

        while (n >= 8) {
            *d64++ = *s64++;
            n -= 8;
        }

        d = (uint8_t *)d64;
        s = (const uint8_t *)s64;
    }

    while (n--) {
        *d++ = *s++;
    }

    return dest;
}

void *memset(void *dest, int c, size_t n) {
    unsigned char *s = dest;
    size_t         k = 0;

    if (!n) return dest;
    s[0]     = c;
    s[n - 1] = c;
    if (n <= 2) return dest;
    s[1]     = c;
    s[2]     = c;
    s[n - 2] = c;
    s[n - 3] = c;
    if (n <= 6) return dest;
    s[3]     = c;
    s[n - 4] = c;
    if (n <= 8) return dest;

    k  = -(uintptr_t)s & 3;
    s += k;
    n -= k;
    n &= -4;

#ifdef __GNUC__
    typedef uint32_t __attribute__((__may_alias__)) u32;
    typedef uint64_t __attribute__((__may_alias__)) u64;

    u32 c32 = ((u32)-1) / 255 * (unsigned char)c;

    *(u32 *)(s + 0)     = c32;
    *(u32 *)(s + n - 4) = c32;
    if (n <= 8) return dest;
    *(u32 *)(s + 4)      = c32;
    *(u32 *)(s + 8)      = c32;
    *(u32 *)(s + n - 12) = c32;
    *(u32 *)(s + n - 8)  = c32;
    if (n <= 24) return dest;
    *(u32 *)(s + 12)     = c32;
    *(u32 *)(s + 16)     = c32;
    *(u32 *)(s + 20)     = c32;
    *(u32 *)(s + 24)     = c32;
    *(u32 *)(s + n - 28) = c32;
    *(u32 *)(s + n - 24) = c32;
    *(u32 *)(s + n - 20) = c32;
    *(u32 *)(s + n - 16) = c32;

    k  = 24 + ((uintptr_t)s & 4);
    s += k;
    n -= k;

    u64 c64 = c32 | ((u64)c32 << 32);
    for (; n >= 32; n -= 32, s += 32) {
        *(u64 *)(s + 0)  = c64;
        *(u64 *)(s + 8)  = c64;
        *(u64 *)(s + 16) = c64;
        *(u64 *)(s + 24) = c64;
    }
#else
    for (; n; n--, s++)
        *s = c;
#endif

    return dest;
}

void *memmove(void *dest, const void *src, size_t n) {
    unsigned char *d = (unsigned char *)dest;
    const unsigned char *s = (const unsigned char *)src;

    if (d == s || n == 0)
        return dest;

    if (d < s) {
        
        if (n < 16) {
            while (n--)
                *d++ = *s++;
            return dest;
        }

        while (n > 0 && ((uintptr_t)d & 7)) {
            *d++ = *s++;
            n--;
        }

        if (n >= 8 && ((uintptr_t)s & 7) == 0) {
            uint64_t *d64 = (uint64_t *)d;
            const uint64_t *s64 = (const uint64_t *)s;
            
            while (n >= 8) {
                *d64++ = *s64++;
                n -= 8;
            }
            d = (unsigned char *)d64;
            s = (const unsigned char *)s64;
        }

        while (n--)
            *d++ = *s++;
    } 

    else {
        d += n;
        s += n;

        if (n < 16) {
            while (n--)
                *--d = *--s;
            return dest;
        }

        while (n > 0 && ((uintptr_t)d & 7)) {
            *--d = *--s;
            n--;
        }

        if (n >= 8 && ((uintptr_t)s & 7) == 0) {
            uint64_t *d64 = (uint64_t *)d;
            const uint64_t *s64 = (const uint64_t *)s;
            
            while (n >= 8) {
                *--d64 = *--s64;
                n -= 8;
            }
            d = (unsigned char *)d64;
            s = (const unsigned char *)s64;
        }

        while (n--)
            *--d = *--s;
    }

    return dest;
}

int memcmp(const void *s1, const void *s2, size_t n) {
    const uint8_t *p1 = (const uint8_t *)s1;
    const uint8_t *p2 = (const uint8_t *)s2;

    while (n > 0 && ((uintptr_t)p1 & 7) && ((uintptr_t)p2 & 7)) {
        if (*p1 != *p2)
            return *p1 < *p2 ? -1 : 1;
        p1++;
        p2++;
        n--;
    }

    if (n >= 8 && ((uintptr_t)p1 & 7) == 0 && ((uintptr_t)p2 & 7) == 0) {
        const uint64_t *p164 = (const uint64_t *)p1;
        const uint64_t *p264 = (const uint64_t *)p2;
        
        while (n >= 8) {
            if (*p164 != *p264) {
                const uint8_t *c1 = (const uint8_t *)p164;
                const uint8_t *c2 = (const uint8_t *)p264;
                for (size_t i = 0; i < 8; i++) {
                    if (c1[i] != c2[i])
                        return c1[i] < c2[i] ? -1 : 1;
                }
            }
            p164++;
            p264++;
            n -= 8;
        }
        p1 = (const uint8_t *)p164;
        p2 = (const uint8_t *)p264;
    }

    for (size_t i = 0; i < n; i++) {
        if (p1[i] != p2[i])
            return p1[i] < p2[i] ? -1 : 1;
    }

    return 0;
}

void *memchr(const void *s, int c, size_t n) {
    const unsigned char *ptr = (const unsigned char *)s;
    unsigned char ch = (unsigned char)c;
    
    if (n < 8) {
        while (n--) {
            if (*ptr == ch)
                return (void *)ptr;
            ptr++;
        }
        return NULL;
    }

    while (n > 0 && ((uintptr_t)ptr & 7)) {
        if (*ptr == ch)
            return (void *)ptr;
        ptr++;
        n--;
    }

#if __SCHAR_MAX__ == __INT_MAX__
    #define UCHAR_MAX (__SCHAR_MAX__ * 2U + 1U)
#else
    #define UCHAR_MAX (__SCHAR_MAX__ * 2 + 1)
#endif
#define ONES ((size_t)-1 / UCHAR_MAX)
#define HIGHS (ONES * (UCHAR_MAX / 2 + 1))
#define HASZERO(x) ((x) - ONES & ~(x) & HIGHS)

    if (n >= 8) {
        size_t *w = (size_t *)ptr;
        size_t k = ONES * ch;
        
        while (n >= sizeof(size_t) && !HASZERO(*w ^ k)) {
            w++;
            n -= sizeof(size_t);
        }
        ptr = (const unsigned char *)w;
    }

    while (n--) {
        if (*ptr == ch)
            return (void *)ptr;
        ptr++;
    }

    return NULL;
}

size_t strlen(const char *s) {
    const char *p = s;

    while ((uintptr_t)p & 7) {
        if (*p == '\0')
            return p - s;
        p++;
    }

    const uint64_t *p64 = (const uint64_t *)p;
    uint64_t mask = 0x0101010101010101ULL;
    uint64_t highmask = 0x8080808080808080ULL;
    
    for (;;) {
        uint64_t word = *p64++;
        if (((word - mask) & ~word & highmask) != 0) {
            const char *cp = (const char *)(p64 - 1);
            while (*cp != '\0')
                cp++;
            return cp - s;
        }
    }
}

char *strcpy(char *dest, const char *src) {
    char *d = dest;

    while (((uintptr_t)d & 7) && *src) {
        *d++ = *src++;
    }
    
    if (*src == '\0') {
        *d = '\0';
        return dest;
    }

    uint64_t *d64 = (uint64_t *)d;
    const uint64_t *s64 = (const uint64_t *)src;
    
    for (;;) {
        uint64_t word = *s64++;
        *d64++ = word;

        if (((word - 0x0101010101010101ULL) & ~word & 0x8080808080808080ULL) != 0) {
            d = (char *)(d64 - 1);
            while (*d != '\0')
                d++;
            *d = '\0';
            return dest;
        }
    }
}

int strcmp(const char *s1, const char *s2) {

    while (((uintptr_t)s1 & 7) && *s1 && *s2 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    
    if (*s1 == '\0' || *s2 == '\0')
        return (unsigned char)*s1 - (unsigned char)*s2;

    const uint64_t *p1 = (const uint64_t *)s1;
    const uint64_t *p2 = (const uint64_t *)s2;
    
    for (;;) {
        uint64_t a = *p1++;
        uint64_t b = *p2++;
        
        if (a != b) {

            const char *c1 = (const char *)&a;
            const char *c2 = (const char *)&b;
            for (int i = 0; i < 8; i++) {
                if (c1[i] != c2[i] || c1[i] == '\0')
                    return (unsigned char)c1[i] - (unsigned char)c2[i];
            }
        } else {

            if (((a - 0x0101010101010101ULL) & ~a & 0x8080808080808080ULL) != 0) {
               
                return 0;
            }
        }
    }
}

int strncmp(const char *s1, const char *s2, size_t n) {
    if (n == 0)
        return 0;
    

    while (((uintptr_t)s1 & 7) && n && *s1 && *s2 && (*s1 == *s2)) {
        s1++;
        s2++;
        n--;
    }
    
    if (n == 0)
        return 0;

    if (*s1 == '\0' || *s2 == '\0')
        return (unsigned char)*s1 - (unsigned char)*s2;
    
  
    const uint64_t *p1 = (const uint64_t *)s1;
    const uint64_t *p2 = (const uint64_t *)s2;
    
    while (n >= 8) {
        uint64_t a = *p1++;
        uint64_t b = *p2++;
        
        if (a != b) {
         
            const char *c1 = (const char *)&a;
            const char *c2 = (const char *)&b;
            for (int i = 0; i < 8; i++) {
                if (c1[i] != c2[i])
                    return (unsigned char)c1[i] - (unsigned char)c2[i];
            }
        } else {
          
            if (((a - 0x0101010101010101ULL) & ~a & 0x8080808080808080ULL) != 0) {
             
                return 0;
            }
        }
        
        n -= 8;
    }
    
  
    if (n) {
        const char *c1 = (const char *)p1;
        const char *c2 = (const char *)p2;
        for (size_t i = 0; i < n; i++) {
            if (c1[i] != c2[i])
                return (unsigned char)c1[i] - (unsigned char)c2[i];
            if (c1[i] == '\0')
                break;
        }
    }
    
    return 0;
}

char *strstr(const char *haystack, const char *needle) {
    if (!needle[0])
        return (char *)haystack;
    
    if (!needle[1])
        return strchr(haystack, needle[0]);
    
    size_t nlen = 0;
    const char *np = needle;
    while (*np != '\0') {  
        nlen++;
        np++;
    }
    
    const char *h = haystack;
    
    for (; *h != '\0'; ++h) {
        if (*h == needle[0]) {
            const char *h1 = h;
            const char *n1 = needle;
            
            while (*h1 && *n1 && *h1 == *n1) {
                h1++;
                n1++;
            }
            
            if (!*n1)  
                return (char *)h;
        }
    }
    
    return NULL;
}

char *strncpy(char *dest, const char *src, size_t n) {
    char *d = dest;
    
  
    while (n > 0 && *src) {
        *d++ = *src++;
        n--;
    }
    
  
    while (n-- > 0)
        *d++ = '\0';
    
    return dest;
}

char *strcat(char *dest, const char *src) {
    char *d = dest;
    
 
    while (*d)
        d++;
    
  
    strcpy(d, src);
    
    return dest;
}

char *strchr(const char *s, int c) {
    char ch = (char)c;
    

    while ((uintptr_t)s & 7) {
        if (*s == ch)
            return (char *)s;
        if (*s == '\0')
            return NULL;
        s++;
    }
    
   
    const uint64_t *s64 = (const uint64_t *)s;
    uint64_t mask = 0x0101010101010101ULL * (unsigned char)ch;
    uint64_t highmask = 0x8080808080808080ULL;
    
    for (;;) {
        uint64_t word = *s64;
   
        if (((word - 0x0101010101010101ULL) & ~word & highmask) != 0) {
    
            const char *cp = (const char *)s64;
            for (int i = 0; i < 8; i++) {
                if (cp[i] == ch)
                    return (char *)(cp + i);
                if (cp[i] == '\0')
                    return NULL;
            }
        } else {
          
            uint64_t matches = word ^ mask;
            if (((matches - 0x0101010101010101ULL) & ~matches & highmask) != 0) {
                const char *cp = (const char *)s64;
                for (int i = 0; i < 8; i++) {
                    if (cp[i] == ch)
                        return (char *)(cp + i);
                }
            }
        }
        
        s64++;
    }
}

char *strrchr(const char *s, int c) {
    const char *found = NULL;
    char ch = (char)c;
   
    if ((uintptr_t)s & 7) {
        while (*s) {
            if (*s == ch)
                found = s;
            s++;
            if (!((uintptr_t)s & 7))
                break;
        }
    }
    
    if (!*s)
        return (char *)found;
  
    const uint64_t *s64 = (const uint64_t *)s;
    uint64_t mask = 0x0101010101010101ULL * (unsigned char)ch;
    uint64_t highmask = 0x8080808080808080ULL;
    
    for (;;) {
        uint64_t word = *s64;
        
     
        if (((word - 0x0101010101010101ULL) & ~word & highmask) != 0) {

            const char *cp = (const char *)s64;
            for (int i = 0; i < 8; i++) {
                if (cp[i] == '\0')
                    return (char *)found;
                if (cp[i] == ch)
                    found = cp + i;
            }
        } else {
      
            uint64_t matches = word ^ mask;
            if (((matches - 0x0101010101010101ULL) & ~matches & highmask) != 0) {
                const char *cp = (const char *)s64;
                for (int i = 0; i < 8; i++) {
                    if (cp[i] == ch)
                        found = cp + i;
                }
            }
        }
        
        s64++;
    }
}
/*
char *strdup(const char *s) {
    if (!s)
        return NULL;
    
    size_t len = strlen(s);
    char *new_str = (char *)malloc(len + 1);
    
    if (new_str) {
        memcpy(new_str, s, len);
        new_str[len] = '\0';
    }
    
    return new_str;
}

char *strndup(const char *s, size_t n) {
    if (!s)
        return NULL;
    
    size_t len = 0;
    while (len < n && s[len])
        len++;
    
    char *new_str = (char *)malloc(len + 1);
    
    if (new_str) {
        memcpy(new_str, s, len);
        new_str[len] = '\0';
    }
    
    return new_str;
} */

long strtol(const char *str, char **endptr, int base) {
    const char *s = str;
    long result = 0;
    int sign = 1;
    

    while (*s == ' ' || *s == '\t' || *s == '\n' || 
           *s == '\r' || *s == '\f' || *s == '\v') {
        s++;
    }
    
    if (*s == '-') {
        sign = -1;
        s++;
    } else if (*s == '+') {
        s++;
    }
    
    if (base == 0) {
        if (*s == '0') {
            s++;
            if (*s == 'x' || *s == 'X') {
                base = 16;
                s++;
            } else {
                base = 8;
            }
        } else {
            base = 10;
        }
    }
    
#define LONG_MAX __LONG_MAX__
#define LONG_MIN (-LONG_MAX - 1L)

    int overflow = 0;
    long cutoff = sign == 1 ? LONG_MAX : LONG_MIN;
    int cutlim = sign == 1 ? (int)(LONG_MAX % base) : (int)(-(LONG_MIN % base));
    
    if (sign == 1)
        cutoff = LONG_MAX / base;
    else
        cutoff = -(LONG_MIN / base);
    
    for (;;) {
        int digit;
        
        if (*s >= '0' && *s <= '9')
            digit = *s - '0';
        else if (*s >= 'A' && *s <= 'Z')
            digit = *s - 'A' + 10;
        else if (*s >= 'a' && *s <= 'z')
            digit = *s - 'a' + 10;
        else
            break;
            
        if (digit >= base)
            break;
            
        if (result > cutoff || (result == cutoff && digit > cutlim))
            overflow = 1;
            
        if (!overflow)
            result = result * base + digit;
            
        s++;
    }
    
    if (endptr)
        *endptr = (char *)(overflow ? str : s);
    
    if (overflow)
        return sign == 1 ? LONG_MAX : LONG_MIN;
    
    return sign * result;
}

char *strtok(char *str, const char *delim, char **saveptr) {
    if (!str)
        str = *saveptr;
    
    if (!str)
        return NULL;
    
    while (*str) {
        const char *d = delim;
        while (*d) {
            if (*str == *d)
                break;
            d++;
        }
        if (!*d)
            break;
        str++;
    }
    
    if (!*str) {
        *saveptr = NULL;
        return NULL;
    }
    
    char *token = str;
    
    while (*str) {
        const char *d = delim;
        while (*d) {
            if (*str == *d) {
                *str = '\0';
                *saveptr = str + 1;
                return token;
            }
            d++;
        }
        str++;
    }
    
    *saveptr = NULL;
    return token;
}