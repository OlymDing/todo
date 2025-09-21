add_rules("mode.debug", "mode.release")

add_requires("sqlite3", {system = true})

target("todo")
    set_kind("binary")
    add_files("src/main.cc")
    add_files("src/todo.cc")
    add_packages("sqlite3")
