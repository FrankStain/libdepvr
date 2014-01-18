#pragma once

namespace pvr {
	template< compression_t >
	class modulation_unit_t {
	public:
		inline void push( const pk_region_t& cs, const uint32_t left, const uint32_t top );
		inline const uint32_t modulation( const uint32_t left, const uint32_t top ) const;
	};

	template<>
	class modulation_unit_t<ct_2bpp> {
	private:
		uint32_t			m_values[16][8];
		uint32_t			m_modes[16][8];

	public:
		modulation_unit_t() {};

		inline void push( const pk_region_t& cs, const uint32_t left, const uint32_t top );
		inline const uint32_t modulation( const uint32_t left, const uint32_t top ) const;
	};

	template<>
	class modulation_unit_t<ct_4bpp> {
	private:
		uint32_t			m_values[8][8];

	public:
		modulation_unit_t() {};

		inline void push( const pk_region_t& cs, const uint32_t left, const uint32_t top );
		inline const uint32_t modulation( const uint32_t left, const uint32_t top ) const;
	};
};
