#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h> 
#include <fcntl.h> 
#include <stdbool.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <assert.h>
#include <sqlite3.h>

#define SERVER_DATA_STORAGE "message.db"

static int callback(void *data, int argc, char **argv, char **azColName) {
   int i;
   fprintf(stderr, "%s: ", (const char*)data);
   
   for(i = 0; i<argc; i++) {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}


void PrintDB(sqlite3* db,const char* data){
    char* printDB = "select * from MessageDB;";
    char* errorMessage = 0;
    int sqlCallInfo = sqlite3_exec(db, printDB, callback, (void*)data, &errorMessage);
    if (sqlCallInfo != SQLITE_OK ){
      
      fprintf(stderr, "Sql print error: %s \n",errorMessage);
      sqlite3_free(errorMessage);
      sqlite3_close(db);
    }
    sqlite3_free(errorMessage);
}




int main(){
    sqlite3* db;

    char* nameDB = "MessageDB";

    const char* data = "Callback function called";

    int sqlCallInfo =  sqlite3_open(SERVER_DATA_STORAGE, &db);
    if (sqlCallInfo !=  SQLITE_OK){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return 1;
    }
    else{
      fprintf(stderr, "Opened database successfully\n");
    }
    
    char* creatTableRequest = "DROP TABLE IF EXISTS MessageDB;"\
                              "CREATE TABLE MessageDB(Id INT,Name TEXT, Message TEXT);"\
                              "INSERT INTO MessageDB(Name) VALUES ('Tom');";

    char* errorMessage = 0;

    sqlCallInfo  = sqlite3_exec(db, creatTableRequest, 0, 0,&errorMessage);

    if (sqlCallInfo != SQLITE_OK ){
      
      fprintf(stderr, "Sql error: %s \n",errorMessage);

      sqlite3_free(errorMessage);
      sqlite3_close(db);

      return 2;
    }


    PrintDB(db,data);
    sqlite3_close(db);
    return 0;
}