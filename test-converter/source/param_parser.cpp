#include <iterator>
#include "param_parser.h"

namespace param {
	parser_t::parser_t( command_t* list ){
		memset( m_literal_hash, 0, sizeof( m_literal_hash ) );
		while( list->m_litera ){
			m_params.push_back( *list );
			list++;
		};

		for( cmd_list_t::iterator pd = m_params.begin(); m_params.end() != pd; pd++ ){
			command_t& cmd = *pd;
			m_names_hash[ cmd.m_name ] = m_literal_hash[ cmd.m_litera ] = &cmd;
		};
	};

	parser_t::parser_t( cmd_list_t& list ){
		memset( m_literal_hash, 0, sizeof( m_literal_hash ) );
		copy( list.begin(), list.end(), insert_iterator<cmd_list_t>( m_params, m_params.begin() ) );
		for( cmd_list_t::iterator pd = m_params.begin(); m_params.end() != pd; pd++ ){
			command_t& cmd = *pd;
			m_names_hash[ cmd.m_name ] = m_literal_hash[ cmd.m_litera ] = &cmd;
		};
	};

	parser_t::~parser_t(){
		m_names_hash.clear();
		memset( m_literal_hash, 0, sizeof( m_literal_hash ) );
		m_params.clear();
	};

	bool parser_t::parse( const int arg_count, char** arg_list ){
		string arg_value;
		command_t* desc = NULL;
	
		for( int ap = 1; arg_count > ap; ap++ ){
			arg_value = arg_list[ ap ];

			if( ( 1 < arg_value.length() ) && ( '-' == arg_value[0] ) ){
				if( desc && desc->m_operator && !desc->m_operator( string( "" ) ) ) return false;

				if( '-' == arg_value[1] ){
					string arg_name = arg_value.substr( 2 );
					cmd_hash_t::iterator hash = m_names_hash.find( arg_name );
					if( hash != m_names_hash.end() ){
						desc = hash->second;
					};
				}else{
					string arg_name = arg_value.substr( 1 );
					if( 1 == arg_name.length() ){
						desc = m_literal_hash[ arg_name[0] ];
					};
				};
			}else{
				if( desc ){
					if( desc->m_operator && !desc->m_operator( arg_value ) ) return false;
					desc = NULL;
				};
			};
		};

		if( desc && desc->m_operator && !desc->m_operator( string( "" ) ) ) return false;

		return true;
	};
};
