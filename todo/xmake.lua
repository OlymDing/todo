add_rules("mode.debug", "mode.release")

add_requires("sqlite3", {system = true})
add_requires("readline", {system = true})

target("todo")
    set_kind("binary")
    add_files("src/*.cc")
    add_includedirs("include")
    add_packages("sqlite3", "readline")
