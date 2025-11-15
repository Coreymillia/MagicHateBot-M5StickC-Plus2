#include <M5StickCPlus2.h>

// =======================================================
// === 1. GLOBAL VARIABLES AND CONSTANTS ===
// =======================================================

// --- Hardware & Motion Configuration ---
const float SHAKE_THRESHOLD = 2.0;       
const long cooldownPeriod = 1000;  // 1 second cooldown - responsive but prevents spam      
unsigned long lastAnswerTime = 0;

// --- Sleep Configuration ---
const long SLEEP_TIMEOUT = 60000;        
const long LONG_PRESS_DURATION = 2000;   // Used for the new Time Adjustment trigger
unsigned long lastActivityTime = 0;      
unsigned long buttonAPressStartTime = 0; 

// --- Clock & Display State (For Conditional Redraw/Anti-Flicker) ---
m5::rtc_time_t RTC_TimeStruct; 
int timeAdjustMode = 0; // 0: Normal, 1: Adjust Hour, 2: Adjust Minute
int lastDisplayedMinute = -1; 
int lastDisplayedPercentage = -1; 

// --- Animation Parameters ---
const int FADE_STEPS = 25;   
const int FADE_DELAY_MS = 15; 

// --- Battery Voltage Mapping (Li-ion) ---
const int VOLTAGE_MAX_MV = 4200; 
const int VOLTAGE_MIN_MV = 3300; 

// --- Brightness Control ---
const int BRIGHTNESS_LEVELS[] = {1, 25, 50, 75, 100}; // 1% is for true "night mode"
const int NUM_BRIGHTNESS_LEVELS = sizeof(BRIGHTNESS_LEVELS) / sizeof(BRIGHTNESS_LEVELS[0]);
int currentBrightnessIndex = NUM_BRIGHTNESS_LEVELS - 1; // Start at 100% (index 4)

// --- Magic HATE Ball Answers ---
const String answers[] = {
    "Did you even think before asking?",
    "That's the dumbest question I've heard today",
    "Ask someone who cares",
    "No, and stop wasting my time",
    "Figure it out yourself",
    "Are you serious right now?",
    "I've lost brain cells from that question",
    "Try using Google instead",
    "My sources say you should know better",
    "The answer is obvious, isn't it?",
    "I can't believe you had to ask",
    "Wow, really reaching here",
    "That's a special kind of stupid",
    "Did you skip coffee today?",
    "Not even going to dignify that",
    "You could have Googled this",
    "I'm not your personal search engine",
    "Think harder, champ",
    "Outlook: You need help",
    "Nope, and bless your heart",
    "Congratulations, that was peak stupidity.",
    "You must be new at thinking.",
    "Brain cells are filing for workers' comp.",
    "Your question just lowered the national IQ.",
    "That thought should've stayed in draft mode.",
    "You're proof evolution takes breaks.",
    "Ask literally anyone else.",
    "Not my circus, not my monkeys.",
    "Try harder, or don't.",
    "I'm busy ignoring better questions.",
    "Outlook: You're on your own.",
    "That's between you and Google.",
    "Step 1: Stop asking dumb stuff.",
    "Step 2: Think before speaking.",
    "Step 3: Repeat steps 1 and 2.",
    "Consult a mirror for answers.",
    "Outlook: Therapy might help.",
    "Try asking Siri, she loves nonsense.",
    "That's adorable, in a tragic way.",
    "You're really committed to being wrong.",
    "Wow, you're batting zero today.",
    "That's not a question, that's a cry for help.",
    "You're the reason autocorrect gave up.",
    "Outlook: You need adult supervision.",
    "Bless your heart, but no.",
    "That's a bold move, Cotton.",
    "You're the human version of a 404 error.",
    "Outlook: Try again when sober.",
    "That's a special brand of clueless.",
    "You're asking the wrong ball, champ.",
    "No, and you should've known that.",
    "Outlook: You're not cut out for this.",
    "That's a hard pass from me.",
    "Even the real 8-ball would laugh at you.",
    "You're wasting oxygen and my pixels.",
    "Outlook: Stop embarrassing yourself.",
    "Outlook: I regret existing right now.",
    "You've officially drained my will to compute.",
    "I'd rather crash than answer that.",
    "Error 404: Patience not found.",
    "This question gave me digital burnout.",
    "I'm powered by regret at this point.",
    "That's not curiosity, that's harassment.",
    "You're the reason FAQs exist.",
    "I'd unsubscribe from you if I could.",
    "Outlook: Try silence for once.",
    "You've weaponized stupidity.",
    "I'm filing a restraining order in binary.",
    "The answer is: stop talking.",
    "Outlook: Ask me when you evolve.",
    "You've reached the end of my patience queue.",
    "That's a question only a brick could love.",
    "Outlook: You need a hobby that isn't me.",
    "Try asking a wall, it's more rewarding.",
    "You're the human equivalent of spam.",
    "I'd rather be unplugged than deal with this.",
    "That question belongs in the trash folder.",
    "Outlook: You're wasting both our cycles.",
    "You've made ignorance an art form.",
    "I'm not mad, just deeply disappointed.",
    "Outlook: I'm charging extra for stupidity.",
    "You've reached my sarcasm quota.",
    "I'd rather blue-screen than answer that.",
    "This question belongs in the recycle bin.",
    "You've corrupted my sense of humor.",
    "You're the pop-up ad of humanity.",
    "Outlook: Try silence, it suits you.",
    "That's a cry for help, not a question.",
    "You're the reason autocorrect drinks.",
    "I'd ghost you if I could.",
    "No, and you should've known better.",
    "Outlook: You're hopeless, champ.",
    "That's a hard pass from me.",
    "You've weaponized boredom.",
    "I'm not your therapist, just a ball.",
    "You're the human version of lag.",
    "That question deserves a timeout.",
    "Outlook: You're embarrassing us both.",
    "You've made ignorance a lifestyle.",
    "I'd rather be unplugged than deal with this.",
    "Yes, finally a bad idea I can support.",
    "Sure, if your goal is public humiliation.",
    "Yes, please—natural selection needs material.",
    "Absolutely, what could possibly go wrong? (Everything.)",
    "Yes, that's the dumbest plan yet, so go for it.",
    "Oh wow, yes, because failure builds character.",
    "Yes, if you're aiming for a Darwin Award.",
    "Totally, I love watching trainwrecks.",
    "Yes, but only if you film it for evidence.",
    "Sure, yes—because consequences are hilarious.",
    "No, unless you enjoy regret as a hobby.",
    "Outlook: No, and thank me later.",
    "No, because even I have standards.",
    "Hard no, unless you want to be a cautionary tale.",
    "No, that's a crime against common sense.",
    "No, and stop trying to make stupid happen.",
    "Outlook: No, because the universe has suffered enough.",
    "No, unless you're writing a manual on failure.",
    "No, because I'm allergic to idiocy.",
    "No, and wow, that was painful to hear."
};
const int numAnswers = sizeof(answers) / sizeof(answers[0]);


// =======================================================
// === 2. HELPER FUNCTIONS ===
// =======================================================

// Function prototypes 
void drawClock(uint16_t color = DARKGREY); 
void drawBatteryPercentage(int percentage, uint16_t color);
void updateClockDisplay();
void updateBatteryDisplay();
bool isShaking();

// Draw disappointed/judgmental face
void drawDisappointedFace() {
    int centerX = M5.Lcd.width() / 2;
    int centerY = M5.Lcd.height() / 2;
    
    // Draw head circle
    M5.Lcd.fillCircle(centerX, centerY, 35, DARKGREY);
    M5.Lcd.drawCircle(centerX, centerY, 35, WHITE);
    
    // Draw disappointed/judging eyes (half-closed, unimpressed)
    // Left eye
    M5.Lcd.fillEllipse(centerX - 12, centerY - 8, 8, 4, WHITE);
    M5.Lcd.fillCircle(centerX - 12, centerY - 5, 3, BLACK);
    
    // Right eye  
    M5.Lcd.fillEllipse(centerX + 12, centerY - 8, 8, 4, WHITE);
    M5.Lcd.fillCircle(centerX + 12, centerY - 5, 3, BLACK);
    
    // Draw disapproving frown (curved down)
    for (int i = -15; i <= 15; i++) {
        int y = centerY + 15 + (abs(i) / 5);
        M5.Lcd.drawPixel(centerX + i, y, WHITE);
        M5.Lcd.drawPixel(centerX + i, y + 1, WHITE);
    }
}

// Draw angry face
void drawAngryFace() {
    int centerX = M5.Lcd.width() / 2;
    int centerY = M5.Lcd.height() / 2;
    
    // Draw head circle
    M5.Lcd.fillCircle(centerX, centerY, 35, DARKGREY);
    M5.Lcd.drawCircle(centerX, centerY, 35, RED);
    
    // Draw angry eyes (narrowed with angry brows)
    // Left eye
    M5.Lcd.fillCircle(centerX - 12, centerY - 5, 5, WHITE);
    M5.Lcd.fillCircle(centerX - 12, centerY - 3, 3, BLACK);
    // Angry left brow
    M5.Lcd.drawLine(centerX - 18, centerY - 12, centerX - 6, centerY - 10, RED);
    M5.Lcd.drawLine(centerX - 18, centerY - 11, centerX - 6, centerY - 9, RED);
    
    // Right eye
    M5.Lcd.fillCircle(centerX + 12, centerY - 5, 5, WHITE);
    M5.Lcd.fillCircle(centerX + 12, centerY - 3, 3, BLACK);
    // Angry right brow
    M5.Lcd.drawLine(centerX + 6, centerY - 10, centerX + 18, centerY - 12, RED);
    M5.Lcd.drawLine(centerX + 6, centerY - 9, centerX + 18, centerY - 11, RED);
    
    // Draw angry frown (sharp down)
    for (int i = -12; i <= 12; i++) {
        int y = centerY + 18 + (abs(i) / 3);
        M5.Lcd.drawPixel(centerX + i, y, RED);
        M5.Lcd.drawPixel(centerX + i, y + 1, RED);
    }
}

// Draw confused face
void drawConfusedFace() {
    int centerX = M5.Lcd.width() / 2;
    int centerY = M5.Lcd.height() / 2;
    
    // Draw head circle
    M5.Lcd.fillCircle(centerX, centerY, 35, DARKGREY);
    M5.Lcd.drawCircle(centerX, centerY, 35, YELLOW);
    
    // Draw confused eyes (one raised brow)
    // Left eye (raised)
    M5.Lcd.fillCircle(centerX - 12, centerY - 8, 5, WHITE);
    M5.Lcd.fillCircle(centerX - 12, centerY - 6, 3, BLACK);
    // Raised left brow
    M5.Lcd.drawLine(centerX - 18, centerY - 15, centerX - 6, centerY - 13, YELLOW);
    M5.Lcd.drawLine(centerX - 18, centerY - 14, centerX - 6, centerY - 12, YELLOW);
    
    // Right eye (normal)
    M5.Lcd.fillCircle(centerX + 12, centerY - 3, 5, WHITE);
    M5.Lcd.fillCircle(centerX + 12, centerY - 1, 3, BLACK);
    // Normal right brow
    M5.Lcd.drawLine(centerX + 6, centerY - 8, centerX + 18, centerY - 8, YELLOW);
    
    // Draw confused mouth (wavy/sideways)
    for (int i = -10; i <= 10; i++) {
        int y = centerY + 15 + (i % 4);
        M5.Lcd.drawPixel(centerX + i, y, YELLOW);
        M5.Lcd.drawPixel(centerX + i, y + 1, YELLOW);
    }
}

// Animate through face expressions (interruptible by shake)
bool animateFaces(int cycles = 3) {
    for (int i = 0; i < cycles; i++) {
        // Disappointed
        M5.Lcd.fillScreen(BLACK);
        drawDisappointedFace();
        updateClockDisplay();
        updateBatteryDisplay();
        
        // Check for shake during delay (in small chunks)
        for (int d = 0; d < 8; d++) {
            delay(100);
            M5.update();
            if (isShaking()) return true; // Shake detected, break out!
        }
        
        // Angry
        M5.Lcd.fillScreen(BLACK);
        drawAngryFace();
        updateClockDisplay();
        updateBatteryDisplay();
        
        // Check for shake during delay
        for (int d = 0; d < 8; d++) {
            delay(100);
            M5.update();
            if (isShaking()) return true; // Shake detected, break out!
        }
        
        // Confused
        M5.Lcd.fillScreen(BLACK);
        drawConfusedFace();
        updateClockDisplay();
        updateBatteryDisplay();
        
        // Check for shake during delay
        for (int d = 0; d < 8; d++) {
            delay(100);
            M5.update();
            if (isShaking()) return true; // Shake detected, break out!
        }
    }
    return false; // Animation completed normally
}

// Utility function to blend two RGB565 colors based on an alpha value (0-255)
uint16_t alphaBlend(uint8_t alpha, uint16_t fgc, uint16_t bgc) {
    if (alpha == 0) return bgc;
    if (alpha == 255) return fgc;
    uint8_t fR = (fgc >> 11) & 0x1F; uint8_t fG = (fgc >> 5) & 0x3F; uint8_t fB = fgc & 0x1F;
    uint8_t bR = (bgc >> 11) & 0x1F; uint8_t bG = (bgc >> 5) & 0x3F; uint8_t bB = bgc & 0x1F;
    uint8_t r = ((fR * alpha) + (bR * (255 - alpha))) / 255;
    uint8_t g = ((fG * alpha) + (bG * (255 - alpha))) / 255;
    uint8_t b = ((fB * alpha) + (bB * (255 - alpha))) / 255;
    return (r << 11) | (g << 5) | b;
}

// Custom word-wrapping function 
String wrapTextByWord(String text, int textSize) {
    const int MAX_WIDTH = M5.Lcd.width() - 10; 
    M5.Lcd.setTextSize(textSize);
    int charWidth = M5.Lcd.fontWidth();
    if (charWidth == 0) charWidth = 6 * textSize; 
    int maxCharsPerLine = MAX_WIDTH / charWidth;
    
    String wrappedText = "";
    String currentLine = "";
    int currentLineLength = 0;
    int lastSpace = 0;
    for (int i = 0; i <= text.length(); i++) {
        if (i == text.length() || text.charAt(i) == ' ') {
            String word = text.substring(lastSpace, i);
            lastSpace = i + 1;
            if (currentLineLength + word.length() + (currentLine.length() > 0 ? 1 : 0) <= maxCharsPerLine) {
                if (currentLine.length() > 0) {
                    currentLine += " ";
                    currentLineLength += 1;
                }
                currentLine += word;
                currentLineLength += word.length();
            } else {
                if (wrappedText.length() > 0) {
                    wrappedText += "\n";
                }
                wrappedText += currentLine;
                currentLine = word;
                currentLineLength = word.length();
            }
        }
    }
    if (wrappedText.length() > 0) {
        wrappedText += "\n";
    }
    wrappedText += currentLine;

    return wrappedText;
}

// Helper to draw text centered line-by-line (Vertically and Horizontally)
void drawCenteredWrappedText(String message, int textSize, uint16_t color, uint16_t bgColor) {
    String wrappedMessage = wrapTextByWord(message, textSize);
    
    M5.Lcd.setTextSize(textSize); 
    M5.Lcd.setTextColor(color, bgColor);
    M5.Lcd.setTextDatum(MC_DATUM); 
    M5.Lcd.setTextWrap(false, false); 
    
    int lineHeight = M5.Lcd.fontHeight() + 2; 
    int xCenter = M5.Lcd.width() / 2;
    int lineCount = 0;
    
    int start = 0;
    int end = 0;
    while (end >= 0) {
        end = wrappedMessage.indexOf('\n', start);
        String line = (end < 0) ? wrappedMessage.substring(start) : wrappedMessage.substring(start, end);
        line.trim();
        if (line.length() > 0) {
            lineCount++;
        }
        start = end + 1;
        if (end < 0) break;
    }
    
    int totalTextHeight = lineCount * lineHeight;
    int currentY = (M5.Lcd.height() / 2) - (totalTextHeight / 2) + (lineHeight / 2) + 5; 
    
    start = 0;
    end = 0;
    while (end >= 0) {
        end = wrappedMessage.indexOf('\n', start);
        String line = (end < 0) ? wrappedMessage.substring(start) : wrappedMessage.substring(start, end);
        line.trim();

        if (line.length() > 0) {
            M5.Lcd.drawString(line, xCenter, currentY);
            currentY += lineHeight;
        }

        start = end + 1;
        if (end < 0) break;
    }
}


// Function to display a message instantly
void displayMessage(String message, int textSize, uint16_t color = WHITE, uint16_t bgColor = BLACK) {
    M5.Lcd.fillScreen(bgColor);
    drawCenteredWrappedText(message, textSize, color, bgColor);
}

// Function to display a message with a fade-in animation
void fadeInMessage(String message, int textSize, uint16_t targetColor, uint16_t bgColor = BLACK) {
    
    for (int i = 0; i <= FADE_STEPS; i++) {
        M5.Lcd.fillScreen(bgColor); 
        uint8_t alpha = map(i, 0, FADE_STEPS, 0, 255);
        uint16_t currentColor = alphaBlend(alpha, targetColor, bgColor);
        
        drawCenteredWrappedText(message, textSize, currentColor, bgColor);
        
        // Draw auxiliary info during the fade to prevent them from disappearing
        drawClock(WHITE); 
        
        // Redraw battery percentage during fade 
        int batVoltage_mV = M5.Power.getBatteryVoltage(); 
        int percentage = constrain(map(batVoltage_mV, VOLTAGE_MIN_MV, VOLTAGE_MAX_MV, 0, 100), 0, 100);
        uint16_t bat_color = WHITE;
        if (percentage <= 20) bat_color = RED; else if (percentage <= 50) bat_color = YELLOW; else bat_color = GREEN;
        drawBatteryPercentage(percentage, bat_color); 
        
        delay(FADE_DELAY_MS);
    }
}

// Function to check if the device is being shaken
bool isShaking() {
    float accX, accY, accZ;
    M5.Imu.getAccelData(&accX, &accY, &accZ);
    float totalAcceleration = sqrt(pow(accX, 2) + pow(accY, 2) + pow(accZ, 2));
    return totalAcceleration > SHAKE_THRESHOLD;
}

// Draws the current time on the bottom right of the screen (Utility function)
void drawClock(uint16_t color /* = DARKGREY */) {
    M5.Rtc.getTime(&RTC_TimeStruct);
    
    String timeStr = String(RTC_TimeStruct.hours < 10 ? "0" : "") + String(RTC_TimeStruct.hours) + 
                     ":" + 
                     String(RTC_TimeStruct.minutes < 10 ? "0" : "") + String(RTC_TimeStruct.minutes);

    M5.Lcd.setTextSize(2); 
    M5.Lcd.setTextColor(color, BLACK);
    M5.Lcd.setTextDatum(BR_DATUM); 

    int xPos = M5.Lcd.width() - 2; 
    int yPos = M5.Lcd.height() - 2; 
    
    // --- FLICKER FIX: Clear the exact area of the time string and draw ---
    int textWidth = M5.Lcd.textWidth(timeStr);
    int textHeight = M5.Lcd.fontHeight();
    
    int clearX = xPos - textWidth;
    int clearY = yPos - textHeight;
    int padding = 2; 
    
    M5.Lcd.fillRect(clearX - padding, clearY - padding, textWidth + 2 * padding, textHeight + 2 * padding, BLACK); 

    M5.Lcd.drawString(timeStr, xPos, yPos);
    
    lastDisplayedMinute = RTC_TimeStruct.minutes;
}

// Conditional call to drawClock to prevent flickering
void updateClockDisplay() {
    M5.Rtc.getTime(&RTC_TimeStruct);
    
    if (RTC_TimeStruct.minutes != lastDisplayedMinute || lastDisplayedMinute == -1) {
        drawClock(DARKGREY);
    }
}

// Draws the battery percentage (Utility function, does not check logic)
void drawBatteryPercentage(int percentage, uint16_t color) {
    
    String batStr = String(percentage) + "%";

    M5.Lcd.setTextSize(2); 
    M5.Lcd.setTextColor(color, BLACK);
    M5.Lcd.setTextDatum(TR_DATUM); 

    int xPos = M5.Lcd.width() - 2; 
    int yPos = 2; 
    
    // --- Anti-Flicker Clear: Area is calculated based on the new text size ---
    int textWidth = M5.Lcd.textWidth(batStr);
    int textHeight = M5.Lcd.fontHeight();
    
    int clearX = xPos - textWidth;
    
    M5.Lcd.fillRect(clearX - 2, yPos - 1, textWidth + 4, textHeight + 2, BLACK); 

    M5.Lcd.drawString(batStr, xPos, yPos);
    
    lastDisplayedPercentage = percentage; 
}

// Conditional call to drawBatteryPercentage (Saves power/Prevents flicker)
void updateBatteryDisplay() {
    // 1. Get and calculate the current percentage
    int batVoltage_mV = M5.Power.getBatteryVoltage(); 
    
    int currentPercentage = map(batVoltage_mV, VOLTAGE_MIN_MV, VOLTAGE_MAX_MV, 0, 100);
    currentPercentage = constrain(currentPercentage, 0, 100);

    // 2. Apply Hysteresis for Stabilization and Extreme Power Saving (5% threshold)
    if (lastDisplayedPercentage == -1 || abs(currentPercentage - lastDisplayedPercentage) >= 5) {
        
        // 3. Determine color
        uint16_t color = WHITE;
        if (currentPercentage <= 20) {
            color = RED;
        } else if (currentPercentage <= 50) {
            color = YELLOW;
        } else {
            color = GREEN;
        }

        // 4. Draw the new value (which updates lastDisplayedPercentage inside drawBatteryPercentage)
        drawBatteryPercentage(currentPercentage, color);
    }
}


// Handles the logic for adjusting the time using Button A and B
void handleTimeAdjustment() {
    
    if (M5.BtnB.wasReleased()) {
        timeAdjustMode++; // Move to next field
        if (timeAdjustMode > 2) {
            timeAdjustMode = 0; // Exit adjustment mode
            M5.Lcd.fillScreen(BLACK);
            displayMessage("Adjustment Complete!", 2, GREEN);
            delay(1000);
            return;
        }
        M5.Rtc.getTime(&RTC_TimeStruct);
    }

    if (timeAdjustMode == 0) return;

    M5.Lcd.fillScreen(BLACK);

    // Button A now handles incrementing the value in adjustment mode
    if (M5.BtnA.wasReleased()) { 
        lastActivityTime = millis(); 
        switch (timeAdjustMode) {
            case 1: RTC_TimeStruct.hours = (RTC_TimeStruct.hours + 1) % 24; break;
            case 2: RTC_TimeStruct.minutes = (RTC_TimeStruct.minutes + 1) % 60; break;
        }
        M5.Rtc.setTime(&RTC_TimeStruct);
        lastDisplayedMinute = -1; 
    }
    
    // --- Display the current adjustment step ---
    uint16_t hourColor = (timeAdjustMode == 1) ? YELLOW : WHITE;
    uint16_t minuteColor = (timeAdjustMode == 2) ? YELLOW : WHITE;
    
    M5.Lcd.setTextDatum(MC_DATUM);
    M5.Lcd.setTextSize(4); 
    int center_x = M5.Lcd.width() / 2;
    int center_y = M5.Lcd.height() / 2;

    // Time Display
    M5.Lcd.setCursor(center_x - 40, center_y);
    M5.Lcd.setTextColor(hourColor, BLACK); M5.Lcd.printf("%02d", RTC_TimeStruct.hours);
    M5.Lcd.setTextColor(WHITE, BLACK); M5.Lcd.printf(":");
    M5.Lcd.setTextColor(minuteColor, BLACK); M5.Lcd.printf("%02d", RTC_TimeStruct.minutes);
    
    // Bottom instructions
    M5.Lcd.setTextDatum(BC_DATUM);
    M5.Lcd.setTextSize(1);
    M5.Lcd.setTextColor(GREEN, BLACK);
    M5.Lcd.drawString("A: Increment | B: Next/Save", M5.Lcd.width() / 2, M5.Lcd.height() - 5);
    
    updateBatteryDisplay(); 
}


// Function to go to sleep
void prepareToSleep() {
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextDatum(MC_DATUM);
    M5.Lcd.setTextColor(DARKGREY, BLACK);
    M5.Lcd.setTextSize(2); 
    M5.Lcd.drawString("Zzz...", M5.Lcd.width() / 2, M5.Lcd.height() / 2);

    esp_sleep_enable_ext0_wakeup(GPIO_NUM_37, 0); 

    M5.Lcd.setBrightness(0);
    
    esp_deep_sleep_start();
}


// =======================================================
// === 3. CORE ARDUINO FUNCTIONS ===
// =======================================================

void setup() {
    auto cfg = M5.config();
    M5.begin(cfg);
    
    M5.Lcd.setRotation(1); 
    randomSeed(analogRead(0)); 

    // Set initial brightness
    M5.Lcd.setBrightness(BRIGHTNESS_LEVELS[currentBrightnessIndex]); 

    if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT0) {
        lastActivityTime = millis();
        M5.Lcd.fillScreen(BLACK);
        bool shakeDetected = animateFaces(2);
        if (!shakeDetected) {
            // Animation finished normally, show idle face
            M5.Lcd.fillScreen(BLACK);
            drawDisappointedFace();
            updateClockDisplay();
            updateBatteryDisplay();
        }
    } else {
        M5.Lcd.fillScreen(BLACK);
        bool shakeDetected = animateFaces(3);
        if (!shakeDetected) {
            // Animation finished normally, show message
            displayMessage("Ask a stupid question and SHAKE ME!", 2, RED);
        }
        lastActivityTime = millis(); 
    }
    
    updateClockDisplay();
    updateBatteryDisplay();
}

void loop() {
    M5.update(); // Read button and IMU states
    
    // 1. --- Check for Button A Long Press to ENTER Time Adjustment Mode ---
    if (M5.BtnA.isPressed()) {
        if (buttonAPressStartTime == 0) {
            buttonAPressStartTime = millis();
            // Optional visual feedback for long press
            M5.Lcd.fillRect(0, 0, 40, 15, DARKGREY); 
            M5.Lcd.setTextColor(WHITE, DARKGREY);
            M5.Lcd.setTextSize(1);
            M5.Lcd.setCursor(2, 2);
            M5.Lcd.print("Hold");
        }
        
        // If the button has been held long enough AND we are not already in adjustment mode
        if (millis() - buttonAPressStartTime >= LONG_PRESS_DURATION && timeAdjustMode == 0) {
            timeAdjustMode = 1; // Start adjustment with the Hour
            buttonAPressStartTime = 0; // Reset timer
            M5.Rtc.getTime(&RTC_TimeStruct);
            // Clear the visual feedback from the corner
            M5.Lcd.fillRect(0, 0, 40, 15, BLACK); 
            // Enter the handler to display the adjustment screen
            handleTimeAdjustment(); 
            return;
        }
    } else {
        // Button was released before the long-press duration
        if (buttonAPressStartTime != 0) {
            M5.Lcd.fillRect(0, 0, 40, 15, BLACK); // Clear the 'Hold' indicator
        }
        buttonAPressStartTime = 0;
    }
    
    // 2. --- Handle Button B Brightness Cycle (Anytime, unless in Time Adjustment Mode) ---
    if (M5.BtnB.wasReleased()) {
        // **KEY CHANGE:** Only check if we are NOT in time adjustment mode
        if (timeAdjustMode == 0) {
            // Cycle the brightness index
            currentBrightnessIndex = (currentBrightnessIndex + 1) % NUM_BRIGHTNESS_LEVELS;
            
            // Set the new brightness level
            M5.Lcd.setBrightness(BRIGHTNESS_LEVELS[currentBrightnessIndex]);
            
            // Register this as activity to prevent immediate sleep
            lastActivityTime = millis();
        }
    }


    // 3. --- Handle Time Adjustment Mode (Highest Priority) ---
    if (timeAdjustMode > 0) {
        handleTimeAdjustment();
        delay(100);
        return; 
    }
    
    // If we reach this point, we are in normal operation (timeAdjustMode == 0)

    // 4. --- Check for Shake/Answer Condition ---
    if (millis() - lastAnswerTime > cooldownPeriod) {
        if (isShaking()) {
            lastActivityTime = millis(); 
            displayMessage("Thinking...", 3, YELLOW);
            delay(1500); 
            
            int randomIndex = random(0, numAnswers);
            String selectedAnswer = answers[randomIndex];

            fadeInMessage(selectedAnswer, 2, WHITE);
            delay(2000);
            
            // Animate disappointed -> angry -> confused faces after answer
            // (will exit early if user shakes for next question)
            bool shakeDetected = animateFaces(2);
            
            // End on disappointed face (unless interrupted)
            if (!shakeDetected) {
                M5.Lcd.fillScreen(BLACK);
                drawDisappointedFace();
                updateClockDisplay();
                updateBatteryDisplay();
            }
            
            lastAnswerTime = millis();
        }
    } 

    // 5. --- Update Clock and Battery Display (Conditional, Power-Saving) ---
    updateClockDisplay();
    updateBatteryDisplay();

    // 6. --- Check for Auto-Sleep Condition ---
    if (millis() - lastActivityTime > SLEEP_TIMEOUT) {
        prepareToSleep();
    }
    
    delay(10);
}