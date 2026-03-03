# King Mixer - VST3 Plugin

A JUCE-based VST3 channel strip plugin that applies rule-based mixing presets based on genre and instrument selection. Insert it on individual tracks in your DAW, select the genre and instrument, and let King Mixer configure EQ, compression, saturation, stereo width, and reverb send to mixing best-practice starting points.

## Features

- **8 genres**: Rock, Pop, Hip-Hop, EDM, Jazz, R&B, Metal, Classical
- **8 instrument categories**: Vocals, Drums, Bass, Electric Guitar, Acoustic Guitar, Keys/Synths, Strings, Brass
- **6 DSP modules** in series:
  1. Input/Output Gain Staging
  2. 4-Band Parametric EQ (Low Shelf, Low-Mid Peak, High-Mid Peak, High Shelf)
  3. Compressor with Makeup Gain
  4. Saturation (tanh soft-clip waveshaper)
  5. Stereo Width (mid-side processing)
  6. Reverb Send (built-in reverb with send level control)
- **64 curated rule presets** (8 genres x 8 instruments) based on professional mixing conventions
- **Mix Amount** knob to blend between dry signal and processed signal
- **Bypass** toggle
- Full preset save/load via DAW session state
- Available as **VST3** and **Standalone** application

## Building

### Requirements

- CMake 3.22+
- C++17 compatible compiler (MSVC 2022, GCC 10+, Clang 12+)
- Git (for FetchContent to download JUCE)

### Steps

```bash
# Configure (downloads JUCE automatically via FetchContent)
cmake -B build -S .

# Build Release
cmake --build build --config Release
```

### Output

- **VST3**: `build/AssistedMixing_artefacts/Release/VST3/King Mixer.vst3/`
- **Standalone**: `build/AssistedMixing_artefacts/Release/Standalone/King Mixer.exe`

To install the VST3 plugin, copy the `.vst3` folder to your system's VST3 directory:
- **Windows**: `C:\Program Files\Common Files\VST3\`
- **macOS**: `~/Library/Audio/Plug-Ins/VST3/`
- **Linux**: `~/.vst3/`

## Usage

1. Insert "King Mixer" as a VST3 plugin on a track in your DAW
2. Select the **Genre** (e.g., Rock, Pop, EDM)
3. Select the **Instrument** (e.g., Vocals, Drums, Bass)
4. Click **Apply Rule** to load the recommended mix settings
5. Fine-tune any parameter manually using the knobs
6. Use the **Mix Amount** slider to blend between dry and processed signal
7. Toggle **Bypass** to compare with the original signal

## Project Structure

```
Source/
  PluginProcessor.h/.cpp    - Audio processor, DSP chain, APVTS parameters
  PluginEditor.h/.cpp       - UI with genre/instrument selectors and knob strips
  DSP/
    GainStage.h/.cpp        - Input/output gain
    ParametricEQ.h/.cpp     - 4-band parametric EQ
    Compressor.h/.cpp       - Dynamics compressor with makeup gain
    Saturation.h/.cpp       - Tanh waveshaper saturation
    StereoWidth.h/.cpp      - Mid-side stereo width control
    ReverbSend.h/.cpp       - Reverb with send level
  Rules/
    GenreInstrumentDefs.h   - Genre and Instrument enums
    MixRule.h               - MixRule struct with all DSP parameter targets
    MixRuleDatabase.h/.cpp  - Lookup table of 64 genre+instrument mix rules
```

## License

This project uses JUCE under the [JUCE Personal/Small Business License](https://juce.com/legal/juce-8-licence/).
