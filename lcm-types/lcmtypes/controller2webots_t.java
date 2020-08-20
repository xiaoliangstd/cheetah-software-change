/* LCM type definition class file
 * This file was automatically generated by lcm-gen
 * DO NOT MODIFY BY HAND!!!!
 */

package lcmtypes;
 
import java.io.*;
import java.util.*;
import lcm.lcm.*;
 
public final class controller2webots_t implements lcm.lcm.LCMEncodable
{
    public float q_des_abad[];
    public float q_des_hip[];
    public float q_des_knee[];
    public float qd_des_abad[];
    public float qd_des_hip[];
    public float qd_des_knee[];
    public float kp_abad[];
    public float kp_hip[];
    public float kp_knee[];
    public float kd_abad[];
    public float kd_hip[];
    public float kd_knee[];
    public float tau_abad_ff[];
    public float tau_hip_ff[];
    public float tau_knee_ff[];
    public int flags[];
 
    public controller2webots_t()
    {
        q_des_abad = new float[4];
        q_des_hip = new float[4];
        q_des_knee = new float[4];
        qd_des_abad = new float[4];
        qd_des_hip = new float[4];
        qd_des_knee = new float[4];
        kp_abad = new float[4];
        kp_hip = new float[4];
        kp_knee = new float[4];
        kd_abad = new float[4];
        kd_hip = new float[4];
        kd_knee = new float[4];
        tau_abad_ff = new float[4];
        tau_hip_ff = new float[4];
        tau_knee_ff = new float[4];
        flags = new int[4];
    }
 
    public static final long LCM_FINGERPRINT;
    public static final long LCM_FINGERPRINT_BASE = 0xecc8eaa6369bc167L;
 
    static {
        LCM_FINGERPRINT = _hashRecursive(new ArrayList<Class<?>>());
    }
 
    public static long _hashRecursive(ArrayList<Class<?>> classes)
    {
        if (classes.contains(lcmtypes.controller2webots_t.class))
            return 0L;
 
        classes.add(lcmtypes.controller2webots_t.class);
        long hash = LCM_FINGERPRINT_BASE
            ;
        classes.remove(classes.size() - 1);
        return (hash<<1) + ((hash>>63)&1);
    }
 
    public void encode(DataOutput outs) throws IOException
    {
        outs.writeLong(LCM_FINGERPRINT);
        _encodeRecursive(outs);
    }
 
    public void _encodeRecursive(DataOutput outs) throws IOException
    {
        for (int a = 0; a < 4; a++) {
            outs.writeFloat(this.q_des_abad[a]); 
        }
 
        for (int a = 0; a < 4; a++) {
            outs.writeFloat(this.q_des_hip[a]); 
        }
 
        for (int a = 0; a < 4; a++) {
            outs.writeFloat(this.q_des_knee[a]); 
        }
 
        for (int a = 0; a < 4; a++) {
            outs.writeFloat(this.qd_des_abad[a]); 
        }
 
        for (int a = 0; a < 4; a++) {
            outs.writeFloat(this.qd_des_hip[a]); 
        }
 
        for (int a = 0; a < 4; a++) {
            outs.writeFloat(this.qd_des_knee[a]); 
        }
 
        for (int a = 0; a < 4; a++) {
            outs.writeFloat(this.kp_abad[a]); 
        }
 
        for (int a = 0; a < 4; a++) {
            outs.writeFloat(this.kp_hip[a]); 
        }
 
        for (int a = 0; a < 4; a++) {
            outs.writeFloat(this.kp_knee[a]); 
        }
 
        for (int a = 0; a < 4; a++) {
            outs.writeFloat(this.kd_abad[a]); 
        }
 
        for (int a = 0; a < 4; a++) {
            outs.writeFloat(this.kd_hip[a]); 
        }
 
        for (int a = 0; a < 4; a++) {
            outs.writeFloat(this.kd_knee[a]); 
        }
 
        for (int a = 0; a < 4; a++) {
            outs.writeFloat(this.tau_abad_ff[a]); 
        }
 
        for (int a = 0; a < 4; a++) {
            outs.writeFloat(this.tau_hip_ff[a]); 
        }
 
        for (int a = 0; a < 4; a++) {
            outs.writeFloat(this.tau_knee_ff[a]); 
        }
 
        for (int a = 0; a < 4; a++) {
            outs.writeInt(this.flags[a]); 
        }
 
    }
 
    public controller2webots_t(byte[] data) throws IOException
    {
        this(new LCMDataInputStream(data));
    }
 
    public controller2webots_t(DataInput ins) throws IOException
    {
        if (ins.readLong() != LCM_FINGERPRINT)
            throw new IOException("LCM Decode error: bad fingerprint");
 
        _decodeRecursive(ins);
    }
 
    public static lcmtypes.controller2webots_t _decodeRecursiveFactory(DataInput ins) throws IOException
    {
        lcmtypes.controller2webots_t o = new lcmtypes.controller2webots_t();
        o._decodeRecursive(ins);
        return o;
    }
 
    public void _decodeRecursive(DataInput ins) throws IOException
    {
        this.q_des_abad = new float[(int) 4];
        for (int a = 0; a < 4; a++) {
            this.q_des_abad[a] = ins.readFloat();
        }
 
        this.q_des_hip = new float[(int) 4];
        for (int a = 0; a < 4; a++) {
            this.q_des_hip[a] = ins.readFloat();
        }
 
        this.q_des_knee = new float[(int) 4];
        for (int a = 0; a < 4; a++) {
            this.q_des_knee[a] = ins.readFloat();
        }
 
        this.qd_des_abad = new float[(int) 4];
        for (int a = 0; a < 4; a++) {
            this.qd_des_abad[a] = ins.readFloat();
        }
 
        this.qd_des_hip = new float[(int) 4];
        for (int a = 0; a < 4; a++) {
            this.qd_des_hip[a] = ins.readFloat();
        }
 
        this.qd_des_knee = new float[(int) 4];
        for (int a = 0; a < 4; a++) {
            this.qd_des_knee[a] = ins.readFloat();
        }
 
        this.kp_abad = new float[(int) 4];
        for (int a = 0; a < 4; a++) {
            this.kp_abad[a] = ins.readFloat();
        }
 
        this.kp_hip = new float[(int) 4];
        for (int a = 0; a < 4; a++) {
            this.kp_hip[a] = ins.readFloat();
        }
 
        this.kp_knee = new float[(int) 4];
        for (int a = 0; a < 4; a++) {
            this.kp_knee[a] = ins.readFloat();
        }
 
        this.kd_abad = new float[(int) 4];
        for (int a = 0; a < 4; a++) {
            this.kd_abad[a] = ins.readFloat();
        }
 
        this.kd_hip = new float[(int) 4];
        for (int a = 0; a < 4; a++) {
            this.kd_hip[a] = ins.readFloat();
        }
 
        this.kd_knee = new float[(int) 4];
        for (int a = 0; a < 4; a++) {
            this.kd_knee[a] = ins.readFloat();
        }
 
        this.tau_abad_ff = new float[(int) 4];
        for (int a = 0; a < 4; a++) {
            this.tau_abad_ff[a] = ins.readFloat();
        }
 
        this.tau_hip_ff = new float[(int) 4];
        for (int a = 0; a < 4; a++) {
            this.tau_hip_ff[a] = ins.readFloat();
        }
 
        this.tau_knee_ff = new float[(int) 4];
        for (int a = 0; a < 4; a++) {
            this.tau_knee_ff[a] = ins.readFloat();
        }
 
        this.flags = new int[(int) 4];
        for (int a = 0; a < 4; a++) {
            this.flags[a] = ins.readInt();
        }
 
    }
 
    public lcmtypes.controller2webots_t copy()
    {
        lcmtypes.controller2webots_t outobj = new lcmtypes.controller2webots_t();
        outobj.q_des_abad = new float[(int) 4];
        System.arraycopy(this.q_des_abad, 0, outobj.q_des_abad, 0, 4); 
        outobj.q_des_hip = new float[(int) 4];
        System.arraycopy(this.q_des_hip, 0, outobj.q_des_hip, 0, 4); 
        outobj.q_des_knee = new float[(int) 4];
        System.arraycopy(this.q_des_knee, 0, outobj.q_des_knee, 0, 4); 
        outobj.qd_des_abad = new float[(int) 4];
        System.arraycopy(this.qd_des_abad, 0, outobj.qd_des_abad, 0, 4); 
        outobj.qd_des_hip = new float[(int) 4];
        System.arraycopy(this.qd_des_hip, 0, outobj.qd_des_hip, 0, 4); 
        outobj.qd_des_knee = new float[(int) 4];
        System.arraycopy(this.qd_des_knee, 0, outobj.qd_des_knee, 0, 4); 
        outobj.kp_abad = new float[(int) 4];
        System.arraycopy(this.kp_abad, 0, outobj.kp_abad, 0, 4); 
        outobj.kp_hip = new float[(int) 4];
        System.arraycopy(this.kp_hip, 0, outobj.kp_hip, 0, 4); 
        outobj.kp_knee = new float[(int) 4];
        System.arraycopy(this.kp_knee, 0, outobj.kp_knee, 0, 4); 
        outobj.kd_abad = new float[(int) 4];
        System.arraycopy(this.kd_abad, 0, outobj.kd_abad, 0, 4); 
        outobj.kd_hip = new float[(int) 4];
        System.arraycopy(this.kd_hip, 0, outobj.kd_hip, 0, 4); 
        outobj.kd_knee = new float[(int) 4];
        System.arraycopy(this.kd_knee, 0, outobj.kd_knee, 0, 4); 
        outobj.tau_abad_ff = new float[(int) 4];
        System.arraycopy(this.tau_abad_ff, 0, outobj.tau_abad_ff, 0, 4); 
        outobj.tau_hip_ff = new float[(int) 4];
        System.arraycopy(this.tau_hip_ff, 0, outobj.tau_hip_ff, 0, 4); 
        outobj.tau_knee_ff = new float[(int) 4];
        System.arraycopy(this.tau_knee_ff, 0, outobj.tau_knee_ff, 0, 4); 
        outobj.flags = new int[(int) 4];
        System.arraycopy(this.flags, 0, outobj.flags, 0, 4); 
        return outobj;
    }
 
}

