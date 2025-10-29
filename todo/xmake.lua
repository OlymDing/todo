add_rules("mode.debug", "mode.release")

add_requires("sqlite3", {system = true})

target("todo")
    set_kind("binary")
    add_files("src/main.cc")
    add_files("src/todo.cc")
    add_files("src/ui.cc")
    add_files("src/util.cc")
    add_includedirs("include")
    add_packages("sqlite3")
