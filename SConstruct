import os
import sys

opts = Variables()
opts.Add(EnumVariable("platform", "Target platform", sys.platform, allowed_values=("win32", "windows", "linux", "darwin")))
opts.Add(EnumVariable("target", "Compilation target", "template_debug", allowed_values=("template_debug", "template_release")))
opts.Add("api_version", "Godot API version", "4.7")

env = Environment(variables=opts)

env.Append(CPPPATH=[
    "src",
    "Players_Database",
    "godot-cpp/include",
    "godot-cpp/gen/include",
    "godot-cpp/include/godot_cpp"
])

sources = Glob("src/*.cpp") + Glob("Players_Database/*.cpp")

SConscript("godot-cpp/SConstruct")

env.Append(LIBPATH=["godot-cpp/bin"])
env.Append(LIBS=[f"godot-cpp.{env['platform']}.{env['target']}.x86_64"])

target_path = f"demo/bin/libfootball.{env['platform']}.{env['target']}.x86_64.dll"

library = env.SharedLibrary(target=target_path, source=sources)
Default(library)

if env["platform"] == "windows":
    env.Append(LINKFLAGS=["-static", "-static-libgcc", "-static-libstdc++"])
