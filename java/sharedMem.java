//package com.baeldung.jni;

public class sharedMem {
    static {
        System.loadLibrary("native");
    }

    public native void add(int pos, String data);
    
    public native String get(int pos);
    
    public native Boolean create(int numChar, int arrayLength);
}