//package com.baeldung.jni;

public class sharedMem {
    static {
        System.loadLibrary("native");
    }

    public static void main(String[] args) {
        new sharedMem().add();
    }

    private native void add();
}