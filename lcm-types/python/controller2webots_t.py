"""LCM type definitions
This file automatically generated by lcm.
DO NOT MODIFY BY HAND!!!!
"""

try:
    import cStringIO.StringIO as BytesIO
except ImportError:
    from io import BytesIO
import struct

class controller2webots_t(object):
    __slots__ = ["q_des_abad", "q_des_hip", "q_des_knee", "qd_des_abad", "qd_des_hip", "qd_des_knee", "kp_abad", "kp_hip", "kp_knee", "kd_abad", "kd_hip", "kd_knee", "tau_abad_ff", "tau_hip_ff", "tau_knee_ff", "flags"]

    __typenames__ = ["float", "float", "float", "float", "float", "float", "float", "float", "float", "float", "float", "float", "float", "float", "float", "int32_t"]

    __dimensions__ = [[4], [4], [4], [4], [4], [4], [4], [4], [4], [4], [4], [4], [4], [4], [4], [4]]

    def __init__(self):
        self.q_des_abad = [ 0.0 for dim0 in range(4) ]
        self.q_des_hip = [ 0.0 for dim0 in range(4) ]
        self.q_des_knee = [ 0.0 for dim0 in range(4) ]
        self.qd_des_abad = [ 0.0 for dim0 in range(4) ]
        self.qd_des_hip = [ 0.0 for dim0 in range(4) ]
        self.qd_des_knee = [ 0.0 for dim0 in range(4) ]
        self.kp_abad = [ 0.0 for dim0 in range(4) ]
        self.kp_hip = [ 0.0 for dim0 in range(4) ]
        self.kp_knee = [ 0.0 for dim0 in range(4) ]
        self.kd_abad = [ 0.0 for dim0 in range(4) ]
        self.kd_hip = [ 0.0 for dim0 in range(4) ]
        self.kd_knee = [ 0.0 for dim0 in range(4) ]
        self.tau_abad_ff = [ 0.0 for dim0 in range(4) ]
        self.tau_hip_ff = [ 0.0 for dim0 in range(4) ]
        self.tau_knee_ff = [ 0.0 for dim0 in range(4) ]
        self.flags = [ 0 for dim0 in range(4) ]

    def encode(self):
        buf = BytesIO()
        buf.write(controller2webots_t._get_packed_fingerprint())
        self._encode_one(buf)
        return buf.getvalue()

    def _encode_one(self, buf):
        buf.write(struct.pack('>4f', *self.q_des_abad[:4]))
        buf.write(struct.pack('>4f', *self.q_des_hip[:4]))
        buf.write(struct.pack('>4f', *self.q_des_knee[:4]))
        buf.write(struct.pack('>4f', *self.qd_des_abad[:4]))
        buf.write(struct.pack('>4f', *self.qd_des_hip[:4]))
        buf.write(struct.pack('>4f', *self.qd_des_knee[:4]))
        buf.write(struct.pack('>4f', *self.kp_abad[:4]))
        buf.write(struct.pack('>4f', *self.kp_hip[:4]))
        buf.write(struct.pack('>4f', *self.kp_knee[:4]))
        buf.write(struct.pack('>4f', *self.kd_abad[:4]))
        buf.write(struct.pack('>4f', *self.kd_hip[:4]))
        buf.write(struct.pack('>4f', *self.kd_knee[:4]))
        buf.write(struct.pack('>4f', *self.tau_abad_ff[:4]))
        buf.write(struct.pack('>4f', *self.tau_hip_ff[:4]))
        buf.write(struct.pack('>4f', *self.tau_knee_ff[:4]))
        buf.write(struct.pack('>4i', *self.flags[:4]))

    def decode(data):
        if hasattr(data, 'read'):
            buf = data
        else:
            buf = BytesIO(data)
        if buf.read(8) != controller2webots_t._get_packed_fingerprint():
            raise ValueError("Decode error")
        return controller2webots_t._decode_one(buf)
    decode = staticmethod(decode)

    def _decode_one(buf):
        self = controller2webots_t()
        self.q_des_abad = struct.unpack('>4f', buf.read(16))
        self.q_des_hip = struct.unpack('>4f', buf.read(16))
        self.q_des_knee = struct.unpack('>4f', buf.read(16))
        self.qd_des_abad = struct.unpack('>4f', buf.read(16))
        self.qd_des_hip = struct.unpack('>4f', buf.read(16))
        self.qd_des_knee = struct.unpack('>4f', buf.read(16))
        self.kp_abad = struct.unpack('>4f', buf.read(16))
        self.kp_hip = struct.unpack('>4f', buf.read(16))
        self.kp_knee = struct.unpack('>4f', buf.read(16))
        self.kd_abad = struct.unpack('>4f', buf.read(16))
        self.kd_hip = struct.unpack('>4f', buf.read(16))
        self.kd_knee = struct.unpack('>4f', buf.read(16))
        self.tau_abad_ff = struct.unpack('>4f', buf.read(16))
        self.tau_hip_ff = struct.unpack('>4f', buf.read(16))
        self.tau_knee_ff = struct.unpack('>4f', buf.read(16))
        self.flags = struct.unpack('>4i', buf.read(16))
        return self
    _decode_one = staticmethod(_decode_one)

    _hash = None
    def _get_hash_recursive(parents):
        if controller2webots_t in parents: return 0
        tmphash = (0xecc8eaa6369bc167) & 0xffffffffffffffff
        tmphash  = (((tmphash<<1)&0xffffffffffffffff) + (tmphash>>63)) & 0xffffffffffffffff
        return tmphash
    _get_hash_recursive = staticmethod(_get_hash_recursive)
    _packed_fingerprint = None

    def _get_packed_fingerprint():
        if controller2webots_t._packed_fingerprint is None:
            controller2webots_t._packed_fingerprint = struct.pack(">Q", controller2webots_t._get_hash_recursive([]))
        return controller2webots_t._packed_fingerprint
    _get_packed_fingerprint = staticmethod(_get_packed_fingerprint)

