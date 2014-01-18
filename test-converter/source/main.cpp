#include <windows.h>
#include <stdio.h>
#include "type_configuration.h"
#include "file_system.h"
#include "pvr_decompress.h"
#include "targa.h"
#include "param_parser.h"
#include "param_commands.h"

const int convert_texture();

param::command_t param_list[] = {
	{ 's', "source", "Source PVR-TC formated texture", param_source },
	{ 'd', "dest", "Path to decompressed TGA texture", param_destination },
	{ 'v', "version", "File version", param_version },
	{ 'h', "help", "Print this messages", param_help },
	{ 0 }
};

param::value_t	params;

int main( int argc, char* argv[] ){
	param::parser_t cmd_parser( param_list );
	params.m_process = true;

	if( !cmd_parser.parse( argc, argv ) ){
		printf( "Wrong command syntax.\n" );
		param_help( "" );
		return -1;
	}else{
		if( params.m_src.empty() ){
			printf( "Source file must be specified\n" );
			return -1;
		}else if( params.m_dst.empty() ){
			printf( "Destination file must be specified\n" );
			return -1;
		}else if( params.m_process ){
			return convert_texture();
		};
	};

	return 0;
};

const int convert_texture(){
	file_system::file_mapping_t file;
	if( !file.construct( params.m_src ) ){
		printf( "Can not open source file. Is it locked?\n" );
		return -1;
	};

	uint8_t* memory;
	if( !file.map_view( memory, 0, 0 ) ){
		printf( "Can not open source file. Is it locked or empty?\n" );
		return -1;
	};

	pvr::header_t* hdr = (pvr::header_t*)memory;
	if( !hdr->is_valid() ){
		printf( "Source file is not a valid PVR-TC texture.\n" );
		return -1;
	};
			
	uint8_t* tex_data = NULL;
	pvr::compression_t mode = pvr::ct_undefined;
	image_desc_t img;

	if( hdr->m_v2.is_valid() ){
		tex_data = memory + pvr::v2::header_file_size;
		img.set_sizes( hdr->m_v2.m_width, hdr->m_v2.m_height );

		switch( hdr->m_v2.m_flags.m_format )
		{
			case pvr::v2::OGL_PVRTC2:
			case pvr::v2::MGLPT_PVRTC2:
				mode = pvr::ct_2bpp;
			break;
			case pvr::v2::OGL_PVRTC4:
			case pvr::v2::MGLPT_PVRTC4:
				mode = pvr::ct_4bpp;
			break;
		};
	}else{
		tex_data = memory + pvr::v3::header_file_size;
		img.set_sizes( hdr->m_v3.m_width, hdr->m_v3.m_height );

		switch( hdr->m_v3.m_format.m_value )
		{
			case pvr::v3::ePVRTPF_PVRTCI_2bpp_RGBA:
			case pvr::v3::ePVRTPF_PVRTCI_2bpp_RGB:
				mode = pvr::ct_2bpp;
			break;
			case pvr::v3::ePVRTPF_PVRTCI_4bpp_RGBA:
			case pvr::v3::ePVRTPF_PVRTCI_4bpp_RGB:
				mode = pvr::ct_4bpp;
			break;
		};
	};

	if( pvr::ct_undefined == mode ){
		printf( "Current texture format is unsupported. Only PVR-TC2 and PVR-TC4 formats supported.\n" );
		return -1;
	};

	if( !pvr::decompress( tex_data, mode, img.width(), img.height(), (uint8_t*)img.memory() ) ){
		printf( "Current texture format is broken. Is this a valid PVR-TC2 or PVR-TC4 texture.\n" );
		return -1;
	};

	if( !tga::save( params.m_dst, img ) ){
		printf( "Can not save destination file.\n" );
		return -1;
	};

	return 0;
};

const bool param_help( const string& arg ){
	printf( "List of usable params : \n" );
	for( param::command_t* cmd = param_list; cmd->m_litera; cmd++ ){
		printf( "  -%c or --%s\t %s\n", cmd->m_litera, cmd->m_name.c_str(), cmd->m_desc.c_str() );
	};
	
	params.m_process = false;
	return true;
};

const bool param_version( const string& arg ){
	printf( "test-convert for lbdepvr testing - v1.0\n" );
	params.m_process = false;
	return true;
};