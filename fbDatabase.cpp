/* $Id: fbDatabase.cpp,v 1.18 2008/04/19 20:37:29 laffer1 Exp $ */

#include "fbDatabase.h"

/**
*  fbDatabase
*  main database access object
*  @author Byron Heads
*  @date MARCH 16, 2008
*/


fbDatabase::fbDatabase(fbErrorLogger* log, string path):errlog(log), cs(), db(errlog), row(0)
{
	db.connect(path);
	ensureAuthSchema();
	errlog->debug(NONE, "fbDatabase.this");
}

fbDatabase::fbDatabase(fbErrorLogger* log, const char* path):errlog(log), cs(), db(errlog), row(0)
{
	db.connect(path);
	ensureAuthSchema();
	errlog->debug(NONE, "fbDatabase.this");
}

fbDatabase::~fbDatabase()
{
	db.close();
	errlog->debug(NONE, "fbDatabase.~this");
}


bool fbDatabase::addBackupJob(string& desc, fbDate& date, fbTime& time, string& path, Repeat_type rt, int rv)
{
	static const char sql[] =
		"INSERT INTO backup (desc, date, time, repeatmode, repeatval, disk)"
		" VALUES (?, ?, ?, ?, ?, ?);";

	//backup (ID INTEGER PRIMARY KEY, desc TEXT, date INTEGER, time INTEGER, repeatmode INTEGER, repeatval INTEGER, disk TEXT);

	sqlite3_stmt* stmt = NULL;
	if (sqlite3_prepare_v2(db.handle(), sql, -1, &stmt, NULL) != SQLITE_OK) {
		errlog->warn(UNKNOWN, "Failed to prepare addBackupJob: %s",
		             sqlite3_errmsg(db.handle()));
		return false;
	}

	sqlite3_bind_text(stmt, 1, desc.c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_int64(stmt, 2, date.getJulian());
	sqlite3_bind_int64(stmt, 3, time.getTicks());
	sqlite3_bind_int(stmt,  4, static_cast<int>(rt));
	sqlite3_bind_int(stmt,  5, rv);
	sqlite3_bind_text(stmt, 6, path.c_str(), -1, SQLITE_TRANSIENT);

	errlog->debug(NONE, "addBackupJob prepared statement executing");
	if (!db.exeStmt(stmt)) {
		errlog->warn(UNKNOWN, "Failed to add Backup Job..");
		return false;
	}
	return true;
}

bool fbDatabase::addRestoreJob(string& tarfile, string& dest)
{
	static const char sql[] =
		"INSERT INTO restore (tarfile, path) VALUES (?, ?);";

	//restore (ID INTEGER PRIMARY KEY, tarfile TEXT, path TEXT);

	sqlite3_stmt* stmt = NULL;
	if (sqlite3_prepare_v2(db.handle(), sql, -1, &stmt, NULL) != SQLITE_OK) {
		errlog->warn(UNKNOWN, "Failed to prepare addRestoreJob: %s",
		             sqlite3_errmsg(db.handle()));
		return false;
	}

	sqlite3_bind_text(stmt, 1, tarfile.c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(stmt, 2, dest.c_str(),    -1, SQLITE_TRANSIENT);

	errlog->debug(NONE, "addRestoreJob prepared statement executing");
	if (!db.exeStmt(stmt)) {
		errlog->warn(UNKNOWN, "Failed to add Restore Job..");
		return false;
	}
	return true;
}

bool fbDatabase::addRepo(string& desc, fbDate& date, fbTime& time, string& path, string& tarfile)
{
	static const char sql[] =
		"INSERT INTO repo (desc, date, time, path, tarfile) VALUES (?, ?, ?, ?, ?);";

	//repo (ID INTEGER PRIMARY KEY, desc TEXT, date INTEGER, time INTEGER, path TEXT, tarfile TEXT);

	sqlite3_stmt* stmt = NULL;
	if (sqlite3_prepare_v2(db.handle(), sql, -1, &stmt, NULL) != SQLITE_OK) {
		errlog->warn(UNKNOWN, "Failed to prepare addRepo: %s",
		             sqlite3_errmsg(db.handle()));
		return false;
	}

	sqlite3_bind_text(stmt, 1, desc.c_str(),    -1, SQLITE_TRANSIENT);
	sqlite3_bind_int64(stmt, 2, date.getJulian());
	sqlite3_bind_int64(stmt, 3, time.getTicks());
	sqlite3_bind_text(stmt, 4, path.c_str(),    -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(stmt, 5, tarfile.c_str(), -1, SQLITE_TRANSIENT);

	errlog->debug(NONE, "addRepo prepared statement executing");
	if (!db.exeStmt(stmt)) {
		errlog->warn(UNKNOWN, "Failed to add Restore Job..");
		return false;
	}
	return true;
}

bool fbDatabase::queryBackups()
{
	char buff[256];
	fbDate date;
	fbTime time;

	// BUG-11: the old predicate (date <= today AND time <= now) permanently
	// skipped jobs from earlier dates whose time-of-day is later than now.
	// Correct predicate: due if date is strictly before today, OR it is
	// today and the scheduled time is at or before the current time.
	snprintf(buff, sizeof(buff), "SELECT * FROM backup WHERE date < %ld"
	         " OR (date = %ld AND time <= %ld);",
	         date.getJulian(), date.getJulian(), time.getTicks());

	string cmd = buff;
	errlog->debug(NONE, cmd);

	db.query(cmd);

	errlog->debug(NONE, "Query Done");

	row = 0;

	return true;
}


bool fbDatabase::queryRestore()
{
	string cmd = "select * from restore;";
	errlog->debug(NONE, cmd);

	db.query(cmd);
	errlog->debug(NONE, "Query Done");

	row = 0;

	return true;
}


bool fbDatabase::queryRepo()
{
	string cmd = "select * from repo;";
	errlog->debug(NONE, cmd);

	db.query(cmd);
	errlog->debug(NONE, "Query Done");

	row = 0;

	return true;
}

string fbDatabase::getBackupList()
{
	string out;
	errlog->debug(NONE, "fbDatabase:  Getting List");
	string cmd = "select * from backup;";
	errlog->debug(NONE, cmd);
	db.query(cmd);

	for ( int i = 0; i < db.rows(); i++ )
	{
		 int index =  db.cols() * i;
		out.append(db.table[index++].c_str()); // id
		out.append("\t");
		out.append(db.table[index++].c_str()); //date
		out.append("\t");
		out.append(db.table[index++].c_str()); // time
		out.append("\t");
		out.append(db.table[index++].c_str()); // rt
		out.append("\t");
		out.append(db.table[index++].c_str()); // rv
		out.append("\t");
		out.append(db.table[index++].c_str()); // path
		out.append("\n");
	}
	db.queryDone();
	return out;
}

bool fbDatabase::getBackupRow(string& desc, fbDate& date, fbTime& time, string& path,
		Repeat_type* rt, int* rv, int* id)
{
	errlog->debug(NONE, "fbDatabase:  Getting Backup Rows");

	if(row >= db.rows())
	{
		errlog->debug(NONE, "fbDatabase:  No Backup Rows To Get");
		db.queryDone();
		return false;
	}

	int index =  db.cols() * row;

	errlog->debug(NONE, "fbDatabase:  Found a ROW!");

	*id = atoi(db.table[index++].c_str());
	desc = db.table[index++];
	date.setJulian(atol(db.table[index++].c_str()));
	time.setTicks(atol(db.table[index++].c_str()));
	*rt = (Repeat_type)atoi(db.table[index++].c_str());
	*rv =  atoi(db.table[index++].c_str());
	path = db.table[index++];

	++row;
	errlog->debug(NONE, "fbDatabase:  Got Row");
	return true;
}


bool fbDatabase::getRestoreRow(string& tarfile, string& path, int* id)
{
	errlog->debug(NONE, "fbDatabase:  Getting Restore Rows");

	if(row >= db.rows())
	{
		errlog->debug(NONE, "fbDatabase:  No Restore Rows To Get");
		db.queryDone();
		return false;
	}

	int index =  db.cols() * row;

	errlog->debug(NONE, "fbDatabase:  Found a ROW!");

	*id = atoi(db.table[index++].c_str());
	tarfile = db.table[index++];
	path = db.table[index++];

	++row;
	errlog->debug(NONE, "fbDatabase:  Got Row");
	return true;
}


bool fbDatabase::getRepoRow(string& desc, fbDate& date, fbTime& time, string& path, string& tarfile, int* id)
{
	errlog->debug(NONE, "fbDatabase:  Getting Repo Rows");

	if(row >= db.rows())
	{
		errlog->debug(NONE, "fbDatabase:  No Repo Rows To Get");
		db.queryDone();
		return false;
	}

	int index =  db.cols() * row;

	errlog->debug(NONE, "fbDatabase:  Found a ROW!");

	*id = atoi(db.table[index++].c_str());
	desc = db.table[index++];
	date.setJulian(atol(db.table[index++].c_str()));
	time.setTicks(atol(db.table[index++].c_str()));
	path = db.table[index++];
	tarfile = db.table[index++];

	++row;
	errlog->debug(NONE, "fbDatabase:  Got Row");
	return true;
}


void fbDatabase::ensureAuthSchema()
{
	// Single-row table (id is pinned to 1) holding the authentication policy
	// and, when basic auth is enabled, the username and the PBKDF2 salt/hash.
	// Created if absent so existing databases upgrade transparently.
	string cmd =
		"CREATE TABLE IF NOT EXISTS auth ("
		"id INTEGER PRIMARY KEY CHECK (id = 1), "
		"auth_mode TEXT NOT NULL DEFAULT 'none', "
		"username TEXT, "
		"salt TEXT, "
		"pw_hash TEXT, "
		"iterations INTEGER);";
	if (!db.exe(cmd))
		errlog->warn(UNKNOWN, "Failed to ensure auth table exists");
}

bool fbDatabase::getAuthConfig(string& mode, string& username, string& saltHex, string& hashHex, int& iterations)
{
	// Defaults: authentication disabled.
	mode = "none";
	username = "";
	saltHex = "";
	hashHex = "";
	iterations = 0;

	string cmd = "SELECT auth_mode, username, salt, pw_hash, iterations FROM auth WHERE id = 1;";
	errlog->debug(NONE, cmd);
	db.query(cmd);

	if (db.rows() < 1 || db.cols() < 5)
	{
		db.queryDone();
		return false;   // no row yet — caller uses the "none" defaults
	}

	int idx = 0;   // first (only) row
	mode       = db.table[idx++];
	username   = db.table[idx++];
	saltHex    = db.table[idx++];
	hashHex    = db.table[idx++];
	iterations = atoi(db.table[idx++].c_str());
	db.queryDone();

	if (mode.empty())
		mode = "none";
	return true;
}

bool fbDatabase::setAuthConfig(const string& mode, const string& username, const string& saltHex, const string& hashHex, int iterations)
{
	static const char sql[] =
		"INSERT OR REPLACE INTO auth (id, auth_mode, username, salt, pw_hash, iterations)"
		" VALUES (1, ?, ?, ?, ?, ?);";

	sqlite3_stmt* stmt = NULL;
	if (sqlite3_prepare_v2(db.handle(), sql, -1, &stmt, NULL) != SQLITE_OK) {
		errlog->warn(UNKNOWN, "Failed to prepare setAuthConfig: %s",
		             sqlite3_errmsg(db.handle()));
		return false;
	}

	sqlite3_bind_text(stmt, 1, mode.c_str(),     -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(stmt, 2, username.c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(stmt, 3, saltHex.c_str(),  -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(stmt, 4, hashHex.c_str(),  -1, SQLITE_TRANSIENT);
	sqlite3_bind_int(stmt,  5, iterations);

	if (!db.exeStmt(stmt)) {
		errlog->warn(UNKNOWN, "Failed to save authentication settings");
		return false;
	}
	return true;
}

bool fbDatabase::deleteRow(const char* table, int id)
{
	// Validate table name against allowlist — table names cannot be bound
	// parameters in SQLite, so we hard-code the permitted set here.
	static const char* const allowed[] = { "backup", "restore", "repo", NULL };
	bool valid = false;
	for (int i = 0; allowed[i] != NULL; ++i) {
		if (strcmp(table, allowed[i]) == 0) {
			valid = true;
			break;
		}
	}
	if (!valid) {
		errlog->warn(UNKNOWN, "deleteRow: rejected unknown table '%s'", table);
		return false;
	}

	// Build SQL with the validated literal table name; bind id as a parameter.
	char sql[64];
	snprintf(sql, sizeof(sql), "DELETE FROM %s WHERE ID = ?;", table);

	sqlite3_stmt* stmt = NULL;
	if (sqlite3_prepare_v2(db.handle(), sql, -1, &stmt, NULL) != SQLITE_OK) {
		errlog->warn(UNKNOWN, "Failed to prepare deleteRow: %s",
		             sqlite3_errmsg(db.handle()));
		return false;
	}

	sqlite3_bind_int(stmt, 1, id);
	return db.exeStmt(stmt);
}




