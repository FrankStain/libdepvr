#pragma once

#include "../type_configuration.h"
#include "pvr_format.h"
#include "pvr_types.h"
#include "pvr_color_unit.h"
#include "pvr_modulation_unit.h"
#include "pvr_deflate_unit.h"
#include "pvr_unpack_unit.h"
#include "pvr_operations.h"

namespace pvr {
	inline const uint32_t decompress( const uint8_t* src, const compression_t mode, const int32_t dst_width, const int32_t dst_height, uint8_t* dst ){
		if( ct_undefined == mode ){
			return 0;
		};
		
		basic_unpack_unit_t* uuptr	= NULL;
		color32_t* tx_pix			= (color32_t*)dst;
		pk_region_t* regs			= (pk_region_t*)src;
		const int32_t tx_width		= max( dst_width, ( ct_2bpp == mode )? 16 : 8 );
		const int32_t tx_height		= max( dst_height, 8 );

		if( ct_4bpp == mode ){
			uuptr = new unpack_unit_t<ct_4bpp>( tx_width, tx_height );
		}else{
			uuptr = new unpack_unit_t<ct_2bpp>( tx_width, tx_height );
		};

		if( ( dst_width ^ tx_width ) | ( dst_height ^ tx_height ) ){
			tx_pix = new color32_t[ tx_width * tx_height ];
		};

		basic_unpack_unit_t& uu = *uuptr;
		while( uu ){
			uu++;
			uu.push( regs );
			uu.pop( tx_pix );
		};

		if( (uint8_t*)tx_pix != dst ){
			const uint32_t row_size = dst_width * sizeof( color32_t );
			
			for( int32_t py = 0; dst_height > py; py++ ){
				memcpy( dst + ( py * row_size ), &tx_pix[ tx_width * py ], row_size );
			};
			
			delete[] tx_pix;
		};

		const uint32_t tex_stride = uu.processed();
		delete uuptr;
		return tex_stride;
	};
};

namespace etc {
	inline const uint32_t decompress(){
		return 0;
	};
};
