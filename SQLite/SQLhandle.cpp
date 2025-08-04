#include "SQLhandle.h"

SqlHandle::SqlHandle(const std::string& name):m_dbname(name), m_db(nullptr)
{
    opendb();
    execute("DROP TABLE IF EXISTS users;");
    execute("CREATE TABLE users ("
            "userid INTEGER PRIMARY KEY AUTOINCREMENT, "
            "name TEXT NOT NULL, "
            "psw TEXT UNIQUE);");
    execute("DROP TABLE IF EXISTS msgs;");
    execute("CREATE TABLE msgs ("
            "msgid INTEGER PRIMARY KEY AUTOINCREMENT, "
            "userid INTEGER NOT NULL, "
            "time TEXT NOT NULL, "
            "context TEXT UNIQUE);");
}

SqlHandle::~SqlHandle() 
{
    closedb();
}

void SqlHandle::execute(const std::string& sql) {
    char* err_msg = nullptr;
    int rc = sqlite3_exec(m_db, sql.c_str(), nullptr, nullptr, &err_msg);
        
    if (rc != SQLITE_OK) {
        std::string error = "SQL错误: " + std::string(err_msg);
        sqlite3_free(err_msg);
        throw std::runtime_error(error);
    }
}
void SqlHandle::opendb() {
    int rc = sqlite3_open(m_dbname.c_str(), &m_db);
    if (rc != SQLITE_OK) {
        std::string err_msg = "无法打开数据库: " + std::string(sqlite3_errmsg(m_db));
        sqlite3_close(m_db);
        throw std::runtime_error(err_msg);
    }
}
void SqlHandle::closedb() {
    if (m_db)sqlite3_close(m_db);
}

std::vector<std::map<std::string, std::string>> SqlHandle::query(const std::string& sql) {
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, nullptr);
        
    if (rc != SQLITE_OK) {
            throw std::runtime_error("准备查询失败: " + std::string(sqlite3_errmsg(m_db)));
    }
        
    std::vector<std::map<std::string, std::string>> results;
    int column_count = sqlite3_column_count(stmt);
        
    std::vector<std::string> column_names;
    for (int i = 0; i < column_count; i++)column_names.push_back(sqlite3_column_name(stmt, i));
        
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        std::map<std::string, std::string> row;
        for (int i = 0; i < column_count; i++) {
            const char* value = reinterpret_cast<const char*>(sqlite3_column_text(stmt, i));
            row[column_names[i]] = value ? value : "NULL";
        }
        results.push_back(row);
    }
    
    if (rc != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        throw std::runtime_error("执行查询失败: " + std::string(sqlite3_errmsg(m_db)));
    }
    
    sqlite3_finalize(stmt);
    return results;
}

void SqlHandle::insert(const std::string& table, const std::map<std::string, std::string>& data) {
    if (data.empty()) {
        throw std::invalid_argument("插入数据不能为空");
        return;
    }
    
    std::string columns;
    std::string values;
    
    for (const auto& pair : data) {
        if (!columns.empty()) {
            columns += ", ";
            values += ", ";
        }
        columns += pair.first;
        values += "'" + escape_sql(pair.second) + "'";
    }
    
    std::string sql = "INSERT INTO " + table + " (" + columns + ") VALUES (" + values + ");";
    execute(sql);
}

void SqlHandle::update(const std::string& table, const std::map<std::string, std::string>& data, const std::string& condition) {
    if (data.empty()) {
        throw std::invalid_argument("更新数据不能为空");
    }
    
    std::string set_clause;
    
    for (const auto& pair : data) {
        if (!set_clause.empty()) {
            set_clause += ", ";
        }
        set_clause += pair.first + " = '" + escape_sql(pair.second) + "'";
    }
    
    std::string sql = "UPDATE " + table + " SET " + set_clause;
    if (!condition.empty()) {
        sql += " WHERE " + condition;
    }
    sql += ";";
    
    execute(sql);
}

void SqlHandle::remove(const std::string& table, const std::string& condition) {
    std::string sql = "DELETE FROM " + table;
    if (!condition.empty()) {
        sql += " WHERE " + condition;
    }
    sql += ";";
    
    execute(sql);
}

void SqlHandle::begin_transaction() {
    execute("BEGIN TRANSACTION;");
}

void SqlHandle::commit_transaction() {
    execute("COMMIT;");
}

void SqlHandle::rollback_transaction() {
    execute("ROLLBACK;");
}

std::string SqlHandle::escape_sql(const std::string& input) {
    std::string output;
    for (char c : input) {
        if (c == '\'') {
            output += "''";
        } else {
            output += c;
        }
    }
    return output;
}

long SqlHandle::last_insert_rowid() const{
    return sqlite3_last_insert_rowid(m_db);
}