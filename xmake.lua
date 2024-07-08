-- project
set_project("zxhook")
set_xmakever("2.8.9")

-- language
set_warnings("allextra")
set_languages("c++23")
set_encodings("utf-8")

-- allows
set_allowedplats("windows")
set_allowedarchs("windows|x86", "windows|x64")
set_allowedmodes("debug", "release", "releasedbg")

-- rules
add_rules("plugin.vsxmake.autoupdate")
add_rules("mode.debug", "mode.release", "mode.releasedbg")

-- lto
if is_mode("releasedbg") or is_mode("release") then
    set_policy("build.optimization.lto", true)
end

-- pkg
add_requires("microsoft-detours")

-- targets
target("zxhook")
    set_kind("$(kind)")
    if is_plat("windows") then
        if is_kind("shared") then
            add_rules("utils.symbols.export_all", {export_classes = true})
        end
    end
    add_files("src/src/**.cpp")
    add_includedirs("src/include", {public = true})
    add_headerfiles("src/include/(**.h)")
    add_syslinks("user32")
    add_packages("microsoft-detours", {public = true})

target("zxhook-test")
    set_default(false)
    set_kind("binary")
    add_files("test/Test/main.cpp")
    add_deps("zxhook")

target("zxhook-sample-dll")
    set_default(false)
    set_kind("shared")
    add_files("test/SampleDll/SampleDll.cpp")
    add_deps("zxhook")
    add_syslinks("user32")

target("zxhook-sample-exe")
    set_default(false)
    set_kind("binary")
    add_files("test/SampleExe/SampleExe.cpp")
    add_syslinks("user32")

target("zxhook-sample-injector")
    set_default(false)
    set_kind("binary")
    add_files("test/SampleInjector/SampleInjector.cpp")
    add_deps("zxhook")