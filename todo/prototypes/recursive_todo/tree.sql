DROP TABLE IF EXISTS todos;

CREATE TABLE IF NOT EXISTS todos (
id INTEGER PRIMARY KEY AUTOINCREMENT,
name TEXT NOT NULL,
timestamp INTEGER NOT NULL,
duetime INTEGER NOT NULL,
status INTEGER NOT NULL,
parentid INTEGER);

INSERT INTO todos (name, id, timestamp, status, duetime) VALUES ('test0', 0, 0, 0, 0);
INSERT INTO todos (name, id, timestamp, status, duetime, parentid) VALUES ('test1', 1, 1, 0, 0, 0);
INSERT INTO todos (name, id, timestamp, status, duetime, parentid) VALUES ('test2', 2, 2, 0, 0, 0);
INSERT INTO todos (name, id, timestamp, status, duetime, parentid) VALUES ('test3', 3, 3, 0, 0, 0);
INSERT INTO todos (name, id, timestamp, status, duetime, parentid) VALUES ('test3', 4, 3, 0, 0, 3);

WITH RECURSIVE todo_tree AS (
    SELECT id, name, timestamp, duetime, status, parentid
    FROM todos
    WHERE id = 0
    UNION ALL
    SELECT sub_task.id, sub_task.name, sub_task.timestamp, sub_task.duetime, sub_task.status, sub_task.parentid
    FROM todos sub_task
    INNER JOIN todo_tree parent_task ON sub_task.parentid = parent_task.id
)
SELECT * FROM todo_tree;

-- SELECT * FROM todos;
