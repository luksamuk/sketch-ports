#pragma once
#include <OficinaFramework/OficinaTypes.hpp>
#include <OficinaFramework/RenderingSystem.hpp>
#include <list>
using namespace OficinaFramework;

void BuildLevel(const std::vector<int> &indexes);

const std::vector<int> levels[10] =
{
	// Level 0: Starter
	{ 84,     87,    90 },

	// Level 1: Three Ways
	{ 0,  1,  2,  3,          6,  7,  8,  9,         12, 13, 14, 15,
	 16, 17, 18, 19,         22, 23, 24, 25,         28, 29, 30, 31 },

	// Level 2: Diamond
	{               23,
	           38,  39,  40,
	      53,  54,  55,  56, 57,
	  68, 69,  70,  71,  72, 73, 74,
	      85,  86,  87,  88, 89,
	          102, 103, 104,
	               119 },

	// Level 3: Smiley
	{                     6,   7,   8,
	                21,  22,  23,  24,  25,
	                37,       39,       41,
	           52,  53,       55,       57,  58,
	           68,  69,  70,  71, 72,   73,  74,
	           84,            87,            90,
	          100, 101,                105, 106,
	               117, 118,      120, 121,
	                    134, 135, 136,
	                         151 },

	// Level 4: Stickmen
	{         2,                       7,                      12,
	    17,       19,            22,       24,            27,       29,
	    33,       35,  		     38,       40,            43,       45,
	          50,   			      55,                      60,
	          66,					  71,                      76,
	     81,  82,  83,			 86,  87,  88,            91,  92,  93,
	 96,	  98,      100, 101,      103,      105, 106,      108,      110,
	          114, 			          119,                     124,
	      129,	  131,			 134,      136,           139,      141,
	      145,	  147,			 150,      152,           155,      157,
	      161,	  163,			 166,      168,           171,      173 },

	// Level 5: Stairway to Heaven
	{ 0,  1,  2,  3,  4,  5,  6,               10, 11, 12,
	16, 17, 18, 19, 20, 21,               25, 26, 27,                31,
	32, 33, 34, 35, 36,               40, 41, 42,               46,  47,
	48, 49, 50, 51,               55, 56, 57,              61,  62,  63,
	64, 65, 66,               70, 71, 72,             76,  77,  78,  79,
	80, 81, 	    	  85, 86, 87,            91,  92,  93,  94,  95,
	96, 	    	100, 101, 102,         106, 107, 108, 109, 110, 111,
	115, 116, 117,         121, 122, 123, 124, 125, 126, 127 },

	// Level 6: Fortress
	{ 0,   1,             4,   5,   6,   7,   8,   9,  10,  11,            14,  15,
	16,  17,            20,                                27,            30,  31,
	32,  33,            36,       38,  39,  40,  41,       43,            46,  47,
	48,  49,            52,       54,  55,  56,  57,       59,            62,  63,
	64,  65,	         68,	   70,  71,  72,  73,       75,            78,  79,
	80,  81,            84,       86,  87,  88,  89,       91,            94,  95,
	96,  97,           100,      102, 103, 104, 105,      107,           110, 111,
	112, 113,	        116,                               123,           126, 127,
	128, 129,	        132, 133, 134, 135, 136, 137, 138, 139,           142, 143,

	160, 161, 162, 163,	164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175 },

	// Level 7: Bom & Arrow
	{ 0,        2,                       7,
	17,       19,                 23,  24,  25,
	32,       34,       36,            39,            42,            45,
	49,       51,       53,       55,            58,            61,  62,
	64,  65,  66,  67,  68,  69,  70,  71,  72,  73,  74,  75,  76,  77,  78,  79,
	81,       83,       85,       87,            90,            93,  94,
	96,       98,      100,           103,           106,            109,
	113,      115,                119, 120, 121,
	128,      130,                     135 },

	// Level 8: Full House
	{ 0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,
	16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,
	32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,
	48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,
	64,  65,  66,  67,  68,  69,  70,  71,  72,  73,  74,  75,  76,  77,  78,  79,
	80,  81,  82,  83,  84,  85,  86,  87,  88,  89,  90,  91,  92,  93,  94,  95,
	96,  97,  98,  99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111,
	112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127,
	128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143,
	144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159,
	160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175 },

	// Level 9: Bye-Bye
	{ 34,  35,            38,       40,       42,  43,  44,
	  50,       52,       54,       56,       58,
	  66,  67,                 71,            74,  75,  76,
	  82,       84,            87,            90,
	  98,  99,                103,           106, 107, 108 }
};
