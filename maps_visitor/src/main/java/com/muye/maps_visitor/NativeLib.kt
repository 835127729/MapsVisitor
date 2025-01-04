package com.muye.maps_visitor

class NativeLib {

    /**
     * A native method that is implemented by the 'maps_visitor' native library,
     * which is packaged with this application.
     */
    external fun stringFromJNI(): String

    companion object {
        // Used to load the 'maps_visitor' library on application startup.
        init {
            System.loadLibrary("maps_visitor")
        }
    }
}