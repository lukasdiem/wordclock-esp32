//////////////////////////////////////////////////
// Mutex support for ESP8266.
// Copyright 2015 Richard A Burton
// richardaburton@gmail.com
// See license.txt for license terms.
// ----------------------------------------------
//
// The MIT License (MIT)
//
// Copyright (c) 2015 Richard A Burton (richardaburton@gmail.com)
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//////////////////////////////////////////////////

#include "mutex.h"

// setup a new mutex
void ICACHE_FLASH_ATTR CreateMutex(mutex_t *mutex) {
	*mutex = 1;
}

// try to get a mutex
// returns true if successful, false if mutex not free
// as the esp8266 doesn't support the atomic S32C1I instruction
// we have to make the code uninterruptable to produce the
// same overall effect
bool ICACHE_FLASH_ATTR GetMutex(mutex_t *mutex) {

	int iOld = 1, iNew = 0;

	asm volatile (
		"rsil a15, 1\n"    // read and set interrupt level to 1
		"l32i %0, %1, 0\n" // load value of mutex
		"bne %0, %2, 1f\n" // compare with iOld, branch if not equal
		"s32i %3, %1, 0\n" // store iNew in mutex
		"1:\n"             // branch target
		"wsr.ps a15\n"     // restore program state
		"rsync\n"
		: "=&r" (iOld)
		: "r" (mutex), "r" (iOld), "r" (iNew)
		: "a15", "memory"
	);

	return (bool)iOld;
}

// release a mutex
void ICACHE_FLASH_ATTR ReleaseMutex(mutex_t *mutex) {
	*mutex = 1;
}
