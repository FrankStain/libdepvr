#pragma once

namespace pvr {
	template< compression_t mtd >
	class color_unit_t {
	private:
		typedef constants<mtd> c;
		
		ex_color_state_t	m_stream[ c::deflate_stream_size ];
		ex_color_t			m_reg[8][2];

		inline void process_stream();

	public:
		inline void push( const pk_color_set_t& lt, const pk_color_set_t& rt, const pk_color_set_t& lb, const pk_color_set_t& rb ){
			memset( m_reg, 0, sizeof( m_reg ) );

			m_reg[0][0] = lt.m_colors[0];
			m_reg[0][1] = lt.m_colors[1];
			m_reg[1][0] = rt.m_colors[0];
			m_reg[1][1] = rt.m_colors[1];
			m_reg[2][0] = lb.m_colors[0];
			m_reg[2][1] = lb.m_colors[1];
			m_reg[3][0] = rb.m_colors[0];
			m_reg[3][1] = rb.m_colors[1];

			m_reg[4][0] = m_reg[1][0] - m_reg[0][0];
			m_reg[4][1] = m_reg[1][1] - m_reg[0][1];
			m_reg[5][0] = m_reg[3][0] - m_reg[2][0];
			m_reg[5][1] = m_reg[3][1] - m_reg[2][1];

			m_reg[0][0] *= c::width;
			m_reg[0][1] *= c::width;
			m_reg[2][0] *= c::width;
			m_reg[2][1] *= c::width;

			process_stream();
		};

		inline const ex_color_state_t* const memory() const							{ return m_stream; };
		inline const ex_color_state_t& operator [] ( const uint32_t index ) const	{ return m_stream[ index ]; };
	};
};
