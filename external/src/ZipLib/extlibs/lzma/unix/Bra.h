/* Bra.h -- Branch converters for executables
2009-02-07 : Igor Pavlov : Public domain */

#ifndef __BRA_H
#define __BRA_H

#include "Types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
These functions convert relative addresses to absolute addresses
in CALL instructions to increase the compression ratio.
  
  In:
    data     - data buffer
    size     - size of data
    ip       - current virtual Instruction Pinter (IP) value
    state    - state variable for x86 converter
    encoding - 0 (for decoding), 1 (for encoding)
  
  Out:
    state    - state variable for x86 converter

  Returns:
    The number of processed bytes. If you call these functions with multiple calls,
    you must start next call with first byte after block of processed bytes.
  
  Type   Endian  Alignment  LookAhead
  
  x86    little      1          4
  ARMT   little      2          2
  ARM    little      4          0
  PPC     big        4          0
  SPARC   big        4          0
  IA64   little     16          0

  size must be >= Alignment + LookAhead, if it's not last block.
  If (size < Alignment + LookAhead), converter returns 0.

  Example:

    UInt32 ip = 0;
    for ()
    {
      ; size must be >= Alignment + LookAhead, if it's not last block
      Size_T processed = Convert(data, size, ip, 1);
      data += processed;
      size -= processed;
      ip += processed;
    }
*/

#define x86_Convert_Init(state) { state = 0; }
Size_T x86_Convert(Byte *data, Size_T size, UInt32 ip, UInt32 *state, int encoding);
Size_T ARM_Convert(Byte *data, Size_T size, UInt32 ip, int encoding);
Size_T ARMT_Convert(Byte *data, Size_T size, UInt32 ip, int encoding);
Size_T PPC_Convert(Byte *data, Size_T size, UInt32 ip, int encoding);
Size_T SPARC_Convert(Byte *data, Size_T size, UInt32 ip, int encoding);
Size_T IA64_Convert(Byte *data, Size_T size, UInt32 ip, int encoding);

#ifdef __cplusplus
}
#endif

#endif
