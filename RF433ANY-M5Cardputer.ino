#include "RF433any.h"
#include "M5Cardputer.h"  // Include the M5Stack library

#define PIN_RFINPUT  1
// Comment the below macro if you wish to output everything.
// As most codes are repeated, this'll likely result in the output of the
// same thing multiple times.
// #define OUTPUT_FIRST_DECODED_ONLY

char serial_printf_buffer[100];
void serial_printf(const char* msg, ...) __attribute__((format(printf, 1, 2)));

void setup() {
    M5.begin();  // Initialize the M5Stack display
    M5.Lcd.setRotation(1); // Rotate the display 90 degrees clockwise
    M5.Lcd.setTextSize(1);  // Set text size to 1

    pinMode(PIN_RFINPUT, INPUT);
    Serial.begin(115200);
    Serial.print(F("Waiting for signal\n"));
}

Track track(PIN_RFINPUT);

const char *encoding_names[] = {
    "RFMOD_TRIBIT",
    "RFMOD_TRIBIT_INVERTED",
    "RFMOD_MANCHESTER",
    "<unmanaged encoding>"
};

const char *id_letter_to_encoding_name(char c) {
    if (c == 'T')
        return encoding_names[0];
    else if (c == 'N')
        return encoding_names[1];
    else if (c == 'M')
        return encoding_names[2];

    return encoding_names[3];
}

void clearDisplay() {
    M5.Lcd.fillScreen(BLACK); // Clear the screen
}

void output_timings(Decoder *pdec, byte nb_bits) {
    TimingsExt tsext;
    if (!pdec)
        return;
    pdec->get_tsext(&tsext);

    const char *enc_name = id_letter_to_encoding_name(pdec->get_id_letter());

    clearDisplay(); // Clear the display for new output

    // Output timing information
    // Print to both serial and display
    Serial.println("\n-----CODE START-----");
    Serial.printf("// [WRITE THE DEVICE NAME HERE]\n"
            "rf.register_Receiver(\n");
    Serial.printf("\t%s, // mod\n", enc_name);
    Serial.printf("\t%u, // initseq\n", tsext.initseq);
    Serial.printf("\t%u, // lo_prefix\n", tsext.first_low);
    Serial.printf("\t%u, // hi_prefix\n", tsext.first_high);
    Serial.printf("\t%u, // first_lo_ign\n", tsext.first_low_ignored);
    Serial.printf("\t%u, // lo_short\n", tsext.low_short);
    Serial.printf("\t%u, // lo_long\n", tsext.low_long);
    Serial.printf("\t%u, // hi_short (0 => take lo_short)\n", tsext.high_short);
    Serial.printf("\t%u, // hi_long  (0 => take lo_long)\n", tsext.high_long);
    Serial.printf("\t%u, // lo_last\n", tsext.last_low);
    Serial.printf("\t%u, // sep\n", tsext.sep);
    Serial.printf("\t%u  // nb_bits\n", nb_bits);
    Serial.printf(");\n");
    Serial.println("-----CODE END-----\n");

    M5.Lcd.setCursor(0, 0); // Set cursor to top-left corner of display
    M5.Lcd.println("\n-----CODE START-----");
    M5.Lcd.printf("// [WRITE THE DEVICE NAME HERE]\n"
            "rf.register_Receiver(\n");
    M5.Lcd.printf("\t%s, // mod\n", enc_name);
    M5.Lcd.printf("\t%u, // initseq\n", tsext.initseq);
    M5.Lcd.printf("\t%u, // lo_prefix\n", tsext.first_low);
    M5.Lcd.printf("\t%u, // hi_prefix\n", tsext.first_high);
    M5.Lcd.printf("\t%u, // first_lo_ign\n", tsext.first_low_ignored);
    M5.Lcd.printf("\t%u, // lo_short\n", tsext.low_short);
    M5.Lcd.printf("\t%u, // lo_long\n", tsext.low_long);
    M5.Lcd.printf("\t%u, // hi_short (0 => take lo_short)\n", tsext.high_short);
    M5.Lcd.printf("\t%u, // hi_long  (0 => take lo_long)\n", tsext.high_long);
    M5.Lcd.printf("\t%u, // lo_last\n", tsext.last_low);
    M5.Lcd.printf("\t%u, // sep\n", tsext.sep);
    M5.Lcd.printf("\t%u  // nb_bits\n", nb_bits);
    M5.Lcd.printf(");\n");
    M5.Lcd.println("-----CODE END-----\n");
}

void loop() {
    track.treset();

    while (!track.do_events())
        delay(1);

    Decoder *pdec0 = track.get_data(RF433ANY_FD_ALL);
    Decoder *pdec = pdec0;
    while (pdec) {
        int nb_bits = pdec->get_nb_bits();
        BitVector *pdata = pdec->take_away_data();

        if (pdata) {
            M5.Lcd.print("Data: ");
            char *buf = pdata->to_str();
            if (buf) {
                M5.Lcd.print(buf);
                free(buf);
            }
            delete pdata;
        }
        M5.Lcd.println();
        output_timings(pdec, nb_bits);

        delay(1000);  // Delay for visualization

#ifdef OUTPUT_FIRST_DECODED_ONLY
        pdec = nullptr;
#else
        pdec = pdec->get_next();
#endif

    }
    delete pdec0;
}
