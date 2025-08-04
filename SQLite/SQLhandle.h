#include <iostream>
#include <vector>
#include <map>
#include <iomanip>
#include <stdexcept>
#include <string>
#include "ChatRoomInfo.h"

extern "C" {
    #include <sqlite3.h>
}

class SqlHandle {
public:
    SqlHandle(const std::string& name);
    ~SqlHandle();

    void execute(const std::string& sql);

    void opendb();
    void closedb();

    std::vector<std::map<std::string, std::string>> query(const std::string& sql);

    void insert(const std::string& table, const std::map<std::string, std::string>& data);

    void update(const std::string& table, const std::map<std::string, std::string>& data, const std::string& condition);
    void remove(const std::string& table, const std::string& condition);
    void begin_transaction();

    void commit_transaction();

    void rollback_transaction();
    long last_insert_rowid() const;

private:
    sqlite3* m_db;

    std::string m_dbname;

    std::string escape_sql(const std::string& input);
};
