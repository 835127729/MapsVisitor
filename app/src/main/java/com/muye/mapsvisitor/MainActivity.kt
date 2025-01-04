package com.muye.mapsvisitor

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.widget.Toast
import com.muye.mapsvisitor.databinding.ActivityMainBinding

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        // Example of a call to a native method
        binding.btnTest.setOnClickListener {
            if(testMapVisitor()){
                Toast.makeText(this, "success", Toast.LENGTH_SHORT).show()
            }else{
                Toast.makeText(this, "failed", Toast.LENGTH_SHORT).show()
            }
        }
    }

    /**
     * A native method that is implemented by the 'mapsvisitor' native library,
     * which is packaged with this application.
     */
    private external fun testMapVisitor(): Boolean

    companion object {
        // Used to load the 'maps_visitor' library on application startup.
        init {
            System.loadLibrary("test")
            System.loadLibrary("maps_visitor")
        }
    }
}