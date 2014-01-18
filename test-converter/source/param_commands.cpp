#include "param_commands.h"
#include "file_system.h"

const bool param_source( const string& arg ){
	if( !file_system::file_exists( arg ) ){
		printf( "Source file does not exist.\n" );
		return false;
	};

	params.m_src = arg;
	return true;
};

const bool param_destination( const string& arg ){
	if( file_system::file_exists( arg ) && !file_system::delete_file( arg ) ){
		printf( "Destination path is buisy.\n" );
		return false;
	};

	file_system::path_t path( arg, "/" );
	if( !( file_system::path_exists( path.path() ) || file_system::make_folder( path.path() ) ) ){
		printf( "Can not create destination folder.\n" );
		return false;
	};

	params.m_dst = arg;
	return true;
};
