#!/usr/bin/env python3
##
## Copyright (c) 2023, Daniel Tabor
## All rights reserved.
## 
## Redistribution and use in source and binary forms, with or without
## modification, are permitted provided that the following conditions are met:
## 
## 1. Redistributions of source code must retain the above copyright notice, this
##    list of conditions and the following disclaimer.
## 
## 2. Redistributions in binary form must reproduce the above copyright notice,
##    this list of conditions and the following disclaimer in the documentation
##    and/or other materials provided with the distribution.
## 
## THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
## AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
## IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
## DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
## FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
## DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
## SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
## CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
## OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
## OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
##
import sys

class Progress:
	PROGRESS_TYPE_ASCII=0
	PROGRESS_TYPE_BLOCK=1
	PROGRESS_TYPE_BAR=2
	PROGRESS_TYPE_FADE=3
	PROGRESS_TYPE_PIE=4
	PROGRESS_TYPE_FALLING=5
	
	_fgcolors = [30, 31, 32, 33, 34, 35, 36, 37,  90,  91,  92,  93,  94,  95,  96,  97]
	_bgcolors = [40, 41, 42, 43, 44, 45, 46, 47, 100, 101, 102, 103, 104, 105, 106, 107]

	_progress_tiles = [
		[" ","/","-","\\","|","#"],
		[bytes([0x20]).decode(),           # 0x00020
		 bytes([0xE2,0x96,0x8F]).decode(), # 0x0258F
		 bytes([0xE2,0x96,0x8E]).decode(), # 0x0258E
		 bytes([0xE2,0x96,0x8D]).decode(), # 0x0258D
		 bytes([0xE2,0x96,0x8C]).decode(), # 0x0258C
		 bytes([0xE2,0x96,0x8B]).decode(), # 0x0258B
		 bytes([0xE2,0x96,0x8A]).decode(), # 0x0258A
		 bytes([0xE2,0x96,0x89]).decode(), # 0x02589
		 bytes([0xE2,0x96,0x88]).decode(), # 0x02588
		],
		[bytes([0x20]).decode(),           # 0x00020
		 bytes([0xE2,0x96,0x81]).decode(), # 0x02581
		 bytes([0xE2,0x96,0x82]).decode(), # 0x02582
		 bytes([0xE2,0x96,0x83]).decode(), # 0x02583
		 bytes([0xE2,0x96,0x84]).decode(), # 0x02584
		 bytes([0xE2,0x96,0x85]).decode(), # 0x02585
		 bytes([0xE2,0x96,0x86]).decode(), # 0x02586
		 bytes([0xE2,0x96,0x87]).decode(), # 0x02587
		 bytes([0xE2,0x96,0x88]).decode(), # 0x02588
		],
		[bytes([0x20]).decode(),          # 0x00020
		 bytes([0xE2,0x96,0x91]).decode(), # 0x02591
		 bytes([0xE2,0x96,0x92]).decode(), # 0x02592
		 bytes([0xE2,0x96,0x93]).decode(), # 0x02593
		 bytes([0xE2,0x96,0x88]).decode(), # 0x02588
		],
		[bytes([0xE2,0x97,0x8B]).decode(), # 0x025CB
		 bytes([0xE2,0x97,0x94]).decode(), # 0x025D4
		 bytes([0xE2,0x97,0x91]).decode(), # 0x025D1
		 bytes([0xE2,0x97,0x95]).decode(), # 0x025D5
		 bytes([0xE2,0x97,0x8F]).decode(), # 0x025CF
		],
		[bytes([0x20]).decode(),                # 0x00020
		 bytes([0xF0,0x9F,0xAC,0x80]).decode(), # 0x1FB00
		 bytes([0xF0,0x9F,0xAC,0x83]).decode(), # 0x1FB03
		 bytes([0xF0,0x9F,0xAC,0x8F]).decode(), # 0x1FB0F
		 bytes([0xF0,0x9F,0xAC,0x90]).decode(), # 0x1FB10
		 bytes([0xF0,0x9F,0xAC,0x93]).decode(), # 0x1FB13
		 bytes([0xE2,0x96,0x8C]).decode(),      # 0x0258C
		 bytes([0xF0,0x9F,0xAC,0x95]).decode(), # 0x1FB15
		 bytes([0xF0,0x9F,0xAC,0x9B]).decode(), # 0x1FB1B
		 bytes([0xF0,0x9F,0xAC,0xB2]).decode(), # 0x1FB32
		 bytes([0xF0,0x9F,0xAC,0xB4]).decode(), # 0x1FB34
		 bytes([0xF0,0x9F,0xAC,0xBA]).decode(), # 0x1FB3A
		 bytes([0xE2,0x96,0x88]).decode(),      # 0x02588
		]
	]
	
	def __init__(self, progress_type, size):
		progress_type = int(progress_type)
		if progress_type < 0 or progress_type > len(self._progress_tiles):
			raise ValueError("progress_type must be Progress.PROGRESS_TYPE_...")
		size = int(size)
		if size <= 0:
			raise ValueError("size must be greater than 0")

		self._tiles = self._progress_tiles[progress_type]
		self._use_color = False
		self._fg = 0
		self._bg = 0
		self._size = size
		self._fp = sys.stdout
		self._min = 0
		self._max = size*(len(self._tiles)-1)

	def set_range(self, minimum, maximum):
		minimum = int(minimum)
		maximum = int(maximum)
		if maximum < minimum:
			raise ValueError("maximum must be at least minimum")
		if maximum == 0:
			raise ValueError("maximum must be greater than 0")
		self._min = minimum
		self._max = maximum

	def set_colors(self, fgcolor, bgcolor):
		fgcolor = int(fgcolor)
		if fgcolor < 0 or fgcolor > 15:
			raise ValueError("Foreground color must be 0-15")
		bgcolor = int(bgcolor)
		if bgcolor < 0 or bgcolor > 15:
			raise ValueError("Background color must be 0-15")
		self._use_color = True
		self._fg = self._fgcolors[fgcolor]
		self._bg = self._bgcolors[bgcolor]

	def set_output(self, fp):
		self._fp = fp
		
	def render(self, value):
		value = int(value)
		units_per_char = len(self._tiles)-1
		full_char = self._tiles[-1]
		empty_char = self._tiles[0]

		if value < self._min:
			units = 0
		elif value > self._max:
			units = self._max * self._size * units_per_char
		else:
			units = ((value-self._min) * self._size * units_per_char) / (self._max - self._min)
		units = int(units)

		if self._use_color:
			self._fp.write(bytes([0x1b,0x5b]).decode() + ("%d;%dm" % (self._fg,self._bg)))
		for i in range(self._size):
			if units >= (i+1)*units_per_char:
				c = full_char
			elif units < i*units_per_char:
				c = empty_char
			else:
				c = self._tiles[units%units_per_char]
			self._fp.write(c)
		
		if self._use_color:
			self._fp.write(bytes([0x1b,0x5b,0x30,0x6d]).decode())



## Demo Code
if __name__ == "__main__":
	import time
	progress_bars = []
	
	# Initialize 4 progress bars
	p = Progress(Progress.PROGRESS_TYPE_ASCII, 12)
	p.set_range( 0, 100 )
	progress_bars.append(p)
	
	p = Progress(Progress.PROGRESS_TYPE_BLOCK, 20)
	p.set_range( 0, 100 )
	p.set_colors( 2, 0 )
	progress_bars.append(p)
		
	p = Progress(Progress.PROGRESS_TYPE_BAR, 15)
	p.set_range( 50, 150 )
	p.set_colors( 9, 15 )
	progress_bars.append(p)

	p = Progress(Progress.PROGRESS_TYPE_FADE, 7)
	p.set_range( 100, 200 )
	p.set_colors( 11, 0 )
	progress_bars.append(p)

	p = Progress(Progress.PROGRESS_TYPE_PIE, 5)
	p.set_range( 100, 200 )
	p.set_colors( 0, 11 )
	progress_bars.append(p)
	
	p = Progress(Progress.PROGRESS_TYPE_FALLING, 10)
	p.set_range( 0, 200 )
	p.set_colors( 12, 0 )
	progress_bars.append(p)

	# Increment and render each progress bar
	# Some will render prior to their minimum being reached
	# All will render past their maximum being reached
	for pv in range(210):
		for p in progress_bars:
			sys.stdout.write("%03d/%03d/%03d (" % (p._min,pv,p._max))
			p.render(pv)
			sys.stdout.write(")\n")
		sys.stdout.flush()
		time.sleep(0.1)
		sys.stdout.write(bytes([0x1b,0x5b,0x36,0x41]).decode())
	sys.stdout.write(bytes([0x1b,0x5b,0x36,0x42]).decode())
	
