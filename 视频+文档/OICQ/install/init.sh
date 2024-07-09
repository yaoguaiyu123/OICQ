#!/bin/bash

if [ "$(id -u)" != "0" ]; then
   echo "该脚本必须以root权限运行" 1>&2
   exit 1
fi

echo "复制 OICQ_Server 和 OICQ_Client 到 /opt 目录..."
cp -r ../OICQ_Server /opt/
cp -r ../OICQ_Client /opt/

if [ -d "/opt/OICQ_Server" ] && [ -d "/opt/OICQ_Client" ]; then
    echo "文件夹复制成功。"
else
    echo "复制失败，请检查原路径文件夹是否存在。"
    exit 1
fi

desktop_file_path="/usr/share/applications/OICQ.desktop"

echo "创建 desktop 文件..."
cat << EOF > $desktop_file_path
[Desktop Entry]
Name=OICQ
Comment=Stay connected with your contacts instantly
GenericName=Instant Messaging
Exec=env XDG_CURRENT_DESKTOP=KDE QT_QPA_PLATFORMTHEME=xdgdesktopportal /opt/OICQ_Client/appoicqclient
Icon=/opt/OICQ_Client/icon/logo.png
Type=Application
Categories=Network;InstantMessaging;
MimeType=x-scheme-handler/im;
EOF

if [ -f "$desktop_file_path" ]; then
    echo "Desktop 文件已成功创建并移动到 /usr/share/applications"
else
    echo "Desktop 文件创建失败。"
    exit 1
fi

echo "所有操作完成。"


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

echo "Database and tables 创建成功"
echo "应用程序初始化完毕!"
