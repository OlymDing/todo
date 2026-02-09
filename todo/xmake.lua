add_rules("mode.debug", "mode.release")

add_requires("sqlite3", {system = true})
add_requires("readline", {system = true})
add_requires("PainterEngine", {system = true})
add_requires("GL", {system = true})
add_requires("glut", {system = true})

target("todo")
    set_kind("binary")
    add_files("src/*.cc")
    add_includedirs("include", "/home/olym/otherProjects/PainterEngine")
    add_packages("sqlite3", "readline", "PainterEngine", "GL", "glut")
