//package com.baeldung.jni;

public class sharedMem {
    static {
        System.loadLibrary("native");
    }

    public static void main(String[] args) {
        new sharedem().add();
    }

    private native void add();
}