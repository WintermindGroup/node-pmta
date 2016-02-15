{
  "targets": [
    {
      "target_name"   : "pmta",
      "sources"       : [ "src/pmta.cpp" ],
      "include_dirs"  : [ 
                          "/opt/pmta/api/include/",
                          "<!(node -e \"require('nan')\")"
                        ],
      "libraries"     : [ "-lpmta" ],
      "cflags"        : [ "-Wno-write-strings" ],
      "cflags!"       : [ "-fno-exceptions" ],
      "cflags_cc!"    : [ "-fno-exceptions" ]
    }
  ]
}
