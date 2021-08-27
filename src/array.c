/* Retreive single precision floats stored as 4 bytes blog. */

#include "sqlite3ext.h"
SQLITE_EXTENSION_INIT1

#include <string.h>

static void nbytes( sqlite3_context *context, int argc, sqlite3_value **argv) {
    sqlite3_result_int(context, sqlite3_value_bytes(argv[0]));
}

static void float32func( sqlite3_context *context, int argc, sqlite3_value **argv) {
    float a=0.0;
    int   nbytes;

    /* if( sqlite3_value_type(argv[0])!=SQLITE_BLOB )
        return sqlite3_result_null(context); */

    nbytes = sqlite3_value_bytes(argv[0]);
    if( nbytes!=4 ) {
        sqlite3_result_null(context);
        return;
    }

    memcpy(&a, sqlite3_value_blob(argv[0]), 4);
    sqlite3_result_double(context, (double) a); 
}

static void float32idx( sqlite3_context *context, int argc, sqlite3_value **argv) {
    float a=0.0;
    int   idx,nbytes;
    
    nbytes = sqlite3_value_bytes(argv[0]);
    if( (nbytes%4)!=0 ) {
        sqlite3_result_null(context);
        return;
    }

    idx = sqlite3_value_int(argv[1]);
    if( idx<0 || idx*4>nbytes ) {
        sqlite3_result_null(context);
        return;
    }

    memcpy(&a, sqlite3_value_blob(argv[0]) + idx*4, 4);
    sqlite3_result_double(context, (double) a);
}

static void float64idx( sqlite3_context *context, int argc, sqlite3_value **argv) {
    int idx,nbytes;
    
    nbytes = sqlite3_value_bytes(argv[0]);
    if( (nbytes%8)!=0 )
        return sqlite3_result_null(context);
    
    idx = sqlite3_value_int(argv[1]);
    if( idx<0 || idx*8>nbytes )
        return sqlite3_result_null(context);

    sqlite3_result_double(context, *((double*) sqlite3_value_blob(argv[0]) + idx));
}

static void int64idx( sqlite3_context *context, int argc, sqlite3_value **argv) {
    int  idx,nbytes;

    nbytes = sqlite3_value_bytes(argv[0]);
    if( (nbytes%8)!=0 )
        return sqlite3_result_null(context);

    idx = sqlite3_value_int(argv[1]);
    if( idx<0 || idx*8>nbytes )
        return sqlite3_result_null(context);

    sqlite3_result_int64(context, *((sqlite3_int64*) sqlite3_value_blob(argv[0]) + idx));
}

static void int32idx( sqlite3_context *context, int argc, sqlite3_value **argv) {
    int idx,nbytes;
    
    nbytes = sqlite3_value_bytes(argv[0]);
    if( (nbytes%4)!=0 )
        return sqlite3_result_null(context);

    idx = sqlite3_value_int(argv[1]);
    if( idx<0 || idx*4>nbytes )
        return sqlite3_result_null(context);

    sqlite3_result_int(context, *((int*) sqlite3_value_blob(argv[0]) + idx));
}

static void int16idx( sqlite3_context *context, int argc, sqlite3_value **argv) {
    int a=0;
    int idx,nbytes;

    nbytes = sqlite3_value_bytes(argv[0]);
    if( (nbytes%2)!=0 )
        return sqlite3_result_null(context);

    idx = sqlite3_value_int(argv[1]);
    if( idx<0 || idx*2>nbytes )
        return sqlite3_result_null(context);

    memcpy(&a, sqlite3_value_blob(argv[0]) + idx*2, 2);
    sqlite3_result_int(context, a);
}

static void byte( sqlite3_context *context, int argc, sqlite3_value **argv) {
    int a=0;
    int idx,nbytes;

    nbytes = sqlite3_value_bytes(argv[0]);

    idx = sqlite3_value_int(argv[1]);
    if( idx<0 || idx>nbytes )
        return sqlite3_result_null(context);

    memcpy(&a, sqlite3_value_blob(argv[0]) + idx, 1);
    sqlite3_result_int(context, a);
}

#ifdef _WIN32
__declspec(dllexport)
#endif
int sqlite3_array_init( /* <== Change this name, maybe */
        sqlite3 *db, 
        char **pzErrMsg, 
        const sqlite3_api_routines *pApi
        ){
    int rc = SQLITE_OK;
    SQLITE_EXTENSION_INIT2(pApi);
    /* insert code to initialize your extension here */
    rc = sqlite3_create_function(
            db, "nbytes", 1, SQLITE_UTF8 | SQLITE_DETERMINISTIC | SQLITE_INNOCUOUS,
            0 /* no user data */, nbytes, 0, 0);
    rc = sqlite3_create_function(
            db, "float32", 1, SQLITE_UTF8 | SQLITE_DETERMINISTIC | SQLITE_INNOCUOUS,
            0 /* no user data */, float32func, 0, 0);
    rc = sqlite3_create_function(
            db, "float32", 2, SQLITE_UTF8 | SQLITE_DETERMINISTIC | SQLITE_INNOCUOUS,
            0 /* no user data */, float32idx, 0, 0);
    rc = sqlite3_create_function(
            db, "float64", 2, SQLITE_UTF8 | SQLITE_DETERMINISTIC | SQLITE_INNOCUOUS,
            0 /* no user data */, float64idx, 0, 0);
    rc = sqlite3_create_function(
            db, "int64", 2, SQLITE_UTF8 | SQLITE_DETERMINISTIC | SQLITE_INNOCUOUS,
            0 /* no user data */, int64idx, 0, 0);
    rc = sqlite3_create_function(
            db, "int32", 2, SQLITE_UTF8 | SQLITE_DETERMINISTIC | SQLITE_INNOCUOUS,
            0 /* no user data */, int32idx, 0, 0);
    rc = sqlite3_create_function(
            db, "int16", 2, SQLITE_UTF8 | SQLITE_DETERMINISTIC | SQLITE_INNOCUOUS,
            0 /* no user data */, int16idx, 0, 0);
    rc = sqlite3_create_function(
            db, "byte", 2, SQLITE_UTF8 | SQLITE_DETERMINISTIC | SQLITE_INNOCUOUS,
            0 /* no user data */, byte, 0, 0);
    return rc;
}
