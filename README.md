# 🎱 Magic Hate Ball - Animated Sarcastic 8-Ball v2.0

## 🎭 The Ultimate Brutally Honest Oracle

Transform your M5StickC Plus2 into an animated sarcastic oracle that tells you what you're **really** thinking! Now featuring **3 animated face expressions** that react to your stupid questions.

---

## ✨ What's New in v2.0

### 🎭 **Animated Face Reactions**
- **😒 Disappointed Face** (White) - Half-closed eyes, disapproving frown
- **😡 Angry Face** (Red) - Narrowed eyes with angry eyebrows  
- **😕 Confused Face** (Yellow) - One raised eyebrow, wavy mouth

### 🌟 **Enhanced User Experience**
- ✅ Improved wake-from-sleep (shows animated faces instead of blank screen!)
- ✅ Face cycles at startup (3 full cycles = 7.2 seconds)
- ✅ Face reactions after each answer (2 full cycles = 4.8 seconds)
- ✅ Ends on disappointed face, judging you silently
- ✅ Clock and battery stay visible during all animations

---

## 🎱 Features

### 💬 **138 Savage Responses** Across 9 Categories:
1. **Intelligence Insults** - "Peak stupidity", "Brain cells filing for workers' comp"
2. **Dismissive** - "Not my circus, not my monkeys"
3. **Sarcastic Advice** - "Step 1: Stop asking dumb stuff"
4. **Savage One-liners** - "You're the human version of a 404 error"
5. **Extra Spicy** - "Bless your heart, but no"
6. **Brutal Honesty** - "Even the real 8-ball would laugh at you"
7. **AI Burnout** - "Error 404: Patience not found"
8. **Sarcastic Yes** - "Yes, finally a bad idea I can support"
9. **Sarcastic No** - "No, unless you enjoy regret as a hobby"

### 🎮 **Controls**
- **Shake Device** - Get sarcastic answer with animated face reaction
- **Button B** - Cycle brightness (100% → 75% → 50% → 25% → 1%)
- **Button A (Long Press 2s)** - Time adjustment mode
- **Power Button** - Wake from sleep (now with animated faces!)

### 📊 **Technical Specs**
- **Memory**: RAM 8.6%, Flash 35.2%
- **Responses**: 138 unique sarcastic answers
- **Animations**: 3 face expressions cycling at 800ms each
- **Display**: Clock and battery always visible
- **Sleep**: 60 seconds inactivity → deep sleep with button wake

---

## 📦 Installation

### For M5Burner:
1. Download **MagicHateBot2.bin** (515KB merged binary)
2. Open M5Burner
3. Select M5StickC Plus2 device
4. Flash MagicHateBot2.bin at address 0x0
5. Power on and watch the animated faces!

### Files Included:
- `MagicHateBot2.bin` - Complete merged firmware (bootloader + partitions + app)
- `m5burner_config.json` - Full metadata and feature documentation
- `src/main.cpp` - Source code (PlatformIO project)
- `platformio.ini` - Build configuration

---

## 🎯 Usage

1. **Power On** → Watch animated faces cycle 3 times
2. **Ask a Question** → Any stupid question works best
3. **Shake Vigorously** → IMU detects shake
4. **"Thinking..."** → Yellow animation (1.5 seconds)
5. **Read Answer** → Sarcastic response with smooth fade-in
6. **Watch Faces** → Disappointed → Angry → Confused (cycles 2 times)
7. **Idle State** → Disappointed face judges you silently
8. **Repeat** → Wait 3 seconds cooldown, shake again!

---

## 🎨 Animation Details

### Startup Sequence:
```
Power On → 😒😡😕 → 😒😡😕 → 😒😡😕 → "Ask a stupid question and SHAKE ME!"
(3 cycles × 2.4 seconds = 7.2 seconds total)
```

### Answer Sequence:
```
Shake → "Thinking..." → Sarcastic Answer (2 sec) → 😒😡😕 → 😒😡😕 → 😒 (idle)
(2 cycles × 2.4 seconds = 4.8 seconds)
```

### Wake from Sleep:
```
Power Button → 😒😡😕 → 😒😡😕 → 😒 (idle)
(2 cycles instead of blank screen!)
```

---

## 💡 Tips

- 🎭 **Watch the face reactions** - they add personality to the sarcasm!
- 🤔 **Stupider questions = better responses**
- 💪 **Shake vigorously** for reliable IMU detection
- 🌙 **Use 1% brightness** for stealth sarcasm
- 🎯 **138 responses** means very low repetition
- 💼 **Perfect for passive-aggressive office humor**
- 🕐 **Clock and battery** visible during all animations

---

## 📝 Version History

### v2.0.0 (2024-11-14)
- ✨ Added 3 animated face expressions
- 🎨 Improved wake-from-sleep with face animations
- 🔧 Enhanced visual feedback throughout
- 📊 Clock and battery overlay always visible

### v1.0.0
- 🎱 Initial release with 138 savage responses
- 😒 Static disappointed face
- ⚡ Shake detection and fade-in animations

---

## 🙏 Credits

**Developer**: coreymillia + GitHub Copilot CLI  
**Inspired By**: Simple-Magic-8-Ball GitHub project (2 weeks old!)  
**Face Animation Concept**: Social Parasite project reactive face  
**Libraries**: M5StickCPlus2 SDK, M5Unified Framework  

---

## ⚖️ Legal

**Content Rating**: PG-13 - Sarcastic humor, no profanity  
**Disclaimer**: For entertainment purposes only. Not actual life advice.  
**Style**: Sarcastic and brutally honest, but not hateful or offensive.

---

## 🎉 Perfect For

- 🏢 **Office Desks** - Passive-aggressive humor
- 🎉 **Parties** - Conversation starter (or ender)
- 🎁 **Gifts** - For friends with a sense of humor
- 🤖 **Makers** - Learning M5Stack animations
- 😤 **Therapy** - When you can't say what you really think

---

**Made with 💀 sarcasm and ☕ caffeine**

*"Ask a stupid question, get an animated judgmental face!" 🎱🎭*
