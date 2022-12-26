#pragma once

#include "./core.hpp"

namespace Mathyw {

// List of all key code available
enum KeyCode : int
{
	// ----------- The unknown key ------------ //
	KeyUnknown			= -1,
	// ----------- Printable keys ------------- //
	KeySpace			= 32,
	KeyApostrophe		= 39,
	KeyComma			= 44,
	KeyMinus			= 45,
	KeyPeriod			= 46,
	KeySlash			= 47,
	Key0				= 48,
	Key1				= 49,
	Key2				= 50,
	Key3				= 51,
	Key4				= 52,
	Key5				= 53,
	Key6				= 54,
	Key7				= 55,
	Key8				= 56,
	Key9				= 57,
	KeySemiColon		= 59,
	KeyEqual			= 61,
	KeyA				= 65,
	KeyB				= 66,
	KeyC				= 67,
	KeyD				= 68,
	KeyE				= 69,
	KeyF				= 70,
	KeyG				= 71,
	KeyH				= 72,
	KeyI				= 73,
	KeyJ				= 74,
	KeyK				= 75,
	KeyL				= 76,
	KeyM				= 77,
	KeyN				= 78,
	KeyO				= 79,
	KeyP				= 80,
	KeyQ				= 81,
	KeyR				= 82,
	KeyS				= 83,
	KeyT				= 84,
	KeyU				= 85,
	KeyV				= 86,
	KeyW				= 87,
	KeyX				= 88,
	KeyY				= 89,
	KeyZ				= 90,
	KeyLeftBracket		= 91,
	KeyBackSlash		= 92,
	KeyRightBracket		= 93,
	KeyGraveAccent		= 96,
	KeyWorld1			= 161,
	KeyWorld2			= 162,
	// ----------- Function keys -------------- //
	KeyEscape			= 256,
	KeyEnter			= 257,
	KeyTab				= 258,
	KeyBackspace		= 259,
	KeyInsert			= 260,
	KeyDelete			= 261,
	KeyRight			= 262,
	KeyLeft				= 263,
	KeyDown				= 264,
	KeyUp				= 265,
	KeyPageUp			= 266,
	KeyPageDown			= 267,
	KeyHome				= 268,
	KeyEnd				= 269,
	KeyCapsLock			= 280,
	KeyScrollLock		= 281,
	KeyNumLock			= 282,
	KeyPrintScreen		= 283,
	KeyPause			= 284,
	KeyF1				= 290,
	KeyF2				= 291,
	KeyF3				= 292,
	KeyF4				= 293,
	KeyF5				= 294,
	KeyF6				= 295,
	KeyF7				= 296,
	KeyF8				= 297,
	KeyF9				= 298,
	KeyF10				= 299,
	KeyF11				= 300,
	KeyF12				= 301,
	KeyF13				= 302,
	KeyF14				= 303,
	KeyF15				= 304,
	KeyF16				= 305,
	KeyF17				= 306,
	KeyF18				= 307,
	KeyF19				= 308,
	KeyF20				= 309,
	KeyF21				= 310,
	KeyF22				= 311,
	KeyF23				= 312,
	KeyF24				= 313,
	KeyF25				= 314,
	KeyKP0				= 320,
	KeyKP1				= 321,
	KeyKP2				= 322,
	KeyKP3				= 323,
	KeyKP4				= 324,
	KeyKP5				= 325,
	KeyKP6				= 326,
	KeyKP7				= 327,
	KeyKP8				= 328,
	KeyKP9				= 329,
	KeyKPDecimal		= 330,
	KeyKPDivide			= 331,
	KeyKPMultiply		= 332,
	KeyKPSubtract		= 333,
	KeyKPAdd			= 334,
	KeyKPEnter			= 335,
	KeyKPEqual			= 336,
	KeyLeftShift		= 340,
	KeyLeftControl		= 341,
	KeyLeftAlt			= 342,
	KeyLeftSuper		= 343,
	KeyRightShift		= 344,
	KeyRightControl		= 345,
	KeyRightAlt			= 346,
	KeyRightSuper		= 347,
	KeyMenu				= 348
};

// List of all mouse button code available
enum MouseButton : int
{
	MouseButton1			= 0,
	MouseButton2			= 1,
	MouseButton3			= 2,
	MouseButton4			= 3,
	MouseButton5			= 4,
	MouseButton6			= 5,
	MouseButton7			= 6,
	MouseButton8			= 7,
	MouseButtonLast			= MouseButton8,
	MouseButtonLeft			= MouseButton1,
	MouseButtonRight		= MouseButton2,
	MouseButtonMiddle		= MouseButton3
};

}