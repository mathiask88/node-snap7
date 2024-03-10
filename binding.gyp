{
    "targets": [{
        "target_name": "node_snap7",
        "include_dirs": [
            "./src"
        ],
        "sources": [
            "./src/node_snap7.cpp",
            "./src/node_snap7_client.cpp",
            "./src/node_snap7_server.cpp",
            "./src/snap7.cpp"
        ],
        "defines": ["NODE_ADDON_API_DISABLE_DEPRECATED"],
        "conditions": [
            ["OS=='win'", {
                "libraries": ["-lws2_32.lib", "-lwinmm.lib"],
                "defines": ["_WINSOCK_DEPRECATED_NO_WARNINGS"] # Make sure the STL doesn't try to use exceptions
            }],
            ['OS=="mac"', {
                'cflags+': ['-fvisibility=hidden'],
                'xcode_settings': {
                    'GCC_SYMBOLS_PRIVATE_EXTERN': 'YES', # -fvisibility=hidden
                }
            }]
        ],
        "dependencies": [
            "<!(node -p \"require('node-addon-api').targets\"):node_addon_api",
            "snap7"
        ]
    }, {
        "target_name": "snap7",
        "type": "static_library",
        "include_dirs": [
            "./deps/snap7/src/sys",
            "./deps/snap7/src/core",
            "./deps/snap7/src/lib"
        ],
        "sources": [
            "./deps/snap7/src/sys/snap_msgsock.cpp",
            "./deps/snap7/src/sys/snap_sysutils.cpp",
            "./deps/snap7/src/sys/snap_tcpsrvr.cpp",
            "./deps/snap7/src/sys/snap_threads.cpp",
            "./deps/snap7/src/core/s7_client.cpp",
            "./deps/snap7/src/core/s7_isotcp.cpp",
            "./deps/snap7/src/core/s7_partner.cpp",
            "./deps/snap7/src/core/s7_peer.cpp",
            "./deps/snap7/src/core/s7_server.cpp",
            "./deps/snap7/src/core/s7_text.cpp",
            "./deps/snap7/src/core/s7_micro_client.cpp",
            "./deps/snap7/src/lib/snap7_libmain.cpp"
        ],
        "conditions": [
            ["OS=='linux' or OS=='freebsd'", {
                "cflags_cc": ["-fPIC", "-pedantic", "-fexceptions"],
                "cflags_cc!": ["-fno-exceptions"]
            }],
            ["OS=='win'", {
                "msvs_settings": {
                    "VCCLCompilerTool": {
                        "ExceptionHandling": 1,
                        "AdditionalOptions": ["/EHsc"] # ExceptionHandling=1 is not enough for some versions
                    }
                },
                "defines!": ["_HAS_EXCEPTIONS=0"],
                "defines": ["_WINSOCK_DEPRECATED_NO_WARNINGS"]
            }],
            ["OS=='mac'", {
                "xcode_settings": {
                    "GCC_ENABLE_CPP_EXCEPTIONS": "YES",
                    "GCC_DYNAMIC_NO_PIC": "NO",
                    "OTHER_CFLAGS": ["-pedantic"]
                }
            }]
        ]
    }]
}
