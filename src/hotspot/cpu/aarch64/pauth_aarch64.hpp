/*
 * Copyright (c) 2021, Oracle and/or its affiliates. All rights reserved.
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 only, as
 * published by the Free Software Foundation.
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * version 2 for more details (a copy is included in the LICENSE file that
 * accompanied this code).
 *
 * You should have received a copy of the GNU General Public License version
 * 2 along with this work; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Please contact Oracle, 500 Oracle Parkway, Redwood Shores, CA 94065 USA
 * or visit www.oracle.com if you need additional information or have any
 * questions.
 *
 */

#ifndef CPU_AARCH64_PAUTH_AARCH64_INLINE_HPP
#define CPU_AARCH64_PAUTH_AARCH64_INLINE_HPP

#ifdef __APPLE__
#include <ptrauth.h>
#endif

// Only the PAC instructions in the NOP space can be used. This ensures the
// binaries work on systems without PAC. Write these instructions using their
// alternate "hint" instructions to ensure older compilers can still be used.

#define XPACLRI "hint #0x7;"

inline address pauth_strip_pointer(address ptr) {
#ifdef __APPLE__
  ptrauth_strip(ptr, ptrauth_key_asib);
#else
  asm volatile ("mov x30, %0;"
                XPACLRI
                "mov %0, x30;" : "+r"(ptr) : : "x30");
#endif
  return ptr;
}

inline bool pauth_ptr_is_raw(address ptr) {
  // Confirm none of the high bits are set in the pointer.
  return ptr == pauth_strip_pointer(ptr);
}

#undef XPACLRI

#endif // CPU_AARCH64_PAUTH_AARCH64_INLINE_HPP
