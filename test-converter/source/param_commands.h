#pragma once

#include "type_configuration.h"

namespace param {
	struct value_t {
		bool	m_process;
		string	m_src;
		string	m_dst;
	};
};

const bool param_source( const string& );
const bool param_destination( const string& );
const bool param_help( const string& );
const bool param_version( const string& );

extern param::value_t	params;
