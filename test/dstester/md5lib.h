/* MD5.H - header file for MD5C.C */

/* Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All
   rights reserved.

   License to copy and use this software is granted provided that it
   is identified as the "RSA Data Security, Inc. MD5 Message-Digest
   Algorithm" in all material mentioning or referencing this software
   or this function.

   License is also granted to make and use derivative works provided
   that such works are identified as "derived from the RSA Data
   Security, Inc. MD5 Message-Digest Algorithm" in all material
   mentioning or referencing the derived work.

   RSA Data Security, Inc. makes no representations concerning either
   the merchantability of this software or the suitability of this
   software for any particular purpose. It is provided "as is"
   without express or implied warranty of any kind.

   These notices must be retained in any copies of any part of this
   documentation and/or software. */
#ifndef __MD5LIB_H
#define __MD5LIB_H

#ifdef __cplusplus
extern "C" {
#endif /* C++ */

#ifndef PROTOTYPES
#define PROTOTYPES 0
#endif

/* POINTER defines a generic pointer type */
typedef unsigned char *POINTER;

/* UINT2 defines a two byte word */
typedef unsigned short int UINT2;

/* UINT4 defines a four byte word */
typedef unsigned int UINT4;

/* PROTO_LIST is defined depending on how PROTOTYPES is defined above.
   If using PROTOTYPES, then PROTO_LIST returns the list, otherwise it
  returns an empty list. */

#if PROTOTYPES
#define PROTO_LIST(list) list
#else
#define PROTO_LIST(list) ()
#endif

/* MD5 context. */
typedef struct {
  UINT4 state[4];                                   /* state (ABCD) */
  UINT4 count[2];        /* number of bits, modulo 2^64 (lsb first) */
  unsigned char buffer[64];                         /* input buffer */
} MSP_MD5_CTX;

void MSP_MD5Init PROTO_LIST ((MSP_MD5_CTX *));
void MSP_MD5Update PROTO_LIST
  ((MD5_CTX *, unsigned char *, unsigned int));
void MSP_MD5Final PROTO_LIST ((unsigned char [16], MSP_MD5_CTX *));

char *MSP_MD5String(const char *input, unsigned long input_len, char output[33], unsigned int output_len);


#ifdef __cplusplus
} /* extern "C" */
#endif /* C++ */

#endif
