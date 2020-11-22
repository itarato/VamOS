asciis = [
	"NUL",
	"SOH",
	"STX",
	"ETX",
	"EOT",
	"ENQ",
	"ACK",
	"BEL",
	"BS",
	"HT",
	"LF",
	"VT",
	"FF",
	"CR",
	"SO",
	"SI",
	"DLE",
	"DC1",
	"DC2",
	"DC3",
	"DC4",
	"NAK",
	"SYN",
	"ETB",
	"CAN",
	"EM",
	"SUB",
	"ESC",
	"FS",
	"GS",
	"RS",
	"US",
	"space",
	"!",
	"\"",
	"#",
	"$",
	"%",
	"&",
	"'",
	"(",
	")",
	"*",
	"+",
	",",
	"-",
	".",
	"/",
	"0",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	":",
	";",
	"<",
	"=",
	">",
	"?",
	"@",
	"A",
	"B",
	"C",
	"D",
	"E",
	"F",
	"G",
	"H",
	"I",
	"J",
	"K",
	"L",
	"M",
	"N",
	"O",
	"P",
	"Q",
	"R",
	"S",
	"T",
	"U",
	"V",
	"W",
	"X",
	"Y",
	"Z",
	"[",
	"\\",
	"]",
	"^",
	"_",
	"`",
	"a",
	"b",
	"c",
	"d",
	"e",
	"f",
	"g",
	"h",
	"i",
	"j",
	"k",
	"l",
	"m",
	"n",
	"o",
	"p",
	"q",
	"r",
	"s",
	"t",
	"u",
	"v",
	"w",
	"x",
	"y",
	"z",
	"{",
	"|",
	"}",
	"~",
	"DEL",
]

scancodes = {
	0x01 => "ESC",
	0x02 => "1",
	0x03 => "2",
	0x04 => "3",
	0x05 => "4",
	0x06 => "5",
	0x07 => "6",
	0x08 => "7",
	0x09 => "8",
	0x0A => "9",
	0x0B => "0",
	0x0C => "-",
	0x0D => "=",
	0x0E => "BS",
	0x0F => "HT",
	0x10 => "q",
	0x11 => "w",
	0x12 => "e",
	0x13 => "r",
	0x14 => "t",
	0x15 => "y",
	0x16 => "u",
	0x17 => "i",
	0x18 => "o",
	0x19 => "p",
	0x1A => "[",
	0x1B => "]",
	0x1C => "LF",
	0x1e => "a",
	0x1f => "s",
	0x20 => "d",
	0x21 => "f",
	0x22 => "g",
	0x23 => "h",
	0x24 => "j",
	0x25 => "k",
	0x26 => "l",
	0x27 => ";",
	0x28 => "'",
	0x29 => "`",
	0x2B => "\\",
	0x2c => "z",
	0x2d => "x",
	0x2e => "c",
	0x2f => "v",
	0x30 => "b",
	0x31 => "n",
	0x32 => "m",
	0x33 => ",",
	0x34 => ".",
	0x35 => "/",
	0x37 => "*",
	0x39 => "space",
}

puts(256.times.map do |i|
	scancode_char = scancodes[i]
	ascii_idx = if scancode_char != nil
		asciis.find_index(scancode_char) || 0
	else
		0
	end
	"#{ascii_idx}, #{scancode_char ? '/* ' + scancode_char + ' */' : ''}"
end)