#pragma once

#include "type_configuration.h"

namespace param {
	typedef const bool (*parser_func_t)( const string& );

	struct command_t {
		char			m_litera;
		std::string		m_name;
		std::string		m_desc;
		parser_func_t	m_operator;
	};

	typedef vector<command_t> cmd_list_t;

	class parser_t {
		private:
			typedef map<std::string, command_t*>	cmd_hash_t;
			
			cmd_list_t	m_params;
			cmd_hash_t	m_names_hash;
			command_t*	m_literal_hash[256];
		
		public:
			parser_t( command_t* );
			parser_t( cmd_list_t& );
			virtual ~parser_t();

			bool parse( const int, char** );
			const int count() const { return m_params.size(); };
			const command_t& operator [] ( int index ) const { return m_params[ index ]; };
	};
};
