// Copyright (C) 2014 Vicente Botet
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/predef/platform.h>

#if BOOST_PLAT_WINDOWS_RUNTIME

#include <wrl\wrappers\corewrappers.h>
#pragma comment(lib, "runtimeobject.lib")

// Necessary for the tests which to keep the Windows Runtime active,
// when running in an actual Windows store/phone application initialization
// is handled automatically by the CRT.
// This is easier than calling in the main function for each test case.
Microsoft::WRL::Wrappers::RoInitializeWrapper runtime(RO_INIT_MULTITHREADED);

#else
    
#if defined(_MSC_VER) && !defined(__CYGWIN__)

extern "C" {
#ifdef _DEBUG
__declspec(dllimport) int _CrtSetReportMode(int, int);
__declspec(dllimport) void* _CrtSetReportFile(int, void*);
#endif
__declspec(dllimport) unsigned __stdcall SetErrorMode(unsigned);
}

namespace boost { namespace detail {

template <typename _>
struct error_popup_suppressor {
    error_popup_suppressor() { switch_off_error_popups(); }

    static void switch_off_error_popups() {
        if (!done) {
            done = true;
#ifdef _DEBUG
            _CrtSetReportMode(1, 1);
            _CrtSetReportFile(1, (void*)-5);
#endif
            SetErrorMode(3);
        }
    };
    static bool done;
};
template <typename _>
bool error_popup_suppressor<_>::done = false;

namespace {
error_popup_suppressor<bool> _;
}

}}

#endif

#endif
