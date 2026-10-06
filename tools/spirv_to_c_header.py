#!/usr/bin/env python3
"""spirv_to_c_header.py <in.spv> <out.h> <SYMBOL>"""
import struct
import sys


def main():
    if len(sys.argv) != 4:
        print("usage: spirv_to_c_header.py in.spv out.h SYMBOL")
        return 2
    src, dst, sym = sys.argv[1], sys.argv[2], sys.argv[3]
    data = open(src, "rb").read()
    if len(data) % 4 or len(data) < 20 or data[:4] != b"\x03\x02\x23\x07":
        raise SystemExit("not a SPIR-V module: " + src)
    words = struct.unpack("<%dI" % (len(data) // 4), data)
    out = ["#pragma once", "#include <stdint.h>",
           "static const uint32_t %s[] = {" % sym]
    for i in range(0, len(words), 6):
        out.append("    " + ", ".join("0x%08x" % w for w in words[i:i + 6]) + ",")
    out.append("};")
    open(dst, "w").write("\n".join(out) + "\n")
    print("wrote %s (%d bytes)" % (dst, len(data)))
    return 0


if __name__ == "__main__":
    sys.exit(main())
