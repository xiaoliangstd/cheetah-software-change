"""LCM type definitions
This file automatically generated by lcm.
DO NOT MODIFY BY HAND!!!!
"""

try:
    import cStringIO.StringIO as BytesIO
except ImportError:
    from io import BytesIO
import struct

class rc_channels_t(object):
    __slots__ = ["ch_1", "ch_2", "ch_3", "ch_4", "ch_5", "ch_6", "ch_7", "ch_8", "ch_9", "ch_10", "ch_11", "ch_12", "ch_13", "ch_14", "ch_15", "ch_16"]

    __typenames__ = ["double", "double", "double", "double", "double", "double", "double", "double", "double", "double", "double", "double", "double", "double", "double", "double"]

    __dimensions__ = [None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None]

    def __init__(self):
        self.ch_1 = 0.0
        self.ch_2 = 0.0
        self.ch_3 = 0.0
        self.ch_4 = 0.0
        self.ch_5 = 0.0
        self.ch_6 = 0.0
        self.ch_7 = 0.0
        self.ch_8 = 0.0
        self.ch_9 = 0.0
        self.ch_10 = 0.0
        self.ch_11 = 0.0
        self.ch_12 = 0.0
        self.ch_13 = 0.0
        self.ch_14 = 0.0
        self.ch_15 = 0.0
        self.ch_16 = 0.0

    def encode(self):
        buf = BytesIO()
        buf.write(rc_channels_t._get_packed_fingerprint())
        self._encode_one(buf)
        return buf.getvalue()

    def _encode_one(self, buf):
        buf.write(struct.pack(">dddddddddddddddd", self.ch_1, self.ch_2, self.ch_3, self.ch_4, self.ch_5, self.ch_6, self.ch_7, self.ch_8, self.ch_9, self.ch_10, self.ch_11, self.ch_12, self.ch_13, self.ch_14, self.ch_15, self.ch_16))

    def decode(data):
        if hasattr(data, 'read'):
            buf = data
        else:
            buf = BytesIO(data)
        if buf.read(8) != rc_channels_t._get_packed_fingerprint():
            raise ValueError("Decode error")
        return rc_channels_t._decode_one(buf)
    decode = staticmethod(decode)

    def _decode_one(buf):
        self = rc_channels_t()
        self.ch_1, self.ch_2, self.ch_3, self.ch_4, self.ch_5, self.ch_6, self.ch_7, self.ch_8, self.ch_9, self.ch_10, self.ch_11, self.ch_12, self.ch_13, self.ch_14, self.ch_15, self.ch_16 = struct.unpack(">dddddddddddddddd", buf.read(128))
        return self
    _decode_one = staticmethod(_decode_one)

    _hash = None
    def _get_hash_recursive(parents):
        if rc_channels_t in parents: return 0
        tmphash = (0x5fd71e3345a45040) & 0xffffffffffffffff
        tmphash  = (((tmphash<<1)&0xffffffffffffffff) + (tmphash>>63)) & 0xffffffffffffffff
        return tmphash
    _get_hash_recursive = staticmethod(_get_hash_recursive)
    _packed_fingerprint = None

    def _get_packed_fingerprint():
        if rc_channels_t._packed_fingerprint is None:
            rc_channels_t._packed_fingerprint = struct.pack(">Q", rc_channels_t._get_hash_recursive([]))
        return rc_channels_t._packed_fingerprint
    _get_packed_fingerprint = staticmethod(_get_packed_fingerprint)

