{
  "targets": [
    {
      "target_name": "compile",
      "cflags!": ["-fexceptions"],
      "cflags_cc!":["-fnb-rtti", "-fexceptions", "-g", "-O0"],
      "sources": [ "compile.cc" ]
    }
  ]
}
