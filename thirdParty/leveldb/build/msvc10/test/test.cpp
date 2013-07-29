// test.cpp : Defines the entry point for the console application.
//

#include <tchar.h>
#include <stdio.h>
#include <Windows.h>
#include <iostream>
#include <conio.h>

#include "leveldb.h"

using std::cout;
using std::endl;

#if defined _DEBUG

#if defined LEVELDB_DLL
#pragma comment(lib,"..\\_exports\\DebugDll\\leveldb_d.lib")
#else
#pragma comment(lib,"..\\_exports\\Debug\\leveldb_d.lib")
#endif

#else

#if defined LEVELDB_DLL
#pragma comment(lib,"..\\_exports\\ReleaseDll\\leveldb.lib")
#else
#pragma comment(lib,"..\\_exports\\Release\\leveldb.lib")
#endif

#endif


#define DB_BENCH

#if defined LEVELDB_DLL
#undef DB_BENCH
#undef DB_TEST
#endif

#if defined DB_BENCH
#include "db\db_bench.cc"
#elif defined DB_TEST
#include "db\db_test.cc"
#else

#ifndef USE_VISTA_API
#define GetTickCount64 GetTickCount
#endif

void WriteDb(leveldb::DB* db)
{
    const char* Content = "We now decided that after three years it might"
        " be a good idea to re-assess our current situation by another,"
        " more extensive survey. It is similar to the original one "
        "(since we included the old questions to be able to compare the results),"
        " but the survey also contains a bunch of new ones to take new developments"
        " such as Ogre usage on mobile devices into account.";
    char Buff[10];
    ZeroMemory(Buff,sizeof(Buff));
    leveldb::WriteOptions wo;
    wo.sync = false;
    for(int i = 0 ; i < 2000 ;i++){
        _itoa_s(i,Buff,10);

        leveldb::Status s = db->Put(wo,Buff,Content);
    }
}

void ReadDb(leveldb::DB* db)
{
    leveldb::ReadOptions ro;
    char Buff[10];
    ZeroMemory(Buff,sizeof(Buff));
    std::string value__;
    for(int i = 0 ; i < 2000 ;i++){
        _itoa_s(i,Buff,10);
        leveldb::Status s = db->Get(ro,Buff,&value__);
    }
}

void tt()
{
    leveldb::DB* db = NULL;
    leveldb::Options options;
    options.create_if_missing = true;
    options.write_buffer_size = 8 * 1024* 1024;
    leveldb::Status status = leveldb::DB::Open(options, "c:/tmp/testdb", &db);
    assert( status.ok() );
    {
        uint64_t tickbegin = GetTickCount64();
        WriteDb(db);
        uint64_t tickend = GetTickCount64();
        cout << "write 2000 in " << tickend - tickbegin  << " Milliseconds"<< endl;
    }
    {
        uint64_t tickbegin = GetTickCount64();
        ReadDb(db);
        uint64_t tickend = GetTickCount64();
        cout << "read 2000 in " << tickend - tickbegin  << " Milliseconds"<< endl;
    }
    delete db;
}

int _tmain(int argc, _TCHAR* argv[])
{
    int repeat = 100;
    for(int i = 0 ; i < repeat ; i++){
        tt();
    } 
    cout << repeat << " times done."<< endl;
    return _getch();
}

#endif