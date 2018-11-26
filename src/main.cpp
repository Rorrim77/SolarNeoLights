#include <Arduino.h>
#include <FastLED.h>
#include <IRremote.h>

#ifdef __AVR__
#include <avr/power.h>
#endif

enum modes
{
	normal,
	split1,
	split2,
	rainbowGlitter,
	rainbow
};
modes current_mode = normal;

#define TERMINAL_DEBUG false
#define SERIAL_SPEED 9600 //115200

// START NEOPIXEL DEFINITION REGION
#define NEOPIXEL_PIN 6
#define NUMBER_OF_LEDS 30
#define FRAMES_PER_SECOND 120
CRGB leds[NUMBER_OF_LEDS];
uint8_t default_Wait = 0;
uint8_t brightness = 100;
CRGB current_color = CRGB::White;

CRGB neo_color1 = CRGB::OrangeRed;
CRGB neo_color2 = CRGB::MediumSpringGreen;
CRGB neo_color3 = CRGB::CadetBlue;
CRGB neo_color4 = CRGB::Coral;
CRGB neo_color5 = CRGB::DarkTurquoise;
CRGB neo_color6 = CRGB::DarkViolet;
CRGB neo_color7 = CRGB::Goldenrod;
CRGB neo_color8 = CRGB::LightSkyBlue;
CRGB neo_color9 = CRGB::MediumPurple;
CRGB neo_color10 = CRGB::Yellow;
CRGB neo_color11 = CRGB::SteelBlue;
CRGB neo_color12 = CRGB::Violet;

uint8_t gHue = 0;
// END NEOPIXEL DEEFINITION REGION

// START INFRARED REMOTE DEFINITION REGION
#define REMOTE_RECV_PIN 11
// define the remote control buttons
#define BUTTON_HOLD 0xFFFFFFFF
#define BRIGHTNESS_UP 0x61D608F7
#define BRIGHTNESS_DOWN 0x61D68877
#define OFF 0x61D648B7
#define ON 0x61D640BF
#define STROBE 0x61D6D02F
#define FLASH 0x61D6F00F
#define ALL_FADE 0x61D6807F
#define RGB_FLASH 0x61D6E817
#define RED 0x61D6C837
#define GREEN 0x61D6D827
#define BLUE 0x61D6E01F
#define WHITE 0x61D600FF
#define COLOR1 0x61D620DF
#define COLOR2 0x61D6A05F
#define COLOR3 0x61D6609F
#define COLOR4 0x61D618E7
#define COLOR5 0x61D658A7
#define COLOR6 0x61D69867
#define COLOR7 0x61D610EF
#define COLOR8 0x61D6906F
#define COLOR9 0x61D650AF
#define COLOR10 0x61D628D7
#define COLOR11 0x61D6A857
#define COLOR12 0x61D66897

IRrecv irrecv(REMOTE_RECV_PIN);
decode_results results;
unsigned long last_result = 0;
// END INFRARED REMOTE DEFINITION REGION

bool CheckIRRemote()
{
	if (irrecv.decode(&results))
	{
#if defined(TERMINAL_DEBUG)
		Serial.println(results.value, HEX);
#endif
		irrecv.resume(); // Receive the next value
		return true;
	}
	return false;
}

void Increase_Brightness()
{
	if (brightness < 245)
	{
		brightness = brightness + 10;
	}
	else
	{
		brightness = 255;
	}
	if (brightness > 255)
	{
		brightness = 255;
	}
#if defined(TERMINAL_DEBUG)
	Serial.println(brightness);
#endif
}

void Decrease_Brightness()
{
	if (brightness > 10)
	{
		brightness = brightness - 10;
	}
	else
	{
		brightness = 0;
	}
	if (brightness < 0)
	{
		brightness = 0;
	}
#if defined(TERMINAL_DEBUG)
	Serial.println(brightness);
#endif
}

void setRainbow()
{
	fill_rainbow(leds, NUMBER_OF_LEDS, gHue, 12);
}

void addGlitter(fract8 chanceOfGlitter)
{
	if (random8() < chanceOfGlitter)
	{
		leds[random16(NUMBER_OF_LEDS)] += CRGB::White;
	}
}

void rainbowWithGlitter()
{
	setRainbow();
	addGlitter(80);
}

void setSplit1()
{
	for (int i = 5; i < 10; i++)
	{
		leds[i] = CRGB::Black;
	}

	for (int i = 20; i < 25; i++)
	{
		leds[i] = CRGB::Black;
	}
}

void setSplit2()
{
	for (int i = 3; i < 6; i++)
	{
		leds[i] = CRGB::Black;
	}

	for (int i = 9; i < 12; i++)
	{
		leds[i] = CRGB::Black;
	}

	for (int i = 18; i < 21; i++)
	{
		leds[i] = CRGB::Black;
	}

	for (int i = 24; i < 27; i++)
	{
		leds[i] = CRGB::Black;
	}
}

// SETUP (runs once) ---------------------------------
void setup()
{

#if defined(TERMINAL_DEBUG)
	Serial.begin(SERIAL_SPEED); // start debug via serial
	Serial.println("Enabling IRin");
#endif

	irrecv.enableIRIn(); // Start the receiver

#if defined(TERMINAL_DEBUG)
	Serial.println("Enabled IRin");
#endif

	LEDS.addLeds<NEOPIXEL, NEOPIXEL_PIN>(leds, NUMBER_OF_LEDS); //start FastLED
}

// The program loop -----------------------------------
void loop()
{
	if (irrecv.isIdle() && CheckIRRemote())
	{
		uint32_t button_pressed = results.value;
		if (results.value != BUTTON_HOLD)
		{
			last_result = results.value;
			button_pressed = results.value;
		}
		else
		{
			button_pressed = last_result;
		}

		switch (button_pressed)
		{
		case RED:
			current_color = CRGB::Red;
			break;

		case BLUE:
			current_color = CRGB::Blue;
			break;

		case GREEN:
			current_color = CRGB::Green;
			break;

		case WHITE:
			current_color =  CRGB::White;
			break;

		case COLOR1:
			current_color =  neo_color1;
			break;

		case COLOR2:
			current_color = neo_color2;
			break;

		case COLOR3:
			current_color =  neo_color3;
			break;

		case COLOR4:
			current_color =  neo_color4;
			break;

		case COLOR5:
			current_color =  neo_color5;
			break;

		case COLOR6:
			current_color = neo_color6;
			break;

		case COLOR7:
			current_color =  neo_color7;
			break;

		case COLOR8:
			current_color =  neo_color8;
			break;

		case COLOR9:
			current_color =  neo_color9;
			break;

		case COLOR10:
			current_color =  neo_color10;
			break;

		case COLOR11:
			current_color =  neo_color11;
			break;

		case COLOR12:
			current_color =  neo_color12;
			break;

		case ON:
			brightness = 255;
			current_color =  CRGB::White;
			break;

		case OFF:
			brightness = 0;
			fadeToBlackBy(leds, NUMBER_OF_LEDS, 40);
			current_color =  CRGB::Black;
			break;

		case BRIGHTNESS_UP:
			Increase_Brightness();
			break;

		case BRIGHTNESS_DOWN:
			Decrease_Brightness();
			break;

		case STROBE:
			current_mode = modes::normal;
			break;

		case FLASH:
			current_mode = modes::split1;
			break;

			case ALL_FADE:
			current_mode = modes::split2;
			break;

		case RGB_FLASH:
			if (current_mode == modes::rainbowGlitter)
				current_mode = modes::rainbow;
			else
				current_mode = modes::rainbowGlitter;
			break;

		default:
			break;
		}
	}

	if (current_mode == modes::rainbowGlitter)
	{
		rainbowWithGlitter();
	}
	else if (current_mode == modes::rainbow)
	{
		fill_solid(leds, NUMBER_OF_LEDS, current_color);
		setRainbow();
	}
	else if (current_mode == modes::split1)
	{
		fill_solid(leds, NUMBER_OF_LEDS, current_color);
		setSplit1();
	}
	else if (current_mode == modes::split2)
	{
		fill_solid(leds, NUMBER_OF_LEDS, current_color);
		setSplit2();
	} else if (current_mode == modes::normal){
		fill_solid(leds, NUMBER_OF_LEDS, current_color);
	}

	EVERY_N_MILLISECONDS(20) { gHue++; } // slowly cycle the "base color" through the rainbow

	if (irrecv.isIdle())
	{
		LEDS.show(brightness);
	}
}