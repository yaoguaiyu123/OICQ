#!/bin/bash

declare -a directories=(
    "/root/.config/OICQ"
    "/root/.config/OICQ/server"
    "/root/.config/OICQ/server/database"
    "/root/.config/OICQ/server/file"
    "/root/.config/OICQ/server/head"
    "/root/.config/OICQ/client"
    "/root/.config/OICQ/client/head"
    "/root/.config/OICQ/client/recv"
    "/root/.config/OICQ/client/send"
    "/root/.config/OICQ/client/temp"
)

for dir in "${directories[@]}"; do
    if [ ! -d "$dir" ]; then
        mkdir -p "$dir"
        if [ $? -ne 0 ]; then
            echo "Failed to create directory: $dir"
            exit 1
        fi
    fi
done

DB_PATH="/root/.config/OICQ/server/database/copyQQ.db"

if ! command -v sqlite3 &> /dev/null; then
    echo "sqlite3 could not be found, please install it first."
    exit 1
fi

sqlite3 $DB_PATH <<EOF
CREATE TABLE IF NOT EXISTS users (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    accountid INTEGER UNIQUE,
    username TEXT,
    password TEXT,
    headpath TEXT
);

CREATE TABLE IF NOT EXISTS friends (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    accountid INTEGER,
    friendid INTEGER,
    unreadCount INTEGER
);

CREATE TABLE IF NOT EXISTS friendRequests (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    accountid INTEGER,
    friendid INTEGER
);

CREATE TABLE IF NOT EXISTS files (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    messageId INTEGER,
    accountId INTEGER,
    friendId INTEGER,
    filename TEXT
);

CREATE TABLE IF NOT EXISTS messages (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    senderId INTEGER,
    receiverId INTEGER,
    message TEXT,
    messageId INTEGER,
    filename TEXT DEFAULT '',
    filesize TEXT DEFAULT '',
    messageDate TEXT,
    messageType TEXT
);
EOF

if [ $? -ne 0 ]; then
    echo "Failed to create tables in the SQLite database"
    exit 1
fi

for i in $(seq -f "%02g" 1 12); do
    src="head$i.jpg"
    dest="/root/.config/OICQ/server/head/$src"
    if [ -f "$src" ]; then
        cp "$src" "$dest"
        echo "Copied $src to $dest"
    else
        echo "File $src not found"
    fi
done

for i in $(seq 1 8); do
    name=$(echo "用户${i}")
    accountId=$((100000 + i))
    headpath="/root/.config/OICQ/server/head/head0$i.jpg"

    sqlite3 $DB_PATH "INSERT INTO users (accountid, username, password, headpath) VALUES ($accountId, '$name', '123456', '$headpath');"
done

declare -A pairs
while [ ${#pairs[@]} -lt 10 ]; do
    id1=$((RANDOM % 8 + 100001))
    id2=$((RANDOM % 8 + 100001))
    if [ $id1 -ne $id2 ] && [ -z "${pairs[$id1:$id2]}" ] && [ -z "${pairs[$id2:$id1]}" ]; then
        pairs[$id1:$id2]=1
        sqlite3 $DB_PATH "INSERT INTO friends (accountid, friendid, unreadCount) VALUES ($id1, $id2, 0);"
        sqlite3 $DB_PATH "INSERT INTO friends (accountid, friendid, unreadCount) VALUES ($id2, $id1, 0);"
    fi
done

echo "Database and tables created successfully"
echo "Sample data and friend pairs inserted into the database"
echo "Application initialized successfully!"
