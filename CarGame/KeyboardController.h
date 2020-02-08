#pragma once
#include <windows.h>

namespace CarGame {
	class myKeyboard{

	public:
		enum KeyCode {
			A = 0x41,
			B = 0x42,
			C = 0x43,
			D = 0x44,
			E = 0x45,
			F = 0x46,
			G = 0x47,
			H = 0x48,
			I = 0x49,
			J = 0x4a,
			K = 0x4b,
			L = 0x4c,
			M = 0x4d,
			N = 0x4e,
			O = 0x4f,
			P = 0x50,
			Q = 0x51,
			R = 0x52,
			S = 0x53,
			T = 0x54,
			U = 0x55,
			V = 0x56,
			W = 0x57,
			X = 0x58,
			Y = 0x59,
			Z = 0x5a,
			LeftShift = 0xa0,
			RightShift = 0xa1,
			LeftControl = 0xa2,
			RightControl = 0xa3,
			Space = 0x20,
		};

		struct KeyboardState
		{
			bool W, S, A, D, R, F, LShift, RShift, LeftArrow, RightArrow, UpArrow, DownArrow;
		};

		myKeyboard();

		KeyboardState state;

		void ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam);
		void ResetState();



	};
}