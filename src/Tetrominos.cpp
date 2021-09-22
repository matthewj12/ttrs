#include "GlobalVariables.h"

#include <vector>
#include <string>
#include <array>

using namespace std;
// A list holding all of the permutaions (distinct orientations) of a given tetromino

array<tetromino_t, 7> tetrominos{{
	{{
		{
			"    ",
			"    ",
			"  GG",
			" GG "
		},
		{
			"    ",
			"  G ",
			"  GG",
			"   G"
		} 
	}},
	
	{{
		{
			"    ",
			"    ",
			" RR ",
			"  RR"
		},
		{
			"    ",
			"  R ",
			" RR ",
			" R  "
		} 
	}},
	
	{{
		{
			"    ",
			"    ",
			"CCCC",
			"    "
		},
		{
			"  C ",
			"  C ",
			"  C ",
			"  C "
		} 
	}},
	
	{{
		{
			"    ",
			"    ",
			" YY ",
			" YY "
		}
	}},
	
	{{
		{
			"    ",
			"    ",
			" BBB",
			"   B"
		},
		{
			"    ",
			"  B ",
			"  B ",
			" BB "
		},
		{
			"    ",
			" B  ",
			" BBB",
			"    "
		},
		{
			"    ",
			"  BB",
			"  B ",
			"  B "
		}
	}},
	
	{{
		{
			"    ",
			"    ",
			" OOO",
			" O  "
		},
		{
			"    ",
			" OO ",
			"  O ",
			"  O "
		},
		{
			"    ",
			"   O",
			" OOO",
			"    "
		},
		{
			"    ",
			"  O ",
			"  O ",
			"  OO"
		}
	}},
	
	{{
		{
			"    ",
			"    ",
			" MMM",
			"  M "
		},
		{
			"    ",
			"  M ",
			" MM ",
			"  M "
		},
		{
			"    ",
			"  M ",
			" MMM",
			"    "
		},
		{
			"    ",
			"  M ",
			"  MM",
			"  M "
		}
	}}
}};
