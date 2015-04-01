{
  "targets": [
    {
      "target_name": "runBin",
      "cflags!": ["-fexceptions"],
      "cflags_cc!":["-fnb-rtti", "-fexceptions", "-g", "-O0"],
      "sources": [ "runBin.cc" ]
    }
  ]
}
