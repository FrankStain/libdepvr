#pragma once

namespace pvr {
	template< compression_t >
	class color_unit_t {
	public:
		inline void push( const pk_color_set_t& lt, const pk_color_set_t& rt, const pk_color_set_t& lb, const pk_color_set_t& rb );

		inline const ex_color_state_t* const memory() const;
		inline const ex_color_state_t& operator [] ( const uint32_t index ) const;
	};

	template<>
	class color_unit_t<ct_2bpp> {
	private:
		ex_color_state_t	m_stream[32];

	public:
		color_unit_t() {};

		inline void push( const pk_color_set_t& lt, const pk_color_set_t& rt, const pk_color_set_t& lb, const pk_color_set_t& rb );

		inline const ex_color_state_t* const memory() const { return m_stream; };
		inline const ex_color_state_t& operator [] ( const uint32_t index ) const { return m_stream[ index ]; };
	};

	template<>
	class color_unit_t<ct_4bpp> {
	private:
		ex_color_state_t	m_stream[16];

	public:
		color_unit_t() {};

		inline void push( const pk_color_set_t& lt, const pk_color_set_t& rt, const pk_color_set_t& lb, const pk_color_set_t& rb );

		inline const ex_color_state_t* const memory() const { return m_stream; };
		inline const ex_color_state_t& operator [] ( const uint32_t index ) const { return m_stream[ index ]; };
	};
};
